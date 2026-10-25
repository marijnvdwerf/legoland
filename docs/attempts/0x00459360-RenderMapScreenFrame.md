# RenderMapScreenFrame (0x00459360)

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 63.96%. Matching has to come from C now.

## 2026-10-19 agent notes (63.96% -> 100.00%)

What the diff was: the original starts with `xor eax,eax; push ebx; mov ebx,1` and stores 1, 0, 0 into a 12-byte
local, which it then passes to PrintSprite: a `struct HoverInfo` (type, ptr, data) initialised to `{1}`, not a
two-field HitInfo set to 0. The 1 stays in ebx for the rest of the function (the `& 1` test and both `= 1`
stores), which frees esi/edi to be pushed only in the double-click block, as in the original. The tick test is a
signed compare (`jge`), so LastMapClickTicks is an int (it was declared unsigned int, giving `jae`).

| Change | Harness |
|---|---|
| `struct HoverInfo hit; hit.type = 1; hit.ptr = NULL; hit.data.value = 0;` (both orders), `= {1, NULL, 0}` | 60.20 |
| `struct HoverInfo hit = {1};` with the unsigned tick compare | 97.00 |
| `struct HoverInfo hit = {1};` and a signed tick compare | 98.00 (the rest is the listing length) |
