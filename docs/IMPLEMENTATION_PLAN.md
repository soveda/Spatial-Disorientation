# Spatial Disorientation implementation plan

Updated 2026-10-08. Original plan © 2026 Adrian Vos (soveda), MIT.
User requirements and hardware reports are the basis for this plan. Platform/API
sources and musical inspiration are credited in ../THIRD_PARTY_NOTICES.md.
The stages below record the original plan and subsequent checkpoints. Alpha6 is
the current room experiment; other functions and 8mu remain planned.

## 1. Finish the Twin Orbits listening/timing pass

Compare alpha1 and alpha2 with identical sources/settings. Distinguish audible
front/back spectral colour from convincing spatial placement. Preserve the tested
left/right behaviour, controls, clock/reset and editor persistence. Complete the
new version's stability run and profile full ISR timing at worst load at 192 MHz.
Alpha1 has 20 minutes of issue-free stability reported; alpha2 starts a new run.

If stronger analytical cues still feel like ordinary tonal changes, evaluate
short direction-dependent FIR/HRTF filters as a separate experiment. Select a
compatible licensed dataset, attribute it fully, estimate RAM/CPU before adoption,
and compare listener results. Do not claim accurate elevation from simple filtering.
Block processing is optional if the eventual filter workload warrants it.

## 2. Integrate 8mu using existing working cards

Reuse established patterns for USB host/device roles, MIDI parsing and core 1
ownership. Workshop_Computer/releases/801_WaveSeq documents both direct 8mu host
control and computer-connected editor forwarding; releases/105_voder documents
motion mappings and control ownership. Inspect their actual code/dependencies,
retain the relevant licenses and credit any reused implementation. References:
https://github.com/TomWhitwell/Workshop_Computer/tree/main/releases/801_WaveSeq
https://github.com/TomWhitwell/Workshop_Computer/tree/main/releases/105_voder

Add live MIDI CC mapping for position, rate, distance and separation, including
motion gestures. Confirm the user's 8mu version and available motion outputs
before promising raw gyro axes. Provide sensitivity, neutral/recentre behaviour,
smoothing and sensible motion limits. Decide additive modulation versus pickup
for controls shared with panel knobs, including return to panel on disconnect.
Support editor forwarding where appropriate. Stress-test rapid motion/MIDI traffic
with audio active; measure rather than quote an untested CPU percentage.

## 3. Extend configuration and presets

Add versioned preset import/export and per-function saved settings, preserving
Read/Apply/Save separation and migration of valid existing settings. Add mappings
for 8mu without making ordinary playing depend on a computer. Consider independent
source offsets/distances and elevation only alongside the corresponding DSP.
Retest malformed messages, disconnect/reconnect and power-cycle persistence.

## 4. Add the two alternative functions

Spatial Mixer: independently place the two sources, with clear source selection
and a consistent physical control layout. Disorientation: intentional motion paths
and near/far movement, with explicit control of Doppler character. Define each
function's controls before implementation, using the common spatial renderer.

Save the default function through the editor and add a simple startup selector;
only one function executes at a time. Agree the startup gesture and knob roles,
avoid hidden menus, and keep mode indication clear. A function selector belongs
with actual implementations, not inactive options in the alpha editor.

## 5. Prepare release only after validation

Finalize controls/LED feedback, test monitoring/mono compatibility and headroom,
profile timing, complete stability/USB/flash tests on available board/card variants,
and update the operator guide, source attribution and MIT notices. Validate release
metadata against the then-current canonical Workshop Computer schema. Generate a
versioned UF2 with checksums and a concrete release/PR summary. Remain in this
independent repository until the user approves copying into releases.

## Alpha3 experiment and block evaluation — 2026-10-08

Alpha2 blind listening remained insufficient. The authorized alpha3 experiment
implements the short measured-filter evaluation described in stage 1 using
Gardner/Martin MIT KEMAR data. Keep alpha2 available for comparison. Next priority:
blind horizontal localization tests and actual ISR measurements, then decide
whether 32-tap approximation quality and per-sample timing are sufficient.

The local Workshop_BlockAudioCard reference was read at the user's request. It
uses 64-frame handoff, core 1 DSP and two-block output scheduling (~2.7 ms), direct
hardware access rather than ComputerCard and no initial jack normalization. Moving
here would require explicit restoration of ComputerCard-equivalent normalization,
clock/reset capture, editor/flash coordination and USB scheduling. No block code
is copied. For 32-tap FIR, fixed-point direct convolution is a reasonable first
experiment; grouping samples does not by itself lower the operation count. Longer
FIRs may justify partitioned convolution or a revised block architecture. Measure
worst-load deadlines before choosing. Do not treat the compiler/memory report as
CPU profiling or a claim that the current callback is fast enough.

## Alpha4 supersedes the per-sample scheduling decision

Alpha3's hardware failure demonstrated that keeping FIR work inside the audio
callback was unsuitable, despite the short filter count. Alpha4 now adapts the
Workshop_BlockAudioCard handoff while retaining ComputerCard hardware service.
The required order is: validate responsive controls and callback/block timings;
validate cooperative USB/persistence under load; then blind localization and
stability. Do not extend filters or add 8mu until those deadlines are measured.
Core 1 now owns both block rendering and bounded USB, so any future host integration
must preserve audio priority and non-blocking service. The spatial filter bank
is unchanged from alpha3; this pass corrects scheduling, not localization tuning.

## Alpha5: current experiment

Optimize repeated per-ear work, then compare matched-gain 32- and 64-tap banks.
The source archive is unchanged; no additional HRTF profile is introduced yet.
First measure worst-load savings on opt32 against alpha4's 1181 us/block. Then
measure the longer bank before accepting any listening improvement. The 64-tap
bank reduces numerical magnitude error, but should be kept only if it helps blind
localization and retains timing margin. Continue with other generic profiles or
listener calibration if longer filters still do not provide sufficient cues.
8mu and other function work remains behind this timing/listening checkpoint.

## Alpha6: current externalization pass

Alpha5 hardware reports favor 32 taps: 916 us/block versus 1244 us with 64 taps,
with no noticeable listening improvement from 64. Keep alpha5-opt32 as baseline.
The approved alpha6 pass adds synthetic directional early reflections and increases
the reflected/direct balance with distance, using the existing room control and
32-tap bank. Validate deadlines under USB load first, then blind outside-head size,
front/back movement, distance and echo/colour tradeoffs. If room only adds width or
slap, evaluate another attributed generic HRTF profile or listener calibration
before expanding functions. 8mu/startup function work remains planned.

## Alpha8: initial direct 8mu pass

Alpha6 stability and alpha7 speed checks are accepted by the user. Alpha8 implements
motion takeover of Main and pickup faders for X/Y/five continuous settings, with
an eighth motion-depth fader and offline timing LEDs. No simultaneous editor is
required; clock division stays in saved settings. Prioritize direct-connect USB,
pickup, recenter/toggle, axis polarity/rate behaviour, disconnect recovery and full
load without warnings. Confirm editor mode separately after reset. USB work shares
core 1 with DSP; host enumeration waits must service the worker. Mapping presets,
sensitivity options, startup functions and other spatial modes remain planned.
