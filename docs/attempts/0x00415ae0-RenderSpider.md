# RenderSpider (0x00415ae0, `src/legoland/spider_ride.c`)

**Best: 98.62%**. Kind: C.

File: `src/legoland/spider_ride.c`. Plain C.

Best: 98.62% (unchanged from the starting source).

## What still differs

Inside the `if (tile->id == elem->tile.id && (b = elem->rider)->flags & 0x80)` block, the original
schedules the loads differently:
- original: `ecx = DAT_0082c660.y`, `eax = 0`, `esi = b->person`, `adj.x = adj.y = 0`, then `eax = DAT_0082c660.x`,
  store `base.x` and `base.y`.
- ours: `eax = DAT_0082c660.x`, `ecx = DAT_0082c660.y`, `esi = b->person`, store `base.x`, then zero `adj`, then store `base.y`.

Every variant that changes the order (below) either leaves the same 98.62% or drops the score, because the change
also moves the branch displacements earlier in the function.

## Tried

| Date | Model | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku 5.5 (spider) | `Person *p;` declared at top, `p = b->person` moved after `base`/`adj` | 96.90% |
| 2026-10-07 | Haiku 5.5 (spider) | `Person *p = b->person` kept, `adj` zeroed before `base = DAT_0082c660` | 96.90% |
| 2026-10-07 | Haiku 5.5 (spider) | `p` declared at top, `base`, `adj` zero, then `p = b->person` | 98.62% (no change) |
| 2026-10-07 | Haiku 5.5 (spider) | No `Person *p` local; use `b->person->...` everywhere | 59.52% |
| 2026-10-07 | Haiku 5.5 (spider) | `base` copied field by field (`base.x = DAT.x; base.y = DAT.y`) | 98.62% (no change) |
| 2026-10-07 | Haiku 5.5 (spider) | Field-wise copy with `base.y` first, `base.x` after `adj` zero | 98.27% |

## Best

98.62% (the original source).
| 2026-10-09 | Opus 5.5 | `adj` zeroed before the `base` copy (fixes the store order); field_35 compare casts | 97.06 (worse than 98.62: ours then loads field_35 into al, jumps shift by 1) |

## Ideas not tried yet

- `Point adj = {0, 0};` brace initialisation at declaration instead of two assignments.
- Splitting the `if (b->field_35 == 1)` block so `adj` is zeroed inside the if/else.

## 2026-10-23 (Opus 5.5): still 98.62
`adj` zeroed before the `base` copy (any of: `Point adj = {0, 0}; Point base = DAT_0082c660;`, or plain statements
`p = ...; adj.x = 0; adj.y = 0; base = DAT_0082c660;`) now gives the original's exact load/store sequence for the
whole block (`ecx = DAT.y`, `xor eax`, `esi = person`, adj stores, `eax = DAT.x`, base stores). Only one difference is
left: ours splits the `field_35 == 1` test into `mov al,[edi+0x35]; cmp al,1` and pairs the load with the `base.y`
store; the original keeps `cmp byte ptr [edi+0x35], 1` after both stores. 97.06 (one instruction longer, so every
later jump shifts). No effect on the split: casts / `1 == x` / `!= 1` with else / local copy / goto label / byte
pointer reads / `volatile` read (96.37) / `volatile` base (93.45); 36 orders of p/adj/base statements; 12-minute
permuter on that version.
