# SpaceTowerUpdate (0x0043bac0, `src/legoland/space_tower.c`)

**Best: 75.32%**

## What still differs

(not analysed yet)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | `register` bloke/obj, `unsigned int y`, `char seat`, node loop as while, tile_y / coord_y temps | 67.51 -> 75.32 |

## Ideas not tried yet

- 

## 2026-10-23 Opus 5.5

Fresh harness 76.96. The original frame is 0x10 with one slot never used ([esp+0x18]); `ride` lives in the
dead `param_1` slot in both. The original keeps `tile->pos.y` from the header in edx up to case 8, reloads it
through the `tile` pointer in case 3, and reads `field_36` as an unsigned int (`xor; mov cl` plus `shr` for
`seat >> 1`). Rewriting case 3/8 that way and using `unsigned int seat` gives 52.98-75.57: every change
reshuffles the registers. A clean rewrite without the permuter leftovers gets 53.98.
