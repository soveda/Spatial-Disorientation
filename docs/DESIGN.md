# Spatial Disorientation: approved initial design

Two-source binaural spatializer inspired by Neuzeit Instruments Quasar for Music Thing Modular Workshop Computer.
Approved in conversation on 2026-10-07. This describes the target, not the scaffold.

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
LED animation, ranges, CV scaling, and trigger-versus-gate details need definition
before the corresponding features are implemented. Use soft takeover if editor
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
