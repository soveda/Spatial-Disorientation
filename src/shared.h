// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
#pragma once
#include "config.h"
#include "mu_controls.h"
#include "hardware/sync.h"
namespace spatial {
struct Shared {
    Config queued;
    // SPSC mailbox: core 1 fills then publishes; core 0 consumes then clears.
    // Naturally aligned 32-bit flags plus barriers avoid locks in the audio ISR.
    alignas(4) volatile uint32_t ready = 0;
    // 0 running; core 1 requests 1; core 0 acknowledges silence with 2.
    alignas(4) volatile uint32_t save = 0;
    volatile uint32_t angle_a=0,angle_b=2048,distance=0,flags=0;
    volatile uint32_t callback_peak_us=0,block_peak_us=0;
    volatile uint32_t usb_ready=0,mode=0,distance_b=0,mixer_state=0;
    // A second SPSC mailbox carries one coherent 8mu control snapshot. Core 1
    // skips publication while full; core 0 never waits for USB or a writer.
    MuInput mu_queued;
    alignas(4) volatile uint32_t mu_ready=0;
    volatile uint32_t mu_feedback=0;
    bool ConsumeMu(MuInput& input){
        if(!mu_ready)return false;
        __dmb();input=mu_queued;__dmb();mu_ready=0;return true;
    }
    void PublishMu(const MuInput& input){
        if(mu_ready)return;
        mu_queued=input;__dmb();mu_ready=1;
    }
    bool Consume(Config& cfg) {
        if (!ready) return false;
        __dmb(); cfg=queued; __dmb(); ready=0;
        return true;
    }
    void Publish(const Config& cfg) {
        queued=cfg; __dmb(); ready=1;
    }
};
}
