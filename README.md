# legoland

A **matching decompilation** of `legoland.exe` (x86 Windows, built with Microsoft Visual C++ 6.0),
in the style of the [LEGO Island decompilation](https://github.com/isledecomp/isle): ordinary C is
compiled with the *original* MSVC6 toolchain and verified **function-by-function** against the
original binary with [`reccmp`](https://github.com/isledecomp/reccmp). Addresses and relocations are
normalized away, so the recompiled binary does **not** need to reproduce the original's layout — a
function "matches" when its compiled bytes equal the original's.

This repo is **self-contained**: the MSVC6 compiler, the [`wibo`](https://github.com/decompals/wibo)
loader, and its support DLLs are downloaded by `setup.py` into a gitignored `toolchain/`. No
proprietary binaries are committed.

## Quick start

```sh
uv run setup.py          # download MSVC6 + wibo + DLLs into toolchain/
cmake --preset msvc6     # configure (uses cmake/msvc6-toolchain.cmake)
cmake --build build      # build legoland.exe + PDB (all 99 TUs)
./tools/verify           # per-function match % vs the original
./tools/verify --html report.html        # searchable HTML report
./tools/verify -v 0x004015c0             # asm diff for one function
```

`tools/verify` runs `reccmp`, whose bundled `cvdump.exe` (PDB reader) is executed
through **CrossOver wine** in a bottle (set via `CX_BOTTLE`, default
`Visual C++ 6.0 SP6`) — see `tools/wine`. Everything else (compile + link) runs
through `wibo`, no wine.

## Layout

| path | what |
|------|------|
| `src/legoland/*.c` | one `.c` per TU (99 of them); functions annotated `// FUNCTION: LEGOLAND 0x...` |
| `src/legoland/legoland.h` | shared forward declarations + globals + the `STUB()` marker |
| `cmake/` | CMake toolchain file driving MSVC6 through `wibo` |
| `setup.py` | downloads the toolchain (hardcoded, pinned URLs) |
| `external/legoland.exe` | the original binary (match target) |
| `reccmp-project.yml` | `reccmp` configuration |

## Conventions

- **Unmatched functions** carry a `STUB();` body. Find remaining work with `grep -rn 'STUB()' src/`.
- **Matched functions** have a real body that compiles (with MSVC6) to bytes identical to the original.
- Each function is tagged with its original virtual address; `reccmp` matches by that address via the build's PDB.

The function inventory (name → offset → TU) comes from `ghidra/functions.csv` in the parent project.
