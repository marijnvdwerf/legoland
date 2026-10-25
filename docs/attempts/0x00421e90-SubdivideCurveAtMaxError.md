# SubdivideCurveAtMaxError (0x00421e90, `src/legoland/castle.c`)

**Best: 99.09%** (kept). Kind: C.

## Tried

| Date | Model | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku 5.5 | `o->coef_t3 * 3.0f` replaced by `o->coef_t3 * FLOAT_004ab43c` (the global the original reads) | 98.18% -> 97.27% (fld/fmul order reversed: ours loads the constant first) |
| 2026-10-07 | Haiku 5.5 | `FLOAT_004ab43c * o->coef_t3` | 97.27% (same reversed order) |
| 2026-10-07 | Haiku 5.5 | `a = o->coef_t3; a = a * FLOAT_004ab43c;` (declarations first, C89) | 99.09% (kept) |
| 2026-10-07 | Haiku 5.5 | On top of the best: `int i;` with `i = 2` after `m = ...` | 99.09% (no change, the constant is still sunk to the loop) |
| 2026-10-08 | permuter + Opus 5.5 | `register int i`, `unsigned int i`, `register i` assigned right after m, do/while(--i), `for (i = 2; ...)` - mov esi, 2 stays after the sqrt call | 99.09 (no change) x5 |
| 2026-10-08 | permuter + Opus 5.5 | `r = roots` moved into the for init | 98.18 (worse) |

## What still differs

- `mov esi, 2` (loop counter `i`): the original sets it right after the `m` division (0x421eb6), ours
  sinks it to just before the loop (0x421f1a). Moving the assignment in source does not change that.
- Nothing else in the sqrt block or the root loop differs.
| 2026-10-09 | Opus 5.5 | index loop `for (i = 0; i < 2; i++)` over roots[i]; explicit `r = roots; i = 2;` after m / after b / before m | 96.83-99.09 (the original sets both `lea ecx,roots` and `mov esi,2` right after the m division; ours sinks them to the loop) |

## Ideas not tried yet

- Make `i` live across the sqrt block by using it there (e.g. a loop bound shared with the sqrt), so MSVC
  cannot sink the constant.

## 2026-10-21 (Opus 5.5): still 99.09
Finding: wrapping the quadratic solve in a `static __inline SolveQuadratic(a, b, c, roots)` helper (called in the
`a` block) fixes `mov esi, 2` / `lea ecx, roots`: they stay before the inlined body as in the original. But then b
is kept on the FPU stack (fst + fmul) where the original stores it and reloads it after computing c: 96.33.
volatile b / sq / parameter inside the helper: 93.15-95.41. Discriminant spelled 6 ways, c-before-b params: 96.33.
Without the volatiles in the current version: 95.41-97.27.
