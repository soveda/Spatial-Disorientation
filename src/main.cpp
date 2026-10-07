// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
// ComputerCard hardware and core split follow Chris Johnson's examples.
#include "ComputerCard.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "pico/multicore.h"
#include "orbits.h"
#include "usb_editor.h"

static spatial::Shared shared;
static spatial::Storage storage;
static spatial::UsbEditor* editor;
static void UsbCore() { editor->Run(); }

class Card : public ComputerCard {
public:
    void Configure(const spatial::Config& cfg) { config_=cfg; }
    uint32_t Capacity() const { return FlashSizeBytes(); }
private:
    void ProcessSample() override {
        uint32_t start=time_us_32();
        bool saving=shared.save!=0;
        bool reset=false;
        if (++scan_==48) {
            scan_=0;
            shared.Consume(config_);
            Switch raw=SwitchVal();
            if (raw!=candidate_) { candidate_=raw;stable_count_=0; }
            else if (stable_count_<5 && ++stable_count_==5) {
                reset=raw==Switch::Down && settled_;
                selected_=raw;
                settled_=true;
            }
            if (selected_!=Switch::Down) linked_=selected_==Switch::Up;
            orbits_.Controls(KnobVal(Knob::Main),KnobVal(Knob::X),KnobVal(Knob::Y),
                Connected(Input::CV1)?CVIn1():0,Connected(Input::CV2)?CVIn2():0,linked_,config_);
            CVOut1(0);CVOut2(0);PulseOut1(false);PulseOut2(false);
        }
        orbits_.Pulse(Connected(Input::Pulse1),PulseIn1RisingEdge(),
            reset || (Connected(Input::Pulse2) && PulseIn2RisingEdge()));
        spatial::Scene scene=orbits_.Advance();
        engine_.SetScene(scene,config_);
        auto output=engine_.Process(Connected(Input::Audio1)?AudioIn1():0,
                                    Connected(Input::Audio2)?AudioIn2():0);
        if (startup_<4800) ++startup_;
        int32_t target=(!saving && startup_==4800)?1024:0;
        fade_=spatial::Slew(fade_,target,128);
        AudioOut1(output.left*fade_>>10);AudioOut2(output.right*fade_>>10);
        if (saving && fade_==0) {
            if (silent_<8) ++silent_;
            if (silent_==8 && shared.save==1) { __dmb();shared.save=2; }
        } else silent_=0;
        if (scan_==0) {
            int32_t a=spatial::Sin(scene.angle[0])>>3,b=spatial::Sin(scene.angle[1])>>3;
            LedBrightness(0,(4095-a)/2);LedBrightness(1,(4095+a)/2);
            LedBrightness(2,(4095-b)/2);LedBrightness(3,(4095+b)/2);
            LedOn(4,linked_);LedOn(5,overrun_);
            shared.angle_a=scene.angle[0]>>20;shared.angle_b=scene.angle[1]>>20;
            shared.distance=scene.distance;
            shared.flags=(linked_?1:0)|(orbits_.ClockLocked()?2:0)|(overrun_?4:0)|(saving?8:0);
        }
        // This measures callback duration, not the framework's surrounding ISR.
        if (!saving && !shared.save && time_us_32()-start>=18) overrun_=true;
    }
    spatial::Config config_;
    spatial::Engine engine_;
    spatial::Orbits orbits_;
    uint32_t startup_=0;
    int32_t fade_=0;
    unsigned scan_=0,stable_count_=0,silent_=0;
    Switch candidate_=Switch::Middle,selected_=Switch::Middle;
    bool linked_=false,settled_=false,overrun_=false;
};
int main() {
    vreg_set_voltage(VREG_VOLTAGE_1_15);
    sleep_ms(10);
    set_sys_clock_khz(192000,true);
    static Card card;
    storage.Init(card.Capacity());
    spatial::Config initial=storage.Load();
    card.Configure(initial);
    static spatial::UsbEditor usb(shared,storage,initial);
    editor=&usb;
    multicore_lockout_victim_init();
    multicore_launch_core1(UsbCore);
    card.EnableNormalisationProbe();
    card.Run();
}
