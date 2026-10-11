# Alpha19: scene-reference optimization experiment

**0.1.0-alpha19-scene-reference-64-poc · two sources · hardware timing pending**

Original code/documentation © 2026 Adrian Vos (soveda), MIT;
[license](../../LICENSE), [dependency/source attribution](../../THIRD_PARTY_NOTICES.md).
HRTF measurements: Bill Gardner and Keith Martin, MIT Media Laboratory (1994),
*HRTF Measurements of a KEMAR Dummy-Head Microphone*, Technical Report #280.
[Measurement terms](../../vendor/KEMAR/SOURCE_TERMS.md) remain separate from MIT.
Preserve [firmware notices](uf2/NOTICE.txt) with redistributed firmware.

## Experiment and fallback

This separate source tree removes only the worker's per-frame scene/config copy.
The preceding [alpha18 source](../mixer-elevation-64/src/block_audio.h),
[alpha18 DUAL UF2](../mixer-elevation-64/uf2/Spatial_Disorientation_0.1.0-alpha18-elevation64-DUAL-optimized.uf2)
and [alpha18 experimental editor](../mixer-elevation-64/web/index.html) are untouched
and remain the fallback. The root editor/source and Workshop Computer release
are also unchanged. Height, controls, presets and filtering should sound identical.

Core 0 still captures every audio frame with its scene/config in the queue.
Core 1 previously copied those values into Engine 64 times per block; now it
passes const references to that owned frame for each Process call. Geometry still
reads the same frame at each source's staggered 32-sample cadence. No reference
is stored beyond the call; Ready/Rendering slot ownership and publication barriers
are unchanged. The existing SetScene/copy API is retained for standalone callers.

Both sources keep 64 taps, measured height profiles, room cues and smoothing.
Clock remains 192 MHz / 1.15 V, audio 48 kHz, blocks 64 frames, transport latency
128 frames. No warning threshold is raised and no diagnostic fault is suppressed.
No height persistence, additional control mapping or vertical path is introduced.

## Load and compare

1. Flash [alpha19 DUAL firmware](uf2/Spatial_Disorientation_0.1.0-alpha19-scene-reference64-DUAL.uf2).
   For height, hold Down at startup, select the middle Main third (left LEDs),
   then release into Mixer. Other startup modes retain their controls.
2. Open the [alpha19 experimental editor](web/index.html) in desktop Chrome/Edge,
   Enable MIDI, Connect & read. Telemetry should show 64 taps and two sources.
   The earlier experimental editor remains protocol-compatible.
3. Repeat the same sound/settings used for alpha18: two sources, rapid CV/knob
   changes, repeated A/B Below/Level/Above swaps, room zero then 30% and maximum.
   Listen for clicks, gaps, unexpected source swaps or control timing changes.
   Static positions should also sound unchanged. Height still requires Send heights
   and is runtime-only; reset restores Level.
4. Repeat Orbits and Disorientation (Fig8, Pendulum, Wander), including stop,
   freeze/release and phase reset. Compare peaks for each mode after a fresh
   hardware reset so each run starts with cleared peaks/warning latches.
5. Run 20 minutes in editor/device mode with both sources active. Target callback
   below 18 µs and block below 1,200 µs, no queue warnings or audible gaps. Record
   callback/block peaks, mode, room/height settings and run duration.
6. If sound or stability regresses, flash the preserved alpha18 UF2 and use its
   editor. Settings formats are unchanged (flash v4 / JSON v3). Save is unnecessary
   for this comparison; height settings are never saved.

Alpha18 user baseline: Mixer 13/1,181 µs; Orbits 11/1,198 µs; Disorientation
14/1,203 µs (callback/block). Height remains clear. Alpha19's actual savings must
be measured on hardware; sample agreement does not establish a timing pass.

## Verification and rebuild

[Comparison test](tests/reference_test.cpp) uses the preserved alpha18 implementation
and actual old/new block transports, comparing 180,000 frames under per-frame
control/config changes, all paths, independent heights, SOLO/DUAL policies and
worker stalls/slot reuse. Outputs and resolved geometry match exactly under
AddressSanitizer/UndefinedBehaviorSanitizer. Existing DSP, room, block, mode and
movement regression suites pass against the new headers. Copied editor tests pass.

Build from the repository root (Pico SDK 2.3.0 / Arm GNU Toolchain 15.2.Rel1):

```sh
cmake -S experimental/scene-reference-64 -B /tmp/spatial-scene-reference64 -DPICO_SDK_PATH=/path/to/pico-sdk -DSPATIAL_ELEVATION_SOLO=OFF
cmake --build /tmp/spatial-scene-reference64 -j2
clang++ -std=c++17 -O2 -fsanitize=address,undefined -DSPATIAL_HRTF_TAPS=64 -Iexperimental/scene-reference-64/src experimental/scene-reference-64/tests/reference_test.cpp -o /tmp/spatial-reference64-test
/tmp/spatial-reference64-test
```

Serve the separate editor from the repository root if local-file MIDI is blocked:

```sh
python3 -m http.server 8796 --bind 127.0.0.1 --directory experimental/scene-reference-64/web
```

Visit `http://127.0.0.1:8796`. [UF2 checksums](uf2/SHA256SUMS.txt).
Build passes: main RAM 175,136 bytes, flash 136,584 bytes and 2,048 bytes
per scratch bank. Hardware timing is still pending.
Measured banks are unchanged from alpha18 and retain the existing KEMAR generator
and data notices; block ownership follows Workshop_BlockAudioCard. Source-reference
integration and comparison tests are original Adrian Vos code under MIT.
