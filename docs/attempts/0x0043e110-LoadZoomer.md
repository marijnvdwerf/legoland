# LoadZoomer (0x0043e110, `src/legoland/plane_ride.c`)

**Best: 97.89%** (unchanged source from the earlier round). Kind: plain C.

## What still differs

- The `ZoomerSampleParams` stores: the original reads `node[1]` into `cl` before the `field_8` store and
  delays the `field_c` store (`mov [esp+0x2c], ecx`) until after the call pushes. Ours stores `field_0 = 2`
  before the pushes and reads `node[1]` after `mov eax, DAT_004b79d0[8]`.
- Possibly fixable only by a layout/scheduling change the compiler does not expose from C.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku 5.5 | Baseline (field_8, field_0, field_c order) | 97.89% (kept) |
| 2026-10-07 | Haiku 5.5 | Order field_8, field_c, field_0 | 97.89% (same, different diff lines) |
| 2026-10-07 | Haiku 5.5 | Order field_c, field_8, field_0 | 95.79% (worse) |
| 2026-10-07 | Haiku 5.5 | Order field_0, field_8, field_c | 95.79% (worse) |
| 2026-10-07 | Haiku 5.5 | Read both bytes into `unsigned char` locals at the top of the loop body | 69.00% (frame grows 0x14 to 0x18, worse) |
| 2026-10-08 | permuter + Opus 5.5 | params store orders (8/c/0 same score; 0-8-c, c-8-0, c-0-8, 0-c-8 worse); params declared in the loop / first - the original sinks `field_0 = 2` into the call's pushes | 97.89 x3 / 93.68-95.79 |

## Ideas not tried yet

- Read the bytes through a different expression (`*((unsigned char *)node + 1)`) or use `unsigned int`
  temporaries for the field_c value.
- Restructure the `SaveGameRead` loop so the node is assigned to `prev` before the parameter block.
