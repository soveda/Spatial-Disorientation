# Spatial Mixer: 64-tap height comparison

**0.1.0-alpha17-elevation-64-poc · experimental · hardware tests pending**

Original code/documentation © 2026 Adrian Vos (soveda), MIT;
[license](../../LICENSE), [dependency attribution](../../THIRD_PARTY_NOTICES.md).
HRTF measurements: Bill Gardner and Keith Martin, MIT Media Laboratory (1994),
*HRTF Measurements of a KEMAR Dummy-Head Microphone*, Technical Report #280.
[Measurement terms](../../vendor/KEMAR/SOURCE_TERMS.md) remain separate from MIT.
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

## Firmware and editor

- [SOLO firmware](uf2/Spatial_Disorientation_0.1.0-alpha17-elevation64-SOLO-startup-fix.uf2):
  **start here**. Every mode renders A only from startup; B's geometry/convolution/room processing
  is skipped and B input is silent. This isolates height listening and reduces
  worker load. B is also silent in Orbits/Disorientation on this SOLO build. Unplugging B on ordinary firmware does not skip its DSP work.
- [DUAL firmware](uf2/Spatial_Disorientation_0.1.0-alpha16-elevation64-DUAL.uf2):
  optional two-source load/listening comparison after SOLO. Both sources render.
- [Experimental editor](web/index.html): identifies alpha17-elevation-64-poc;
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

## Focused test

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

Both firmware builds pass: alpha17 SOLO main RAM 174,080 bytes / flash 136,544 bytes;
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
