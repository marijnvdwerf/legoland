# DrawAppraisalBar (0x00444a70, `src/legoland/challenge.c`)

**Best: 80.21%** (start value, no change yet). Kind: C.

## What still differs

- Frame: the original starts with `push ecx` (a 4-byte slot at `E-4`). That slot holds `param_2 + 2`: the original computes it with `lea eax,[ebx+2]` and stores it with `mov [esp+0x38], eax` between the pushes of the first `RenderBlock` call. It is read back as the `y` argument of the marker `PrintSprite` (`mov ecx, [esp+0x4c]`). Ours has no such slot and stores the value into the dead `param_3` slot instead, so every later `esp+N` is 4 bytes off.
- Ours recomputes `param_2 + 2` in both calls, so the `add ecx,-2` / `lea` order differs.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku 5.5 (a2) | `mark` computed before the first `PrintSprite` | 80.21% (no change) |
| 2026-10-08 | Haiku 5.5 (a2) | `int y = param_2 + 2;` declared at the top and assigned before the first `PrintSprite`, used in both `RenderBlock`/marker calls | 54.55% (worse) |
| 2026-10-08 | Haiku 5.5 (a2) | Same `y`, assigned after the first `PrintSprite` (just before the first `RenderBlock`) | 80.21% (no change; ours stores `y` into the `param_3` slot) |
| 2026-10-08 | Haiku 5.5 (a2) | Same as above with `int y` declared first among the locals | 80.21% (no change) |

## Best

80.21%, the original file content (the `y` experiments were reverted).

## Ideas not tried yet

- Make the `y` value live across the `PrintSprite(AppBarSprite...)` call by using it there as well (for example `PrintSprite(AppBarSprite, param_1, y, 0, 0)` is wrong for the output, so instead compute `y` in an earlier block that is only used later), so MSVC cannot reuse `param_3`'s slot.
- Write the first `RenderBlock` arguments as one expression with `y` assigned inside the argument list (`RenderBlock(param_1 + 3, y = param_2 + 2, ...)`), to match the original's order of `lea`/`mov [slot]`.
