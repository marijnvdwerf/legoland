# DoMapAI (0x00462ef0, `src/legoland/map_object.c`)

**Best: 93.43%**. Kind: C.

## What still differs

- Frame: the original is `sub esp, 8` (`i` and the `TileId id`). Ours is `sub esp, 0x10`: the
  `unsigned char tx` / `ty` locals get spilled. Removing them (reading `tile->field_4/5` directly into
  `id.pos`) matches the frame but makes MSVC swap `lpConfig` (ebx in the original) and the zero
  constant (edi in the original), which drops the score to 81.24%.
- Hence the remaining diffs are the `je`/`jl` displacements that follow the spilled-local layout.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku agent | Baseline (start of this session) | 93.43% |
| 2026-10-07 | Haiku agent | `tx`/`ty` locals removed, `tile->field_4/5` read directly into `id.pos` | 81.24% (worse: lpConfig/zero register swap) |
| 2026-10-07 | Haiku agent | Same as above plus unused `cls` local removed (`cls` is used at the end, so this failed to build; restored) | 81.24% (same) |

## Ideas not tried yet

- Keep the direct `tile->field_4/5` reads but move the `lpConfig` load so its register is pinned to ebx
  (e.g. read `lpConfig->height` / `width` before the switch).
- Reorder the case 0/1/2 bodies only in the source (not the switch) to influence coloring.

## 2026-10-23 Opus 5.5

Harness 93.77. The only real difference: `tx`/`ty` get stack homes (`mov byte [esp+N], al; mov edx, [esp+N];
and edx, 0xff`), while the original keeps them in al/dl (`mov edx, eax; and edx, 0xff`). These did not change it
(93.73-93.77): block-scoped tx/ty, nested ifs, `char`, declaring them first, swapped id stores, reordering
sum/v, and a 203-order declaration search. Reading `tile->field_4/5` directly gives 81.54, assigning `id.pos`
before the compare 73.49, and loading both before the test 90.18.
