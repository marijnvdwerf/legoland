# LogFlumeTrackRemoveObject (0x0040c8d0, `src/legoland/log_flume.c`)

**Best: 92.59%** (unchanged from start). Kind: C.

## What still differs

- One hunk, 10 instructions. The original pushes the `cursor` argument (`mov ecx,[esp+0x14]; push ecx`) between the two
  footprint decrements, then re-reads `tile` and `elem` from the stack slots (`[esp+0x14]`, `[esp+0x10]`) after the second
  decrement. Ours loads all three arguments after both decrements and pushes them together. The original's `mov`/`push` order is
  real code scheduling, not an address difference: the operands are the same stack offsets in both versions.
- The rest of the function (the `rep movsd` memcpy, the decrement pair, the `FindFlumeSubEntryByTile` / `FUN_00409440`
  tail) is byte-identical. The diff is not layout-dependent; no call target or data address differs in the hunk.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku 5.5 (flumeb) | Swapped the order of the two footprint decrements (`y1` before `x1`) | 83.33% (worse; the current order is the better one) |
| 2026-10-07 | Haiku 5.5 (flumeb) | `FindFlumeSubEntryByTile(&t)` on a local copy of `tile` instead of `&tile` | 62.39% (worse) |
| 2026-10-09 | Opus 5.5 | decrements as comma expression inside the `tile` argument; `x--`, `-= 1`, `x = x - 1`; struct assignment instead of memcpy | 92.59 (no change; the early `cursor` push never appears) |

## Ideas not tried yet

- Look for a statement shape that makes MSVC keep the cursor load next to the first decrement (e.g. separating the two
  decrements with the `cursor` use), without changing the call's argument list.

