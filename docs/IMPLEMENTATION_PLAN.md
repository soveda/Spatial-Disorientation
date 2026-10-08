# Spatial Disorientation implementation plan

Updated 2026-10-08. Original plan © 2026 Adrian Vos (soveda), MIT.
User requirements and hardware reports are the basis for this plan. Platform/API
sources and musical inspiration are credited in ../THIRD_PARTY_NOTICES.md.
Only the alpha2 DSP pass is implemented here; later stages are planned.

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
