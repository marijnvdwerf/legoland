# ValidateCursor (0x0045f810, `src/legoland/map_object.c`)

**Best: 94.15%** (start value). Kind: C.

## What still differs

- Building the `box` RECT inside the rect loop. The original loads `tile_y` first (top, then bottom) and
  loads `tile_x` later for left/right. Ours computes right/left first, so the code between `test ah, 0x20`
  and the `je` after `cmp edi, esi` is 2 bytes longer, and every later jump offset differs.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku 5.5 agent | `box` assigned in order top, bottom, left, right | 91.04% (worse) |
| 2026-10-07 | Haiku 5.5 agent | same as above, reverted to right, left, top, bottom | 92.01% (start) |
| 2026-10-08 | permuter + Opus 5.5 | `x1`/`y1` temps for rect->x1 / rect->y1 (x1 read before box.left, y1 before box.top) | 92.01 -> 94.15 |

## Ideas not tried yet

- Compute `tile_x` and `tile_y` into locals before the box assignments, so their loads are shared.
- Assign `bounds` after `box`.
