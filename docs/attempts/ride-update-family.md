# Ride update functions: shared frame signature (2026-10-24, Opus 5.5)

SafariRideUpdate, CarouselUpdate, PlaneRideUpdate, SpinningBarrelsUpdate, SpaceTowerUpdate and TempleSlideUpdate all have
a bigger original frame than ours (+4 to +12 bytes; `framescan.py` in the agent scratch compares every non-100
function's `sub esp`). Two causes, the same in every one:

1. The BNV start-position arrays are `int coords[3]` / `int coords2[3]` in the original (index 2 never touched).
   SpinningBarrels declares them `[2]`: `[3]` gives the original's frame minus 4 (score unchanged until cause 2 is fixed).
2. The boarding case computes `(x - y) * tw >> 9` into a register and also writes it once, dead, into the stack slot of
   the tile/node pointer (Safari -52, SpinningBarrels -56, TempleSlide -48), and the 4 bytes above that slot are never
   referenced. That is an 8-byte `struct Point` (e.g. SpiderRideUpdate's `off`, whose `.x` gets the same dead store at
   -32 and whose `.y` is never stored) packed into the pointer's slot. VC6 only writes `.x` through because the struct
   has a memory home; SpiderRideUpdate is the one member where the Point got its own slot instead of being packed.

Not reproduced yet: with a `struct Point off` in Safari (spider form, block-scoped dx/dy, either coords order) VC6
scalar-replaces off completely (no frame home, harness 43-44 because every slot shifts). In SpiderRideUpdate our off
keeps a home only because off.y is spilled. Self-assignments (`tw = tw;`, `(void)th;`) do not change the frame order
(it is decided after optimisation). Next idea: find what gives off a memory home (a second use in another region, as in
RunAppraisal's write-through variables) and then what makes VC6 pack it with the tile pointer.

## 2026-10-24 Opus 5.5, second pass (no change)
- Synthetic VC6 tests (a stand-alone ride-update skeleton compiled with /FAs): neither a `__int64`-returning inline
  helper for the playfield offset (`*(__int64 *)&off = MapToPlayfield(px, py)`), nor a `union { __int64; struct { int lo,
  hi; } }` local, nor an `__int64 t` gives the Point a memory home packed with the tile pointer.
- TempleSlideUpdate: every placement of tw/th/t/px/py at block or function scope (32 masks x 2 product orders), dead
  stores of a field/call/product/address, dropping the `tile` local, and a `person` local: no frame change. One real
  gain: computing the x offset straight into `x` with `x += view_x - sx` (81.22 -> 81.72, landed).
- Pairwise merges of same-typed locals and block-to-function hoists (slotsweep) on Safari, Carousel, Plane, Spinning
  Barrels and Spider: all tie.
