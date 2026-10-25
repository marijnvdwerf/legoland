# RemapTexCoordsToCell (0x00442040, `src/legoland/render3d.c`)

**Best: 45.93%**. Kind: C.

## What still differs

- (2026-10-08) After the idx1-first change only the frame differs: every float slot in the original is 4 bytes higher. The original's `[esp+0x3c]` is the fild scratch slot and `[esp+0x40]` is reserved but never used - some variable keeps a home slot that MSVC never touches (v5 lives on the FPU stack in both).
- Frame: original `sub esp, 0x30`, ours `0x2c` (one slot short).
- The original stores `lo_x`, `hi_x`, `lo_y`, `hi_y` at `[esp+0x2c..0x38]` in that order; the float temporaries are at `[esp+0x40..0x54]`.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 r2 | Haiku agent | Reordered the locals | no change |
| 2026-10-08 | permuter + Opus 5.5 | idx1 and iVar22 computed first (the original's first loads are idx1 into ebp, then iVar22 into edi) - prologue now matches | 41.17 -> 45.93 |
| 2026-10-08 | permuter + Opus 5.5 | float declaration variants (v5 first, s0/s1 first or split, extra s2, all declared at top) for the missing slot | 45.93 (no change) x5 |
| 2026-10-08 | permuter + Opus 5.5 | new permuter --decl-orders (6 min, 736 orders): best 44.30, found idx1 early on its own but not the full hand order | 44.30 (worse than current) |

## Ideas not tried yet

- Find the missing 4-byte local.
