# PrintCertificate (0x00451740, `src/legoland/certificate.c`)

**100% (2026-10-17) with documented naked asm**; best C 89.11%. Kind: C + asm.

## What still differs

- Frame slots: the original keeps `returned` at `esp+0x28` (after the 4 pushes) and `needed` at `esp+0x30`. Ours puts `returned` at `esp+0x2c`, so the zero stores before `EnumPrintersA` use `[esp+0x44]`/`[esp+0x4c]` in the original and `[esp+0x48]`/`[esp+0x4c]` in ours. Declaration order does not move it.
- Register choice: the original uses `ebp` as the zero register and `ebx` for the `_open` result `fd`; ours has them swapped (`xor ebx, ebx`, `fd` in `ebp`).
- Error exits: the original repeats the full cleanup (`GlobalFree`/`DeleteDC`/`pop`s/`ret`) at each failure site. Ours jumps back to a shared tail (`jmp -0xc5`, `jmp -0xf1`), so the layout and the `pInfo == NULL` path differ.
- Cleanup order around `GetDeviceCaps(RASTERCAPS)` / `DeleteObject(hBitmap)` / `je` differs: the original calls `DeleteObject` and `DeleteDC` in the tail after a `test al,1` and a separate `push 0x26` call.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-08 | Haiku 5.5 (a2) | Declaration order: `printers` moved to the end of the locals | 81.20% (no change) |
| 2026-10-08 | Haiku 5.5 (a2) | Declaration order: `returned` declared before `needed` and `bmfh`/`bmih` first | 81.20% (no change) |
| 2026-10-08 | permuter + Opus 5.5 | permuter 84.1% came from a temp landing as the body of the braceless `if (biBitCount < 9)` so `_read` always ran (behaviour change); without it no gain | rejected |

## Best

81.20%, the version currently in the file (unchanged from the start).


## 2026-10-17 findings (Opus 5.5)

- **Real source difference found:** the first `TextOutA` y is `vres * 678 / pInfo->bmiHeader.biHeight` (the original
  loads `pInfo` from its slot and divides by `[ecx+8]`), not `bmih.biHeight`. That read keeps `pInfo` live into the
  font code, so it no longer shares a slot with `hFont`/`hOldFont`. 87.26 -> 89.11.
- With that fix and **all** artifacts removed (no volatile, no ptmp10, `ncolors` as the ternary inside GlobalAlloc),
  the frame layout is exactly the original's (`/FAs` listing: fd/hres 0x10, hFont/hOldFont 0x14, hBitmap 0x18,
  hBits 0x1c, hInfo 0x20, pInfo 0x24, returned 0x28, pBits 0x2c, needed 0x30, hMemDC 0x34), but the score is 84.83:
  the zero constant gets ebx and fd ebp (original: zero in ebp, fd in ebx), so ours caches GlobalAlloc in ebx and
  the error tails merge differently (ours merges the hBits/pBits fails into the shared _close tail and RASTERCAPS
  into StartDoc's block; the original merges only font2 into font1).
- The two volatile temporaries (`volatile int ncolors`, `ptmp10`) are what steer the register choice (they add uses);
  removing either costs 5-6 points. The bare `returned;`/`pInfo;` statements, `register` and the extra braces had no
  effect and were removed.
- Tried without gain on the clean version: DOCINFO field order (name last: +0.16), memset(&di), `hFont = NULL` /
  `hOldFont = NULL` / other handle inits (dead stores, dropped before slot assignment), `pInfo = NULL` removed,
  `returned`/`needed` init order, `register int fd`, unsigned fd, `fd >= 0` without cast, RASTERCAPS test as `== 0` /
  `!= RC_BITBLT`, `register` hInfo/hBitmap.
- Tool: `/FAs` listing of the TU gives the named slot map (scratchpad codslots.sh); the original's map via push-depth
  tracking of the disassembly (espdepth.py).

## Ideas not tried yet

- Find the 4-byte hole the original leaves at `esp+0x2c`. Something the original declares is either in a block or not spilled at all; try block scope for `needed`/`returned` (for example a `{ }` around the `EnumPrintersA` check).
- Write the first failure path as the original does (a full copy of the cleanup and `return 0`), so the tail is not merged.
- Swap `fd` and the zero register by changing which of `_open`'s result or `needed=0` is assigned first.

## 2026-10-17: 100% with documented asm

The MSVC6 build uses a naked __asm transcription commented block by block; the #else branch is
the plain C (no volatile temporaries). The strings became named arrays so the asm can push them:
CertificateDocName (local) and LegoFontName (globals.c, shared with InitScreen in draw.c, which now
copies it with memcpy(..., sizeof(LegoFontName)): MSVC6 inlines that exactly like the original's
inlined strcpy of the literal; a plain strcpy from the array is not inlined, 71%).

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 38.31%. Matching has to come from C now.
Then the best C from before the asm version was put back (it is better than the plain `#else` C; its `volatile`
or extra local is commented in the source): 89.11%.
