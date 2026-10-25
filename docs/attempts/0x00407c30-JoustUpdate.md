# JoustUpdate (0x00407c30, `src/legoland/joust.c`)

**Best: ?**

## What still differs

- (2026-10-11) The original's jump table (0x408504, 31 entries) gives every case its own block:
  0:0x103 1:0x162 2:0x229 3:0x2b3 4:0x316 5:0x373 6:0x3c4 7:0x4a7 10/15:0x4f9 11:0x52d 12:0x541
  13/25:0x567 14:0x59e 16:0x5ce 20:0x5f4 21:0x60b 22:0x622 24:0x639 23/26:0x65f 27:0x679 28:0x693
  29:0x6e8 30:0x72e default(8,9,17,18,19):0x742. Ours merges the cases whose bodies are identical
  (11 = 14, 21 = 28, 16 = 29, 10/15 = 20) onto one entry, so our function is 29 instructions shorter.
  In the original those pairs only share a tail (from the CalcMoveLine call, or after the dest.x store),
  and their prefixes use different registers (case 16 pushes ebp/edi copies, case 29 reloads), so either the
  bodies differ slightly or the global register allocation differs enough that cross-jumping cannot
  merge them (it may resolve once the rest of the function matches). Things to try per case (dest as a local Point, the
  order of the dest stores, `x * 256` vs `x << 8`, ...); the case blocks are in source order 11 then 12.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-09 | Opus 5.5 | `stop`/`play` sample sources as `struct TileSampleSource` initialisers | 70.80 harness (no change; the stores already match) |

| 2026-10-11 | Opus 5.5 | case order search (harness + reccmp) | cases 12 and 11 swapped: 69.04 -> 74.66 (kept) |

## Ideas not tried yet

- 

## 2026-10-24 Opus 5.5 (analysis)
Jump table check (raw tables from both exes): ours has 21 distinct case targets, the original 24. Cases 14, 28 and 29
have bodies identical to 11, 21 and 16 in our C, so MSVC6 merges them; the original keeps each as its own block and
only cross-jumps their common tails (checked with capstone: the constants are the same, so this is not a behaviour
bug). The original's copies differ in which dest field is reloaded (case 14 passes dest.y from a register, case 28
reloads both), so their source differs slightly. Rewrites of case 29 / case 14 (`x <<= 8` locals, field order): 74.8-77.16.

## 2026-10-25 agent notes (75.94% -> 76.22%)


## 2026-10-24 Opus 5.5: harness 77.16 -> 77.45
The two per-loop `struct JoustPair` copies (`h` in the update loop, `s` in the sound loop) are one function-scope
local in the original. Found by the slotsweep block-merge pass.
