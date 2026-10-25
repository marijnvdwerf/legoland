# RenderRestaurant2 (0x00430b10, `src/legoland/eatery.c`)

**Best: 97.36%**. Kind: C.

## What still differs

- The original shares the "layer 3 LLSSetFrame + PrintSprite" tail between the `s18 == 2` branch and the
  `s18 == 4 || s18 == 5` branch, and it jumps into the shared block from the `s18 == 4/5` side. Ours lays out
  the layer-3 print differently (the `s18 == 4/5` side jumps to a copy placed elsewhere), so about 20 lines
  at the branch joins differ.
- Smaller: the `s18 == 4 || s18 == 5` chain ends in a `jne` to a different target (`0x14a` vs `0x169` in the
  original), which follows from the block layout above.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku agent | Start: if / else-if chain (`s18 == 0 \|\| 1`, `== 2`, `== 5 \|\| 4`) | 97.36% |
| 2026-10-07 | Haiku agent | Same branches rewritten as `switch (s18)` with `case 0: case 1:`, `case 2:`, `case 4: case 5:` and `break`s | 42.79% (much worse, switch codegen) |

## Ideas not tried yet

- Reorder the branches (`s18 == 4/5` block before the `s18 == 2` block) to change which side gets the shared tail.
- Write the layer-3 print once per branch with the `LLSSetFrame` before it moved, so the tail matches on both sides.

## 2026-10-24 Opus 5.5 (no change: 97.36)
The only structural diff: the s18 == 2 and s18 == 4/5 branches end with the same layer-3 PrintSprite. The original
keeps the copy in the s18 == 2 branch and the 4/5 branch jumps back to it; ours keeps the later copy. A `goto` to a
label on the first copy: 71.57. The chain as a switch (either 4/5 order): 43.22.
