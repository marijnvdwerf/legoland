# RenderFullMap (0x004567a0, `src/legoland/mapscreen.c`)

**Best: 94.07%**. Kind: C (a switch with a jump table: asm2naked can't transcribe it).

## What still differs

- Stack-slot assignment of the locals. The six `ElemID` results and the three LoadSprite results sit in different frame slots than in the original (e.g. square_track at esp+0x88 in ours, esp+0x8c in the original; square_track_height at esp+0x9c in ours, esp+0xb4 in the original; stick at esp+0x98 vs esp+0x9c). Frame size (`sub esp, 0xf8`) already matches.
- The 15 diff hunks run through the whole function (about 0x4567a0 to 0x457850); the first ones are the stack-slot differences, the later ones are the rendering loop and the terrain pass.

## Tried (don't repeat)

No attempts recorded yet (first session: baseline measured at 81.60%).

## Ideas not tried yet

- Declare the ElemID result locals in a different order (or in nested blocks) to see which order reproduces the original slot assignment.
- Check which locals are only live in the first block, since the original shares slot 0x8c between square_track and square_track_height_path.

## Notes

No earlier round notes for this function.

| 2026-10-08 | permuter + Opus 5.5 | many temps (bounds_y1/y2, dest_x, uid_x, ...), `register` pen/base_x/x1, loops as while, nested ifs, unsigned py/x2 (permuter 84.5% included a y1/y2 bug: expression dropped) | 81.60 -> 81.85 |
| 2026-10-11 | Opus 5.5 | `volatile int hx` in the layered-sprite loop (per-local volatile search): it fixes the stack-slot order of the whole function (hx needs more weight). Natural forms tried first: hx/hy before GetTileBounds (79.59 / 87.67), `hx = sub_x; hx = HALF(hx);` styles (87.99) | 87.99 -> 93.98 with volatile (kept, last resort) |
| 2026-10-17 | Opus 5.5 | hoisting block locals to the outer block (so they can't share slots): `struct MapPoint off` 84.17, `RideSpriteInfo loc` 94.07 (equal), `origin[4]` + `base` 86.26, `piece` + `dest` 85.70, off + loc 84.17, one function-wide `struct Point pt` 74.78 (with off hoisted 74.44), `height` + `dest_height` 85.82 | none better |

Note (2026-10-17): the last diff hunk (the jump table's index table, 4 entries longer in ours) is reccmp cutting the
original's table short: read from legoland.exe, the original's index table is the same 0x31 bytes as ours (case 0x30
-> the light green block). Only the frame (`sub esp, 0xf8` in the original, 0xf0 in ours) and slot order differ.
