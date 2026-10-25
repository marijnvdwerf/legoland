# legoland — matching decompilation (instructions for Claude)

This is a matching decomp of `legoland.exe` (MSVC6, x86) in the isledecomp/reccmp style. Goal: write C
that the *original* MSVC6 compiler turns into bytes identical to the original binary, checked
per-function by `reccmp`. We do **not** reproduce the binary's layout — reccmp normalizes addresses.

## Toolchain

- Everything is downloaded by `uv run setup.py` into `toolchain/` (gitignored): MSVC6 (`toolchain/msvc6`),
  msvcrt DLLs (`toolchain/dlls`). Do not commit `toolchain/`.
- `wibo` must be on `PATH`. Both compile and link run through it (`/O2 /Z7`).
- System import libs (`kernel32`, `ddraw`, …) are committed in `libs/` (not downloadable); the link
  wrapper adds them automatically and LINK pulls only what's referenced.

## Build & verify

```sh
cmake --preset msvc6 && cmake --build build   # -> build/legoland.exe + PDB
cmake --build build --clean-first             # full rebuild (needed after header changes — no dep tracking)
./tools/verify                                 # per-function + total match %
./tools/verify -v 0x004015c0                   # asm diff for one function
uv run tools/progress.py                       # per-TU progress table
uv run tools/progress.py --worst 30            # the 30 lowest-scoring functions
uv run tools/asm2naked.py 0x00466d80 --apply   # transcribe a hand-written-asm function as naked __asm
```

**Header changes require `--clean-first`** — the MSVC6 wrapper does not track header dependencies.

`tools/verify` wraps `reccmp --target LEGOLAND`. reccmp's `cvdump.exe` (PDB reader) runs
through wibo via `tools/wine`; `tools/winepath` maps wibo's `Z:`-root PDB paths.

When you decompile a function: replace its `void name(void) { STUB(); }` body with
real C and the correct signature, rebuild, and run `./tools/verify -v <addr>` to
iterate the asm diff to 100%. A few empty/trivial functions already match the bare
stub. The image links with `/NODEFAULTLIB` and a dummy `/ENTRY` (`legoland_entry` in
`bootstrap.c`); imported APIs resolve from the committed `libs/` import libs.

## decomp.me workflow

Target asm for each function is in `../port2/project/asm/0x<ADDR>.asm`. Use the decomp-match
agent type to iterate scratches on decomp.me to a byte-match, then integrate the result into
the codebase. Scratches can be created via the MCP tools or the CLI in `../mcp/`.

See `docs/decomp-tips.md` for MSVC6 /O2 codegen patterns (register allocation, switch vs if-else,
struct assignment, etc.).

## Conventions

- One `.c` per TU (translation unit) under `src/legoland/`, named from `ghidra/functions.csv`'s `tu`
  column (`TU_RIDE_BLOKE` → `ride_bloke.c`). Functions appear in **address order**.
- Every function is tagged `// FUNCTION: LEGOLAND 0x<addr>` immediately above it.
- Unmatched functions have a `STUB();` body (macro in `legoland.h`). No game function is at `STUB()` any more:
  the last ones (hand-written assembly) were decompiled on 2026-10-07, and all but one match.
  `docs/remaining-work.md` lists what is left.
- When you decompile a function: replace its `STUB()` body with real C, build, run reccmp, iterate to 100%.
- **Before retrying a partial match, read its file in `docs/attempts/`** (what was tried, with scores) and don't
  repeat a listed attempt; afterwards add your attempts there, failed ones included. `docs/attempts/README.md`
  has the lessons that apply to most of them.
- No `ctx.h` — include real MSVC6 headers; shared decls go in `src/legoland/legoland.h`.
- **No forward declarations in `.c` files** — put all declarations in the TU's `.h` header.
  Run `uv run tools/needsdecl.py` to check.
- **Inline `__asm` and `__declspec(naked)` only where the original is hand-written assembly** — i.e. it
  contains instructions MSVC6 never emits from C (`rdtsc`, `xchg`, `pusha`, `shrd`, `fistp` after `fstp/fld`, …;
  see "Functions With an ebp Frame" in `docs/decomp-tips.md`). `uv run tools/progress.py <tu>` flags them.
  Everything else stays pure C. First try C with `__asm` only for the parts C can't produce. If MSVC6's
  register or stack-slot choices around the asm won't converge, transcribe the whole function as
  `__declspec(naked)` (`tools/asm2naked.py` does it) and say so in the comment above `// FUNCTION:`. The port
  replaces these functions with plain-C equivalents tagged `// [library:asm]`.
- **No `__asm` for compiler choices.** When the original was compiled C, the function stays C even if a
  register, stack-slot or instruction choice keeps it below 100%: write the best natural C and record the
  attempts in `docs/attempts/`. (The 2026-10-17 rule that allowed documented naked transcriptions for these
  was withdrawn on 2026-10-19; the 22 functions it produced went back to their C.)
- **`goto` only with named labels** — cleanup/error paths like `goto fail;` are fine (the original code
  used them, and they often match better than duplicated cleanup). That includes multi-label cleanup
  ladders (`fail_release:` / `fail_free:` / `fail_close:`) when the original's failure paths enter one
  shared cleanup tail at different steps. No Ghidra-style `goto LAB_00446b71;`: give the label a
  meaningful name or restructure with `if`/loops.
- **`volatile` is allowed when it is the only way** to get a load or store the original does (a field
  re-read from memory at each use, a store that must stay in place). Try the natural forms first (reading
  the field at each use, statement order, struct copies), and put a short comment on it, e.g.
  `/* volatile: the original re-reads this */`. It is a matching device, not a claim about the original
  source. Permuter temporaries (`ptmp45`) get a meaningful name when you touch them.
- **Run `clang-format -i`** on all modified `.c`/`.h` files before committing.
- **Retype globals** when casts can be removed. If a global is always cast to `struct Foo *`,
  change its type in `globals.h`/`globals.c`. Consecutive globals that form a struct
  (e.g., `EditMode` + `DAT_008119b4` + `DAT_008119b8`) should be merged into one struct global.
- **`TU_CRT` and `TU_IMPORTS` are NOT decompiled** — they exist as stub symbol files
  (`src/legoland/crt.c`, `imports.c`) using `// STUB: LEGOLAND 0x<addr>` annotations. Game functions
  call into them (CRT helpers like `memcpy`/`__ftol`/heap routines, and import thunks), so the symbols
  must be present for callers to *link* and for reccmp to *match* the call. **Leave every function in
  them as `STUB()` — never fill them in.** `// STUB:` keeps them out of the match % (so it reflects
  game functions only). One day they could be satisfied by a real CRT lib instead.

## Don't

- Don't edit `external/legoland.exe` (the match target) or anything in `toolchain/`.
- Don't try to make the binary *runnable* — that's a non-goal; it only needs to link + carry symbols.
- Don't commit downloaded toolchain binaries.
- Don't integrate/decompile `crt.c` / `imports.c` functions — they stay `STUB()` (symbols only; see above).
