# AddObjectToMap (0x0045dd80, `src/legoland/map_object.c`)

**Best: 99.10%**. Kind: C.

## What still differs

- At the y-loop join (after the inner x loop), the original reloads `rect.x0` into `eax` first
  (`mov eax, [esp+0x14]`), then `ebp` (`rect.x1`), then `edx` (y). Ours loads `ebp`, `edx`, then `eax`.
  That is the only difference left (two lines).

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku agent | `tile` set with a single ternary instead of if/else | 99.10% (same) |
| 2026-10-07 | Haiku agent | `struct MapElement *tile` moved from the function top into the inner x-loop block | 99.10% (same) |
| 2026-10-08 | permuter + Opus 5.5 | decl order (y before x; rect first), tx/ty swapped, x0/x1 locals per row, outer for(;;) as do/while(1) | 99.10 (no change) x5 |
| 2026-10-08 | permuter + Opus 5.5 | loop compares flipped (`rect.x1 >= x` / `rect.y1 >= y`); inner loop as guarded do/while | 97.30 / 97.30 / 47.83 (worse) |
| 2026-10-09 | Opus 5.5 | permuter 12 min (about 180-270 variants) | no gain |

## Ideas not tried yet

- Restructure the outer `for (;;)` / `rect = *rect.next` so the x0 reload is emitted in the join block.
- Use `rect.x0` directly in the y-loop body instead of the `x` induction variable's initial value.

## 2026-10-21 (Opus 5.5): still 99.10
| Change | Result |
|---|---|
| ty hoisted to the y loop; tx/ty swapped; `x + param_2.pos.x`; tx/ty at function scope; bounds inlined | 43.97-99.10 |
| bounds as a `static __inline` MapTileAt helper (if/return and ?: forms); `tile = 0` then if | 90.50-99.10 |
| `y = rect.y0; for (; ...)`; `if (rect.next) { rect = *rect.next; continue; } break;`; do/while(1); unsigned x | 98.20-99.10 |
