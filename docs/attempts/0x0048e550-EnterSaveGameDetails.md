# EnterSaveGameDetails (0x0048e550, `src/legoland/savegame_ui.c`)

**Best: 100.00% (verified)**

## What still differs

(not analysed yet)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | nested `count < 0x1f` / `0xcb > DAT_00798738` ifs, `register int right`, `unsigned int bottom`, `register RECT rc`, name / sprite_y temps, fy before fx | 76.70 -> 84.10 |
| 2026-10-09 | Opus 5.5 | one function-scope `RECT rc` for both by-value calls (no `register` RECTs) | 89.51; the original frame (0x18) reserves 16 bytes it never touches, ours is 8 |

## Ideas not tried yet

- 

## 2026-10-23 agent notes (90.91% -> 100.00%)


## 2026-10-22: reconstruct the shared text rectangle

Baseline refreshed from current main: harness 90.91. The original reserves a
24-byte frame; ours reserved only eight bytes because two block-local RECTs
were reconstructed from unrelated scalar bounds. One RECT used directly for
both by-value text calls restores the original 24-byte frame (95.10).

A conditional expression for the blinking string and precomputing the OK icon
bottom before the horizontal hit test bring the harness to 97.20, with
100% register-normalized structure. Removed register annotations, ptmp192,
and redundant sprite-y copies. The only remaining difference is ecx/edx
allocation for the horizontal mouse bounds (four instructions).

Tried: enlarging the two-byte cursor buffer (8/12/16/20) worsens the match;
RECT zero initialization also worsens it. Mouse-x local, explicit right bound,
operand swapping, and fx/fy assignment order did not improve the best.
No padding, volatile, or asm introduced.

## Current verified result

Full verifier: **100.00%**, improved from 90.91%, with no other per-function
regressions. The four harness-only ecx/edx differences are normalized by
reccmp. The apparently unused 16-byte frame region is the shared RECT, not
padding and not an oversized cursor string.
