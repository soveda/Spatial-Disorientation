# Spatial Disorientation — Twin Orbits

**0.1.0-alpha3-hrtf**: experimental measured-filter prototype for Music Thing Modular Workshop
Computer. Two independent mono inputs orbit around the listener and mix to binaural
stereo. Listen on headphones. Uses ComputerCard **0.4.0**, 48 kHz audio and
**192 MHz / 1.15 V**. Firmware builds and host checks pass. User hardware tests
on **alpha1** pass for the main controls, left/right movement and editor/persistence;
front/back cues were weak. Alpha1 stability is ongoing (20 minutes without issues
reported on 2026-10-08). Alpha2 improved tonal distinction but front/back movement remained insufficient
with the visual display hidden. Alpha3 HRTF listening, stability and full interrupt
timing measurements remain pending.

Original code and documentation © 2026 Adrian Vos (soveda), MIT. Hardware/library
patterns: Chris Johnson and the Workshop Computer contributors. Musical inspiration:
[Neuzeit Instruments Quasar](https://www.neuzeit-instruments.com/Quasar).
See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) for sources and licenses.
This is an independent implementation; no Quasar DSP is used. The new derived
HRTF bank credits **Bill Gardner and Keith Martin, MIT Media Laboratory (1994)**;
see [dataset terms and processing](vendor/KEMAR/SOURCE_TERMS.md). Measurements
retain their own attribution terms; original project code remains MIT.

## Try it

Flash `uf2/Spatial_Disorientation_Twin_Orbits_0.1.0-alpha3-hrtf.uf2` using the usual
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

Each source has fractional arrival delays, measured direction-dependent ear
filters, distance attenuation and short feed-forward room reflections. The
horizontal filters derive from Gardner and Martin's diffuse-field-equalized MIT
KEMAR data. The short minimum-phase filters replace alpha2's rear notch, panning
law and direction-dependent one-pole shadow filter; interaural delay remains
analytic. Mild distance darkening retains more high-frequency directional detail.
No elevation or individual ear calibration is implemented. Generic short HRTFs
may still produce front/back confusion; test by ear with the display hidden.

The original alpha1 previews in `previews/` use the actual alpha1 C++ DSP with original synthetic sources:
a 220 Hz harmonic tone on A and noise percussion on B. Both are 12 seconds, stereo
48 kHz/16 bit, with identical levels, settings and source signals. One uses linked
motion and one opposing motion. They simulate DSP only, without the physical
ADC/DAC, USB activity or hardware timing. Later version-labelled files use that
version's renderer. Generation source: `tools/render_preview.cpp`.

## Alpha3 HRTF listening comparison

Use a bright mono signal in Audio 1, Audio 2 empty, X noon, Y down, room zero and
spatial strength full. Reset at Main noon for front, then turn Main to either end
for back. Hide the editor display. Test stationary positions, then a slow orbit;
judge perceived direction separately from tonal differences. Repeat with room
and distance raised only after testing the near, dry sound.

The ear filters now vary across 72 measured horizontal directions. Rear is **not**
necessarily darker: the measured frequency patterns replace the artificial rear
colour. Spatial strength smoothly blends from neutral at zero to the measured
bank at full, also scaling the separate interaural delay. A single conservative
bank gain preserves ear/direction level differences and provides FIR headroom;
volume can differ from alpha2 and with strength changes. Compare at comfortable,
matched monitoring levels. Saved settings/schema and hardware controls are unchanged.

`previews/alpha3-front-back.wav` alternates front/back every 3 seconds with the
same broadband source, stationary A, distance near and room zero. Alpha3 linked
and opposing previews use both synthetic sources. All include the derived KEMAR
filters and their Gardner/Martin attribution. Alpha1/alpha2 previews and firmware
are retained for comparison. No alpha3 stability result is inherited from alpha1.

The 32-tap filters keep this experiment per sample. Workshop_BlockAudioCard was
reviewed: its reference uses 64-frame blocks, core 1 rendering and two-block
scheduling (~2.7 ms). It does not directly preserve this card's ComputerCard 0.4.0,
USB-on-core-1 and jack-probe arrangement. A block adaptation remains an option for
longer filters or measured timing problems; buffering alone does not reduce FIR
multiply count. See docs/IMPLEMENTATION_PLAN.md.

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
/tmp/spatial-render previews/alpha3-linked.wav 1
/tmp/spatial-render previews/alpha3-opposed.wav 0
/tmp/spatial-render previews/alpha3-front-back.wav front-back
```

DSP stays on core 0; USB MIDI/editor stays on core 1. Staggered geometry updates
avoid recalculating both sources on the same sample. Audio remains per sample;
this first fixed-point implementation does not need FFT/convolution blocks.
The program executes from RAM. Build use: 50,880 bytes flash; 65,912 bytes main RAM,
plus 2 KB in each scratch bank. See [docs/TEST_PROTOCOL.md](docs/TEST_PROTOCOL.md)
for instrument checks and [docs/PROTOCOL.md](docs/PROTOCOL.md) for editor messages.
This independent repository is not a Workshop_Computer release submission.
