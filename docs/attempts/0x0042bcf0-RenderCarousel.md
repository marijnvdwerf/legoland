# RenderCarousel (0x0042bcf0, `src/legoland/carousel.c`)

**Best: 42.80%** (round 3: `local_28` has 10 entries, not 11). Kind: C.

## What still differs

- Frame: ours `sub esp, 0x5c` (after the round-3 fix), original `0x68`: the original has 12 more bytes of locals below `local_28`, so every stack offset in the body is off.
- The original zeroes a dword at `[esp+0x48]` right after the GetLayer call; that slot is live, so it is a real variable we don't have yet.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 r2 | Haiku agent | Moved `ride = param_1->ride` after the zero-fill (so `param_1` lands in `edx`) | 40.21% (worse) |
| 2026-10-07 r2 | Haiku agent | Block-scoped `layerres` into the GetLayer call | no change |
| 2026-10-07 r3 | Haiku 5.5 | `local_28` as `[10]` instead of `[11]` (the zero-fill and the `[esp+0x50]` store show 10 entries) | 41.03% -> 42.80% (kept) |
| 2026-10-07 r3 | Haiku 5.5 | An `int local_38[2]` zeroed after GetLayer | no change: MSVC dropped it as dead |

## Ideas not tried yet

- Find the 12 bytes of locals the original has below `local_28`, starting with what it stores at `[esp+0x48]` after GetLayer (it must be read later, or MSVC drops it).
