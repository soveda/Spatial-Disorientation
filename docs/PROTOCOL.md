# Twin Orbits editor protocol v1

© 2026 Adrian Vos (soveda), MIT. SysEx/editor transport structure follows Chris
Johnson's ComputerCard web_interface example. See [../THIRD_PARTY_NOTICES.md](../THIRD_PARTY_NOTICES.md).

Frame bytes: `F0 7D 53 44 01 command sequence payload F7`. Experimental non-commercial
manufacturer ID 7D, product signature ASCII SD, schema 1. All interior bytes are
7-bit. Sequence 1–127 correlates requests; telemetry uses 0. One outstanding request
per editor. Unknown signatures are ignored; unsupported versions/commands and
invalid settings are rejected. No dynamic buffers in the firmware parser.

| Command (hex) | Payload / response |
|---|---|
| 01 Read | Empty; response 41 with six parameter triples |
| 02 Apply | Six triples; response 42 after audio core consumes settings |
| 03 Save | Empty; response 42 after muted flash save/verification |
| 41 Snapshot | Triples: stable ID, value low 7 bits, value high 7 bits |
| 42 Acknowledge | Request command, status |
| 43 Telemetry | A angle low/high, B angle low/high, distance low/high, flags; about 20 Hz |

Statuses: 0 success, 1 unsupported command/length, 2 unsupported schema, 3 invalid
settings, 4 busy, 5 save failed. Apply requires every ID exactly once; validation
is transactional. Realtime MIDI bytes are ignored within SysEx. Oversized or
malformed frames are discarded. Editor timeout: 1.5 s, or 3 s for Save.

| ID | Setting | Range / default |
|---|---|---|
| 1 | Separation | 0–4095 maps a turn / 2048 |
| 2 | Room | 0–4095 / 1200 |
| 3 | Source A level | 0–4095 / 2048 |
| 4 | Source B level | 0–4095 / 2048 |
| 5 | Spatial strength | 0–4095 / 4095 |
| 6 | Clock pulses/turn | 1, 2, 4, 8, 16 / 4 |

Angles are 0–4095: 0 front, 1024 right, 2048 back, 3072 left. Flags: bit 0 linked,
bit 1 clock locked, bit 2 latched callback timing warning, bit 3 saving. Telemetry
is observational; separately read fields can differ by one control update.

Flash record: little-endian uint32 magic 0x314f4453, uint32 schema 1, uint32 FNV-1a
checksum of six uint16 values, then those values (24 bytes total). Stored in the
first page of the final 4 KB sector of actual flash. No erase at boot, no autosave.
Audio core fades to zero and allows DAC pipeline flushing before acknowledging;
USB core locks out audio core and disables its local interrupts for erase/program.
RAM execution and a firmware-overlap check are required. A failed/torn save uses
defaults next boot; this prototype does not promise power-loss recovery of old settings.

## Alpha4 timing diagnostics (compatible extension)

Command 44, sequence 0, four payload bytes: callback peak low/high 7 bits, DSP
block peak low/high 7 bits, in microseconds, saturated at 16383. Sent alongside
telemetry at approximately 20 Hz when the bounded TX queue is empty. Peaks reset
on reboot and ignore intentional flash-save pauses. Callback measurement excludes
the framework's surrounding ISR; block measurement includes core-1 interruption.

Additional 43 flags: bit 4 block render >=1200 us, bit 5 audio queue miss, bit 6
callback >=18 us. Bit 2 remains the combined latched warning. These fields do not
alter parameter IDs, config schema, existing commands or flash records. USB TX is
non-blocking: if the host does not consume, queued bytes stay bounded; telemetry
is skipped and an overloaded command client may time out and retry. Commands
should remain one at a time.

## Alpha5 tap identity

Timing command 44 may append a fifth payload byte: active FIR taps, 32 or 64.
The updated editor accepts both alpha4's four-byte and alpha5's five-byte form.
The config schema and parameter IDs remain unchanged. Tap count is a compile-time
comparison profile, not a saved editor setting.

## Alpha8 separate USB roles

The editor protocol/configuration schema remains version 1. A computer-connected
reset selects device/editor mode on Rev1.1 hardware; an accessory/empty port
selects host/8mu mode. Reset after switching roles. Direct 8mu runtime controls
are volatile overrides and do not modify this protocol's saved configuration.
Clock division remains its saved value. Offline 8mu diagnostics use LEDs rather
than SysEx; no simultaneous editor connection is required. Implementation/source
credit: Adrian Vos (soveda), 2026, MIT; Chris Johnson's WaveSeq/EightMU and the
rppicomidi driver are credited in ../vendor/EightMU/SOURCE.md.
