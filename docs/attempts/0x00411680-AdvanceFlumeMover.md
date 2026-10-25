# AdvanceFlumeMover (0x00411680, `src/legoland/log_flume.c`)

**Best: 63.04%**. Kind: C.

## What still differs

- Stack frame: the original reserves 12 bytes of locals, with `flag` in memory at `[esp+0xc]`, `rev` in `ebx` and `shape` in `edi`. Ours reserves 8 bytes, keeps `flag` in `ebx`, `rev` in `edi` and `shape` in `eax`.
- The original's fallback path reads an uninitialised `r` from `[esp+0x14]` and `[esp+0x18]`; ours doesn't.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 r2 | Haiku agent | Reordered the declarations | no change |
| 2026-10-07 r2 | Haiku agent | Took the address of `flag` to force it into memory | no change |
| 2026-10-08 | permuter + Opus 5.5 | `volatile int flag`: frame now 0xc and flag kept in [esp+0xc] like the original | 40.52 -> 41.56 |
| 2026-10-08 | permuter + Opus 5.5 | on top of volatile: register shape, shape = NULL as a statement, flag = (int)shape, next compared with shape, rev = 0 as a statement - the original keeps shape (0) in ebx from entry and uses it for flag = 0 and the NULL test | 41.56 (no change) x5 |
| 2026-10-08 | permuter + Opus 5.5 | new permuter (graded register/stack score): rev and flag assigned as statements, volatile sub and volatile next_y (tile.pos.y), register TileId tile, f18 + f20, operand flips. Replaces the volatile flag hand fix | 41.56 -> 63.04 |

## Ideas not tried yet

- Make `r` a real (uninitialised) local struct/pair that the fallback reads.
