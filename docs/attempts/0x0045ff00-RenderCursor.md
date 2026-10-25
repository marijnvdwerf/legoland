# RenderCursor (0x0045ff00, `src/legoland/map_object.c`)

**Best: 95.48%** (unchanged from start). Kind: C.

## What still differs

- Inside the x/y loop, the original loads `cursor->tile_x` into `edx` and `cursor->tile_y` into `eax`, then adds
  `x` / `y` into them (`add edx, esi`, `add eax, edi`). Ours loads both into `ebx`/`ecx` and copies `x`/`y` into
  `edx`/`eax` first (`mov edx, esi; mov eax, edi`). Those two extra 2-byte copies make ours 4 bytes longer, which
  shifts every later jump and the jump table entries.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku agent | `tilept.x = x + cursor->tile_x` (operand order) | 95.48% (same asm) |
| 2026-10-07 | Haiku agent | `tilept.y` assignment before `tilept.x` | 95.31% (worse) |
| 2026-10-07 | Haiku agent | `tilept.x = cursor->tile_x; tilept.x += x;` | 95.48% (same asm) |
| 2026-10-07 | Haiku agent | Compound `+=` for both x and y | 95.48% (same asm) |

## Best

95.48% with the original source (no edit kept).

## Ideas not tried yet

- Find what makes MSVC hoist the two `cursor` loads into callee-saved/caller-saved temporaries before the `x`/`y`
  copies; the `int x, y` declaration order or a `struct Point` temp for tilept may change it.
