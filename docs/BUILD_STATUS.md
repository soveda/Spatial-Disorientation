# Beta2 verification — 2026-10-15

©2026 Adrian Vos(soveda), MIT. [Source and dependency attribution](../THIRD_PARTY_NOTICES.md).

**0.3.0-beta2** promotes user-tested0.3.0-alpha3, commit`c6906d0`, without DSP or
control changes. The distributed UF2 and root Release rebuild both have SHA256:
`d8f2c50715d2963bcb964203e19795e8d15f59419d62241e1da6e01a834d98ef`.
The firmware bytes are identical to the alpha3 accepted by the user.

Build passes without warnings with PicoSDK2.3.0/ArmGNU15.2.Rel1. Flash147108B,
mainRAM187968B, scratch2048B each.192MHz/1.15V, audio48kHz, two sources/64taps,
64frame blocks and128frame transport remain. ComputerCard0.4.0 includes the local
attributed ADC/DMA flash-pause adaptation; it is not an unmodified upstream header.

All22C++host programs pass ASan/UBSan against promoted root, including Orbits pages/
pickup/held values/fresh-session/D-hold/A/B/C/reconnect,1000changed+100unchanged
mock saves and injected failures, actual parser init→Apply→Read, settings/migrations,
HRTF/FIR/scene/transport/height/slope/room/smoothing/8mu/MIDIqueue regressions.
Actual editor script passes simulatedMIDI lifecycle, stale-readback detection,
randomise/reset per-mode isolation/no-send/bounds, Apply/Save/preset/migration tests.
These tests supplement rather than replace hardware acceptance.

User reports: save fix passes; alpha3 all tests pass and page indicator effective;
randomisation reveals effective elevation. No new alpha3 timing or run-duration
numbers were supplied. Historical alpha26 Wander30min14/1193µs and alpha1 Orbits
22min12/1188µs give context only, not beta2 measurements or a universal load limit.
Additional testers should log exact settings/board/USB role and peaks, especially
continuous height changes, heavyCV and separate editor/8mu boots.

Metadata validates against canonical Workshop Computer schema with0errors/0warnings.
Beta1 complete source/editor/docs/UF2/original testerZIP is preserved in archive/beta1;
all29 archived UF2 checksums are verified. Current distribution contains one UF2 and
matching editor/notices/source/test guide. Beta1 migration/downgrade is documented.
Upstream Workshop release and remote Pages are not changed by local packaging.
