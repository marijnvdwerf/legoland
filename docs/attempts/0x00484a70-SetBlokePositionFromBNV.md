# SetBlokePositionFromBNV (0x00484a70, `src/legoland/bloke.c`)

**Best: 95.12%**. Kind: C.

## What still differs

- Third normalisation sqrt (around 0x484b28-0x484b44): the original's x87 stack order differs from ours, so the sum is computed in a different FP stack order and our `fstp st(3)` / `fxch` sequence does not line up.
- `xor edi, edi` (the loop counter `i = 0`) is scheduled by the original inside the third sqrt (0x484b36, between FP ops). Ours puts it after the `fdivr` (0x484b44).
- The epilogue (`add esp, 8; ret`) is reported as extra in ours; it looks like a knock-on of the shifted FP block.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku agent | Third sqrt sum reordered to `m30*m30 + m2c*m2c + m28*m28` (original load order) | 94.15% (no change) |
| 2026-10-07 | Haiku agent | Third sqrt sum reordered to `m30*m30 + m28*m28 + m2c*m2c` | 94.15% (no change) |
| 2026-10-07 | Haiku agent | `int i = 0;` at declaration, loop as `for (; i < 8; i++)` | 94.15%, but the diff moves into the prologue (worse shape) |
| 2026-10-08 | permuter + Opus 5.5 | `float m28` temp for the third sqrt; `register int z`; `m14 = m14 * scale` | 94.15 -> 95.12 |

## Ideas not tried yet

- Write the third normalisation with separate `len` steps and an explicit float temp for the squared sum, to change FP stack depth at the sqrt.
- Move the sumX/sumY/sumZ loop ahead of the third normalisation block.

## Notes

Earlier round notes (`docs/agent-workflow.md`) list this as a dead end (x87 stack order in the 3rd sqrt).
