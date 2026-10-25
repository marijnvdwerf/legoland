# LoadColourTable (0x0044e580, `src/legoland/gfx.c`)

**Best: 100% (matched 2026-10-09)**

## What still differs

(not analysed yet)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | lookup assigned before src | 82.43 -> 83.78 |
| 2026-10-09 | Opus 5.5 | index loop over 256 entries (MSVC strength-reduces it), peFlags = 4 after the lookup store; DAT_00813b20/DAT_00813e20 zero-initialised so they stay adjacent | **100** |

## Ideas not tried yet

- 
