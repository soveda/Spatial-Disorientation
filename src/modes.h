// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
#pragma once
#include "startup_mode.h"
#include "orbits.h"
namespace spatial {
class Modes {
public:
    // source: 0 selects A, 1 selects B, -1 retains the last selection.
    void Controls(Mode mode,int32_t main,int32_t x,int32_t y,int32_t cv1,int32_t cv2,bool linked,const Config& cfg,int source=-1,bool motion=false){
        mode_=mode;
        if(mode==Mode::Orbits){orbits_.Controls(main,x,y,cv1,cv2,linked,cfg);return;}
        if(mode!=Mode::Mixer)return; // Disorientation is an explicitly reserved slot.
        int32_t knobs[3]={Clamp(main,0,4095),Clamp(x,0,4095),Clamp(y,0,4095)};
        if(!initialized_){
            initialized_=true;selected_=source==1?1:0;picked_=7;
            for(int i=0;i<3;++i)value_[selected_][i]=knobs[i];
        }else if(source>=0&&source!=selected_){selected_=source;picked_=0;}
        // Same near/crossing pickup principle as WaveSeq and our 8mu controls.
        // Compare against the previous physical value, never an invented zero.
        for(int i=0;i<3;++i){
            int32_t diff=knobs[i]-value_[selected_][i],old=previous_[i]-value_[selected_][i];
            if((diff>=-64&&diff<=64)||(old<0&&diff>=0)||(old>0&&diff<=0)||(i==0&&motion))picked_|=1u<<i;
            if(picked_&(1u<<i))value_[selected_][i]=knobs[i];
            previous_[i]=knobs[i];
        }
        // CV modulation applies to both stored placements without overwriting
        // them. Returning CV to zero restores each source's panel setting.
        for(int e=0;e<2;++e){
            fixed_.angle[e]=static_cast<uint32_t>(value_[e][0]-2048+cv1*2)*1048576u;
            int32_t distance=Clamp(value_[e][1]+cv2*2,0,4095);
            if(e){fixed_.distance_b=distance;fixed_.level_b=value_[e][2];}
            else{fixed_.distance=distance;fixed_.level_a=value_[e][2];}
        }
    }
    void Pulse(bool connected,bool edge,bool reset){if(mode_==Mode::Orbits)orbits_.Pulse(connected,edge,reset);}
    Scene Advance(){return mode_==Mode::Orbits?orbits_.Advance():fixed_;}
    bool ClockLocked()const{return mode_==Mode::Orbits&&orbits_.ClockLocked();}
    int SelectedSource()const{return selected_;}
    uint32_t Pickup()const{return picked_;}
private:
    Orbits orbits_;Scene fixed_;
    Mode mode_=Mode::Orbits;
    // Unedited source defaults: A front, B back, near, full panel level.
    int32_t value_[2][3]={{2048,0,4095},{0,0,4095}},previous_[3]={};
    int selected_=0;uint32_t picked_=0;bool initialized_=false;
};
}
