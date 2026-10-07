# Spatial Disorientation

Two-source binaural spatializer inspired by Neuzeit Instruments Quasar for Music Thing Modular Workshop Computer.

**Status: initial development scaffold, version 0.0.1.** Effect DSP has not been
implemented. Current firmware is a stereo passthrough smoke check: the left LED
column shows Main/X/Y, the right column shows up/middle/down switch position.
CV outputs are zero and pulse outputs low. It has not been tested on hardware.

The approved target controls and milestones are in [docs/DESIGN.md](docs/DESIGN.md).
This standalone repository has no build dependency on the Workshop_Computer checkout.

## Build

Install the Raspberry Pi Pico SDK and ARM embedded toolchain, set PICO_SDK_PATH,
then run:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j2
```

Output: `build/spatial_disorientation.uf2`. This is scaffold firmware, not a finished effect.
Build artifacts are ignored; release firmware will be added when ready.
Uses vendored ComputerCard **0.4.0**, a 192 MHz CPU clock at 1.15 V, a 48 kHz audio callback,
RAM execution and oscillator startup multiplier 64. USB/UART stdio are disabled.

## License and credits

Original code and documentation: copyright 2026 soveda, MIT; see [LICENSE](LICENSE).
ComputerCard: Chris Johnson, MIT. See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md)
for upstream sources, preserved notices and documentation credits.
