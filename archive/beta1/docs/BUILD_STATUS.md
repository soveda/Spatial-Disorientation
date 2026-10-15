# Beta1 verification

© 2026 Adrian Vos(soveda), MIT. Source/data/dependency attribution in
[THIRD_PARTY_NOTICES.md](../THIRD_PARTY_NOTICES.md). Recorded2026-10-13.

0.2.0-beta1 promotes alpha26 without DSP/control changes. Packaged UF2 is byte-for-
byte the user-tested alpha26 binary. SHA256:
`79482afee79ec2576f27c444a253411be2936322fc17519bb1bc642165f04855`.

Root Release rebuild with Pico SDK2.3.0/Arm GNU15.2.Rel1 passes: flash141504B,
RAM182252B, scratch2048B each. Rebuild differs from packaged UF2 by one payload
byte in embedded build date (Oct12→Oct13); executable code/data are otherwise
identical. This is not a fresh DSP hardware revision. Clock192MHz/1.15V, ComputerCard
0.4.0, audio48kHz, two sources/64taps, block64, transport128 remain.

Host checks with ASan/UBSan cover all root C++test programs, including FIR scalar
reference,1024000 exactHRTF comparisons,756000 alpha25-exact zero-tilt scene/transport
frames,180000 changing-slope transport frames, independent rotation reference,
heightbounds/freeze/reverse, settings/migration/rejection, smoothing, room, block
starvation/recovery, modes/startup,8mu ownership/pickup and MIDIbackpressure.
Actual editor script passes simulated-MIDI lifecycle/Apply/Read/Save/preset legacy
migration/slope/height/telemetry checks. This does not replace hardware USB testing.

User reports: Pendulum25min14/1175µs, audible slope and no clicks/gaps/queue faults;
Fig8 20min14/1176µs (listening/queue result not separately supplied); Wander30min
14/1193µs, audio clean/no queue warnings. Earlier tests used audio-rateCV; exact
settings/USBrole not fullylogged. Narrowest block warning margin7µs; deadline
1333.3µs. These readings are one setup, not guaranteed limits for other boards.

Beta tester priorities: matched hardware/headphone reports, persistence/preset
roundtrip, separate8mu boots and high-load sustained runs. Current root metadata
uses canonical Workshop Computer fields; upstream release remains unchanged.
Archives preserve28 previousUF2 files and a checksum manifest. Tester bundle
contains only current beta/editor/docs/licenses; it does not remove fallback files.
