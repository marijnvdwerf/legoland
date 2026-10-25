# SetupControllers (0x00451e70, `src/legoland/controller.c`)

**Best: 91.89%**. Kind: C.

## What still differs

- The original loads `2` into `ecx` after the `CONTROLLERBUFFER` store and keeps `4` in `edx` until the `DAT_00813a54` store, which comes after `GamePad`/`DAT_00813a4c`. Ours hoists both constants to the top of the function (`mov ecx, 4; mov eax, 2`) and CSEs the `4` into `MouseTileY`.
- Final `ControllersInitialized` store and return path differ in placement (`je` target).

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku 5.5 (agent a3) | `struct CtrlBuffer *buf` local for the malloc result, with `CONTROLLERBUFFER = buf` | 80.56% (no change) |
| 2026-10-08 | Haiku 5.5 (agent a3) | `MouseTileY = DAT_00813a54;` instead of `= 4` | 80.56% (no change) |
| 2026-10-08 | permuter + Opus 5.5 | DAT_00813a4c stored right after DAT_00813a5c; constant-first / cast compares on ControllersInitialized and CONTROLLERBUFFER | 80.56 -> 91.89 |
| 2026-10-09 | Opus 5.5 | assignments in the original store order (a5c, ac8, GamePad, a4c, ...) | 83.78 (worse; the original reloads CONTROLLERBUFFER right after the "2" stores, ours schedules the 4 constant first) |

## Ideas not tried yet

- Reorder the `DAT_00813a54` store relative to the `DAT_00813a4c` store (the store order is already the original's, so only a non-store change could move the constant load).
