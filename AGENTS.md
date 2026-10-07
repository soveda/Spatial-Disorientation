# Development requirements

Read docs/WORKSHOP_COMPUTER_AI_DIRECTIVE.md and docs/DESIGN.md before substantive changes.
The user has approved the initial design. Confirm material changes in scope.
User requirements override older guidance in the copied directive:
- Use the vendored ComputerCard 0.4.0 API and its actual implementation as authoritative.
- The user explicitly does not require AI-assistance disclosures; do not add them.
- Use MIT for original code and documentation. Credit sources and preserve third-party notices.
- Work in this independent repository; do not change Workshop_Computer during development.

Search Workshop_Computer examples before introducing new hardware/build/DSP patterns.
Use 48 kHz audio callbacks at an initial 144 MHz CPU clock. The ADC's 96 kHz
sampling does not change the 48 kHz ProcessSample callback rate.
Keep ProcessSample under about 20 microseconds; use fixed-point DSP, bounded work,
and smooth controls. Read hardware inputs only inside ProcessSample.
Use RAM execution when memory permits, oscillator startup multiplier 64,
and disabled USB/UART stdio. USB MIDI/editor processing belongs on core 1;
keep audio DSP on core 0 and use safe bounded communication between cores.
Verify APIs against vendor/ComputerCard/ComputerCard.h. Explain borrowed algorithms
and their musical behaviour. Keep docs, metadata and actual controls consistent.
Commit meaningful checkpoints, including successful builds. Do not claim hardware
validation from a compilation or publish placeholder firmware as a finished effect.
Before release, validate metadata against Workshop_Computer's canonical schema and
include tested UF2 firmware. See THIRD_PARTY_NOTICES.md for source attribution.
