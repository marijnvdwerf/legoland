# OpenAviMovie (0x00476460, `src/legoland/interface.c`)

**Best: 89.77%**. Kind: C.

## What still differs

- Tail sharing: the original's `video_stream == NULL` path jumps into the single `AVIFileRelease(file)` call of the `malloc`-failure block. Ours duplicates the `AVIFileRelease` + `AviOpenCount` epilogue in the `NULL` path.
- The original's `malloc` block is placed before the success block, with the `jne` to the success path at `0x476589`.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku 5.5 (a6) | Removed `AVIFileRelease` and the epilogue from the `malloc`-failure block (it falls through to the shared bottom) | 85.71% (worse) |
| 2026-10-08 | Haiku 5.5 (a6) | Moved `AVIFileRelease(file)` after the `video_stream` if/else, shared by both paths, `malloc`-failure falls through | 70.85% (worse) |
| 2026-10-08 | permuter + Opus 5.5 | `int left` temp for frame_left; handle->file stored before handle->frame; `++AviOpenCount` | 89.11 -> 89.77 |

## Best

89.11%, the version currently in the file.

## Ideas not tried yet

- Make the `NULL` path a nested `if (audio_stream != NULL)` that falls into the same release as the `malloc` failure without a shared statement.
