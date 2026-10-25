# CoptersPlaceRider (0x00404630, `src/legoland/copters.c`)

**Best: 54.39%** (`volatile int index`, `index` named in the `fistp` asm block). Kind: C + __asm.

## What still differs

- Hand-written asm in the original (the `fld/fmul/fistp` macro) inside compiled C, and a `switch` with a jump table, so it cannot be transcribed as naked asm.
- What is left is a register permutation: the original keeps `entry`/`layer`/`s` in `edi`/`ebx`/`esi` and spills `entry` into `node`'s parameter slot `[ebp+8]`; ours uses `esi`/`edi`/`ebx`.
- `index` doubles as the y offset (the default case keeps the original index) and as the asm's float/int scratch.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Claude | First C version with a separate `yoff` | 29% (frame 0x2c, original 0x28) |
| 2026-10-07 | Claude | `index = 0xeb` etc. instead of `yoff` | no frame change |
| 2026-10-07 | Claude | Drop the `frame` pointer local (repeat the expression) | no frame change |
| 2026-10-07 | Claude | `entry` computed before the GetScreenCoordsForObject call; body inside `if (rider != NULL) {}` | structure closer |
| 2026-10-07 | Claude | `fld dword ptr index` / `fistp index` in the asm (index stays in memory) | 36.7%, frame now 0x28 (kept) |
| 2026-10-07 | Claude | `volatile int index` (reloaded at every use, as in the original) | 54.39% (kept) |
| 2026-10-07 | Claude | Reassign `node` to the entry (`node = (struct CopterNode *)&node->layer[index]`) | 44.77% (worse) |
| 2026-10-11 | Opus 5.5 | `volatile struct Point p1` (found by a per-local volatile search); natural forms tried first: all 24 declaration orders of p2/p3/p1/base, `base.x + p1.x + p2.x` order | 61.45 -> 81.55 with volatile (kept, last resort); natural forms all 61.45 |

## Ideas not tried yet

- Get `entry` spilled to `[ebp+8]` without reusing `node` (maybe an inner block, or a second pointer local).

## 2026-10-19 agent notes (81.61% -> 82.76%)

The jump table showed the case bodies were attached to the wrong `index` values: the original sends index 0 to
layer 3 (y offset 0xd7), 1 to layer 0 (0xeb), 2 to layer 4 (0xe1), 3 to layer 1 (0xe6) and 4 to layer 2 (0xe6),
and its case bodies are laid out in layer order, so the source listed them as `case 1, case 3, case 4, case 0,
case 2`. Fixed (this was also a behaviour bug: riders were placed on the wrong layer).

Still different: the p1 store after GetRenderOffsetForLayer (volatile kept), the operand order of two of the
six products in the cross product `mat[2] = mat[1] x mat[0]` (MSVC6 picks the order itself: swapping the
source operands, writing it with row pointers, without the `frame` local or as `m = ...mat` changes nothing or
makes it worse), and eax/ecx swapped for the j offset in the fixed-point loop.

| Change | Harness |
|---|---|
| operands of every product swapped / cross written as b x a | 81.55 (no change) |
| row pointers a, b, c | 80.36 |
| `float (*m)[3]` local | 73.81 |
| no `frame` local (full expression each time) | 53.49 |
