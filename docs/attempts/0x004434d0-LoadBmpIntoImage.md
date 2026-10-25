# LoadBmpIntoImage (0x004434d0, `src/legoland/challenge.c`)

**Best: 84.04%** (start value, no change yet). Kind: C.

## What still differs

- Spill slots for the copy loop pointers. The original keeps `dst` in `[esp+0x10]` (stored at `[esp+0x18]` before the call at `0x443621`, then reloaded at each row) and `src` in `[esp+0x64]` (the parameter slot). Ours spills `dst` to `[esp+0x64]` and keeps `src` in `ebp`.
- The three locals `pixels`, `pixel_offset` and `offbits` sit one 4-byte slot lower in ours (`+0x10`, `+0x14`, `+0x18`) than in the original (`+0x14`, `+0x18`, `+0x1c`). The original has an extra slot at `+0x10` (the `dst` spill).
- Inner copy loop: the original keeps `x` in `edi` and `src` in `eax`, and writes `*dst` through `ebp`. Ours uses the reverse role for the two pointers.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku 5.5 (a2) | Declaration order of `offbits` .. `y`: `dst`/`src` moved first, `dst` moved before `pixels`, `dst`/`src` moved last | 82.98% (no change; declaration order does not matter here) |
| 2026-10-08 | Haiku 5.5 (a2) | `src = pixels + row_size` moved after the first `RES_SetFilePointer` | 79.79% (worse) |
| 2026-10-08 | permuter + Opus 5.5 | `register` file, `int aligned_width`, clear loop and inner copy loop as while loops, `(unsigned)param_1->height` compare | 82.98 -> 84.04 |

## Best

82.98%, the version currently in the file (unchanged from the start).

## Ideas not tried yet

- Make `dst` a value that is read back from `param_1->data` after the NULL check, so it is spilled like the original's `[esp+0x10]` (the original stores the malloc result and writes `param_1->data` without reloading it).
- Compute `src` as `pixels + row_size` in the same statement as the `RES_SetFilePointer(file, pixel_offset)` setup, to change which pointer gets the parameter slot.
- Use the 8-bit branch (`cmp word ptr [esp+0x42], 8`) as the `if` body with `dst`/`src` declared inside a block, which may move the spill decisions.

## 2026-10-23 agent notes (89.71% -> 91.10%)

Keep separate successful cleanup paths for the 8-bit copy branch and the fallback; MSVC6 cross-jumps their free/RES_CloseFile tails differently from one shared source cleanup. Removed the no-op register storage specifier. Bottom-up BMP pixel copying and ownership behaviour unchanged. Remaining mismatch is the inner copy loop's pointer/index register allocation. Tested indexed, pointer, countdown and do/while forms; none improved further.
