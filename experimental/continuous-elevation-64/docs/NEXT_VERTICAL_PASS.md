# Staged next: vertical movement in Disorientation

© 2026 Adrian Vos (soveda), MIT. Existing measurement/dependency attribution:
[third-party notices](../../../THIRD_PARTY_NOTICES.md). This is a plan, not an
implemented or hardware-tested feature.

First validate alpha22 continuous Mixer height, persistence and worst-case
intermediate-height timing. Preserve alpha22 source/UF2 as the next pass's fallback.
Keep both sources, 64 taps and 192 MHz; require callback <18 µs/block <1,200 µs
without queue/audio faults before widening movement scope.

Proposed next experiment: add bounded vertical coordinates to core-1 path
resolution for Fig8, Pendulum and Wander. Reuse the alpha22 continuous measured
height interpolation; avoid per-sample floating point or a new HRTF bank.

- Fig8: phase-related rise/fall across the lobes, with independently phased A/B.
- Pendulum: an optional rising/falling arc alongside the current angular swing.
- Wander: smooth deterministic height targets tied to existing segment/reset
  epochs, retaining reversible traversal and freeze behaviour.

Provide mode-specific editor height centre and excursion settings, saved/exported
in the Disorientation bank; default vertical excursion zero to preserve current
horizontal audio. Keep freeze/reset/clock/D movement selection semantics. The
exact ranges and phase relationship should be chosen for playability in that pass.

Fader 8 currently controls Disorientation motion amount. Preserve that mapping
for the first vertical experiment; use editor settings for vertical centre/depth
until a different live mapping is explicitly agreed. Mixer fader 8 remains height.
No accelerometer takeover in Mixer is proposed.

Measure both sources moving at intermediate height, high room and rapid CV before
adding further controls. Compare zero vertical depth sample-for-sample with alpha22
and validate deterministic/reversible/frozen paths, bounded height, saved-mode
isolation, then blind headphone listening. If timing is insufficient, profile
coefficient interpolation before considering any sound/geometry-rate compromise.
