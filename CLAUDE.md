# legoland — matching decompilation (instructions for Claude)

This is an **isledecomp/reccmp-style matching decomp** of `legoland.exe` (MSVC6, x86). Goal: write C
that the *original* MSVC6 compiler turns into bytes identical to the original binary, verified
per-function by `reccmp`. We do **not** reproduce the binary's layout — reccmp normalizes addresses.

## Toolchain

- Everything is downloaded by `uv run setup.py` into `toolchain/` (gitignored): MSVC6 (`toolchain/msvc6`),
  `wibo` (`toolchain/wibo`), msvcrt DLLs (`toolchain/dlls`). Do not commit `toolchain/`.
- MSVC6 `.exe` tools run through `wibo`. Compile with `/O2` (and `/Zi` for the PDB reccmp needs).
- Fallback executor: the `Visual C++ 6.0 SP6` CrossOver bottle's wine, if `wibo` can't drive a tool.

## Build & verify

```sh
cmake --preset msvc6 && cmake --build build   # -> build/legoland.exe + PDB
uv run reccmp ...                              # per-function + aggregate match %
```

## Conventions

- One `.c` per translation unit under `src/legoland/`, named from `ghidra/functions.csv`'s `tu`
  column (`TU_RIDE_BLOKE` → `ride_bloke.c`). Functions appear in **address order**.
- Every function is tagged `// FUNCTION: LEGOLAND 0x<addr>` immediately above it.
- **Unmatched** functions have a `STUB();` body (macro in `legoland.h`). Remaining work: `grep -rn 'STUB()' src/`.
- When you decompile a function: replace its `STUB()` body with real C, build, run reccmp, iterate to 100%.
- No `ctx.h` — include real MSVC6 headers; shared decls go in `src/legoland/legoland.h`.

## Don't

- Don't edit `external/legoland.exe` (the match target) or anything in `toolchain/`.
- Don't try to make the binary *runnable* — that's a non-goal; it only needs to link + carry symbols.
- Don't commit downloaded toolchain binaries.
