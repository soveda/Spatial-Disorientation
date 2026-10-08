// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "sine_table.h"
#include "hrtf.h"
#include "../config.h"
namespace spatial {
inline int32_t Clamp(int32_t x, int32_t lo, int32_t hi) { return x < lo ? lo : (x > hi ? hi : x); }
inline int32_t Sin(uint32_t phase) {
    uint32_t i = phase >> 24;
    int32_t f = (phase >> 16) & 255;
    return kSine[i] + ((kSine[(i+1)&255] - kSine[i]) * f >> 8);
}
inline int32_t Slew(int32_t from, int32_t to, int32_t divisor) {
    int32_t d = to - from;
    return from + (d == 0 ? 0 : d / divisor + (d > 0 ? 1 : -1));
}
struct Stereo { int32_t left = 0, right = 0; };
struct Scene {
    uint32_t angle[2] = {0, 0x80000000u}; // 0 front; quarter turn right.
    int32_t distance = 0;
};
class Delay {
public:
    void Write(int32_t x) { data_[head_] = static_cast<int16_t>(Clamp(x,-4096,4095)); head_=(head_+1)&2047; }
    int32_t Read(int32_t q8) const {
        int32_t a=(head_-1-(q8>>8))&2047, b=(a-1)&2047;
        return data_[a]+((data_[b]-data_[a])*(q8&255)>>8);
    }
private:
    int16_t data_[2048] = {};
    int32_t head_ = 0;
};
// A short stereo line supplies only ITD; the shared mono line supplies distance
// and room taps. 64 samples safely cover the <=32-sample interaural delay.
class EarDelay {
public:
    EarPair Process(EarPair sample,const int32_t* delay){
        int32_t in[2]={sample.left,sample.right},out[2];
        for(int e=0;e<2;++e){
            data_[e][head_]=static_cast<int16_t>(in[e]);
            int32_t a=(head_-(delay[e]>>8))&63,b=(a-1)&63;
            out[e]=data_[e][a]+((data_[e][b]-data_[e][a])*(delay[e]&255)>>8);
        }
        head_=(head_+1)&63;return {out[0],out[1]};
    }
private:
    int16_t data_[2][64]={};int32_t head_=0;
};
class Source {
public:
    void Geometry(uint32_t phase,int32_t distance,int32_t level,int32_t strength,int32_t room){
        hrtf_.Geometry(phase,strength);
        int32_t lateral=(static_cast<int32_t>(Sin(phase)>>3)*strength)>>12;
        int32_t front=(static_cast<int32_t>(Sin(phase+0x40000000u)>>3)*strength)>>12;
        base_target_=(64<<8)+distance*20;
        // A distant source loses direct energy faster than its room sound. The
        // first reflection dominates in front; later arrivals dominate behind.
        // This is an original small-room cue, not a measured room response.
        int32_t wet=(room*(1024+((distance*2662)>>12)))>>12;
        wet=(wet*((3072-((distance*1024)>>12))*level>>12))>>12;
        room_target_[0]=(wet*(4096-(lateral>>2)))>>12;
        room_target_[1]=(wet*(4096+(lateral>>2)))>>12;
        early_target_=2048+(front>>2);
        int32_t directGain=((3072-((distance*2304)>>12))*level)>>12;
        gain_target_=(directGain*(4096-((room*(256+(distance>>3)))>>12)))>>12;
        tone_target_=32700-((distance*6500)>>12);
        delay_target_[0]=lateral>0?lateral*2:0;
        delay_target_[1]=lateral<0?-lateral*2:0;
    }
    Stereo Process(int32_t input){
        int32_t q8=Clamp(input,-2048,2047)*256;
        dc_=q8-previous_+dc_-(dc_>>10);previous_=q8;
        line_.Write(dc_>>8);
        base_=Slew(base_,base_target_,256);
        room_[0]=Slew(room_[0],room_target_[0],512);
        room_[1]=Slew(room_[1],room_target_[1],512);
        early_=Slew(early_,early_target_,256);gain_=Slew(gain_,gain_target_,128);
        tone_=Slew(tone_,tone_target_,128);
        // Shared Q3 low-pass state replaces two Q8/int64 multiplies. With input
        // bounded to [-4096,4095], |difference|<=65528 and tone<=32700, so the
        // signed 32-bit product is <=2,142,765,600. State remains in input range.
        int32_t direct=line_.Read(base_);
        shadow_+=((direct*8-shadow_)*tone_)>>15;
        hrtf_.Update();
        EarPair filtered=hrtf_.Process(Clamp(shadow_>>3,-4096,4095));
        delay_[0]=Slew(delay_[0],delay_target_[0],256);
        delay_[1]=Slew(delay_[1],delay_target_[1],256);
        filtered=ears_.Process(filtered,delay_);
        int32_t reflected[2]={};
        // Unequal arrival times (7–24 ms after the direct sound) give the ears
        // room-width cues without adding latency to the direct path. Three
        // feed-forward taps per ear have no feedback or accumulating reverb tail.
        for(int32_t e=0;e<2;++e){
            int32_t taps=0;
            if(room_[e]!=0){
                int32_t first=line_.Read(base_+(e==0?337:401)*256);
                int32_t later=(line_.Read(base_+(e==0?631:727)*256)
                              +line_.Read(base_+(e==0?1069:1153)*256))>>1;
                taps=later+(((first-later)*early_)>>12);
            }
            // Q3 one-pole wall absorption: softer reflections preserve the
            // direct HRTF's fine spectral cues. Keep draining when room is zero.
            wall_[e]+=(taps*8-wall_[e])>>1;
            reflected[e]=((wall_[e]>>3)*room_[e])>>12;
        }
        return {((filtered.left*gain_)>>12)+reflected[0],((filtered.right*gain_)>>12)+reflected[1]};
    }
private:
    Delay line_;Hrtf hrtf_;EarDelay ears_;
    int32_t previous_=0,dc_=0,shadow_=0;
    int32_t base_=64<<8,base_target_=64<<8;
    int32_t room_[2]={},room_target_[2]={},wall_[2]={};
    int32_t early_=2048,early_target_=2048;
    int32_t delay_[2]={},delay_target_[2]={};
    int32_t gain_=0,gain_target_=0,tone_=24000,tone_target_=24000;
};
class Engine {
public:
    void SetScene(const Scene& scene,const Config& config) { scene_=scene;config_=config; }
    Stereo Process(int32_t a,int32_t b) {
        // Stagger the two geometry calculations to spread work within each DSP block.
        if ((count_&31)==0) source_[0].Geometry(scene_.angle[0],scene_.distance,config_.value[2],config_.value[4],config_.value[1]);
        if ((count_&31)==16) source_[1].Geometry(scene_.angle[1],scene_.distance,config_.value[3],config_.value[4],config_.value[1]);
        ++count_;
        Stereo x=source_[0].Process(a),y=source_[1].Process(b);
        return {Clamp(((x.left+y.left)*3)>>2,-2048,2047),Clamp(((x.right+y.right)*3)>>2,-2048,2047)};
    }
private:
    Scene scene_;
    Config config_;
    Source source_[2];
    uint32_t count_=0;
};
}
