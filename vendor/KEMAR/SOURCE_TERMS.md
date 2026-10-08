# MIT KEMAR measurement source and usage terms

Measured data copyright 1994 MIT Media Laboratory. Authors: **Bill Gardner and
Keith Martin**, MIT Media Lab. Cite: Gardner, W. G., and Martin, K. D., *HRTF
Measurements of a KEMAR Dummy-Head Microphone*, MIT Media Lab Perceptual Computing
Technical Report #280, May 1994.

Official source and usage statement:
https://sound.media.mit.edu/resources/KEMAR.html
Technical documentation:
https://sound.media.mit.edu/resources/KEMAR/hrtfdoc.txt
Unmodified archive downloaded 2026-10-08:
https://sound.media.mit.edu/resources/KEMAR/diffuse.zip
SHA-256: a48e73137390626ef1e45d1de6a4d98530c6aa570e71b40f7c4442241ed5fa22

The official source makes the data freely available without restrictions on use,
conditional on crediting the authors in research or commercial applications.
These are the dataset's own attribution terms, not an MIT software-license grant.
This project's MIT license covers original code and documentation, not ownership
or relicensing of those measurements. Carry this attribution with derived tables,
firmware and rendered demonstrations. Robert Cain supplied the Windows zip files.

## Derived bank

`tools/generate_hrtf.py` reads only elevation zero, mirrors the hemisphere and
swaps ears following the original compact dataset convention (0 front, 90 right).
It resamples 44.1 to 48 kHz with a 160/147 polyphase filter, derives minimum-phase
responses via the real cepstrum, retains 32 taps with an eight-tap tail taper,
then applies one shared gain scale and Q14 quantization. Direction/ear loudness
relationships are preserved; no separate per-direction normalization is used.
Minimum-phase reconstruction removes measured transport phase/delay; a separate
analytic interaural delay in the renderer supplies timing cues. This is a hybrid
short-filter approximation of the measured dataset, not full measured HRIR playback.

The generated bank `src/dsp/hrtf_table.h` has 72 five-degree horizontal directions,
two ears, 32 taps each (9,216 bytes). Nearby directions are linearly interpolated.
One coefficient per ear/sample is updated and slewed to spread interpolation work.
`generation.json` records generator metrics and the input archive hash. Those
metrics describe numerical approximation, not localization performance or CPU time.

Rebuild: `python3 tools/generate_hrtf.py` (NumPy/SciPy required; generated with
NumPy 2.5.2 and SciPy 1.18.0). No network is needed after obtaining the archive.
Generator, renderer integration and this explanatory note: © 2026 Adrian Vos
(soveda), MIT. The original measurements retain the attribution terms above.
