# BalloonzUpdate (0x0042aa90, `src/legoland/balloonz.c`)

**Best: 94.65%**. Kind: C.

## What still differs

- Prologue register assignment: the original loads `obj` into `eax` and the `"Bloke??"` buffer halves into `ecx`/`edx`; ours uses `edx` for `obj` and `eax`/`ecx` for the buffer.
- Loop variable `elem` lives in `eax` in the original (reloaded from `[esp+0x24]`); ours keeps it in `ebp` and reuses `ebp` for `tile`.
- Write-back of the balloon state: the original stores `riders`, `queued`, `lap`, `cars`, `can_board`, the `cars` tail word, `frame`, `leaving`, `can_unload` in that order; ours differs and the register for `leaving`/`lap` byte stores is swapped.
- Stack slots of char locals (`[esp+0x12]`, `[esp+0x14]`, `[esp+0x1c]`) are assigned differently; declaration order does not change them.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 p4 | Haiku agent | Permuted the local declarations (reversed, `dir` first, `buf` first, chars first, `queued` last) | 94.20% (no change) |
| 2026-10-07 p4 | Haiku agent | `for (elem = ride->riders; ...; elem = next)` instead of `while` | 94.20% (no change) |
| 2026-10-07 p4 | Haiku agent | Write-back stores reordered to the original's order | 92.98% (worse) |
| 2026-10-08 | permuter + Opus 5.5 | reordered next/pos/leaving/riders stores, `unsigned int can_unload`, `char dir`, nested `if (riders) { if (!stop)`, constant-first operands | 94.20 -> 94.65 |
| 2026-10-09 | Opus 5.5 | state write-back in the original store order (riders, queued, lap, cars, can_board, frame, leaving, can_unload); riders first only; can_board earlier only | 95.06 / 95.84 / 96.00 harness (all below the current 96.94) |

## Ideas not tried yet

- Load `obj->ride` through a separate `ride` temporary after the buffer init, to push `obj` into `eax`.
- Split the `FindBalloonNode` lookup so `elem` stays live across the `state` block (to keep `elem` in `eax`).
