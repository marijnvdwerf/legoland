# PrintSpriteEx (0x004856a0, `src/legoland/print_sprite.c`)

**Best: 96.64%**. Kind: C.

## What still differs

- `result = 1` is kept in `ecx` for the whole group path in the original (`mov ecx, 1` right after the y
  offset arithmetic; the count-zero exit returns `ecx`, while the loop exit returns literal `mov eax, 1`).
  Ours never keeps it in a register: MSVC folds the constant into the zero-count return.
- Because `ecx` is free in ours, `group->count` is loaded into `ecx` (original: `edx`), which changes the
  `test`/`jle` pair and the loop's count reload registers.
- Everything else (non-group path, mask loop, the calls) already lines up.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku agent | Move `result = 1` after the x/y offset lines | 96.64% (no change) |
| 2026-10-07 | Haiku agent | Move `result = 1` to just before the `group->count <= 0` check | 96.64% (no change) |

## Ideas not tried yet

- Make the non-group paths assign `result` and fall through to a shared `return result` so `result` is
  genuinely live across the branch (needs a restructure; check the non-group asm stays identical).
- Use a signed/unsigned change on `result` or the count comparison to influence which register is used for
  the count load.

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 96.64%. Matching has to come from C now.

## 2026-10-19 Opus 5.5 (plain C, no change: 96.64%)

What still differs: the original sets `ecx = 1` at entry and its `group->count <= 0` exit returns ecx
(`mov eax, ecx`), while the loop exit has its own `mov eax, 1`: `result` is a real variable there. Ours
constant-folds `result`, so the count test uses ecx instead of edx and both exits share one `mov eax, 1`.
Tried, all 96.64 unless noted: `result = 0; return result;` for the non-group failure, `result = RenderSprite...;
return result;` in the non-group path, both together, a single-exit non-group path (82.90), `result = Render...`
inside the loop (dead stores), `result = 1` inside the mask test, `return result` at the end (76.76).
