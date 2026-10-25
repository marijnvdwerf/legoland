# LoadBaseMap (0x00461a50, `src/legoland/map_object.c`)

**Best: 84.15%** (unchanged from start). Kind: C.

## What still differs

- Four diff hunks (at 0x461a62, 0x461ad7, 0x461c84, 0x461dc3). Most of the function matches.
- Stack layout: small scalar locals sit 4 bytes lower in ours than in the original (for example `len`
  is `[esp+0x14]` in ours and `[esp+0x18]` in the original; `elem` is `[esp+0x18]` vs `[esp+0x1c]`).
  The large buffers (`[esp+0x134]`, `[esp+0x340]`, `[esp+0x350]`) and one slot at `[esp+0x10]` match, and the
  frame size (`0x524`) is the same. The cause is not yet found.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku agent | `elem` declared before `file` | 81.95% (same) |
| 2026-10-08 | Haiku agent | `len` declared first in the locals list | 81.95% (same) |
| 2026-10-08 | permuter + Opus 5.5 | `register int off`/`runlen`, reordered tile/v and prev/phase stores, tile_flags / rf temps, nested `(len & 8) == 0` if, constant-first operands | 81.95 -> 84.15 |

MSVC does not seem to care about declaration order for these slots (the `file`/`elem` and `len` swaps changed nothing).

## Ideas not tried yet

- Check whether the nested blocks (`n/idx/objpos` and `prev/carry/hibit/runlen/rle`) take slots that
  overlap in the original; hoisting them to the top changes the frame size, so compare the frame first.
- Check whether `hdr[200]` / `strbuf` / `namebuf` ordering or size (512 vs another size) moves the scalars.

## 2026-10-21 (Opus 5.5): still 96.71 harness / 97.17 verify
Frame is the same size (0x524). The original reuses slot [esp+0x40] for an address-taken local and later
for a compiler temporary in the tile loop; ours gives the temporary its own slot, shifting six slots by 8.
| Change | Result |
|---|---|
| 295 random declaration orders of the 20 locals | 96.71 (layout ignores declaration order) |
| n/idx block-scoped in the first loop (frame shrinks to 0x51c); separate block-scoped pos per loop | 89.88-92.51 |

## 2026-10-24 agent notes (97.17% -> 97.60%)


## 2026-10-24 Opus 5.5: harness 96.71 -> 97.20
Slot map (orig vs ours, from the -8/+20/+8 offset diffs): the original's bridge-name length shares a slot with `sample`,
and the overlay `count` shares gfx1's slot; ours had them the other way round. Reading the length into `sample`
instead of `count` gives the original's packing for both. Same behaviour: sample is set to 0 before the BRIDGES header,
and the overlay loop only reads 2 bytes into it, so a name length below 0x10000 leaves its high half 0 as before.
Left: sq1/sq2 swap partners (sq1 should share sample's slot), pos/tilemap one slot lower, and the hoisted `prev & 0x20`
and runlen temps of the first RLE block sit in each other's slots. No change from: the four Points declared in any
order, per-case or per-if Point scopes, reusing n/idx for any block int, unifying block vars across blocks, volatile probes.
