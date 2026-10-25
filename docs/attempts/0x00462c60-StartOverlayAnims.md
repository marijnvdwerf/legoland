# StartOverlayAnims (0x00462c60, `src/legoland/map_object.c`)

**Best: 100%* (effective match, 2026-10-09)**. Kind: C.

## What still differs

- The index read in the BridgesData branch (`sample = ... (node->field_10 & 0xff) ...`) is emitted as
  `mov cl, byte ptr [esi+0x10]` by MSVC when it is written through `short`. The original does a dword load
  (`mov ecx, [esi+0x10]`, then `and ecx, 0xff`). The same byte load shows up in the ILF branch (`mov al, byte ptr`).
- Otherwise the loop and both branches match.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku agent | Start: `if (*(short *)&node->field_10 & 0xff00)` | 89.16% |
| 2026-10-08 | Haiku agent | `if (node->field_10 & 0xff00)` (int) | 58.97% (test ah right, but index CSE'd and branches reshuffled) |
| 2026-10-08 | Haiku agent | `if (((node->field_10 >> 8) & 0xff) != 0)` | 58.97% |
| 2026-10-08 | Haiku agent | `if (*(unsigned short *)&node->field_10 & 0xff00)` | 89.16% (byte test again) |
| 2026-10-08 | Haiku agent | `if ((unsigned short)node->field_10 & 0xff00)` | 89.16% (byte test again) |
| 2026-10-08 | Haiku agent | int local `flags = node->field_10; if (flags & 0xff00)` | 58.97% |
| 2026-10-08 | Haiku agent | `unsigned short flags` local | 89.16% (byte test again) |
| 2026-10-08 | Haiku agent | int test (`node->field_10 & 0xff00`) + index as `(*(short *)&node->field_10 & 0xff)` in both branches | **95.35%** (kept) |
| 2026-10-08 | Haiku agent | same as above, index as `(*(unsigned int *)&node->field_10 & 0xff)` | 63.41% |
| 2026-10-09 | Opus 5.5 | index as `(*(volatile unsigned int *)&node->field_10 & 0xff)`: a fresh full-dword load in each branch like the original (int/long/unsigned views get CSE'd with the test load, short gets narrowed) | **100%*** (only the OverlayILF/field load order swapped) |

## Ideas not tried yet

- Get a dword index load without CSE against the int test, e.g. a read through a different pointer type
  (`*(int *)((char *)node + 0x10)`), or reading the index before the test.
