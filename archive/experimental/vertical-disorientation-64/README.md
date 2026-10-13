# Alpha23: vertical Disorientation movement

**0.1.0-alpha23-vertical-disorientation64-DUAL · experimental · hardware validation pending**

Original code, tests and documentation © 2026 Adrian Vos (soveda),
[MIT](../../alpha14/LICENSE). Preserve [third-party attribution](../../alpha14/THIRD_PARTY_NOTICES.md)
and full [firmware notices](uf2/NOTICE.txt). Ear filters derive from Bill Gardner
and Keith Martin, MIT Media Laboratory (1994), *HRTF Measurements of a KEMAR
Dummy-Head Microphone*, Technical Report #280, under
[separate measurement terms](../../alpha14/vendor/KEMAR/SOURCE_TERMS.md). ComputerCard,
Workshop_BlockAudioCard, WaveSeq, EightMU/rppicomidi and all existing adaptations
remain credited in those notices. Vertical paths are original mathematical
extensions; no additional external code or measurement dataset is introduced.

## Vertical paths

Disorientation now renders height movement for both sources, using their existing
independent phases and the tested alpha22 continuous elevation renderer:

| Movement | Vertical behaviour |
|---|---|
| Fig8 | One rise/fall cycle per traversal, lifting one lobe and lowering the other. |
| Pendulum | Height is lowest at the swing centre and highest at both ends, retracing the same arc. |
| Wander | Deterministic height targets on the same extended nodes and cosine easing as horizontal wandering. |

Editor **Height centre** spans −40° to +40°. **Vertical excursion** is a half-range,
0–40° around that centre, shared by both sources. Actual heights clamp to ±40°;
off-centre settings with large excursion flatten at the limits. Source separation
and linked/opposing traversal produce different A/B heights. Excursion zero holds
a fixed height; **centre 0° + excursion 0° preserves alpha22 horizontal audio**.
These are the defaults for new and migrated settings.

This adds elevation angle to the existing horizontal paths, not a new full 3D
Cartesian room/distance model. Distance and horizontal direction retain their
existing behaviour. The renderer interpolates Level and measured ±40° ear filters
with smooth coefficients and timing projection; perceived height is approximate
and listener-dependent. Broadband sounds on headphones are the main test.

## Editor and controls

Use [alpha23 DUAL firmware](uf2/Spatial_Disorientation_0.1.0-alpha23-vertical-disorientation64-DUAL.uf2)
with this [experimental editor](web/index.html). It requires **SysEx v5** firmware;
older editors are not compatible. Root/current and alpha22 editors are unchanged.

Hold Down on startup, choose Main's upper third (right LEDs), release to select
Disorientation. Set Height centre and Vertical excursion in the editor, then
**Apply** to audition. **Save to card** persists them; **Read** and JSON v5
export/import include them. Import stages edits; reconnect retains them until
Apply or Read. Reset to init returns this mode's centre to 0° and excursion to
zero without writing flash. Movement ID 13 still selects Fig8/Pendulum/Wander;
new IDs **16/17** are centre/excursion. Live telemetry includes rendered A/B height;
the existing overhead diagram still shows horizontal direction only.

Knobs, CV, pulses, linked/opposing movement, freeze and reset retain their current
roles. Holding panel Down or 8mu C freezes phase, including height; release both
to resume. D tap still cycles movements, D hold retains timing diagnostics.
8mu fader 8 **still controls motion amount** in Disorientation; vertical settings
come from the editor/saved bank in this pass. Mixer fader 8 remains continuous
selected-source height, with pickup. Mixer ignores motion. No height CV or new
8mu Save gesture is added. In a separate 8mu boot, saved vertical settings load
normally. 8mu does not write flash automatically.

Computer LED roles and 8mu movement/timing LEDs are unchanged; no additional height
LED display. Computer LED 5 remains the latched timing/queue fault indicator.

## Persistence and fallback

Flash **v6**, JSON **v5**, SysEx **v5**. Flash v1–v5 and preset v1–v4 migrations
preserve their supported settings, adding vertical centre 0° and excursion zero.
In particular, alpha22 Mixer saved heights remain intact. There are still separate
mode banks; changing Disorientation vertical settings does not change Mixer height.
Only explicit editor Save writes the new record, using the existing mute/lockout
handshake. Live phase/height is not saved; centre and excursion define the restarted
path. Normal boot still selects Twin Orbits.

[Alpha22 source, editor and UF2](../continuous-elevation-64/README.md) remain the
**tested fallback**; the user reports tests pass including Save to card on
2026-10-12. Back up alpha22 presets before saving with alpha23 if you expect to
restore them after downgrading: alpha22 cannot read flash v6 and starts defaults;
its editor cannot import JSON v5. No Workshop release or root firmware/editor is
changed by this experiment.

## Processing and verification

CPU remains **192 MHz / 1.15 V**, ComputerCard **0.4.0**, audio **48 kHz**, two
sources/all **64 taps**, **64-frame** blocks and **128-frame** transport latency.
Geometry and vertical path resolution stay on core 1 at staggered 32-sample
intervals per source. Core 0 captures centre/excursion with phase and audio, so
Apply remains aligned with captured frames. No HRTF/path mathematics is in the ISR.
Wander reuses its segment/easing and adds height hashing only when excursion is
nonzero. Fig8/Pendulum reuse fixed-point sine lookup. FIR kernel, smoothing,
delay reads, room response and geometry cadence are unchanged.

Moving intermediate heights keep coefficient blending active. Hardware peaks
must be measured; there is no headroom claim. Targets are callback <18 µs, block
<1,200 µs, no queue faults; the actual block deadline is 1,333.3 µs.

Pico SDK 2.3.0 / Arm GNU Toolchain 15.2.Rel1 build passes. Flash 139,424 bytes,
main RAM 179,092 bytes; scratch banks 2,048 bytes each. Packaged
[SHA256SUMS](uf2/SHA256SUMS.txt) and notices accompany the UF2.

Host tests with AddressSanitizer/UndefinedBehaviorSanitizer pass:

- 756,000 frame-exact alpha22 comparisons: horizontal/default Disorientation,
  Mixer height anchors, Orbits, transport reuse/starvation and dynamic scenes.
- Vertical phase/arc/bounds; deterministic Wander reversal across wraps,
  reset epochs and held phase; independently phased two-source paths.
- Sample-aligned block capture with per-frame centre/excursion changes.
- Flash v1–v5 migration/v6 roundtrip, parameter bounds and atomic malformed
  decode; existing Mixer/8mu pickup, DSP/room/block/mode/movement/startup/MIDI tests.
- Actual editor script with simulated MIDI: v5 vertical Apply/Read/Save/export,
  old file migration, mode isolation, offline staging and height telemetry.

These are host checks. Hardware listening, timing, persistence and stability
remain pending; follow [the hardware test protocol](docs/TEST_PROTOCOL.md).

## Rebuild / local editor

From repository root:

```sh
cmake -S experimental/vertical-disorientation-64 -B /tmp/spatial-vertical-disorientation64 -DPICO_SDK_PATH=/path/to/pico-sdk -DSPATIAL_ELEVATION_SOLO=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/spatial-vertical-disorientation64 -j2
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=address,undefined -DSPATIAL_HRTF_TAPS=64 -Iexperimental/vertical-disorientation-64/src experimental/vertical-disorientation-64/tests/vertical_test.cpp -o /tmp/spatial-vertical-test
/tmp/spatial-vertical-test
python3 -m http.server 8800 --bind 127.0.0.1 --directory experimental/vertical-disorientation-64/web
```

Visit `http://127.0.0.1:8800` in a Chrome-family browser; use a USB-C data cable and
allow MIDI/SysEx. Close Serial Monitor/other MIDI apps. Editor is self-contained;
iOS is unsupported. Run `node tests/editor_test.cjs` from the experiment directory.
No hosted alpha23 publication is implied by this local build.


## Reported hardware timings (2026-10-12)

Adrian Vos supplied the following alpha23 readings, all with 64 taps/two sources:

| Displayed path / height | Callback peak | Block peak | Warning |
|---|---:|---:|---|
| Fig8, A/B 0°; distances 4%/30% | 13 µs | 1,131 µs | None shown |
| Wander, A −32.1° / B −33.7°; distances 8%/23% | 13 µs | 1,209 µs | Slow DSP block |
| Pendulum, A −40° / B −23.1°; distances 25%/25% | 13 µs | 1,209 µs | Latched slow DSP block |
| Fig8, A −40° / B −8.8°; distances 43%/41% | 13 µs | 1,209 µs | Latched slow DSP block |

Callback peak is 5 µs below its 18 µs warning. Maximum reported block peak is
9 µs above the 1,200 µs warning and about 124 µs below the 1,333.3 µs processing
deadline. This reports reduced safety margin, not proof of missed playback. No
audio-queue warning is shown. Peak values and fault flags latch until reset;
identical later 1,209 µs readings cannot establish individual path peaks without
separate resets. The higher reading is first shown during Wander; it is not a
controlled isolated benchmark of that path.

Vertical movement is reflected in telemetry, but subjective sound, settings/run
duration, Save/reset and audio-abnormality results were not supplied. Do not
record the full protocol as passed. Recover timing headroom before implementing
the user's requested future selectable sloping paths. Firmware remains unchanged;
alpha22 stays the tested fallback. Record © 2026 Adrian Vos (soveda), MIT.
