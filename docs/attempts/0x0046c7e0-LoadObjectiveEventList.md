# LoadObjectiveEventList (0x0046c7e0, `src/legoland/nerps.c`)

**Best: 98.29%** (starting version, unchanged). Kind: C.

## What still differs

- Tail after the last loop: the original emits `mov [ScriptLoadErrorCount], eax` after `pop edi; pop esi`
  (the store is sunk past the epilogue pops); ours stores before the pops.
- The `node->next == -1` branch: the original's `prev == NULL` exit is a separate copy (`je` forward to
  `xor ebx,ebx; ...; mov eax,ebx`); ours jumps backward into the shared `return NULL` tail.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku agent | Free branch as `if (prev == NULL) return NULL; prev->next = NULL; return head;` | 98.29% (same two diffs, no gain) |
| 2026-10-07 | Haiku agent | `ScriptLoadErrorCount++` -> `= ScriptLoadErrorCount + 1` in the tail | 98.29% (no change) |
| 2026-10-07 | Haiku agent | Free branch: `if (prev != NULL) prev->next = NULL; else head = NULL; return head;` (to match the original's `xor ebx,ebx`) | 89.16% (extra `push ebp`, frame changed; reverted) |
| 2026-10-09 | Opus 5.5 | `if (prev == NULL) return NULL;` before the unlink | no change (the original places a separate return-NULL block at the end; ours jumps back to an earlier one) |
| 2026-10-09 | Opus 5.5 | permuter 12 min (about 180-270 variants) | no gain |

## Ideas not tried yet

- Find a source form where the tail `return NULL` is not shared with the free-branch exit (the original keeps
  two epilogue copies).
- Try moving the counter increment before `FreeObjectiveEventList` (check that the callee never reads
  `ScriptLoadErrorCount`).

## 2026-10-23 Opus 5.5

The two diffs are one: the original sinks the `ScriptLoadErrorCount` store past `pop edi; pop esi`. That makes
the end tail differ from the free-branch exit, so `prev == NULL` jumps forward to the later return-NULL block
instead of back to the shared tail. These score 98.29 and do not move it: for(;;) with the read failure inside,
`+= 1`, `head = NULL; return head;`, `return 0`.
