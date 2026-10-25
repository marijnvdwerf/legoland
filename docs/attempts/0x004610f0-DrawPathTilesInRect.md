# DrawPathTilesInRect (0x004610f0, `src/legoland/map_object.c`)

**Best: 89.95%** (unchanged from start). Kind: C.

## What still differs

- Prologue register/stack layout. The original keeps `param_1` in `eax` (loaded from the arg at `[esp+0x3c]`)
  and `start.x` in `ecx`; ours keeps `param_1` in `edx` and `start.x` in `eax`.
- The original stores `size` (`[esp+0x14]`) before `dbl` (`[esp+0x10]`); ours stores `dbl` first and uses a
  `lea edx` temp where the original uses `ecx`.
- The inner loop body and the tail (from `0x461203`) match. Only the prologue and a few epilogue lines differ.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku agent | `param_1->x -= dbl; start.x = param_1->x;` instead of `start.x = param_1->x - dbl; param_1->x = start.x;` | 77.36% |
| 2026-10-08 | Haiku agent | `half_x` declared `int` | 83.09% |
| 2026-10-08 | Haiku agent | `half_y` declared `int` | 63.37% |
| 2026-10-08 | Haiku agent | `half_x`/`half_y` computed in swapped order | 89.00% (and 61.00% with a broken sed edit, discarded) |
| 2026-10-08 | Haiku agent | `start.y = param_1->y` read before the x store | 75.60% |
| 2026-10-09 | Opus 5.5 | `param_1->x -= dbl; start = *param_1;` and `PlayfieldToMap(*param_1)` (struct copy) | 65.38 harness |

## Ideas not tried yet

- Use a local `struct Point *p = param_1;` to try to pin `param_1` into `eax` from the start.
- Compute `dbl` as `size + size` with a `(short)` cast on the store, to change the temp register.
