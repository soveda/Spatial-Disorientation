# Previous firmware and experiments

© 2026 Adrian Vos (soveda), MIT. Existing dependency and Gardner/Martin KEMAR
attribution/terms remain in each snapshot and the repository notices.

Archived 2026-10-13 when 0.2.0-beta1 became the current tester build.
`alpha14/` is the complete former root version, with its original documentation,
source, editor and UF2 history. `experimental/` preserves alpha15–alpha26
experiments, including alpha25 and the tested alpha26 fallback, exactly in DSP
behaviour. Experiment CMake paths and Markdown links are rebased for this archive.
No old firmware is deleted. [SHA256SUMS.txt](SHA256SUMS.txt) records all archived
UF2 hashes. Archives are historical, not current beta instructions.

Alpha25 flash v6/JSON5 and alpha14 flash v4/JSON3 do not accept beta flash v7/JSON6.
Back up old presets before beta Save; use old exports and the matching archived
editor to restore saved settings after downgrading. Apply-only testing preserves
old flash. Build archived experiments with their CMake file and the shared root
vendor folders. `alpha14/` also contains its original dependency snapshot.
