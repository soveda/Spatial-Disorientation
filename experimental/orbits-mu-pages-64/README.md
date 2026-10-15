# Orbits 8mu pages and elevation tools — 0.3.0-alpha3

Separate experiment based on the **user-passed alpha2 save fix**. Root beta1,
alpha1 and alpha2 source/editor/UF2 remain intact. Hardware validation of this
new mapping is pending. No DSP, settings schema or flash-save algorithm change.

Original code, editor, tests and documentation ©2026 Adrian Vos(soveda), [MIT](../../LICENSE).
Uses the project's original mapping/pickup code adapted from WaveSeq/EightMU,
Chris Johnson, and rppicomidi; preserved [attribution](../../THIRD_PARTY_NOTICES.md),
[firmware notices](uf2/NOTICE.txt), [local ComputerCard0.4.0 patch](vendor/ComputerCard/PATCHES.md).
Measured ear filters: Bill Gardner and Keith Martin, MIT Media Laboratory1994,
*HRTF Measurements of a KEMAR Dummy-Head Microphone*, Technical Report280,
[separate measurement terms](../../vendor/KEMAR/SOURCE_TERMS.md). All dependency
and original MIT notices remain; no new external code/data. Neuzeit Quasar is
musical inspiration, not a source of firmware or transfer functions.

## 8mu Twin Orbits pages

Short **D press/release under500ms** toggles normal ↔ height/slope. Holding D at
least500ms shows timing bands; release of a long hold does not switch pages.
Normal page is selected at connection/startup; a D button already held on connection
must not create a switch on release. Orbits only: Disorientation D retains its
movement selection, and Mixer D remains diagnostics.

| Fader | Normal page | Height/slope page |
|---|---|---|
|1|Speed/direction|Height −40°..+40°|
|2|Distance|Tilt0..40°|
|3|Source separation|Uphill direction0..360°|
|4|Room|Room|
|5/6|A/B trim|A/B trim|
|7|Spatial strength|Spatial strength|
|8|Motion amount|Motion amount|

**Faders1–3 rearm pickup on each page switch.** Moving them to/through each target
engages control; initial values on another page cannot invent a crossing. Normal
speed/distance/separation retain their last remote-owned values on height page;
returning to normal does not jump to the physical fader positions. Height/slope
values similarly remain when leaving their page. Normal panel controls remain
available when not remotely owned; CV remains additive. Shared faders4–8 retain
ownership across page changes.

On page entry,8mu **LEDs1–3** briefly identify normal; **LEDs6–8** identify height
for1second, then normal pickup/level feedback resumes. Waiting faders blink.
Long-D timing diagnostics take priority over page notices. LED8 shows motion on;
held C flashes LED8 for freeze after the page notice. These are8mu LEDs, not the
Computer's six LEDs. The Computer bottom-right remains the timing/queue/save-fault
warning from alpha2.

A resets/recentres, B toggles motion takeover of Main, C holds automatic orbit.
Motion can still move position while C is held. Release C restores the previous
speed on either page. No new motion axis is assigned: fader8 still controls motion
amount and never becomes height. Gyro/roll continue to place the sources along
the inclined orbit.

Height/slope are **live overrides**, never flash writes. Disconnect restores saved
height/slope and disables motion; remotely owned X/Y retain their last values until
panel pickup. Reconnection starts on normal with fresh fader pickup. Changing
8mu pages does not change Disorientation/Mixer saved banks. Configure persistent
values with the editor and Save in a separate USB boot. No host Save gesture.

## Editor buttons at the bottom

[Matching experimental editor](web/index.html): **Randomise elevation** and
**Reset elevation** act on the displayed preset/function, including offline:

- Orbits: height, uphill direction, tilt.
- Mixer: independent A/B heights; no slope controls exist in Mixer.
- Disorientation: height centre, vertical excursion, uphill direction, tilt.

Both buttons **stage edits only**. Apply auditions; Save to card is explicit.
No MIDI message or flash write occurs when pressing either button. Unrelated
room, levels, strength, separation, placements and movement choice stay unchanged.
Randomise chooses legal values and constrains centre/bob/tilt combinations to avoid
immediate height clipping. Reset elevation sets heights tolevel0°, vertical
excursion/tilt0° and uphill direction front; it does not reset other sound settings.
Reset to init remains the separate full editor-settings reset. Orbits readback
verification from alpha2 remains in place after Apply/init.

## Compatibility and downloads

**192MHz/1.15V,48kHz,two sources,64taps,64frame blocks**. SysEx7/JSON7/flash8
unchanged from alpha1/2. Existing alpha1/2 presets and saved settings work; the
alpha3 editor is needed for the new buttons, and alpha3 firmware for8mu pages.
The alpha2 editor is protocol-compatible but has no elevation-tool buttons.
Root beta1 uses an earlier protocol/flash format: back up old beta presets before
any alpha Save; downgrade instructions are in the alpha2/beta operator guide.

[Alpha3 UF2](uf2/Spatial_Disorientation_0.3.0-alpha3-orbits-mu-pages64-DUAL.uf2) /
[SHA256SUMS](uf2/SHA256SUMS.txt). [Passed alpha2 fallback](../orbits-save-fix-64/README.md).
Core0 still captures all hardware and control snapshots; core1 renders geometry/
audio and nonblockingUSB. The alpha2 orderly ADC/DMA pause, flash lockout, verified
resume, unchanged-save fast path and save-fault indication are preserved.

## Hardware test protocol

Editor and8mu must be tested in separate boots; no simultaneous connection required.

1. Editor boot: select Orbits, verify versionalpha3. Try Randomise elevation and
   Reset elevation. They should update fields without changing sound until Apply.
   Reset restores height0°/tilt0° while other settings stay. Apply/Read/Export/Import
   should agree; repeat for Mixer and Disorientation. Saved settings restore after
   reset. Repeat several changed Saves to check the passed save fix remains sound.
2. Save useful Orbits settings, boot with8mu. Normal page: pick up speed/distance/
   separation and set a slow orbit, audible separation and distance. Tap D: expect
   LEDs6–8 briefly lit, no sound/control jump. Speed/distance/separation should hold.
3. Height page: move fader1 near centre and through saved height, fader2 through
   saved tilt and fader3 through saved bearing to pick up. Sweep height and tilt;
   fader3 rotates the uphill direction. Both sources remain bounded±40°; combined
   centre+tilt can clip at extremes. Use height0°/tilt40° initially for listening.
4. Tap D back: LEDs1–3 briefly lit. Faders1–3 must wait for the retained normal
   values before taking over. Switch repeatedly at widely different fader positions;
   no speed/distance/height jumps merely from switching. Faders4–8 work on both pages.
5. Hold D>500ms on each page: timing bands; releasing must keep the page. Eight
   steady LEDs is the high timing band, not a warning; all flashing means a latched
   warning. Tap/release distinguishes page selection. Connect with D already held;
   releasing should leave normal page selected.
6. On both pages test A reset/recentre, B motion, C hold/resume and fader8 motion
   amount. C release restores prior speed, not a frozen centre value. Motion/knob/CV
   positioning should follow the slope. Unplug8mu: saved elevation returns, motion
   stops and panel X/Y wait for pickup. Reconnect normal; verify no inherited jumps.
7. Regression: Mixer fader8 height/A/B pickup; Disorientation D tap movement/D hold
   timing and existing mappings. They must not acquire Orbits page behaviour.
8. Both sources, highroom/strength, rapid controls/CV, continuously moving intermediate
   height/tilt/bearing. Test20–30minutes, then reset and compare normal/height pages.
   Targets callback<18µs/block<1200µs, no queue/save faults or clicks/gaps/phantomCV.
   Report peaks, run duration, USBrole, pickup/motion/page behaviour and audibility.

## Build and host checks

From repository root:

```sh
cmake -S experimental/orbits-mu-pages-64 -B /tmp/spatial-orbits-alpha3 -DPICO_SDK_PATH=/path/to/pico-sdk -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/spatial-orbits-alpha3 -j2
python3 -m http.server 8807 --bind 127.0.0.1 --directory experimental/orbits-mu-pages-64/web
```

Use `http://127.0.0.1:8807` in Chrome/Edge. No hosted publication is implied.
Run `node tests/editor_test.cjs` from this experiment folder. Saved settings and
all DSP files remain alpha2-compatible; actual hardware load needs retesting.


Final build: PicoSDK2.3.0/ArmGNU15.2.Rel1, flash147108B, RAM187968B,
scratch2048B each.22C++host programs pass ASan/UBSan, including the new two-page
mapping/pickup/held-value/reconnect/D-hold/A/B/C stress test and existing save-failure,
settings/migration,1024000HRTF,756000scene/transport,180000Orbits default and180000
changing-height transport comparisons, smoothing/control/queue regressions.
Actual editor script tests100randomisations per mode, bounds/reset, unchanged
non-elevation values, no-send staging, Apply/Read, saved/preset migration and stale
readback rejection. All DSP headers and alpha2 local ComputerCard header are
byte-identical. No new hardware timing or physical8mu result is claimed.
