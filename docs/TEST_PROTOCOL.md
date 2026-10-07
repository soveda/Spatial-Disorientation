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
