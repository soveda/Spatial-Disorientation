# Alpha23 hardware test protocol

© 2026 Adrian Vos (soveda), MIT. Source/measurement attribution in
[the experiment README](../README.md) and [third-party notices](../../../THIRD_PARTY_NOTICES.md).

1. Flash alpha23 DUAL. For editor testing, boot with the computer connected.
   Hold Down during startup, choose Main's upper third (right LEDs), release.
   Open the matching alpha23 editor, enable MIDI/SysEx and select Spatial
   Disorientation for both ports. Read: old settings should retain movement/
   mode values and show centre 0° / excursion zero. Confirm default horizontal
   paths still sound like alpha22 before enabling elevation.
2. Use headphones, broadband A, near placement, modest room, slow X speed.
   Set Fig8, centre 0°, excursion 20° (raw 1024); Apply. Listen for rise/fall
   alongside the lobes. Try 40° excursion (2048). Height telemetry should move
   above/below 0°. Repeat without looking at the display: note whether the height
   is audible as placement, spectral change, or neither. Reconnect/read must
   retain applied settings without silently resetting excursion.
3. Set Pendulum. Height should be low near the angular centre and high at either
   end of the swing, retracing when it reverses. Test X both directions. Set
   excursion zero: vertical movement stops, while horizontal movement continues.
4. Set Wander. Listen for smooth irregular height changes; no discontinuous
   jumps at nodes or wraps. Reverse X and then reverse again: it should retrace.
   Reset phase: vertical/horizontal path returns to its repeatable seeded start.
5. Add B with source separation. Both sources should move in height with their
   own phases; no A-only rendering. Try linked/opposing, Main/Y and CV changes.
   Set centre +20° with excursion 20°: range approximately 0°..+40°. Try −20°.
   Centre +40° with excursion 40° should clamp at +40°, never wrap below.
6. Hold panel Down: height and phase hold. Release: both resume smoothly. Test
   clock/reset pulses and movement changes. No clicks/gaps or stuck height. Height
   telemetry may take a block to settle; filter smoothing continues after freeze.
7. Set distinct vertical centre/excursion, Save, reset into Disorientation and
   Read: values restore. Export/import a nonzero-excursion preset, Apply/Read:
   values restore exactly. Import an older Disorientation preset: centre 0°,
   excursion zero, previous movement/settings retained. Reset to init clears
   vertical settings in this mode only; reboot without Save restores last saved.
8. Boot Mixer: saved A/B heights and fader-8 pickup remain intact. Change/save
   Mixer heights, return to Disorientation: vertical centre/excursion still
   present. Orbits remains horizontal with its prior controls/settings.
9. Separately connect 8mu, reset into Disorientation. Saved vertical settings
   load without the editor. A reset, B motion toggle, C hold freeze, D tap cycles
   all three movements and D hold displays timing. Fader 8 still controls motion
   amount, not height; motion/CV changes must not overwrite saved vertical settings.
   Mixer fader 8 remains height. No simultaneous editor/8mu connection is needed.
10. Stress both sources, centre 0°, excursion 40°, high room, rapid/audio-rate
    CV, fast movement and 8mu activity for at least 20 minutes across Fig8,
    Pendulum and Wander. In an editor session, record callback and block peaks
    separately for each path; in an 8mu session use D timing LEDs and Computer
    LED 5. Target <18 µs callback, <1,200 µs block; no queue faults, locked controls
    or audio abnormalities. Reset clears latched peaks/warnings between runs.

Report path-specific peaks, duration, settings and blind listening impressions,
plus Save/reset and mode-isolation results. If overloaded, return to alpha22;
retain/export older presets before writing flash v6 for fallback restoration.
