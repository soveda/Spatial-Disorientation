# Twin Orbits height and slope — 0.3.0-alpha1

**Separate experiment; hardware validation pending. Beta1 remains the current beta.**

Original code, integration, tests and documentation ©2026 Adrian Vos(soveda),
[MIT](../../LICENSE). Measured ear filters: Bill Gardner and Keith Martin, MIT
Media Laboratory1994, *HRTF Measurements of a KEMAR Dummy-Head Microphone*,
Technical Report280, under [separate data terms](../../vendor/KEMAR/SOURCE_TERMS.md).
Preserve [source attribution](../../THIRD_PARTY_NOTICES.md), [firmware notices](uf2/NOTICE.txt)
and the existing ComputerCard, Workshop_BlockAudioCard, WaveSeq/EightMU/rppicomidi,
PicoSDK/TinyUSB/runtime licenses. No new external code or measurements are introduced.
Musical inspiration: Neuzeit Quasar; independent implementation.

## Change

Twin Orbits now has its own saved **height**, **uphill direction** and **path tilt**.
Both independently phased sources use the tested continuous elevation renderer and
same SlopePath geometry as Disorientation. Height offsets both sources; slope is
resolved from each actual orbit position, including Main/CV offset and separation.
Linked/opposing motion, reset/clock, speed and distance remain unchanged.

| Setting in Twin Orbits | Range / default |
|---|---|
| Height centre, ID16 | −40°..+40° /0° |
| Uphill direction, ID18 |0°front,90°right,180°back,270°left; continuous /front |
| Path tilt, ID19 |0°..40° /0° |

Tilt rises toward uphill and falls toward the opposite bearing; perpendicular
positions stay at centre height. The horizontal projection also changes, while
path distance remains unchanged. Centre+tilt clips at±40°, so high centre/tilt
can flatten peaks. There is **no separate orbit bob oscillator** in this pass.
Height0°/tilt0° reproduces beta1 Orbits exactly. Tilt also applies to stationary
positions: X centre stops phase but Main/CV can still move around the inclined path.

The experiment changes only its own source/editor/firmware. Root beta1, its editor,
UF2 and tester bundle, archived versions and Workshop release remain intact.
Mixer/Disorientation controls and renderer semantics remain unchanged. Both sources,
64taps,192MHz/1.15V,48kHz,64frame block/128frame transport and core ownership remain.
No FIR work moves into the callback; new slope math runs at staggered geometry rate.
Timing margin must be measured again; beta Wander's1193µs is not this build's result.

## Editor and persistence

Use [the separate experimental editor](web/index.html) with this UF2. Changes wait
for **Apply**. Orbits defaults/Reset to init are level/zero tilt. Read/Apply/Save and
JSON exports include the three new settings; inactive mode banks are preserved.
The existing beta editor cannot control protocol7 firmware. The experimental
editor rejects earlier protocol versions clearly.

**SysEx7 / JSON7 / flash8**. Imports old JSON1–6 and reads flash1–7, preserving old
supported settings and defaulting the new Orbits height to level/tilt to zero.
In particular beta1 Mixer heights and Disorientation centre/bob/slope survive.
New IDs are scoped to each mode; changing Orbits height/slope does not change
Disorientation values. There is no autosave or boot migration write.

Export beta1 presets for all modes before experimental **Save to card**. Apply-only
leaves beta flash unchanged. After flash8 Save, beta1 cannot read the new record
and starts defaults; restore old exports with the beta1 editor and save each mode.
Beta1 also cannot import JSON7. Returning to beta1 requires its matching editor.
Firmware fallback: [beta1 UF2](../../uf2/Spatial_Disorientation_0.2.0-beta1.uf2).

8mu mappings are unchanged: fader8 remains Orbits/Disorientation motion amount,
Mixer selected-source height. No new height/tilt fader is assigned. Configure/save
Orbits height/tilt in an editor boot, then boot separately with8mu. Motion/Main/CV
position offsets follow the sloping orbit. No8mu gesture writes flash.

## Hardware protocol

1. Back up beta presets. Flash the experimental UF2, boot normally into Orbits,
   use its editor and Read. Older saved settings should retain room/levels/etc
   and start with Orbits height0°,tilt0°. Disconnect/reconnect if switching editors.
2. At height0°/tilt0°, repeat beta1 panel/CV/clock/reset checks. Sound should match
   beta1. Record independent callback/block peaks under the same CV/audio load.
3. X centred, tilt0°: audition height−40°,0°,+40° and intermediate values. Listen
   with both sources and one at a time. No clicks/gaps; centre height applies to
   both, with left/right position retained.
4. Height0°,tilt40°,slow X: Apply uphill front, right and diagonal45°. Each source
   should rise as it approaches uphill and fall opposite. First linked, then
   opposing; alter separation so the sources reach different heights. Listen
   without watching the diagram after confirming settings. Source distance
   should keep responding normally.
5. Stop with X centred; Main and CV1 should place the source along the inclined
   path. Reverse X and check retracing. Down/A/Pulse2 reset to the reset position's
   appropriate height, which need not be level. Check Pulse1 clocked operation.
6. Add height offset and tilt together; rendered heights remain within±40°. Clip
   flattening at extremes is expected. No settings should affect Mixer or change
   saved Disorientation height/slope. Verify all startup modes.
7. Test Apply/Read, init without Save, preset Export/Import, old beta preset import,
   and optionally Save/reset/Read after backup. New Orbits settings should restore
   independently of Mixer and Disorientation. Normal boot is still Orbits.
8. Both audio sources, high room/strength, rapid/audio-rate CV and moving
   intermediate height: compare zero tilt with40°tilt. Reboot between runs to
   clear latched peaks. Run the heaviest setup20–30min. Record duration, callback/
   block peaks, LED/warnings and audible results. Targets<18µs callback/<1200µs
   block; block deadline1333.3µs. Queue faults or audio gaps indicate failure.
9. Separate8mu boot after editor Save: check normal faders/pickup, B motion, A
   recenter/reset, C held phase freeze and D diagnostics. Frozen phase can still
   move via motion/CV. Ensure fader8 still affects motion amount, not height.
   Also regression-check Mixer fader8 and Disorientation D movement selection.

Report height/tilt/bearing, linked/opposing, separation, audio/CV/USB setup,
callback/block peaks and duration. An acceptable listening/timing result is
needed before changing beta1 or publishing this as the next beta.

## Build and verification

Build from repository root:

```sh
cmake -S experimental/orbits-height-64 -B /tmp/spatial-orbits-alpha1 -DPICO_SDK_PATH=/path/to/pico-sdk -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/spatial-orbits-alpha1 -j2
python3 -m http.server 8805 --bind 127.0.0.1 --directory experimental/orbits-height-64/web
```

[Download experimental UF2](uf2/Spatial_Disorientation_0.3.0-alpha1-orbits-height64-DUAL.uf2) / [SHA256](uf2/SHA256SUMS.txt).

Visit `http://127.0.0.1:8805` in Chrome/Edge. Direct local HTML may also work with
WebMIDI permissions. No publication is implied by this local experiment.
Run `node tests/editor_test.cjs` from this experiment directory. Firmware notices
and SHA256 accompany the UF2. Build/host evidence is recorded below after checks;
host timing does not predict RP2040 cost.


Final build: PicoSDK2.3.0 / ArmGNU15.2.Rel1, flash142880B, mainRAM183652B,
scratch2048B each. All20 C++host programs pass ASan/UBSan, including180000
beta-exact Orbits frames at level/zero tilt,180000 changing-height/slope transport
frames, axes/clipping/held positions, flash8 and wire7 roundtrips with independent
mode banks, beta flash7 migration and rejection. Existing1024000 directHRTF and
756000 scene/transport comparisons, vertical/Mixer/control/clock/8mu/smoothing/
room/queue tests pass. Actual editor lifecycle tests pass with simulatedMIDI,
including legacy preset migration, Orbits-specific Apply/Save/Read/Export/Import
and malformed rejection. These are host/build checks; hardware timing, listening,
stability and saved-setting behaviour still require the protocol above.
