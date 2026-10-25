# PrintProfileDetails (0x0048cf10, `src/legoland/profile.c`)

**Best: 61.11%**

## What still differs

(not analysed yet)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | slot / icon_y / last_x temps, `(int)icon->slot` compare | 60.26 -> 61.11 |
| 2026-10-09 | Opus 5.5 | all 720 declaration orders; no `name` local; `name` assigned inside the if | 96.97 (last/name slots stay swapped) |

## Ideas not tried yet

- 

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 96.97%. Matching has to come from C now.

## 2026-10-21 (Opus 5.5): still 96.97
`last` and `name` have swapped frame slots ([esp+0x18] / [esp+0x14]).
| Change | Result |
|---|---|
| all 720 declaration orders of the 6 locals | 96.97 |
| name / sel / show (and combinations) block-scoped in the icon loop | 96.97 |
