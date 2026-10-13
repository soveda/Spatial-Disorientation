# Spatial Mixer: 64-tap height comparison

**0.1.0-alpha18-elevation-64-poc · experimental · DUAL hardware validation pending**

Original code/documentation © 2026 Adrian Vos (soveda), MIT;
[license](../../alpha14/LICENSE), [dependency attribution](../../alpha14/THIRD_PARTY_NOTICES.md).
HRTF measurements: Bill Gardner and Keith Martin, MIT Media Laboratory (1994),
*HRTF Measurements of a KEMAR Dummy-Head Microphone*, Technical Report #280.
[Measurement terms](../../alpha14/vendor/KEMAR/SOURCE_TERMS.md) remain separate from MIT.
Preserve [firmware notices](uf2/NOTICE.txt) with either binary.

The user reports alpha15 passes but height is very subtle. This pass compares
64-tap filters at the same measured −40° / 0° / +40° directions and shared gain.
The current editor, alpha15 experiment and Workshop Computer release are unchanged.

## Startup correction (2026-10-11)

The user reports alpha16 SOLO callback 11 µs / block 1,359 µs, a slow-block and
queue warning. The original SOLO policy depended on `shared.mode == Mixer`;
startup defaults to Orbits during the 50 ms ADC warmup and can visit other modes
while choosing. Both 64-tap sources ran in those intervals, even while output was
muted, and the peaks/faults remained latched after Mixer selection.

Alpha17 establishes SOLO on core 1 before USB/audio readiness, independent of
mode. Warning thresholds and latching are unchanged. This removes a known
startup workload error; hardware must determine whether it explains the entire
reported overrun. Reset and verify peak/warning state before changing heights,
then during repeated height/CV changes. The alpha16 SOLO binary is retained only
as the failed comparison artifact; use alpha17 for the next test.

User reports alpha17 passes: peak callback **14 µs**, peak 64-frame block
**707 µs**, 64 taps, Mixer source A only. Pendulum works as expected with the
documented centre/width controls. Editor telemetry showed A selected, X/Y pickup
pending, and both distances zero. The user subsequently confirms height is
clearer with 64 taps. This is a positive subjective listening result alongside
the initial SOLO timing pass. Run duration and a blind-identification score were
not specified; two-source 64-tap performance remains unvalidated.

## Alpha18 two-source pass (2026-10-11)

Both sources now use the 64-tap height bank in the optimized DUAL build. Working
FIR history and slewed coefficients use int32 words instead of int16 halfwords;
M0+ word loads accept immediate offsets, avoiding repeated halfword address setup.
The convolution loop processes eight taps per iteration to reduce loop overhead.
Measured tables remain int16, all coefficient values and arithmetic are unchanged,
and no filter taps, room cues or stereo source paths are removed. This trades
1,024 bytes of working state across two sources for fewer kernel instructions.
Core 0's callback, warning thresholds and block transport remain unchanged.
Alpha18 build: main RAM 175,120 bytes, flash 136,564 bytes, scratch banks
2,048 bytes each. This is memory usage, not a processor timing measurement.

Sanitizer tests pass for all measured height-direction impulse references,
64-tap horizontal scalar FIR references, strength bypass, rapid changing geometry/
profiles, bounded output, room and block transport. The experimental editor passes
its height/preset/lifecycle and SOLO/DUAL telemetry tests. Hardware timing must
still establish whether this reduction provides sufficient two-source headroom.
The passing alpha17 SOLO binary remains available unchanged as the fallback.

### Alpha18 hardware feedback (2026-10-11)

The user confirms height remains clear with both sources. Reported peaks:

| Mode | Callback | 64-frame block |
|---|---:|---:|
| Twin Orbits | 11 µs | 1,198 µs |
| Disorientation (Wander, Pendulum and Fig8, reported together) | 14 µs | 1,203 µs |
| Spatial Mixer | 13 µs | 1,181 µs |

Callbacks remain below 18 µs and reported blocks below the 1,333 µs deadline.
Disorientation crosses the 1,200 µs warning threshold. Mixer has just 19 µs
below that warning threshold, and Orbits 2 µs. This is positive two-source height
feedback with marginal warning headroom, not a complete timing/stability pass.
The run duration and queue-fault status were not supplied.

The original experimental timing text said "Mixer: two sources" for every mode;
that identified the build's renderer policy, not the selected startup mode.
The editor now labels it "Rendering: two sources". Firmware and timing thresholds
are unchanged; the separate live readout identifies the selected mode.

## Firmware and editor

- [Passing alpha17 SOLO reference](uf2/Spatial_Disorientation_0.1.0-alpha17-elevation64-SOLO-startup-fix.uf2):
  Every mode renders A only from startup; B's geometry/convolution/room processing
  is skipped and B input is silent. This isolates height listening and reduces
  worker load. B is also silent in Orbits/Disorientation on this SOLO build. Unplugging B on ordinary firmware does not skip its DSP work.
- [Optimized alpha18 DUAL firmware](uf2/Spatial_Disorientation_0.1.0-alpha18-elevation64-DUAL-optimized.uf2):
  **use this for the next test**. Both sources render, including during startup.
- [Experimental editor](web/index.html): identifies alpha18-elevation-64-poc;
  timing telemetry shows 64 taps and Mixer source A only / two sources.
- [Checksums](uf2/SHA256SUMS.txt).

Boot with Down held, choose the middle Main third (left LEDs), release for Mixer.
Use a computer USB connection and desktop Chrome/Edge. Open this folder's editor,
Enable MIDI and Connect & read. Alternatively, from the repository root:

```sh
python3 -m http.server 8795 --bind 127.0.0.1 --directory experimental/mixer-elevation-64/web
```

Visit `http://127.0.0.1:8795`. Choose A Below/Level/Above and press **Send heights**.
Dropdown changes are staged, not automatic. **Both level** sends the reference
immediately. B height/placement controls have no audible effect in SOLO.

Height remains runtime-only: hardware reset restores Level; Save/JSON exclude it.
Ordinary Apply/Save and editor disconnect retain applied height while running.
Reset to init also resets height. Existing flash v4 / JSON v3 formats and MIDI
settings IDs remain unchanged. There is no 8mu height control; changing USB roles
requires reset, so use editor/device mode for the height comparison.

## Alpha18 DUAL test

1. Flash alpha18 DUAL and boot into Mixer. Connect the alpha18 experimental editor;
   verify telemetry says **64 taps / Rendering: two sources**. Check for a startup
   warning before changing settings.
2. Patch Audio 1 only and repeat A Below/Level/Above at fixed front position,
   near distance, full strength and room zero. Then patch Audio 2 only and repeat
   for B at front. Unplugged inputs should remain silent.
3. Patch different bright sources to both inputs. Set A Below / B Above, then
   swap them. Each height change should affect only its own source. Compare the
   retained alpha17 SOLO for A's tonal/height clarity if needed.
4. Send heights repeatedly, change independent positions/distances, add rapid CV,
   and test room at 0%, 30% and maximum. Exercise both active inputs at once and
   watch timing. Thresholds remain callback 18 µs and block 1,200 µs; block deadline
   is 1,333 µs. Look for queue warnings, gaps and the latched warning LED.
5. Run at least 20 minutes with editor connected. Report callback/block peaks and
   whether both sources retain the clearer height result. If there are warnings,
   record them and return to the passing alpha17 SOLO reference.
6. Because height applies only in Mixer, Orbits/Disorientation use horizontal
   64-tap filters. Optional regression checks should include slow movement and
   phase freeze/reset, with their timing reported separately. No 8mu height
   controls or saved-height fields are added in this pass.

## Previous SOLO comparison protocol

1. Flash SOLO, select Mixer, patch a bright mono source to Audio 1 and monitor
   through headphones. Noise bursts, brushed percussion or a bright texture work
   better initially than a sine or dark bass. Set A front (2048), distance zero,
   panel level full, strength full, room zero; Apply. Keep knobs/CV fixed.
2. Compare A Below / Level / Above, using Send heights and waiting at least half
   a second after each. Judge perceived height separately from tone or loudness.
   Pitch should stay constant. Use a helper choosing a random order, or turn away
   from the screen. Try at least three repetitions of each setting.
3. Repeat with A right (3072), then rear (0), and a harmonic/percussive musical
   source. Only then try room ~30% and greater distance. Record whether height is
   clearer than alpha15, not merely whether the sound is different.
4. Compare against [alpha15 32-tap firmware](../mixer-elevation/uf2/Spatial_Disorientation_0.1.0-alpha15-elevation-poc.uf2)
   with the same source/settings and its [matching editor](../mixer-elevation/web/index.html).
   Level uses 64 taps in alpha16/alpha17, so it is not bit-identical to alpha14/alpha15's
   32-tap Level. Compare height distinctions within each build before comparing
   the two builds overall. No added gain or exaggerated spectral cue is used.
5. In SOLO, Audio 2 alone must be silent. Repatch Audio 1, rapidly change heights,
   position/distance and CV while watching timings. Run 20 minutes with the
   editor connected. Targets: callback below 18 µs, block below 1,200 µs,
   no bottom-right warning or gaps. Reset clears latched peaks/warnings.
6. If SOLO gives useful height, optionally flash DUAL and repeat with A, then B
   alone, then both. Check timing even with B unplugged: DUAL still processes B.
   The block deadline remains 1,333 µs; previously tested horizontal 64-tap work
   reached the warning region. If gaps or warnings occur, record peaks and return
   to SOLO/alpha15 rather than treating DUAL as ready for use.

Please report perceived Below/Level/Above without visual cues and peak callback/
block time for each firmware. Compile/host checks do not establish a hardware pass.

## DSP and build provenance

The alpha15 generator workflow is retained: KEMAR diffuse-field data, actual
sparse-ring azimuth labels, magnitude interpolation to 72 directions, ear mirroring,
44.1→48 kHz resampling, minimum-phase reconstruction, eight-tap tail taper,
shared gain 0.48727477260326996 and Q14 quantization. The new bank retains 64 taps;
Level uses the existing horizontal 64-tap table. Separate analytical ITD retains
cos(40°) scaling at nonzero height; room cues are unchanged. Coefficients slew
across changes. Convolution/geometry remain on core 1; no ISR height DSP is added.
ComputerCard 0.4.0, 192 MHz / 1.15 V, 48 kHz and 64-frame transport are unchanged.

[Generator](tools/generate_elevation.py), [generation metrics](generation.json).
Across the ±40° rings, numerical magnitude RMS error (1–12 kHz) falls from
1.18→0.61 dB below and 0.83→0.32 dB above when retaining 64 rather than 32 taps.
These are approximation metrics, not proof of better perceived height.
The generator verifies accumulator and ear-delay bounds including coefficients
mixed across slewed profiles. Measurements retain Gardner/Martin attribution;
new generator/integration is original Adrian Vos code under MIT. NumPy/SciPy
are offline generation tools and are not linked into firmware.

Build from the repository root with Pico SDK 2.3.0 / Arm GNU Toolchain 15.2.Rel1:

```sh
cmake -S experimental/mixer-elevation-64 -B /tmp/spatial-elevation64-solo -DPICO_SDK_PATH=/path/to/pico-sdk -DSPATIAL_ELEVATION_SOLO=ON
cmake --build /tmp/spatial-elevation64-solo -j2
cmake -S experimental/mixer-elevation-64 -B /tmp/spatial-elevation64-dual -DPICO_SDK_PATH=/path/to/pico-sdk -DSPATIAL_ELEVATION_SOLO=OFF
cmake --build /tmp/spatial-elevation64-dual -j2
```

Earlier builds: alpha17 SOLO main RAM 174,080 bytes / flash 136,544 bytes;
DUAL main RAM 174,080 bytes / flash 136,540 bytes; scratch banks 2,048 bytes each.

Only 64 taps are supported in this experiment. SOLO applies from startup in all modes;
DUAL renders both sources with horizontal 64-tap filters in Orbits/Disorientation
and needs separate timing validation before use.

Host sanitizer checks cover all height-direction impulse references, neutral
bypass, rapid profile switching/bounds, agreement with the matching 64-tap
horizontal renderer, SOLO output agreement with A-only DUAL rendering, and
B silence throughout warmup/selector traversal/release.
DSP, room, block transport, mode and settings regression suites pass against the
experimental headers. The copied editor tests cover height sending/readback,
SOLO/DUAL telemetry and existing preset/lifecycle behaviour.
