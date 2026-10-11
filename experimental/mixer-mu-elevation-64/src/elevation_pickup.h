// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
#pragma once
#include "dsp/fixed.h"
namespace spatial {
class ElevationPickup {
public:
    void Rearm(){picked_=false;have_previous_=false;}
    void Start(uint32_t height){height_=applied_=height<3?height:0;Rearm();}
    // Return -1 unless a picked-up fader actually changes the selected height.
    int Update(int32_t raw,uint32_t applied){
        if(applied!=applied_){
            applied_=applied;
            // An acknowledgement of our own request keeps pickup. A change
            // from another owner rearms it against the newly applied profile.
            if(applied!=height_){height_=applied<3?applied:0;Rearm();}
        }
        raw=Clamp(raw,0,4064);int32_t value=raw+(raw>>7);
        int32_t centre=height_==1?682:height_==2?3412:2048;
        int32_t d=value-centre,old=previous_-centre;
        if((d>=-64&&d<=64)||(have_previous_&&((old<0&&d>=0)||(old>0&&d<=0))))picked_=true;
        previous_=value;have_previous_=true;
        if(!picked_)return -1;
        uint32_t next=height_;
        // Three existing profiles, with 64 Q12 units of hysteresis either side
        // of the nominal 1365/2730 boundaries to suppress fader noise.
        if(height_==0){if(value<1301)next=1;else if(value>=2794)next=2;}
        else if(height_==1){if(value>=2794)next=2;else if(value>=1429)next=0;}
        else {if(value<1301)next=1;else if(value<2666)next=0;}
        if(next==height_)return -1;
        height_=next;return static_cast<int>(next);
    }
    bool Picked()const{return picked_;}
    uint32_t Height()const{return height_;}
private:
    int32_t previous_=0;uint32_t height_=0,applied_=0;
    bool picked_=false,have_previous_=false;
};
}
