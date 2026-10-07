# Sources and attribution

Original project code and documentation: soveda, 2026, MIT; see LICENSE.

## ComputerCard 0.4.0

ComputerCard by Chris Johnson, copyright 2024–2026, MIT.
The unmodified header is in vendor/ComputerCard/ComputerCard.h and the upstream
license file is preserved beside it. The header also includes the full MIT notice.
The CMake setup follows ComputerCard's example build conventions; the initial
firmware follows its passthrough example and README (Chris Johnson).
pico_sdk_import.cmake is copied from the same upstream directory; the Raspberry Pi
copyright and BSD-3-Clause notice are preserved in vendor/PicoSDK/LICENSE.TXT.

Source: https://github.com/TomWhitwell/Workshop_Computer/tree/7e2b0416093b397b5512de3f111472bd91084c6e/Demonstrations%2BHelloWorlds/PicoSDK/ComputerCard

## Workshop Computer documentation

The development directive is an unmodified reference copy from the Music Thing
Modular Workshop Computer repository, revision 7e2b0416093b397b5512de3f111472bd91084c6e.
Its source does not include a separate author or license notice; this project's
MIT license does not relicense that upstream document. Consult upstream for reuse.
Platform and metadata guidance is credited to the Workshop Computer maintainers,
including Tom Whitwell and Chris Johnson.

Source: https://github.com/TomWhitwell/Workshop_Computer/blob/7e2b0416093b397b5512de3f111472bd91084c6e/Demonstrations%2BHelloWorlds/AI/WORKSHOP_COMPUTER_AI_DIRECTIVE.md
Metadata: https://github.com/TomWhitwell/Workshop_Computer/blob/7e2b0416093b397b5512de3f111472bd91084c6e/documentation/info.yaml.md

## Pico SDK

Builds use an externally installed Raspberry Pi Pico SDK. Its components retain
their own licenses. Preserve applicable SDK and dependency notices when distributing
firmware. The SDK itself is not vendored in this repository.

## Musical inspiration

Neuzeit Instruments Quasar is the inspiration for two-source binaural positioning
and orbital movement. Product reference: https://www.neuzeit-instruments.com/Quasar
No Quasar firmware, manual text, graphics, or transfer-function data is included.
This is an independent project, without an affiliation or endorsement claim.

Clock and regulator setup follows Chris Johnson's ComputerCard NOTES.md
(Programming and optimisation / Clock speed) from the same upstream directory
referenced above: 192 MHz with the regulator at 1.15 V.
