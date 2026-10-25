# UpdateControllerFromMouseData (0x00473b00, `src/legoland/input.c`)

**Best: 90.83%**. Kind: C.

## What still differs

- The original reloads `buffer->mouse_threshold1` for each `abs()` compare, and the zero it compares against is kept in `edx` (`xor edx,edx; cmp eax,edx`) for both the x<0 and y<0 clamps. Ours uses `test eax,eax` and immediate-0 stores there.
- The x-clamp block reads `lpConfig` into `edx` at a different point. The original loads `edi` = `lpConfig` before the first clamp and keeps `edi` as the pointer to `MouseState`/`lpConfig` throughout.
- Register choice for the `buffer->x`/`buffer->y` values after the clamps differs (`edi`/`esi` vs `ebx`/`ebp`).

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku 5.5 (agent a3) | `((volatile struct CtrlBuffer *)buffer)->mouse_threshold1` for both threshold compares in the accel block | 85.32% -> 88.99% (kept) |
| 2026-10-08 | permuter + Opus 5.5 | `register int dy`, `unsigned int mode`, `cur_x` temp for delta_x, flags computed after delta_y, `2 | buttons` | 88.99 -> 90.83 |

## Ideas not tried yet

- Read `mouse_threshold2` through a volatile cast too, to match the reload pattern in the mode==2 block.
- Write the x/y clamps with a shared zero local so MSVC keeps 0 in a register for the stores and compares.

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 82.73%. Matching has to come from C now.
Then the best C from before the asm version was put back (it is better than the plain `#else` C; its `volatile`
or extra local is commented in the source): 93.21%.
