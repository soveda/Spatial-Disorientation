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
