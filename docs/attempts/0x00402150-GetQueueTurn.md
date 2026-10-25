# GetQueueTurn (0x00402150, `src/legoland/ride_bloke.c`)

**Best: ?**

## What still differs

(see the rows below)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-09 | Opus 5.5 | clean version: `q == NULL || q->id != id` combined, no volatile temps | 61.24 / 88.63 (worse than the permuter version 92.17) |

## Ideas not tried yet

- 
