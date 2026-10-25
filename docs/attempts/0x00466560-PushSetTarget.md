# PushSetTarget (0x00466560, `src/legoland/draw.c`)

**Best: 100% (matched 2026-10-09)** (start). Kind: C.

## What still differs

- Store `renderEngineTargets[renderEngineTargetIdx] = renderEngine`: the original loads the index into edx and renderEngine into eax; ours loads the index into eax.
- The original reloads `renderEngineTargetIdx` for the `++` (`mov eax,[idx]; inc eax`) after the store.
- The original loads `sprite` into ecx before the store (`mov ecx,[esp+4]`), ours loads it late.
- Ends with a tail `jmp FUN_004640f0`; ours matches that.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku 5.5 (a4) | Moved `renderEngine = sprite->surface` before `renderEngineTargetIdx++` | 80.00% (worse) |
| 2026-10-08 | Haiku 5.5 (a4) | Read `sprite->surface` into a new local `next` at the top | 68.49% (worse, extra callee-saved reg esi) |
| 2026-10-09 | Opus 5.5 | store through `slot = &renderEngineTargets[idx]; *slot = ...` (a pointer store aliases the index, so it is reloaded like the original), then `idx++`, then `renderEngine = sprite->surface` | **100** |

## Ideas not tried yet

- Index the store through a temp copy of the index.
- Read `sprite->surface` into ecx right before the store without a new local.
