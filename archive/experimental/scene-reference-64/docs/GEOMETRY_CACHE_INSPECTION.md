# Geometry cache savings inspection

2026-10-11. Original investigation/documentation © 2026 Adrian Vos (soveda),
[MIT](../../../alpha14/LICENSE). Existing DSP/source attribution remains in
[THIRD_PARTY_NOTICES](../../../alpha14/THIRD_PARTY_NOTICES.md); KEMAR measurements by
Bill Gardner and Keith Martin, MIT Media Laboratory (1994), retain their
[separate measurement terms](../../../alpha14/vendor/KEMAR/SOURCE_TERMS.md).

## Current work

Engine schedules A geometry on samples 0/32 and B on 16/48 of each 64-frame
block: four source calculations per block, 3,000 per second combined. For
Disorientation, path resolution precedes Source::Geometry. Figure-eight and
Wander include Cartesian angle/radius resolution; Pendulum is a sine-based arc.
Source::Geometry derives HRTF addressing, two interpolated sine values, room
balance, direct gain, distance delay/tone and interaural delay targets.

Hrtf::Geometry already caches phase, strength and elevation. It skips unchanged
HRTF addressing and does not restart coefficient settling for unchanged inputs.
However Source::Geometry still recalculates its other targets even if unchanged.
Hrtf::Update calculates only one coefficient index for both ears per source frame,
and stops after 64 consecutive unchanged index visits. Smoothing, convolution,
delay reads and room processing continue at audio rate and must remain intact.

## Temporary probe

An isolated copy under `/tmp/spatial-geometry-inspection` added an exact Source
cache keyed by resolved phase, distance, effective level, strength, room and
height. It returns before target calculation only when every key matches. Each
source owns its cache; the first call always calculates. Path resolution and
Wander state bookkeeping remain unchanged, and rendered-scene telemetry is still
updated on its original cadence. No queue reference is retained.

A separate counter measured the reuse potential for lazy HRTF targets: each tap
was marked with the current HRTF geometry generation. A visit was reusable only
when that tap had already been calculated in the same generation. Actual HRTF
target caching was **not implemented** in this probe.

Each scenario processed 48,000 frames with two 64-tap sources, default Config,
shape 2400/excursion 3000 where relevant, deterministic changing audio, and
frame-exact output comparison against unchanged alpha19. Except the last row,
height changed every 4,096 frames to exercise invalidation. Step angles changed
every 4,800 frames; moving paths traversed approximately one phase revolution
per second. These are synthetic inputs, not recorded hardware controls.

| Scenario | Full geometry calls / calculations | Calculations avoided | Lazy HRTF index visits / reusable |
|---|---:|---:|---:|
| Stationary, periodic height changes | 3,000 / 24 | 99.2% | 39,054 / 37,502 |
| Angle step every 100 ms | 3,000 / 42 | 98.6% | 56,151 / 53,447 |
| 1 Hz Orbits | 3,000 / 3,000 | 0% | 96,000 / 0 |
| 1 Hz Fig8 | 3,000 / 3,000 | 0% | 96,000 / 544 |
| 1 Hz Pendulum | 3,000 / 2,995 | 0.17% | 96,000 / 0 |
| 1 Hz Wander | 3,000 / 2,836 | 5.47% | 94,024 / 8,600 |
| Fixed angle, random per-frame distance | 3,000 / 3,000 | 0% | 39,054 / 37,502 |
| Stationary, constant height | 3,000 / 2 | 99.93% | 3,842 / 3,698 |

Each lazy index visit represents the pair of ear targets. Cache hits eliminate
only the associated target calculation, never its coefficient slew. The constant
stationary case already stops HRTF updating after settling; lazy caching could
save repeated interpolation during settling, not ongoing work after that point.
Moving HRTF geometry changes every 32 samples while each tap is revisited every
64 samples, explaining the lack of reuse during continuous orbiting.

All eight comparisons passed under AddressSanitizer/UndefinedBehaviorSanitizer
(384,000 total frames). The temporary Source-cache implementation also passed
the existing 180,000-frame transport comparison against alpha18, including
per-frame config changes, paths, heights, solo/dual, slot reuse and starvation.
These establish agreement in tested cases, not a hardware timing improvement.

## Savings and recommendation

Full-target caching is a small, safe candidate for stationary Mixer or frozen
scenes. At most it skips four Source::Geometry bodies per block, saving eight
sine interpolations and the other target arithmetic. The baseline already skips
unchanged HRTF addressing. Cache-key comparisons add overhead on misses.

Separate angular and distance/gain caches can retain part of that benefit when
only one group changes. Dependencies must be explicit: angular terms depend on
phase/strength; projected ITD also depends on height; base/tone depend on distance;
gain depends on distance/effective level/room; room targets combine distance,
effective level/room and lateral position. Shared effective level must include
both panel level and configuration trim. Treat keys as exact fixed-point values;
quantization or lower update cadence would be a separate sound-changing proposal.

For continuously moving Orbits and Fig8, a whole-geometry cache has little scope
and could slightly increase time. It should not be counted on to reduce the
reported 1,158 microsecond peak toward 1,000–1,050 microseconds. No microsecond
saving or percentage of total DSP time can be inferred from these hit counts;
actual savings require instrumented RP2040 A/B timing, including cold misses and
rapid control changes.

Avoid eager full-filter target generation at every geometry update. It would
calculate 128 ear targets per source every 32 samples, versus the present 64 ear
target calculations over that interval: twice the target work during movement.
A lazy per-tap cache preserves the existing timing/smoothing but introduces
validity checks and storage (about 1,024 bytes for two-source int32 target arrays,
plus validity state). Its weak dynamic-path hit rate makes it a lower priority.

Wander offers another, separate cache opportunity: its four hashed node values
remain constant within each quarter-phase segment even while interpolation moves.
Caching those values by node can help a moving Wander path, provided reversal,
wrap and reset behaviour remain exact. This would not improve Orbits or Fig8.

Recommendation: benchmark exact full-source or dependency-group caching as a
static-scene optimization, while treating broader per-frame DSP optimization as
the stronger route to headroom across all moving modes. Firmware, editor, root
source and preserved UF2s were not changed by this investigation.
