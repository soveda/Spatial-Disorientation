# Alpha24: HRTF row-address caching

**0.1.0-alpha24-hrtf-address-cache64-DUAL · experimental · hardware timing pending**

Original code, tests and documentation © 2026 Adrian Vos (soveda),
[MIT](../../LICENSE). Preserve [third-party notices](../../THIRD_PARTY_NOTICES.md)
and full [firmware notices](uf2/NOTICE.txt). Ear-filter measurements: Bill Gardner
and Keith Martin, MIT Media Laboratory (1994), *HRTF Measurements of a KEMAR
Dummy-Head Microphone*, Technical Report #280, under
[separate terms](../../vendor/KEMAR/SOURCE_TERMS.md). Existing ComputerCard,
Workshop_BlockAudioCard, WaveSeq and EightMU/rppicomidi adaptations remain credited.
No new external code/data is introduced.

## Purpose and single change

Alpha23's reported audio-rate-CV stress peak reached 1,203 µs, just beyond the
1,200 µs warning. Earlier readings reached 1,209 µs; peaks and flags latch until
reset. The user reports no warnings in the 8mu run and vertical movement audible
but subtle, possibly affected by ear fatigue. These reports do not establish
complete listening/persistence/stability validation of alpha23.

This experiment copies alpha23 and changes only **HRTF table addressing**:
geometry resolves four immutable direction-row pointers (primary current/next,
Level current/next) and whether intermediate-height blending is needed. The
per-frame coefficient updater uses those rows directly, avoiding repeated bank
selection and direction-row offset calculations. Default pointers preserve
Update-before-Geometry behaviour. Each source owns its cache entirely on core 1;
there is no publication, lock, dynamic allocation or cached live-control pointer.

Coefficient interpolation, strength scaling, integer rounding, smoothing,
quiet-update detection, tap traversal and geometry frequency are unchanged.
Both sources, all 64 taps, FIR unrolling, delay reads, room, paths, controls,
settings, editor commands and 8mu mapping remain alpha23. Wander-node caching,
larger FIR unrolls and sloping paths are excluded so timing results isolate this
experiment. Geometry now prepares addresses; its added work must also be measured.

## Firmware, editor and fallback

Flash [alpha24 DUAL](uf2/Spatial_Disorientation_0.1.0-alpha24-hrtf-address-cache64-DUAL.uf2).
[Alpha23 source/editor/UF2](../vertical-disorientation-64/README.md) remains
untouched as fallback. Root/current firmware/editor and Workshop release are
unchanged. Alpha24 uses the same **SysEx v5 / JSON v5 / flash v6** formats as
alpha23: no new migration or parameter. The [alpha23 editor](../vertical-disorientation-64/web/index.html)
remains compatible; the [copied alpha24 editor](web/index.html) only updates
version/help text. Heights, vertical centre/excursion and presets retain meaning.

Clock remains **192 MHz / 1.15 V**, ComputerCard **0.4.0**, audio **48 kHz**,
64-frame blocks, 128-frame transport latency. Callback warning **18 µs**; block
warning **1,200 µs**; actual block deadline **1,333.3 µs**. No threshold or
processor-clock increase is used to hide the warning.

Pico SDK 2.3.0 / Arm GNU Toolchain 15.2.Rel1 build passes. Flash 139,600 bytes;
main RAM 179,300 bytes; scratch banks 2,048 bytes each. This is +176 bytes flash
and +208 bytes main RAM versus alpha23, including +32 bytes of two-source
pointer state and the linked code/data changes. See [SHA256SUMS](uf2/SHA256SUMS.txt).

## Verification

AddressSanitizer/UndefinedBehaviorSanitizer tests pass:

- 1,024,000 exact direct-HRTF frame comparisons against alpha23: constructor
  defaults, measured anchors/intermediate heights, direction wrap, strength,
  geometry changes at 1/16/32/64/4096 frames and full settling.
- 180,000 exact block-transport and 576,000 exact scene frames against alpha23,
  now including moving vertical centre/excursion as well as paths, rapid controls,
  room, full-scale inputs, queue reuse/starvation/recovery and solo/dual rendering.
- Vertical path/freeze/reset/reversal/bounds/sample-alignment tests; continuous
  Mixer height/pickup; settings migration; existing DSP/room/block/modes/movements,
  8mu and MIDI TX regressions.
- Actual editor script with simulated MIDI, including vertical and Mixer
  Save/preset roundtrips, legacy imports, mode isolation and telemetry.

Compiled inspection confirms **61 FIR instructions per eight taps**, no stack
accesses or MOV instructions inside the FIR loop in either build. Source::Process
has fewer static instructions/stack references outside that loop. See
[compiled inspection](docs/INSPECTION.md). These counts do not establish cycle
savings; **hardware timing is still required**. Expected sound is unchanged.

## Hardware A/B timing test

1. Keep the same presets, input signals, CV rate/depth, room, strength, X speed
   and source levels for alpha23 and alpha24. Use headphones and both inputs.
   Reset before each measured run; latched peaks cannot distinguish later modes.
2. Start with alpha24 normal Orbits/Mixer and Disorientation centre 0°/excursion
   zero. Verify normal sound/controls and no warning. Then use vertical excursion
   40° and moving intermediate heights in Fig8/Pendulum/Wander. Confirm height
   telemetry and audio match alpha23; include save/reset and preset checks.
3. Repeat the exact audio-rate CV setup that produced 1,203 µs. Keep the matching
   editor connected if that was the original setup. Run each path separately,
   resetting between them; use high room and both sources. Record callback/block
   peaks, settings and duration. Level/exact ±40° are useful anchor comparisons,
   but continuously changing intermediate heights are the important stress case.
4. Run the heaviest case for at least 20 minutes. Target callback <18 µs, block
   <1,200 µs, no LED 5/queue warning, clicks/gaps or locked controls. Prefer a
   meaningful margin below the warning, not merely a 1–3 µs improvement.
5. In a separate 8mu boot, use saved vertical settings; verify fader pickup,
   motion amount, A reset/B motion/C freeze/D path selection and held-D timing.
   Computer LED 5 should remain off; D timing bands should not flash. Mixer fader
   8 remains continuous selected height. No simultaneous editor/8mu connection.
6. If alpha24 is slower or sounds different, return to alpha23 and report the
   mode/scene. Same settings formats permit fallback without a version downgrade
   migration. Sloping paths remain staged until adequate headroom is established.

Please report each path's peaks and whether the rapid-CV warning disappears.

## Rebuild

From repository root:

```sh
cmake -S experimental/hrtf-address-cache-64 -B /tmp/spatial-hrtf-address-cache64 -DPICO_SDK_PATH=/path/to/pico-sdk -DSPATIAL_ELEVATION_SOLO=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/spatial-hrtf-address-cache64 -j2
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=address,undefined -DSPATIAL_HRTF_TAPS=64 -Iexperimental/hrtf-address-cache-64/src experimental/hrtf-address-cache-64/tests/address_test.cpp -o /tmp/spatial-address-test
/tmp/spatial-address-test
python3 -m http.server 8801 --bind 127.0.0.1 --directory experimental/hrtf-address-cache-64/web
```

Visit `http://127.0.0.1:8801`. Run `node tests/editor_test.cjs` from this experiment
directory. No hosted alpha24 publication is implied by this local build.
