# Build verification history

Original verification notes © 2026 Adrian Vos (soveda), MIT. Platform/dependency
credits: [../THIRD_PARTY_NOTICES.md](../THIRD_PARTY_NOTICES.md). Current alpha9
results are in the final section; the scaffold record below is historical.

## Initial scaffold

Verified 2026-10-07 on macOS with Pico SDK 2.3.0 and ARM GCC 15.2.1.
Both CMake configuration and Release compilation completed successfully and
produced ELF and UF2 files in the ignored build directory.

The scaffold uses approximately 19 KB flash and 23 KB main RAM, with a 2 KB
core-0 stack reservation. These figures do not predict the finished effect size.
No hardware flash, audio listening test, reset test or callback timing measurement
has been performed. Effect DSP and the Spatial Disorientation editor are pending.

The scaffold was rebuilt successfully after changing the initial CPU target to
192 MHz and core voltage to 1.15 V, following ComputerCard's clock guidance.
Audio remains 48 kHz. This build check does not constitute hardware validation
of the forthcoming spatial DSP or editor.

## Alpha9 build checkpoint — 2026-10-09

Pico SDK 2.3.0 / ARM GCC 15.2.1 build passes at 192 MHz / 1.15 V, 48 kHz,
32 HRTF taps. Flash image 80,064 bytes; main RAM 113,528 bytes; scratch banks
2,048 bytes each. ComputerCard 0.4.0 remains unmodified. Startup, modes, DSP,
room, 64-frame transport, 8mu and MIDI TX host tests pass with AddressSanitizer /
UndefinedBehaviorSanitizer. Editor lifecycle/preset/mode tests pass in Node.
Block regression includes independent source distances and panel levels.
At this build checkpoint, alpha9 hardware validation was pending. Alpha8 and preset passes are
user reports, not newly repeated hardware measurements.
Original record: Adrian Vos (soveda), 2026, MIT; sources in ../THIRD_PARTY_NOTICES.md.

## Alpha9 Spatial Mixer hardware report — 2026-10-09

The user reports the supplied Spatial Mixer test protocol passes. Two reported
peak callback / 64-frame block readings are 7 / 1021 us and 8 / 996 us, both
with 32 HRTF taps. Both are below the callback 18 us / block 1200 us warning
thresholds; actual 64-frame deadline is 1333.3 us. No change to firmware is made.
The USB roles corresponding to the two readings were not specified. Do not infer
separate alpha9 8mu or full Twin Orbits regression passes from this Mixer report.
Source: user hardware test report in this conversation, 2026-10-09.
Original record: Adrian Vos (soveda), 2026, MIT.
