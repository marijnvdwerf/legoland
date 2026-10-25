# UpdateAviAudioBuffer (0x00476d20, `src/legoland/interface.c`)

**Best: 97.69%** (start version, unchanged). Kind: C.

## What still differs

- Parameter registers: the original loads `param_1` into `edi` and `param_2` into `eax` before pushing `esi`; ours uses `esi` for `param_1` and `ecx` for `param_2`, and `esi` is pushed at entry.
- `loops` lives in `esi` in the original (`mov esi, 0xb`); ours keeps it in `eax`.
- Stack slots differ by 4 for `play_pos` (original `[esp+0x18]`, ours `[esp+0x14]`) because the original pushes `ebx`/`edi` inside the loop.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 p8 | Haiku agent | Reordered locals (`loops`, `play_pos`, `bytes_out`, `count` declared after `play`) | 92.62% (no change) |
| 2026-10-08 | permuter + Opus 5.5 | REJECTED permuter 97.4%: split `A && B` that had an else into nested ifs, dropping the else when A is false (behaviour change) | not kept |
| 2026-10-08 | permuter + Opus 5.5 | `int rem`, `register int play_pos`, DAT_00668fa0 computed after `loops = 0xb` | 92.62 -> 97.69 |

## Ideas not tried yet

- Compute `loops = param_1 - param_2` through a temp so the allocator puts the difference in `esi`.

## 2026-10-24 Opus 5.5 (no change: 97.69)
The original pushes esi/edi only after the AviAudioReady/AviAudioStream test (shrink-wrapped early return). Removing
the `do { } while (0)` (early return kept, or the body nested in `if (ready && stream) { ... } return 0;`): 96.26.
