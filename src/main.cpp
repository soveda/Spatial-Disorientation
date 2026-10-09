// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
// ComputerCard hardware and core split follow Chris Johnson's examples.
#include "tusb_config.h" // Configure both roles before EightMU/TinyUSB headers.
#include "ComputerCard.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "pico/multicore.h"
#include "modes.h"
#include "block_audio.h"
#include "usb_editor.h"
#include "mu_host.h"

static spatial::Shared shared;
static spatial::Storage storage;
static spatial::BlockAudio blocks;
static spatial::Engine engine;
static void RenderBlock() {
    uint32_t start=time_us_32();
    bool saving=shared.save!=0;
    bool rendered=blocks.Render(engine);
    // Keep 10% block headroom. Flash lockout pauses both cores intentionally.
    if(rendered && !saving && !shared.save) {
        uint32_t elapsed=time_us_32()-start;
        if(elapsed>shared.block_peak_us)shared.block_peak_us=elapsed;
        if(elapsed>=1200)blocks.worker_fault|=2;
    }
}
// TinyUSB host enumeration includes debounce/reset delays of many milliseconds.
// Keep audio moving during those waits without recursively running USB tasks.
extern "C" uint32_t tusb_time_millis_api(void) {
    return to_ms_since_boot(get_absolute_time());
}
extern "C" void tusb_time_delay_ms_api(uint32_t ms) {
    uint32_t start=tusb_time_millis_api();
    while(tusb_time_millis_api()-start<ms){RenderBlock();tight_loop_contents();}
}
static spatial::UsbEditor* editor;
static bool host_mode=false;
static void UsbCore() {
    if(host_mode)spatial::RunMuHost(shared,RenderBlock);
    else editor->Run();
}

class Card : public ComputerCard {
public:
    void Configure(const spatial::Config& cfg) { config_=cfg;effective_=cfg; }
    uint32_t Capacity() const { return FlashSizeBytes(); }
    bool HostMode() { return USBPowerState()==DFP; }
private:
    void ProcessSample() override {
        uint32_t start=time_us_32();
        bool saving=shared.save!=0;
        bool reset=false;
        if (++scan_==48) {
            scan_=0;
            shared.Consume(config_);
            boot_.Tick(SwitchVal()==Switch::Down,KnobVal(Knob::Main));
            shared.mode=static_cast<uint32_t>(boot_.Selected());
            Switch raw=SwitchVal();
            if (raw!=candidate_) { candidate_=raw;stable_count_=0; }
            else if (stable_count_<5 && ++stable_count_==5) {
                reset=raw==Switch::Down && settled_;
                selected_=raw;
                settled_=true;
            }
            if (selected_!=Switch::Down) linked_=selected_==Switch::Up;
            int32_t main=KnobVal(Knob::Main),x=KnobVal(Knob::X),y=KnobVal(Knob::Y);
            if(shared.ConsumeMu(mu_input_))mu_age_=0;
            else if(mu_age_<150)++mu_age_;
            if(mu_age_==150)mu_input_.yaw=0; // No stale integrated motion during USB waits.
            effective_=config_;
            bool orbits=boot_.Selected()==spatial::Mode::Orbits;
            if(boot_.Ready())reset=mu_controls_.Apply(mu_input_,main,x,y,effective_,
                orbits&&reset,orbits)||reset;
            shared.mu_feedback=mu_controls_.Picked()|(mu_controls_.Motion()?256:0);
            if(boot_.Ready())modes_.Controls(boot_.Selected(),main,x,y,
                Connected(Input::CV1)?CVIn1():0,Connected(Input::CV2)?CVIn2():0,linked_,effective_,
                selected_==Switch::Up?0:(selected_==Switch::Down?1:-1),mu_controls_.Motion());
            CVOut1(0);CVOut2(0);PulseOut1(false);PulseOut2(false);
        }
        modes_.Pulse(Connected(Input::Pulse1),PulseIn1RisingEdge(),
            reset || (Connected(Input::Pulse2) && PulseIn2RisingEdge()));
        spatial::Scene scene=boot_.Ready()?modes_.Advance():spatial::Scene{};
        auto output=blocks.Tick(Connected(Input::Audio1)?AudioIn1():0,
                                    Connected(Input::Audio2)?AudioIn2():0,scene,effective_);
        if (startup_<4800) ++startup_;
        int32_t target=(!saving && startup_==4800 && boot_.Ready() && boot_.Selected()!=spatial::Mode::Disorientation)?1024:0;
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
            uint32_t block_fault=blocks.fault|blocks.worker_fault;
            overrun_=overrun_ || block_fault;
            LedOn(4,boot_.Selected()==spatial::Mode::Mixer?modes_.SelectedSource()==0:linked_);LedOn(5,overrun_);
            if(boot_.Selected()==spatial::Mode::Mixer&&boot_.Ready()&&modes_.Pickup()!=7){
                if((led_tick_++/250)&1){int first=modes_.SelectedSource()*2;LedOff(first);LedOff(first+1);}
            }else led_tick_=0;
            if(!boot_.Ready()||boot_.Selected()==spatial::Mode::Disorientation)for(int i=0;i<6;++i)LedOn(i,(boot_.LedMask()>>i)&1);
            shared.angle_a=scene.angle[0]>>20;shared.angle_b=scene.angle[1]>>20;
            shared.distance=scene.distance;shared.distance_b=scene.distance_b<0?scene.distance:scene.distance_b;
            shared.mixer_state=modes_.SelectedSource()|(modes_.Pickup()<<1);
            shared.flags=(linked_?1:0)|(modes_.ClockLocked()?2:0)|(overrun_?4:0)|(saving?8:0)
                |((block_fault&2)?16:0)|((block_fault&1)?32:0)|(callback_slow_?64:0);
        }
        // This measures callback duration, not the framework's surrounding ISR.
        if (!saving && !shared.save) {
            uint32_t elapsed=time_us_32()-start;
            if(elapsed>shared.callback_peak_us)shared.callback_peak_us=elapsed;
            if(elapsed>=18){overrun_=true;callback_slow_=true;}
        }
    }
    spatial::Config config_,effective_;
    spatial::MuInput mu_input_;
    spatial::MuControls mu_controls_;
    spatial::Modes modes_;
    spatial::StartupMode boot_;
    uint32_t startup_=0,led_tick_=0;
    int32_t fade_=0;
    unsigned scan_=0,stable_count_=0,silent_=0,mu_age_=0;
    Switch candidate_=Switch::Middle,selected_=Switch::Middle;
    bool linked_=false,settled_=false,overrun_=false,callback_slow_=false;
};
int main() {
    vreg_set_voltage(VREG_VOLTAGE_1_15);
    sleep_ms(10);
    set_sys_clock_khz(192000,true);
    static Card card;
    // Same power-role selection as Chris Johnson's WaveSeq: a computer selects
    // editor/device mode; a powered accessory or empty port selects host mode.
    // Older boards cannot report the role and retain editor/device operation.
    sleep_ms(150);
    host_mode=card.HostMode();
    storage.Init(card.Capacity());
    spatial::Config initial=storage.Load();
    card.Configure(initial);
    static spatial::UsbEditor usb(shared,storage,initial,RenderBlock);
    editor=&usb;
    multicore_lockout_victim_init();
    multicore_launch_core1(UsbCore);
    // Start capture only after USB initialization, avoiding startup starvation.
    while(!shared.usb_ready)tight_loop_contents();
    __dmb();
    card.EnableNormalisationProbe();
    card.Run();
}
