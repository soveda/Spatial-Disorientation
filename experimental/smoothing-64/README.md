# Alpha20: smoothing-only 64-tap experiment

**0.1.0-alpha20-smoothing-64-poc · two sources · user reports all tests pass**

Original code, tests and documentation © 2026 Adrian Vos (soveda),
[MIT](../../LICENSE). Preserve [third-party attribution](../../THIRD_PARTY_NOTICES.md)
and [firmware notices](uf2/NOTICE.txt) with redistribution. Measured HRTFs are by
Bill Gardner and Keith Martin, MIT Media Laboratory (1994), *HRTF Measurements
of a KEMAR Dummy-Head Microphone*, Technical Report #280. Their
[measurement terms](../../vendor/KEMAR/SOURCE_TERMS.md) remain separate from MIT.

## Change and fallback

This independent experiment changes only the arithmetic spelling of the eight
per-source audio-rate slews and HRTF coefficient slews. A power-of-two magnitude
step removes sign-dependent branches while preserving truncation toward zero,
minimum one-unit movement, the zero shortcut and all smoothing rates. The legacy
Slew API remains for other callers and comparisons. Bounded fixed-point DSP
values are required; this is not an arbitrary INT32_MIN-safe helper.

Both 64-tap sources, measured height banks, distance/room processing, delay
addressing, geometry cadence, scene-reference transport and USB/8mu controls are
unchanged. CPU is 192 MHz / 1.15 V; audio is 48 kHz; blocks contain 64 frames and
transport latency is 128 frames. Warning thresholds remain 18 µs callback and
1,200 µs block; the block deadline is 1,333.3 µs.

The entire [alpha19 source](../scene-reference-64/src/dsp/spatial.h),
[alpha19 UF2](../scene-reference-64/uf2/Spatial_Disorientation_0.1.0-alpha19-scene-reference64-DUAL.uf2)
and [alpha19 editor](../scene-reference-64/web/index.html) remain untouched as
fallback. Alpha19's highest reported block peak is 1,158 µs with fast CV and no
audio abnormalities reported. Initial alpha20 hardware timings are recorded below; the user subsequently reports all tests pass (2026-10-11).
Root source/editor and Workshop Computer release are unchanged.

A copy of the experimental editor has updated version/help text only. Its controls
and protocol remain the same, and the alpha19 editor is also compatible. Height
still applies only in Mixer, requires **Send heights**, is runtime-only, and
resets to Level on startup. Flash v4 / preset JSON v3 / SysEx v3 are unchanged.

## Load and test

1. Flash [alpha20 DUAL UF2](uf2/Spatial_Disorientation_0.1.0-alpha20-smoothing64-DUAL.uf2).
   Open the [experimental editor](web/index.html), Enable MIDI, Connect & read.
   Timing should show HRTF 64 taps / Rendering two sources. No settings changes
   are required to use the new smoothing.
2. Repeat your alpha19 audio/settings and rapid CV stimulus. After a fresh reset
   for each mode, record callback/block peaks for Orbits, Mixer and Disorientation.
   Peaks and timing-warning LED 5 latch until reset and include startup. Normal
   boot selects Orbits; Down held at boot selects modes with Main thirds, then
   release to confirm (Orbits no LEDs, Mixer left, Disorientation right).
3. Orbits: linked/opposing, stopped/slow/fast, distance zero then maximum, rapid
   knob/CV changes, phase reset and clock operation. Listen for clicks, gaps,
   unexpected gain/tone changes or different movement.
4. Mixer: independent A/B direction, distance and level, source selection/pickup;
   compare Below/Level/Above at fixed placement, then rapidly swap heights. Test
   room zero, the same amount as alpha19, and maximum. Both sources remain active.
5. Disorientation: Fig8, Pendulum and Wander; freeze/release, phase reset, extremes
   of depth/excursion and rapid CV. Test room zero and maximum in each mode.
6. Run at least 20 minutes with both inputs active and frequent CV/control changes.
   Pass criteria: callback below 18 µs, block below 1,200 µs, no timing/queue warning
   or latched LED 5, no audible abnormalities, and normal controls. Record peaks,
   room/height settings, mode/path and duration. A fresh alpha19 comparison with
   the same stimulus is the strongest timing evidence. Do not assume the smaller
   compiled code guarantees a microsecond saving.
7. In a separate boot/session, connect 8mu and confirm the existing source/movement
   selection, faders, freeze and pickup behave as before. Use the existing held-D
   diagnostics since editor and 8mu cannot be connected together. Mixer continues
   to ignore accelerometer input; no mapping is added by alpha20.
8. If behaviour or timing regresses, flash the preserved alpha19 UF2. Preset/settings
   formats match. Saving is unnecessary for the optimization comparison.

## Initial hardware timings (2026-10-11)

User-reported alpha20 readings, 64 taps and two sources:

| Mode | Callback peak | Block peak | Margin below 1,200 µs warning |
|---|---:|---:|---:|
| Twin Orbits, opposing/free-running, distance 100% | 11 µs | 1,118 µs | 82 µs |
| Spatial Mixer, A selected, Main/X/Y pickup pending, A/B distance 0% | 14 µs | 1,113 µs | 87 µs |
| Third report: mode/path not included | 14 µs | 1,118 µs | 82 µs |

All supplied callback/block peaks are below the warning thresholds. The highest
reported block peak is 1,118 µs, about 215 µs below the 1,333.3 µs deadline.
Compared with alpha19's initial Orbits/Mixer reports (1,156/1,136 µs), these peaks
are 38/23 µs lower. The reported maximum is 40 µs below alpha19's later 1,158 µs
maximum, about 3.5%. These compare reported maxima, not a controlled benchmark:
room/height settings, CV stimulus, run durations and explicit audio/control/queue
results were not provided. Do not identify the third report as Disorientation
without confirmation. The user subsequently reports all tests pass (2026-10-11), supporting alpha20
as the current tested fallback. Individual duration/settings and the third
mode label remain unspecified. No firmware change accompanies this record.

## Verification

Firmware builds successfully with Pico SDK 2.3.0 / Arm GNU Toolchain 15.2.Rel1.
Main RAM: 175,024 bytes; flash: 136,464 bytes; scratch banks: 2,048 bytes each.
Only `dsp/fixed.h`, `dsp/spatial.h` and `dsp/hrtf.h` differ from alpha19 firmware
source. Header guards/comments and an explicit fixed-helper include support the
new arithmetic; no delay sharing or geometry cache is included.

The linked Source::Process is 1,640 bytes versus alpha19's 1,760. Its FIR loop
remains 61 instructions per eight taps (24 word loads, 16 multiplies, 19 additions,
one compare and one branch), with no stack accesses inside the loop. It has 27
branch instructions across the wider generated function versus 60, and 53 explicit
stack accesses versus 57. These static counts are not measured timing savings.

Host checks under AddressSanitizer/UndefinedBehaviorSanitizer pass:

- [Reference test](tests/reference_test.cpp): 180,000 frame-exact comparisons against
  alpha19 through old/new block transports, per-frame config/control changes, all
  paths/heights, solo/dual, queue reuse and worker starvation/recovery.
- [Scene test](tests/scene_test.cpp): 576,000 frame-exact engine comparisons with
  static/moving scenes, silence/impulses/full-scale/random input, room zero/max/
  changing, ring wraps and repeated height changes.
- [Slew test](tests/slew_test.cpp): 6,291,468 exact one-step comparisons plus complete
  increasing/decreasing convergence curves, including settled values.
- Existing DSP, room, block, mode and movement regression suites pass with these
  64-tap headers. The copied editor lifecycle/preset/height tests pass.

Build and focused comparisons, from the repository root:

```sh
cmake -S experimental/smoothing-64 -B /tmp/spatial-smoothing64 -DPICO_SDK_PATH=/path/to/pico-sdk -DSPATIAL_ELEVATION_SOLO=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/spatial-smoothing64 -j2
clang++ -std=c++17 -O2 -fsanitize=address,undefined -DSPATIAL_HRTF_TAPS=64 -Iexperimental/smoothing-64/src experimental/smoothing-64/tests/reference_test.cpp -o /tmp/spatial-smoothing64-reference-test
/tmp/spatial-smoothing64-reference-test
clang++ -std=c++17 -O2 -fsanitize=address,undefined -DSPATIAL_HRTF_TAPS=64 -Iexperimental/smoothing-64/src experimental/smoothing-64/tests/scene_test.cpp -o /tmp/spatial-smoothing64-scene-test
/tmp/spatial-smoothing64-scene-test
clang++ -std=c++17 -O2 -fsanitize=address,undefined -DSPATIAL_HRTF_TAPS=64 -Iexperimental/smoothing-64/src experimental/smoothing-64/tests/slew_test.cpp -o /tmp/spatial-smoothing64-slew-test
/tmp/spatial-smoothing64-slew-test
```

Run `node tests/editor_test.cjs` from this experiment's directory. For a local
editor server, from the repository root:

```sh
python3 -m http.server 8797 --bind 127.0.0.1 --directory experimental/smoothing-64/web
```

Visit `http://127.0.0.1:8797`. [UF2 checksum](uf2/SHA256SUMS.txt).
The [preceding inspection](../scene-reference-64/docs/DELAY_SLEW_INSPECTION.md)
explains why shared delay addressing was left out. Initial alpha20 hardware timings are recorded above; the user reports all tests pass (2026-10-11).
