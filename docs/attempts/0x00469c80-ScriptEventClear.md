# ScriptEventClear (0x00469c80, `src/legoland/objectives.c`)

**Best: 94.32%**. Kind: C.

## What still differs

- Per-object loop body (around 0x469d57): the original reads `current->tile_y` / `tile_x` (unsigned char fields at +4 / +5 of `SweepInstance`) as `xor edx,edx; mov dl,[edi+4]`, zero-extended into a 32-bit register kept as `ebp`. Ours reads them as `movzx si, byte` / `movzx cx, byte` + `movsx edx, cx` with `short` locals.
- Register choice for the tile values and the sums (`add esi, edx` in the original vs `add ebp, edx` in ours) follows from the load type.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku agent | `tile_x` and `tile_y` locals as `unsigned char` | 80.86% (frame 0x185c, worse) |
| 2026-10-07 | Haiku agent | Only `tile_y` as `unsigned char` (`tile_x` stays `short`) | 84.30% (frame 0x1858, worse) |
| 2026-10-07 | Haiku agent | `tile_x` and `tile_y` locals as `int` | 65.33% (worse) |
| 2026-10-08 | permuter + Opus 5.5 | `right` temp for footprint.v[2]; tile_x read before tile_y; decl order | 93.18 -> 94.32 |

## Ideas not tried yet

- Keep `short` locals but read the byte fields straight into `point.y` / `point.x` (int) before the `rect` block, so the zero-extend lands in a 32-bit register.
- Split the `rect` computation so the footprint adds use the same register as the tile loads.

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 94.89%. Matching has to come from C now.

## 2026-10-19 Opus 5.5 (plain C, no change: 94.89%)

- `ObjectiveEvent.field_28` / `field_2c` were `unsigned int` next to the `int` `field_30` / `field_34`: the four
  are one rectangle (x0, y0, x1, y1) and the original compares all of them signed (`jl`/`jg`). Now all `int`; the
  two `jb` lines of the diff are `jl` like the original (score unchanged, the rest of the diff is the tile loads).
- With that fix, `int` (or `unsigned int`) `tile_x`/`tile_y` locals give the original's `xor edx,edx; mov dl,[edi+4]`
  zero-extending loads, but tile_x then takes ebx and `next` moves from ebx to ebp: 65.33. 30 random declaration
  orders of the int version: all 65.33. Fresh `left/top/right/bottom` assignments from int tiles: 54.12.
