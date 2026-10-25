# UpdateSampleVolume (0x004966a0, `src/legoland/sound_music.c`)

**Best: 64.15%**

## What still differs

Cases 1 and 3 share their last store (`mov [esp+8],eax` = coord[1]) and the one SetSampleVolumeAndPan call in the original
(case 1 ends with `jmp` to it). Ours duplicates the whole call tail into case 1.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | obj[8] read before obj[7] via temp | 61.64 -> 64.15 |
| 2026-10-09 | Opus 5.5 | coord as `struct Point` (with and without the obj8 temp); no temp | 62.16-64.86 harness |

## Ideas not tried yet

- 


## 2026-10-19 Opus 5.5 (no change, 70.44% in verify / 65.31% in the harness)

The diff, read again: in the original, cases 1 and 3 both end with the same `mov [esp+8],eax` (coord[1]). MSVC6
merges those two stores into one (case 1 jumps into case 3's last store), and after that one shared tail reads
coord back from the stack and calls SetSampleVolumeAndPan. Ours copies the whole call tail into case 1 and case 3
and passes the registers straight to the call, so those two cases have their own call and epilogue. Every variant
below kept that duplication:

| Change | Harness exact |
|---|---|
| all 24 orders of the four case blocks | 43.24-65.31 (0,1,3,2 is the current best) |
| `int result = SetSampleVolumeAndPan(...); return result;` | 62.16 |
| explicit `default: break;` | 62.16 |
| `switch` on a local `type` copy | 62.16 |
| the three early returns as one `||` test, or nested `if (... && ... && ...) { switch; return ...; } return 0;` | 44.60 |
| single exit through `result` (initialised to 0) | 39.71 |
| `struct Point coord`, case 1 as a struct copy `coord = *(struct Point *)&obj[7]` (x/y and y/x order in case 3) | 58.50-62.59 (copy_xy is 80.27 ignoring registers, still duplicated) |
| `#pragma optimize("s", on)` / `("t", off)` around the function | 18.75. The `s` build shows the original's shared `mov [coord+4],eax` store, but it uses a frame pointer and a compare chain instead of the jump table |
| tools/permuter.py, 20 minutes | no improvement |

A jump table cannot be written in naked asm here, so the remaining path is C. Idea not tried yet: find a statement
form that makes MSVC6 treat the post-switch call as too big to duplicate. The original's tail is no bigger than
ours, so the source must differ before the switch, or in how the coord values are produced.

## Additional typed/shared-tail checks

Refreshed current main baseline: harness 65.31, full verify 70.44.
Scalar x-then-y stores in cases 1/3 score 62.16. Replacing int-array
pointer arithmetic with RideNode->rider->pos (including the owning headers)
also scores 62.16. An explicit source_y temporary and named shared apply
label score 65.31 in the scalar form, 63.95 in the typed form. A shared-call
label alone scores 62.16. None removes the duplicated call tail; none kept.
