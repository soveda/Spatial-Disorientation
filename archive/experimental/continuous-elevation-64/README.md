# Alpha22: continuous Mixer elevation and saved heights

**0.1.0-alpha22-continuous-elevation64-DUAL · experimental · user reports tests pass (2026-10-12)**

Original code, tests and documentation © 2026 Adrian Vos (soveda),
[MIT](../../alpha14/LICENSE). Preserve [third-party notices](../../alpha14/THIRD_PARTY_NOTICES.md)
and the full [firmware notices](uf2/NOTICE.txt). Ear-filter measurements are by
Bill Gardner and Keith Martin, MIT Media Laboratory (1994), *HRTF Measurements
of a KEMAR Dummy-Head Microphone*, Technical Report #280, under
[separate data terms](../../alpha14/vendor/KEMAR/SOURCE_TERMS.md). Existing ComputerCard,
Workshop_BlockAudioCard, WaveSeq, EightMU and rppicomidi adaptations retain their
attribution in those notices. No new external code or data is introduced.

## Controls and sound

Spatial Mixer now has independent **continuous A/B height**, from −40° Below,
through 0° Level, to +40° Above. Editor sliders and selected-source **8mu fader 8**
use 0–4096 internally, with Level at 2048. A/B source buttons select which height
fader 8 edits. Pickup follows the actual saved/applied height and rearms on source
change, external changes or a new USB session. Crossing or approaching within 64
units engages pickup. Once picked, intermediate values are continuous; the old
three-region selection/hysteresis is removed. Mixer still ignores motion.

This is an approximation between the existing measured Level and ±40° filters,
not a new full-elevation dataset. Coefficients interpolate in fixed point;
existing smoothing settles the filter and delay changes. Lateral timing projection
also blends from its Level value to cos(40°) at each extreme. Exact endpoints and
Level remain sample-for-sample identical to alpha21, including during movement.
Intermediate heights may sound like a smooth spectral/placement change rather
than a precisely perceived angle; test with headphones and broadband sound.

Faders 1–7 keep selected distance/level/position, room, A/B trims and strength.
Twin Orbits and Disorientation remain horizontal; fader 8 still controls motion
amount in those modes. Computer LED 4 remains A on / B off in Mixer; LED 5 remains
the latched queue/timing warning. 8mu LED 8 fast-blinks pending height pickup,
then stays steady for A or slow-blinks for B. Brightness rises continuously with
height. Held D retains timing-display priority.

## Editor, persistence and compatibility

Use this pass's [editor](web/index.html) with this
[UF2](uf2/Spatial_Disorientation_0.1.0-alpha22-continuous-elevation64-DUAL.uf2).
The current/root editor is unchanged. This editor requires **SysEx v4** firmware;
older editors are not compatible with alpha22. Parameter IDs **14/15** are A/B
height; ID 13 remains Disorientation movement. Mixer settings snapshots contain
14 fields (43 bytes). Height audition/read commands use two 14-bit MIDI values.
See [protocol details](docs/PROTOCOL.md).

- Move sliders to stage height; **Send heights** auditions just the heights.
- **Apply** sends all displayed settings, including height. It does not save.
- **Both level** auditions 0° for both sources.
- **Read** captures actual live Mixer placements and heights.
- **Save to card** captures applied height and live placements, retaining other
  modes' banks. The existing short mute/flash-lockout handshake is preserved.
- **Export** downloads the displayed settings as JSON v4, including Mixer height.
  **Import** stages them offline; reconnect retains edits until Apply/Read.
- **Reset to init** resets the selected mode only. Mixer heights become Level;
  nothing is written to flash until Save.

Flash records are now **v5**. Records v1–v4 migrate in RAM, preserving their
existing settings and adding Level for both heights. JSON v1–v3 imports remain
supported; older Mixer files default heights to Level. No flash write happens
on migration, Apply, height audition or 8mu activity. Startup mode selection is
unchanged: hold Down and select Mixer with Main's middle third, then release.
Saved Mixer heights restore on this startup; normal boot still selects Orbits.

The editor and 8mu need separate USB boots. **There is no 8mu Save gesture**:
height changes made in host mode are temporary and a reboot restores the last
editor-saved height. To retain an 8mu performance height, set the same height in
an editor session and Save. Other 8mu configuration overrides remain live only.

[Alpha21 source/editor/UF2](../mixer-mu-elevation-64/README.md) is untouched as
fallback. After an alpha22 Save, older firmware cannot read the v5 record and
uses defaults. Keep an older JSON preset if you need to restore settings after
downgrading; older editors cannot import a v4 preset. Root firmware/editor and
Workshop Computer release are unchanged.

## Architecture and processing

192 MHz / 1.15 V, ComputerCard 0.4.0, 48 kHz, both sources, all 64 taps remain.
Core 0 captures hardware/audio and performs 1 kHz controls; core 1 renders 64-frame
blocks and services USB. Transport latency stays 128 frames (2.67 ms).
Height publication uses an aligned self-contained sequence/value word per source;
core 1 alone owns applied height after initialization. Read/Save captures one
packed A/B height snapshot through the existing handshake.

The FIR kernel, exact alpha20 slew, delay reads, room model and staggered geometry
cadence are unchanged. Intermediate heights add a second bank interpolation and
blend during coefficient updates; static settled geometry can skip this work.
Anchor heights use a single bank. There is no measured CPU saving claim.
Targets remain callback <18 µs and block <1,200 µs, with the actual block deadline
1,333.3 µs. Compare hardware peaks at **intermediate heights with rapid CV**.

## Hardware test protocol

1. Flash alpha22. In an editor/device USB boot, hold Down, select the middle
   Main third (left LEDs), release. Open the matching experimental editor in a
   Chrome-family browser using a USB-C data cable; grant MIDI/SysEx, connect both
   Spatial Disorientation ports. Close other MIDI/Serial applications.
2. Use headphones and broadband A, fixed direction/distance/level. Audition
   −40°, −20°, 0°, +20°, +40° with Send heights. Listen for smooth intermediate
   changes and no click/gap. Repeat with B and both sources; changing A must not
   alter B. Compare anchors against alpha21 if desired.
3. Set different intermediate heights (e.g. A −20°, B +20°). Apply, Save, Read.
   Reset into Mixer, reconnect/read: both heights and placements must restore.
   Boot Orbits/Disorientation: sound remains horizontal and mode settings intact.
   Return to Mixer: saved heights must still be present.
4. Export a non-Level Mixer preset. Change heights; import it (no audio change
   yet), Apply and Read: exact heights return. Import an older v3 Mixer preset:
   placements remain and heights become Level. Disconnect/reconnect with staged
   edits: reconnect must retain those edits until Apply or Read.
5. Reset to init: Mixer heights become Level; reset without Save should restore
   the previously saved heights. Both level auditions Level without flash.
6. Separately connect 8mu and boot Mixer. Fader 8 starts unpicked unless near the
   saved height. Sweep through it, then slowly move throughout the range. LED 8
   must show pickup then continuous brightness. A/B source switches rearm pickup
   and retain each height. Test disconnect/reconnect and held D; no height jump.
   Reset into Mixer confirms unsaved 8mu heights return to editor-saved values.
7. Run both sources at intermediate heights, high room, rapidly changing/audio-rate
   CV and fast fader changes for at least 20 minutes. No unexpected clicks, gaps,
   locked controls, queue warning or latched LED 5. Record callback/block peaks
   and height/room settings. Repeat at Level/extremes under the same stimulus;
   intermediate-height moving scenes are the added workload.
8. Regression: verify faders 1–7, panel pickup, all Twin Orbits controls, and
   Fig8/Pendulum/Wander including freeze/reset/D selection and fader-8 motion.

Report listening results, save/reset results, run duration and mode-specific peaks.
The user reports the tests pass, including Save to card; see the result record below.

## Verification and rebuild

Pico SDK 2.3.0 / Arm GNU Toolchain 15.2.Rel1 build passes without warnings.
Flash 137,960 bytes; main RAM 176,560 bytes; scratch banks 2,048 bytes each.
[SHA256SUMS](uf2/SHA256SUMS.txt) covers the packaged UF2.

AddressSanitizer/UndefinedBehaviorSanitizer host checks pass: 180,000 block-transport
and 576,000 scene frames exactly match alpha21 at measured anchors; 6,291,468
slew comparisons and full settling curves; intermediate impulse responses;
continuous pickup (>2,000 distinct values), source/session rearm and bounded
mailbox; flash v1–v4 migration/v5 roundtrip and malformed atomic decode; existing
DSP/room/block/modes/movements/8mu/startup/MIDI regressions. Actual editor script
with simulated MIDI passes height audition/read/save/preset v4, old v1–v3 imports,
offline reconnect retention, independent mode banks and malformed rejection.
These checks do not replace hardware save/flash or timing tests.

```sh
cmake -S experimental/continuous-elevation-64 -B /tmp/spatial-continuous-elevation64 -DPICO_SDK_PATH=/path/to/pico-sdk -DSPATIAL_ELEVATION_SOLO=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/spatial-continuous-elevation64 -j2
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=address,undefined -DSPATIAL_HRTF_TAPS=64 -Iexperimental/continuous-elevation-64/src experimental/continuous-elevation-64/tests/continuous_test.cpp -o /tmp/spatial-continuous-test
/tmp/spatial-continuous-test
```

Run `node tests/editor_test.cjs` from this experiment directory. Serve from the
repository root with `python3 -m http.server 8799 --bind 127.0.0.1 --directory
experimental/continuous-elevation-64/web`, then visit `http://127.0.0.1:8799`.
Publication is separate from this local build.

Vertical Disorientation movement is [staged next](docs/NEXT_VERTICAL_PASS.md),
with no vertical path implementation in this firmware.


## Reported hardware result (2026-10-12)

Adrian Vos reports: “tests pass including saving to card.” Alpha22's hardware
test protocol is recorded as passed, with Save to card explicitly confirmed.
No new callback/block peaks, run duration or detailed listening observations
were supplied; do not infer numerical headroom from this report. Firmware and
checksum remain unchanged. Alpha22 is the tested fallback for the staged vertical
Disorientation pass. Result record © 2026 Adrian Vos (soveda), MIT.
