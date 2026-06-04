# legoland — matching decompilation (instructions for Claude)

This is an **isledecomp/reccmp-style matching decomp** of `legoland.exe` (MSVC6, x86). Goal: write C
that the *original* MSVC6 compiler turns into bytes identical to the original binary, verified
per-function by `reccmp`. We do **not** reproduce the binary's layout — reccmp normalizes addresses.

## Toolchain

- Everything is downloaded by `uv run setup.py` into `toolchain/` (gitignored): MSVC6 (`toolchain/msvc6`),
  `wibo` (`toolchain/wibo`), msvcrt DLLs (`toolchain/dlls`). Do not commit `toolchain/`.
- Compile runs through `wibo` (`/O2 /Z7`). **Linking runs through CrossOver wine** (`tools/wine`,
  bottle from `CX_BOTTLE`) — wibo 1.1.0 can't build a PE import section (missing `GetTempFileNameW`).
- System import libs (`kernel32`, `ddraw`, …) are committed in `libs/` (not downloadable); the link
  wrapper adds them automatically and LINK pulls only what's referenced.

## Build & verify

```sh
cmake --preset msvc6 && cmake --build build   # -> build/legoland.exe + PDB
./tools/verify                                 # per-function + aggregate match %
./tools/verify -v 0x004015c0                   # asm diff for one function
```

`tools/verify` wraps `reccmp --paths`. reccmp's `cvdump.exe` (PDB reader) runs via
CrossOver wine (`tools/wine`, bottle from `CX_BOTTLE`); `tools/winepath` is a
`Z:`-root transform matching wibo's PDB paths.

When you decompile a function: replace its `void name(void) { STUB(); }` body with
real C and the correct signature, rebuild, and run `./tools/verify -v <addr>` to
iterate the asm diff to 100%. A few empty/trivial functions already match the bare
stub. The image links with `/NODEFAULTLIB` and a dummy `/ENTRY` (`legoland_entry` in
`bootstrap.c`); imported APIs resolve from the committed `libs/` import libs.

## Conventions

- One `.c` per translation unit under `src/legoland/`, named from `ghidra/functions.csv`'s `tu`
  column (`TU_RIDE_BLOKE` → `ride_bloke.c`). Functions appear in **address order**.
- Every function is tagged `// FUNCTION: LEGOLAND 0x<addr>` immediately above it.
- **Unmatched** functions have a `STUB();` body (macro in `legoland.h`). Remaining work: `grep -rn 'STUB()' src/`.
- When you decompile a function: replace its `STUB()` body with real C, build, run reccmp, iterate to 100%.
- No `ctx.h` — include real MSVC6 headers; shared decls go in `src/legoland/legoland.h`.
- **`TU_CRT` and `TU_IMPORTS` are NOT decompiled.** They're the static C runtime and the linker's
  import-thunk table — library/linker code, not game source. There are no `crt.c`/`imports.c`; don't
  regenerate them. They'll be satisfied later by linking the real CRT lib + the linker's own import
  thunks. Skip them in any stub regeneration.

## Don't

- Don't edit `external/legoland.exe` (the match target) or anything in `toolchain/`.
- Don't try to make the binary *runnable* — that's a non-goal; it only needs to link + carry symbols.
- Don't commit downloaded toolchain binaries.
- Don't recreate `crt.c` / `imports.c` (see above).
