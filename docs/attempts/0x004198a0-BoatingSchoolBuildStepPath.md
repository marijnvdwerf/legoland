# BoatingSchoolBuildStepPath (0x004198a0, `src/legoland/boating_school.c`)

**Best: 81.26%** (start 81.26%). Kind: C. Near-copy of JungleCruiseBuildStepPath (see that file).

## What still differs

- Same as the jungle version: register choice for the loop counter and `arc` in the arc/straight block, and the far placement of the `to >= from` arm.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku agent a7 | Final ring loop walks `p = &step_xy[-6]` (+2 per iteration) instead of indexing | 81.56% (+0.30) |
| 2026-10-08 | permuter + Opus 5.5 | permuter 83.5% came from `(i + 4) * 2 + 1` -> `(i + 4) * 1 + 2` (precedence bug); without it no gain | rejected |

## Ideas not tried yet

- Port the jungle ideas once they work there (arc pointer as `DAT_004b5158 + bit`).
