# ExplorersInstituteUpdate (0x0043a1e0, `src/legoland/shops.c`)

**Best: ?**

## What still differs

(see the rows below)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-09 | Opus 5.5 | clean cases 3/4 (no volatile temps, case 4 dest.x before dest.y as the original) | 74.42 (the original tail-merges case 3 into case 4 after the dest.x store; without the temps ours keeps separate tails) |

## Ideas not tried yet

- 
