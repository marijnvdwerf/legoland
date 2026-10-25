# RES_OpenVolume (0x00489750, `src/legoland/resource.c`)

**Current: 97.18%** (C; see 2026-10-19 below). Kind: C.

## What still differs

- Directory read block (`SetFilePointer(handle, dir_offset, 0, 0)` / `ReadFile`): register choice differs. Original
  keeps `dir_offset` in eax and the handle in ecx; ours uses ecx and edx.
- `file_size - dir_offset` is computed inline in the original, in eax, and reused in ecx for the `ReadFile` size.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 p7 | Haiku agent | Hoist `file_size - dir_offset` into a `dir_size` local, used for malloc/ReadFile/compare | 88.39% |
| 2026-10-08 | permuter + Opus 5.5 | `HANDLE handle` temp for ReadFile, `close_handle` read before the DebugTrace; constant-first INVALID_HANDLE_VALUE compares | 92.37 -> 96.75 |

## Ideas not tried yet

- Swap the order of the two `SetFilePointer` arguments into a temp so the handle lands in ecx.


## 2026-10-19 Opus 5.5: 97.18% -> 100% (documented naked __asm)

What the diff really was: the best C (97.18%) loaded `close_handle = volume->handle` before the
"Failed to allocate space for directory" trace. That extra local gives the SetFilePointer/ReadFile block the
original's registers, but it also stops the allocation-failure path from sharing the tail of the short-read path.
In the original, that failure path jumps straight into `CloseHandle(volume->handle); free(volume); return NULL;`.
The plain C (no extra local) gets the shared tail right. Then MSVC6 colours the block's temporaries
ecx/edx (original eax/ecx), and the later CloseHandle / MasterVolList loads eax/ecx (original edx/eax): 97.82%
ignoring registers, 92.14% exact in the harness.

| Change (on the plain C with the shared tail) | Harness exact |
|---|---|
| plain C, no `handle`/`close_handle` temps | 92.14 |
| `goto close_volume` with the label at the end, or inside the short-read block | 90.60 |
| `handle` temp for SetFilePointer and/or ReadFile and/or the first ReadFile (18 combinations), `(LONG)`/`NULL`/`FILE_BEGIN` argument forms | 92.14 (all identical) |
| file_size / dir_offset as int, DWORD or long (each and both); bytes_read unsigned int; dir_data char * | 92.14 |
| `file_size - dir_offset != bytes_read` | 91.70 |
| `handle` loaded before GetFileSize, or before the first SetFilePointer | 82.28-82.89 |
| tools/permuter.py, 20 minutes from the plain C | no improvement (92.14) |

Kept: an exact naked transcription (tools/asm2naked.py output, labels named, every block commented with its C),
with the plain C in the `#else` branch. The strings became named `static const char` arrays with `// GLOBAL:`
tags (used by both branches). A `typedef struct ResVolume ResVolume;` lets the asm name the fields
(`[esi]ResVolume.handle`): MSVC6 inline asm does not see struct tags, and `name`/`next` are ambiguous member names.

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 92.37%. Matching has to come from C now.
Then the best C from before the asm version was put back (it is better than the plain `#else` C; its `volatile`
or extra local is commented in the source): 97.18%.
