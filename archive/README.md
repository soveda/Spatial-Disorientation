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


Archived2026-10-15: `beta1/` preserves the previous root0.2.0-beta1 source, editor,
documentation, notices, UF2 and original tester ZIP (`distribution/`). Its comparison
test paths and external Markdown links are rebased. Beta1 uses protocol6/JSON6/
flash7, while beta2 uses protocol7/JSON7/flash8. Back up beta1 presets before beta2
Save; beta1 cannot read flash8. Separate alpha1/alpha2/alpha3 experiments remain in
`experimental/` at the repository root as historical test/fallback versions.
Original archive record ©2026 Adrian Vos(soveda),MIT; existing measurement and
dependency terms remain. No archived UF2 bytes have changed.


Archived2026-10-15 for0.4.0-beta3: [beta2](beta2/README.md) preserves the full
previous0.3.0-beta2 root source/editor/docs/firmware and original tester ZIP in
its distribution/ folder. Beta2 and beta3 share SysEx7/JSON7/flash8; no migration
needed between them. Beta2 retains the earlier yaw-based8mu motion. Both pitch/roll
experiments remain in root experimental/; alpha2 is promoted, alpha1 has the old
roll direction. No previous firmware bytes are changed.
