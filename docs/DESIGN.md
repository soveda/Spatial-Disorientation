# Spatial Disorientation: approved initial design

Two-source binaural spatializer inspired by Neuzeit Instruments Quasar for Music Thing Modular Workshop Computer.
Approved in conversation on 2026-10-07. This records the approved target; implementation status is noted below.

## Performance controls

| Control or jack | Planned behaviour |
|---|---|
| Main | Position around the head |
| X | Orbit speed, with a stationary region |
| Y | Distance |
| Z up / middle / momentary down | Linked movement / opposite orbits / phase reset |
| CV 1 / CV 2 | Position / distance modulation |
| Pulse 1 / Pulse 2 | Orbit clock / phase reset |
| Audio 1 / Audio 2 in | Independent mono sources |
| Audio 1 / Audio 2 out | Binaural left / right |

CV and pulse outputs are reserved; zero/low until useful behaviour is agreed.
LED animation, ranges, CV scaling, and trigger-versus-gate details are now defined
for the initial prototype in README.md. Use soft takeover if editor
settings and physical controls can otherwise cause jumps.

## DSP direction

Render two independently placed mono sources into binaural stereo. Start with
interaural timing, head-shadow filtering, distance attenuation and early room
reflections. Move positions smoothly and support linked and opposite orbits.
Evaluate front/back and elevation perception with listening tests rather than
claiming a full HRTF model. Headphones are the primary monitoring target.

A WebMIDI/SysEx editor is part of the intended instrument: source separation,
elevation, room amount, modulation and presets. It must follow upstream editor
requirements: stable versioned parameter IDs, validated ranges, Read/Apply/Save
separation, safe flash writes, reconnect handling and documented cable/browser
requirements. Any external HRTF dataset must have compatible terms and full credit.
Long delays and the full Quasar feature set are outside the initial scope.

## Development checkpoints

1. Independent repository, attributed dependencies, buildable hardware scaffold.
2. Core effect DSP and deterministic host-side checks for meaningful invariants.
3. Performance controls, modulation and feedback, with matching operator docs.
4. Editor and persistence where specified; on-device timing and listening tests.
5. Release metadata, versioned UF2 and eventual Workshop_Computer contribution.

Do not describe the effect as working until DSP is implemented and tested.

## Confirmed platform and configuration decisions

The user selected 192 MHz as the initial CPU target based on stable experience
with other cards. Use 1.15 V per Chris Johnson's ComputerCard NOTES, set before
raising the clock. Audio remains 48 kHz with ComputerCard 0.4.0. Performance
measurements for this card must still cover its actual workload.

The user supports web configuration as complexity grows. Plan for Twin Orbits,
Spatial Mixer and Disorientation as alternative functions on one card, with
settings saved through the web UI and a startup function selector. Only the
selected function runs. The saved startup default and exact selection gesture
remain to be defined. Twin Orbits and its initial editor are implemented; the other functions remain planned.

## Initial Twin Orbits implementation (0.1.0-alpha1)

The controls above are implemented with ranges, CV scaling, LED roles and edge
semantics documented in README.md. Source levels, separation, room, strength and
clock division are editor settings; knobs have separate roles, so no takeover
is necessary. Elevation and other functions remain planned. Hardware/listening
validation remains pending; see TEST_PROTOCOL.md. Original implementation and
this addendum: Adrian Vos (soveda), 2026, MIT; upstream sources in
../THIRD_PARTY_NOTICES.md.

## Future live-control option

The user requested future 8mu integration, including gyroscopic control of sound.
See OPTIONS.md for candidate mappings and implementation questions. This does not
change the current Twin Orbits alpha1 firmware or its hardware test scope.

## Alpha2 DSP pass (2026-10-08)

Stronger rear high-frequency shadowing and a rear-weighted three-sample
feed-forward spectral cue supplement the existing interaural timing/level model.
No control or settings-schema change. The filter is an original analytical
approximation by Adrian Vos (soveda), MIT, not a measured HRTF. Tests verify
low-frequency retention, high-frequency rear attenuation and strength-zero bypass;
headphone perception and hardware timing/stability remain to be retested.
See IMPLEMENTATION_PLAN.md for the remaining staged work.
