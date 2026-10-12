# Alpha24 compiled HRTF address-cache inspection

© 2026 Adrian Vos (soveda), MIT. Existing source/KEMAR attribution and terms:
[third-party notices](../../../THIRD_PARTY_NOTICES.md). Inspection 2026-10-12,
Arm GNU Toolchain 15.2.Rel1, linked Release DUAL/64-tap firmware, Pico SDK 2.3.0.

Only row addressing and the derived intermediate-height flag change. Audio
comparisons cover 1,024,000 direct HRTF and 756,000 complete scene/transport frames
against alpha23 exactly. The cache is core-1-owned and points into immutable
measurement banks. Four pointers add 16 bytes per source on RP2040. No filter
values, update schedule or arithmetic are replaced by cached approximations.

| Linked Source::Process measure | Alpha23 | Alpha24 |
|---|---:|---:|
| Static instructions across all paths | 784 | 770 |
| Explicit stack references across all paths | 74 | 60 |
| FIR instructions per eight taps | 61 | 61 |
| FIR stack accesses | 0 | 0 |
| FIR MOV instructions | 0 | 0 |

Counts exclude literal .word data. Static all-path counts are not executed
instruction counts or cycle timings; they include conditional paths and all
surrounding DSP. They do not quantify a percentage/time saving. Geometry now
prepares four pointers and a flag, adding work there in exchange for less
per-frame addressing. Hardware A/B timing must determine the net effect.

The FIR has 24 loads, 16 multiplications, 16 accumulator additions and five
loop-control instructions per eight taps in both builds. Register names/order
change, but there is no extra move or spill penalty in this repeated kernel.
This avoids the four-MOV regression seen in the earlier delay-address experiment.

Addresses: alpha23 FIR 0x20002e00–0x20002e78; alpha24 FIR
0x20002d64–0x20002ddc. These apply only to the inspected ELF builds.

## FIR excerpts

### alpha23

```asm
20002e00:	6831      	ldr	r1, [r6, #0]
20002e02:	6814      	ldr	r4, [r2, #0]
20002e04:	434c      	muls	r4, r1
20002e06:	1964      	adds	r4, r4, r5
20002e08:	683d      	ldr	r5, [r7, #0]
20002e0a:	434d      	muls	r5, r1
20002e0c:	6871      	ldr	r1, [r6, #4]
20002e0e:	182d      	adds	r5, r5, r0
20002e10:	6850      	ldr	r0, [r2, #4]
20002e12:	4348      	muls	r0, r1
20002e14:	1900      	adds	r0, r0, r4
20002e16:	687c      	ldr	r4, [r7, #4]
20002e18:	4361      	muls	r1, r4
20002e1a:	6894      	ldr	r4, [r2, #8]
20002e1c:	194d      	adds	r5, r1, r5
20002e1e:	68b1      	ldr	r1, [r6, #8]
20002e20:	434c      	muls	r4, r1
20002e22:	1820      	adds	r0, r4, r0
20002e24:	68bc      	ldr	r4, [r7, #8]
20002e26:	4361      	muls	r1, r4
20002e28:	68d4      	ldr	r4, [r2, #12]
20002e2a:	194d      	adds	r5, r1, r5
20002e2c:	68f1      	ldr	r1, [r6, #12]
20002e2e:	434c      	muls	r4, r1
20002e30:	1820      	adds	r0, r4, r0
20002e32:	68fc      	ldr	r4, [r7, #12]
20002e34:	4361      	muls	r1, r4
20002e36:	6914      	ldr	r4, [r2, #16]
20002e38:	194d      	adds	r5, r1, r5
20002e3a:	6931      	ldr	r1, [r6, #16]
20002e3c:	434c      	muls	r4, r1
20002e3e:	1820      	adds	r0, r4, r0
20002e40:	693c      	ldr	r4, [r7, #16]
20002e42:	4361      	muls	r1, r4
20002e44:	6954      	ldr	r4, [r2, #20]
20002e46:	194d      	adds	r5, r1, r5
20002e48:	6971      	ldr	r1, [r6, #20]
20002e4a:	434c      	muls	r4, r1
20002e4c:	1820      	adds	r0, r4, r0
20002e4e:	697c      	ldr	r4, [r7, #20]
20002e50:	4361      	muls	r1, r4
20002e52:	6994      	ldr	r4, [r2, #24]
20002e54:	194d      	adds	r5, r1, r5
20002e56:	69b1      	ldr	r1, [r6, #24]
20002e58:	434c      	muls	r4, r1
20002e5a:	1820      	adds	r0, r4, r0
20002e5c:	69bc      	ldr	r4, [r7, #24]
20002e5e:	4361      	muls	r1, r4
20002e60:	69f4      	ldr	r4, [r6, #28]
20002e62:	1949      	adds	r1, r1, r5
20002e64:	69d5      	ldr	r5, [r2, #28]
20002e66:	3620      	adds	r6, #32
20002e68:	4365      	muls	r5, r4
20002e6a:	182d      	adds	r5, r5, r0
20002e6c:	69f8      	ldr	r0, [r7, #28]
20002e6e:	3220      	adds	r2, #32
20002e70:	4360      	muls	r0, r4
20002e72:	3720      	adds	r7, #32
20002e74:	1840      	adds	r0, r0, r1
20002e76:	429e      	cmp	r6, r3
20002e78:	d1c2      	bne.n	20002e00 <spatial::Source::Process(long)+0x3cc>
```

### alpha24

```asm
20002d64:	6832      	ldr	r2, [r6, #0]
20002d66:	6801      	ldr	r1, [r0, #0]
20002d68:	4351      	muls	r1, r2
20002d6a:	1949      	adds	r1, r1, r5
20002d6c:	683d      	ldr	r5, [r7, #0]
20002d6e:	4355      	muls	r5, r2
20002d70:	6872      	ldr	r2, [r6, #4]
20002d72:	192d      	adds	r5, r5, r4
20002d74:	6844      	ldr	r4, [r0, #4]
20002d76:	4354      	muls	r4, r2
20002d78:	1864      	adds	r4, r4, r1
20002d7a:	6879      	ldr	r1, [r7, #4]
20002d7c:	434a      	muls	r2, r1
20002d7e:	6881      	ldr	r1, [r0, #8]
20002d80:	1955      	adds	r5, r2, r5
20002d82:	68b2      	ldr	r2, [r6, #8]
20002d84:	4351      	muls	r1, r2
20002d86:	190c      	adds	r4, r1, r4
20002d88:	68b9      	ldr	r1, [r7, #8]
20002d8a:	434a      	muls	r2, r1
20002d8c:	68c1      	ldr	r1, [r0, #12]
20002d8e:	1955      	adds	r5, r2, r5
20002d90:	68f2      	ldr	r2, [r6, #12]
20002d92:	4351      	muls	r1, r2
20002d94:	190c      	adds	r4, r1, r4
20002d96:	68f9      	ldr	r1, [r7, #12]
20002d98:	434a      	muls	r2, r1
20002d9a:	6901      	ldr	r1, [r0, #16]
20002d9c:	1955      	adds	r5, r2, r5
20002d9e:	6932      	ldr	r2, [r6, #16]
20002da0:	4351      	muls	r1, r2
20002da2:	190c      	adds	r4, r1, r4
20002da4:	6939      	ldr	r1, [r7, #16]
20002da6:	434a      	muls	r2, r1
20002da8:	6941      	ldr	r1, [r0, #20]
20002daa:	1955      	adds	r5, r2, r5
20002dac:	6972      	ldr	r2, [r6, #20]
20002dae:	4351      	muls	r1, r2
20002db0:	190c      	adds	r4, r1, r4
20002db2:	6979      	ldr	r1, [r7, #20]
20002db4:	434a      	muls	r2, r1
20002db6:	6981      	ldr	r1, [r0, #24]
20002db8:	1955      	adds	r5, r2, r5
20002dba:	69b2      	ldr	r2, [r6, #24]
20002dbc:	4351      	muls	r1, r2
20002dbe:	190c      	adds	r4, r1, r4
20002dc0:	69b9      	ldr	r1, [r7, #24]
20002dc2:	434a      	muls	r2, r1
20002dc4:	69f1      	ldr	r1, [r6, #28]
20002dc6:	1952      	adds	r2, r2, r5
20002dc8:	69c5      	ldr	r5, [r0, #28]
20002dca:	3620      	adds	r6, #32
20002dcc:	434d      	muls	r5, r1
20002dce:	192d      	adds	r5, r5, r4
20002dd0:	69fc      	ldr	r4, [r7, #28]
20002dd2:	3020      	adds	r0, #32
20002dd4:	434c      	muls	r4, r1
20002dd6:	3720      	adds	r7, #32
20002dd8:	18a4      	adds	r4, r4, r2
20002dda:	42b3      	cmp	r3, r6
20002ddc:	d1c2      	bne.n	20002d64 <spatial::Source::Process(long)+0x330>
```
