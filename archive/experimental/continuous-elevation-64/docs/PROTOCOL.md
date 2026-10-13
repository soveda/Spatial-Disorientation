# Alpha22 height and persistence contract

© 2026 Adrian Vos (soveda), MIT. [Source attribution](../../../alpha14/THIRD_PARTY_NOTICES.md).
Extends this project's existing attributed ComputerCard web-interface adaptation.

SysEx: `F0 7D 53 44 04 command sequence payload F7`. Existing commands and
status codes retain their meaning. New editor and firmware must both use v4.

| Mode | IDs in Apply/snapshot | Payload length |
|---|---|---:|
| Twin Orbits (0) | 1–6 | 19 |
| Spatial Mixer (1) | 1–12, 14, 15 | 43 |
| Disorientation (2) | 1–6, 13 | 22 |

Settings payload is mode byte followed by ID/low7/high7 triples. Duplicate,
missing, foreign IDs and invalid ranges reject atomically. IDs 14/15 are Mixer
height A/B, integer 0–4096: 0 Below (−40°), 2048 Level, 4096 Above (+40°).
ID 13 is still Disorientation movement (0 Fig8, 1 Pendulum, 2 Wander).

Command 4 auditions both Mixer heights as `Alo Ahi Blo Bhi`, without flash write.
Command 5 requests height; response 0x45 returns the same four-byte encoding.
Both commands are Mixer-only. Invalid heights reject without changing either.
Commands 1 Read and 3 Save capture applied A/B height and live placement.
Command 2 Apply includes height and rearms physical/8mu pickup as before.
Snapshots fit the existing fixed 64-byte MIDI/parser queues.

Flash v5: 68-byte record, 56-byte Settings payload, appending two uint16 heights
at offsets 52/54 after v4's mode configs, placements and movement/reserved.
Checksum covers the full payload. Legacy v1–v4 records are validated under their
original layout and add Level heights. Only explicit Save writes v5.

JSON v4 uses the same stable IDs with the existing format/function/name envelope.
Mixer has 14 parameters. v1 Mixer imports add default placement and height;
v2/v3 Mixer imports retain placements and add Level heights. Other mode
migration behaviour remains unchanged. Motion, phase and temporary configuration
ownership are excluded. Preset export uses displayed values; Read refreshes live
height/placement first. Firmware startup selection remains independent of presets.
