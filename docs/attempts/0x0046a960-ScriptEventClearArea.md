# ScriptEventClearArea (0x0046a960, `src/legoland/nerps.c`)

**Best: 72.83%**

## What still differs

The original hoists `lpConfig` into edx for the loops and reloads `GameMap` inside the bounds test
(`mov eax,[GameMap]; mov eax,[eax+edi*4]; add eax,esi`), with `count` kept in memory. Ours hoists GameMap and
strength-reduces `&GameMap[y]` (`lea edx,[edx+edi*4]`), reloading lpConfig instead. FUN_0045c900 has the opposite
symptom (the original hoists GameMap there).

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-09 | Opus 5.5 | tile lookup as one `?:` expression | harness 76.83, verify 72.83 (no change, reverted) |

## Ideas not tried yet

- 
