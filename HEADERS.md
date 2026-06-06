# Header refactor — conventions

Replacing the scattered inline pre-declarations (cross-TU `extern`s and same-TU
forward decls) with proper per-TU headers, **without changing any compiled byte**
(`./tools/verify` must stay at its current aggregate — no function may drop).

Worked TUs so far: tooltip, binv, setcustomcallbacks, certificate, main, debug,
money, path_control. Process **smallest TU first** (see `tools/defmap.py`).

## Per TU `T`
- Create `src/legoland/T.h` (`#pragma once`, MSVC6-supported) with a plain prototype
  for **every** function defined in `T.c` (functions are found via their
  `// FUNCTION:` annotations — the next line is the signature). No reccmp annotations
  in headers. If a prototype needs a struct, put that struct's definition (or a
  forward `struct X;`) in `T.h`; if the struct was defined locally in `T.c`, **move**
  it into `T.h`.
- `T.c` `#include "T.h"` and delete its own internal forward declarations.
- Every other `.c` that inline-declares one of `T`'s functions: replace the inline
  `extern` with `#include "T.h"`. Find them: `grep -rn '<fnname>' src/legoland/*.c`.

## Canonical signature — one per function, no exceptions
- When the definition and a caller disagree, the **caller's signature is usually more
  correct**, especially when `T`'s function is still a `STUB()` whose definition reads
  `void FUN_x(void)` but callers pass real arguments. Pick the most type-informed
  signature; update the definition (and stub signature) AND all call sites to it.
- **No divergent externs may survive.** No `// differs elsewhere` escape hatches.

## Prefer merging types over CASTING — casts are a last resort
- If the impl took `int`/`void*` and a caller passes a real `struct Foo*`, the truth is
  `struct Foo*`: **fix the type at its source.** Retype the offending local — or the
  `DAT_xxxx` global (in `globals.c` AND its per-TU `extern`s) — to the real pointer/
  struct type, and merge the struct into the owning header. Then the value flows with
  no cast. (A 32-bit `unsigned int` ↔ 32-bit pointer retype is byte-neutral, so the
  match holds — verify it does.)
- Only keep/add a cast when a value **genuinely is a different type** in that context
  (e.g. a global truly used as both an int and a pointer in different places). That is
  rare; justify it. Do not paper over a type mismatch with a cast by default.
- Globals are otherwise out of scope (leave `extern DAT_xxxx` alone) — the exception is
  retyping a handle global specifically to eliminate a cast.

## Gate (after EVERY TU)
`cmake --build build` GREEN (warnings ok) and `./tools/verify` aggregate unchanged,
no function dropped. If something de-matches it's a signature/type mismatch — fix it;
never revert to a divergent extern, never leave a broken build.
