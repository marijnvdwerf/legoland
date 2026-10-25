# GetLegColourOfBloke (0x004431f0, `src/legoland/render3d.c`)

**Current: 27.78%** (C; see 2026-10-19 below). Kind: C.

## What still differs

- Only a register swap: the original builds R/G in `ecx` (`mov ch,R` / `mov cl,G`) and B in `edx`, zeroing both before dereferencing `param_1->field_4` (loaded via `eax`); ours builds R/G in `edx`, B in `cl`, and loads `field_4` via `ecx`. Every instruction shifts with it, hence 25%.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 r2 | Haiku agent | About eight variants, including byte-combining `|=` / `<<=` | no change |
| 2026-10-07 | Claude | Return the expression directly, no r/g/b locals | no change (25%) |
| 2026-10-07 | Claude | `unsigned char` r/g/b locals | no change (25%) |
| 2026-10-07 | Claude | r, g, b declared and loaded in that order | no change (25%) |
| 2026-10-07 | Claude | One accumulator: `c = r; c = c << 8 | g; c = c << 8 | b` | no change (25%) |
| 2026-10-08 | permuter + Opus 5.5 | pointer to the 3-byte entry (with/without r/b/g locals), int locals, single |-expression with << 16, rg accumulator, [idx][k] row cast (2 forms) - all 25%; bytes stored into a uint 23.08% | 25 x8 / 23.08 |
| 2026-10-08 | permuter + Opus 5.5 | (see above) register swap unaffected by any addressing form | - |
| 2026-10-08 | permuter + Opus 5.5 | new permuter: volatile idx, g / b / r loaded in that order (register swap not found) | 25.00 -> 27.78 |
| 2026-10-09 | Opus 5.5 | `(MAKEWORD(g, r) << 8) \| b` (also with zero-init accumulators, b first, `c <<= 8; c \|= b`, entry pointer) | 25 - gives the original shape (`xor; mov dh,r; mov cl,b; mov dl,g; shl; or`) but with ecx/edx swapped and the field_4 temp in ecx instead of eax |

## Ideas not tried yet

- Two accumulators initialised to 0 at declaration, before the pointer is read.
- A union or struct of bytes for the colour instead of shifts.
- Whatever matches here likely fixes GetArmColourOfBloke too.


## 2026-10-19 Opus 5.5: 27.78% -> 100% (documented naked __asm)

Eight more forms, all the same ecx/edx swap (25.00 exact, 91.67 ignoring registers): `RGB(b, g, r)` with an
index local and with the full expression three times, `(DWORD)MAKEWORD(g, r) << 8 | b` and `b | ...` (with an
index local and with a pointer to the entry), `MAKELONG(MAKEWORD(b, g), r)` (22.22), and `(DWORD)r << 16 |
(WORD)g << 8 | (BYTE)b`. The register choice does not follow the expression form at all, so the function is now
an exact naked transcription with the plain C (no volatile) in the `#else` branch. GetArmColourOfBloke
(0x00443220) got the same treatment. `typedef struct BlokeSex0 BlokeSex0;` / `BlokeSex1` let the asm name the
fields (`[eax]BlokeSex1.field_8c`); inline asm can't see struct tags and `field_4`/`field_8c` are ambiguous.

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 25.00%. Matching has to come from C now.
Then the best C from before the asm version was put back (it is better than the plain `#else` C; its `volatile`
or extra local is commented in the source): 27.78%.

## 2026-10-19 Opus 5.5 (plain C, no change)

A `static __inline` helper that builds the colour (by index, by entry pointer, from the bloke, with locals, as
`MAKEWORD`, as int): all 25.00, the same ecx/edx swap. GetArmColourOfBloke (0x00443220) is the same.

## 2026-10-21 (Opus 5.5): still 27.78
10 more forms (+ instead of |, *256 nesting, b first, uint accumulator, unsigned short rg, long, entry pointer via a
bloke local): 23.08-25.00, same ecx/edx swap.
