# RenderTempleSlide (0x00416fa0, `src/legoland/temple_slide.c`)

**Best: 100%**. Kind: C.

## What still differs

- Stack slots of the two `struct Point` locals (`off1`, `off2`) differ. The original puts the `&off1` temp at `[esp+0x18]`
  and the `&off2` temp at `[esp+0x14]` before the call pushes. Ours puts them at `[esp+0x10]` and `[esp+0x1c]`, so every
  store and reload in the block shifts. The diff is 31 lines starting at 0x417027.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku agent | Original (current best): `off1` at function scope, `off2` inside the loop block | 95.00% |
| 2026-10-07 | Haiku agent | Move `off2` to function scope, after `off1` | 95.00% (no change in the slot layout) |
| 2026-10-07 | Haiku agent | Declare `off2` then `off1` at function scope | 95.00% (no change) |
| 2026-10-07 | Haiku agent | Move both `off1` and `off2` into the loop block | 95.00% (no change) |
| 2026-10-09 | Opus 5.5 | semantic fix: `person->offset += off2` (the DAT_004cbf88 offset), not off1 - read off the original once the pending pushes are counted | 95.00 -> **96.67**, struct 100; left: the screen sums add off1/off2 in the other order (all 24 term orders, groupings and declaration placements give the same code) |

## Ideas not tried yet

- A single `struct Point off[2]` array (changes the addressing, so it may give a different slot order).
- Reading `bloke->screen_x` and the `DAT_004cbf8*` globals into locals before the first `AdjustOffsetForViewMode` call.

## 2026-10-21 (Opus 5.5): still 96.67
The remaining diff is the operand order of the off1/off2 adds (the original adds off1 first in both sums).
| Change | Result |
|---|---|
| `struct Point off[2]` (both index orders) | 89.17-90.00 |
| off2 at function scope, off1 in the block | 96.67 |
| all locals at function scope, ~150 random declaration orders | 96.67 |

## 2026-10-24 (Opus 5.5): 100
The add-order diff was not the expression: TempleSlideDrawOffsetX/Y are one `struct Point`, and `off1 = TempleSlideDrawOffset;`
(a struct copy, like `off2 = SafariRideDrawOffset` in RenderSafari) gives the original's operand order. LoadTempleSlide reads
ZTempSlideSprite as the word after TempleSlideDrawOffset.y (`[f30*4 + 0x4cbfcc]`): written as `(&ZTempSlideSprite)[field_30 - 1]`,
with both globals initialised in globals.c so they stay adjacent in .data (otherwise LoadTempleSlide drops to 98.70).
