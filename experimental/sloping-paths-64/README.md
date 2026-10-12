# Alpha26: sloping-path timing and listening experiment

**0.1.0-alpha26-sloping-paths-64-DUAL · experimental · hardware validation pending**

Original code, mathematical lookup, tests and documentation © 2026 Adrian Vos
(soveda), [MIT](../../LICENSE). Preserve [third-party attribution](../../THIRD_PARTY_NOTICES.md)
and [full firmware notices](uf2/NOTICE.txt). Ear filters derive from Bill Gardner
and Keith Martin, MIT Media Laboratory (1994), *HRTF Measurements of a KEMAR
Dummy-Head Microphone*, Technical Report #280, under
[separate data terms](../../vendor/KEMAR/SOURCE_TERMS.md). Existing ComputerCard,
Workshop_BlockAudioCard, WaveSeq, EightMU/rppicomidi and Pico SDK credits remain.
Slope rotation and the mathematical asin table are original; no external code or
new HRTF measurements are introduced. Musical inspiration remains Neuzeit Quasar.

## Scope and fallback

User authorizes a separate sloping-path experiment despite limited measured
headroom. [Alpha25](../fir-unroll16-64/README.md) source/editor/UF2 remain intact:
20-minute Fig8 peak **13 µs callback / 1180 µs block**, Wander **14 / 1197 µs**.
No new listening or queue-warning result was supplied with those timing reports.
These are hardware reference readings, not guarantees for alpha26.

[Alpha26 UF2](uf2/Spatial_Disorientation_0.1.0-alpha26-sloping-paths-64-DUAL.uf2)
retains **192 MHz / 1.15 V**, **ComputerCard 0.4.0**, **48 kHz**, both sources,
all **64 HRTF taps**, alpha25's 16-tap FIR unroll/address cache/smoothing, room
model and block scheduling. DSP remains core 1; sample/control capture core 0.
Geometry stays staggered every 32 samples per source; no additional audio-rate
trigonometry or FIR work. The extra geometry and captured settings can still
increase measured block/callback time. Root firmware/editor and Workshop release
are unchanged. This is a test, not a release replacement.

## Sloping controls

Choose Disorientation at startup (hold Down, Main upper third, release). The
separate [alpha26 editor](web/index.html) adds two Disorientation-only settings:

| Setting | Range / effect |
|---|---|
| Uphill direction, ID 18 | Continuous 0–360°: front 0°, right 90°, back 180°, left 270°; diagonals between |
| Path tilt, ID 19 | 0–40°. Zero bypasses the slope and retains alpha25's audio exactly |
| Existing height centre / vertical excursion, IDs 16/17 | Retained; added to slope height, then clipped to ±40° |

A fixed-point unit direction rotates out of the horizontal plane toward the
chosen uphill bearing. The opposite direction goes down; directions 90° away
stay level. Rotation also changes the projected azimuth, with horizontal path
radius/distance kept at its previous value. The same rule applies to Fig8,
Pendulum and Wander using each source's independently resolved direction.
This is an inclination around the listener, not rotation of the whole translated
Cartesian trajectory about its own centre. Existing vertical bob is added after
rotation, so bob+tilt is an artistic combined path rather than one rigid plane.
Clipping can flatten its peaks; set vertical excursion **0°** and centre **0°**
first to hear slope alone. Height remains limited by the ±40° measured profiles;
this does not introduce new elevation data or guarantee personal localization.

Tilt follows position, not a new oscillator: freeze holds the slope, reversal
retraces it and reset returns to the current path's reset position. A reset is
not necessarily level; for example Wander resets in front, which is uphill when
bearing is front. Changing movement does not reset the phase. Orbits/Mixer and
all existing 8mu mappings are unchanged. 8mu D still selects movement, not slope;
configure slope in the separate editor boot, Save, then boot with 8mu.

## Editor / persistence compatibility

**Use the alpha26 editor with this firmware.** SysEx **v6**, JSON presets **v6**,
flash **v7**. Old editors reject this protocol instead of misinterpreting settings.
The experimental editor accepts v1–v5 presets, preserving their existing values
and defaulting tilt/direction to zero; firmware migrates flash v1–v6 the same way.
Apply changes the running settings; changes are not sent automatically. Reset to
init applies tilt zero. Save to card persists slope plus all existing mode banks.
Export includes slope; Import stages it until Apply. New IDs are Disorientation-only.

**Before experimental Save, export your alpha25 presets for every mode you want
to preserve.** Apply-only testing leaves the old flash intact and permits direct
alpha25 fallback. After saving v7, alpha25 cannot read it and starts defaults;
restore by importing your old exports into the alpha25 editor and saving each
mode again. Alpha25 also cannot import JSON v6 files. Original source/UF2 fallback
files are preserved; saving experimental settings changes the physical card's
flash, not those files.

## Hardware comparison protocol

1. Keep the alpha25 audio inputs, rapid/audio-rate CV signal/depth, speed, room,
   source levels and other settings for a comparable workload. Export old presets.
   Flash alpha26, select Disorientation, connect its editor and Read.
2. Set Path tilt **0°** and repeat each movement with the same existing vertical
   settings used in alpha25. Reboot between timed runs to clear latched peaks.
   Record callback/block peaks; this measures any build/layout/capture overhead
   even when slope calculations are bypassed.
3. For listening, use slow X, centre **0°**, vertical excursion **0°**, both sources
   and tilt **40°**. Apply front, right and diagonal (45°) uphill directions.
   Check each movement: the selected side should rise and its opposite fall;
   positions perpendicular to uphill should be level. Watch rendered heights
   only to confirm settings, then listen without looking. Tilt zero should
   return to the horizontal path. Check freeze/release, reverse and reset.
4. For maximum workload, repeat the same rapid-CV stress setup with tilt **40°**
   and moving intermediate heights. Test all three movements, rebooting between
   tests. Then enable the previous vertical excursion as well and repeat. Choose
   the heaviest case for **20 minutes**, keeping audio/USB/CV conditions matched.
   Report movement, tilt/direction, bob/centre, duration, callback/block peaks,
   LED 5 and any slow-DSP/audio-queue warning or audible change.
5. Warning targets remain callback **<18 µs**, block **<1200 µs**; block audio
   deadline is **1333.3 µs**. Crossing 1200 flags limited headroom, not necessarily
   an audio dropout. Queue faults/clicks/gaps mean the experiment failed under
   that workload. Record the result and return to alpha25 if necessary.
6. Briefly check Orbits and Mixer; slope settings must not alter them. Verify
   Apply/Read, slope Export/Import, and optionally Save/reset/Read after backing
   up old presets. For separate 8mu testing, Save chosen slope first; then boot
   with 8mu, check normal faders/buttons, frozen heights with C, D movement cycle
   and held-D timing diagnostics. There is no new live slope fader mapping.

Compare alpha26 tilt-zero and tilt-enabled runs; compare alpha25 too. A peak is
latched, so successive settings in the same boot cannot prove equal timing.

## Build and host verification

Pico SDK **2.3.0**, Arm GNU Toolchain **15.2.Rel1**. Build succeeds with flash
**141504 B**, RAM **182252 B**, scratch **2048 B each**; +1788 flash/+2840 main
RAM relative to alpha25. See [SHA256SUMS](uf2/SHA256SUMS.txt). Hardware cost remains
unmeasured; host runtime is not an RP2040 timing prediction.

ASan/UBSan checks pass: independent floating-point rotation comparison (height
error <0.16°, azimuth <0.23°), uphill/opposite/side axes, zero bypass, combined
height bounds, frozen output, **180000** new sample-aligned sloping frames through
all movements with rapidly changing settings, slope wire/flash roundtrip, v6
migration and malformed rejection. Existing vertical/continuous Mixer/settings
checks pass. **756000 scene/transport frames exactly match alpha25 at tilt zero**;
**1024000 direct HRTF frames** match the unchanged renderer. Actual editor lifecycle,
v6 slope Apply/Read/Save/Export/Import, legacy defaulting, telemetry, independent
mode banks and malformed atomic rejection pass with simulated MIDI.

From repository root:

```sh
cmake -S experimental/sloping-paths-64 -B /tmp/spatial-sloping-paths-64 -DPICO_SDK_PATH=/path/to/pico-sdk -DSPATIAL_ELEVATION_SOLO=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/spatial-sloping-paths-64 -j2
clang++ -std=c++17 -O2 -Wall -Wextra -fsanitize=address,undefined -DSPATIAL_HRTF_TAPS=64 -Iexperimental/sloping-paths-64/src experimental/sloping-paths-64/tests/slope_test.cpp -o /tmp/spatial-slope-test
/tmp/spatial-slope-test
python3 -m http.server 8803 --bind 127.0.0.1 --directory experimental/sloping-paths-64/web
```

Visit `http://127.0.0.1:8803` in Chrome/Edge. Run `node tests/editor_test.cjs` from
this experiment directory. Generator `tools/generate_slope_table.py` reproduces
the original mathematical table. No hosted editor publication is implied.
