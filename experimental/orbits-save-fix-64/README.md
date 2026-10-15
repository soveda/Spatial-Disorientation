# Repeated-save fix candidate — 0.3.0-alpha2

**Separate experiment; hardware validation pending. Beta1 remains the current beta.**

Original code, integration, tests and documentation ©2026 Adrian Vos(soveda),
[MIT](../../LICENSE). Measured ear filters: Bill Gardner and Keith Martin, MIT
Media Laboratory1994, *HRTF Measurements of a KEMAR Dummy-Head Microphone*,
Technical Report280, under [separate data terms](../../vendor/KEMAR/SOURCE_TERMS.md).
Preserve [source attribution](../../THIRD_PARTY_NOTICES.md), [firmware notices](uf2/NOTICE.txt)
and the existing ComputerCard, Workshop_BlockAudioCard, WaveSeq/EightMU/rppicomidi,
PicoSDK/TinyUSB/runtime licenses. No new external code or measurements are introduced.
Musical inspiration: Neuzeit Quasar; independent implementation.


## Save-failure investigation and first retest

User reports Orbits height/slope generally works, though slope is subtle. Repeated
Save first stopped responding while audio continued; another attempt stopped audio
and showed rapid movement resembling CV with no CV patched. Warning LED stayed off.
Old code suppresses normal timing diagnostics while saving, so absence of that LED
does not establish a safe save or distinguish hardfault from sampler desync.

The old save locks out core0 while ADC/DMA keep running. This candidate stops the
sampler cleanly before lockout, then restarts FIFO/round-robin/DMA in a known state.
See [local ComputerCard0.4.0 patch](vendor/ComputerCard/PATCHES.md); Chris Johnson's
original MIT notice remains intact. Root vendor, beta and alpha1 are unchanged.
This is an evidence-led fix candidate, not proof that the reported failure is fixed.

1. Back up old presets. Flash alpha2 and use its matching editor (alpha1 editor is
   protocol-compatible too). Boot Orbits; height0°,tilt40°,slow X; verify normal sound.
2. With no CV patched, vary height/tilt/bearing and use Apply then Save20–30times,
   waiting for each reply/read to complete. Audio should briefly fade for changed
   writes, resume, and remain controllable. Live geometry must not jump as if false
   CV were present. Check Main/X/Y/source mapping after each few saves.
3. Save identical settings10times: these should not interrupt audio or erase flash.
   Do not deliberately power off during writes. If an error occurs, record the editor
   message, whether audio/knobs still work and whether bottom-rightLED lights.
4. Save several different presets and reset/read each one. Check Orbits height/slope,
   Mixer placements/heights and Disorientation centre/bob/slope independently.
   Repeat a shorter changed-save test in Mixer/Disorientation. No settings-format
   change: SysEx7/JSON7/flash8 stay compatible with alpha1.
5. Repeat audio-rateCV stress and a20–30minute clean audio run. Check separate8mu boot
   after saving; mappings/DSP remain alpha1. No queue warning or phantom controls.

Save waits for mute, actual hardware pause and actual restart, with bounded waits.
All failure paths release pause/mute requests and return status5. Failed save/resume
latches a general fault for the bottom-rightLED once the ISR is running; it cannot
promise an LED indication if the core/ISR itself has stopped. No retry is hidden.
Unchanged settings skip fade/flash. Reset clears the latched save fault. A successful
acknowledgment means verified settings and sampler restart; it is not a lifetime
flash-endurance guarantee. Two seconds between manual saves is a sensible retest
pace; this is not an autosave feature.

Reset to init intentionally sets Orbits height0°/tilt0°, removing slope. Restore
height0°/tilt40° and Apply before judging audibility; reset also restores room,
levels and separation. This candidate does not strengthen the elevation DSP.

## Original Orbits experiment controls

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
cmake -S experimental/orbits-save-fix-64 -B /tmp/spatial-orbits-alpha2 -DPICO_SDK_PATH=/path/to/pico-sdk -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/spatial-orbits-alpha2 -j2
python3 -m http.server 8806 --bind 127.0.0.1 --directory experimental/orbits-save-fix-64/web
```

[Download experimental UF2](uf2/Spatial_Disorientation_0.3.0-alpha2-orbits-save-fix64-DUAL.uf2) / [SHA256](uf2/SHA256SUMS.txt).

Visit `http://127.0.0.1:8806` in Chrome/Edge. Direct local HTML may also work with
WebMIDI permissions. No publication is implied by this local experiment.
Run `node tests/editor_test.cjs` from this experiment directory. Firmware notices
and SHA256 accompany the UF2. Build/host evidence is recorded below after checks;
host timing does not predict RP2040 cost.


Final candidate build: PicoSDK2.3.0/ArmGNU15.2.Rel1, flash144300B, RAM185104B,
scratch2048B each. Save parser/storage tests execute the actual firmware classes
with hardware mocks:1000 changed saves,100 unchanged no-write saves, timeout at
mute/pause/resume, lockout failure, verification corruption, explicit error replies,
request cleanup and successful retry. Mocks assert flash is touched only while
sampling is paused; they do not simulate real ADC alignment or validate hardware.
Twenty unchanged DSP/settings/control regressions and the actual editor lifecycle
suite pass ASan/UBSan/simulatedMIDI. DSP headers are byte-identical to alpha1.
Compiler per-function stack reports include UsbEditor::Run216B, Message272B,
Storage::Save112B and RenderBlock168B; these are not complete worst-case stack
usage measurements. The important new physical retest is repeated Save with clean
sampling after every flash operation.

The user later confirmed Read did not retain newly applied height/tilt after init,
and hardware reset restored the effect. This may be another consequence of the
failed sampling/session state; it is not assumed to be mere reduced audibility.
The actual firmware parser/SPSC handoff now has an init→Apply→Read regression.
The experimental editor verifies Orbits height/tilt readback after init/Apply and
keeps desired edits staged with an explicit error if the card returns old values.
The editor regression simulates a falsely acknowledged Apply to verify this case.
This does not claim a separate init firmware bug was reproduced or fixed.
