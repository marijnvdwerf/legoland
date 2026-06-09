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

- **`port2/project/c/0x00<ADDR>.c` is the START for every function — read it FIRST.**
  Filename is UPPERCASE hex: address `0x0041cc50` → `port2/project/c/0x0041CC50.c`. These
  are already byte-matched in port2, so the logic is correct — you are **translating and
  cleaning** (raw Ghidra names, `pad[0x14]`, `uVar1` → real types, named locals, real
  structs), **not reverse-engineering**. Do not write a body from scratch.
- **Do NOT call Ghidra to derive a function body.** The port2 C already has it. Ghidra MCP
  (see `../ghidra/CLAUDE.md`) is a last resort *only* to look up a single struct field
  name/type when the port2 draft is genuinely unclear — and even that is optional. Re-deriving
  from disassembly when a byte-matched port2 draft exists is wasted, off-task work.
- The original asm is the ground truth; the asm diff from `verify -v` is what you tune against.

## House style

- **Symbol names stay canonical.** Use the `name` from `ghidra/functions.csv` (a real
  name like `GetGameTimer`, or `FUN_<addr>`). Do **not** rename exported functions or
  globals yet — other TUs reference them by that name, and there is no shared
  declaration header to coordinate a rename. Renaming is a later, coordinated pass.
- **Clean everything else:** real types (`uint32_t`, not `unk32_t`); named locals
  (not `uVar1`); real `struct` definitions with named fields when you understand the
  layout (not `pad_0[0x14]`); correct signatures and calling conventions.
- **Comments: the only comments allowed are reccmp annotations** — `// FUNCTION:`,
  `// GLOBAL:`, and `// STRING:` — **plus struct-field offset comments** (`/* 0x18 */`,
  see project-owner conventions below). For a pooled string constant, put `// STRING: LEGOLAND
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
- **Calling a not-yet-integrated function with arguments — NO function-pointer casts.**
  When a function you're integrating calls one that's still a `STUB()`, give the call its
  real signature the clean way, never `((ret(*)(args))FUN_x)(...)`:
  - **Callee in *another* TU:** `extern <ret> FUN_<addr>(<args>);` at the top of your file.
    Leave the other TU's stub alone — the linker doesn't check signatures, so it still
    links and matches.
  - **Callee in the *same* TU (e.g. an off-limits stub in your range):** change that
    stub's signature in place to the real one, keeping the `STUB();` body —
    `void FUN_x(void) { STUB(); }` → `<ret> FUN_x(<args>) { STUB(); }`. This is the one
    allowed edit to an off-limits function; do not give it a body. Then call it normally.
    (A non-void stub will warn `-Wreturn-type` — that's fine.)
- **Imports (Win32/DirectX):** include the real MSVC6 header (`#include <windows.h>`,
  `<ddraw.h>`, …) and call the API normally; the import libraries resolve it at link.
- **One `// STRING:` annotation per address, globally.** MSVC6 pools identical string
  literals, so a string referenced by several functions exists once in the binary. reccmp
  wants it annotated exactly once — a second `// STRING: LEGOLAND 0x<addr>` for the same
  address (same file or another TU) triggers `[ERROR] Dropped duplicate address … on STRING
  annotation`. Before adding a `// STRING:`, grep the tree for that address; if it already
  has one, just write the literal with no annotation.
- **CRT mem/str helpers (`memset`, `memcpy`, `strlen`, `strcpy`, …):** declare them in
  `crt.h` (NOT `#include <string.h>`) so we control exactly what's visible, and `#include
  "crt.h"` in the caller. A visible prototype does **not** make MSVC6 `/O2` intrinsify
  these to inline `rep movs`/`stos` — verified empirically: every caller (including
  constant-size `memcpy(dst, src, 84)` / `memset(p, 0, 0xac)`) stays byte-identical with
  the prototype present, keeping the `call _memset`/`call _memcpy` form. The tree has ZERO
  implicit function declarations. (If you ever add a helper whose prototype *does* de-match
  a function under reccmp, leave that one implicit and record the asm reason.)

## Project-owner conventions (apply to all new + cleaned code)

These refine the house style above; where they conflict, these win.

- **Shared structs live in shared headers.** When 2+ TUs use the same layout, define
  the `struct` once in the owning `<tu>.h` (or `globals.h` for cross-cutting types)
  and `#include` it — never redefine per-TU. Merging existing duplicate struct
  definitions into shared headers is an active goal.
- **Prefix every struct field with its byte offset:** `/* 0x00 */ char name[8];`.
  This is the one explanatory-comment exception — offset comments don't affect
  codegen and make the layout auditable. Keep them accurate.
- **`malloc`/`calloc` sizes use `sizeof(struct Foo)`**, never a magic byte count
  (`malloc(0x2c)` → `malloc(sizeof(struct Foo))`; a count*stride array →
  `n * sizeof(struct Foo)`). The struct's size must equal the original allocation.
- **No pointer↔int casts.** A value that is a pointer should be *typed* as that
  pointer. A function that returns a pointer should be declared returning the pointer
  (returning `int`/`unsigned int` and casting at the call site usually matches
  identically — prefer the pointer type). Don't cast a pointer to `int` to pass/return/store it.
- **`pointer + offset` access becomes a struct member.** `*(int *)((char *)p + 0x18)`
  → `p->field_18` (name the field when known); extend the struct rather than
  hand-computing offsets.
- **The only cast that should survive is `(struct Foo *)malloc(...)` at an allocation
  site.** Aim to eliminate every other cast by giving values their real type. In
  particular: a global/struct-field/local that holds a heap pointer is typed `void *`
  (or the real struct pointer) — then `free(p)` and `p = malloc(...)` need no cast and
  no `(void *)`/`(unsigned int)` round-trips. Fix the *type at the source* (the
  function signature, the global, the field) and let it propagate to all callers; a
  pointer↔`unsigned int` retype is byte-identical on x86, so the match holds — don't
  flinch because it ripples to many files.
- **No internal "fake" duplicate struct views.** Don't define a private `struct XRec`
  in a `.c` that re-describes the same object a shared struct already models — add the
  needed fields (offset-commented) to the *real* shared struct and use it. At most one
  genuine reinterpret cast is allowed where a field is a tagged union (e.g. a `Sprite`'s
  image pointer reused as a group list when a flag is set).
- **Keep a subsystem's internal representation private.** Encoding details (e.g. an
  id's page/slot split, a table's layout) live in the owning `.c`, not its public
  `.h` — other TUs reach data through accessor functions, not leaked macros. An
  out-param that yields a pointer is `T **`, never `int *`/`unsigned int *` holding an
  address-as-int.
- **Suspect "byte-correct but semantically fake" matches.** A match that needs odd
  `stream + offset` casts or a `#pragma intrinsic` a real developer wouldn't write is
  a red flag — prefer natural C even at equal score, and flag such functions for
  re-audit (e.g. `FUN_00498d00`).

## Gotchas (from real TUs)

- **Keep every function signature on ONE line.** reccmp's source parser drops a function
  with a multi-line signature (it gets stuck waiting for the `{`) — AND silently swallows
  the functions after it until it recovers. They show as "Failed to find a match". Long
  parameter lists go on a single line, even if it's wide.
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
- **Shared structs go in shared headers** (see project-owner conventions below) — a
  struct used by 2+ TUs is defined once in the owning `<tu>.h`/`globals.h`, not
  redefined per-TU.

## Done criteria for a TU

1. Every function in the TU has a real body (no `STUB()` left — `grep -n 'STUB()' src/legoland/<tu>.c`).
2. `./tools/verify` shows **100%** for every address in the TU (`verify -v <addr>` per function).
3. The whole project still builds and the aggregate did not regress.
4. Code is clean per the house style above.
