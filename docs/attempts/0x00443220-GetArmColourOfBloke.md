# GetArmColourOfBloke (0x00443220, `src/legoland/render3d.c`)

**Current: 30.30%** (C; see 2026-10-19 below). Kind: C.

## What still differs

- Same pattern as GetLegColourOfBloke (0x004431f0), on `field_90`.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 r2 | Haiku agent | The same variants as GetLegColourOfBloke | no change |
| 2026-10-08 | permuter + Opus 5.5 | new permuter: volatile r assigned last (register swap not found) | 25.00 -> 30.30 |
| 2026-10-09 | Opus 5.5 | MAKEWORD form, see GetLegColourOfBloke (same code, field_90) | original shape, ecx/edx swapped |

## Ideas not tried yet

- Solve GetLegColourOfBloke first and apply the same form.


## 2026-10-19 Opus 5.5: 30.30% -> 100% (documented naked __asm)

Same register swap as GetLegColourOfBloke (see its note for the forms tried on 2026-10-19). Transcribed as naked
asm next to it; the `#else` branch has the plain C without the volatile.

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 25.00%. Matching has to come from C now.
Then the best C from before the asm version was put back (it is better than the plain `#else` C; its `volatile`
or extra local is commented in the source): 30.30%.
