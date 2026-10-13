# Alpha23 vertical settings contract

© 2026 Adrian Vos (soveda), MIT; [source attribution](../../../alpha14/THIRD_PARTY_NOTICES.md).
Extends the project's existing attributed ComputerCard web-interface adaptation.

SysEx header `F0 7D 53 44 05 command sequence`; terminator `F7`. Existing status
codes/actions retain meaning. Mixer height audition/read commands remain v4's
four-byte pair encoding, now carried under protocol v5.

| Mode | Parameter IDs | Settings payload |
|---|---|---:|
| Twin Orbits (0) | 1–6 | 19 bytes |
| Mixer (1) | 1–12, 14, 15 | 43 bytes |
| Disorientation (2) | 1–6, 13, 16, 17 | 28 bytes |

Payload is mode then ID/low7/high7 triples. ID 16 is vertical centre 0–4096
(2048 Level, ±40° endpoints); ID 17 is excursion 0–2048 (0–40° half-range).
Missing/duplicate/foreign IDs or out-of-range values reject atomically. Mixer
heights 14/15 and Disorientation movement 13 retain their ranges and roles.

Telemetry 0x43 keeps its first 13 bytes unchanged and appends rendered height
A low/high then B low/high (17-byte body), each 0–4096. These are worker-resolved
geometry targets, not saved values or an instantaneous acoustic filter measurement.
The packed resolved height word is core-1-owned; it is informational telemetry.

Flash v6 appends uint16 centre/excursion at Settings offsets 56/58; Settings is
60 bytes and full record 72 bytes. v1–v5 load/migrate in RAM; v5 Mixer heights
are preserved, centre/excursion default 2048/0. Only explicit Save writes v6.
JSON v5 Disorientation has nine parameters; v1–v4 migrations add default vertical
settings as appropriate, preserving supported movement/configuration. Mixer
JSON v4 heights retain their meaning. Current live path phase/height is excluded.
