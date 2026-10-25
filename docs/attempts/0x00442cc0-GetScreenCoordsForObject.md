# GetScreenCoordsForObject (0x00442cc0, `src/legoland/render3d.c`)

**Best: 100% (matched 2026-10-09)**

## What still differs

(not analysed yet)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | `unsigned int iVar1` (compared as (int)), `register struct Point r`, bounds0 temp, `bounds[0] + iVar2` in the fallthrough | 71.60 -> 79.01 |
| 2026-10-09 | Opus 5.5 | halve dx and dy the same way (signed, `-(-v >> 1)`), then one `r = bounds + d` tail: MSVC duplicates the tail into both y branches | **100** |

## Ideas not tried yet

- 
