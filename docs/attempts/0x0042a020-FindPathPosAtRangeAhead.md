# FindPathPosAtRangeAhead (0x0042a020, `src/legoland/castle.c`)

## What still differs

(Behind and Ahead are twins.) The original passes the `hi` argument of the first `DAT_004b63fc` call through the FPU:
it reserves the slot with an early `push ecx` and later does `fld hi; fstp [esp]`. Ours pushes hi with an integer move.
`lo` lives in the dead `src` parameter slot in both.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-09 | Opus 5.5 | analysis only | - |

## Ideas not tried yet

- an expression for the hi argument that is a float rvalue (e.g. the value of an assignment or a ?: of floats)
