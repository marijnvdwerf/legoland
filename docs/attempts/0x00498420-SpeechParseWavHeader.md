# SpeechParseWavHeader (0x00498420, `src/legoland/stream.c`)

**Best: 93.19%**. Kind: C.

## What still differs

- The original reads the first chunk tag before the loop, then tests `tag == 'data'` at the top of the loop, with the
  next tag read at the bottom (`jne` back to the top). Ours puts the read in the loop condition, so the bottom test and
  the early-return layout differ.
- Original has the `tag != 'data'` return only as a shared fallthrough; ours has an explicit final check.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 p7 | Haiku agent | Read tag before loop; `while (tag != 'data')` with the tag read at the bottom | 87.43% |
| 2026-10-07 p7 | Haiku agent | `for (;;)` with `if (tag == 'data') break;` at top and the initial read outside the loop | 68.69% (early returns stop being inlined) |

## Ideas not tried yet

- Original's early returns are inline and only the loop-end return is shared; try moving the `tag != 'data'` check
  into the data path.

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 93.19%. Matching has to come from C now.

## 2026-10-19 agent notes (93.19% -> 100.00%)

What the diff was: after the chunk-skipping loop the C tested `if (tag != 'data') return 0;`, which the original
does not have: there a failed tag read returns 0 straight away and the 'data' tag jumps to the data path. The loop
in the original is `do { if (read tag fails) return 0; if (tag == 'data') break; skip the chunk; } while (1);`
followed by the data path. MSVC6 rotates that loop (the tag read appears again at the bottom) and keeps every
early return inline, exactly as in the original.

| Loop form | verify |
|---|---|
| `while (read == 4) { if (tag == data) break; ... }` + `if (tag != data) return 0` (old) | 93.19 |
| the same with the skip in an `else` | 93.19 |
| `for (;;)` + read/return/break inside | 74.16 (all early returns merged into one) |
| `while (1)` + read/return/break inside | 89.83 (not rotated) |
| data path inside the loop, or `goto found_data` | 72.31-74.16 |
| `do { ... } while (1);` + read/return/break inside | 100 |
