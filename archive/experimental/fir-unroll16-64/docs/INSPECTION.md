# Alpha25 compiled FIR unroll inspection

© 2026 Adrian Vos (soveda), MIT. Existing measurement/code attribution:
[third-party notices](../../../alpha14/THIRD_PARTY_NOTICES.md). Arm GNU Toolchain
15.2.Rel1 / Pico SDK 2.3.0 linked Release DUAL firmware, inspected 2026-10-12.

Only FIR grouping changes from eight to 16 taps. Explicit always_inline retains
alpha24's inlining into Source::Process; the first outlined build added a new
per-source-frame call and is excluded. Final candidate exactly matches alpha24
for 1,024,000 direct HRTF and 756,000 transport/scene frames under host sanitizers.

| Linked measure | Alpha24 | Alpha25 final |
|---|---:|---:|
| Taps per FIR loop iteration | 8 | 16 |
| Iterations per 64-tap source frame | 8 | 4 |
| Instructions per iteration | 61 | 119 |
| Instructions across iterations per source frame | 488 | 476 |
| FIR stack accesses | 0 | 0 |
| FIR MOV instructions per iteration | 0 | 2 |
| FIR calls per iteration | 0 | 0 |
| Static Source::Process instructions across all paths | 770 | 830 |
| Static Source::Process stack references across all paths | 60 | 68 |

The final loop has 48 loads, 32 multiplies, 32 accumulator additions, three
pointer advances, comparison, branch and two MOVs. It executes 12 fewer loop
instructions per source frame (1,536 fewer per two-source 64-frame block). Its
larger static instruction count is expected from a larger unrolled body; static
code size is not executed work. Two additional MOVs partly offset the reduction
in loop-control overhead. No FIR spills, stack loads/stores or function calls
were introduced, but the surrounding function has more static stack references.
Not all of those paths execute every frame. No microsecond saving can be inferred
from these counts; hardware A/B timing must determine the net effect.

Addresses apply only to these linked builds: alpha24 0x20002d64–0x20002ddc;
alpha25 0x20002de8–0x20002ed4.

## Final alpha25 FIR excerpt

```asm
20002de8:	6802      	ldr	r2, [r0, #0]
20002dea:	6829      	ldr	r1, [r5, #0]
20002dec:	4351      	muls	r1, r2
20002dee:	468c      	mov	ip, r1
20002df0:	44b4      	add	ip, r6
20002df2:	6826      	ldr	r6, [r4, #0]
20002df4:	6861      	ldr	r1, [r4, #4]
20002df6:	4356      	muls	r6, r2
20002df8:	6842      	ldr	r2, [r0, #4]
20002dfa:	18f6      	adds	r6, r6, r3
20002dfc:	686b      	ldr	r3, [r5, #4]
20002dfe:	4353      	muls	r3, r2
20002e00:	434a      	muls	r2, r1
20002e02:	68a9      	ldr	r1, [r5, #8]
20002e04:	1996      	adds	r6, r2, r6
20002e06:	6882      	ldr	r2, [r0, #8]
20002e08:	4463      	add	r3, ip
20002e0a:	4351      	muls	r1, r2
20002e0c:	185b      	adds	r3, r3, r1
20002e0e:	68a1      	ldr	r1, [r4, #8]
20002e10:	434a      	muls	r2, r1
20002e12:	68e9      	ldr	r1, [r5, #12]
20002e14:	1996      	adds	r6, r2, r6
20002e16:	68c2      	ldr	r2, [r0, #12]
20002e18:	4351      	muls	r1, r2
20002e1a:	185b      	adds	r3, r3, r1
20002e1c:	68e1      	ldr	r1, [r4, #12]
20002e1e:	434a      	muls	r2, r1
20002e20:	6929      	ldr	r1, [r5, #16]
20002e22:	1996      	adds	r6, r2, r6
20002e24:	6902      	ldr	r2, [r0, #16]
20002e26:	4351      	muls	r1, r2
20002e28:	185b      	adds	r3, r3, r1
20002e2a:	6921      	ldr	r1, [r4, #16]
20002e2c:	434a      	muls	r2, r1
20002e2e:	6969      	ldr	r1, [r5, #20]
20002e30:	1996      	adds	r6, r2, r6
20002e32:	6942      	ldr	r2, [r0, #20]
20002e34:	4351      	muls	r1, r2
20002e36:	185b      	adds	r3, r3, r1
20002e38:	6961      	ldr	r1, [r4, #20]
20002e3a:	434a      	muls	r2, r1
20002e3c:	69a9      	ldr	r1, [r5, #24]
20002e3e:	1996      	adds	r6, r2, r6
20002e40:	6982      	ldr	r2, [r0, #24]
20002e42:	4351      	muls	r1, r2
20002e44:	185b      	adds	r3, r3, r1
20002e46:	69a1      	ldr	r1, [r4, #24]
20002e48:	434a      	muls	r2, r1
20002e4a:	69e9      	ldr	r1, [r5, #28]
20002e4c:	1996      	adds	r6, r2, r6
20002e4e:	69c2      	ldr	r2, [r0, #28]
20002e50:	4351      	muls	r1, r2
20002e52:	185b      	adds	r3, r3, r1
20002e54:	69e1      	ldr	r1, [r4, #28]
20002e56:	434a      	muls	r2, r1
20002e58:	6a29      	ldr	r1, [r5, #32]
20002e5a:	1996      	adds	r6, r2, r6
20002e5c:	6a02      	ldr	r2, [r0, #32]
20002e5e:	4351      	muls	r1, r2
20002e60:	185b      	adds	r3, r3, r1
20002e62:	6a21      	ldr	r1, [r4, #32]
20002e64:	434a      	muls	r2, r1
20002e66:	6a69      	ldr	r1, [r5, #36]	@ 0x24
20002e68:	1996      	adds	r6, r2, r6
20002e6a:	6a42      	ldr	r2, [r0, #36]	@ 0x24
20002e6c:	4351      	muls	r1, r2
20002e6e:	185b      	adds	r3, r3, r1
20002e70:	6a61      	ldr	r1, [r4, #36]	@ 0x24
20002e72:	434a      	muls	r2, r1
20002e74:	6aa9      	ldr	r1, [r5, #40]	@ 0x28
20002e76:	1996      	adds	r6, r2, r6
20002e78:	6a82      	ldr	r2, [r0, #40]	@ 0x28
20002e7a:	4351      	muls	r1, r2
20002e7c:	185b      	adds	r3, r3, r1
20002e7e:	6aa1      	ldr	r1, [r4, #40]	@ 0x28
20002e80:	434a      	muls	r2, r1
20002e82:	6ae9      	ldr	r1, [r5, #44]	@ 0x2c
20002e84:	1996      	adds	r6, r2, r6
20002e86:	6ac2      	ldr	r2, [r0, #44]	@ 0x2c
20002e88:	4351      	muls	r1, r2
20002e8a:	185b      	adds	r3, r3, r1
20002e8c:	6ae1      	ldr	r1, [r4, #44]	@ 0x2c
20002e8e:	434a      	muls	r2, r1
20002e90:	6b29      	ldr	r1, [r5, #48]	@ 0x30
20002e92:	1996      	adds	r6, r2, r6
20002e94:	6b02      	ldr	r2, [r0, #48]	@ 0x30
20002e96:	4351      	muls	r1, r2
20002e98:	185b      	adds	r3, r3, r1
20002e9a:	6b21      	ldr	r1, [r4, #48]	@ 0x30
20002e9c:	434a      	muls	r2, r1
20002e9e:	6b69      	ldr	r1, [r5, #52]	@ 0x34
20002ea0:	1996      	adds	r6, r2, r6
20002ea2:	6b42      	ldr	r2, [r0, #52]	@ 0x34
20002ea4:	4351      	muls	r1, r2
20002ea6:	185b      	adds	r3, r3, r1
20002ea8:	6b61      	ldr	r1, [r4, #52]	@ 0x34
20002eaa:	434a      	muls	r2, r1
20002eac:	6ba9      	ldr	r1, [r5, #56]	@ 0x38
20002eae:	1996      	adds	r6, r2, r6
20002eb0:	6b82      	ldr	r2, [r0, #56]	@ 0x38
20002eb2:	4351      	muls	r1, r2
20002eb4:	185b      	adds	r3, r3, r1
20002eb6:	6ba1      	ldr	r1, [r4, #56]	@ 0x38
20002eb8:	434a      	muls	r2, r1
20002eba:	4694      	mov	ip, r2
20002ebc:	6bc2      	ldr	r2, [r0, #60]	@ 0x3c
20002ebe:	44b4      	add	ip, r6
20002ec0:	6bee      	ldr	r6, [r5, #60]	@ 0x3c
20002ec2:	3040      	adds	r0, #64	@ 0x40
20002ec4:	4356      	muls	r6, r2
20002ec6:	18f6      	adds	r6, r6, r3
20002ec8:	6be3      	ldr	r3, [r4, #60]	@ 0x3c
20002eca:	3540      	adds	r5, #64	@ 0x40
20002ecc:	4353      	muls	r3, r2
20002ece:	3440      	adds	r4, #64	@ 0x40
20002ed0:	4463      	add	r3, ip
20002ed2:	42b8      	cmp	r0, r7
20002ed4:	d188      	bne.n	20002de8 <spatial::Source::Process(long)+0x3b4>
```
