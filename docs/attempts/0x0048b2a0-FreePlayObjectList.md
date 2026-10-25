# FreePlayObjectList (0x0048b2a0, `src/legoland/freeplay.c`)

**Best: 82.76%**. Kind: C.

## What still differs

- `b` (the 2nd argument) is hoisted into `ecx` at function entry in ours. The original keeps it in `[esp+0x14]` and reads it late, in the `else` branch (`mov edi, [esp+0x14]`). That hoist also takes `ecx` away from `sprite2`, which the original loads into `ecx` and ours into `edx`.
- The original's `b = icon->x` store goes into the parameter slot `[esp+0x38]` (b's own slot), which ours already does.
- Tail: the `e == 0x12c` branch and the `else` branch are laid out differently; the original's `else` is a separate `mov edi, [esp+0x14]` block.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku 5.5 (a6) | `icon->x` kept in a new local `int x` instead of reusing `b` | 74.12% (worse; adds a stack slot, the original reuses the `b` slot) |
| 2026-10-08 | Haiku 5.5 (a6) | `list = (struct PanelNode *)b;` moved before the `if` chain and the `else` dropped | 80.38% (worse) |
| 2026-10-08 | permuter + Opus 5.5 | `register` group, `unsigned int y` via icon_y temp, Up2 sprite before Down2, `icon->y + icon->height` | 81.50 -> 82.76 |

## Best

81.50%, the version currently in the file (unchanged from the start).

## Ideas not tried yet

- Make the `else` branch's `list = b` use a value the compiler cannot hoist, without adding a local.
- Try swapping the `sprite` / `sprite2` assignment order inside the branches to change which register takes `sprite2`.

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 81.50%. Matching has to come from C now.

## 2026-10-19 agent notes (81.50% -> 100.00%)

What the diff was: the C had `else list = (struct PanelNode *)b;` for an `e` that is none of the four pages, so
MSVC6 loaded b into ecx at entry for both uses. The original has no such branch: on that path `list` is simply
uninitialised and the original reads its home slot (which MSVC6 put in b's argument slot) late, at the test.
Dropping the else (list uninitialised when e is not 200/500/400/300, as in the original) gives the original's
code; a `switch` on e instead of the if/else chain is worse (87.23).
