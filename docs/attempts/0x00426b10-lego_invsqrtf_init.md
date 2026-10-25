# lego_invsqrtf_init (0x00426b10, `src/legoland/castle.c`)

**Best: 97.83%** (the code is byte-identical)

## What still differs

Only reccmp's symbolization of the first loop's end pointer: `cmp eax, &invsqrtf_table[0] + 0x204`. In the original
that address is `invsqrtf_exp_table` (0x610e44), because there are 4 unexplained bytes at 0x610e40 between the two
tables. In our image nothing lies there, so it prints as `invsqrtf_table+0x204`.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-09 | Opus 5.5 | analysis only | - |

## Ideas not tried yet

- Fixing it needs something 4 bytes long at 0x610e40 followed by `invsqrtf_exp_table` in our .bss (a `[129]` table
  or a filler global). That is padding, which the project rules don't allow, unless the real variable at 0x610e40 is found.

## Solved (2026-10-21, Opus 5.5): 97.83 -> 100

The only diff was the loop end pointer: the mantissa loop walks `invsqrtf_table` 8 bytes at a time and stops
at `invsqrtf_table + 0x204`, which in the original is `invsqrtf_exp_table` (0x610e44; 0x610e40 is never
referenced). In our build the two arrays were uninitialised commons in globals.c, placed apart by the
linker, so the same address was some other global. Fix: define both in castle.c next to the function,
initialised (`= {0}`, so they go to .data in definition order), with `invsqrtf_table[129]` covering the
unused float. Codegen of the function itself never changed.
