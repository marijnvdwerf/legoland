# ScrollIconRegion (0x0046d850, `src/legoland/icon.c`)

**Best: 71.73%**

## What still differs

(not analysed yet)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | both scroll if/else swapped (negated), clip_left / content_bottom / field_0 temps, `dx + l` | 70.78 -> 71.73 |
| 2026-10-09 | Opus 5.5 | `l`/`t` and `nl`/`nt` as `struct Point` (each and both) | 71.73 (no change; the original has l/t in edx/edi and nl/nt in ebp/ebx, ours l/t in ebp/ebx) |

## Ideas not tried yet

- 
