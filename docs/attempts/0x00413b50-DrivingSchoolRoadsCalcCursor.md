# DrivingSchoolRoadsCalcCursor (0x00413b50, `src/legoland/roads.c`)

**Best: 94.67%** (2026-10-18, plain C; starting full-build score 84.21%).

## What still differs

- The default ID path at original 0x00413c9d reloads the saved entry pointer's low word with
  `mov si, word ptr [esp + 0x44]`. Ours copies the entry register with `mov esi, edi`.
  This is three bytes shorter, shifting the surrounding branches and trailing padding.
- The remaining instruction sequence matches. The object scorer also shows a false difference
  on an address referring to `EditCursor.next`; the linked full comparison normalises that address.
- The fallback's meaning is unknown; the existing pointer-to-low-word conversion is unchanged.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-18 | Codex | Remove volatile from the footprint pointer and final y-coordinate temporary; use direct memcpy or a footprint struct assignment | Object scorer 82.14–84.00%, versus its 84.77% baseline; removal alone changes register allocation |
| 2026-10-18 | Codex | Initialise the neighbor mask before the footprint copy, at declaration, or before capturing the ride; remove both volatile qualifiers | **84.21 -> 94.67%** in full build; restores the original mask/entry registers and most of the instruction sequence |
| 2026-10-18 | Codex | Unsigned mask; initialise mask after ScreenToMapRef; initialise entry pointer to NULL; widen the selected type | No improvement over the retained candidate; moving mask initialisation late restores the register mismatch |
| 2026-10-18 | Codex | Snapshot fallback ID as unsigned int/unsigned short/TileId, before validation or CheckForPeople | Same 94.22% object score as the best candidate; compiler still uses the entry register for the fallback |
| 2026-10-18 | Codex | Snapshot entry as pointer/const pointer/UINT_PTR, optionally using it for the final coordinates | Same 94.22% object score |
| 2026-10-18 | Codex | Initialise fallback type before neighbor selection; signed-short type with unsigned-short comparison casts | 90.83% / 94.22% object scores; no improvement |
| 2026-10-18 | Codex | Remove ptmp236/237/553/554, use typed footprint assignment and direct coordinate reads; name entry and neighbor-mask locals | Full score remains **94.67%**; retained |

Object scores are not directly comparable with full scores because function boundaries, padding
and address normalisation differ. The retained candidate scores 94.22% in the object harness.

## Ideas not tried yet

- A natural source form that reloads the low word from the existing saved entry slot, without
  adding another store, stack slot or volatile memory access.
