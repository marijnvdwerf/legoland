# UpdateEditSelection (0x00452030, `src/legoland/controller.c`)

**Best: 100.00%** (matched 2026-10-19). Kind: C.

## How it matched

- `DAT_00813a84..90` are one rectangle: typed as `struct MapRect SelectionRect` (x0, y0, x1, y1) and stored in natural order. With four separate ints MSVC keeps the store order reversed; with the struct the natural order matches.
- Body from the 2026-10-09 natural-order version (`decomp-rw/0x00452030/land.c`).
- `extern struct MapRect SelectionRect;` is declared in `controller.h`, NOT appended to `globals.h`: appending it there drops `MusicThreadProc` (0x492db0, sound_sfx.c, unrelated) from 100% to 99.89% (register scheduling shift).

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 r1 | Haiku agent | Read the four cursor globals into locals on both paths | 9.71% -> 14.58% (kept) |
| 2026-10-07 r2 | Haiku agent | Read `DAT_00813a74..80` directly instead of copying to locals (frame 0x1c -> 0x14, matches) | kept |
| 2026-10-07 r2 | Haiku agent | `v74 == v7c && v78 == v80` -> `memcmp(&DAT_00813a74, &DAT_00813a7c, 8) == 0` (the original uses `repe cmpsd`) | 14.6% -> 21.7% (kept) |
| 2026-10-07 r2 | Haiku agent | Else branch reuses four locals like the original; `local_c + v74` -> `FootprintWidth - 1 + DAT_00813a74` | kept |
| 2026-10-07 r2 | Haiku agent | `(int)` casts on the compared globals (they are `unsigned int`; the original uses jge/jle) | kept |
| 2026-10-07 r2 | Haiku agent | `abs(74-7c) > abs(78-80)` instead of `<` with swapped operands (78/80 evaluated first) | kept, total 33.15% |
| 2026-10-08 | permuter + Opus 5.5 | EditMode==2 block like the original: both minimums as if/else, maximums into local_c/iVar5, footprint non-NULL case first, the four globals stored together at the end; else block: `74 >= 7c` / `78 >= 80` conditions, local_c as if/else | 33.15 -> 52.43 |
| 2026-10-08 | permuter + Opus 5.5 | DAT_00813a74..90 and FootprintWidth/Height retyped to int in globals.h/.c (every use cast them to int; the original divides with cdq/idiv) - casts removed, no other function changed | 52.43 -> 52.91 |
| 2026-10-08 | permuter + Opus 5.5 | memcmp test written != 0 with the equal case in the else; width/height special cases written positively first (FootprintWidth > 1 && ... ==) | 52.91 -> 57.36 |
| 2026-10-08 | permuter + Opus 5.5 | local_c / iVar5 declaration order swapped (for the swapped [esp+0x18]/[esp+0x1c] slots) | 57.36 (no change) |
| 2026-10-09 | Opus 5.5 | DAT_00813a84..90 stores in natural order a84, a88, a8c, a90 (the original order) in both arms | 74.36 (store order right, but min/max registers all shift; the reversed order was compensating). The four globals are a rect |
| 2026-10-09 | Opus 5.5 | permuter 12 min (about 180-270 variants) | no gain |
| 2026-10-19 | Opus 5.5 | 81 local variants of the reversed-order version | best 98.16% (2/272 instr: FootprintHeight load vs `repe cmpsd`) |
| 2026-10-19 | Opus 5.5 | a84..a90 -> `struct MapRect SelectionRect`, natural store order, extern at end of globals.h | 100% but MusicThreadProc 100 -> 99.89 (rejected) |
| 2026-10-19 | Opus 5.5 | same, extern moved to controller.h | **100%, 0 regressions (kept)** |
