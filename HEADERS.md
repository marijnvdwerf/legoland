# Header refactor — conventions

Replacing the scattered inline pre-declarations (cross-TU `extern`s and same-TU
forward decls) with proper per-TU headers, **without changing any compiled byte**
(`./tools/verify` must stay at its current total — no function may drop).

Worked TUs so far: tooltip, binv, setcustomcallbacks, certificate, main, debug,
money, path_control. Process **smallest TU first** (see `tools/defmap.py`).

## Per TU `T`
- **Only MOVE pre-declarations that already exist — do NOT declare every function.**
  A function of `T` belongs in `T.h` ONLY if it was actually pre-declared somewhere:
  it's referenced from another `.c` (it had an `extern`), or it's used before its own
  definition in `T.c` (it had an internal forward decl). A function that is
  defined-and-then-called-in-order, with no `extern` anywhere, gets **no declaration**.
  Run `uv run --no-project tools/needsdecl.py <tu>` to get the exact KEEP set.
- **If `T` has no such functions, it gets NO header at all** — don't create an empty one.
- Create `src/legoland/T.h` (`#pragma once`, MSVC6-supported) with plain prototypes for
  ONLY the KEEP set. No reccmp annotations in headers. If a kept prototype needs a struct,
  move that struct's definition (or a forward `struct X;`) from `T.c` into `T.h`. A struct
  used only inside `T.c`'s bodies stays in `T.c`.
- `T.c` `#include "T.h"` (only if `T.h` exists) and delete the internal forward decls it
  replaced.
- Every other `.c` that inline-declares one of `T`'s KEEP functions: replace the inline
  `extern` with `#include "T.h"`. Find them: `grep -rn '<fnname>' src/legoland/*.c`.

## One signature per function, no exceptions
- When the definition and a caller disagree, the **caller's signature is usually more
  correct**, especially when `T`'s function is still a `STUB()` whose definition reads
  `void FUN_x(void)` but callers pass real arguments. Pick the most type-informed
  signature; update the definition (and stub signature) AND all call sites to it.
- **No divergent externs may survive.** No `// differs elsewhere` escape hatches.

## Fix the type, don't add a cast — casts are a last resort
- If the impl took `int`/`void*` and a caller passes a real `struct Foo*`, the truth is
  `struct Foo*`: **fix the type at its source.** Retype the offending local — or the
  `DAT_xxxx` global (in `globals.c` AND its per-TU `extern`s) — to the real pointer/
  struct type, and merge the struct into the owning header. Then the value flows with
  no cast. (A 32-bit `unsigned int` ↔ 32-bit pointer retype doesn't change the match
  — verify it doesn't.)
- Only keep/add a cast when a value **genuinely is a different type** in that context
  (e.g. a global truly used as both an int and a pointer in different places). That is
  rare; justify it. Do not paper over a type mismatch with a cast by default.
- Globals are otherwise out of scope (leave `extern DAT_xxxx` alone) — the exception is
  retyping a handle global specifically to eliminate a cast.

## Check after each TU
`cmake --build build` GREEN (warnings ok) and `./tools/verify` total unchanged,
no function dropped. If something breaks the match it's a signature/type mismatch — fix it;
never revert to a divergent extern, never leave a broken build.
