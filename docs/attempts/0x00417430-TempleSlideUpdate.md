# TempleSlideUpdate (0x00417430, `src/legoland/temple_slide.c`)

**Best: 79.39%**

## What still differs

(not analysed yet)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | `register unsigned int cy` (cy >>= 9 becomes shr - suggests the original is unsigned), `unsigned short sx`, tile before bloke, field_73 temp. permuter's `cx -= a / 2 - b` (wrong) not kept | 75.35 -> 79.39 |

## Ideas not tried yet

- 

## 2026-10-24 agent notes (79.83% -> 81.22%)


## 2026-10-24 (Opus 5.5): harness 81.38 -> 82.81
Cleaned up the permuter leftovers (`register`, `ptmp227`, a bare `node;`, `target_dir` copy) and rewrote case 3 in plain C.
Frame map (the `fn ADDR frame` command prints our listing's `_var$ = N` offsets): the original is node -48, ? -44, tw -40,
th -36, next -32, dx/dy -28/-24, coords -20, off -12 (3 ints, off[2] unused). VC6 packs block-scoped and dead-range
variables into one slot: in both builds the (px - py) * tw >> 9 value gets a dead store into node's slot.
| Change | Result |
|---|---|
| dx/dy as one `Point d` (lands between next and coords like the original) | 81.09 -> 82.81 |
| `int off[3]` (the original's size) | 80.23 (everything below moves 4 bytes: the original has one more unreferenced 4-byte slot at -44) |
| 432 case-3 forms: which of x/y/t/u/px/py hold pos.x, pos.y, the two products and the screen sums, statement orders, off[] written directly or via x/y | best 82.01 |
| `struct Point screen` at function or block scope (carousel/spider style) | 72-80 |
| declaration order (400 random orders), local renames | no change (VC6 frame order here does not follow declaration order or names) |
Left: ours keeps coords.x in ebx across the scroll calls and spills py; the original keeps coords in memory and puts px in
ebx (copy in ebp). The rest of the diff is the 4-byte frame shift. Same family as Safari/Spider/Plane/CarouselUpdate.

## 2026-10-24 agent notes (81.22% -> 81.72%)


## 2026-10-24 Opus 5.5: harness 82.81 -> 83.33
Case 3 computes the playfield x offset straight into `x` and adds the scroll with `x += view_x - sx`, the same shape as
the y line, instead of going through a separate `t`. Found by a sweep that merges same-typed locals pairwise
(slotsweep). The frame still lacks the original's unused slot (see ride-update-family.md).
