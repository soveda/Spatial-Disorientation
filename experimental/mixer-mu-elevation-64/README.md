# Alpha21: Mixer 8mu elevation

**0.1.0-alpha21-mixer-mu-elevation-64-poc · two sources · fader 8 and initial timings confirmed**

Original code, tests and documentation © 2026 Adrian Vos (soveda),
[MIT](../../LICENSE). Preserve [third-party attribution](../../THIRD_PARTY_NOTICES.md)
and [firmware notices](uf2/NOTICE.txt). Measured HRTFs: Bill Gardner and Keith
Martin, MIT Media Laboratory (1994), *HRTF Measurements of a KEMAR Dummy-Head
Microphone*, Technical Report #280, under [separate measurement terms](../../vendor/KEMAR/SOURCE_TERMS.md).
Original 8mu mapping/pickup integration uses this project's bounded WaveSeq/
EightMU/rppicomidi adaptation; external sources remain credited in those notices.

## New control

In Spatial Mixer, **8mu fader 8 selects the current source's elevation** using
the existing three measured profiles:

| Fader region, after pickup | Height |
|---|---|
| Lower third | Below (−40°) |
| Middle third | Level (0°) |
| Upper third | Above (+40°) |

This selects discrete profiles; it does not continuously interpolate elevation.
A/B buttons and panel source-selection gestures choose which source fader 8
edits. A and B retain their own runtime heights. Pickup rearms on source change,
USB reconnect/new session, and placement restoration. Approach/cross the centre
of the current region before moving elsewhere: centre of lower third for Below,
midpoint for Level, centre of upper third for Above. A fresh snapshot cannot
invent a crossing from an unreceived zero. Once picked, a small hysteresis band
around each boundary prevents noisy fader values from repeatedly swapping profiles.

Mixer ignores accelerometer/gyro input. Faders 1–7 retain their tested mappings:
selected distance, selected level, selected position, room, A trim, B trim,
strength. Twin Orbits and Disorientation retain fader 8 as motion amount.

Height changes are temporary. Disconnect leaves applied heights in place; a reset
restores both to Level. Save/JSON export do not store height. Existing settings
formats remain flash v4 / JSON v3 / SysEx v3. No height CV mapping or vertical
Disorientation path is introduced.

## LED feedback

8mu LED 8 now supports the height fader while retaining selected-source feedback:

- Fast blinking: waiting for elevation pickup.
- Once picked: steady for A, slow blinking for B.
- Picked brightness: Below dim, Level medium, Above bright.
- Hold D: existing timing display takes priority, with 150 µs bands per LED.

Computer LED 4 (bottom-left, indices 0–5) remains on for A / off for B in Mixer.
Computer LED 5 remains the latched timing/queue warning. Other pickup and source
position LEDs retain their existing behaviour.

## Architecture and fallback

Core 0 reads 8mu snapshots and evaluates the small bounded pickup mapping at the
existing 1 kHz scan. It publishes an aligned sequence-and-height word per source.
Core 1 consumes the latest requested value for each source before rendering and
remains the sole writer of applied height state. Requests for A/B cannot overwrite
one another; neither core waits and no DSP or flash work is added to the callback.
The state mailbox follows the project's aligned single-writer transport pattern.
Each word is self-contained, with no separately published payload or pointer.

All DSP headers are byte-identical to tested alpha20: exact smoothing, two sources,
64 taps, measured height banks, room/distance, FIR unrolling, delay addressing and
geometry cadence are unchanged. CPU remains 192 MHz / 1.15 V; audio 48 kHz, blocks
64 frames, transport latency 128 frames. Warning thresholds remain callback 18 µs
and block 1,200 µs; actual block deadline is 1,333.3 µs. New control work needs
hardware timing validation despite unchanged DSP.

The entire [alpha20 source](../smoothing-64/src/main.cpp),
[alpha20 UF2](../smoothing-64/uf2/Spatial_Disorientation_0.1.0-alpha20-smoothing64-DUAL.uf2)
and [alpha20 experimental editor](../smoothing-64/web/index.html) are retained as
fallback. The user reports all alpha20 tests pass (2026-10-11), with reported
callback peaks 11/14/14 µs and block peaks 1,118/1,113/1,118 µs. Root source/editor
and Workshop Computer release are unchanged.

The [copied experimental editor](web/index.html) updates only version/help text.
Alpha15–alpha20 elevation editors remain protocol-compatible with this firmware;
manual Send heights and readback still work in a separate editor session.

## Hardware test protocol

1. Flash [alpha21 DUAL](uf2/Spatial_Disorientation_0.1.0-alpha21-mixer-mu-elevation64-DUAL.uf2).
   Connect 8mu, reset with Down held, select the middle Main third (left LEDs),
   then release into Mixer. Do not connect the editor simultaneously.
2. Keep direction/distance/level fixed, use headphones and broadband source A.
   Both heights start Level. Press A. With fader 8 initially at an extreme, height
   should remain Level and LED 8 should fast-blink. Move fader 8 through halfway:
   pickup engages and LED 8 becomes steady at medium brightness.
3. Move fader 8 down, middle and up. Listen for the same Below / Level / Above
   changes as the height editor; LED brightness should be dim / medium / bright.
   Move slowly around the boundaries: no repeated chatter from small noise.
4. Leave A Above. Press B with the fader still up: B should stay Level and pickup
   should rearm. Move through halfway, then down to make B Below. Return to A:
   A should still be Above; approach the upper-region centre to pick it up. Test
   rapid A/B selection and confirm changing one height never changes the other.
5. Verify faders 1–7, source selection and panel handback still behave normally.
   Move/rotate 8mu: Mixer placement and height should not respond to motion.
   Disconnect/reconnect: heights remain, but fader 8 requires pickup again.
6. Hold D for timing bands and release: height must not change. Repeat rapid
   height swaps with both audio inputs, high room and rapid CV; no clicks/gaps,
   unexpected source swaps or warning LED 5. Run at least 20 minutes.
7. In a separate editor boot/session, verify Connect & read, Send heights, reset
   to Level, ordinary settings/presets and timing telemetry. Compare peaks with
   alpha20 under the same stimulus. Target callback below 18 µs and block below
   1,200 µs, no queue/timing warnings. Hardware reset clears the latched peaks.
8. Verify Orbits and all Disorientation paths, especially their unchanged 8mu
   fader-8 motion amount. Reset confirms both Mixer heights return to Level.
   Report mode/path, callback/block peaks, duration, room/height settings and sound.

If behaviour or timing regresses, flash alpha20. No Save is needed for testing.

## Reported hardware results (2026-10-11)

The user confirms **fader 8 works as planned** and reports 64 taps / two sources:

| Mode / reported scene | Callback peak | Block peak | Margin below 1,200 µs warning |
|---|---:|---:|---:|
| Disorientation Fig8, free-running, distances A 3% / B 34% | 13 µs | 1,114 µs | 86 µs |
| Twin Orbits, opposing/free-running, distance 35% | 11 µs | 1,119 µs | 81 µs |
| Spatial Mixer, A selected, Main/X/Y pickup pending, distances A/B 0% | 13 µs | 1,114 µs | 86 µs |

All supplied callback/block timings are below their warning thresholds. Maximum
block peak is 1,119 µs, about 214 µs below the 1,333.3 µs deadline. This is 1 µs
above alpha20's reported maximum of 1,118 µs: no material timing change is apparent
in these reports, although scene/settings differ and these are not controlled
benchmarks. The selected-source elevation control is confirmed; run duration,
room/height settings, exact test coverage and explicit queue/audio-regression
results were not supplied. Do not infer completion of the full stability protocol.
No firmware change accompanies this result record; alpha20 remains fallback.

## Verification and rebuild

Build passes with Pico SDK 2.3.0 / Arm GNU Toolchain 15.2.Rel1. Main RAM 175,744
bytes; flash 137,164 bytes; scratch banks 2,048 bytes each. Packaged checksum:
[SHA256SUMS](uf2/SHA256SUMS.txt). Full firmware notices are preserved.

Host checks under AddressSanitizer/UndefinedBehaviorSanitizer pass:

- [Elevation test](tests/elevation_mu_test.cpp): selected-source mapping/pickup,
  A/B isolation, source/session rearm, acknowledgements, observed external changes,
  disconnect retention, hysteresis, held-fader stability and bounded mailbox.
- [Reference test](tests/reference_test.cpp): 180,000 exact alpha20/alpha21 transport
  frames, config/control changes, paths/heights, solo/dual, queue reuse/starvation.
- [Scene test](tests/scene_test.cpp): 576,000 exact frames, static/moving scenes,
  room zero/max/changing, silence/impulses/full-scale, ring wraps and height changes.
- Slew step/settling checks, existing Mixer/Orbits/Disorientation 8mu, block and
  mode regression suites pass. Copied editor lifecycle/preset/height checks pass.

From the repository root:

```sh
cmake -S experimental/mixer-mu-elevation-64 -B /tmp/spatial-mixer-mu-elevation64 -DPICO_SDK_PATH=/path/to/pico-sdk -DSPATIAL_ELEVATION_SOLO=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/spatial-mixer-mu-elevation64 -j2
clang++ -std=c++17 -O2 -fsanitize=address,undefined -DSPATIAL_HRTF_TAPS=64 -Iexperimental/mixer-mu-elevation-64/src experimental/mixer-mu-elevation-64/tests/elevation_mu_test.cpp -o /tmp/spatial-elevation-mu-test
/tmp/spatial-elevation-mu-test
```

Run `node tests/editor_test.cjs` from this experiment directory. To serve its
copied editor from the repository root:

```sh
python3 -m http.server 8798 --bind 127.0.0.1 --directory experimental/mixer-mu-elevation-64/web
```

Visit `http://127.0.0.1:8798`. Initial hardware timings and fader-8 operation are confirmed above. Full stability
protocol coverage remains unspecified.
