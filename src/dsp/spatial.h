// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "sine_table.h"
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
class Source {
public:
    void Geometry(uint32_t phase, int32_t distance, int32_t level, int32_t strength, int32_t room) {
        int32_t side = Sin(phase) >> 3; // Q12: positive is right.
        int32_t rear = (32767 - Sin(phase + 0x40000000u)) >> 4;
        int32_t lateral = (side * strength) >> 12;
        int32_t base_q8 = (64 << 8) + distance * 20; // ~1.3 to 8 ms.
        room_target_ = (room * (512 + ((distance * 1536) >> 12))) >> 12;
        base_target_ = base_q8;
        // Analytical rear spectral cue, not a measured pinna/HRTF response.
        // A 3-sample feed-forward pair has its first cancellation at 8 kHz.
        // Blend up to 75% towards that pair at the back, scaled by strength.
        rear_target_ = (((rear * strength) >> 12) * 3) >> 2;
        for (int32_t ear = 0; ear < 2; ++ear) {
            int32_t away = ear == 0 ? lateral : -lateral;
            // Far ear arrives up to 32 samples later (~0.67 ms).
            delay_target_[ear] = base_q8 + (away > 0 ? away * 2 : 0);
            int32_t direct = 3072 - ((distance * 2304) >> 12);
            int32_t pan = 4096 - ((away * 2500) >> 12);
            gain_target_[ear] = (((direct * pan) >> 12) * level) >> 12;
            tone_target_[ear] = Clamp(24000 - ((distance * 10000) >> 12)
                - ((((rear * strength) >> 12) * 12000) >> 12)
                - ((away > 0 ? away * 10000 : 0) >> 12), 1800, 28000);
            level_target_ = level;
        }
    }
    Stereo Process(int32_t input) {
        // DC removal with high precision keeps very quiet tails alive.
        int32_t q8 = Clamp(input,-2048,2047)*256;
        dc_ = q8 - previous_ + dc_ - (dc_ >> 10);
        previous_ = q8;
        int32_t sample = Clamp(dc_ >> 8,-4096,4095);
        rear_mix_ = Slew(rear_mix_,rear_target_,256);
        int32_t delayed = rear_history_[rear_index_];
        rear_history_[rear_index_] = sample;
        if (++rear_index_ == 3) rear_index_ = 0;
        // Convex feed-forward mix: unity DC gain, no feedback or boost.
        // Integer-sample history avoids changing the interaural delay geometry.
        line_.Write(sample + (((delayed-sample)*rear_mix_) >> 13));
        base_ = Slew(base_,base_target_,256);
        room_ = Slew(room_,room_target_,512);
        level_ = Slew(level_,level_target_,512);
        Stereo result;
        for (int32_t ear = 0; ear < 2; ++ear) {
            delay_[ear] = Slew(delay_[ear],delay_target_[ear],256);
            gain_[ear] = Slew(gain_[ear],gain_target_[ear],128);
            tone_[ear] = Slew(tone_[ear],tone_target_[ear],128);
            int32_t direct = line_.Read(delay_[ear]);
            // Q8 by Q15 needs a widened intermediate for full-scale transients.
            shadow_[ear] += static_cast<int32_t>((static_cast<int64_t>(direct*256-shadow_[ear])*tone_[ear])>>15);
            // Feed-forward early reflections: no feedback can build up.
            int32_t reflections = (line_.Read(base_+(ear==0 ? 211 : 293)*256)
                                 +line_.Read(base_+(ear==0 ? 367 : 433)*256)) >> 1;
            int32_t reflected = (((reflections * room_) >> 12)*level_) >> 12;
            int32_t out = (((shadow_[ear]>>8)*gain_[ear]) >> 12) + reflected;
            if (ear==0) result.left=out; else result.right=out;
        }
        return result;
    }
private:
    Delay line_;
    int32_t previous_=0,dc_=0;
    int32_t rear_history_[3]={},rear_index_=0,rear_mix_=0,rear_target_=0;
    int32_t base_=64<<8,base_target_=64<<8,room_=0,room_target_=0,level_=0,level_target_=0;
    int32_t delay_[2]={64<<8,64<<8},delay_target_[2]={64<<8,64<<8};
    int32_t gain_[2]={},gain_target_[2]={},shadow_[2]={};
    int32_t tone_[2]={24000,24000},tone_target_[2]={24000,24000};
};
class Engine {
public:
    void SetScene(const Scene& scene,const Config& config) { scene_=scene;config_=config; }
    Stereo Process(int32_t a,int32_t b) {
        // Stagger the two geometry calculations to bound individual ISR peaks.
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
