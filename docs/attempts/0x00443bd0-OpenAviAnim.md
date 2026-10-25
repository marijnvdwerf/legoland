# OpenAviAnim (0x00443bd0, `src/legoland/challenge.c`)

**Best: 91.98%**. Kind: C.

## What still differs

- Failure path of `AVIFileOpenA`: the original does `cmp [DAT_00665f48], ebx; jmp` into a shared exit tail; ours jumps directly.
- `found == NULL` path: the original jumps to the `AVIFileRelease` call, and the `malloc`-failure path falls through into it. Ours has the two blocks in the opposite layout.
- Register choice for the last `AVIFileRelease` argument: the original uses `ecx`, ours `eax`.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 p9 | Haiku agent | Swapped the last two branches (`result != NULL` first, `AVIStreamRelease` in the `else`) | 79.32% (worse) |
| 2026-10-07 p9 | Haiku agent | Failure path written out as an early return with its own `DAT_00665f48 == 0` / `AVIFileExit` cleanup | 89.76% (worse) |

## Best

91.98%, the version currently in the file.

## Ideas not tried yet

- Declare `file` or `stream` in a different order so the final push picks `ecx`.