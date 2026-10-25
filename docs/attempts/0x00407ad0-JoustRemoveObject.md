# JoustRemoveObject (0x00407ad0, `src/legoland/joust.c`)

**Best: 100% (matched 2026-10-09)**. Kind: C.

## What still differs

- The original does `xor ecx/edx`, loads node->id.pos.x and .y into ecx/edx, then `lea eax,[esp+4]` and the two pushes
  (-200, &source), and only then stores `source.kind = 2`, `source.x` and `source.y`. Ours stores kind (and x) before the
  `lea`/pushes, so the store block sits in a different place in the stream.
- The original never writes the `pad` field (esp+8 of the struct stays untouched).

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku agent | Original (current best): x, kind, y assigned in that order | 95.00% |
| 2026-10-07 | Haiku agent | Load x and y into `unsigned int` locals first, then assign kind, x, y | 62.50% (worse, extra eax use) |
| 2026-10-07 | Haiku agent | Assign x, y, then kind | 92.50% (worse) |
| 2026-10-07 | Haiku agent | `struct {...} source = {2, 0, node->id.pos.x, node->id.pos.y};` inside the `if` | 88.64% (loads and pushes match, but the pad field gets a `mov [esp+0x10], 0` the original lacks) |
| 2026-10-09 | Opus 5.5 | block-scope initialiser `{2, x, y}` with the pad as an unnamed `unsigned int : 32;` bit-field (C leaves unnamed members uninitialised, so no store) | **100** |

## Ideas not tried yet

- Find a way to initialize the struct without writing `pad` (the initializer always zeroes the unnamed middle member).
- A helper that copies x/y after the call arguments are pushed, if MSVC will sink stores past pushes that way.
