# RemObjFromMap (0x00459c90, `src/legoland/gamemap.c`)

**Current: verify 87.05%** (was 85.65%). Kind: C.

## 2026-10-23 Opus 5.5: 85.65 -> 87.05

- Sound block: `blk.f8`, `blk.f0 = 2`, then `blk.fc` (the original stores f0 before fc).
- Path loop: the first statement goes through a `unsigned short *flags` pointer to the cell's flag word: the
  original's dead `lea eax,[cell+0xc]` is that pointer.
- Bounds read through `query->footprint` (signed ints) instead of `(int)` casts on `field_1414`: same code.

## What still differs

The loop bounds. The original loads every operand into a register and shares `tile_y`/`tile_x` between the
start and end expressions (`mov eax,[1408]; mov ecx,[1418]; mov edi,[1420]; add ecx,eax; add eax,edi`), so
in its IL the store to `blk.f4` comes after both bounds. Ours stores `blk.f4` first and then reloads `tile_y`
for the end (`mov edx,[1420]; add edx,[1408]`). The bottom-of-loop tests show the same register-only form.

## Tried (no change, 87.10 harness)

| Change | Result |
|---|---|
| operand orders of the four bounds (`y0 + ty`, `ty + y0`, ...) | same |
| `end >= blk.f4` | 86.64 |
| `< end + 1` | 79.18 |
| `static __inline` sum helper for the bounds | same |
| separate `struct Point pt` loop variable | 80.18 (frame grows) |
| `volatile` query / tile_y | 72-87 (two-register loads but no sharing) |
| 15-minute permuter | nothing |
