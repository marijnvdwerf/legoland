# IsFootprintClear (0x00457970, `src/legoland/bricks.c`)

**Best: 87.80%**. Kind: C.

## What still differs

- The original has an extra 4-byte frame slot (`push ecx` in the prologue) that holds the `unsigned int idx` passed to
  `LLIDB_FindElement` (`lea ecx,[esp+0x10]`). Ours has no frame slot: MSVC puts `idx` into the dead `dx` parameter slot
  (`lea ecx,[esp+0x14]`).
- Loop bookkeeping order: the original keeps the inner loop count in `ebx`/`edi` differently (`lea eax,[ecx+ebx]` reloads
  `FootprintWidth`), so the register picks differ after the first inner loop.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku agent (a1) | Start: `unsigned int idx` declared inside the `if (elem->field_12)` block | 87.80% |
| 2026-10-08 | Haiku agent (a1) | Hoist `unsigned int idx;` to function scope (uninitialised) | 87.80% (no change) |
| 2026-10-08 | Haiku agent (a1) | `unsigned int idx = 0;` at function scope | 65.14% (adds a zero store and a different frame layout) |
| 2026-10-08 | Haiku agent (a1) | Hoisted `idx` plus `int x0 = dx + ...; int y0 = dy + ...;` locals (params not modified) | 58.90% (worse) |
| 2026-10-08 | Haiku agent (a1) | `struct Element *idx` with a `(unsigned int *)&idx` cast for the call | 87.80% (no change) |
| 2026-10-09 | Opus 5.5 | new locals `x0`/`y0` instead of `dx +=`/`dy +=` (both, x only, y only) | 58.54-85.37 (the original writes dy back into its own slot and gives idx a fresh slot; ours still puts idx in the dead dx slot) |

## Ideas not tried yet

- Find a source form that makes MSVC keep a fresh frame slot instead of reusing the `dx` parameter slot (e.g. take the
  address of a value that is still live in the original, or compute `dx`/`dy` into the loop bounds differently).

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 87.80%. Matching has to come from C now.

## 2026-10-19 Opus 5.5: 87.80% -> 100% in plain C

The offset is one `struct Point` passed by value: `int IsFootprintClear(struct Point off)`, and the only caller
passes its `pt` (it was `IsFootprintClear(pt.x, pt.y)`). MSVC6 never packs a local into a struct argument's slot,
so `idx` gets the fresh frame slot (`push ecx`) the original has, and everything else falls into place.
Lesson: when ours puts a local into a dead parameter slot and the original doesn't, check whether the
parameters are really a struct passed by value (look at the callers).
