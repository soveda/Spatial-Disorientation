// Copyright (c) 2026 Adrian Vos (soveda). SPDX-License-Identifier: MIT
// Reference data retains Gardner/Martin KEMAR attribution terms.
#define spatial baseline
#include "../../../src/dsp/spatial.h"
#undef spatial
#include "dsp/spatial.h"
#include "elevation_control.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>

int main(){
    uint32_t packed=99;
    for(int a=0;a<3;++a)for(int b=0;b<3;++b){
        uint8_t bytes[]={uint8_t(a),uint8_t(b)};
        assert(spatial::DecodeElevation(bytes,2,packed));assert(packed==unsigned(a|(b<<2)));
    }
    uint8_t bad[]={3,0};assert(!spatial::DecodeElevation(bad,2,packed));
    assert(!spatial::DecodeElevation(bad,1,packed));assert(packed==10);
    // Level is bit-identical to the matching 64-tap horizontal renderer for both inputs and changing placements.
    baseline::Engine old;spatial::Engine fresh;uint32_t seed=7;
    for(int i=0;i<100000;++i){
        seed=seed*1664525u+1013904223u;
        if((i&63)==0){
            baseline::Scene a;spatial::Scene b;baseline::Config ca;spatial::Config cb;
            a.angle[0]=b.angle[0]=seed;a.angle[1]=b.angle[1]=0u-seed;
            a.distance=b.distance=seed&4095;a.distance_b=b.distance_b=(seed>>12)&4095;
            for(int n=0;n<5;++n)ca.value[n]=cb.value[n]=(seed>>(n*3))&4095;
            old.SetScene(a,ca);fresh.SetScene(b,cb);
        }
        int a=int(seed&4095)-2048,b=int((seed>>12)&4095)-2048;
        auto x=old.Process(a,b);auto y=fresh.Process(a,b);assert(x.left==y.left&&x.right==y.right);
    }
    // Each settled measured filter agrees with an independent impulse reference,
    // including ear swapping around the full azimuth ring and strength bypass.
    for(int elevation=1;elevation<=2;++elevation)for(int direction=0;direction<72;++direction){
        spatial::Hrtf h;
        auto phase=uint32_t((uint64_t(direction)<<32)/72+1);
        h.Geometry(phase,4095,elevation);
        for(int i=0;i<8192;++i){h.Update();h.Process(0);}
        for(int n=0;n<spatial::kHrtfTaps+8;++n){
            h.Update();auto x=h.Process(n==0?1024:0);
            for(int e=0;e<2;++e){
                int q=n<spatial::kHrtfTaps?spatial::kElevationHrtf[elevation-1][direction][e][n]:0;
                int neutral=n==0?16384:0;
                int coefficient=neutral+(((q-neutral)*4095)>>12);
                int expected=(1024*coefficient)>>14;
                assert(std::abs((e?x.right:x.left)-expected)<=1);
            }
        }
        h.Geometry(phase,0,elevation);
        for(int i=0;i<8192;++i){h.Update();h.Process(0);}
        auto bypass=h.Process(1024);assert(bypass.left==1024&&bypass.right==1024);
    }
    // Rapid independent height/azimuth/strength switching stays bounded through
    // mixed, slewed coefficient vectors and delayed full-range input.
    for(int i=0;i<200000;++i){
        seed=seed*1664525u+1013904223u;
        if((i&31)==0){
            spatial::Scene s;s.angle[0]=seed;s.angle[1]=0u-seed;s.distance=seed&4095;
            spatial::Config c;c.value[4]=(seed>>12)&4095;
            fresh.SetScene(s,c);fresh.SetElevation((seed%3)|(((seed>>16)%3)<<2));
        }
        auto x=fresh.Process(int(seed&4095)-2048,int((seed>>12)&4095)-2048);
        assert(x.left>=-2048&&x.left<=2047&&x.right>=-2048&&x.right<=2047);
    }
    // Single-source mode explicitly excludes B; the active A path is unchanged.
    spatial::Engine solo,reference;solo.SetSolo(true);spatial::Scene scene;spatial::Config cfg;
    solo.SetScene(scene,cfg);reference.SetScene(scene,cfg);solo.SetElevation(9);reference.SetElevation(9);
    for(int i=0;i<48000;++i){seed=seed*1664525u+1013904223u;int a=int(seed&4095)-2048;
        auto x=solo.Process(a,2047),y=reference.Process(a,0);assert(x.left==y.left&&x.right==y.right);}
    std::puts("PASS: atomic height validation, bit-identical Level baseline, measured height FIR references, bypass and rapid-control bounds");
}
