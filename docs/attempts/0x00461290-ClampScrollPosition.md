# ClampScrollPosition (0x00461290, `src/legoland/map_object.c`)

**Best: 43.92%**. Kind: C.

## What still differs

- The prologue and first load block match through `0x4612b5`. After that, register allocation: the original keeps `iv4` in `ebx`, `w` in `ebp` and `lpConfig` in `edx`; ours uses `edi`/`ebx`/`esi`. The later branch and clamp blocks are scheduled differently.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 r2 | Haiku agent | `hneg = -(lpConfig->height * w)` -> `-lpConfig->height * w` | no change |
| 2026-10-07 r2 | Haiku agent | `h = w >> 1` -> `w / 2` | 26.77% (worse: signed-division fixup) |
| 2026-10-09 | Opus 5.5 | right/left/bottom corner pairs as `struct Point` (all 7 subsets) | 28.04-44.44 harness (no gain; with left as a struct the frame grows to 0x2c) |

## Ideas not tried yet

- None recorded.
