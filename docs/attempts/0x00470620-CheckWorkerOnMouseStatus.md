# CheckWorkerOnMouseStatus (0x00470620, `src/legoland/worker_mouse.c`)

**Best: ?**

## What still differs

(see the rows below)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-09 | Opus 5.5 | the never-looping `for (;;)` replaced by a shared `goto fail` exit (`DAT_00668954 = 1; SetWorkersPositionAtMouse();`) | harness 80.11, verify 77.36 (reverted). The original keeps the constant 1 in ebp (`mov esi, ebp` for isOrder); ours uses immediates |

## Ideas not tried yet

- 

## 2026-10-21 (Opus 5.5): still 78.36 in verify

What differs: the original materialises the constant 1 in ebp (`mov ebp, 1` at entry, after the
`lpConfig->height` / GameMap use of ebp, and before the 0x307 test) and stores ebp for every `= 1`
(`DAT_00667c48`, `isOrder` via `mov esi, ebp`, every `DAT_00668954 = 1`). Ours stores immediates. The
"row out of range" block at 0x4707a3 is the fail path duplicated only because ebp must be reloaded there.

| Change (harness scores; its baseline moved: repo version = 75.63) | Result |
|---|---|
| goto `fail` shared exit (as 2026-10-09) | 77.36 harness (verify 77.36, worse than the repo's 78.36) |
| row check folded into the `goto fail` condition; all four bounds + unconditional elem; bounds macro with `else goto fail`; `if (y >= h) {...}` first | 76.37-77.36 |
| `goto done` instead of the `DAT_00668954 != 0` tail; direct `if (SetXOrder(...) == 0) goto fail` (no `result`) | 74.86-77.36 |
| `do { } while (0)` / `while (1)` instead of `for (;;)`; `DAT_00667c48 = 1` inside the loop | 75.63 |
| local `int on = 1` for every 1; `unsigned` isOrder/result; globals read as `int` (macro test) | no change |
| 120+ random declaration orders of the 6 locals | no change |
| permuter 20 min on the goto version (97 variants) | no gain |

## Ideas not tried yet

- Find what makes MSVC6 keep a constant in a callee-saved register across blocks: compare with a function in
  this TU that does it (`grep -B2 'mov ebp, 1'` in the original disassembly of worker_mouse.c).
