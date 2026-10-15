# Spatial Disorientation — 0.2.0-beta1

Two independent mono sources placed and moved in headphone stereo on the Music
Thing Modular Workshop Computer. Inspired by Neuzeit Instruments Quasar, with an
independent implementation. Three startup modes: **Twin Orbits**, **Spatial Mixer**
and **Disorientation** (Fig8, Pendulum, Wander). This beta includes continuous Mixer
height, vertical movement and user-selected sloping paths.

Original code and documentation © 2026 Adrian Vos (soveda), [MIT](LICENSE).
Measured ear filters: Bill Gardner and Keith Martin, MIT Media Laboratory (1994),
*HRTF Measurements of a KEMAR Dummy-Head Microphone*, Technical Report #280;
[separate data terms](vendor/KEMAR/SOURCE_TERMS.md). ComputerCard, Workshop_BlockAudioCard,
WaveSeq/EightMU/rppicomidi, Pico SDK, TinyUSB and linked runtime credits/licenses:
[THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md), [firmware notices](uf2/NOTICE.txt),
[build-component notices](licenses/BUILD_COMPONENTS.md). MIT does not relicense data
or dependencies. No Quasar firmware, graphics or transfer functions are included.

## Get started

1. Download [the beta UF2](uf2/Spatial_Disorientation_0.2.0-beta1.uf2).
   [Checksums](uf2/SHA256SUMS.txt) identify the current binary. Back up existing
   presets with your old editor before using this beta's Save to card.
2. Put the Computer in its normal BOOTSEL firmware-loading mode and copy the UF2
   to the mounted drive. Reset with Main/X/Y near centre.
3. Connect Audio In 1 and/or 2 to mono sources. Unplugged inputs are silent.
   Audio Out 1 is **left ear**, Audio Out 2 **right ear**. Use an appropriate stereo
   headphone monitoring path; connect these modular-level outputs through that path.
4. Normal startup selects Twin Orbits. To select another mode, hold the switch
   **Down during reset**, choose with Main and release to confirm:

| Main region | Mode | Startup LEDs |
|---|---|---|
| Lower third | Twin Orbits | None |
| Middle third | Spatial Mixer | Left column |
| Upper third | Disorientation | Right column |

Startup selection is not saved. Each mode has its own saved settings.
Headphones are the listening target; generic HRTFs vary with listener/headphones.
Elevation may sound subtle or partly like spectral changes rather than a precise
physical position. Speaker stereo/mono will not reproduce the same binaural scene.

## Panel and patch controls

| Control | Twin Orbits | Spatial Mixer | Disorientation |
|---|---|---|---|
| Main | Position offset | Selected-source position | Fig8/Wander front/back depth; Pendulum arc centre |
| X | Signed orbit speed | Selected-source distance | Signed traversal speed |
| Y | Distance | Selected-source level | Path excursion |
| Switch Up | Linked | Select A | Linked |
| Switch Middle | Opposing | Keep selected source | Opposing |
| Down | Reset phase once per press | Select B; release retains B | Hold to freeze phase; release resumes |
| CV 1 | Adds position | Adds both positions | Adds depth/arc centre |
| CV 2 | Adds distance | Adds both distances | Adds excursion |
| Pulse 1 | Movement clock | Unused | Movement clock |
| Pulse 2 | Phase reset | Unused | Phase reset, including while frozen |

X has a centre stop region; its manual maximum is ±1.5 cycles/second. Clock input
sets movement cycle length with the editor's pulses/cycle (default 4). CV outputs
stay nominal zero; pulse outputs stay low. Mixer uses soft pickup after selecting,
restoring or remotely editing a source; move Main/X/Y near or through the stored
value to regain control. Each source retains its position/distance/level.

Fig8 is a crossing path with independently phased sources. Pendulum sweeps an arc
through the selected centre; Wander follows repeatable smoothly eased targets.
Changing movement preserves phase. Freeze holds phase only: knobs/CV/live motion
can still reshape held positions. Pulse reset and 8mu A still reset while frozen.

## Web editor and presets

Use the **matching beta editor**, [web/index.html](web/index.html), in desktop
Chrome/Edge with WebMIDI/SysEx permission. Use a USB-C **data** cable, close Serial
Monitor/other MIDI apps and choose “Spatial Disorientation” for both ports.
iOS is unsupported. If opening the local file blocks MIDI, run this from the
unpacked tester bundle or repository:

```sh
python3 -m http.server 8804 --bind 127.0.0.1 --directory web
```

Open `http://127.0.0.1:8804`. Hosted editors require HTTPS. This package does not
publish a new hosted editor automatically; an old alpha editor is incompatible.
Select the desired startup mode before connecting or applying its settings.

Changes wait for **Apply**; there is no automatic send. **Read** replaces displayed
edits with current card values. **Save to card** persists applied settings and
briefly fades audio for the flash write. **Reset to init** applies factory editor
settings for the active mode; it does not save automatically. Export downloads the
displayed settings; Import stages them until Apply. Preset names live in JSON
files, not card flash. Three separate settings banks preserve inactive modes.

Shared settings: separation/phase separation, room reflections, A/B level trims,
spatial strength and clock division. Separation and clock division are unused in
Mixer. Mixer Read/Save capture both current placements and applied heights.

| Height/path setting | Behaviour |
|---|---|
| Mixer A/B height | Continuous −40° to +40°, independently saved/exported |
| Disorientation height centre | −40° to +40°, both sources |
| Vertical excursion | 0–40° half-range; phase-related Fig8, lifted Pendulum ends, eased Wander height |
| Uphill direction | 0° front, 90° right, 180° back, 270° left; continuous diagonals |
| Path tilt | 0–40° inclination; zero bypasses slope |

Slope rotates horizontal direction toward the chosen uphill bearing and changes
its horizontal projection. The opposite bearing goes down; perpendicular
positions remain level. Path distance stays the original path distance. This is
inclination about the listener, not rotation of the translated path about its own
centre. Existing height centre/bob is added after slope, then clipped to ±40°;
combined bob+tilt is not one rigid plane and large settings can flatten peaks.
Start at centre **0°**, excursion **0°**, tilt **40°**, slow X to audition slope
alone. Reset can return to an elevated point if that reset direction is uphill.
The live diagram shows direction only; its radius does not represent distance.

### Migration and downgrade

Beta uses **SysEx 6 / preset JSON 6 / flash 7**. It reads flash 1–6 and preset files
1–5, retaining supported old fields and defaulting missing height to level, vertical
excursion/tilt to zero. No migration writes flash automatically. Alpha26 settings
are already compatible. Alpha25 and older editors reject the beta protocol.

**Export your old presets before beta Save.** Apply-only beta testing leaves old
flash intact. After beta Save, alpha25/older firmware cannot read flash 7 and uses
defaults; restoring old saved banks requires old JSON exports, the matching old
editor and explicit Save in each startup mode. Old editors cannot import beta JSON6.
[Archived versions and checksums](archive/README.md) remain recoverable.

## Direct 8mu control

Connect 8mu and reset separately from an editor session. On hardware Rev1.1 USB
power role selects host/device at boot; older boards retain editor/device operation.
Reset after changing USB roles. Mixer ignores accelerometer/gyro; other modes
can use motion to replace Main. Motion starts off; CV remains additive.

| 8mu fader | Twin Orbits | Mixer | Disorientation |
|---|---|---|---|
| 1 | Speed/direction | Selected distance | Speed/direction |
| 2 | Distance | Selected level | Excursion |
| 3 | Source separation | Selected position | Phase separation |
| 4 | Room | Room | Room |
| 5 / 6 | A / B trim | A / B trim | A / B trim |
| 7 | Spatial strength | Spatial strength | Spatial strength |
| 8 | Motion amount | Selected-source continuous height | Motion amount |

Faders use pickup near/crossing existing values, with waiting LED feedback. Mixer
source changes rearm source-specific faders; other source values stay put.
Mixer A/B select sources; C is unused; D held shows timing. Mixer LED8 identifies
A steady/B blinking with height brightness and pickup indication.

For Orbits/Disorientation: A resets phase and recentres at Main/current pose;
B toggles motion; C held freezes automatic traversal. Roll offsets the anchor;
yaw rate integrates a held position/depth with deadband and bounded controls.
In Disorientation, **tap D and release within 500 ms** cycles Fig8/Pendulum/Wander;
hold D at least 500 ms for diagnostics without cycling. LEDs1/2/3 briefly identify
movement. Orbits D is diagnostics only. No new live height/tilt mapping is assigned
in Disorientation; configure and Save those settings with the editor before the
8mu boot. Mixer fader8 height remains available.

8mu never writes flash. Live motion, phase and trim overrides are excluded from
saved presets. Main/X/Y use pickup on handback from Disorientation/Mixer ownership.
Editor and 8mu cannot be tested simultaneously on the card's one USB controller.

## LEDs, timing and known limits

The four upper LEDs show source A/B left/right direction pairs; equal brightness
can mean front or back. Mixer selected pairs blink while panel pickup is pending.
Bottom-left indicates linked movement or editing A in Mixer. **Bottom-right latches
a timing/queue warning**: callback ≥18 µs, block ≥1200 µs, or a queue fault.
Reset clears peaks/warnings. Startup LED patterns override runtime feedback.

Hold 8mu D for offline peak block-time bands: each lit LED represents 150 µs;
eight lit means above 1050 µs. All eight flash if a timing/queue warning latched.
A lit band alone is not a warning. The editor reports exact latched peaks and
separate slow-DSP/callback/queue flags. Reboot between comparisons.

ComputerCard **0.4.0**, **192 MHz / 1.15 V**, **48 kHz**, **two sources / 64 taps**.
Core0 captures audio/controls; core1 renders 64-frame DSP blocks and services
bounded USB work. Added transport latency is 128 frames, about **2.67 ms**. Block
deadline is **1333.3 µs**; 1200 µs is an early warning. A missed output block can
produce 64 frames of silence; late audio is discarded. Flash saves intentionally
pause/fade audio and are excluded from timing peaks.

User-reported alpha26 slope stress results underlying this beta:

| Movement | Duration | Callback / block peak |
|---|---|---|
| Pendulum | 25 minutes | 14 / 1175 µs; audible slope, no clicks/gaps/queue warnings |
| Fig8 | 20 minutes | 14 / 1176 µs; no separate listening/queue report supplied |
| Wander | 30 minutes | 14 / 1193 µs; clean audio, no queue warnings |

Audio-rate CV was used in the preceding tests. Exact stress settings and USB role
were not fully logged. Wander leaves only **7 µs below the warning**; these results
cover one tester/setup, not every board/headphone/USB load. Beta1 ships the exact
alpha26 UF2 bytes; the root rebuild differs only in its embedded build-date byte.
Independent testing should cover saved settings, separate 8mu boots and worst-load
CV. [Tester protocol and feedback template](docs/TEST_PROTOCOL.md).

## Source and build

Build with Pico SDK2.3.0 / Arm GNU Toolchain15.2.Rel1 or compatible tools:

```sh
cmake -S . -B build_beta -DPICO_SDK_PATH=/path/to/pico-sdk -DCMAKE_BUILD_TYPE=Release
cmake --build build_beta -j2
```

The beta defaults to64 taps/two sources. Source-A-only is diagnostic, not this
package. [Build evidence](docs/BUILD_STATUS.md), [protocol](docs/PROTOCOL.md),
[design record](docs/DESIGN.md), [future options](docs/OPTIONS.md).
Pre-generated measured tables ship with the source; building firmware needs no
Python scientific libraries or measurement download. Mathematical slope-table
reproduction uses `tools/generate_slope_table.py`; measured-bank provenance and
archived elevation generator are recorded in the notices. Old preview audio is
historical, not proof of the beta's sound. [Archive](archive/README.md) holds earlier
versions; current `uf2/` contains only beta1 and its notices/checksum.

The next alpha is a separate [Twin Orbits height/slope experiment](experimental/orbits-height-64/README.md).
It has its own firmware/editor and persistence version; beta1 above remains the
tester build. Original experimental code/docs ©2026 Adrian Vos(soveda),MIT;
existing KEMAR/data/dependency terms are preserved. Hardware validation is pending.
