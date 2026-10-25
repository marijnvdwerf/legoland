# LoadObjectClassAliasElements (0x004809d0, `src/legoland/objclass.c`)

**Best: 98.63%** (verified 2026-10-18; previously 73.10%). Plain C.

## What still differs

Only the table-end comparison at `0x480a7a` differs in reccmp:
`cmp ebx, 0x4bcdf8` versus a reference rendered as `ScaledSpriteBreakText[4]`.
The source computes `&ObjClassAliasTable[5].aliases`, offset `0x2c` from the table
annotated at `0x4bcdcc`, which gives the original address. This is four bytes past
the five-entry table, so the comparison gets named after neighboring data in the
rebuilt image. The standalone object comparison also leaves only this address
normalization difference. All other instructions, registers and branches match.

The retained change initializes `token` before `strcpy` and restores `*semicolon`
before advancing `token`. MSVC6 then keeps `token` in EBP and `elem` in EDI,
leaving EBX for the table iterator instead of hoisting the constant flag 4 into it.
This restores the original immediate flag test and load/OR/store sequence.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-09 | Opus 5.5 | `_strcmpi` (the original calls strcmpi, not stricmp) + `GLOBAL` annotation on ObjClassAliasTable (0x4bcdcc) | 73.10 (call and table now match; kept) |
| 2026-10-09 | Opus 5.5 | index loop `for (i = 0; i < 5; i++)` over the table | 73.10 |
| 2026-10-18 | Codex | Advance `token = semicolon + 1` immediately after `ElemID`, before the load tests; also split the advance into assignment/increment and vary initial token setup | 95.89-97.26% in the standalone harness; the advance is scheduled too early |
| 2026-10-18 | Codex | Restore the delimiter before the load tests | 94.52-95.89% harness; rejected |
| 2026-10-18 | Codex | `ElemID(token++)`, with the normal later token assignment and initialization before `strcpy` | 98.63% harness/full build; discarded in favor of the natural form below, which needs no redundant increment |
| 2026-10-18 | Codex | Initialize token before copying; restore delimiter before advancing token; retain the original `while` | **73.10 -> 98.63%**, retained and commented |
| 2026-10-18 | Codex | Same setup with `for`, search in the `for` clause, guarded `do/while`, restoring through `*token++`, or advancing by `semicolon - token + 1` | All 12 combinations 98.63% harness; retained the simplest `while` and direct assignment |

## Validation (2026-10-18)

- Built in WSL `/home/fsterzer/legoland`; formatted the changed function and passed `tools/needsdecl.py objclass` and `git diff --check`.
- Compared all 3,594 entries in full verification reports before/after: only this function changed; no regression or effective-match change elsewhere.
- No assembly, volatile access, header changes, or table-layout changes.

## Ideas not tried yet

- Address normalization for the pointer four bytes beyond the table. Do not change the
  game data or substitute an unrelated neighboring symbol just to change the score.

## Solved (2026-10-21, Opus 5.5): 98.63 -> 100

The only diff was the loop end pointer `&ObjClassAliasTable[5].aliases`: in the original it is 0x4bcdf8, inside
the string "BOATING SCHOOL WATER;" (0x4bcdf4) that follows the table, because the table is in .data at
0x4bcdcc with its alias strings right after it. Ours was `static const`, so it went to .rdata and the same
pointer pointed past an unrelated end. Dropping `const` from the table (`slot` is `const char **`) puts it in
.data with the strings after it. Codegen of the function never changed.
