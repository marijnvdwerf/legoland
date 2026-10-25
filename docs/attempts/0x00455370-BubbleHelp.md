# BubbleHelp (0x00455370, `src/legoland/text.c`)

**Best: 24.23%**. Kind: C.

## What still differs

- The call sequence matches the original (FUN_00455d40, the CreateCompatibleDC/SetBkMode/SelectFont/DrawTextA/DeleteDC branch, three RenderBlocks with GetNearestColour, five PrintSprite calls, PrintTextCell, the hover tests). The difference is stack layout and register allocation.
- The original keeps `font` in `ebp` and `text`, then `rect`, in `esi`; the `box` struct is at `[esp+0x3c..0x48]` and `sprite_arg` at `[esp+0x30..0x38]`. No `push ebp` frame, `sub esp, 0x38`.
- Not a bug (checked 2026-10-07): the second Hover check compares against `width`, which the C reuses for `corner_h + top4` (the middle section's top). The original computes the same value into `edi` with `lea edi, [ecx + ebx]`. Only the name is misleading.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 r1 | Haiku agent | Reordered the local declarations | no change |
| 2026-10-07 r2 | Haiku agent | `(unsigned int)lpConfig->screen_width` to `(int)` in the clamps (the original uses signed jl/jge/jle) | no change |

## Ideas not tried yet

- Restructure the `box`/`sprite_arg` locals so they land at the original's offsets (structs vs separate ints).
