# Spatial Mixer elevation proof of concept

**0.1.0-alpha15-elevation-poc · experimental · hardware validation pending**

Original implementation/documentation © 2026 Adrian Vos (soveda), MIT;
[application license](../../LICENSE), [existing dependency attribution](../../THIRD_PARTY_NOTICES.md).
Measured responses: Bill Gardner and Keith Martin, MIT Media Laboratory, 1994,
*HRTF Measurements of a KEMAR Dummy-Head Microphone*, Technical Report #280.
[Retained measurement terms](../../vendor/KEMAR/SOURCE_TERMS.md). Data retains its
own terms; MIT applies to original code/documentation.

This isolated source tree and [experimental editor](web/index.html) add independent
**Below (−40°), Level (0°), Above (+40°)** choices to Spatial Mixer. The root editor,
root firmware source and Workshop Computer release are unchanged. This is a
stationary-height listening experiment, not a full 3D renderer or vertical path.

## Load and use

Flash [experimental firmware](uf2/Spatial_Disorientation_0.1.0-alpha15-elevation-poc.uf2).
Hold Down at startup, choose the middle Main third (left LEDs), then release for
Spatial Mixer. Connect to a computer for editor/device USB mode.

Open `web/index.html` **from this folder** in desktop Chrome/Edge, or run:

```sh
python3 -m http.server 8794 --bind 127.0.0.1 --directory experimental/mixer-elevation/web
```

Run that command from the repository root, then visit `http://127.0.0.1:8794`.
The page identifies itself as alpha15-elevation-poc. Enable MIDI and Connect & read.
Select A/B heights in the experimental section, then press **Send heights**.
Dropdown changes are staged; **Both level** sends Level for both sources immediately.
Read retrieves applied heights; Reset to init also sets both Level.

Height is runtime-only, retained while the card runs, including editor disconnect
and ordinary Apply/Save. Hardware reset restores Level. **Save and JSON export
exclude height**, retaining the unchanged alpha14 flash v4 / JSON v3 schemas and
existing presets. No additional flash write occurs for height. Switching to 8mu
requires reset and therefore returns both heights to Level. There is no 8mu height
mapping in this pass. Panel controls, CV and LEDs keep their existing Mixer roles;
the diagram shows horizontal direction only. Other modes use the horizontal bank.

## Listening and timing protocol

1. Use headphones. Patch a broad-spectrum source to Audio 1 only; leave Audio 2
   unplugged. Noise bursts, brushed percussion or a bright sustained texture are
   more useful initially than a pure sine or a dark bass sound.
2. In the editor set A position to front (2048/noon), A distance to zero,
   A panel level full, spatial strength full and room zero. Apply. Leave physical
   knobs and CV fixed; Mixer pickup prevents unrelated knob jumps.
3. Compare A Below, Level and Above using Send heights. Wait at least half a
   second after each change. Listen for placement below ear level versus above
   the head, separately from brightness or loudness changes. Both level is the
   horizontal reference. Pitch should remain unchanged.
4. Repeat blind with a helper choosing the three settings in random order, or
   turn away from the screen between selections. Record perceived Below/Level/
   Above, confidence and whether it is merely a different tone. Try at least
   three repetitions per setting. The nominal angle is a measurement direction,
   not a promise of the perceived angle for your ears.
5. Repeat at A right (3072) and rear (0), then with a harmonic/percussive musical
   source. Try room around 30% and a greater distance only after the dry test;
   these cues may obscure height. Do not judge height from the diagram.
6. Patch only Audio 2 and repeat for B with B position set front (2048). Then use
   two different sources, A Below / B Above, and swap their heights. Each source
   should change independently; position/distance/level must remain intact.
7. Send heights repeatedly while changing position/distance and applying rapid
   CV. Watch peak callback and 64-frame block timing. Targets: callback below
   18 µs, block below 1,200 µs, no latched bottom-right warning or audible gaps.
   Reset clears peaks/warnings. Test at least 20 minutes with the editor connected.
8. Both level should restore the familiar horizontal sound. With room zero and
   spatial strength zero, height spectral/timing differences should disappear.
   Hardware reset restores both Level; normal Orbits/Disorientation should remain
   familiar. Ordinary presets still round-trip without height fields.

Please report peak callback/block times and whether **blind perceived height** is
useful for each source/azimuth, rather than only whether the tones differ. A pass
would justify continuous height control, persistence and an 8mu mapping next.

## DSP, provenance and verification

The existing unmodified horizontal 32-tap table remains the Level reference.
The retained KEMAR diffuse-field archive contains 29 measured azimuths per ±40°
ring, labelled 0–180°. The generator interpolates magnitudes using the actual
integer labels to the existing five-degree grid, mirrors/swaps ears for the other
hemisphere, resamples 44.1→48 kHz, and derives tapered 32-tap minimum-phase Q14
filters with the same shared gain as alpha14. There is no per-direction gain
normalization. The analytical interaural delay is scaled by cos(40°) for elevated
profiles; room cues remain the original horizontal model.

Only one stereo filter pair runs for each source. Coefficients slew during height
changes; no second convolver or trigonometry is added at audio rate. Combined
bank/neutral-impulse coefficient bounds cover intermediate slewed vectors.
Geometry and height selection remain on core 1; core 0's callback is unchanged.
192 MHz / 1.15 V, 48 kHz, 64-frame blocks and transport latency remain unchanged.

Source copying/build conventions and scheduling retain ComputerCard/WaveSeq/
Workshop_BlockAudioCard credits from the root notices. New generation/integration
is original Adrian Vos code, MIT. [Generator](tools/generate_elevation.py) and
[generation record](generation.json) retain the archive hash and source directions.
NumPy/SciPy are generation tools, not firmware dependencies.

Build from the repository root:

```sh
cmake -S experimental/mixer-elevation -B /tmp/spatial-elevation-poc -DPICO_SDK_PATH=/path/to/pico-sdk
cmake --build /tmp/spatial-elevation-poc -j2
```

32 taps are required for this experiment. Pico SDK 2.3.0 / Arm GNU Toolchain
15.2.Rel1 build passes: flash 109,672 bytes, main RAM 146,684 bytes, each scratch
bank 2,048 bytes. The two new banks add 18,432 table bytes. Actual device timings
are pending; a successful build does not prove deadline or localization performance.

Sanitizer tests verify all elevated-direction FIR impulse references, exact Level
agreement with alpha14, strength bypass, malformed-height rejection and rapid
independent height/position/strength bounds. Existing host suites and the copied
editor's lifecycle/preset/height-send tests also run against this experiment.

Experimental MIDI extension (existing transport version 3): command 4 sends two
bytes, A then B, each 0 Level / 1 Below / 2 Above; normal ACK. Command 5 reads
heights, reply 0x45 with the same two bytes. Both commands require Mixer. Older
firmware rejects them; use the matching experimental editor and UF2. Stable
settings IDs and formats are unchanged.

Keep [firmware notices](uf2/NOTICE.txt) with redistributed firmware;
[SHA-256 checksum](uf2/SHA256SUMS.txt).
