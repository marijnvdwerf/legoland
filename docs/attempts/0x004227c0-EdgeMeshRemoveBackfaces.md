# EdgeMeshRemoveBackfaces (0x004227c0, `src/legoland/castle.c`)

**Best: 95.06%** (unchanged). Kind: C.

## Tried

| Date | Model | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku 5.5 | `z = dx1 * dy2 - dy1 * dx2` (operands of the products swapped) | 95.06% (no change) |
| 2026-10-07 | Haiku 5.5 | `dy1 = ...` statement before `dx1 = ...` | 95.06% (no change) |

## What still differs

- Register choice for the triangle vertices: the original keeps p0.x in edi and p0.y in ecx, ours uses ecx/edi
  the other way round, so the p2 loads and the subtraction order differ (0x422938 to 0x422963).
- Original keeps dx1 and dy1 on the FPU stack (`fild` both before the p2 loads); ours reloads and uses `fmulp`
  where the original uses `fmul st(3)` and `fxch`.
- The tail (free calls and the return path) differs: the original reuses `pop edi; pop esi` before the
  `free` calls, ours does `mov eax, ebx; pop ebp; pop ebx; add esp, 0x38; ret`.

## Ideas not tried yet

- Declare `p0`/`p2` as `struct MVert *` locals in a different order, or compute the p2 pointer before
  p1, to change which registers hold the base addresses.
- Use `unsigned int` for `v` indices, or build the `int` deltas with explicit casts, to change the
  integer sub/fild pattern.
