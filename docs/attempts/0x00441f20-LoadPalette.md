# LoadPalette (0x00441f20, `src/legoland/render3d.c`)

**Best: 81.00%**. Kind: C.

## What still differs

- Successful-path layout: the original computes the 565 value in dx and stores it inside its own branch (`mov word ptr [edi], dx; jmp`). Ours computes both branches in eax and merges the store, so the 555 and 565 branches share one `mov word ptr [edi], ax`.
- The 565 branch loads `path` as a byte (`movzx dx, byte ptr`) in the original, but a dword in ours.
- Failure branches (`je`) jump to a shared return block in the original; offsets differ by a few bytes.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku agent | `(unsigned char)path & 0xf8` in the 565 branch | 76.33% (branch layout flips, worse) |
| 2026-10-08 | Haiku agent | Separate `unsigned char r` read buffer for the red channel, both branches | 74.40% (stack layout changes, worse) |
| 2026-10-08 | Haiku agent | `(unsigned short)` cast on the 565 store expression | 81.00% (no change) |
| 2026-10-09 | Opus 5.5 | per-channel `(unsigned short)` casts in both branches (LoadColourTable style); 565 red mask as `~7`, `-8`, `0xfff8`, `~7u`, via an `unsigned short`/`int` red local | 76.85 (our 565 branch now equals the original 555 branch exactly; MSVC always narrows the red mask to a byte `and 0xf8`, the original keeps `movzx dx; and edx,-8`) |

## Ideas not tried yet

- Write the 565 expression in the same operand order as the original (`g & 0xfc` first, then `b >> 3` last) and check whether the store stays in its own branch.
- Restructure with an explicit `ok`-style flag for the success path.

## Notes

No earlier round notes for this function.
