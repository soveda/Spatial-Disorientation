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
