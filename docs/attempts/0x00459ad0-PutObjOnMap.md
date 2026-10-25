# PutObjOnMap (0x00459ad0, `src/legoland/gamemap.c`)

**Current: verify 75.66%, harness 75.29%.** Kind: C.

## What still differs

- The ENTRANCE 1 block: the original holds the entrance ride in edx, `pos->x` in eax, lpConfig in ecx and the
  cell in ecx. Ours rotates the three (entrance in eax, `pos->x` in ecx, lpConfig/cell in edx).
- `ParkExitPos.y`: the original keeps the two terms apart, `((v3 - v1) >> 1) << 8` (`shl 7; and dl, 0`) plus
  `(byte_5 + v1) << 8`. Our `* 0x100` form lets MSVC factor them into one `(a + b) << 8`.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-23 | Opus 5.5 | y as `(((unsigned)(v3 - v1) >> 1) << 8) + ((byte_5 + v1) << 8)`, also signed, as `<< 7 & ~0xff`, operands swapped | harness 74.71: the y block matches, but one extra byte of padding shifts the score |
| 2026-10-23 | Opus 5.5 | x/y statement order, `/ 2`, ternary or negated bounds test, cell lookup before the entrance load, `x`/`y` locals, x expression forms, all six declaration orders | 53.75-75.29 |

## Ideas not tried yet

- Find what pins `pos->x` to eax in the original (the register rotation is the main remaining cost).
