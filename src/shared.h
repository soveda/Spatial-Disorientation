// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
#pragma once
#include "config.h"
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
