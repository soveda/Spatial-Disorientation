# Spatial Disorientation — 0.3.0-beta2

Two independent mono sources placed and moved in headphone stereo on the Music
Thing Modular Workshop Computer. Inspired by Neuzeit Instruments Quasar, with an
independent implementation. Three startup modes: **Twin Orbits**, **Spatial Mixer**
and **Disorientation** (Fig8, Pendulum, Wander). This beta includes Orbits height/slope and two8mu pages, continuous Mixer
height, vertical Disorientation, elevation randomise/reset tools and the tested
repeated-save fix.

Original code and documentation © 2026 Adrian Vos (soveda), [MIT](LICENSE).
Measured ear filters: Bill Gardner and Keith Martin, MIT Media Laboratory (1994),
*HRTF Measurements of a KEMAR Dummy-Head Microphone*, Technical Report #280;
[separate data terms](vendor/KEMAR/SOURCE_TERMS.md). ComputerCard, Workshop_BlockAudioCard,
WaveSeq/EightMU/rppicomidi, Pico SDK, TinyUSB and linked runtime credits/licenses:
[THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md), [firmware notices](uf2/NOTICE.txt),
[build-component notices](licenses/BUILD_COMPONENTS.md) and the
[local ComputerCard0.4.0 flash-pause adaptation](vendor/ComputerCard/PATCHES.md). MIT does not relicense data
or dependencies. No Quasar firmware, graphics or transfer functions are included.

## Get started

1. Download [the beta UF2](uf2/Spatial_Disorientation_0.3.0-beta2.uf2).
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

At the bottom, **Randomise elevation** stages new height/slope values for the
shown mode; **Reset elevation** stages level height and zero excursion/tilt.
These tools leave other sound settings alone and send nothing until Apply.
Random combinations avoid immediate height clipping. Mixer randomises/resets A/B
height only. Orbits Apply/init checks height/tilt readback and reports a mismatch
rather than claiming success if the card returns old values.

Shared settings: separation/phase separation, room reflections, A/B level trims,
spatial strength and clock division. Separation and clock division are unused in
Mixer. Mixer Read/Save capture both current placements and applied heights.

| Height/path setting | Behaviour |
|---|---|
| Orbits height centre | −40° to +40°, both sources, independently saved from Disorientation |
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

Beta2 uses **SysEx7 / preset JSON7 / flash8**, compatible with the0.3.0 alpha1–3
experiments. It reads flash1–7 and imports JSON1–6, preserving supported old fields.
Migrating beta1 defaults only the new Orbits height/slope to level/zero tilt.
Older missing Mixer heights default to level; missing vertical excursion/tilt to
zero. No automatic boot migration write. Beta1’s editor uses an older protocol;
use the matching beta2 editor. Each mode still has its own settings bank.

**Export old presets before beta2 Save.** Apply-only leaves old flash intact.
After beta2 Save, beta1 cannot read flash8 and starts defaults. To downgrade,
load the archived beta1 UF2/editor, import your old exports and explicitly Save
in each desired startup mode. Beta1 cannot import JSON7. Alpha1/2/3 have the same
formats as beta2 and need no schema downgrade. [Archived versions](../README.md).

## Direct 8mu control

Connect8mu and reset separately from an editor session. Rev1.1 detects USB power
role at boot; older boards retain editor/device operation. Reset after changing
roles. Mixer ignores motion; Orbits/Disorientation can replace Main with motion.
Motion starts off, and CV remains additive. No8mu gesture writes flash.

**Twin Orbits D tap/release under500ms toggles normal and height/slope pages.**
Holding D at least500ms shows timing; releasing that long hold keeps the page.
Normal is selected on startup/connection. Brief8mu indication after page changes:
**LEDs1–3 = normal; LEDs6–8 = height**, lit for1second. Then pickup/level feedback
resumes: there is no persistent page indicator. These are8mu LEDs, not Computer LEDs.

| Fader | Orbits normal | Orbits height/slope | Mixer | Disorientation |
|---|---|---|---|---|
|1|Speed/direction|Height −40°..+40°|Selected distance|Speed/direction|
|2|Distance|Tilt0..40°|Selected level|Excursion|
|3|Separation|Uphill direction|Selected position|Phase separation|
|4|Room|Room|Room|Room|
|5/6|A/B trim|A/B trim|A/B trim|A/B trim|
|7|Spatial strength|Spatial strength|Spatial strength|Spatial strength|
|8|Motion amount|Motion amount|Selected-source height|Motion amount|

Faders1–3 rearm pickup on each Orbits page switch. Remote-owned speed/distance/
separation stay held while editing height; return to normal waits for pickup.
Height/slope values also remain when leaving their page. Shared faders4–8 retain
ownership. A fader waiting for pickup blinks. Normal panel controls remain available
when not remotely owned. No change to Disorientation/Mixer page behaviour.

Orbits/Disorientation A resets/recentres at Main/current pose, B toggles motion,
C holds phase. Motion/CV can still reshape held positions. C release restores the
prior speed. Roll offsets the anchor and yaw rate integrates position/depth.
Fader8 remains motion amount, including on Orbits height page; no new gyro axis.
Disorientation D tap cycles Fig8/Pendulum/Wander, with LEDs1/2/3 briefly identifying
movement; longD keeps timing. Mixer A/B select sources, C is unused and D is timing.
Mixer LED8 identifies A steady/B blinking, with height brightness/pickup feedback.

Orbits disconnect restores saved height/slope and disables motion. Remotely owned
X/Y wait for panel pickup; reconnect starts normal with fresh fader pickup. Runtime
8mu values do not change saved presets. For persistent height/slope use the editor
and Save in a separate USB boot. Other8mu mapping/ownership rules remain unchanged.

## LEDs, timing and known limits

The four upper LEDs show source A/B left/right direction pairs; equal brightness
can mean front or back. Mixer selected pairs blink while panel pickup is pending.
Bottom-left indicates linked movement or editing A in Mixer. **Bottom-right latches
a general warning**: callback ≥18 µs, block ≥1200 µs, a queue fault or a failed save/resume (once callbacks run).
Reset clears peaks/warnings. Startup LED patterns override runtime feedback.

Hold 8mu D for offline peak block-time bands: each lit LED represents 150 µs;
eight lit means above 1050 µs. All eight flash if a timing/queue/save warning latched.
A lit band alone is not a warning. The editor reports exact latched peaks and
separate slow-DSP/callback/queue flags. Reboot between comparisons.

ComputerCard **0.4.0**, **192 MHz / 1.15 V**, **48 kHz**, **two sources / 64 taps**.
Core0 captures audio/controls; core1 renders 64-frame DSP blocks and services
bounded USB work. Added transport latency is 128 frames, about **2.67 ms**. Block
deadline is **1333.3 µs**; 1200 µs is an early warning. A missed output block can
produce 64 frames of silence; late audio is discarded. Flash saves intentionally
fade, stop ADC/DMA, lock out for verified flash access, then restart sampling in
a known state before acknowledging. This fixes the tested repeated-save failure.
Unchanged saves skip mute/erase; failures return an error and latch the general
faultLED once callbacks run. A stopped core cannot guarantee LED feedback. Save
pauses are excluded from timing peaks.

**Beta2 ships the exact user-tested alpha3 UF2 bytes; the promoted root rebuild
also matches byte-for-byte.** The user reports all alpha3 tests pass, including
8mu pickup/page control and effective indicator. Randomisation listening confirms
elevation changes are effective. The alpha2 save fix separately passed.

Historical timing context from earlier builds (not new beta2 measurements):

| Build / mode | Duration | Callback / block peak |
|---|---|---|
| Alpha26 Pendulum |25min|14 /1175µs; clean, no queue warnings|
| Alpha26 Fig8 |20min|14 /1176µs; no separate listening/queue report|
| Alpha26 Wander |30min|14 /1193µs; clean, no queue warnings|
|0.3alpha1 opposing Orbits|22min|12 /1188µs; clean, no queue warnings|

No new alpha3 peak values or run duration were supplied with its pass. Headroom
remains narrow; historical Wander was only7µs below the block warning. Additional
testers should log their own board/headphones, controls/CV, duration and USBrole,
covering editor and separate8mu boots. [Protocol/feedback](docs/TEST_PROTOCOL.md).

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
historical, not proof of the beta's sound. [Archive](../README.md) holds earlier
versions; current `uf2/` contains only beta2 and its notices/checksum.


## Pitch/roll positioning experiment

[0.4.0-alpha1 pitch/roll](../../experimental/pitch-roll-position-64/README.md) replaces
8mu yaw integration with direct pitch front/back and roll left/right positioning.
Includes a separate UF2/editor and hardware test protocol; beta2 above is preserved.
Host/build checks pass; physical testing of this mapping remains pending.


[Alpha2 roll polarity correction](../../experimental/pitch-roll-polarity-64/README.md)
reverses roll in both motion mappings: lowering the right edge moves right,
lowering the left edge moves left. Pitch remains unchanged. Alpha1 otherwise
passed user testing, but its physical roll convention was reversed; both prior
alpha and beta2 remain available. Hardware confirmation of alpha2 is pending.
