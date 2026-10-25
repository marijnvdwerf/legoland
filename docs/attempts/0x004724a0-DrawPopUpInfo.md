# DrawPopUpInfo (0x004724a0, `src/legoland/popupinfo.c`)

**Best: 89.18%**

## What still differs

(not analysed yet)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | `unsigned int entry_index`, icon_y before icon_x, constant-first compares / masks, `mid = mid - 0x20` | 84.39 -> 89.18 |

## Ideas not tried yet

- 

## 2026-10-24 agent notes (94.43% -> 96.20%)


## 2026-10-24 Opus 5.5: harness 94.33 -> 96.10
The text-row y (`ty`) and the sprite-row y (`sprite_y`) are one variable (`row_y`) in the original: merging them fixes
most of the stack-slot diffs. Found with a pairwise same-type local merge sweep (211 pairs, then 191 from the result:
no second gain).
