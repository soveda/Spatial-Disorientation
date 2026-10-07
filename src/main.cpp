// Copyright (c) 2026 soveda. SPDX-License-Identifier: MIT
// Scaffold based on Chris Johnson's ComputerCard passthrough example.
// This deliberately provides a hardware smoke check before effect DSP is added.
#include "ComputerCard.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"

class Card : public ComputerCard {
    void ProcessSample() override {
        // Preserve stereo input for now; this is not the planned effect.
        AudioOut1(AudioIn1());
        AudioOut2(AudioIn2());
        CVOut1(0);
        CVOut2(0);
        PulseOut1(false);
        PulseOut2(false);
        LedBrightness(0, KnobVal(Knob::Main));
        LedBrightness(2, KnobVal(Knob::X));
        LedBrightness(4, KnobVal(Knob::Y));
        LedOn(1, SwitchVal() == Switch::Up);
        LedOn(3, SwitchVal() == Switch::Middle);
        LedOn(5, SwitchVal() == Switch::Down);
    }
};

int main() {
    // Follow Chris Johnson's ComputerCard NOTES: 192 MHz at 1.15 V.
    // Settle the regulator before raising the clock, before audio starts.
    vreg_set_voltage(VREG_VOLTAGE_1_15);
    sleep_ms(10);
    set_sys_clock_khz(192000, true);
    // Static storage leaves stack space available for future DSP and USB work.
    static Card card;
    card.EnableNormalisationProbe();
    card.Run();
}
