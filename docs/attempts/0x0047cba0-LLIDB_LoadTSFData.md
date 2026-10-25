# LLIDB_LoadTSFData (0x0047cba0, `src/legoland/llidb.c`)

**Best: 93.40%** (unchanged C from the start of this round). Kind: C.

## What still differs

- Register permutation only: the original takes `sprites[i]` in `eax` (reloaded from `si->sprites`), ours uses `ecx`.
- After `RES_ReadFile(file, &len, 4)` the original takes `&len` in `ecx` and ours in `edx`.
- `element` is loaded into `eax` before the `head->data` store in the original, into `ecx` in ours; the `*(element + 0xc)` temp is `edx` vs `eax`.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Claude Haiku 5.5 | Duplicate `anim = si->sprites[i]->image;` line | no change (90.58%) |
| 2026-10-07 | Claude Haiku 5.5 | Read the sprite through a `struct Sprite *sp` local | no change (90.58%) |
| 2026-10-08 | permuter + Opus 5.5 | `char *data_c` temp for the first RES_ReadFile; `if (ret == 4) { if (len != 0)` nested; terminator store before the name read; data_14 before head->data | 90.58 -> 93.40 |

## Ideas not tried yet

- Reorder the `element` use so the original's `eax`/`edx` split comes out (e.g. read `*(int *)(element + 0xc)` into a local before the `head->data` store).

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 94.46%. Matching has to come from C now.

## 2026-10-19 Opus 5.5 (plain C, no change: 94.46%)

The original stores `name[len] = 0` after the name read (it is scheduled between the next call's pushes), but
moving the terminator back after `RES_ReadFile(file, name, len)` gives 90.58, also with the `if (ret == 4 && len)`
flattened, without `ret`, without `data_c`, and with `&si->sprite_ids[i]` indexing. The permuter's order stays.
