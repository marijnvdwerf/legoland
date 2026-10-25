# SearchJunglePathConnected (0x00437260, `src/legoland/jungle_cruise.c`)

**Best: 38.41%**. Kind: C.

## What still differs

- The original turns the last recursive call (the `x - 5` branch) into a jump back to the top; MSVC already does that from the plain recursive source.
- The gap is register allocation: which of `x`/`y`/`tx`/`ty` land in `esi`/`edi`/`ebp`/`ebx`.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 r2 | Haiku agent | Rewrote as `for (;;)` with the tail call as `continue` (two variants) | 19.86% (much worse) |

## Ideas not tried yet

- None recorded.

## 2026-10-21 (Opus 5.5): still 38.41
The original keeps x/y/tx/ty in esi/edi/ebp/ebx and found/owner in memory (node lives in tx's slot); ours keeps
found in ebx and CSEs x+5 / y-5 across the calls. SearchBoatPathConnected (0x0041c940) has byte-identical
original code, so one fix solves both.
| Change | Result |
|---|---|
| nested ifs per direction; `next` per block; `!next->visited`; `*found != 0`; one combined early-return if; whole body inside `if` | 40.30 harness |
| no `next` (Find twice) | 20.00 |
| found check after the first Find | 46.84 harness, but the original checks found first (wrong shape) |
| `5 + x`, `y + -5`, casts on the call args only / Find args only (to break the CSE) | 40.30 |
| permuter 20 min (56 compiled) | no gain |
