# Remaining work (as of 2026-10-07, after the inline-asm round)

`./tools/verify` reported `Progress: 95.96%` for this build. That figure is the sum of every function's match score divided
by all 3634 functions reccmp counts, which include CRT and import entries. Counted per game function (`// FUNCTION:`):

| State | Functions |
|---|---|
| 100% (exact or "effective") | 3002 |
| Pure-C partials (1-99%) | 246 |
| Inline asm, partial | 7 (see below) |
| Still `STUB()` (0%) | 0 |
| Total | 3255 |

Recount with `uv run tools/agent/status.py` (per function) and `uv run tools/agent/partials.py` (pure-C partials).

## Inline-asm functions (the former `STUB()`s, matched 2026-10-07)

The 44 functions that were hand-written assembly in the original, and so stayed `STUB()` while the project was
pure C, are done. `find_inline_asm` in `tools/progress.py` detects such functions, and `partials.py` leaves them out.

- **C with small `__asm` blocks, 100%:** FUN_004236f0, FUN_00423730 (FPU control word) and FUN_004292f0 (two
  `rdtsc` timing blocks).
- **Transcribed whole as `__declspec(naked)`, 100%:** the other 39 in castle, draw, render, render3d and man3d.
  For SetPersonRotation, FUN_00441980 and FUN_0043fa80 a C + `__asm` version got the structure and every
  instruction right, but MSVC6 kept picking different registers or stack slots around the asm.
- **Were reported < 100% because of a reccmp false positive, now 100%:** FUN_00440a30 and the older naked
  FUN_00426980, FUN_00426ab0 load plain constants (0x500000, 0x5a0000, 0x7fffff) that fall inside a data symbol
  in only one image; `tools/reccmp_fixes.py` now counts such byte-identical lines as equal. FUN_00440a30 also
  had three raw original addresses (0x641004, 0x643ef0) left in its asm by an early asm2naked; regenerated.
- **Two more found later, same blitter family:** FUN_00467640 and FUN_00467f00 (draw.c) were C attempts at 10%
  and 5%; they have the blitters' prologue and `rol` (which `find_inline_asm` doesn't flag, since `_rotl` can
  emit it). Transcribed with `tools/asm2naked.py`, 100%.
- **Not matched:** CoptersPlaceRider (FUN_00404630, copters) at 54%, C with the `fistp` macro in `__asm`. It has a `switch` jump
  table, so it cannot be naked; what is left is a register permutation (the original keeps entry/layer/sprite in
  edi/ebx/esi and spills entry to the `node` slot).

Inline asm, partly written in C (finish with `__asm`): Render3DPerson 51.6%, RenderTransSprite 18.8%, SoftPrint_Clear 18.9%,
FUN_00488730 17.5%, ApplyObjectOrientationToPerson 7.0%; HASM_lego_sqrtf and HASM_lego_invsqrtf 95.2% (`__declspec(naked)`),
LLSPlay 16.2% (`int 3`). Inline asm already at 100%: _ftol, RenderingComplete, ReadBigEndianU32, ReadBigEndianU16.

## The untried partials: round 6 (2026-10-05)

All 47 functions on the round-5 "not attempted yet" list (plus FUN_0042a020 and FUN_00485fe0) got one pass of up to three
variants each. `./tools/verify` went from 94.72% to 94.76%; 2961 game functions now match.

Matched (100%): CoptersUpdate (dropped the two `volatile` locals, own local for the queue index), RES_OpenFile and
RES_OpenFileFromVolume (pointers set before the first call, `while` loop for leading `.\\`, refcount through
`file->volume`).

Improved: LegoShop2Update 46.9 -> 71.6 (y computed before x), RenderBuildObjectIcon 81.7 -> 83.7, PrintCertificate
79.4 -> 81.2, DrawNewObjectPopup 62.2 -> 63.7 (inlined icon-placement helper), FindPathPosAtRangeAhead 73.0 -> 74.5,
PlaneRideUpdate 78.9 -> 79.1, LogFlumeEntranceAddObject 61.7 -> 62.1.

FUN_00485fe0 is inline asm (ebp frame + `rep stosd` clear block) that `find_inline_asm` misses: match it with `__asm`.

Named (28, all scores unchanged by the rename): the per-frame ride callbacks (`cb_a8`) PlaneRideUpdate, LogFlumeEntranceUpdate,
DrivingSchoolUpdate, TempleSlideUpdate, SafariRideUpdate, ExplorersInstituteUpdate, LegoShop2Update, SpaceTowerUpdate,
SpinningBarrelsUpdate, OctopusCafeUpdate, Restaurant2Update, JoustUpdate, FortUpdate; FortWanderUpdate, AdvanceFlumeMover,
EdgeMeshRemoveBackfaces, JungleCruiseBuildStepPath, BoatingSchoolBuildStepPath, GetQueueTurn, PickQueueTurn,
FindPathPosAtRangeAhead/Behind, FindMapPathAStar, FileSelectDialog, ListBoxDialog, SetPersonYawFromDir16,
SearchBoatPathConnected, RemapTexCoordsToCell.

Still partial, with what blocks them (no further attempts unless there is a new idea):

| % | Function | What differs |
|---|---|---|
| 95.48 | RenderCursor | two values in different registers only |
| 95.06 | EdgeMeshRemoveBackfaces | x87 operand order (orig keeps dx1/dy1 on the FPU stack) |
| 83.61 | JungleCruiseBuildStepPath | straight case placed first via a ternary; rewrites scored lower |
| 81.26 | BoatingSchoolBuildStepPath | same as the jungle version |
| 79.72 | GetQueueTurn | orig keeps three separate fallback calls; merging the tests makes ours tail-merge |
| 78.63 | LogFlumeEntranceUpdate | ride in ebx/stack instead of esi |
| 77.47 | FortWanderUpdate | node kept in edi for the whole function, param slots reused |
| 76.96 | DrivingSchoolUpdate | constant 7 kept in ebx |
| 76.47 | FortUpdate | tile x kept as an int in ebp |
| 75.35 | TempleSlideUpdate | node kept on the stack |
| 75.09 | SafariRideUpdate | register allocation |
| 74.30 | SpinningBarrelsUpdate | frame 12 bytes larger, ebx/ebp swapped |
| 72.33 | ExplorersInstituteUpdate | ours merges case 0 into the identical case 3 |
| 70.04 | FindMapPathAStar | point/result kept in dead parameter slots (would need `(&a)[1]`, UB in the port) |
| 69.93 | ParseScriptResFile | `cmd` in ebp, reloaded from the parameter at the loop end |
| 69.44 | ListBoxDialog | ebx/edi swapped, -1 kept in esi |
| 69.03 | FUN_00402780 | scheduling around the inlined MapToPlayfield |
| 67.67 | FUN_00423a10 | two zero registers, different local slots |
| 67.51 | SpaceTowerUpdate | ride re-read from the stack |
| 63.32 | SetPersonYawFromDir16 | `&field_40` in esi and 0 in edx across the switch |
| 61.81 | PickQueueTurn | `rand() & 3` in ebx, t1..t3 on the stack |
| 61.48 | LoadPos | frame pointer strength-reduced to `&f->mat` |
| 60.26 | PrintProfileDetails | 0x400 kept in ebp |
| 56.52 | SearchBoatPathConnected | tx/ty in registers, `found` reloaded |
| 54.66 | FileSelectDialog | dir/fname/ext buffers in another stack order (declaration order and names have no effect) |
| 41.17 | RemapTexCoordsToCell | FPU-heavy, not attempted |
| 40.52 | AdvanceFlumeMover | our build turns the `if (sub == N)` chain into jumps to the end |
| 38.41 | SearchJunglePathConnected | same as SearchBoatPathConnected |

Not attempted this round (large): LoadBaseMap 82.0, RenderFullMap 81.6, JoustUpdate 69.0, FUN_00466770 61.5,
OctopusCafeUpdate 60.9, FUN_004608c0 56.5, FUN_0040ae90 45.1, Restaurant2Update 41.9.

Patterns that worked this round: statement order matters around calls (initialise pointers where the original does, before
the first call); remove `volatile` workarounds left from earlier rounds and re-score; when the original's registers for two
values are swapped, try computing them in the other order.

Also not attempted, but triaged as unlikely to match from C (`agent-workflow.md`, round 5): 37 layout-dependent and
8 register-only partials.
