# BoatingSchoolUpdate (0x0041a720, `src/legoland/boating_school.c`)

**Best: 96.58%**. Kind: C.

Current checkout verified at **94.41% on 2026-10-18**; the historical best above
is not the current baseline.

## What still differs

- Register choice after the bloke-count check in `case 0`: the original increments `bloke_count` through `edx` and loads `BoatingSchoolRide` into `edx` for the removal call; ours uses `ecx` and `eax`. The `eax` load is a 5-byte short form, so the function is 1 byte shorter and every later jump displacement shows as a diff.
- Call tail: the original jumps into the shared `RemoveBlokeFromRide` call at `0x41ab35` with `push ebp; push edx`.
- `source2` stack store: the original stores `source2.type = 1` before `param_action++` (before the `[esi+0x60]` byte update); ours schedules it after the `push`.
- Store order in `case 0`: the original writes `field_73` before `low_level_action` (ours matches that order, but the compiler still emits the word store first).

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 p4 | Haiku agent | Swapped declaration order of `source` and `source2` | 94.41% (no change) |
| 2026-10-07 p4 | Haiku agent | Moved `source2.type`/`.bloke` stores above `bloke->param_action++` in case 5 | 94.13% (worse) |
| 2026-10-07 p4 | Haiku agent | Swapped `field_73` and `low_level_action` stores in case 0 | 94.41% (no change) |
| 2026-10-08 | permuter + Opus 5.5 | `unsigned int slot`; case 1 condition split into two nested ifs | 94.41 -> 96.58 |
| 2026-10-09 | Opus 5.5 | all 6 orders of `param_action++` / `source2.type = 1` / `source2.bloke = bloke` | 95.53-96.09 (current order best; type-first puts the store before NewDirForAction's cleanup) |
| 2026-10-18 | Codex | Recheck case-1 nesting against the current checkout: four nested conditions, early-break guards (all, outer pair, or inner pair), reversed outer tests, split inner tests; signed/unsigned slot | Standalone baseline 96.09%; 12 variants 95.25-96.09%, no gain. Source unchanged; full build remains 94.41% |

## Ideas not tried yet

- Restructure the `score->blokes[i]` search so `slot` and the count increment get different temporaries.
- Write the case-5 `source2` setup as a helper-free sequence of `&source2` fields to change the scheduling.
