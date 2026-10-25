# Restaurant2Update (0x0042fbb0, `src/legoland/eatery.c`)

**Best: 41.89%**. Kind: C.

## What still differs

- No `push ebp` frame (`sub esp, 0x44`), so the frame-pointer pragma does not apply.
- The original keeps `ride` in `ebx` (spilled to `[esp+0x4c]`), `node` in `eax` and `pos` in `ebp`; ours keeps `node` in `ebp`, `pos` in `ebx` and `ride` only in a spill slot. The switch body and the save-block copy differ throughout.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 r2 | Haiku agent | Split the `ride` declaration from its initialisation | no change |
| 2026-10-07 r2 | Haiku agent | Made `ride` `unsigned int` | no change |

## Ideas not tried yet

- None recorded.
