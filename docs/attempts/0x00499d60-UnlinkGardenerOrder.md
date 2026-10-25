# UnlinkGardenerOrder (0x00499d60, `src/legoland/worker.c`)

**Best: 76.73%**

## What still differs

The original has one "Work orders START" DBPrintf block placed right after the head-of-list branch; the search path
jumps back to it (tail merge). Ours emits the print twice: the first copy loads Tail into ecx (Head/Tail registers
differ between the copies), so MSVC does not merge them.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-09 | Opus 5.5 | one shared print after an if/else (not-found path returns early) | 71.23 (else body laid out before the print) |

## Ideas not tried yet

- make both print copies load Head/Tail the same way so the tail merge happens

## 2026-10-23 Opus 5.5

Original layout: the head branch falls into ONE shared START print (Head/Tail reloaded from memory, 0x499dcc).
The search loop and the not-found print come after the `ret`; the found path jumps back to the shared print
(`jne -0x4d` / `jmp -0x55`). Both candidate forms reach the original's 68 instructions but score lower: a
`goto done` label inside the head branch scores 73.53 (MSVC moves the labeled block to the end), and if/else
with the print after the join scores 74.45 (else body before the print). Inverted condition: 56.16; for+break
inside the else: 71.23.
