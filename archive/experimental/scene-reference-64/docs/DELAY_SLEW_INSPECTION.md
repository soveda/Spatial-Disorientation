# Delay-read and smoothing inspection — alpha19

2026-10-11. Original investigation, temporary candidates, tests and documentation
© 2026 Adrian Vos (soveda), [MIT](../../../alpha14/LICENSE). Existing implementation and
firmware retain [third-party attribution](../../../alpha14/THIRD_PARTY_NOTICES.md).
KEMAR measurements are by Bill Gardner and Keith Martin, MIT Media Laboratory
(1994), under their [separate measurement terms](../../../alpha14/vendor/KEMAR/SOURCE_TERMS.md).
No replacement measurement data or external DSP implementation was introduced.

## Outcome

**Smoothing is the stronger next hardware experiment.** It removes sign-dependent
branches while preserving exact output, and keeps the FIR loop at its baseline
61 instructions per eight taps. Shared delay addressing produces smaller surrounding
code but changes register allocation, adding four instructions to every FIR loop
iteration. Delay savings are therefore uncertain and require a separate hardware
comparison; smaller function size alone would give a misleading conclusion.

Firmware, editor, root source and packaged UF2s remain unchanged. These are
inspection candidates and host checks, not a new alpha or hardware-validated
optimization. Both sources, all 64 taps, smoothing rates and geometry cadence
remain intact; the intended clock remains 192 MHz.

## 3. Delay-line reads

Each source performs one mono direct-delay read and, when both room gains are
nonzero, six mono reflection reads per frame. With two sources, this is up to
14 mono reads per frame / 896 per block. The separate four ear-delay reads per
frame use different delay amounts and are outside this candidate.

The reflection offsets (337, 401, 631, 727, 1069 and 1153 samples) are integers.
Consequently all seven mono reads share the fractional part of the base delay.
Their starting positions can use one shared `head - 1 - (base >> 8)` calculation,
then subtract each integer offset and wrap. Each read still needs two sample
loads, interpolation, and its own wrapped positions. No taps or interpolation
can be removed without changing the sound.

Compiled alpha19 already shares some head/address state, but still forms several
Q8 offset additions, shifts and fractional masks independently. A temporary
`Prepare`/`ReadPrepared` candidate makes the common position/fraction explicit.
The preferred source spelling prepares it only for the reflections after the
FIR and ear delay; the direct read stays unchanged. Exact fraction, wrap and
interpolation rounding are retained.

The generated Source::Process contains 44 stack accesses versus baseline 57,
and is 64 bytes smaller. However its FIR loop uses high registers for some
accumulation and requires four extra MOV instructions per iteration: 65 instead
of 61. There are still 24 loads, 16 multiplies and 19 additions, with no spills
inside the FIR. This adds 32 executed FIR instructions per source frame, or
4,096 per two-source block. Those counts are not CPU cycles.

Initially the candidate prepared the position before the direct read. Moving
that preparation after the FIR did not fix the register-allocation consequence:
GCC generated the same sizes/counts, with common work still affecting register
allocation. A source-level lifetime boundary does not force the desired machine
schedule. Isolating the room work or constraining scheduling might fix this, but
would introduce further call/register/stack tradeoffs that were not tested here.

Room-off scenes already skip reflection reads after their smoothed gains reach
zero, so this candidate has little benefit there. For room-on scenes, only actual
RP2040 timings can establish whether saved address/stack work outweighs the
additional FIR instructions. Do not combine it into a smoothing firmware pass
without a separate measurement.

## 4. Smoothing

There are eight Source::Process slews per source frame: base distance delay,
two room gains, early-reflection balance, direct gain, tone and two ear delays.
That is 768,000 source slews per second combined, or 1,024 per block. HRTF
coefficient settling adds two more per source frame while active (up to 256 per
block / 192,000 per second); it already stops after convergence.

The original expression is `from + d/divisor + sign(d)` for nonzero `d`, otherwise
`from`. GCC already replaces all these constant power-of-two divisions with
shifts and negative-rounding corrections. There is no software division call
here to eliminate. The remaining opportunity is sign-dependent control flow
and surrounding register/temporary handling.

The temporary replacement, for divisors `1 << Shift`, is:

```cpp
int32_t d = to - from;
if (d == 0) return from;
int32_t sign = d >> 31;
int32_t magnitude = (d ^ sign) - sign;
int32_t step = (magnitude >> Shift) + 1;
return from + ((step ^ sign) - sign);
```

This keeps the zero shortcut, magnitude truncation and minimum one-unit movement
toward the target. The signed arithmetic assumes the same bounded DSP differences
as the current implementation; it is not a generic overflow-safe INT32_MIN helper.
Shifts are 7/8/9 for the source slews and 2 for HRTF coefficient settling. No update
is skipped and settling speed/rounding remain exact.

A standalone ARM wrapper for divisor 256 executes 14 instructions on an increasing
value and 16 on a decreasing value in the baseline, versus 12 for either direction
in the candidate, including the return instruction. Both execute four instructions
when already settled. These are wrapper-path instruction counts, not cycle costs
or guaranteed savings when inlined into the larger function.

With the real main.cpp compile flags, Source::Process shrinks by 120 bytes and
its branch-instruction count falls from 60 to 27 across all generated paths.
Its FIR retains 61 instructions per iteration with no stack access. This provides
a more broadly useful opportunity under moving controls than a whole-geometry
cache. Quiet, settled values will gain less, since both retain the zero shortcut.

## Compiled comparison

Arm GNU Toolchain 15.2.Rel1 / GCC 15.2.1, using the existing alpha19 flags.make:
Cortex-M0+, Thumb, soft-float, `-O3`, C++17, exceptions/RTTI/unwind disabled.
Each main.cpp was compiled to an object; firmware was not linked or packaged.
Baseline object size, stack allocation and FIR instruction count match the
previous inspection of the actual linked alpha19 ELF.

| Source::Process generated code | Baseline | Delay | Smoothing | Combined |
|---|---:|---:|---:|---:|
| Function size, bytes including literal pools | 1,760 | 1,696 | 1,640 | 1,572 |
| Static instructions across all paths | 788 | 758 | 728 | 698 |
| Explicit stack-access instructions across all paths | 57 | 44 | 53 | 40 |
| Branch instructions across all paths, including return | 60 | 58 | 27 | 25 |
| FIR instructions per eight-tap iteration | 61 | 65 | 61 | 65 |
| Local stack allocation, bytes | 52 | 52 | 52 | 52 |

Static counts include mutually exclusive paths and cannot establish time saved
per frame or percentage CPU utilization. Function-size reductions likewise are
not timing percentages. The combined candidate is useful for exactness checks
but cannot establish that both changes should ship together.

## Verification and reproduction

Each candidate (delay, smoothing and combined) passed:

- 180,000 frame-exact transport comparisons against alpha19, with rapidly changing
  per-frame config/controls, all movement paths, heights, solo/dual, queue slot reuse
  and worker starvation/recovery.
- 576,000 additional frame-exact engine comparisons: static and moving scenes,
  room zero/max/changing, silence/impulse/full-scale/random input, delay ring wraps
  and repeated height changes.

The combined primitives additionally passed 6,291,468 slew comparisons (differences
from -262,144 through +262,144 inclusive, three starting offsets, four divisors)
and 7,340,032 delay-read comparisons (all 256 fractions, seven offsets and 4,096
successive writes covering ring wraps). All host checks used AddressSanitizer and
UndefinedBehaviorSanitizer. The scripts were rerun after changing the delay
preparation location; tests and ARM object compilation passed.

[Reproduction script](delay-slew-inspection/check.py), temporary patches and test
templates are kept alongside this report. The script writes only a new temporary
directory and reads unchanged alpha19 source. From the repository root:

```sh
python3 experimental/scene-reference-64/docs/delay-slew-inspection/check.py
# Also compile ARM objects using an existing alpha19 build's recorded flags:
python3 experimental/scene-reference-64/docs/delay-slew-inspection/check.py --arm-build /tmp/spatial-scene-reference64
```

`--arm-prefix` selects another installation of the same ARM toolchain if needed.
Existing build flags can contain machine-specific SDK/vendor paths. No host
wall-clock measurements are offered as RP2040 timing estimates.

## Next experiment

Implement **smoothing only** as an isolated hardware pass with alpha19 source and
UF2 preserved. Repeat exact-output tests, then measure all three modes, room zero
and maximum, independent heights and rapid CV/8mu control changes after fresh
resets. Compare block peaks against the reported alpha19 maximum of 1,158 µs,
with callback below 18 µs, block below 1,200 µs and no queue faults or audio changes.
Keep delay sharing separate until its register-allocation cost has been measured
or removed. No firmware pass was made as part of this inspection.
