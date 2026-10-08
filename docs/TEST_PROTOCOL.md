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
