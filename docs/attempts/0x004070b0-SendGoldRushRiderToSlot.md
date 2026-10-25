# SendGoldRushRiderToSlot (0x004070b0, `src/legoland/gold_rush.c`)

**Best: 79.31%**

## What still differs

The original keeps `dest.y` in edi (pushes edi) across the `__ftol` call and applies `- 0x50` to it there; `dest.x`
is reloaded from memory after the call. Ours reloads `dest.y` after the x update and never uses edi.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-09 | Opus 5.5 | `dest.y -= 0x50` before the x update | 77.88 (folded into the earlier `+=`) |
| 2026-10-09 | Opus 5.5 | `off = (int)(w * 512)` (or `0x80 - ...`) local, then y, then x | 77.88 |

## Ideas not tried yet

- a local for dest.y across the last three statements

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 79.31%. Matching has to come from C now.

## 2026-10-19 Opus 5.5 (plain C, no change: 79.31%)

What still differs: the original keeps `b->dest.y` in edi (pushed at entry) from the first `+=` through the
`_ftol` call to the CalcMoveLine push, while dest.x is reloaded from memory; ours reloads both. Tried, all <= 79.31:
a `Point *d = &b->dest` (two forms), no `w` local (63.41), slot/weight load order (69.57), y statements before x
(59.26), `y -= 0x50` before the x update (75.23), `*p` / `p[1] * 256`, an `int y` local carried through the three
updates (four forms, 64.29), a struct copy `b->dest = Gold_Points[slot]; b->dest.x -= 0x280;` (50.41), an `off`
local for the ftol (63.41), `-= (int)(w * 512) - 0x80`, `512.0f * w`, `+= -0x50`.
