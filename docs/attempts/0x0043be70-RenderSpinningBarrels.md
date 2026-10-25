# RenderSpinningBarrels (0x0043be70, `src/legoland/spinning_barrels.c`)

**Best: 99.20%** (starting version, unchanged). Kind: C.

## What still differs

- The `seat = DAT_0062fdd8` copy (rider loop, before the `person->offset` stores). The original loads
  `DAT_0062fdd8+4` into `eax` before `person` is loaded, and loads `DAT_0062fdd8` (x) only after
  `bloke->screen_x` is read. Ours loads both at the copy site, which costs 3 bytes and shifts later jumps.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku agent (barrels) | Seat copy moved after `person->offset.x` store | 95.31% (worse) |
| 2026-10-07 | Haiku agent (barrels) | Seat copy moved to before `bloke = riders->rider` | 92.00% (worse) |
| 2026-10-07 | Haiku agent (barrels) | Seat copy split into `seat.x = DAT.x; seat.y = DAT.y;` at original spot | 99.20% (no change) |
| 2026-10-07 | Haiku agent (barrels) | Seat copy moved after both `person->offset` stores | 95.05% (worse) |
| 2026-10-08 | permuter + Opus 5.5 | seat.y then seat.x at the copy site | 98.93 (worse) |
| 2026-10-08 | permuter + Opus 5.5 | seat copy between bloke and person; seat.y / person / seat.x split; seat.y early + seat.x after offset.x | 99.20 / 98.93 / 95.85 |
| 2026-10-08 | permuter + Opus 5.5 | `*&DAT`, memcpy, `*(__int64 *)` copy, `(&DAT)->x` fields - all identical code; person->offset through a struct temp | 99.20 x4 / 67.74 |
| 2026-10-09 | Opus 5.5 | seat copy as field stores (y,x / x,y) and at each position in the rider block; permuter 12 min (190 variants) | 99.20 (no gain) |

## Ideas not tried yet

- Reading `DAT_0062fdd8` through a pointer or temporary `Point *` instead of a struct copy.
- Changing `AdjustOffsetForViewMode(&seat)` to operate on a `DAT_0062fdd8` copy directly (not tried).

## 2026-10-23 (Opus 5.5): still 99.20
The original stores `seat.y` before `seat.x`. Field stores `seat.y = DAT.y; seat.x = DAT.x;` right after
`person = bloke->person` give that store order (98.93), but both loads stay at the copy; the original loads
`DAT.x` only after `bloke->screen_x`. All 40 orders of {person, seat.y, seat.x, offset.x, offset.y}: 95.17-99.20.
`seat = off` / `off2` reuse 93.30; `static __inline` copy helpers 98.93-99.20.
