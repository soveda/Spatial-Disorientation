# Initial scaffold build verification

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
