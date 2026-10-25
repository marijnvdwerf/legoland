# stackdump (0x00453da0, `src/legoland/exceptlog.c`)

**Best: 80.12%**

## What still differs

(not analysed yet)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | decl order: ctx after sep, hFile after bufend | 79.53 -> 79.82 |
| 2026-10-08 | permuter + Opus 5.5 | Full rewrite attempt (stack order, locals) | No gain: the original reads the stack base with inline asm (mov eax, fs:[4]) and clamps pStackTop with it (0x454124-0x454141); not expressible in MSVC6 C without asm, and the asm also changes the frame layout. It is Bruce Dawson's RecordExceptionInfo (exceptlog.txt). |

## Ideas not tried yet

- 
