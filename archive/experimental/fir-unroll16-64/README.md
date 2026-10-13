# Alpha25: 16-tap FIR unroll experiment

**0.1.0-alpha25-fir-unroll16-64-DUAL · experimental · hardware timing pending**

Original code, tests and documentation © 2026 Adrian Vos (soveda),
[MIT](../../alpha14/LICENSE). Preserve [third-party attribution](../../alpha14/THIRD_PARTY_NOTICES.md)
and full [firmware notices](uf2/NOTICE.txt). Ear filters derive from Bill Gardner
and Keith Martin, MIT Media Laboratory (1994), *HRTF Measurements of a KEMAR
Dummy-Head Microphone*, Technical Report #280, under
[separate data terms](../../alpha14/vendor/KEMAR/SOURCE_TERMS.md). Existing ComputerCard,
Workshop_BlockAudioCard, WaveSeq and EightMU/rppicomidi adaptations remain credited.
No new external code or measurement data is introduced.

## Single experiment

Alpha24 reports a peak of **1,199 µs** with **no audible issues** from the user,
leaving only 1 µs below the block warning. This experiment changes only the
FIR kernel's loop grouping from eight to **16 paired taps per iteration**.
All **64 taps**, both ears and both sources remain. Input/left/right accumulation
order is unchanged; no precision, filter length or spatial detail is reduced.

The first compiler build outlined the larger Hrtf::Process into a separate call.
The final candidate explicitly keeps it inlined in Source::Process, as it was
in alpha24. This prevents a new call on every source frame from obscuring the
unroll comparison. The initial outlined candidate is not the packaged firmware.

Alpha24 HRTF address caching, rounding/smoothing, coefficient updates, geometry
cadence, delay reads, room model, vertical paths and controls remain unchanged.
No Wander-node cache, sloping path or clock change is included.

## Firmware and compatibility

[Alpha25 DUAL UF2](uf2/Spatial_Disorientation_0.1.0-alpha25-fir-unroll16-64-DUAL.uf2)
is ready for hardware comparison. [Alpha24 source/editor/UF2](../hrtf-address-cache-64/README.md)
remain available as the working fallback; its reported timing/listening results
do not imply full stability-protocol completion.

**192 MHz / 1.15 V**, ComputerCard **0.4.0**, audio **48 kHz**, two sources/all
64 taps, 64-frame blocks and 128-frame transport latency are retained. Callback
warning **18 µs**, block warning **1,200 µs**, actual deadline **1,333.3 µs**.
Core ownership, bounded queues and fade/flash handshake are unchanged.

**Alpha23/alpha24 experimental editors remain compatible.** The
[copied alpha25 editor](web/index.html) only updates version/help text. Settings
remain SysEx v5 / JSON v5 / flash v6, so presets and saved height settings require
no new migration. Fader 8 remains Mixer height / Disorientation motion amount.
Root/current firmware/editor and Workshop release are unchanged.

## Verification and compiled result

Pico SDK 2.3.0 / Arm GNU Toolchain 15.2.Rel1 build passes. Flash **139,716 bytes**,
main RAM **179,412 bytes**, scratch banks **2,048 bytes each**. Compared with
alpha24 this adds 116 bytes flash and 112 bytes RAM, primarily linked code/layout;
no new DSP working buffers are allocated. See [SHA256SUMS](uf2/SHA256SUMS.txt).

AddressSanitizer/UndefinedBehaviorSanitizer checks pass, including final inlined
candidate audio checks:

- **1,024,000** exact direct-HRTF frames against alpha24, including constructor
  defaults, anchors/intermediate heights, strength, wraps, geometry cadence and
  settling. **180,000** exact transport and **576,000** exact scene frames.
- Vertical path/freeze/reset/reversal/bounds/sample-alignment and continuous
  Mixer height/pickup; scalar int64 FIR reference and DSP/room/block/modes/8mu/
  settings/MIDI regressions; actual editor lifecycle/preset/save/telemetry checks.

The linked FIR loop uses **119 instructions per 16 taps**, four iterations per
64-tap source frame: **476 instructions**, versus alpha24's **488** (61 × eight).
It has **two MOV instructions per iteration, no stack accesses/spills and no
function calls**. Net kernel reduction is 12 instructions per source frame, or
1,536 across two sources in a 64-frame block. Surrounding Source::Process static
stack references rise 60→68, so a net speed gain is not guaranteed. See
[compiled inspection](docs/INSPECTION.md). Counts are not cycle timings;
**hardware A/B measurements decide whether to retain this candidate**.

Expected sound is unchanged. Sloping paths remain staged until useful headroom
is established.

## Hardware A/B test

1. Keep the alpha24 preset, inputs, CV signal/rate/depth, room, source levels,
   strength, X speed and USB/editor connection unchanged. Flash alpha25, select
   Disorientation at startup. Same saved settings and editor should work.
2. Repeat the audio-rate CV stress setup that peaked at 1,199 µs. Use both audio
   sources, moving intermediate heights and vertical excursion 40°. Test Fig8,
   Pendulum and Wander separately, resetting between runs to clear latched peaks.
   Record callback/block peaks and run duration for each path. A later identical
   peak may be retained from an earlier path if you do not reset.
3. Verify identical sound, vertical movement, freeze/reset/clock and controls;
   no clicks/gaps, locked controls, LED 5 or audio-queue warnings. Run the heaviest
   case for at least 20 minutes. Targets remain callback <18 µs/block <1,200 µs,
   preferably with more than a marginal 1–3 µs improvement.
4. Check Orbits and Mixer under rapid CV too: this FIR optimization runs in all
   modes. Read/Apply/Save/preset roundtrip should remain normal. Test a separate
   8mu boot using saved vertical settings: normal faders/motion/source pickup,
   A reset/B motion/C freeze/D movement selection and held-D timing; LED 5 off
   and no flashing timing warning. Editor and 8mu are tested in separate boots.
5. If alpha25 is slower, return to alpha24 with the same settings/inputs and
   repeat. Same data formats support direct fallback; no flash schema downgrade.

Report peaks and any sound/stability difference. Firmware build/host comparison
alone does not establish hardware headroom.

## Rebuild / local editor

From repository root:

```sh
cmake -S experimental/fir-unroll16-64 -B /tmp/spatial-fir-unroll16-64 -DPICO_SDK_PATH=/path/to/pico-sdk -DSPATIAL_ELEVATION_SOLO=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/spatial-fir-unroll16-64 -j2
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=address,undefined -DSPATIAL_HRTF_TAPS=64 -Iexperimental/fir-unroll16-64/src experimental/fir-unroll16-64/tests/address_test.cpp -o /tmp/spatial-unroll16-test
/tmp/spatial-unroll16-test
python3 -m http.server 8802 --bind 127.0.0.1 --directory experimental/fir-unroll16-64/web
```

Visit `http://127.0.0.1:8802`. Run `node tests/editor_test.cjs` from this experiment
directory. No hosted alpha25 publication is implied by this local build.
