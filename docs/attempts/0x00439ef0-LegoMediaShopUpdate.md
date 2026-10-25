# LegoMediaShopUpdate (0x00439ef0, `src/legoland/shops.c`)

**Best: 80.11%**. Kind: C.

## What still differs

- The original shares the tail of `case 0` and `case 4`: `case 0` does `or byte [esi+0x62], 8` and falls into the `x = (x-2) << 8` block at offset 0x6c (jump table entry 4 is `start + 0x6c`). Ours has a separate `case 4`.
- The jump-table targets for cases 2 and 3 are 5 to 6 bytes further in ours, which shifts every later jump by a few bytes.
- The `rand() % 3` branches (cases 1 and 3) use a different branch order from the original (`cmp dl, 2` then `cmp dl, 1`, with the `== 2` arm placed before `== 1` in the original).

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku agent (a5) | `case 0:` falls through (`/* fall through */`) into a shared `case 4:` body (the original's tail sharing) | 43.30% (much worse) |

## Best

80.11%, the version currently in the file (unchanged).

## Ideas not tried yet

- Fall through the other way: keep `case 4` as the first body and put `case 0`'s `flags |= 8` in front of it with `if (param_action == 0)`, so the tail is shared without changing the switch shape.
- Swap the `== 2` and `== 1` arms in the case 1 and case 3 `rand()` chains.

## Additional source-shape checks

Refreshed current main before scoring: harness 81.07, full verify 80.11.
Changing case 5 to assign dest.x directly, assigning case 1/r==1 through x,
or combining the two changes all tie at 81.07. Duplicating the movement call
inside the three random-position arms moves the shared tail to the wrong
block and drops to 56.64. None retained. An old fn.c was stale and initially
scored 56.64, so do not trust it as the repo baseline.
