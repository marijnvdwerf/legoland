# MarkConnectedNodes (0x004829c0, `src/legoland/pathfind.c`)

**Best: 72.55%**

## What still differs

- The original pushes all four callee-saved registers at entry and uses `edi` for the `field_20 |= 2` temp; ours
  uses `esi` and only pushes/pops `edi` around the inline memcpy.
- The original loads `AdjacentNodeCount` into `eax` and copies it to `ebp` (loop counter) before the `je`; ours loads
  straight into `ebp`.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-09 | Opus 5.5 | separate `n` (malloc/memcpy size) and `count` (loop), count assigned before the if / inside the if / in the for header | 72.55 (no change, copy-propagated) |

## Ideas not tried yet

- the permuter

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 72.55%. Matching has to come from C now.

## 2026-10-19 agent notes (72.55% -> 74.51%)

Indexed loop (`for (i = 0; i < count; i++) ... copy[i]`) instead of the pointer walk: 72.55 -> 74.51 (harness).
Still different: the original pushes edi at entry and does `node->field_20 |= 2` through edi; ours uses esi and
pushes edi only around the inline memcpy. No change from: `count` tested via the global, a `size` local, an early
`return` when count is 0, `field_20 = field_20 | 2`, `== 0` vs `!` tests (15 combinations, 72.55-74.51).
