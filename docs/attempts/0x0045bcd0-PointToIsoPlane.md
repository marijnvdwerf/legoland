# PointToIsoPlane (0x0045bcd0, `src/legoland/tilemap.c`)

**Best: 46.84%** (round 2 version). Kind: C.

## What still differs

- The original's `sub esp, 0xc` frame is not reproduced.
- Screen-to-isometric maths: watch signed shifts/divisions and float vs int.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 r1 | Haiku agent | Changed a type to `char` | 23.67% -> 22.04% (worse, reverted) |
| 2026-10-07 r2 | Haiku agent | Load `*param_1` and `param_1[1]` once into `int x`, `int y` (the original reads each once) | 23.67% -> 25.70% (kept) |
| 2026-10-08 | permuter + Opus 5.5 | half9 = (iVar9 + 1) >> 1 as a local right after x/y, used in the comparison (the original holds it in ebp, which pushes iVar1-3 to the stack) | 25.70 -> 30.47 |
| 2026-10-08 | permuter + Opus 5.5 | new permuter (12 min): sprite / y routed through temps, volatile y and iVar5, register iVar3, half9 after iVar1, iVar2/iVar6 order swapped. Its unsigned iVar4 (47.65%) made (x + iVar4) / iVar8 an unsigned division - kept signed | 30.47 -> 46.84 |

## Ideas not tried yet

- Find the three locals behind the 0xc frame.
