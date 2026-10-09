// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
#pragma once
#include "config.h"
#include <cstring>
namespace spatial {
constexpr uint8_t kProtocolVersion=2;
struct Placement {
    uint16_t value[6]={2048,0,4095,0,0,4095}; // A then B: position, distance, level.
    bool Valid()const{for(auto v:value)if(v>4095)return false;return true;}
};
struct Settings {
    Config config[2];Placement mixer;
    bool Valid()const{return config[0].Valid()&&config[1].Valid()&&mixer.Valid();}
};
static_assert(sizeof(Settings)==36,"Stable flash layout");
inline bool DecodeSettings(const uint8_t* data,size_t n,Settings& settings,uint32_t& mode){
    if(!n||data[0]>1)return false;
    uint32_t m=data[0];size_t fields=m?12:6;
    if(n!=1+fields*3)return false;
    Settings candidate=settings;Config cfg;Placement place;
    uint32_t seen=0;
    for(size_t i=1;i<n;i+=3){
        uint32_t id=data[i];if(id<1||id>fields||data[i+1]>127||data[i+2]>127)return false;
        uint32_t bit=1u<<(id-1);if(seen&bit)return false;seen|=bit;
        uint16_t v=data[i+1]|(uint16_t(data[i+2])<<7);
        if(id<=6)cfg.value[id-1]=v;else place.value[id-7]=v;
    }
    if(!cfg.Valid()||(m&&!place.Valid()))return false;
    candidate.config[m]=cfg;if(m)candidate.mixer=place;
    settings=candidate;mode=m;return true;
}
inline size_t EncodeSettings(const Settings& settings,uint32_t mode,uint8_t* data){
    data[0]=mode;EncodeConfig(settings.config[mode],data+1);
    if(mode)for(int i=0;i<6;++i){data[19+i*3]=i+7;data[20+i*3]=settings.mixer.value[i]&127;data[21+i*3]=settings.mixer.value[i]>>7;}
    return mode?37:19;
}
constexpr uint32_t kRecordMagic=0x314f4453;
struct Record {uint32_t magic,version,checksum;Settings settings;};
struct LegacyRecord {uint32_t magic,version,checksum;Config config;};
inline Record MakeRecord(const Settings& settings){return {kRecordMagic,2,Checksum(reinterpret_cast<const uint8_t*>(&settings),sizeof(settings)),settings};}
inline Settings LoadRecord(const uint8_t* data,size_t n){
    Settings result;
    if(n<sizeof(LegacyRecord))return result;
    LegacyRecord old;std::memcpy(&old,data,sizeof(old));
    if(old.magic!=kRecordMagic)return result;
    if(old.version==1&&old.config.Valid()&&old.checksum==Checksum(reinterpret_cast<const uint8_t*>(&old.config),sizeof(Config))){
        result.config[0]=result.config[1]=old.config;return result;
    }
    if(old.version==2&&n>=sizeof(Record)){
        Record current;std::memcpy(&current,data,sizeof(current));
        if(current.settings.Valid()&&current.checksum==Checksum(reinterpret_cast<const uint8_t*>(&current.settings),sizeof(Settings)))return current.settings;
    }
    return result;
}
}
