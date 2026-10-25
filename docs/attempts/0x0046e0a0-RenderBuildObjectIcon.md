# RenderBuildObjectIcon (0x0046e0a0, `src/legoland/icon.c`)

**Best: 83.66%**. Kind: C.

## What still differs

- Stack layout of the locals: the original's `PrintCtx ctx` is at `[esp+0x14]` (flags, node, field_8 at 0x14/0x18/0x1c) and `buf` overlaps its lower bytes; ours puts `ctx` at `[esp+8]`.
- Extra 5 bytes in the `if (node->y > 0 ...)` block (`jle 0x1a2` in the original, `0x1a7` in ours).
- Attract-highlight block: the original reloads `EditMode.unk8` and `node->field_8` in a different order, and the `GetBlink()` ternary is laid out as a branch on the sprite pointer.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku 5.5 (a6) | `char buf[12]` moved to function top, before `ctx` | 83.66% (no change; ctx still at `[esp+8]`) |
| 2026-10-08 | permuter + Opus 5.5 | permuter 85.2% came from `0x156 < h + y` -> `h > 0x156 + y` (precedence bug); without it no gain | rejected |

## Best

83.66%, the version currently in the file.

## Ideas not tried yet

- Declare `ctx` in a nested block so MSVC allocates it at a different offset.
- Rewrite the attract-highlight `if` as nested ifs to change the branch layout.
