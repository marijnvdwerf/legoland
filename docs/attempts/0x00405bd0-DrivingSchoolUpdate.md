# DrivingSchoolUpdate (0x00405bd0, `src/legoland/driving_school.c`)

**Best: 84.09%** (verified 2026-10-17, plain C).

## What still differs

- The original reserves 0x20 bytes; the current build reserves 0x24. The volatile tile-y and
  rider-count temporaries introduce stack traffic absent from the original.
- The original keeps `5 * FUN_00401c40(...)` in EBX across `FUN_00413970`, then reloads the constant
  7 into EBX on each rider iteration. The current build spills the count, computes the product after
  the second call, and holds 7 in EBX throughout the loop.
- State 0's ride/tile coordinate loads use different registers and scheduling. State 3 already
  matches apart from displacements caused by the earlier differences.
- The count comparison now correctly uses signed `jge`, and the successful state-1 entry's
  load/increment/flag/store sequence matches.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | `register` next, `unsigned char move`, `unsigned int count`, rider/ride_x/ride_y/tile_y temps, flags via temp | 76.96 -> 80.18 |
| 2026-10-17 | Codex | Rebuild/current full-report baseline before editing | 81.82% |
| 2026-10-17 | Codex | Remove volatile from count/tile-y, try signed/unsigned count, and delay ride-y load | Off-tree 73.29–83.57%, baseline 83.57%; no improvement retained |
| 2026-10-17 | Codex | Compute the weighted count before the capacity call; direct coordinates, operand swaps, or delayed ride-y load | Off-tree 59.72–64.93%; fixes product placement but loses the constant-7 register and perturbs later registers |
| 2026-10-17 | Codex | Walking store order in states 0/3, explicit walking constant, reusing count for that constant, volatile next pointer, result types, count initialisation, flag/state temporaries | No improvement over the original off-tree baseline; variants are in ignored `build/dschool-*` directories |
| 2026-10-17 | Codex | Increment state before setting flag 0x80; explicitly cast the unsigned weighted count to int before comparison | **81.82 -> 84.09%**, verified in the full build; every other reported match unchanged |

The off-tree disassembler omits jump-table entries when scoring these trials; its percentages are
not directly comparable to the full reccmp percentages. The retained candidate scored 85.92% there.

## Ideas not tried yet

- Find a natural C form that lets EBX hold the weighted count in state 1 while still materialising
  7 once at each loop iteration. Removing volatile alone does not reproduce this allocation.
- Once that is solved, remove the remaining coordinate and rider-alias temporaries and check the
  state-0 instruction schedule again.
