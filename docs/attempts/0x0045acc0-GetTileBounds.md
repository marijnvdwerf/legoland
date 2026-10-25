# GetTileBounds (0x0045acc0, `src/legoland/tilemap.c`)

**Best: 88.89%**. Kind: C.

## What still differs

- (2026-10-09) Same as GetTileCentre: the `ScrollY >> 8` subtraction is scheduled before the `view_y` add, and `ref->x`/`ref->y`
  for the sum load in the other order. ScrollX/ScrollY are exported, so they cannot become one struct.

- Register assignment: the original loads `size` into `edx` and keeps `ref` in `ecx`.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 r2 | Haiku agent | Inlined the sprite lookup | no change or worse |
| 2026-10-07 r2 | Haiku agent | `int` vs `short` for the size | no change or worse |
| 2026-10-07 r2 | Haiku agent | A `dia` local | no change or worse |
| 2026-10-07 r2 | Haiku agent | Store into a `left` local, then `out[0]` | 14.43% (much worse) |
| 2026-10-08 | permuter + Opus 5.5 | new permuter (12 min): `volatile int top`, size assigned as a statement after the declarations | 28.30 -> 75.63 |
| 2026-10-08 | permuter + Opus 5.5 | by hand, before the permuter: out[2]/out[3] in the original's operand order, int size / dbl locals, out[3] first - all identical code | 28.30 (no change) x5 |
| 2026-10-09 | Opus 5.5 | no `top` local: write `out[1]` directly and use `out[1] - 1 + size` for out[3] (drops the volatile) | 75.63 -> **88.89** |
| 2026-10-09 | Opus 5.5 | non-volatile `int top`, `top -= ScrollY >> 8`, struct Point, `ref->y + ref->x` | 22.86-88.89 |

## Ideas not tried yet

- None recorded.
