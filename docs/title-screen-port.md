# Scoping plan: run the LEGOLAND title screen interactively

Read-only analysis (no code changed). Goal, per the project owner: **the game boots,
loads the title-screen UI, and runs its per-frame render + input loop so the title
screen is shown and responds to input.** Cutscenes (FLC/AVI/Smacker movies) are
explicitly **out of scope** — a port ifdefs them out. The title-screen UI load + the
per-frame render/input loop are **in scope**.

Ground truth is Ghidra (`legoland.exe`, image base `0x00400000`). Match % values come
from `./tools/verify` (total today: **52.71%** across 3392 functions). "MATCHED"
below means the `.c` body is real and reccmp ≈ 100% unless noted; "STUB" means the body
is still `STUB()`.

---

## 1. Boot → title-screen → loop flow

```
_WinMainCRTStartup (CRT)                         [TU_CRT, out of match %]
└─ WinMain (thunk, debug.c)              0x------  (synthetic, not annotated)
   └─ WinMainBody                        0x0047fd10  debug.c   MATCHED 100%
      ├─ CreateMutexA / cmdline parse  (-nointro / -nomusic / WINDEBUG / BLT)
      ├─ CheckHostSystemGPU             0x004637c0  draw.c    MATCHED (zeroes DDRAWENV, calls InitHostSystemGPU)
      └─ FUN_0047f880  (main init+loop) 0x0047f880  debug.c   MATCHED 99.62%
         ├─ FUN_0047f830 open log
         ├─ FUN_004515e0 (alloc/profile init)
         ├─ RES_OpenVolume ×3           0x00489750  resource.c MATCHED 91.5%   (LEGO000/001/002 .res)
         ├─ FUN_00498d00
         ├─ InitHostSystemGPU           0x00463700  draw.c    STUB  *** DDraw device glue — STUB ***
         ├─ InitScreen                  0x00463870  draw.c    STUB  *** DDraw surfaces+window — STUB ***
         ├─ InitInputSystem             0x00473870  input.c   STUB  *** DInput device glue — STUB ***
         ├─ string table load (GetString backing)
         ├─ LoadSprite ×8  (cursors: erase it / no build / yes build / question it …)  MATCHED
         ├─ LLIDB_LoadICM               0x0047aff0  llidb.c   STUB  (in-scope: icon/cursor DB load)
         ├─ LLIDB_RegisterNewElement ×5 (menu element registration)                    MATCHED
         └─ FUN_00459520  (game bootstrap + frame-loop driver)  0x00459520  screens.c  STUB
```

`FUN_00459520` is **not** the per-frame loop itself — it is a long sequential
"front-end + game bootstrap" routine. It:

1. Inits sound (`InitSoundSystem`, `SetMusicGrooveLevel`), controllers
   (`SetupControllers`), clears the level DB (`LLIDB_ClearOnLevel`).
2. Calls `FUN_004588c0` (screens.c, **MATCHED 100%**) which paints the static
   **`TitleScreen1.lls`** splash directly to the locked surface (this is the first
   thing visible — but it is a one-shot paint, not the interactive title).
3. Plays the **intro logo movie** `lmi.avi` via `FUN_004771f0` — **OUT OF SCOPE**
   (cutscene). Spins a `PeekMessageA`/`Sleep(100)` wait loop until `DAT_007988bc`.
4. Loads game/world assets (`LoadWorkerInterfaceGFX`, `InitialiseBlokes`,
   `InitGameMap` 100%, `LoadMapTiles`, `CreateObjectClasses`, `Ir50_32.dll` for
   Indeo/AVI, …) interleaved with `FUN_004663f0` loading-screen ticks.
5. Plays `Intro.avi` (`FUN_004771f0`) — **OUT OF SCOPE**.
6. **Enters the real interactive loop:**
   ```c
   while (FUN_00458c00() != 0) { /* per-frame */ }
   ```
7. On exit, tears everything down (sprites, map, man, sound, input).

> **Port note:** because the two `FUN_004771f0(...avi...)` calls sit inline in this
> routine and in the frame loop, a port wraps them in an `#if ENABLE_MOVIES` (or makes
> `FUN_004771f0` a no-op returning 1). The wait-for-`DAT_007988bc` loop after `lmi.avi`
> also needs the movie-completion signal stubbed so it doesn't spin forever. The asset
> loads in step 4 are world/game data the title menu does **not** strictly need; an
> interactive-title-only build can skip most of them, but they are cheap to keep since
> many are already MATCHED.

---

## 2. The frame loop and how it routes to the current screen

### Driver: `FUN_00458c00` @ `0x00458c00` (screens.c) — **STUB, critical**
One tick of the main loop. Returns 0 to quit. Each tick it:
- handles map-screen enter/exit and savegame-load transitions, then
- **switches on the global screen-mode `DAT_008119b4`:**
  | `DAT_008119b4` | meaning | handler |
  |---|---|---|
  | 0 | quit | `return 0` |
  | 1 | (transition) | `FUN_00459360` (STUB) |
  | **2** | **front-end screens** | `InitScreens(DAT_0080ff88 & 0xff)` |
  | 3 | in-game | `FUN_00458ee0` (STUB) |
- then runs input + audio for the frame: `FUN_004969d0` (sound, MATCHED) →
  **`ReadGameButtons`** (controller poll, STUB) → frame counter → handles a couple of
  `DAT_00832ba0` mode-change transitions → `FUN_0046d110`.

### Front-end state machine: `InitScreens` @ `0x00458640` (screens.c) — **STUB, critical**
Called from frame-loop case 2 with the screen id in `DAT_0080ff88`. On a screen change
(`DAT_0080ff84 != id`) it runs the screen's **init**; every frame it runs the screen's
**render** via `RenderFrontEndScreen` (if a background sprite `DAT_00810148` exists):

| `DAT_0080ff88` | screen | init fn | status |
|---|---|---|---|
| 0 | profile list | `InitListProfiles` | STUB |
| **1** | **TITLE** | **`InitTitleScreen` `0x0048fc40`** | **STUB (title.c)** |
| 3 | free play | `InitFreePlayScreen` | STUB |
| 4 | saved game | `InitSavedGameScreen` | STUB |
| 5 | options | `InitOptionScreen` | STUB |
| 6 | progress | `InitProgressScreen` | STUB |
| 7 | (intervals/help) | `FUN_00490c70` | STUB |
| 8 | certificate | `FUN_00490350` | MATCHED-ish (real body in title.c) |
| 9 | (movie menu) | `FUN_00490150` | STUB |

### Per-frame title render: `RenderFrontEndScreen` @ `0x00458740` (screens.c) — **STUB, critical**
This is the **title screen's per-frame render + input handler**. Sequence:
`ResetHitInfo` (100%) → `PushRenderingStatusAndLockVideoSurface` (STUB) →
`PrintSprite(DAT_00810148,…)` (background, STUB) → **`RenderIcons`** (STUB) → per-screen
extra (title id 1 → `PrintExitCheckBox`, STUB) → `ProcessFrontEndHelp` (STUB) →
**`UpdateFocussedIconPtr`** (MATCHED 100%) → `PopRenderingStatus` (STUB) →
`SetPointer(6)` if hovering an icon → **`CheckFocussedIcon`** (STUB — fires the menu
button's event handler) → `RenderingComplete` (STUB → blit/flip via `PTR_FUN_004b9ca4`).

So the closed interactive title loop is:
```
FUN_00458c00 ──case 2──▶ InitScreens ──▶ RenderFrontEndScreen
   │                                         │  draws bg + RenderIcons
   │                                         │  UpdateFocussedIconPtr (hit-test mouse → focus)
   │                                         └─ CheckFocussedIcon (mouse/keys → icon->event_handler)
   └──per-frame──▶ ReadGameButtons (controller/mouse/kbd → button bits + cursor xy)
                   ▲ fed by ProcessSystemEvents (in RenderingComplete) → ScanKeyboard/ScanMouse
```

The title's button handlers already exist in `title.c` and are mostly **MATCHED**
(`FUN_0048fe20` Load, `FUN_0048ff20` Free-play, `FUN_0048ff70` Reg, `FUN_0048feb0`
New-game (decompiled in Ghidra), `FUN_00490090/d0/110` the per-region movie buttons →
out of scope AVI, `FUN_0048f0a0` Exit). They are wired up by `InitTitleScreen` into
`SpriteIcon.event_handler` (offset +0x2c) — so once the loop + icon plumbing match,
input already routes to real code.

---

## 3. Prioritized worklist — STUBs that must be decompiled

Grouped by area; **do the blockers first**. "size" is a rough estimate from the Ghidra
decompile (S ≤ ~30 lines, M ~30–80, L > 80, XL > 150). "deps" notes whether the
functions it calls are already done.

### A. Frame loop / screen routing  (THE backbone — do first)
| addr | name | file | size | deps | notes |
|---|---|---|---|---|---|
| `0x00458c00` | `FUN_00458c00` | screens.c | L | most MATCHED; calls `ReadGameButtons`(B), `InitScreens`(A), `FUN_00458ee0`(in-game, can stay STUB for title), `FUN_00459360`(STUB), `FUN_004771f0`(OOS) | **single per-frame tick + screen switch. Top blocker.** |
| `0x00458640` | `InitScreens` | screens.c | M | `InitTitleScreen`(C), `RenderFrontEndScreen`(below), other screen inits can stay STUB | front-end router |
| `0x00458740` | `RenderFrontEndScreen` | screens.c | M | needs `PrintSprite`(E), `RenderIcons`(D), `CheckFocussedIcon`(D), `PrintExitCheckBox`(F), `ProcessFrontEndHelp`(F), `RenderingComplete`(E/stub) | **per-frame title render** |
| `0x00459520` | `FUN_00459520` | screens.c | XL | many MATCHED; `InitSoundSystem`(G), `SetupControllers`(B), `LLIDB_*`, `FUN_004771f0`(OOS, ifdef) | main init that *enters* the loop; large but mostly sequential calls |

### B. Input (controller / mouse / keyboard poll)
| addr | name | file | size | deps | notes |
|---|---|---|---|---|---|
| `0x00452460` | `ReadGameButtons` | controller.c | L | `GetTickCount`, `MouseScrollMap`, `FUN_00451f70/030/390/2030`(STUB), `ScreenToMapRef` | edge/hold-detect for all pad bits + cursor xy. **Required for "responds to input."** Map-scroll branch only runs in-game; title path uses the button+cursor part. |
| `0x00451e70` | `SetupControllers` | controller.c | M | allocs `CONTROLLERBUFFER` | called by `FUN_00459520`; needed so `ReadGameButtons` has a buffer |
| `0x00480050` | `ProcessSystemEvents` | wndenv.c | M | `PeekMessageA/GetMessageA/TranslateMessage/DispatchMessageA`, `ScanKeyboard`(MATCHED), `ScanMouse`(MATCHED), `UpdateControllerFromMouseData/KeyboardData`(STUB) | Win32 message pump + DInput scan. Called from `RenderingComplete` every frame. |
| `0x00473b00` | `UpdateControllerFromMouseData` | input.c | M | DInput mouse state → `CONTROLLERBUFFER` | fills the buffer `ReadGameButtons` reads |
| `0x00473c10` | `UpdateControllerFromKeyboardData` | input.c | M | keyboard state → `CONTROLLERBUFFER` | same |
| `0x004738b0` | `FUN_004738b0` | input.c | M | DInput create mouse device | sub of `InitInputSystem`(stub) — see §4 |
| `0x00473970` | `FUN_00473970` | input.c | M | DInput create keyboard device | sub of `InitInputSystem`(stub) — see §4 |
| `0x004740b0` | `GetInputChar` | input.c | S | keyboard buffer → char | needed only for name-entry popups, not bare title |

### C. Title UI load
| addr | name | file | size | deps | notes |
|---|---|---|---|---|---|
| `0x0048fc40` | `InitTitleScreen` | title.c | L | `LoadSprite`(100%), `LoadSpriteIcon`(100%), `GetString`(100%), `FUN_0048fc30`, handlers already in title.c | **builds the title menu**: loads `TitleScreenBk.lls` + 7 menu icons (New/Free/Reg/Exit/Load/MovieOn) and wires each icon's `event_handler`. Logic is self-contained; the heavy lifting (`LoadSpriteIcon`) is MATCHED. **Most useful next target.** |
| `0x0048feb0` | `FUN_0048feb0` | title.c | S | `RemoveIconGroup`(D), `KillTitleScreenSprites`, `PlayInstanceOfSample`(MATCHED) | New-game button handler (also stored as default action). Already decompiled in Ghidra; trivial. |
| `0x0048ffe0` | `FUN_0048ffe0` | title.c | S | — | MovieOn button handler |
| `0x00490150` | `FUN_00490150` | title.c | M | movie-menu screen init (id 9) | only needed if movie sub-menu is in scope; otherwise leave STUB |

### D. Render — icon system (the menu visuals + hit handling)
| addr | name | file | size | deps | notes |
|---|---|---|---|---|---|
| `0x0046eee0` | `RenderIcons` | icon.c | M | `PrintSprite`(E), `StoreClipping`/`RestoreClipping`(100%), `FUN_0046df60`(100%), `FUN_0046ec50`(STUB), `RenderThickBox`(E), `GetNearestColour`(100%), `LLSPlayOnce` | **draws all visible icons + focus highlight.** Core title visual. |
| `0x0046f010` | `RenderIcons2` | icon.c | M | same as above (group-filtered variant) | used by other screens + in-game; cheap once RenderIcons done |
| `0x0046f4c0` | `CheckFocussedIcon` | icon.c | M | calls `FocussedIconPtr->event_handler` and the default `DAT_006687bc/c0` handlers using `ReadGameButtons` bits | **routes mouse/keys to the menu button handlers — the "responds to input" core.** |
| `0x0046d520` | `RemoveIconGroup` | icon.c | S/M | icon list unlink | teardown used by every title handler; needed for clean transitions |
| `0x0046d6c0` | `InsertIcon` | icon.c | M | icon list alloc/link | `LoadSpriteIcon` (MATCHED) already calls it? — verify; needed by icon construction |
| `0x0046ec50` | `FUN_0046ec50` | icon.c | M | icon animation tick | called by `RenderIcons`; can stub-return early initially |
| `0x0046f2e0` | `FUN_0046f2e0` | icon.c | M | drag/hold icon handler | called by `CheckFocussedIcon` non-button path; lower priority |
| `0x0046f360` | `GetIconAtPos` | icon.c | M | hit-test | used by `UpdateFocussedIconPtr` (MATCHED) — confirm it isn't already pulled in |

### E. Render — sprite drawing + present (mixed: game code routing is real, rasterizer is a stub)
| addr | name | file | size | deps | notes |
|---|---|---|---|---|---|
| `0x004853a0` | `PrintSprite` | print_sprite.c | L | `GetVRAMAddress`(100%), `FUN_00499500`(100%), `RenderSprite`(E/stub), `RenderSpriteX`(E/stub) | **sprite draw routing + hit-info writeback (`DAT_004bdd00`).** The routing/hit logic is real game code worth matching; it calls the rasterizer below. |
| `0x004856a0` | `PrintSpriteEx` | print_sprite.c | L | same | variant |
| `0x00489390` | `RenderThickBox` | (interface/icon) | S/M | line draws into surface | focus highlight box |
| `0x00466500` | `RenderingComplete` | draw.c | S | `ProcessSystemEvents`(B), `PTR_FUN_004b9ca4`→`FUN_004661d0`(stub blit), `rdtsc`, `GetTickCount` | **the per-frame "present" + input pump.** Small; logic is real, but the actual flip is the stub `FUN_004661d0`. |

### F. Title overlays / help (only if you want the full title behavior)
| addr | name | file | size | notes |
|---|---|---|---|---|
| `0x0048f2d0` | `PrintExitCheckBox` | options.c | M | drawn for title (id 1) + options (id 5); the "are you sure you want to exit" box. Needed for the Exit button flow. |
| `0x0046d080` | `ProcessFrontEndHelp` | help.c | M | tooltip/help text under the cursor; non-essential, can stay STUB initially |
| `0x004585c0` | `FUN_004585c0` | screens.c | S | screen-clear helper called by `InitScreens` on change + `FUN_004594e0` |

### G. Audio (called during boot; not strictly required to *see* the title)
| addr | name | file | size | notes |
|---|---|---|---|---|
| `0x004964f0` | `InitSoundSystem` | sound_music.c | M | DirectSound init; **port can replace** — but it's called in `FUN_00459520`. If kept real it pulls DSound; otherwise leave it as a stub. `PlayInstanceOfSample` (the click SFX) is already MATCHED. |

---

## 4. Stubs that a port will replace (link only — DO NOT byte-match)

These are DirectDraw / DirectInput / DirectSound / Win32 device glue. A real port
reimplements them against SDL/modern APIs, so for the matching decomp they only need to
**link** (keep `STUB()`), not reach 100%. They are on the boot path but are
device I/O, not game logic.

| addr | name | file | API surface |
|---|---|---|---|
| `0x00463700` | `InitHostSystemGPU` | draw.c | DirectDraw object creation |
| `0x004637e0` | `KillHostSystemGPU` | draw.c | DDraw teardown |
| `0x00463870` | `InitScreen` | draw.c | `RegisterClassExA`+`CreateWindowExA`, DDraw `CreateSurface` (primary/back/work), `SetCooperativeLevel`, palette — **XL Win32+DDraw glue** |
| `0x00463ef0` | `FUN_00463ef0` | draw.c | DDraw mode set helper |
| `0x00463fc0` | `PushRenderingStatusAndLockVideoSurface` | draw.c | `IDirectDrawSurface::Lock` (+clip rect) |
| `0x00464080` | `PushRenderingStatusAndUnlockVideoSurface` | draw.c | surface unlock |
| `0x004641f0` | `PopRenderingStatus` | draw.c | restore lock state |
| `0x00464310` | `GetVideoSurface` | draw.c | surface ptr accessor |
| `0x004661d0` | `FUN_004661d0` | draw.c | **the blit/flip** (`IDirectDrawSurface::Blt`, vtbl+0x14) — `PTR_FUN_004b9ca4` callback driven by `RenderingComplete` |
| `0x004663f0` | `FUN_004663f0` | draw.c | loading-screen cursor blit (DDraw) |
| `0x00488a10` | `RenderSprite` | (image) | software sprite rasterizer into locked surface |
| `0x00488b90` | `RenderSpriteX` | (image) | transparent/blended sprite raster |
| `0x00473870` | `InitInputSystem` | input.c | `DirectInputCreateA` + device setup |
| `0x004738b0` | `FUN_004738b0` | input.c | DInput mouse `CreateDevice`/`SetDataFormat`/`SetCooperativeLevel` |
| `0x00473970` | `FUN_00473970` | input.c | DInput keyboard device setup |
| `0x004964f0` | `InitSoundSystem` | sound_music.c | DirectSound init (if kept as stub) |

> The input *polling logic* on top of these (`ScanKeyboard` MATCHED, `ScanMouse`
> MATCHED, `ReadGameButtons`, `UpdateControllerFrom*`) is **game logic, in scope** — only
> the raw device-create/lock/blit calls are stubs. Similarly `PrintSprite`'s routing is
> in scope while the `RenderSprite*` rasterizers underneath are stubs.

### Out of scope entirely (cutscenes / movies)
| addr | name | notes |
|---|---|---|
| `0x004771f0` | `FUN_004771f0` | AVI/Smacker movie player (`lmi.avi`, `Intro.avi`, `Billund/Windsor/California.avi`). Ifdef out / no-op returning 1. |
| `0x00476460` | `FUN_00476460` | movie open |
| `0x004766f0` | `FUN_004766f0` | movie play loop |
| `0x00476630` | `FUN_00476630` | movie close |
| `Ir50_32.dll` load in `FUN_00459520` | — | Indeo codec for the AVIs; drop with the movies |
| `0x00490090/d0/110` | per-region movie buttons (title.c) | already real, but call the OOS movie player |

---

## 5. Counts and most useful next targets

**In-scope STUBs that block an interactive title screen** (areas A–F above, excluding
the §4 stubs and OOS movies): **~22 functions**, concentrated in:
`screens.c` (frame loop + routing: `FUN_00458c00`, `InitScreens`, `RenderFrontEndScreen`,
`FUN_00459520`, `FUN_004585c0`), `icon.c` (`RenderIcons`, `RenderIcons2`,
`CheckFocussedIcon`, `RemoveIconGroup`, `InsertIcon`, `FUN_0046ec50`, `FUN_0046f2e0`,
`GetIconAtPos`), `controller.c` (`ReadGameButtons`, `SetupControllers`), `wndenv.c`
(`ProcessSystemEvents`), `input.c` (`UpdateControllerFromMouseData/KeyboardData`),
`title.c` (`InitTitleScreen`, `FUN_0048feb0`, `FUN_0048ffe0`), `print_sprite.c`
(`PrintSprite`), `draw.c` (`RenderingComplete`), `options.c` (`PrintExitCheckBox`).

The ~16 §4 stubs and ~5 movie functions are **not** in that count (link-stubs / OOS).

### Top 10 most useful next targets (do in this order)
1. **`FUN_00458c00` `0x00458c00`** (screens.c) — the per-frame tick + screen switch; nothing runs without it.
2. **`InitScreens` `0x00458640`** (screens.c) — routes to the title state.
3. **`RenderFrontEndScreen` `0x00458740`** (screens.c) — the title's per-frame render.
4. **`InitTitleScreen` `0x0048fc40`** (title.c) — builds the menu; its heavy dependencies (`LoadSpriteIcon`, `GetString`) are already MATCHED, so big payoff.
5. **`ReadGameButtons` `0x00452460`** (controller.c) — turns device input into the button/cursor state the menu reads; "responds to input" depends on it.
6. **`CheckFocussedIcon` `0x0046f4c0`** (icon.c) — routes input to the menu button handlers (which are already mostly real).
7. **`RenderIcons` `0x0046eee0`** (icon.c) — draws the menu + focus highlight.
8. **`PrintSprite` `0x004853a0`** (print_sprite.c) — sprite draw + hit-info writeback used by both bg and icons (rasterizer underneath is stubbed).
9. **`RenderingComplete` `0x00466500`** (draw.c) — per-frame present + drives `ProcessSystemEvents`; small, real logic.
10. **`ProcessSystemEvents` `0x00480050`** (wndenv.c) — Win32 pump + DInput scan; closes the input loop.

Runner-ups once the above match: `RemoveIconGroup`, `SetupControllers`,
`UpdateControllerFromMouseData/KeyboardData`, `FUN_0048feb0`, `PrintExitCheckBox`,
and `FUN_00459520` (large but mostly MATCHED-callee sequential glue; can be done last
with the movie calls ifdef'd).

> **port2 cross-check:** for each target, start from `port2/project/c/0x00<ADDR>.c`
> (uppercase hex) per DECOMPILING.md — these are byte-matched drafts to translate, not
> re-derive. I did not spot a disagreement between Ghidra and the port3 source for the
> already-decompiled title handlers; the Ghidra decompiles above match the existing
> `title.c`/`screens.c` bodies. Note `ghidra/functions.csv` referenced in CLAUDE.md is
> not present in this checkout (only `ghidra/crt.csv` exists) — addresses/names here come
> straight from the live Ghidra program and the in-tree `// FUNCTION:` annotations.
