# LogFlumeEntranceRemoveObject (0x0040abf0, `src/legoland/log_flume.c`)

**Best: 95.10%**. Kind: C.

## What still differs

- Store order inside the loop body. The original computes `cursor.tile_x` (edx) first, then stores
  `cursor.tile_y` (at `[esp+0x141c]`), then stores `cursor.tile_x` (at `[esp+0x1418]`), and only then runs the
  20-byte memcpy into `cursor.footprint.v`. Ours stores `tile_x` before `tile_y`.
- Load order: the original loads `LogFlumeFootprint+8` (x1) first, then `cur->tile.pos.x`, then `LogFlumeFootprint`.
- Everything else (the memcpy into `LogFlumeTrackRide->footprint`, the two decrements, the `StandardRemoveObject`
  call, the `flags10 & 2` check) already matches.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku 5.5 (flumea) | Swap the `cursor.tile_x` / `cursor.tile_y` assignment lines | 91.18% (worse) |
| 2026-10-07 | Haiku 5.5 (flumea) | `int tx` local holding tile_x, stored after tile_y | 82.35% (worse) |
| 2026-10-18 | Codex | Replace first/second/both footprint memcpy operations with struct assignment, or use `&cursor.footprint` as destination | 95.10% object score, unchanged; no edit retained |
| 2026-10-18 | Codex | Compute both coordinates in a Point, storing x/y or y/x; separate x/y locals; preload the footprint y offset | 72.55% / 82.35% / 95.10% object scores; no improvement, including combinations with the struct copies |
| 2026-10-18 | Codex | Save TileId before/between/after coordinate calculations or before the first copy; use saved tile coordinates or a TileId pointer | 60.19–70.87% object scores; changes later register allocation |
| 2026-10-18 | Codex | Move cursor footprint copy before or between coordinate stores | 72.55–74.51% object scores; worse |

## Ideas not tried yet

- A snapshot of the footprint offsets, rather than of the completed coordinates, may affect load scheduling.
- The typed footprint assignment and TileId-pointer forms have now been tried; neither fixes the order.
