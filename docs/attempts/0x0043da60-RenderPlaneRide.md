# RenderPlaneRide (0x0043da60, `src/legoland/plane_ride.c`)

**Best: 96.54%**. Kind: C.

## What still differs

- Register choice: the original keeps the element pointer in `eax` and the zero in `ecx`; ours is swapped.
- The byte `n = 0` is stored as an immediate in the original (`mov byte [esp+0x3c], 0`); ours stores `al`.
- Tail: the original's epilogue is `add esp, 0x14` / pops / `add esp, 0x24` / `ret` with the `ret` in a separate
  block; ours differs in the tail merge.
- Scheduling of `DAT_0081caec` load and `ride->riders` (`edi+0xcc`) reload in the loop-to-PrintSprite region.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 p6 | Haiku agent | `char n = 0;` changed to `char n;` plus `n = 0;` after `r = ride->riders;` | 93.26% to 95.45% (kept) |
| 2026-10-07 p6 | Haiku agent | Moved `n = 0;` to the first statement | 93.26% (worse) |
| 2026-10-07 p6 | Haiku agent | Moved `off2.y = DAT_0081caec;` after `person->offset.x` | 94.72% (worse) |
| 2026-10-08 | permuter + Opus 5.5 | `register` coords; first rider loop as while with `rider` temp; ride list loop as while with `r = ride->riders` before the ZoomerSprite store; `x = coords.x` temp in last PrintSprite | 95.45 -> 96.54 |

## Ideas not tried yet

- Reorder `ride = element->ride` / `r = ride->riders` relative to the `n = 0` store to flip the eax/ecx choice.
- Use `element->ride` through a differently typed temporary to change which register the element lives in.
