# SaveGame (0x0047d8e0, `src/legoland/saveload.c`)

**Best: 91.55%**

## What still differs

(not analysed yet)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | permuter 93.3% came from `FindeIneList(&temp)` instead of `&BlokeSaveBuffer.favourite[1]` (result written to the temp - behaviour change) | rejected |

## Ideas not tried yet

- 

## 2026-10-24 agent notes (95.92% -> 98.29%)


## 2026-10-24 Opus 5.5: harness 95.82 -> 98.24
All 48 remaining diffs were stack slots. `len` (element/TSF name length) declared at function scope instead of in the
element block moves it out of the slot it shared with y/n and puts flags/num/len where the original has them.
Left: the ride loop's `count` and block 12's `n` share elem's slot in ours, the num/flags slot in the original.
No change from: every declaration order in the ride and bloke blocks, hoisting or merging any other address-taken
int (pairwise and greedy over len/flags/n/num/count/num/n), renaming them onto tab_count/x/k/n_elems.
