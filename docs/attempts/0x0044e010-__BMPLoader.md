# __BMPLoader (0x0044e010, `src/legoland/gfx.c`)

**Best: 39.57%**. Kind: C.

## What still differs

- Frame layout: the original has `ext` at `esp+0x4c`, ours at `esp+0x50`. Everything above the palette (the memset at `esp+0x164`) matches, so 4 bytes too many sit in the locals below `ext`; candidates are the spilled scalars around `esp+0x10..0x1c`. The first differing instruction is `lea eax, [esp+0x4c]` (original) vs `[esp+0x50]` (ours); the `type` spill is at `[esp+0x10]` in the original and `[esp+0x14]` in ours.
- Register allocation: the original keeps the GetGFXFName result in `ebp` and the file in `ebx`; ours uses `ebx` for the path.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 r2 | Haiku agent | `_stricmp(".lls", ext)` argument order (matches the original's pushes) | no change |
| 2026-10-07 r2 | Haiku agent | `i`/`j` declared inside the blocks that use them | no change |
| 2026-10-07 r2 | Haiku agent | `lls`/`size` declared inside the LLS branch | no change |
| 2026-10-07 r2 | Haiku agent | `ext` as `[0x100]` | 37.63% (worse: moves the palette too) |
| 2026-10-07 r2 | Haiku agent | Reordered `palette`, `ext`, `header`, `info` | no change |
| 2026-10-07 r3 | Haiku 5.5 | `lut` declared inside the 8-bit block, `out` at function scope | no change |
| 2026-10-07 r3 | Haiku 5.5 | Removed the `type` local (assign `image->field_14` directly) | 37.53% (worse) |

## Ideas not tried yet

- Find the extra 4-byte scalar (one of the values the original keeps in a register).
