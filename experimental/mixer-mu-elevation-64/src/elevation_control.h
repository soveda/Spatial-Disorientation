// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include <cstddef>
namespace spatial {
// Experimental command 4: two exact values (0 level, 1 below, 2 above).
// Runtime only; unrelated to stable parameter IDs or stored Settings.
inline bool DecodeElevation(const uint8_t* data,size_t count,uint32_t& packed){
    if(count!=2||data[0]>2||data[1]>2)return false;
    packed=data[0]|(uint32_t(data[1])<<2);return true;
}
}
