# BuildObject (0x0045eb30, `src/legoland/map_object.c`)

**Best: 96.81%**. Kind: C.

## What still differs

- Stack slot for `packed` (TileId): the original stores it into the dead `editObj` argument slot
  (`[esp+0x20]` after the pushes, i.e. `E+4`), ours into the `coords` slot (`E+8`). The prologue is
  `sub esp, 8` in both. Everything from the `if` branch onwards is shifted by one slot.
- Argument shuffle in the 0x80000 branch before `FindMapPathAStar` (`mov ecx, [esp+0x18]` ordering).

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku agent | `out` and `effect` moved into each branch's block scope | 95.21% (worse) |
| 2026-10-07 | Haiku agent | `cost` local removed, `GetObjCost(obj)` called inline in the first `if` | 96.81% (same) |
| 2026-10-07 | Haiku agent | `obj = editObj->obj;` moved before the `packed` byte stores | 82.98% (worse) |
| 2026-10-09 | Opus 5.5 | analysis | packed (TileId) lives in a dead parameter slot in both builds: the original reuses editObj's slot (entry+4), ours coords' (entry+8) |

## Ideas not tried yet

- Declare `packed` as a plain `unsigned short` (or `TileId` built via a temp) so its slot choice changes.
- Swap the `coords[0]`/`coords[1]` byte loads to a single struct copy from `coords`.
- Change the `FindMapPathAStar` argument expressions so `out.x`/`out.y` are read through `effect` first.

## 2026-10-21 (Opus 5.5): still 96.81
The original stores `packed` in the dead `editObj` parameter slot ([esp+0x20] after 5 pushes); ours uses
the `coords` slot, so every other slot moves by 4.
| Change | Result |
|---|---|
| all 120 declaration orders | 96.81 |
| obj before packed; y before x; no casts; packed built late / block-scoped in the 0x80000 branch | 73.89-96.81 |
