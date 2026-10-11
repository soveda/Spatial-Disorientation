# Compiled FIR inspection — alpha19

Inspection date: 2026-10-11. Original analysis/documentation © 2026 Adrian Vos
(soveda), under the [MIT license](../../../LICENSE). The inspected implementation
and firmware retain [third-party attribution](../../../THIRD_PARTY_NOTICES.md)
and [firmware notices](../uf2/NOTICE.txt). KEMAR measurements are by Bill Gardner
and Keith Martin, MIT Media Laboratory (1994); their
[measurement terms](../../../vendor/KEMAR/SOURCE_TERMS.md) remain separate.

## Findings

The inner FIR loop has **no stack accesses, register spills, stores, function
calls, halfword loads or sign-extension instructions**. It processes eight taps
per iteration, using word-sized working history and coefficients. Each input
sample is loaded once and reused for both ears. The three word loads per tap
(input, left coefficient, right coefficient) already match the straightforward
scalar paired-ear formulation; further reductions would require another layout
or algorithm, rather than removing redundant loads from this loop.

Alpha18 and alpha19 have identical inner-loop disassembly, at addresses
`0x20002d32` through `0x20002daa`. Alpha19's scene-reference improvement is outside
this loop. FIR processing is inlined into `spatial::Source::Process(long)` at
`0x20002a34`, rather than emitted as a standalone kernel.

| Operation | Instructions per eight taps |
|---|---:|
| Word loads | 24 |
| Multiplications | 16 |
| Accumulator additions | 16 |
| Pointer advances | 3 |
| Compare | 1 |
| Conditional branch | 1 |
| Total | 61 |

There are eight iterations per 64-tap source frame: 488 loop instructions,
including 192 word loads and 128 multiplications. Two sources over a 64-frame
block execute 62,464 instructions in these FIR loops. This is a static instruction
count, **not a cycle count or measured share of block time**.

The three pointer advances, comparison and branch account for five of the 61
instructions (about 8.2%). Unrolling to 16 or 32 taps could reduce their frequency.
It cannot remove the convolution's loads, multiplies and accumulator additions;
any timing gain must be measured on the actual card. Larger generated code can
also affect instruction fetching and register allocation.

The wider Source::Process function reserves 52 bytes of local stack and saves
36 bytes of registers. It contains 57 explicit stack memory instructions across
all generated paths, outside the inner FIR loop, and no function calls. These
accesses include compiler temporaries and ordinary local values; this inspection
does not establish how many execute per frame or which dominate elapsed time.
It would be misleading to call all of them avoidable FIR spills.

## Practical next step

A small, isolated 16/32-tap unroll comparison is a reasonable next experiment,
with frame-exact comparison against alpha19 and hardware measurements across all
three modes. Expect a limited opportunity from loop overhead, rather than a large
assembly rewrite saving. Keep the alpha19 binary/source as fallback.

If that produces little improvement, inspect and benchmark the surrounding
per-frame work (coefficient smoothing, fractional delay reads and room processing)
and geometry updates. This disassembly alone cannot rank their actual cost.
Maintain two 64-tap sources, current smoothing and sound, 192 MHz operation, and
the existing warning thresholds. No firmware, editor or processing change was made
for this inspection.

## Reproducibility

Inspected existing release-build ELFs:

- Alpha18: `/tmp/spatial-elevation64-dual/spatial_disorientation.elf`
- Alpha19: `/tmp/spatial-scene-reference64/spatial_disorientation.elf`

Their generated UF2 hashes match the preserved packaged firmware:

- Alpha18: `2708df8e4b2017c99325093b9ac10dbc8b7ef101a1887cc0dce73d97afad8368`
- Alpha19: `8acd0ace32cccb56c634053faf8851add3ff61b52be3a5d5a337f33e39fccd3a`

Compiler: Arm GNU Toolchain 15.2.Rel1, GCC 15.2.1. Relevant recorded flags:
`-mcpu=cortex-m0plus -mthumb -mfloat-abi=soft -O3 -DNDEBUG -std=gnu++17`,
with exceptions, RTTI and unwind tables disabled. Both builds use two sources
and 64 taps. The inspected loop resides at a RAM execution address.

Disassembly was obtained with the toolchain's `arm-none-eabi-objdump -d -C`.
The complete alpha19 loop follows; alpha18 is identical:

```asm
20002d32:	6833      	ldr	r3, [r6, #0]
20002d34:	6801      	ldr	r1, [r0, #0]
20002d36:	4359      	muls	r1, r3
20002d38:	1949      	adds	r1, r1, r5
20002d3a:	683d      	ldr	r5, [r7, #0]
20002d3c:	435d      	muls	r5, r3
20002d3e:	6873      	ldr	r3, [r6, #4]
20002d40:	192d      	adds	r5, r5, r4
20002d42:	6844      	ldr	r4, [r0, #4]
20002d44:	435c      	muls	r4, r3
20002d46:	1864      	adds	r4, r4, r1
20002d48:	6879      	ldr	r1, [r7, #4]
20002d4a:	434b      	muls	r3, r1
20002d4c:	6881      	ldr	r1, [r0, #8]
20002d4e:	195d      	adds	r5, r3, r5
20002d50:	68b3      	ldr	r3, [r6, #8]
20002d52:	4359      	muls	r1, r3
20002d54:	190c      	adds	r4, r1, r4
20002d56:	68b9      	ldr	r1, [r7, #8]
20002d58:	434b      	muls	r3, r1
20002d5a:	68c1      	ldr	r1, [r0, #12]
20002d5c:	195d      	adds	r5, r3, r5
20002d5e:	68f3      	ldr	r3, [r6, #12]
20002d60:	4359      	muls	r1, r3
20002d62:	190c      	adds	r4, r1, r4
20002d64:	68f9      	ldr	r1, [r7, #12]
20002d66:	434b      	muls	r3, r1
20002d68:	6901      	ldr	r1, [r0, #16]
20002d6a:	195d      	adds	r5, r3, r5
20002d6c:	6933      	ldr	r3, [r6, #16]
20002d6e:	4359      	muls	r1, r3
20002d70:	190c      	adds	r4, r1, r4
20002d72:	6939      	ldr	r1, [r7, #16]
20002d74:	434b      	muls	r3, r1
20002d76:	6941      	ldr	r1, [r0, #20]
20002d78:	195d      	adds	r5, r3, r5
20002d7a:	6973      	ldr	r3, [r6, #20]
20002d7c:	4359      	muls	r1, r3
20002d7e:	190c      	adds	r4, r1, r4
20002d80:	6979      	ldr	r1, [r7, #20]
20002d82:	434b      	muls	r3, r1
20002d84:	6981      	ldr	r1, [r0, #24]
20002d86:	195d      	adds	r5, r3, r5
20002d88:	69b3      	ldr	r3, [r6, #24]
20002d8a:	4359      	muls	r1, r3
20002d8c:	190c      	adds	r4, r1, r4
20002d8e:	69b9      	ldr	r1, [r7, #24]
20002d90:	434b      	muls	r3, r1
20002d92:	69f1      	ldr	r1, [r6, #28]
20002d94:	195b      	adds	r3, r3, r5
20002d96:	69c5      	ldr	r5, [r0, #28]
20002d98:	3620      	adds	r6, #32
20002d9a:	434d      	muls	r5, r1
20002d9c:	192d      	adds	r5, r5, r4
20002d9e:	69fc      	ldr	r4, [r7, #28]
20002da0:	3020      	adds	r0, #32
20002da2:	434c      	muls	r4, r1
20002da4:	3720      	adds	r7, #32
20002da6:	18e4      	adds	r4, r4, r3
20002da8:	4296      	cmp	r6, r2
20002daa:	d1c2      	bne.n	20002d32 <spatial::Source::Process(long)+0x2fe>
```
