// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
#pragma once
#include "hrtf_table.h"
namespace spatial {
// Fixed 32-tap convolution per ear. Update one coefficient per ear/sample so
// direction changes do not put a whole interpolation loop into geometry peaks.
class Hrtf {
public:
    Hrtf() { for(int e=0;e<2;++e) coeff_[e][0]=16384; }
    void Geometry(uint32_t phase,int32_t strength) {
        uint64_t position=static_cast<uint64_t>(phase)*kHrtfDirections;
        index_=position>>32;next_=(index_+1)%kHrtfDirections;
        fraction_=static_cast<uint32_t>(position)>>20;strength_=strength;
    }
    void Update() {
        for(int e=0;e<2;++e) {
            int32_t a=kHrtf[index_][e][update_],b=kHrtf[next_][e][update_];
            int32_t measured=a+(((b-a)*fraction_)>>12);
            int32_t neutral=update_==0?16384:0;
            int32_t target=neutral+(((measured-neutral)*strength_)>>12);
            int32_t delta=target-coeff_[e][update_];
            coeff_[e][update_]+=delta==0?0:delta/4+(delta>0?1:-1);
        }
        update_=(update_+1)&31;
    }
    int32_t Process(int ear,int32_t sample) {
        // Input is bounded by caller. Duplicated ring gives a contiguous dot
        // product with no modulo operations inside the convolution loop.
        int32_t head=(head_[ear]-1)&31;head_[ear]=head;
        history_[ear][head]=history_[ear][head+32]=static_cast<int16_t>(sample);
        const int16_t* data=history_[ear]+head;
        int32_t acc=0;
        for(int i=0;i<32;++i)acc+=data[i]*coeff_[ear][i];
        return acc>>14;
    }
private:
    static_assert(kHrtfMixtureL1Bound*4096<2147483647,"32-bit FIR accumulator bound");
    int16_t history_[2][64]={};
    int32_t coeff_[2][32]={},head_[2]={};
    int32_t index_=0,next_=1,fraction_=0,strength_=4095,update_=0;
};
}
