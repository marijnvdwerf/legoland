# InsertChildIntoList (0x00475630, `src/legoland/interface.c`)

**Best: 89.55%**

## What still differs

- `push ebp` is at the prologue in the original; ours pushes ebp only in the found path (ebp = prev).
- The original loads `param_1->parent_element` after the `current != NULL` guard; ours before it.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-09 | Opus 5.5 | `parent = param_1->parent_element` local before the loop: node/data registers now edi/ebx like the original | 77.61 -> **89.55** |
| 2026-10-09 | Opus 5.5 | `GetObjCost(current->data) >= GetObjCost(node->data)` | 74.63 |
| 2026-10-09 | Opus 5.5 | `obj = node->data` local in the inner loop | 64.66 |
| 2026-10-09 | Opus 5.5 | `if (current) { parent = ...; do {...} while (current); }` | 54.01 |
| 2026-10-18 | Codex | Separate parent search from insertion: early return on missing parent, named `parent_found`/`parent_missing` paths, separate if/else, or shared cleanup label; parent cached before search, after a null guard, or read directly | 13 standalone trials including baseline: 75.36-90.37%, baseline 90.37%; no improvement. Current full-build score remains 89.55%, source unchanged |

## Ideas not tried yet

- 
