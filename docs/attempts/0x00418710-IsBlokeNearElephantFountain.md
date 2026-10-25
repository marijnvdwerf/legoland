# IsBlokeNearElephantFountain (0x00418710, `src/legoland/water_works.c`)

**Best: 82.69%** (unchanged from start). Kind: C.

## What still differs

- Call setup register choice: the original loads `FirstBloke` into `eax` and computes `&rect` into `edx` (`lea edx,[esp+4]` first); ours loads `FirstBloke` into `ecx` and computes `&rect` into `eax`. Same for the Gardener and Mechanic calls (original `lea ecx,[esp]`, `mov edx,[GardenerList]`).
- Structure (three calls, `return 1` early exits, final `neg/sbb` return) already matches.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Claude Haiku 5.5 | Single `return a || b || c` expression | 68.09% (worse) |
| 2026-10-09 | Opus 5.5 | `struct WaterRect *r = &rect` passed to the calls (assigned before the stores or before the call), stores through `r->` | 82.69 (copy-propagated, no change) |
| 2026-10-09 | Opus 5.5 | store order x0, y0, x1, y1 | 60.95 |

## Ideas not tried yet

- Pass `&rect` through a `struct WaterRect *` local, or compute `rect.x0/x1/y0/y1` in a different store order, to change which register the address lands in.

## 2026-10-21 agent notes (82.69% -> 100.00%)

# IsBlokeNearElephantFountain (0x00418710, `src/legoland/water_works.c`)

**100%** (was 82.69%). Kind: C.

## What fixed it

`rect.x1` / `rect.y1` are written as the full sums again (`coords[0] + base[0] + 3`) instead of `rect.x0 + 3`.
Reading back the stored field kept the sum in a register across the stores, which shifted the register
choice for `&rect` and the list pointers at the three `ListHasPointInRect` calls (edx/eax vs eax/ecx).

## Tried

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-21 | Opus 5.5 | `struct WaterRect *r` pointer, `int` return, base-first sums, x/y temps | 60.95-82.69 |
| 2026-10-21 | Opus 5.5 | `x1 = coords[0] + base[0] + 3`, `y1 = coords[1] + base[3] + 3` | **100** |
