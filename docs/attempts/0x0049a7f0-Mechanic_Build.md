# Mechanic_Build (0x0049a7f0, `src/legoland/worker.c`)

**Best: 96.15%**. Kind: C.

## What still differs

- Cell lookup in case 0x6c (`cell = &GameMap[y][x]` after the bounds check). The original computes `x*5` into `edx` first (`lea edx,[eax+eax*4]`), then loads `GameMap` into `eax` with the 5-byte short form, then `lea eax,[ecx+edx*4]`. Ours loads `GameMap` into `edx` (6-byte form) first. This makes case 0x6c 1 byte longer, which shifts the jump table entry for case 0x6d by one byte (`start+0x276` vs `start+0x277`), and the later jumps by one byte.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku agent | `cell = GameMap[coords[1]] + coords[0];` | 96.15% (no change) |
| 2026-10-07 | Haiku agent | `cell = &GameMap[worker->order->pos.y][worker->order->pos.x];` | 90.79% (worse) |
| 2026-10-07 | Haiku agent | Row temp `row = GameMap[coords[1]]; cell = &row[coords[0]];` | 96.15% (no change) |

## Ideas not tried yet

- Different bounds-check shape (for example a single combined check, or `unsigned` casts on the coords) to change which register holds `x` when the global is loaded.
- Compute the `x` index via a separate local before the bounds check.
