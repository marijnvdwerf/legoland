# LegoShop2Update (0x00439950, `src/legoland/shops.c`)

**Current: verify 76.66%** (correct behaviour; 75.11% with the old bug, 46.68% right after the fix). Kind: C.

## Behaviour bug (fixed 2026-10-23)

Case 3 (pick a browse spot), `r == 2`: the original sets `dest = ((x - 3) << 8, y << 8)` (0x4399fd: `add ebp,-3;
shl ebp,8; mov [esi+0x24],ebp; shl edi,8; jmp 0x439b07`, which joins after case 1's `sub edi,0x80`). The C before the fix
wrote `(y << 8) - 0x80`. Case 5 `r == 2` (the same spot) already uses `y << 8`. The fixed C scores harness
46.70-48.86: with one less use of `y`, MSVC swaps the x/y registers (ebp/edi), and that shift spreads through every
case. Landed 2026-10-23 at verify 46.68% (correct behaviour takes priority). Next step: find a form that keeps x in
ebp and y in edi.

## Original structure (for the next attempt)

- Header order: `next`, `tile = &node->tile`, `bloke`, `x = tile->pos.x + ride->x` (ebp), `y = ... + ride->y` (edi).
- The CalcMoveLine call setups are cross-jumped into three shared tails: B1 0x439a39 (case 0, case 3 default),
  B2 0x439b07 (case 1/9, case 3 r==2) and B3 0x439b31 (case 2, case 3 r==1, case 10). Case 11 has its own copy
  and falls into case 12.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-23 | Opus 5.5 | fix r==2, header in the original's order, 120 random orders of dest.x store vs y shift per case | 46.70-48.86 |
| 2026-10-23 | Opus 5.5 | fixed C with direct `dest = expr` stores (no x/y reassignment), int/uint, both x/y declaration orders, 226-order decl search | 46.70-48.86 |

## 2026-10-23 Opus 5.5: correct C back above the old score (46.68% -> 76.66%)

The x/y register swap after the behaviour fix came from the number of CalcMoveLine call copies MSVC sees
before cross-jumping, not from statement forms. 830 random dest/x/y statement orders, name changes, and helper
or `?:` forms all tie (the IL normalises them). What moves it is case 3's structure:

| case 3 shape | harness | verify |
|---|---|---|
| one shared walk after the if/else chain (the fix as first landed) | 48.86 | 46.68 |
| each of the three spots with its own walk + `break` | 75.97 | 71.95 |
| spot r==2 with its own walk + `break`, r==1/else share the walk after the chain (kept) | 81.01 | **76.66** |
| as a nested `switch (rand() % 3)` | 67.26 | - |
| case 1 and case 9 as two copies | 60.27 | - |

What still differs: the original keeps the LAST copy of each cross-jumped call setup and ours the first. The
original's groups are B1 {case 0, 3c (kept)}, B2 {3a, case 1/9 (kept)} and B3 {case 2, 3b, case 10 (kept)}. In
ours, case 2 takes the B2 register rotation (edx/ecx/eax) instead of B3 (ecx/eax/edx), and case 0 keeps its
own setup. These had no effect: per-copy `NewDirForAction` argument forms (298 combinations), an `__inline`
walk helper, case 2 written in other algebraic forms, flag/field_58/case-label orders. A 10-minute permuter run
on the 3-copy form found nothing.
