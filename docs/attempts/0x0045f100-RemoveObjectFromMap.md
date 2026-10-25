# RemoveObjectFromMap (0x0045f100, `src/legoland/tilemap.c`)

**Best: 63.78%**

## What still differs

(see the rows below)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-09 | Opus 5.5 | `int cx, cy` (the original zero-extends the bytes into 32-bit and keeps cx in a local slot, cy in the coords slot); + one `tile` pointer per inner iteration | 37.99 / 35.75 (cx/cy get registers in ours; the original spills both and runs the inner loop as a down-counter) |

## Ideas not tried yet

- 
