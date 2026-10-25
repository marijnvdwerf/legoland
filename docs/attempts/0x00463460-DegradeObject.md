# DegradeObject (0x00463460, `src/legoland/map_object.c`)

**Best: 70.07%** (current code, with a volatile threshold)

## What still differs

The original keeps durability_level in cl, loads the flags word into dx (no zeroing), zero-extends a copy for the old
flags (`mov edi,edx; and edi,0xffff`) and tests the updated flags from dl/dh.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-09 | Opus 5.5 | no volatile, plain `int threshold` | 69.17 |
| 2026-10-09 | Opus 5.5 | + `unsigned short nf` for the new flags (two forms) | 67.67-69.17 |
| 2026-10-09 | Opus 5.5 | + `unsigned char level` local | 59.26 |

## Ideas not tried yet

- 
