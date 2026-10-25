# ConvertWaveToPcm16 (0x004921c0, `src/legoland/sound_sfx.c`)

**Best: 92.26%** (start 92.26%)

## Tried
| Date | Model | Change | Result |
|---|---|---|---|
| 2026-10-07 | claude-haiku-5-5 | Moved `dst.wFormatTag = 1` before `dst.nBlockAlign = ...` | 90.97% (worse, reverted) |

## What still differs
- Original copies `*src` as dwords and computes `nBlockAlign` from `src->nChannels` before loading the
  copy of offset 0xc/0x10; ours loads offsets 0xc and 0x10 at the top of the copy.
- Early-return epilogues and the success tail differ slightly (ours tail-duplicates the `acmStreamUnprepareHeader` path).

## Ideas not tried yet
- Copy the fields individually (no struct assignment) to get the original's load/store interleave.
