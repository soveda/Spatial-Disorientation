// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
#include "mixer_mu_controls.h"
#include "elevation_mailbox.h"
#include <cassert>
#include <cstdio>
using namespace spatial;
struct Rig {
    Modes modes;MixerMuControls mu;MuInput input;ElevationMailbox mailbox;Config cfg;
    int32_t main=2048,x=1000,y=2000;uint32_t heights=0;int changes=0;
    void Tick(bool consume=true){
        Config effective=cfg;
        mu.Apply(input,modes,main,x,y,effective,heights);
        int change=mu.ElevationChange();
        if(change>=0){++changes;mailbox.Publish(modes.SelectedSource(),change);}
        if(consume)heights=mailbox.Consume(heights);
    }
    void Fader(int raw){input.fader[7]=raw;Tick();}
    void Button(unsigned value){input.buttons=value;Tick();input.buttons=0;Tick();}
};
int main(){
    Rig r;r.modes.Controls(Mode::Mixer,2048,1000,2000,0,0,false,r.cfg,0);
    r.input.connected=1;r.input.session=1;r.input.fader[7]=4064;r.Tick();
    assert(!(r.mu.Feedback()&128));assert(r.heights==0); // Invalid snapshots cannot pick up.
    r.input.faders_ready=1;r.Fader(4064);assert(!(r.mu.Feedback()&128));
    r.Fader(2000);assert(r.mu.Feedback()&128);assert(r.heights==0);
    r.Fader(0);assert(r.heights==1);r.Tick();assert(r.mu.Feedback()&128); // Own acknowledgement retains pickup.
    r.Fader(4064);assert(r.heights==2);assert((r.mu.Feedback()>>16)==2);
    r.Button(2);assert(r.modes.SelectedSource()==1);assert(!(r.mu.Feedback()&128));assert(r.heights==2);
    r.Fader(4064);assert(r.heights==2); // Held fader cannot set unpicked B to Above.
    r.Fader(2000);assert(r.mu.Feedback()&128);r.Fader(0);assert(r.heights==6); // A Above, B Below.
    r.Button(1);assert(r.modes.SelectedSource()==0);assert(!(r.mu.Feedback()&128));
    r.Fader(0);assert(r.heights==6);r.Fader(3400);assert(r.mu.Feedback()&128);
    r.Fader(2000);assert(r.heights==4);r.Tick(); // A Level, B Below; observe acknowledgement.
    // An external owner change rearms pickup, never immediately writes back.
    r.heights=6;r.Tick();assert(!(r.mu.Feedback()&128));assert(r.heights==6);
    r.Fader(3400);assert(r.mu.Feedback()&128);
    r.input.connected=0;r.Tick();assert(!(r.mu.Feedback()&128));assert(r.heights==6);
    r.input.connected=1;++r.input.session;r.input.buttons=2;r.Fader(0);
    assert(r.modes.SelectedSource()==0);assert(!(r.mu.Feedback()&128));assert(r.heights==6);
    // Two pending source commands cannot overwrite one another.
    ElevationMailbox m;m.Publish(0,1);m.Publish(1,2);assert(m.Consume(0)==9);
    assert(m.Consume(9)==9);m.Publish(0,0);m.Publish(0,2);assert(m.Consume(9)==10);
    m.Publish(2,0);m.Publish(1,3);assert(m.Consume(10)==10);
    // Boundary noise retains the current profile until the hysteresis edge.
    ElevationPickup p;p.Start(0);assert(p.Update(2032,0)==-1&&p.Picked());
    for(int i=0;i<1000;++i)assert(p.Update(i&1?1320:1380,0)==-1);
    assert(p.Update(1280,0)==1);assert(p.Update(1400,1)==-1);assert(p.Update(1430,1)==0);
    assert(p.Update(2750,0)==-1);assert(p.Update(2800,0)==2);
    assert(p.Update(2700,2)==-1);assert(p.Update(2640,2)==0);
    // No events or drift while a picked fader remains stationary.
    r.input.buttons=0;r.Fader(3400);int n=r.changes;for(int i=0;i<10000;++i)r.Tick();assert(r.changes==n);
    std::puts("PASS: selected-source elevation, pickup/source/session rearm, independent A/B, acknowledgements/external changes, disconnect retention, hysteresis and bounded mailbox");
}
