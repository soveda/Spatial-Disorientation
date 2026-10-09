# Spatial Disorientation

**0.1.0-alpha11**: separate saved mode settings and Mixer placement presets, using the 32-tap HRTF and room externalization prototype for Music Thing Modular Workshop
Computer. Two independent mono inputs can orbit around the listener or stay at
individual placements, mixed to binaural stereo. Listen on headphones. Uses ComputerCard **0.4.0**, 48 kHz audio and
**192 MHz / 1.15 V**. Firmware builds and host checks pass. User hardware tests
on **alpha1** pass for the main controls, left/right movement and editor/persistence;
front/back cues were weak. Alpha1 stability is ongoing (20 minutes without issues
reported on 2026-10-08). Alpha2 improved tonal distinction but front/back movement remained insufficient
with the visual display hidden. Alpha3 overran the hardware callback: instant timing LED, uncontrolled fast
motion and unresponsive knobs. **Do not use alpha3 for continued testing.** Alpha4
moved HRTF DSP out of that callback. User readings on alpha4 were 5 µs callback
and 1,181 µs/block, with front/back localization still very subtle. Alpha5 user readings were 5 µs callback / 916 µs block at 32 taps, and
5 µs / 1,244 µs at 64 taps. The longer bank did not noticeably improve placement;
32 taps gave some directional difference but remained close to the head. Alpha6
returns to 32 taps and changes the room cues. The user reports 5 µs callback /
1,015 µs block, less inside-head sound, clearer circling and no echoes. The user accepted 13 minutes with rapid CV changes as a stability pass. Alpha7 reduces free-running X speed to 75% of alpha6
(maximum 1.5 turns/sec); clock-driven rates and the alpha6 DSP remain unchanged.
The user reports alpha7 and alpha8 8mu tests pass. Alpha8 adds direct 8mu
control. The editor now supports preset import/export (2026-10-09); this update
uses the tested alpha8 firmware without a reflash. The user reports preset tests pass.
Alpha9 adds the boot selector and Spatial Mixer; Disorientation is reserved by
user request. The user reports the Spatial Mixer test protocol passes (2026-10-09):
peak callback/block readings **7 / 1,021 µs** and **8 / 996 µs**, both at 32 taps.
Both are below the 18 µs callback / 1,200 µs block warning thresholds.
Separate alpha9 8mu and full Twin Orbits regression results are not reported.

Original code and documentation © 2026 Adrian Vos (soveda), [MIT](LICENSE).
Third-party code and measurement data retain their own notices and terms; the
project license does not relicense them. See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md)
for pinned sources, file mappings and the preserved license texts.

| Source | Use here | License / terms |
|---|---|---|
| Chris Johnson: ComputerCard 0.4.0 and web_interface | Hardware API, example build/core split, adapted USB descriptors and editor transport | [MIT notice](vendor/ComputerCard/LICENSE) |
| Chris Johnson: WaveSeq / EightMU; rppicomidi USB MIDI host driver | Adapted direct 8mu host implementation, USB role setup; pickup/file UI examples reviewed | MIT notices retained in [EightMU.h](vendor/EightMU/EightMU.h); [source/change record](vendor/EightMU/SOURCE.md) |
| Adrian Vos and contributors: Workshop_BlockAudioCard | Adapted 64-frame ring/two-block scheduling pattern | [MIT notice](vendor/Workshop_BlockAudioCard/LICENSE) |
| Adrian Vos: You spin me round | Reused mathematical sine table | [MIT notice](vendor/YouSpinMeRound/LICENSE) |
| Raspberry Pi Pico SDK / TinyUSB | Firmware platform and USB stacks | [BSD-3-Clause](vendor/PicoSDK/LICENSE.TXT) / [MIT](vendor/TinyUSB/LICENSE), plus [USB copyright notices](vendor/TinyUSB/NOTICE.md) |
| Bill Gardner and Keith Martin, MIT Media Laboratory (1994) | Derived horizontal KEMAR HRTF coefficients and rendered previews | [Dataset attribution terms](vendor/KEMAR/SOURCE_TERMS.md), separate from the MIT software license |

Musical inspiration: [Neuzeit Instruments Quasar](https://www.neuzeit-instruments.com/Quasar).
This is an independent implementation; no Quasar DSP, manual text or graphics are
used. The copied upstream AI directive retains upstream ownership and is excluded
from this project's MIT grant. Keep LICENSE, THIRD_PARTY_NOTICES.md and the referenced
vendor notices/measurement terms with redistributed source or firmware packages.

## Try it

First flash `uf2/Spatial_Disorientation_0.1.0-alpha11-mode-presets.uf2` using the usual
Workshop Computer BOOTSEL procedure. Start with a mono sound in Audio 1, both audio
outputs connected to the left/right sides of a headphone monitoring path, Main and
X at noon, Y down. Turn X right to start an orbit; turn left for reverse motion.
Patch a different sound into Audio 2 to hear the second source.

### Twin Orbits (normal boot)

| Control | Behaviour |
|---|---|
| Main | Full-turn position offset; noon puts A at the front after reset |
| X | Signed orbit speed: centre deadband stops; either extreme approaches 1.5 turns/sec |
| Y | Near to far: quieter, darker, later, with more room contribution |
| Switch up | Linked motion; separation stays constant |
| Switch middle | Opposing motion; A and B rotate in opposite directions |
| Switch down | Reset orbit phase once per press; retains previous motion relationship while held |
| CV 1 | Adds position offset; approximately ±6 V adds ±one full turn |
| CV 2 | Adds distance; approximately ±6 V spans the knob's full range, clamped at either end |
| Pulse 1 | Clock: default 4 pulses/turn; X still selects direction or stop |
| Pulse 2 | Rising edge resets orbit phase |
| Audio 1 / 2 in | Independent sources A / B; unplugged inputs are silent |
| Audio 1 / 2 out | Left / right binaural mix |
| CV / pulse outputs | Zero / low |

X's stationary region is approximately ±6% of its travel around noon. Clock lock
requires two edges; accepted edge intervals are 25 ms–60 s. Speed is capped at
2 turns/sec. After no clock for the greater of 3 seconds or 3 clock intervals,
manual X speed resumes. Unpatching the clock also restores manual speed. Use pulse
widths of at least 1 ms for hardware testing. Reset restores the phase relative to
Main and the editor's separation, and motion resumes immediately. CV values are
nominal, not calibrated position/distance measurements.

Top LED pair shows A's left/right position; middle pair shows B. Equal brightness
means front **or** back, not necessarily stopped. Bottom left is lit for linked
motion. Bottom right latches for a callback of at least 18 µs, a DSP block of at least
1,200 µs, or an audio queue overrun/underrun. Reset clears it. The editor displays
peak callback and block times and the warning category. Callback timing excludes
the framework's surrounding ISR work; measure the full ISR before release.

## Editor

Open `web/index.html` in desktop Chrome/Edge. If local file MIDI access is blocked,
serve it from localhost:

```sh
python3 -m http.server 8793 --bind 127.0.0.1 --directory web
```

Visit http://127.0.0.1:8793. Use a USB-C data cable; close Serial Monitor/other apps.
Enable MIDI and allow SysEx, select **Spatial Disorientation** as both MIDI ports,
then Connect & read. iOS is unsupported. The page is a single static file with no
external scripts or services.

- **Reset to init:** immediately apply factory editor settings and refresh the page
  values. Use Save to card separately to retain these defaults across reset.
- **Read:** fetch applied settings, replacing edits in the page.
- **Apply:** audition separation, room amount, A/B levels, spatial strength and
  clock pulses/turn. Main/X/Y keep their own roles; no editor/knob takeover needed.
- **Save to card:** persist applied settings across reset. Audio briefly fades out
  during the flash write. Apply pending edits first; Save is disabled while dirty.

Defaults: 180° separation, 29% room, 50% level per source, full spatial strength,
4 clock pulses/turn. Level controls provide headroom; their maximum is not an
automatic loudness compensation. A corrupt or absent record restores defaults.
Only explicit Save writes flash; the last 4 KB of the actual card capacity is
reserved. Reflashing may replace or invalidate saved settings; Read afterwards.

Twin Orbits and Spatial Mixer run in alpha9. Disorientation is a reserved startup
slot, with silent output and steady right-column LEDs. Elevation remains planned.
Alpha11 saves separate editor settings for each working mode and Mixer source
positions/distances/levels. Startup choice is never saved; normal boot is Twin Orbits.

## Alpha9 startup and Spatial Mixer

Hold the switch **Down while resetting/powering up**, then keep it held while
turning Main. Its lower third selects **Twin Orbits** (all LEDs off), middle third
**Spatial Mixer** (all three left LEDs), upper third **Disorientation** (all three
right LEDs). Release to confirm. Audio is muted during selection. Small hysteresis
prevents flickering at the boundaries. Boot without Down always selects Twin
Orbits, irrespective of Main or saved settings. Selection is fixed until reset.
Disorientation is reserved for later: confirming it leaves audio muted and the
right LEDs lit; reset to choose a working function. USB role selection is separate.

In **Spatial Mixer**, the sources stay where you put them:

| Control | Behaviour |
|---|---|
| Switch Up | Select source A |
| Switch Down, then release to middle | Select source B; middle retains the last selection |
| Main | Selected source position; noon front, minimum/maximum back |
| X | Selected source distance, near to far |
| Y | Selected source level, silence to full panel level |
| CV 1 / CV 2 | Modulate both stored positions / distances; the same nominal scale as Twin Orbits |
| Pulse inputs | Unused; no clock movement or phase reset |
| Audio inputs / outputs | Same two sources and binaural stereo mix as Twin Orbits |

Mixer starts with saved placements. Without a valid saved record, both start
near at full panel level, A front and B back. Panel controls require pickup of
those placements; their boot positions do not overwrite the scene. Up selects A on entry; releasing
the startup gesture to middle starts with A. Each source retains its three values
until another placement is applied or restored. After switching sources, move each knob to or through that source's
stored value to pick it up; tolerance is about 1.6% of travel. The selected top
(A) or middle (B) LED pair blinks until all three controls are picked up, then shows
position steadily. Bottom left is on for editing A and off for B. Bottom right
retains the timing warning. CV modulation never overwrites stored panel values.

Editor A/B levels act as trims multiplied by the individual Y levels. Room and
spatial strength remain shared. Separation and clock division are unused and
disabled in the Mixer editor, but retained for Twin Orbits. Reset to init resets
the active mode, including Mixer placements. Apply/Save affects only the active
mode’s settings; the other mode is retained. See the alpha11 section below.

### Alpha10: Spatial Mixer with 8mu

Connect the 8mu before reset. Hold Down through startup, select Mixer with Main
(left-column LEDs), then release. This works without a web editor connection on
Rev 1.1 hardware; earlier boards retain device/editor operation. USB host setup
and identify/default-bank behavior are unchanged from alpha8.

| 8mu control | Spatial Mixer behavior |
|---|---|
| Fader 1 | Selected source distance (X) |
| Fader 2 | Selected source panel level (Y) |
| Fader 3 | Selected source position (Main): noon front, minimum/maximum back |
| Fader 4 | Shared room reflections |
| Fader 5 / 6 | Source A / B level trims |
| Fader 7 | Shared spatial strength |
| Fader 8 | Unused |
| Button A / B | Select source A / B |
| Button C | Unused |
| Button D, held | Show peak block-time bands; flashing all LEDs means a timing/queue warning |
| Accelerometer / gyro | Unused; moving the 8mu does not change Mixer sound |

Faders 1–3 pick up the selected source's stored distance/level/position, rather
than the physical knob locations. Switching sources rearms those three pickups;
the other source stays put and faders 4–7 retain control. Move a fader to or through
its stored value. Fader LEDs 1–7 blink while waiting and show level after pickup.
**LED 8 is steady for A and blinks for B**; it is a source indicator, not fader-8
pickup. Card bottom-left LED also identifies A (on) or B (off).

Panel selection works on switch gestures: Up selects A; Down selects B; middle
retains the last selection. A switch left Up does not override an 8mu B selection;
move it away and back Up to select A again. The latest selection gesture wins;
if panel and 8mu select simultaneously, the 8mu button takes precedence.
While a fader owns a control, its corresponding panel knob is inactive. Disconnect
returns that control to panel pickup, preserving the source placement until the
knob reaches its stored value. Shared editor trims return to saved settings on
disconnect, as in Twin Orbits. Replug may need reset if enumeration fails.

No motion takeover/recenter is used in Mixer. Twin Orbits retains its alpha8
motion/button mappings. Alpha10 kept shared settings and volatile placements; alpha11 adds the
mode-specific save/preset behavior below. The user reports alpha10 Mixer 8mu
tests pass; no new timing readings were supplied. The user-tested alpha9 Mixer UF2 is retained as fallback.

Original selector/mixer code and guide: Adrian Vos (soveda), 2026, MIT. Source
selection/pickup patterns were reviewed in Chris Johnson's WaveSeq; renderer,
ComputerCard and measured HRTF attribution remain in THIRD_PARTY_NOTICES.md.

## What it sounds like

Each source has fractional arrival delays, measured direction-dependent ear
filters, distance attenuation and short feed-forward room reflections. The
horizontal filters derive from Gardner and Martin's diffuse-field-equalized MIT
KEMAR data. The short minimum-phase filters replace alpha2's rear notch, panning
law and direction-dependent one-pole shadow filter; interaural delay remains
analytic. Mild distance darkening retains more high-frequency directional detail.
No elevation or individual ear calibration is implemented. Generic short HRTFs
may still produce front/back confusion; test by ear with the display hidden.

The original alpha1 previews in `previews/` use the actual alpha1 C++ DSP with original synthetic sources:
a 220 Hz harmonic tone on A and noise percussion on B. Both are 12 seconds, stereo
48 kHz/16 bit, with identical levels, settings and source signals. One uses linked
motion and one opposing motion. They simulate DSP only, without the physical
ADC/DAC, USB activity or hardware timing. Later version-labelled files use that
version's renderer. Generation source: `tools/render_preview.cpp`.

## Alpha11: separate settings, placement save and presets

Reflash alpha11 **and reload the updated editor**. SysEx now uses version 2;
older editors/firmware are not compatible with this page. Existing v1 JSON preset
files still import. The startup gesture and normal Twin Orbits default are unchanged.

Each mode keeps its own separation, room, A/B trims, strength and clock division.
Mixer additionally keeps A/B position, distance and panel level. Separation/clock
remain unused in Mixer. The editor shows six extra placement sliders in Mixer.
Applied/imported/restored placements rearm panel and source-fader pickup. Main/X/Y
and 8mu can edit them after pickup. CV modulates the placements without being saved.

- **Read** captures the active mode’s applied base settings and current Mixer
  placements. Use it before export to capture what the physical knobs are doing.
- **Apply** auditions the displayed settings and, in Mixer, both displayed
  placements. Read first if you want to keep panel edits while changing room/trims.
- **Save to card** captures current Mixer placements plus applied base settings,
  retaining the inactive mode’s settings. It briefly fades audio, writes flash and
  refreshes the editor. Unsaved page edits must be applied first.
- **Reset to init** resets only the active mode, including Mixer placements.
  Use Save separately to retain the reset.
- **Export preset** downloads the displayed values, including unapplied edits.
  It works offline and does not Read or Save automatically.
- **Import preset** validates and stages a named JSON file; nothing is sent.
  Connecting retains staged edits. Choose the matching startup mode, Apply then
  Save. Read discards staged edits. Preset names are not stored on the card.

V2 files have six parameters for Twin Orbits or twelve for Mixer, with stable
IDs and a function tag. V1 Twin Orbits files retain their six settings. V1 Mixer
files retain six settings and use default placements (A front, B back, both near
and full panel level), because older files had no placements. Invalid files change
nothing; limit 16 KB. Schema details: [docs/PROTOCOL.md](docs/PROTOCOL.md).

A valid old flash record migrates in RAM: its six settings seed both modes,
with default Mixer placements. No boot-time write occurs. Save writes v2 to the
same final 4 KB sector; corrupt/blank records use defaults. Older firmware cannot
read a v2 record and will use defaults; export files before saving with a fallback
firmware if you need to recover settings. Reflashing this alpha11 UF2 does not
include the saved sector, but Read after any reflash to check your settings.

8mu trim/room/strength overrides are live and not saved automatically. Mixer
placements changed by 8mu are also live until saved via the editor; switching USB
roles requires reset, which restores the last saved placements. There is no 8mu
Save gesture in this pass. To set its starting scene, Apply/Save through the editor,
then connect 8mu and reset. Twin Orbits knob position, phase, motion on/off/depth
and calibration remain outside presets.

The user reports alpha10 8mu tests and the alpha11 validation pass (2026-10-09).
Alpha11 build/host checks also pass. Alpha11 reported peak callback/block readings are **9 / 1,015 µs** and
**11 / 991 µs**, both with 32 taps and below the 18 / 1,200 µs warning thresholds.
USB roles for those two readings were not specified.
Original storage/preset code and documentation: Adrian Vos (soveda), 2026, MIT;
existing platform/editor and KEMAR credits remain in THIRD_PARTY_NOTICES.md.

## Direct 8mu control — Twin Orbits mapping

For Mixer mapping differences, see the alpha9 section above.

Use a USB-C data cable between the 8mu and Workshop Computer. Connect it before
reset/power-up. On Rev 1.1 hardware the USB power role selects host mode for the
8mu, or device mode for a computer/editor, following Chris Johnson's WaveSeq.
The role is fixed until reset: reconnect to the computer and reset for the editor.
Older boards without USB role detection retain editor/device operation in this
first pass. No simultaneous editor/8mu connection or editor setup is needed.
The identify handshake temporarily selects the 8mu default map/bank and disables
its bank buttons until the 8mu is power-cycled.

The card starts from its saved settings (or init defaults). Move each fader through
its current panel/saved value to **pick up** control. A blinking fader LED means
waiting; a steady level means picked up. On disconnect the panel knobs and saved
settings regain control. The five continuous editor settings map to faders; clock
division retains its saved value, default 4. No 8mu gesture writes flash.

| 8mu control | Initial mapping |
|---|---|
| Fader 1 / CC34 | X: orbit speed/direction, centre stop; same 1.5 turns/sec manual maximum |
| Fader 2 / CC35 | Y: distance; CV2 still adds |
| Fader 3 / CC36 | Source separation |
| Fader 4 / CC37 | Room reflections |
| Fader 5 / CC38 | Source A level |
| Fader 6 / CC39 | Source B level |
| Fader 7 / CC40 | Spatial strength |
| Fader 8 / CC41 | Motion depth (starts full; move to full to pick up, then reduce) |
| A / note36 | Recenter motion at the current Main knob and reset orbit phase |
| B / note48 | Toggle motion takeover of Main; initially off |
| C / note60, held | Stop automatic orbit; release resumes. Motion can still place the sound |
| D / note72, held | Show peak block-time bands on the eight 8mu LEDs |

When motion is enabled, Main's **base position is replaced**, rather than added to
a moving knob. Side tilt (EightMU Roll, CC44/45) moves position relative to the
captured neutral pose; gyro rotation (CC46/47) integrates a position offset. Gyro
is rotation rate, so stopping holds the resulting heading. It has a small deadband
and a 150 ms stale-message guard; sustained-rate response needs hardware checking.
Pitch/flip are unused in this pass. Motion depth scales tilt/rotation. A or the
card's Down switch captures a new neutral pose and current Main knob reference.
B off restores the Main knob. CV1 remains additive and the X orbit still runs.
Start with X centred when testing motion alone. Main/CV/orbit positions retain
smoothing in the renderer; recenter deliberately resets the reference.

Hold D for diagnostics: each steady lit LED represents a 150 µs band of the peak
64-frame DSP render time (seven lit covers 901–1050 µs; eight means above 1050 µs).
All eight flash together if a callback/block/queue warning has latched. The card's
bottom-right LED also retains its timing/queue warning. Release D to return to
pickup/level feedback; LED8 is full while motion is enabled. Reset clears peaks
and warnings. This gives an offline load check, not precise numeric profiling.

The integration uses Chris Johnson's EightMU class and its bundled rppicomidi
host driver (MIT notices preserved), adapted to bounded TX/RX and the existing
cooperative audio worker. TinyUSB enumeration waits also render waiting blocks.
Original mappings, takeover and tests: Adrian Vos (soveda), 2026, MIT. See
[vendor/EightMU/SOURCE.md](vendor/EightMU/SOURCE.md) for source revision and edits,
and [the offline test protocol](docs/TEST_PROTOCOL.md#alpha8-offline-8mu-test).
Build size: 78,144 bytes flash / 108,432 bytes main RAM, plus 2 KB per scratch
bank and the driver's small startup FIFO allocations. Build, sanitizer tests for
mapping/DSP/room/block transport, and editor regressions pass.
The user reports the alpha8 hardware tests pass (2026-10-09). Alpha7 remains
available as a fallback. Numeric host-mode timing was not supplied; offline
diagnostics remain available.

## Alpha7 speed range

The free-running X range is scaled uniformly to 75% of its previous speed in both
directions: maximum 1.5 turns/sec, with the same centre stop. Clock lock retains
its existing rate and 2 turns/sec ceiling; timeout/unpatch returns to the new
manual range. No saved settings change. Alpha6 is retained for comparison.
Original range change: Adrian Vos (soveda), 2026, MIT.

## Alpha6 room externalization experiment

Use the 32-tap `alpha6-room32` UF2. Alpha5-opt32 remains the timing/listening
baseline. Controls and six saved fields are unchanged; existing settings load.
Reload the editor for the new room guidance. No extra startup mode is introduced.
Build size: 55,260 bytes flash / 80,860 bytes main RAM, plus 2 KB in each scratch
bank. Firmware and sanitizer checks for DSP, room response and block transport
pass, as do editor regressions. The dry front/back preview is byte-identical to
alpha5-opt32; new room previews are in `previews/alpha6-room-*.wav`.

Three unequal reflection arrivals per ear replace the earlier pair: approximately
7–24 ms after the direct sound, with softened high frequencies and no feedback.
Front positions emphasize the earlier arrival; rear positions emphasize the later
pair. Side position changes the reflected ear balance. These are original synthetic
small-room cues by Adrian Vos (soveda), 2026, MIT, rather than measured room data.
The direct 32-tap KEMAR filters remain attributed to Gardner/Martin (MIT, 1994).

Y now changes direct/reflected balance more strongly: direct sound falls faster
than room sound as distance rises. Room zero keeps the alpha5 dry path. At nonzero
room, a modest direct-level reduction leaves space for reflections. The new taps
reuse the existing delay memory and add no direct-path or block latency. Perceived
externalization and actual RP2040 timing require listening/hardware checks.

Start with one bright mono source, X noon, Y down, room zero and full strength.
Compare front/back without the editor, then Apply room 30–50% and repeat. Slowly
raise Y to the middle and compare apparent distance, direction and loudness
separately. Try a slow orbit, then two sources and maximum room with USB active.
Report peak callback/block times, whether the orbit feels outside the head, and
whether reflections help placement or merely sound like a short echo. See the
alpha6 section of [the test protocol](docs/TEST_PROTOCOL.md).

## Alpha5: test optimization, then longer filters

Two versioned UF2s are included; both retain 64-frame block processing, ComputerCard
0.4.0, 192 MHz, the existing controls/settings and 128-frame (2.67 ms) scheduling
latency. Reload the editor: the timing readout now identifies the running tap count.

| Build | Purpose | Flash / main RAM |
|---|---|---|
| `0.1.0-alpha5-opt32` | Measure optimization against alpha4's 1,181 µs/block | 54,644 / 80,220 bytes |
| `0.1.0-alpha5-hrtf64` | Compare longer measured filters after timing check | 63,092 / 89,180 bytes |

Start with **opt32**. Confirm controls, centre stop, USB/editor/persistence and no
warning LED. Run two sources with fast movement, CV and room active; report peak
callback/block times. Then try **hrtf64** and repeat those checks. Keep callback
below 18 µs and block render below 1,200 µs with no queue warnings. These are
experimental builds; actual speed savings and 64-tap suitability need the card's
measurements. Alpha4 is retained as the measured comparison; alpha2 is the fallback
if a timing/control problem occurs. Avoid alpha3, which failed ISR timing.

For listening, use the same bright mono source, room zero, Y down/CV2 unpatched,
full strength and a hidden editor. Reset Main noon for front; turn Main to either
end for back, then try a slow orbit. Keep settings and monitoring levels the same
between builds. Judge whether the sound is in front/behind separately from colour.
Only add room and distance after comparing the near/dry baseline.

The optimized renderer shares mono distance delay/filtering, uses one FIR history
for both ears, stores coefficients as int16 and performs paired unrolled MACs.
The separate short ear delays supply ITD after the HRTF. For fixed settings these
linear operations commute; moving settings can produce small transient differences.
The shared Q3 distance filter removes the former duplicated 64-bit multiplies.
Settled, stationary HRTF coefficients stop recalculating, and room-zero avoids
unneeded reflection reads; moving/full-room timing remains the relevant worst case.
The ISR and USB/block handoff remain unchanged.

The 32-tap coefficients are unchanged from alpha4. A deterministic stationary
noise comparison at four cardinal directions and three distances differed by at
most one 12-bit step (0.142-step RMS). Both banks use the same gain scale for A/B.
The 64-tap bank retains more measured detail: 1–12 kHz RMS magnitude approximation
error is 0.42 dB, compared with 1.05 dB for 32 taps; 95th-percentile absolute error
is 0.52 versus 1.77 dB. These are numerical filter metrics, not localization scores.
The longer bank is still a generic horizontal minimum-phase approximation, with
analytic ITD and no elevation/personal ear calibration.

`previews/alpha5-opt32-front-back.wav` and `alpha5-hrtf64-front-back.wav` alternate
front/back every three seconds using identical broadband noise, room zero, distance
near. `alpha5-hrtf64-opposed.wav` uses both original synthetic musical sources.
All derived filters/previews credit Bill Gardner and Keith Martin, MIT Media
Laboratory (1994); see vendor/KEMAR/SOURCE_TERMS.md. These previews do not exercise
hardware scheduling, USB or the ADC/DAC. Alpha1–alpha4 artifacts remain available.

## Alpha3 HRTF listening comparison

Use a bright mono signal in Audio 1, Audio 2 empty, X noon, Y down, room zero and
spatial strength full. Reset at Main noon for front, then turn Main to either end
for back. Hide the editor display. Test stationary positions, then a slow orbit;
judge perceived direction separately from tonal differences. Repeat with room
and distance raised only after testing the near, dry sound.

The ear filters now vary across 72 measured horizontal directions. Rear is **not**
necessarily darker: the measured frequency patterns replace the artificial rear
colour. Spatial strength smoothly blends from neutral at zero to the measured
bank at full, also scaling the separate interaural delay. A single conservative
bank gain preserves ear/direction level differences and provides FIR headroom;
volume can differ from alpha2 and with strength changes. Compare at comfortable,
matched monitoring levels. Saved settings/schema and hardware controls are unchanged.

`previews/alpha3-front-back.wav` alternates front/back every 3 seconds with the
same broadband source, stationary A, distance near and room zero. Alpha3 linked
and opposing previews use both synthetic sources. All include the derived KEMAR
filters and their Gardner/Martin attribution. Alpha1/alpha2 previews and firmware
are retained for comparison. No alpha3 stability result is inherited from alpha1.

## Alpha4 processing fix

Alpha3's hardware report matched the documented ComputerCard ISR-overrun failure:
instant timing warning, fast uncontrolled motion and frozen knobs. Alpha4 retains
the same HRTF bank and rendering but processes captured 64-frame blocks on core 1.
Core 0's callback only reads controls/inputs, advances orbit positions, captures
frames and plays completed output. Each frame carries its position and config,
so clock/reset timing is retained relative to the buffered audio. Extra transport
latency is 128 frames (2.67 ms). The vendored ComputerCard 0.4.0 is unmodified,
including normalization and hardware service. The handoff adapts the fixed-ring,
two-block scheduling pattern from Workshop_BlockAudioCard (Adrian Vos, MIT).

Core 1 prioritizes pending DSP before pumping USB. MIDI TX now uses a bounded
non-blocking queue instead of a send/wait loop, and the save-mute wait continues
servicing blocks. Flash writes still occur only after output has faded to silence.
USB initializes before audio capture begins. Queue misses output silence and
latch a warning rather than blocking capture or repeating stale audio. This
protects hardware input cadence; it does not guarantee sufficient DSP throughput.

Open the updated editor for peak **callback µs** and **64-frame block µs**. Warning
thresholds are 18 and 1,200 µs respectively; the block period is 1,333.3 µs.
Readouts are clamped to 16,383 µs and reset on reboot. Check controls and these
readings first, before judging spatial sound. Alpha3 is retained for traceability,
not as the recommended fallback: use alpha2 if alpha4 fails.

Alpha3 previews still represent alpha4's unchanged spatial DSP, apart from the
new transport latency. Host tests compare block output sample-for-sample with the
unbuffered renderer after accounting for its 128-frame delay.

## Build and verify

Requires an installed Raspberry Pi Pico SDK (this build: 2.3.0), CMake and
ARM GCC (this build: 15.2.1).

```sh
cmake -S . -B build -DPICO_SDK_PATH=/path/to/pico-sdk -DSPATIAL_HRTF_TAPS=32
cmake --build build -j2
cmake -S . -B build_hrtf64 -DPICO_SDK_PATH=/path/to/pico-sdk -DSPATIAL_HRTF_TAPS=64
cmake --build build_hrtf64 -j2
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=undefined,address -Isrc tests/dsp_test.cpp -o /tmp/spatial-dsp-test
/tmp/spatial-dsp-test
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=undefined,address -DSPATIAL_HRTF_TAPS=64 -Isrc tests/dsp_test.cpp -o /tmp/spatial-dsp64-test
/tmp/spatial-dsp64-test
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=undefined,address -Isrc tests/room_test.cpp -o /tmp/spatial-room-test
/tmp/spatial-room-test
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=undefined,address -Isrc tests/mu_controls_test.cpp -o /tmp/spatial-mu-controls-test
/tmp/spatial-mu-controls-test
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=undefined,address -Isrc tests/startup_mode_test.cpp -o /tmp/spatial-startup-test
/tmp/spatial-startup-test
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=undefined,address -Isrc tests/modes_test.cpp -o /tmp/spatial-modes-test
/tmp/spatial-modes-test
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=undefined,address -Isrc tests/mixer_mu_controls_test.cpp -o /tmp/spatial-mixer-mu-test
/tmp/spatial-mixer-mu-test
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=undefined,address -Isrc tests/settings_test.cpp -o /tmp/spatial-settings-test
/tmp/spatial-settings-test
node tests/editor_test.cjs
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=undefined,address -Isrc tests/block_audio_test.cpp -o /tmp/spatial-block-test
/tmp/spatial-block-test
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=undefined,address -Isrc tests/midi_tx_test.cpp -o /tmp/spatial-midi-tx-test
/tmp/spatial-midi-tx-test
clang++ -O2 -std=c++17 -Isrc tools/render_preview.cpp -o /tmp/spatial-render
/tmp/spatial-render previews/alpha6-dry-front-back.wav front-back 0 0
/tmp/spatial-render previews/alpha6-room-near-front-back.wav front-back 2048 0
/tmp/spatial-render previews/alpha6-room-far-front-back.wav front-back 2048 3072
clang++ -O2 -std=c++17 -DSPATIAL_HRTF_TAPS=64 -Isrc tools/render_preview.cpp -o /tmp/spatial-render64
/tmp/spatial-render64 /tmp/spatial-current64-opposed.wav 0
/tmp/spatial-render64 /tmp/spatial-current64-front-back.wav front-back
```

Hardware service stays on core 0; block DSP and non-blocking USB/editor share
core 1. Geometry updates remain staggered within blocks. Convolution is direct
fixed-point FIR inside the block renderer; this is not FFT convolution.
The program executes from RAM. Build sizes are listed above,
plus 2 KB in each scratch bank. See [docs/TEST_PROTOCOL.md](docs/TEST_PROTOCOL.md)
for instrument checks and [docs/PROTOCOL.md](docs/PROTOCOL.md) for editor messages.
This independent repository is not a Workshop_Computer release submission.

Alpha10 Mixer 8mu tests reported passed by the user on 2026-10-09. No new timing
readings supplied. Record: Adrian Vos (soveda), 2026, MIT.
