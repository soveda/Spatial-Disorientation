# Twin Orbits alpha1 test protocol

© 2026 Adrian Vos (soveda), MIT. Hardware/API guidance: Chris Johnson's
ComputerCard 0.4.0 and the Workshop Computer maintainers; full sources in
[../THIRD_PARTY_NOTICES.md](../THIRD_PARTY_NOTICES.md).

Host verification passed: sanitizer-backed DSP/config/orbit checks; simulated
editor connection/read/apply/save/reconnection; JavaScript syntax; firmware build.
The following instrument checks are **pending**, not implied by those results.

1. **Boot:** flash, reset ten times, then power-cycle. Audio should fade in after
   roughly 100 ms, controls respond, bottom-right warning LED stays off. Repeat
   with USB connected and disconnected. Verify 2 MB and 16 MB cards if available.
2. **One source:** headphone stereo output, broadband/bright signal in Audio 1,
   Audio 2 empty. X noon, Y down, Main noon, momentarily reset. Turn Main through
   a circle. Right position should be louder/brighter/earlier in the right ear;
   left should mirror this. Front/back filtering should differ, though perception
   may be ambiguous. Repeat using only Audio 2, accounting for 180° default separation.
3. **Motion:** sweep X across the centre. Confirm stop, smooth forward/reverse
   motion and increasing rate. At full travel expect about 2 turns/sec. With two
   distinct sources, switch up keeps separation fixed; middle moves oppositely.
   Hold down: phase resets once and rotation continues. Releasing restores the
   selected switch position. Compare with the supplied software previews.
4. **Distance/CV:** sweep Y near/far; farther should be quieter/darker with more
   reflections. Modulate CV 1 for position and CV 2 for distance. Unpatch both:
   no stale modulation. Sweep controls rapidly and listen for clicks or bursts.
5. **Clock/reset:** feed 120 BPM to Pulse 1, X outside centre. After two edges,
   default speed is one turn per 2 seconds. Try both X directions and centre stop.
   Remove clock pulses while patched: manual rate resumes after timeout. Unpatch:
   manual rate resumes at the next control scan. Pulse 2 resets on rising edges;
   holding the gate high must not continually reset.
6. **Editor round trip:** Chrome/Edge, USB-C data cable, allow MIDI/SysEx, select
   Spatial Disorientation input/output. Connect/read defaults. Change separation
   to 90°, room to zero, apply and listen. Save, reset, reconnect/read: values must
   match. Change/apply without Save, reset/read: saved values must return. Repeat
   Save while audio is running: short fade/mute, no uncontrolled burst, playback
   resumes. Confirm knobs remain independent of editor settings.
7. **Editor resilience:** disconnect USB while connected and while a request is
   waiting; controls disable and errors settle. Reconnect/read/apply. Deny MIDI
   permission and retry. Choose unrelated ports: timeout instead of false success.
   Run only one editor client. Check status and live source display agree with LEDs.
8. **Worst load:** both full-scale sources, maximum room/levels/strength, maximum
   speed, moving CV and editor traffic for at least 30 minutes. No stuck controls,
   unexplained silence or bottom-right warning. Output can clip at extreme summed
   levels; reduce source levels if needed. Profile the full interrupt with a scope
   or instrumented build before release; callback LED alone cannot certify margin.
9. **Silence/tails:** remove sources. Reflections decay promptly; no runaway
   feedback or repeating buffer noise. Reserved CV/pulse outputs remain zero/low.

Record board revision, card capacity, monitoring headphones, firmware version,
clock source/rate, LED warning state, and audible observations. Prioritize left/right
motion, two-source independence, reset playability, centre stop and distance before
judging front/back realism. Do not move to releases until hardware tests pass.

## User hardware report — 2026-10-08

Results reported by Adrian Vos for Twin Orbits 0.1.0-alpha1. Numbers below refer
explicitly to the nine-step conversational test list, whose ordering differs from
this document's detailed list:

- Steps 1, 3, 4, 5, 6, 7 and 8 passed: boot/reset, orbit speed, two-source movement,
  phase reset, distance/CV, clock and editor/persistence.
- Step 2 position: left/right positioning works; front/back differences are not
  really audible. Front/back differentiation remains a listening limitation.
- Step 9 stability: ongoing, with no issues reported after 10 minutes. The full
  30-minute run has not yet been reported complete.

Board revision, card capacity, headphones, individual timing-warning LED readings
and full interrupt profiling were not separately supplied. This report does not
establish every additional check in the detailed protocol above. Next DSP listening
priority: improve front/back cues while preserving the working left/right movement
and controls. No firmware change is implied by recording these results.

Report attribution: Adrian Vos (soveda). This test record © 2026 Adrian Vos, MIT.

## Follow-up and alpha2 retest — 2026-10-08

Adrian Vos reports alpha1 remains stable after 20 minutes; the run remains ongoing.
No 30-minute completion or full interrupt profile is claimed.

Alpha2 changes only spatial DSP and version-labelled delivery/docs. Its stronger
rear shadowing and smoothly blended 8 kHz spectral notch require a new listening
and timing/stability pass; alpha1 results do not transfer automatically.

1. With room zero, strength full, Y down, X noon and only a bright/broadband source
   in Audio 1, reset at Main noon: front. Turn Main to either end: back. Compare
   brightness/hollow colour, then whether the source actually seems behind you.
   Test noise, voice and a rich musical source; a low sine tone is a poor spectral
   cue test. Compare alpha1 and alpha2 with the same settings and monitor level.
2. Raise room to the default 29% and repeat. Reflections may help or mask direction.
   Try intermediate spatial strength. At strength zero and room zero, front/back
   directional colouring should disappear (distance remains active).
3. Recheck left/right balance, both source paths, fast orbits, reverse, centre stop,
   reset, distance/CV, clock and saved editor settings. Report tonal changes and
   perceived spatial improvement separately, including unwanted muffling.
4. Run alpha2 under the detailed worst-load test for at least 30 minutes. Observe
   the timing-warning LED, including while applying editor settings and saving.
   Profile full interrupt timing before release.

The original synthetic comparison preview alternates front/back every 3 seconds;
see README.md. Report attribution and original test documentation: Adrian Vos
(soveda), © 2026, MIT. No external HRTF data was used for this pass.

## Alpha2 listening report — 2026-10-08

Adrian Vos reports front/back is much more evident. Perceived position is slightly
influenced by observing the path in the editor, and positional distinction is less
obvious as the source becomes further away. No additional stability duration,
timing measurement or full regression completion was supplied with this report.

Follow-up listening check: hide the editor or close eyes, compare stationary front
and rear at near/middle/far distance with the same bright source. Repeat with room
zero and then the usual room setting. Keep monitor volume fixed first; optionally
compare at matched perceived loudness to distinguish reduced level from loss of
directional cues. Record tonal difference and perceived location separately.

The current renderer darkens the direct path and increases relative reflections
with distance, which may mask the high-frequency front/back cues; this is a DSP
interpretation to test, not a measured explanation of the listening report.
Consider retaining more directional detail at distance only after those comparisons.
Firmware remains alpha2; no DSP change is made by this record.

Listening observations: Adrian Vos (soveda). Original record © 2026 Adrian Vos, MIT.

## Alpha2 blind result and alpha3 HRTF test — 2026-10-08

Adrian Vos reports insufficient front/back movement without visual cues. Alpha2's
improved tonal distinction therefore does not establish adequate localization.
The user authorized a measured-filter experiment and consideration of
Workshop_BlockAudioCard where appropriate.

Alpha3 uses 32-tap minimum-phase horizontal filters derived from Bill Gardner and
Keith Martin's MIT KEMAR data (copyright 1994 MIT Media Laboratory; terms and
processing in ../vendor/KEMAR/SOURCE_TERMS.md). Its FIR outputs are reference-tested
against the generated bank; this is not a perceptual or hardware timing result.

- Hide the editor: dry bright mono source A, distance near, stationary X, full
  strength. Compare front (Main noon after reset) with rear (Main either end).
  Have another person set the angle if possible to reduce knowledge of position.
  Record whether it seems in front/behind separately from frequency colour.
- Try slow orbits with noise, voice and rich musical audio. Recheck left/right,
  then source B alone, then both sources. Measured rear spectra need not be darker.
- Repeat dry at middle/far distance, then add room. Judge lost detail separately
  from reduced level; match monitor loudness for an additional comparison.
- At strength zero, room zero, distance fixed, directional spectral/level/timing
  differences should vanish once filters settle. Intermediate strength should be
  smooth. Check rapid position, strength, distance and separation changes for clicks.
- Recheck all alpha1 controls, editor/persistence and USB reconnect. Run worst-load
  stability for at least 30 minutes. Bottom-right timing warning must stay off;
  report any warning or control freeze immediately. Profile total ISR before release.
  A compiling short FIR does not prove the 20 microsecond deadline.

Alpha1/alpha2 UF2s remain available to revert. No block transport switch is made
in this experimental pass; see IMPLEMENTATION_PLAN.md for the evaluation.
Original test notes © 2026 Adrian Vos (soveda), MIT; data attribution as above.

## Alpha3 timing failure and alpha4 replacement — 2026-10-08

Adrian Vos reports alpha3 spins much too fast, does not stop or respond to knobs,
and lights the bottom-right warning LED immediately. This is consistent with
ComputerCard audio-ISR overrun and mux/control corruption. Alpha3 fails hardware
validation; its successful host tests/build did not establish real-time suitability.
Fallback firmware is alpha2.

Alpha4 moves the unchanged HRTF engine into 64-frame core-1 blocks with a four-slot
handoff and 128-frame output delay. USB shares core 1 cooperatively with bounded
non-blocking TX. The unmodified ComputerCard 0.4.0 keeps hardware service, jack
normalization and sample-rate controls on core 0. See README.md for attribution
and scheduling details. Host checks cover exact output/latency, queue starvation
silence/recovery, MIDI TX backpressure/wrap, DSP and editor regression.

Test the timing fix before another localization test:
1. Flash alpha4, X noon. Confirm sound is stationary, Main and Y respond and the
   bottom-right LED stays off. Turn X slowly: rate/direction should respond normally.
2. Open the updated editor and report **peak callback** and **peak block** times.
   Move all knobs/CVs and apply maximum room/strength/source levels with two sources.
   Callback should remain below 18 us; block below 1200 us, with no queue warning.
3. Read/Apply/Save, reset, reconnect and Read. Repeat rapid edits, USB disconnect
   and reconnect while audio runs. Saving should mute briefly then resume without
   timing flags or stalled input; it must not block the DSP waiting for USB TX.
4. Retest clock/reset and switch modes. All buffered audio is delayed by 2.67 ms;
   controls/clock/reset are captured at sample rate with their corresponding input.
5. If those pass, perform alpha3's display-hidden front/back listening test and a
   fresh 30-minute worst-load stability run. Do not infer alpha4 stability from
   any previous version. If the warning lights, note the editor's category/peaks
   and revert to alpha2 rather than continuing an uncontrolled test.

Original report: Adrian Vos. Test notes © 2026 Adrian Vos (soveda), MIT.

## Alpha4 initial timing/listening report — 2026-10-08

Adrian Vos reports the editor reads peak callback 5 us, peak 64-frame DSP block
1181 us, linked/free-running, distance 46%. Motion still sounds side to side.
No full stability duration or complete control regression is claimed by this report.

The callback reading is below its 18 us warning threshold. The block reading uses
about 88.6% of the 1333.3 us block period: roughly 152 us of deadline margin and
19 us below the conservative 1200 us warning threshold. This supports the intended
ISR scheduling improvement but shows limited DSP headroom; it does not certify
worst-case timing or prove front/back localization.

Next listening comparison: hide display, source A only with bright/broadband audio,
distance near (Y down, CV2 unpatched), room zero, full spatial strength, X noon.
Compare front (reset at Main noon) with back (Main either end), then a slow orbit.
The reported 46% distance was not the near/dry baseline; distance darkening and
relative room energy may obscure the cue. If the near/dry case still fails,
optimize the worker before experimenting with longer filters, a different generic
HRTF or listener/headphone calibration. Do not simply add taps to this workload.

Report: Adrian Vos. Original record © 2026 Adrian Vos (soveda), MIT.

## Alpha5: optimized 32-tap and 64-tap comparison — 2026-10-08

Adrian Vos subsequently describes alpha4 front/back localization as possibly very
subtle, rather than an adequate pass, and authorizes the optimization/longer-filter
experiment. Alpha4's reported timing baseline is 5 us callback / 1181 us block.

1. Flash alpha5-opt32 and reload the editor; it should identify 32 taps. Test
   controls/centre stop, fast orbits, CV, full room and two sources, then Read/Apply/
   Reset to init/Save/reset/reconnect. Record peak callback and block times and any
   queue/slow-block warning. Do not compare only the cheaper stationary/dry case.
2. If controls and deadlines pass, flash alpha5-hrtf64 with identical settings.
   Editor should identify 64 taps. Repeat the same worst-load test and measurements.
   Thresholds remain callback 18 us / DSP block 1200 us, block period 1333.3 us.
3. Hide the display, dry bright A-only source, distance near, strength full. Compare
   stationary front/rear and slow orbits across the 32/64 builds. Use the same
   monitor level and settings. Record spatial placement separately from tonal
   differences, then retest both sources, distance and room.
4. Run at least 30 minutes on the preferred build under load. Recheck USB/save
   behaviour and warnings throughout. A longer filter's numerical accuracy does
   not prove useful localization, and host tests do not measure RP2040 headroom.

Host validation covers both tap counts with sanitizer-backed DSP/config/orbit tests,
paired FIR versus an independent int64 scalar reference (every direction/ear,
full-range input, impulse/tail and ring wrap), block output equivalence/128-frame
latency/starvation recovery, MIDI TX and editor/tap-ID regression. Stationary 32-tap
output comparison against alpha4 was <=1 12-bit step, 0.142 step RMS, across four
cardinal angles and three distances. Dynamic delay/filter order differs slightly;
listen for clicks or changed motion. Alpha4 is retained for comparison; alpha2 is
available if the new build has timing/control trouble. No alpha5 hardware pass is
claimed. Dataset attribution: Gardner/Martin, MIT Media Laboratory, 1994; original
notes © 2026 Adrian Vos (soveda), MIT.

## Alpha5 user results and alpha6 room pass — 2026-10-08

User reports: alpha5-opt32 callback 5 us / block 916 us; alpha5-hrtf64
callback 5 us / block 1244 us. The latter exceeds the 1200 us warning threshold.
64 taps did not noticeably improve localization. 32 taps gave some difference,
but it sounded very close to the head. These reports do not certify worst-load
USB stability. Alpha6 uses 32 taps and tests externalization through room cues.

1. Flash alpha6-room32, reload editor and Read. Existing settings should load.
   For listening, use only Audio 1, a bright mono source, X noon, Main noon/front,
   Y down, CV2 unpatched, strength full. Apply room zero. Check centre stop,
   position/left-right movement and the dry front/back comparison.
2. Apply room 1200 (29%), then 2048 (50%). Compare front/back with the screen
   hidden; slowly turn Main through a full orbit. Listen for outside-head size
   separately from tonal change, width or a recognizable slap/echo.
3. Slowly raise Y from near to middle/far at each room level. Reflections should
   become more prominent relative to direct sound. Record whether the source
   sounds farther away or merely quieter/diffuse. Keep volume fixed initially;
   repeat at comfortable matched levels if needed. Room zero is the dry reference.
4. Retest source B alone, both sources, linked/opposing movement, clock/reset,
   CV position/distance, and editor Read/Apply/Save/reset/reconnect. Init still
   applies settings without saving. Alpha5-opt32 is the unchanged fallback.
5. With two sources, fast orbits, changing CV and maximum room/levels/strength,
   exercise editor Apply/Read and a deliberate Save. Record peak callback/block
   times and warnings. Targets remain <18 us and <1200 us; the block deadline is
   1333.3 us. A warning is a failed margin check even if audio continues.
6. Run at least 20 minutes with USB activity. Report clicks/dropouts, timing and
   whether room improves front/back placement or masks it. No release claim yet.

Host room tests check reflection arrival windows, direct-level retention, stronger
room/direct ratio with distance, front/rear early/late weighting and tail drainage.
They establish signal behaviour, not headphone externalization or device timing.
Previews alpha6-dry-front-back.wav, alpha6-room-near-front-back.wav and
alpha6-room-far-front-back.wav use the original repeated broadband source with
room 0/2048/2048 and distance 0/0/3072 respectively. They alternate front/back
at three-second intervals; level differences are intentional. Source audio and
room model: Adrian Vos, MIT. Derived HRTFs: Bill Gardner and Keith Martin,
MIT Media Laboratory, 1994; vendor/KEMAR/SOURCE_TERMS.md retains data terms.

Alpha6 host/build results: warning-free 32-tap RP2040 build (55,260 bytes flash,
80,860 bytes main RAM, 2 KB per scratch bank); address/undefined sanitizer DSP,
room and block tests pass, including randomized bounds and starvation recovery.
Editor regressions pass. The alpha6 dry front/back WAV matches alpha5-opt32 byte
for byte. UF2 structure, preview headers and firmware SHA256 are verified.
Hardware validation remains pending.

## Alpha7 speed range check

Alpha6 user report: callback 5 us / block 1015 us; less inside-head sound, clearer
circling, no echoes. Extended stability remains pending. Alpha7 scales free-running
X speed to 75% (1.5 turns/sec maximum); clock-driven rates retain their old ceiling.
Check centre stop, both directions and both X extremes, then clock lock, timeout
and unpatch return to manual speed. Repeat the alpha6 listening/load/stability
protocol above. No DSP or configuration schema change. Original protocol:
Adrian Vos (soveda), 2026, MIT; upstream/data attribution remains as above.

## Alpha8 offline 8mu test

Original protocol: Adrian Vos (soveda), 2026, MIT. EightMU/USB sources and data
attribution: ../THIRD_PARTY_NOTICES.md and ../vendor/EightMU/SOURCE.md.
No simultaneous web editor is needed. Use alpha7 as the stable fallback.

1. Connect the 8mu with a USB-C data cable and reset the card. Rev1.1 USB power
   detection selects host mode; older boards without it retain editor mode.
   After identify, 8mu LEDs should show pickup waits (blinking). Keep X centred
   and start with one audio source. The identify handshake selects the 8mu default
   map/bank; its bank buttons remain disabled until the 8mu is power-cycled.
2. With motion initially off, verify Main still works. Fader1 controls X, 2 Y,
   3 separation, 4 room, 5 A level, 6 B level, 7 strength, 8 motion depth. Each
   needs pickup: move it through the corresponding knob/saved value. For init
   settings, separation and A/B are near halfway, room near 29%, strength/depth
   full. Source B needs its own audio patch. A blinking LED becomes a steady level
   after pickup. Other settings must remain unchanged when one fader moves.
3. Centre fader1 after pickup. Hold the 8mu comfortably and press B once. Motion
   takes over Main; moving the Main knob alone should no longer move the sound.
   Tilt sideways to place it. Rotate clockwise/anticlockwise to change heading;
   stopping should hold heading rather than return it to centre. Check the actual
   tilt axis/polarity: manual versions disagree, so report if front/back tilt is
   the active pair. Gyro depth/deadband/stale guard are experimental.
4. Press A in a new pose: that pose becomes neutral at the current Main knob
   reference and orbit phase resets once. Card Down also recentres/resets. Turn
   B off: Main immediately regains its normal role. Pitch/flip have no mapping.
   Pick fader8 up at full, reduce it and compare tilt/rotation response. CV1 still
   offsets position and CV2 distance; neither should be lost to 8mu takeover.
5. Start a slow orbit with fader1. Hold C: orbit stops; release resumes it. Motion
   may still place the source while C is held. Retest linked/opposing switch,
   clock/reset, clockwise/anticlockwise movement and the 1.5 turns/sec manual cap.
6. Hold D for offline load feedback: peak DSP time uses 150 us LED bands; seven
   steady LEDs covers 901–1050 us, eight steady means above 1050 us. All eight
   flashing together, or the card's bottom-right LED on, means a latched timing
   or queue warning. Release D for normal feedback (LED8 full means motion on).
   Capture the lit LED count; precise numbers are unavailable in this session.
7. Use two sources, maximum room/strength/levels, fast orbits, rapidly moving CV,
   all faders and motion together. No clicks/dropouts or warning LEDs. Run at
   least 13 minutes, matching the previously accepted alpha6 duration. Hold D
   periodically to check headroom. Callback/block limits remain 18/1200 us with
   a 1333.3 us block deadline; warning bands do not replace the queue warning.
8. Unplug/replug 8mu while audio runs: panel controls/saved settings return, no
   stuck gesture, and reconnection starts pickup again. If enumeration fails,
   reset with the 8mu attached; automatic USB stack restart is deliberately off.
   Connect a computer instead and reset to select editor mode. Verify Read/Apply/
   Save/reset/reconnect, and retained settings. 8mu runtime edits must not have
   overwritten saved configuration. Reset also clears timing peaks/warnings.

No alpha8 hardware pass is claimed by the build/host tests.

Alpha8 host/build checkpoint: warning-free firmware build, 78,144 bytes flash /
108,432 bytes main RAM plus 2 KB per scratch bank and startup driver FIFO
allocations. Address/undefined sanitizer tests pass for pickup/mapping/motion,
DSP, room and block scheduling; editor regressions pass. The fast detach/attach
case resets ownership using a connection generation even if no disconnected
snapshot reaches core 0. RP2040 UF2 structure and all firmware SHA256s verified.
Linked host delay calls were inspected to confirm the cooperative audio hook.
Actual 8mu USB/audio deadlines and axes remain pending hardware testing.

## Preset editor pass (2026-10-09)

The user reports alpha8 8mu tests pass. This editor-only pass uses that firmware;
no reflash is needed. Original protocol: Adrian Vos (soveda), 2026, MIT.

1. Reload web/index.html. Without MIDI enabled, edit settings, give them a name
   and Export preset. Change the sliders, then Import the file: all six values
   and the name should return. Apply/Save remain unavailable until connected.
2. Connect/read the card: imported edits must remain displayed, with a status
   explaining they are staged. Audio must remain at the card's previous settings.
   Apply auditions them; Save only becomes available after successful Apply.
3. Save, reset/reconnect and Read: verify the six settings persist. Export after
   Read if you want an exact record of the applied settings. Name stays a file
   label; physical knobs, CV and live motion are not preset data.
4. Import malformed JSON, wrong format/version/function, missing/extra IDs,
   out-of-range/non-integer values or invalid clock division. Reject them without
   changing controls/name, sending MIDI or enabling Save. Reject files over 16 KB.
5. Import while connected: no sound change until Apply. Explicit Read must discard
   imported edits. Reconnect must retain staged edits. Re-selecting the same file
   should work. Export captures unapplied edits as displayed, not stale card state.
6. For offline 8mu operation: Apply/Save the preset using the editor, swap the cable
   to 8mu and reset. Saved values should be the starting points for fader pickup.

Regression tests cover offline round trips, atomic invalid-file rejection, staged
connect, explicit Apply/Save and Read discard plus prior lifecycle/init behaviour.
A real browser check confirms offline export and file-chooser import, correct
labels/values, disabled Apply/Save without a card, and the preset controls' layout.
Hardware persistence for this editor pass remains to be verified by the user.
