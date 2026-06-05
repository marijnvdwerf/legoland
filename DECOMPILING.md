# Decompiling a TU

The unit of work is one **translation unit** (a `src/legoland/<tu>.c`, grouped by the
`tu` column of `ghidra/functions.csv`). You take it from all-`STUB()` to clean,
idiomatic C where **every function is a 100% reccmp match**.

## The loop

```sh
cmake --build build              # compile + link (wibo + MSVC6)
./tools/verify -v 0x004XXXXX     # asm diff for one function
./tools/verify                   # whole-binary aggregate
```

For each function in the TU: write C → build → `verify -v <addr>` → read the asm diff
→ adjust types/order/locals → repeat until `100% match`. Then move to the next.

## Only do functions port2 already matched

Only integrate functions port2 already got byte-perfect — `current_score == 0` in
`port2/project/functions.csv`. Leave everything else as `STUB()`. **Do not try to match
an unmatched function from scratch** — that's slow, separate work, not for these runs.

**Don't get stuck.** A few port2-matched functions still won't reach 100% under reccmp
(objdiff hid relocations reccmp checks — e.g. an unpairable `call`, or reccmp prints
"Failed to find a match"). After a few honest attempts on one function, revert it to
`<its signature> { STUB(); }` and move on. Never loop on a single function.

## Reference material (not the deliverable)

- `port2/project/c/0x<ADDR>.c` — AI first-drafts; ~1,543 already byte-match. Great
  starting point, but they are raw (Ghidra names, `pad[0x14]`, `uVar1`). **Clean them.**
- Ghidra MCP (see `../ghidra/CLAUDE.md`) — for naming/struct context on the matched ones.
- The original asm is the ground truth; the asm diff from `verify -v` is what you tune against.

## House style

- **Symbol names stay canonical.** Use the `name` from `ghidra/functions.csv` (a real
  name like `GetGameTimer`, or `FUN_<addr>`). Do **not** rename exported functions or
  globals yet — other TUs reference them by that name, and there is no shared
  declaration header to coordinate a rename. Renaming is a later, coordinated pass.
- **Clean everything else:** real types (`uint32_t`, not `unk32_t`); named locals
  (not `uVar1`); real `struct` definitions with named fields when you understand the
  layout (not `pad_0[0x14]`); correct signatures and calling conventions.
- **Comments: the ONLY comments allowed are reccmp annotations** — `// FUNCTION:`,
  `// GLOBAL:`, and `// STRING:`. For a pooled string constant, put `// STRING: LEGOLAND
  0x<addr>` directly above the **source line that contains the string literal** (inside the
  body — NOT above `// FUNCTION:`; a STRING marker above FUNCTION makes reccmp drop the
  function). One per literal; write the literal normally (`"PATH CONTROL"`) — it compiles
  now. Do **not** write explanatory comments about variables, types, or logic — names only.
- **Annotations:** every function keeps `// FUNCTION: LEGOLAND 0x<addr>` directly above
  it. The address is how reccmp matches — never drop or change it.
- **Globals** go in `src/legoland/globals.c`, one definition each, with
  `// GLOBAL: LEGOLAND 0x<addr>` above them (reccmp needs this to match data refs).
  Name them `DAT_<addr>` for now (canonical), real type.
- **Cross-TU calls / data:** declare what you use with `extern` at the top of the TU
  file, by canonical name. The callee already exists as a stub in its own TU, so the
  link resolves and reccmp matches the call by symbol. Confirm the target's address/name
  in `ghidra/functions.csv`.
- **Imports (Win32/DirectX):** include the real MSVC6 header (`#include <windows.h>`,
  `<ddraw.h>`, …) and call the API normally; the import libraries resolve it at link.

## Gotchas (from real TUs)

- **Missing functions / inventory gaps.** `ghidra/functions.csv` is not complete. If a
  function `call`s an address that has no `// FUNCTION` annotation and isn't in the CSV
  (often a small thunk in the gap between two listed functions), it's a real function
  the inventory missed — add it to the right TU with its own
  `// FUNCTION: LEGOLAND 0x<addr>` and decompile it too. (`GetTicks` at `0x00499450`
  was found this way.)
- **Imports vs. thunks.** Including a Win32/DirectX header makes that API
  `__declspec(dllimport)`, which forces *every* call to it in the TU to the indirect
  `call dword ptr [__imp_X]` (`ff 15`) form. A relative `call` to a **lone**,
  16-byte-aligned, NOP-padded `jmp dword ptr [__imp_X]` sitting in source order among a
  module's functions is a **real one-line wrapper** (`return X();` tail-call-optimized)
  — reproduce it as its own function with its own `// FUNCTION` and call it. (`GetTicks`
  at `0x00499450` is one.) Do **not** confuse it with the linker's own import-thunk
  *tables*: dense packed runs of `jmp [__imp_*]` grouped away from user code (e.g.
  `0x49d050`, `0x49e3a0`) are linker-generated, not source — don't hand-write those.
- **Structs are per-TU for now.** Define the struct you need locally in the TU; a
  shared, consolidated struct header is a later coordinated pass.

## Done criteria for a TU

1. Every function in the TU has a real body (no `STUB()` left — `grep -n 'STUB()' src/legoland/<tu>.c`).
2. `./tools/verify` shows **100%** for every address in the TU (`verify -v <addr>` per function).
3. The whole project still builds and the aggregate did not regress.
4. Code is clean per the house style above.
