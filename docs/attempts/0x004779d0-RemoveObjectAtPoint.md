# RemoveObjectAtPoint (0x004779d0, `src/legoland/gamemain.c`)

**Current: verify 85.97%, harness 86.49%.** Kind: C (with permuter leftovers: `register`, `ptmp*` locals).

## What still differs

- The original compares `p->x`/`p->y` signed (`jge`); ours casts to unsigned (`jae`), a permuter artefact.
- The original loads the tile flags into ax and so loses `p->x`. It reloads `[edi]` for
  `GetGardenerWorkOrderAt(p->x, p->y)`. Ours keeps `p->x` live in eax from the bounds test and puts the flags in cx.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-23 | Opus 5.5 | clean version: no ptmp/register, signed bounds test, direct `order->element` arguments | harness 76.28: struct 99.10, but the flags/x register choice spreads through the function |
| 2026-10-23 | Opus 5.5 | + `flags` local (ushort/int/uint), `GameMap[y] + x`, `(int)` casts on the bounds | 76.05-76.28 |

## Ideas not tried yet

- A form where the bounds test does not leave `p->x` reusable (the original reloads it), then remove the ptmp
  leftovers.
