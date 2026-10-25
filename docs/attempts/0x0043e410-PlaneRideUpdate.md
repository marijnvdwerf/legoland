# PlaneRideUpdate (0x0043e410, `src/legoland/plane_ride.c`)

**Best: 79.57%**

## What still differs

(not analysed yet)

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | permuter + Opus 5.5 | `register TileId *pos`, flags |= 0x80 after the coords2 stores, constant-first compares | 79.09 -> 79.57 |
| 2026-10-09 | Opus 5.5 | inline map-to-screen math as `struct Point mp` shifted in place | 83.76 (worse than 88.26) |

## Ideas not tried yet

- 
