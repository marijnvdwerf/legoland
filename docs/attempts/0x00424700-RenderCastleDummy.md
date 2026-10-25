# RenderCastleDummy (0x00424700, `src/legoland/castle.c`)

**Best: 88.41%**. Kind: C.

## What still differs

- Register allocation in the prologue after the `FUN_00425cb0` call: the original keeps `want` in `esi` (loaded from
  `[esp+0x84]` after the loop-entry `test`) and the loop count in `edx` (loaded from `DAT_00610a08` after the key stores).
  Ours keeps the loop count in `esi` and reloads `want` from memory inside the loop.
- The original loads `p[1]` before `p[0]` into `dx`/`cx`, then stores `key[1]` before `key[0]`.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku agent (a1) | Start: `key[0] = p[0]; key[1] = p[1];` | 88.41% |
| 2026-10-08 | Haiku agent (a1) | Swap the key stores to `key[1] = p[1]; key[0] = p[0];` | 85.51% (worse) |

## Ideas not tried yet

- Declare the loop count as a local read after the key stores (e.g. `int n = DAT_00610a08;` placed after `want`) to pull
  the global load after `want` and change which register holds the count.
- Compare with an unsigned loop count to see whether the `jle` in the original needs the signed cast kept.
