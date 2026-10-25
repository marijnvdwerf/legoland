# ObjectIsBuilt (0x0045ed30, `src/legoland/map_object.c`)

**Best: 100% (matched 2026-10-09)**. Kind: C.

## What still differs

- Stack slots: the original passes `&source` at `esp+0x14` where ours uses `esp+0x1c`. The original keeps
  `out` at `+0x2c` and reloads `source.field_0` from `+0x18` (ours `+0x20`). The frame size (0x4c) matches.
- Declaration order and block scope of the loop variables do not change the frame layout.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku 5.5 agent | `source` declared before `pos` | 93.42% (same) |
| 2026-10-07 | Haiku 5.5 agent | loop variables (`cfg`, `next`, `rect`, `x`, `y`, `tx`, `ty`, `tile`) moved into a nested block | 93.42% (same) |
| 2026-10-07 | Haiku 5.5 agent | `out` declared before `pos` | 93.42% (same) |
| 2026-10-09 | Opus 5.5 | `source` in its own block around the UnSource call; the saved cursor tile is a separate `struct Point saved_tile` (it used to reuse `source`) | **100** |

## Ideas not tried yet

- Block-scope `source` and `out` into the `GetTileCentre` / `UnSourceAndFadeAllSamplesFromSource` section only.
- Read `saved_rect` through a `struct FootprintNode *` temp rather than a struct copy.
