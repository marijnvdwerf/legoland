# RenderEateryBaseLayers (0x004304e0, `src/legoland/eatery.c`)

**Best: 87.10%**. Kind: C.

## What still differs

- Early in the function the original loads `*param_1` into `cx` and `*(param_2+0xc4)` into `eax`; ours uses `ax` / `ecx`.
- The original's `je` to the NULL return is 2 bytes further (`je 0x5cb`, ours `je 0x5c9`), so the return path differs in size.
- The `switch` load order: the original loads `state->field_8` (`cl`, spilled to `[esp+0x34]`) and `field_9` before `state->field_18`; ours loads `field_18` first.
- Each `GetRenderOffsetForLayer` / `AdjustOffsetForViewMode` / `GetSpriteForLayer` block has its push and store order rotated (the `Point off` return is stored and passed in a different order).

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku agent (a5) | Swap `cfg.field_8 = *param_1` and `cfg.field_4 = ...` order | 86.72% (+0.38) |
| 2026-10-08 | Haiku agent (a5) | Put `cfg.field_0 = 0x103` first, then field_4, field_8 | 87.10% (+0.38) |
| 2026-10-08 | Haiku agent (a5) | Move `c8`/`c9` loads before the `sy`/`sx` coords assignments | 87.10% (no change) |
| 2026-10-08 | permuter + Opus 5.5 | permuter 96.0%: off.y read into a temp BEFORE AdjustOffsetForViewMode(&off) (behaviour change); with the temp after it the layout breaks (36%). Not kept | rejected |

## Best

87.10%, the version currently in the file.

## Ideas not tried yet

- Declare `c8`/`c9`/`c6`/`c7` as `unsigned char` (the original uses `mov cl`/`mov bl` and `movsx` for the signed ones; `c7`/`c9` go through `movsx`).
- Write the `off` return as a temp `struct Point` per block to see if the `lea ecx,[esp+0x20]` placement changes.
