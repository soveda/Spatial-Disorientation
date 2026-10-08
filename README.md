# Spatial Disorientation — Twin Orbits

**0.1.0-alpha2**: second listening-test prototype for Music Thing Modular Workshop
Computer. Two independent mono inputs orbit around the listener and mix to binaural
stereo. Listen on headphones. Uses ComputerCard **0.4.0**, 48 kHz audio and
**192 MHz / 1.15 V**. Firmware builds and host checks pass. User hardware tests
on **alpha1** pass for the main controls, left/right movement and editor/persistence;
front/back cues were weak. Alpha1 stability is ongoing (20 minutes without issues
reported on 2026-10-08). Alpha2 listening reports substantially clearer front/back cues, weaker at greater
distance, with some influence from the visual path display. Further listening,
alpha2 stability testing and full interrupt timing measurements remain pending.

Original code and documentation © 2026 Adrian Vos (soveda), MIT. Hardware/library
patterns: Chris Johnson and the Workshop Computer contributors. Musical inspiration:
[Neuzeit Instruments Quasar](https://www.neuzeit-instruments.com/Quasar).
See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) for sources and licenses.
This is an independent implementation; no Quasar DSP or measured HRTF data is used.

## Try it

Flash `uf2/Spatial_Disorientation_Twin_Orbits_0.1.0-alpha2.uf2` using the usual
Workshop Computer BOOTSEL procedure. Start with a mono sound in Audio 1, both audio
outputs connected to the left/right sides of a headphone monitoring path, Main and
X at noon, Y down. Turn X right to start an orbit; turn left for reverse motion.
Patch a different sound into Audio 2 to hear the second source.

| Control | Behaviour |
|---|---|
| Main | Full-turn position offset; noon puts A at the front after reset |
| X | Signed orbit speed: centre deadband stops; either extreme approaches 2 turns/sec |
| Y | Near to far: quieter, darker, later, with more room contribution |
| Switch up | Linked motion; separation stays constant |
| Switch middle | Opposing motion; A and B rotate in opposite directions |
| Switch down | Reset orbit phase once per press; retains previous motion relationship while held |
| CV 1 | Adds position offset; approximately ±6 V adds ±one full turn |
| CV 2 | Adds distance; approximately ±6 V spans the knob's full range, clamped at either end |
| Pulse 1 | Clock: default 4 pulses/turn; X still selects direction or stop |
| Pulse 2 | Rising edge resets orbit phase |
| Audio 1 / 2 in | Independent sources A / B; unplugged inputs are silent |
| Audio 1 / 2 out | Left / right binaural mix |
| CV / pulse outputs | Zero / low |

X's stationary region is approximately ±6% of its travel around noon. Clock lock
requires two edges; accepted edge intervals are 25 ms–60 s. Speed is capped at
2 turns/sec. After no clock for the greater of 3 seconds or 3 clock intervals,
manual X speed resumes. Unpatching the clock also restores manual speed. Use pulse
widths of at least 1 ms for hardware testing. Reset restores the phase relative to
Main and the editor's separation, and motion resumes immediately. CV values are
nominal, not calibrated position/distance measurements.

Top LED pair shows A's left/right position; middle pair shows B. Equal brightness
means front **or** back, not necessarily stopped. Bottom left is lit for linked
motion. Bottom right latches if the measured callback takes at least 18 µs; reset
clears it. This is a useful warning, not proof of total interrupt timing margin.

## Editor

Open `web/index.html` in desktop Chrome/Edge. If local file MIDI access is blocked,
serve it from localhost:

```sh
python3 -m http.server 8793 --bind 127.0.0.1 --directory web
```

Visit http://127.0.0.1:8793. Use a USB-C data cable; close Serial Monitor/other apps.
Enable MIDI and allow SysEx, select **Spatial Disorientation** as both MIDI ports,
then Connect & read. iOS is unsupported. The page is a single static file with no
external scripts or services.

- **Read:** fetch applied settings, replacing edits in the page.
- **Apply:** audition separation, room amount, A/B levels, spatial strength and
  clock pulses/turn. Main/X/Y keep their own roles; no editor/knob takeover needed.
- **Save to card:** persist applied settings across reset. Audio briefly fades out
  during the flash write. Apply pending edits first; Save is disabled while dirty.

Defaults: 180° separation, 29% room, 50% level per source, full spatial strength,
4 clock pulses/turn. Level controls provide headroom; their maximum is not an
automatic loudness compensation. A corrupt or absent record restores defaults.
Only explicit Save writes flash; the last 4 KB of the actual card capacity is
reserved. Reflashing may replace or invalidate saved settings; Read afterwards.

Only **Twin Orbits** runs in this prototype. Elevation, preset files, Spatial Mixer,
Disorientation and startup function selection remain future work.

## What it sounds like

Each source has fractional arrival delays, level differences between the ears,
far-ear and stronger rear filtering, a smoothly blended rear spectral notch, distance attenuation and short feed-forward room
reflections. Delays and gains move smoothly. Front/back differentiation is subtle
and listener dependent; this is an approximate binaural model, not a measured
personal HRTF. Stereo speakers reduce the intended binaural effect.

The previews in `previews/` use the actual C++ DSP with original synthetic sources:
a 220 Hz harmonic tone on A and noise percussion on B. Both are 12 seconds, stereo
48 kHz/16 bit, with identical levels, settings and source signals. One uses linked
motion and one opposing motion. They simulate DSP only, without the physical
ADC/DAC, USB activity or hardware timing. Generation source: `tools/render_preview.cpp`.

## Alpha2 listening comparison

Rear positions now blend in a short three-sample feed-forward filter (first
cancellation at 8 kHz), alongside stronger rear high-frequency shadowing. The
front should sound open/bright; the rear darker and more hollow. Low-frequency
level is largely retained. Spatial strength scales both cues and zero bypasses
directional colouring. This is an analytical approximation, not measured pinna
filtering; a tonal distinction does not guarantee externalized rear perception.
Controls, parameter IDs and saved settings format are unchanged from alpha1.

`previews/alpha2-front-back.wav` alternates **front / back / front / back** every
3 seconds, using the same broadband noise segment, room zero, distance near,
stationary A only. Compare on headphones. The linked/opposing files are retained
alpha1 references; `alpha2-linked.wav` and `alpha2-opposed.wav` use the revised DSP.
The previous UF2 is retained for A/B comparison. Alpha1's reported stability does
not establish alpha2 stability. See docs/IMPLEMENTATION_PLAN.md for remaining work.

## Build and verify

Requires an installed Raspberry Pi Pico SDK (this build: 2.3.0), CMake and
ARM GCC (this build: 15.2.1).

```sh
cmake -S . -B build -DPICO_SDK_PATH=/path/to/pico-sdk
cmake --build build -j2
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=undefined,address -Isrc tests/dsp_test.cpp -o /tmp/spatial-dsp-test
/tmp/spatial-dsp-test
node tests/editor_test.cjs
clang++ -O2 -std=c++17 -Isrc tools/render_preview.cpp -o /tmp/spatial-render
/tmp/spatial-render previews/alpha2-linked.wav 1
/tmp/spatial-render previews/alpha2-opposed.wav 0
/tmp/spatial-render previews/alpha2-front-back.wav front-back
```

DSP stays on core 0; USB MIDI/editor stays on core 1. Staggered geometry updates
avoid recalculating both sources on the same sample. Audio remains per sample;
this first fixed-point implementation does not need FFT/convolution blocks.
The program executes from RAM. Build use: 44,064 bytes flash; 58,064 bytes main RAM,
plus 2 KB in each scratch bank. See [docs/TEST_PROTOCOL.md](docs/TEST_PROTOCOL.md)
for instrument checks and [docs/PROTOCOL.md](docs/PROTOCOL.md) for editor messages.
This independent repository is not a Workshop_Computer release submission.
