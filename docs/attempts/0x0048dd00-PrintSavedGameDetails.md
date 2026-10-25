# PrintSavedGameDetails (0x0048dd00, `src/legoland/savegame_ui.c`)

**Best: 92.33%** (start version, unchanged). Kind: C.

## What still differs

- Register allocation for the `by` and `rc.top` locals in the DeletePopUpShown branch: the original puts `by` in `ebp` and `rc.top` in `edi`, ours the other way round.
- The `rc.right` constant (0x14e) goes to `ebx` in the original and `ebp` in ours.
- The NewPrintCent `RECT` argument is stored in a different order: the original reads `rc.right` back from the stack and stores `top`/`right`/`bottom` in a different slot order.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 p8 | Haiku agent | Moved `RECT rc` above `int by` / `int draw` in the locals | 92.33% (no change) |

## Ideas not tried yet

- Restructure the DeletePopUpShown branch so `by` is assigned from a value computed first (changes which local the allocator sees first).
