# HTBubbleHelp (0x004557c0, `src/legoland/text.c`)

**Best: 100.00% (verified)**

## What still differs

(not analysed yet)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | `register HGDIOBJ old_font`, cell_h read before box.right | 74.02 -> 76.47 |

## Ideas not tried yet

- 

## 2026-10-23 agent notes (76.47% -> 100.00%)


## Restore the intermediate bubble rectangle

Current baseline: harness/full verify 76.47. The source held the tentative
right edge in a scalar local and did not store box.right until after the
clamp. The original writes both box.left and box.right before the branch.
Using box.right directly restores that store and register lifetimes: 98.05
in the harness. Removing register (no change) and the redundant cell_h
copy, reading the cached height directly, gives 98.54. The only remaining
harness hunk is our epilogue beyond the reported original extent.
No padding, volatile, assembly, or copied data added.

Full per-function verification of the rectangle fix: **76.47% -> 100.00%**,
with no other function regressions. Harness-only epilogue differences do not
remain in reccmp.
