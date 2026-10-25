# SaveScripts (0x0046c920, `src/legoland/nerps.c`)

**Best: 89.23%** (unchanged from start, the original nested early-return form). Kind: C.

## What still differs

- The original has one shared `xor eax, eax / pop esi / add esp, 8 / ret` block near the top (reached by `je` backward from the string and node loops). Ours emits the `return 0` epilogue at the end of the function, so every loop-failure branch is a forward jump.
- The first early return has an explicit `xor eax, eax`; the later ones (after `test eax, eax`) do not. Ours emits `xor` in the final check and not in the first.
- Final tail (`CurrentScriptSection` block and `neg/sbb` return) is identical in shape; only the branch layout differs.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Claude Haiku 5.5 | Single `for (;;)` with `break` on every failure, shared `return 0` after the loop | 51.59% (worse) |
| 2026-10-08 | Claude Haiku 5.5 | Nested `if (x != 0)` chain, success path inside, `return 0` at the end | 75.48% (worse) |
| 2026-10-08 | Claude Haiku 5.5 | Keep early returns, string and node loops end in `break` plus `if (i < count) return 0;` / `if (node != NULL) return 0;` | 71.35% (worse) |
| 2026-10-08 | Claude Haiku 5.5 | Early returns kept, last check as `if (SaveGameWrite(&i,4) != 0) {...} return 0;` | 73.75% (worse) |
| 2026-10-08 | Claude Haiku 5.5 | Early returns kept, loop failures as `return 0` (same as start) | 89.23% (same) |

## Ideas not tried yet

- Find a source form that makes MSVC place the shared epilogue at the top: e.g. the first check as `if (SaveObjectiveIcons() != 0) {...}` with the rest of the work inside it and only the loop's failure as `return 0`.
