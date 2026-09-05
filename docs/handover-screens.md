# Screens handover

## Touched screens functions

| Address | Name | Final | Status |
|---|---|---:|---|
| 0x004585C0 | FUN_004585c0 | 100.00% | matched |
| 0x004589A0 | FUN_004589a0 | 91.11% | parked-with-reason |
| 0x00458A50 | FUN_00458a50 | 100.00% | matched |
| 0x00458EE0 | FUN_00458ee0 | 59.19% | parked-with-reason |
| 0x00459360 | FUN_00459360 | 59.57% | parked-with-reason |
| 0x00459520 | FUN_00459520 | 100.00% | matched |
| 0x00459710 | FUN_00459710 | 85.07% | parked-with-reason |

## Parked partials

- **FUN_004589a0 (91.11%)** — Target is a small setup routine with the existing short
  frame and ordered initialization/file-loading calls. Larger local reshuffles, explicit
  temporary variables, and alternate nesting did not improve it. Best hypothesis is one
  remaining MSVC6 scheduling/type distinction in the configuration and reset-call chain.
- **FUN_00459710 (85.07%)** — Target uses the compact edit-mode/global state layout and
  short branch sequence already represented. Merging globals into larger structs, splitting
  them back out, and changing signedness did not improve the match. Remaining difference
  is likely declaration-driven register allocation around the mode transition calls.
- **FUN_00459360 (59.57%)** — Target has a 0x2c frame, a copied HitInfo-sized local and
  map-screen/render calls in the observed order. Extra RECT locals (0x3c frame), private
  HitInfo definitions, and alternate local ordering all failed; shared HitInfo is retained.
  Best hypothesis is to recover the exact aggregate/local lifetime and compiler-generated
  address reuse rather than add more semantic code.
- **FUN_00458ee0 (59.19%)** — Target has a 0x1c outgoing-argument area plus RECT help
  local, early AI/process/render calls, and a dense 0x103..0x10c switch table. A hand-made
  byte table, two source switches, and explicit range/if restructuring were tried and are
  not retained; source now has one switch over input (with the separate 0x7e4 guard).
  The remaining best hypothesis is MSVC6 switch lowering/grouping and reserved outgoing
  argument stores; do not reintroduce the explicit table.

## Shared type fixes

- `ElemID` now returns `struct Element *`, exposing the real element record and removing casts.
- `HTBubbleHelp` and `FUN_00455fc0` now take `RECT *`, matching field-based rectangle use.
- `DAT_004bdd04` is `struct Bloke *`, matching visitor and ride APIs.
- `ObjClass.field_c4` is `unsigned int *`; `method_94` accepts that pointer for direct callback passing.
- `GetVisitorName` has the fixed one-argument `struct Bloke *` signature.
- `HandleRideAI` takes three fixed integer arguments to model its opening stack setup.
- `FUN_0047afb0` takes `const char *`, removing the profile filename cast.
- `DAT_00667c70` and `DAT_00667c74` are `long`, matching signed POINT-coordinate subtraction.

## General codegen facts

- MSVC6 can emit a byte index table for a dense switch range, with absent values mapping to
  the default body; source should not declare that compiler-generated table as a global.
- Calls may write arguments directly into the reserved `[esp+n]` area after `sub esp`; fixed
  signatures alone can make MSVC6 choose pushes instead, changing the opening bytes.
- Large aggregate copies commonly pull ESI/EDI and use `rep movsd`; local aggregate size and
  declaration order strongly affect register saves, frame offsets, and later codegen.

Final screen percentages are 100.00, 91.11, 100.00, 59.19, 59.57, 100.00, and 85.07
in address order above. `FUN_004731e0` is 64.24% (up from the 60.87% baseline),
`FUN_004723f0` remains 95.45%, and the full regression comparison found no lower function
score than `/tmp/verify-before.txt`. Clean build, needsdecl, and clang-format checks pass.
