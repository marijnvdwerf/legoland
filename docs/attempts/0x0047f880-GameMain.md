# GameMain (0x0047f880, `src/legoland/debug.c`)

**Best: 99.62%**. Kind: C.

## What still differs

- One instruction only: the first resource-volume loop (`for (i = 0; i < 3; i++)`) ends with `jb` in the original (unsigned compare on `i`) and `jl` in ours. The error-path loop after it (`for (j = 0; j < i; j++)`) needs a signed `i` (`jle`), so the two loops disagree on signedness.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 | Haiku 5.5 | Baseline (previous round's C) | 99.62% |
| 2026-10-07 | Haiku 5.5 | `unsigned int i` for the whole function | 99.24% (jb/jl still differs, error loop becomes `jbe`) |
| 2026-10-07 | Haiku 5.5 | `(unsigned int)i < 3` in the loop condition | 99.62% (MSVC still emits `jl`) |
| 2026-10-07 | Haiku 5.5 | `i < sizeof(ResourceFileNames) / sizeof(ResourceFileNames[0])` | 99.62% (still `jl`) |
| 2026-10-07 | Haiku 5.5 | `unsigned int i` plus `j < (int)i` in the error loop | 99.62% (still `jl`) |
| 2026-10-07 | Haiku 5.5 | First loop rewritten as `do { } while (i < 3)` | 82.07% (worse, codegen restructured) |
| 2026-10-07 | Claude Opus 5.5 | `i < 3U` (unsigned constant) | 99.62% (still `jl`) |
| 2026-10-07 | Claude Opus 5.5 | `(unsigned)i < 3U` | 99.62% (still `jl`: MSVC6 picks the signed jump even for an explicitly unsigned compare here) |
| 2026-10-07 | Claude Opus 5.5 | `i < (int)3U` (control) | 99.62% (`jl`) |
| 2026-10-09 | Opus 5.5 | volume loop bound as `unsigned int i`, `3u`, `sizeof(ResourceFileNames)/sizeof(...)` (the original uses `jb` for `i < 3` but signed for `0 < i`) | no change (MSVC proves i >= 0 and emits jl for every form); note ResourceVolumes is declared [4] but the loop runs 3 |

## Ideas not tried yet

- A loop shape where MSVC cannot range-analyse `i` (so it keeps the unsigned compare), while the error-path loop still sees a signed `i`.
- Check whether the `jb` comes from a different bound expression (e.g. a size_t-typed count) that is not folded to a constant.

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 99.62%. Matching has to come from C now.

## 2026-10-23 Opus 5.5

Pointer-compare loop forms aimed at the `jb` (`&ResourceVolumes[i] < &ResourceVolumes[3]`, a `vol` pointer loop
with `i` alongside, `i = vol - ResourceVolumes`, `&ResourceFileNames[i] < &ResourceFileNames[3]`) restructure
the loop: harness 75-79.

## 2026-10-24 Opus 5.5 (no change: 99.62)
MSVC6 emits `jl` for `i < 3` here even with `unsigned int i` (it knows i in [0,3)), and an unsigned i turns the error
guard (`cmp esi, edi; jle`) into `jbe` (99.24). Loop forms tried with int and unsigned: while, for without increment,
`++i`, `i += 1`, `i <= 2`, a separate unsigned counter copied into i, `(unsigned)i < 3`, error loop with a cast or a
reused index: all keep `jl`.
