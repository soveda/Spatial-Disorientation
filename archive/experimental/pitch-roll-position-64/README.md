# Direct pitch/roll positioning — 0.4.0-alpha1-pitch-roll

Separate alpha based on the user-passed **0.3.0-beta2**. Beta2 source, editor and
UF2 remain intact. Host checks and firmware build pass; physical 8mu, listening
and timing validation of this mapping are pending.

Original code, tests, editor and documentation ©2026 Adrian Vos (soveda),
[MIT](../../../LICENSE). Uses the existing WaveSeq/EightMU-derived pickup integration
and Chris Johnson's ComputerCard0.4.0 with the attributed flash-pause adaptation;
[third-party attribution](../../../THIRD_PARTY_NOTICES.md),
[driver patch](vendor/ComputerCard/PATCHES.md), [firmware notices](uf2/NOTICE.txt).
Ear filters derive from Bill Gardner and Keith Martin, MIT Media Laboratory1994,
*HRTF Measurements of a KEMAR Dummy-Head Microphone*, Technical Report280;
[separate measurement terms](../../../vendor/KEMAR/SOURCE_TERMS.md) remain applicable.
Neuzeit Quasar is musical inspiration, not a code/data source. No new external
code/data: the integer directional lookup uses this project's existing atan table.

## Motion mapping

**B** toggles motion; it starts off. **A** captures the held pose as neutral and
captures Main as the anchor, as well as resetting phase. Pitch controls front/back;
roll controls left/right. Yaw/angular rate is ignored. These are gravity-component
readings, not Euler angles in degrees. Sensor polarity follows EightMU: lifting
the back edge gives positive pitch; lifting the right edge gives positive roll.
Keep the unit approximately flat when capturing neutral for useful travel both ways.

### Twin Orbits

Tilt acts as a directional pointer around the captured Main anchor. Relative to
that anchor, back-edge-up points front, right-edge-up points right, front-edge-up
points back and left-edge-up points left; combined tilt gives intermediate angles.
Holding a pose holds that direction without accumulating rotation. Near neutral,
the last direction is held to avoid an undefined/noisy angle. **Fader8 sets tilt
sensitivity**: lower settings require more tilt to leave the neutral region; zero
returns to the captured Main anchor. Above the deadzone it does not reduce angular
travel because direction comes from the ratio of the two axes.

Automatic X orbit and CV still add movement. Centre X and remove CV to audition the
pointer alone. Held C freezes automatic orbit but permits tilt positioning. Pitch/
roll place the sources along the configured inclined orbit; they do not directly
change saved height or slope. Normal/height pages, D tap/hold, fader pickup and
page LEDs remain beta2 behaviour. [Existing page guide](../orbits-mu-pages-64/README.md).

### Disorientation

Pitch directly changes Main's front/back depth around the captured value: positive
pitch moves towards front, negative towards back, bounded at the ends. Roll turns
the whole Fig8/Pendulum/Wander path left/right, approximately ±90° from flat at
full motion amount. Fader8 scales both pitch excursion and roll rotation; zero
returns depth to its anchor and path rotation to zero. Holding a pose holds its
contribution while the movement keeps running. A recaptures both axes and resets
path rotation. Turning motion off/disconnecting eases rotation back to zero; Main
waits for panel pickup as before. Existing world-space slope follows the rotated
path; path distance and vertical bob retain their existing behaviour.

C freezes movement phase; changing tilt can still move the held path. D tap cycles
movement and D hold shows timing; it does not switch Orbits control pages.

### Spatial Mixer

Motion remains unused. Fader8 still controls selected-source elevation. All other
fader, source-select and panel-pickup behaviour remains beta2 behaviour.

## Downloads and compatibility

[Alpha UF2](uf2/Spatial_Disorientation_0.4.0-alpha1-pitch-roll64-DUAL.uf2) ·
[SHA256](uf2/SHA256SUMS.txt) · [Matching editor](web/index.html).
[User-passed beta2 fallback](../../beta2/uf2/Spatial_Disorientation_0.3.0-beta2.uf2).

192MHz/1.15V,48kHz,64 taps,two sources,64-frame DSP blocks/128-frame transport
latency. SysEx7/JSON7/flash8 stay compatible with beta2 and 0.3alpha1–3. Beta2 editor
works; this editor describes the new motion mapping. No persisted settings change,
no motion autosave, no flash algorithm change. Live tilt/path rotation are not
saved. The passed ComputerCard ADC/DMA flash-pause/resume fix is unchanged.

## Hardware test protocol

Use separate boots for editor and 8mu. Reboot between timing comparisons because
peaks latch. Headphones and a broadband mono source are useful for positioning.

1. Boot with8mu in Orbits. Pick up fader1 and centre speed, remove CV, set useful
   distance/separation. Start approximately flat; press A then B to enable motion.
   Tilt front/back and left/right through the four directions above. Combined tilt
   should move smoothly around the head. Keep a non-neutral pose for30seconds:
   no continuing rotation from yaw drift. Twist about vertical while keeping the
   same tilt: it should not add movement.
2. Return near neutral: last direction holds, small jitter should not spin it.
   A recaptures pose/Main. Reduce fader8: a larger tilt is required to engage;
   zero returns to the captured anchor. Restore amount and verify all directions.
   Test B off/on, C hold/release, and X automatic orbit added to tilt.
3. Tap D to height page (8mu LEDs6–8 briefly), pick up height/tilt/uphill. Tilt should
   position the sources along that slope. Return normal (LEDs1–3); no page jump,
   pickup still required for faders1–3. Hold D>500ms: timing LEDs, no page change.
4. Boot Disorientation. A captures neutral at mid Main, B enables motion. Pitch
   should move front/back depth and roll should turn the whole path left/right.
   Test all three movements, steady poses, fader8 low/zero, A recapture, C freeze,
   B handback and disconnected panel pickup. With slope enabled, check rotation
   also follows the configured uphill bearing. Yaw must not accumulate movement.
5. Mixer regression: fader8 elevation, A/B selection, independent pickup; motion
   must have no effect. Reconnect should begin with motion off and fresh pickup.
6. Separate editor boot: Apply/Read, elevation randomise/reset and preset import/
   export remain consistent. Repeat changed Saves and hardware reboot to confirm
   saved settings and the save fix. No live motion should be stored.
7. Both sources, high room/strength, slope/elevation, rapid controls and audio-rate
   CV: run20–30minutes in each movement/role. Check audio for clicks/gaps, controls
   for stalls and Computer bottom-right for latched faults. Hold8mu D for offline
   timing bands; all flashing means warning, eight steady is merely the high band.
   Use a separate editor boot for exact peaks. Targets callback<18µs/block<1200µs;
   report duration, USBrole, peaks, warnings and listening results.

## Build and verification

```sh
cmake -S archive/experimental/pitch-roll-position-64 -B /tmp/spatial-pitch-roll-alpha1 -DPICO_SDK_PATH=/path/to/pico-sdk -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/spatial-pitch-roll-alpha1 -j2
python3 -m http.server 8808 --bind 127.0.0.1 --directory archive/experimental/pitch-roll-position-64/web
```

Use localhost8808 in Chrome/Edge for the matching editor. Build: PicoSDK2.3.0,
ArmGNU15.2.Rel1, flash147884B/RAM189808B, scratch2048B per core. Control mapping
runs at1kHz on core0; core1 still renders FIR/geometry and services bounded USB.
The directional lookup uses one bounded integer division; hardware timing must be
remeasured. Scene transport captures path rotation with each audio frame.

23 C++ host programs pass ASan/UBSan; editor script regressions pass. Added checks
cover independent atan2 reference, cardinal directions, neutral hold, held-pose/yaw
rejection, motion amount/recenter and180000 changing-rotation transport frames.
Existing page/pickup, save failure/resume, settings/preset migration and DSP/block
transport checks remain. These checks do not substitute for hardware validation.
