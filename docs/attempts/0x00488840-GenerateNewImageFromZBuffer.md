# GenerateNewImageFromZBuffer (0x00488840, `src/legoland/render.c`)

**Best: 78.05%**

## What still differs

(not analysed yet)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | saved-rect stores reordered through right/left/top temps, `register` top, heights stored after both widths, yy loop as while, CurrentSurfaceDesc restored before .top | 71.19 -> 76.27 |
| 2026-10-09 | Opus 5.5 | DAT_0066b620..62c merged into `RECT DAT_0066b620` (saved clip rect); save/restore as struct copies (also field orders left/right/bottom/top around the surface copy) | struct copy 78.05 (kept, was 78.86 with four ints and register hacks); field orders 77.24-77.50 |

## Ideas not tried yet

- 
