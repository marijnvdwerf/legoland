# JungleCruiseBuildStepPath (0x00433840, `src/legoland/jungle_cruise.c`)

**Best: 83.61%** (start 83.61%). Kind: C.

## What still differs

- The original keeps the loop counter in `esi` and the walking pointer in `edi` in the final ring loop, and `arc` in `eax`; ours puts `i` in `edi` and the arc in `esi`.
- The arc block addresses the table as `[ecx+eax]` (the original's `arc` is in `eax`), ours loads it into `esi` and adds.
- The `to >= from` arm of the ternary is placed far away in the original; ours inlines it.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku agent a7 | Final ring loop walks `p = &step_offsets[-6]` (+2 per iteration) instead of indexing `(i+4)*2` and `(i-3)*2` | 84.23% (+0.62) |
| 2026-10-08 | Haiku agent a7 | Same, plus a `frame` pointer for the `step_frames` store | 84.23% (no change, reverted) |
| 2026-10-08 | Haiku agent a7 | Replaced the nested `to < from ? ... : ...` ternary with an `int ok` flag set in an if/else before `if (ok == 0)` | 84.23% (no change, reverted) |
| 2026-10-08 | permuter + Opus 5.5 | permuter 84.8% came from swapping `* DAT_004ab3fc + sy` to `* sy + DAT_004ab3fc` (precedence bug); without it no gain | rejected |

## Ideas not tried yet

- Write the arc table pointer as `arc = DAT_004b7188 + bit` directly to change which register `arc` lands in.
- Declare `arc` as a plain `int` offset and index the table from it.
