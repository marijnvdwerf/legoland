# Running agent batches on this repo

Lessons from the first multi-agent sessions. Helper scripts and prompts live in `tools/agent/`.

## One-time setup in a fresh container
```sh
export PATH=$HOME/.local/bin:$PATH
# wibo is not preinstalled: download the release binary to ~/.local/bin/wibo (chmod +x)
uv run setup.py                                  # toolchain (needs SSL_CERT_FILE=/root/.ccr/ca-bundle.crt behind the proxy)
cmake --preset msvc6 && cmake --build build
uv run reccmp-project detect --search-path external   # writes reccmp-user.yml
cp build/reccmp-build.yml . && sed -i 's#^project: .*#project: .#' reccmp-build.yml   # tools/progress.py needs this at repo root
uv run tools/progress.py                         # progress table (expect ~2941 matched / 3255 at 90.4%)
```
`reccmp-build.yml` and `reccmp-user.yml` are gitignored. `uv run` rewrites `uv.lock`: `git checkout uv.lock` before committing.

## Helpers (`tools/agent/`)
- `status.py` prints `addr tu name pct` for every game function (needs the reccmp configs above).
- `partials.py` lists pure-C partial matches (excludes inline-asm functions) to /tmp/partials.json.
- `regress.sh BASELINE` compares against a saved `status.py` output: prints `REGRESSED` / `NEWLY MATCHED`.
  Save the baseline first: `uv run tools/agent/status.py > /tmp/baseline.txt`.
  Empty output means no change; make sure the build actually succeeded first (a failed build looks the same).
- `wt-setup.sh` prepares an agent worktree. `*-prompt.md` are the agent prompts used (paths point at /tmp/names).
- `score.py ADDR...` prints the match % of single functions (a few seconds; `EFFECTIVE` marks register/order-only matches that
  status.py counts as 100%). Much faster than a full `regress.sh` while iterating.
- `sbs.py ADDR...` prints the whole function side by side (original | ours), `~`/`-`/`+` on differing lines. `verify -v` only shows hunks.
- `diffsum.py`, `difftype.py` and `deadends.py` triage a list of addresses: size of the diff, whether the instruction multiset is
  identical (pure register/scheduling difference), and diffs on layout-dependent operands reccmp cannot normalize (skip those).

## Rules that worked
- **Triage partials before touching code.** Run `difftype.py` and `deadends.py` over the candidate list and work only
  functions with a small diff that is neither register-only nor layout-dependent. That rules out most dead ends before any attempt.
- **Cap each function at about 3 attempts** (one variant batch each), then record it in the dead-end list and move on.
- **Push to main** (`git push origin HEAD:main`, plain fast-forward) after each verified stage, and keep the working branch in sync.
- **Do not run clang-format.** The container's version differs from the repo's and rewrites untouched code (it also breaks `progress.py` by wrapping signatures).
- Max ~20 concurrent subagents. Sonnet agents with isolation=worktree: tell them to `git reset --hard` to the branch tip first
  (worktrees may start from an older commit, which makes regression baselines wrong).
- **Merging an agent's commit**: `git cherry-pick`, rebuild, run `regress.sh`. If it conflicts, apply the diff by hand; do not trust the
  agent's own "regressions" report (stale baseline) and do not trust a regress run on a conflicted tree (conflict markers = fake mass regressions).
- Agents that are stopped leave uncommitted WIP in their worktree: test it as a patch and keep it only if something goes up and nothing goes down.

## Naming passes (pure renames)
Pipeline: proposer agents (read-only, evidence required, skip when unsure) -> script merges and rejects duplicate/colliding names ->
verifier agents re-derive each claim independently (ACCEPT / REJECT / RENAME) -> one scripted word-boundary replace over
`src/legoland/*.[ch]` -> clean rebuild -> all 3255 scores must be identical -> commit + push.
`docs/naming-skipped.tsv` records every symbol already considered (named / skipped-no-evidence / rejected-by-verifier /
dropped-collision): exclude them from candidate lists. Only ~35% of candidates get a name; the easy ones are taken first.

## Partial matches: known dead ends (do not retry)
Several partials cannot be fixed from C because of how reccmp compares, not because of the code:
- A constant like `0x800000` that equals a data address is shown as `EditCursor+5184`: SetObjRectFlags, FUN_0045e080, AddBasicObject, AddObjectToMap.
- Indexed table calls `call [reg*4+TABLE]`: reccmp compares raw displacements: FUN_0046b2d0, FUN_0046a140, FUN_0041ec50, FUN_0041ec70, FUN_0041eca0.
- Loop-end pointers that land on CRT/float symbols in our layout: InitFreePlayLists, FUN_0049b350.
- SEH scope-table address in the prologue (layout dependent): WinMain, WriteModuleInfo.
Ones agents worked on and left unfinished (compiler scheduling/register differences): GameMain, LoadZoomer, FUN_00457a70, FUN_0043be70,
FUN_00415ae0, FUN_00429cf0, FUN_004232b0, FUN_00430b10, FUN_0040c8d0, FUN_0040f5b0, FUN_0040be00, FUN_004092b0,
FUN_0045fca0, FUN_0045fad0, DoMapAI, ObjectIsBuilt, GetTileCentre, FUN_0044fe80, FUN_00459970, FUN_0041a720, FUN_0042aa90.
Patterns that did match something: wrap a body in `do { ... } while (0)` to move register saves after an early return;
`for(;;)` with the early `return` inside the loop; `switch` instead of `if` for `dec/je` chains; reusing a pointer parameter as
a spill slot (ugly, "effective" match); replacing a temp pointer with an explicit if/else.
Best remaining value: partials between 60% and 95% in files not yet worked (see `tools/agent/partials.py`), then the
inline-asm stubs, which need `__asm` (CLAUDE.md).

### Round 3 (single session, no subagents)
Matched: FUN_004766f0, FUN_00441830, FUN_0042e560, FUN_00481170, FUN_0046da20, FindCarouselNode, FindWaterNodeByKey,
GetFirstRenderObject.
Improved: FUN_00407ad0 (95.0), FUN_00411fa0 (85.1), FUN_00469c80 (93.2), CreateSampleFromWAV (97.5).
What worked (look for these shapes first, they are cheap):
- `lea reg,[p+off]` immediately overwritten by `mov reg16,word ptr [p+off]` then `cmp reg16,[key]`: an inlined
  `memcmp(&node->id, key, 2)` (needs `<string.h>`). Node finders are `if (!node) return NULL;
  while (memcmp(&node->id, key, sizeof(node->id)) != 0) { node = node->next; if (!node) return NULL; } return node;`.
- A `break`/flag block placed after the function's `ret` (out of line) means the loop is a plain `while (cond)`, not
  `if (cond) do { } while (cond)` (FUN_004766f0).
- `and eax,0xffff; and eax,0xff` after a `unsigned short` call: `int i = Call() & 0xffff;` then index with `(unsigned char)i`
  (same shape as GetHedgeSpriteInfo).
- Decompiler-style strength-reduced loops (`offset += 0x14`, `x >= width` re-checks) match when rewritten as the natural
  `for (y...) for (x...) { tile = in-bounds ? &GameMap[y][x] : NULL; ... }` (FUN_00481170).
- `return` inside a nested `if` of a branch shares epilogues differently from an `if/else` with one `return` after it (FUN_0046da20).
- RIFF chunk scanners: `while (Read(&tag, 4) == 4) { if (tag == 'data') { ...; break; } skip chunk }` (CreateSampleFromWAV).
- Unsigned char fields read into an `int`/`short` local: the original `xor reg,reg; mov regl,[..]` vs our `movsx` tells the type.
- A `word` global split with `mov cl, dh` and no `and ecx,0xff` is two bytes: `((unsigned char *)&g)[0]` / `[1]`, not
  `(unsigned char)(g >> 8)` (GetFirstRenderObject).
Dead ends found this round (tried 5-15 variants each, register allocation or block placement only):
SetBlokePositionFromBNV (x87 stack order in the 3rd sqrt), SpeechParseWavHeader and SaveScripts and UnlinkGardenerOrder and
InitDirectSound and OpenAviAnim (early `return 0` blocks merged/duplicated differently from the original), FUN_00476d20,
ConvertWaveToPcm16, RES_OpenVolume, FUN_0046f2e0 (our compiler emits setcc), FUN_0041e4a0/FUN_0041e4b0 (movsx byte load is
narrowed away in C; probably C++ in the original), FUN_00462c60, PushSetTarget, FUN_00451390, FUN_00444a70 and FUN_00457970
(original has an extra `push ecx` local where ours reuses a dead parameter slot), FUN_0041ee40, FUN_004829c0, InsertChildIntoList,
LightUpthisDeleteIcon, FUN_00402490, FUN_00424700, FUN_0043ad90, FUN_004610f0, FUN_00418710, SetupControllers, FUN_0040e440,
FUN_0043aac0, FUN_00413450, FUN_004019c0, FUN_0042f0f0, FUN_004401b0, FUN_004070b0, FUN_00463460, RemoveObjectFromMap,
FUN_004966a0 (ours tail-duplicates the final call into each switch case), FUN_004779d0, PutObjOnMap.
Layout dead ends (flagged by `deadends.py`): LoadObjectClassAliasElements and FUN_004860f0 (loop-end pointer lands on a float
symbol), FUN_0041ed50, FUN_0041ece0, FUN_0041ed00, DoLowLevelAI (indexed table calls), FUN_00444970 (EditCursor+5184).

### Round 4 (triage first, 3 attempts per function)
Triage: 81 untried partials between 60% and 95% -> 17 layout-dependent, 0 register-only, 45 with large diffs -> 19 worked.
Improved: FUN_004428f0 (75.2), OpenAviMovie (89.1), RenderUsingRin (72.8), Calc_Item_Attractiveness (73.1), FUN_00488c80 (89.0).
Patterns that helped:
- Early-return blocks: `if (handle == NULL) { cleanup; return NULL; }` placed the failure block where the original has it (OpenAviMovie).
- Success block placed inline after the last check, with `goto ok` from the first test, matched the original block order (FUN_00488c80).
- Reusing an earlier local for the result (`rating = ...; counter = rating;`) kept it in eax like the original (Calc_Item_Attractiveness).
Dead ends (3 attempts each, scheduling/register allocation or tail merging): LLIDB_LoadTSFData, ValidateCursor,
UpdateControllerFromMouseData (the original reloads a field our compiler caches; only a `volatile` cast helps), PrintSavedGameDetails,
LoadPalette, FUN_00429f30 (a float argument goes through the FPU in the original), FUN_0046d850 and FUN_00402dc0 and FUN_00439ef0
(the original shares the tails of different switch cases/branches), FreePlayObjectList, FUN_0040f050, FUN_00465ee0,
FUN_00425e20 (our compiler reads the source of a struct copy instead of the copied global), EnterSaveGameDetails (larger stack frame).

### Round 5 (all remaining partials, 3 attempts per function)
Triage of the 163 partials not covered above: 37 layout-dependent, 8 register-only, 4 with inline `__asm`, the rest ranked by
how much decompiler residue the source still has (`* 0x14` offsets, `param_N` reuse, `do {} while` around `if`). That ranking was
the best predictor of a win. About 70 were worked; the remaining ones are listed at the end of this section.
Matched: CoptersFindNode, GetNthNextQueueNode, RenderThickBox, PrintSprite, FUN_0046ac00, FUN_0045cb20, FUN_0045d3d0,
FUN_0046a690 (register-only difference left).
Improved: CalculateMapRenderOrder (60.1), FUN_0045a660 (51.6), FUN_0046a5b0 (74.3), FUN_0046a960 (72.8), FUN_0046a3b0 (63.4),
GetScreenCoordsForObject (71.6), FUN_00471d90 (96.7), FUN_00455fc0 (77.6), HTBubbleHelp (74.0), FUN_00451280 (75.6),
GenerateNewImageFromZBuffer (71.2), FUN_0045d5d0 (42.3), FUN_00423200 (64.5), FUN_0045c900 (63.5), FUN_0041e000 (60.1),
FUN_0040c250 (42.9), FUN_00408f90 (41.0), FUN_0046f9a0 (41.5), FUN_0045ade0 (43.4), PrintSpriteEx (96.6).
Patterns that helped:
- Map scanners: replace the decompiler's `xoff = x * 0x14` byte offsets with natural `for (y...) for (x...)` loops and
  `tile = (x, y in bounds) ? &GameMap[y][x] : NULL`; writes go straight to `GameMap[y][x].field` (FUN_0045d3d0, FUN_0045cb20).
- A `(cond ? 1 : 2) & 0xff` added to a word: the `& 0xff` gives `and edx,0xff` instead of byte arithmetic (FUN_0045cb20).
- `cmp word ptr [x], 0; sete; test 0x10` is the precedence bug `!tile->flags & 0x10`; keep it and comment it (FUN_0046a690).
- Walk a copy of a pointer parameter instead of the parameter itself (GetNthNextQueueNode); compute a sub-expression at each call
  instead of in a temp (RenderThickBox).
- Branch order follows the source: when the original tests `mode != 0` first and calls the renderer on both VRAM paths, write it
  that way; `Hover = *(struct HoverInfo *)p` gives the interleaved register copy (PrintSprite).
- An original frame 16 bytes larger than ours usually means a RECT local: the text boxes draw their border from a `frame` RECT and
  pass `right - left` (FUN_00455fc0, HTBubbleHelp, FUN_00471d90).
- Decompiled `for (n = 0x2000; n; n--) *p++ = 0;` is `memset(..., 0, sizeof(array))` (intrinsic `rep stosd`, CalculateMapRenderOrder).
- `result = 0; break;` inside a loop instead of `return 0;` let the compiler save ebx after the early returns (FUN_00451280).
- Out-of-bounds bug found: GetTileBounds writes four ints, so callers need `int bounds[4]` (GetScreenCoordsForObject).
- Screen for hand-written asm first (`push ebp; mov ebp, esp`, `pushal`, `shrd`, `xchg` at the original start): SoftPrint_Clear and
  FUN_00488730 are asm; FUN_00485fe0, ApplyObjectOrientationToPerson, FUN_004251c0 and RenderTransSprite were flagged by that
  scan but not checked by hand. HASM_lego_sqrtf, HASM_lego_invsqrtf, Render3DPerson and LLSPlay contain `__asm`.
Dead ends (3 attempts or fewer, reason in brackets):
- Register allocation only: FUN_00461290, FUN_0042d560, GetTileBounds, RemoveNewObject, UpdateProfileCheckBoxIcons (an inline
  helper changes nothing), FUN_00465850, FUN_0040adb0, FUN_00467640 and FUN_00467f00 (same structure, every register rotated),
  FUN_004304e0, LoadBmpIntoImage, FUN_0041df00, FUN_00410180 (separate byte locals: +0.5 only), FUN_0043a7a0 (the original loads
  both struct fields into registers where ours uses a memory operand), Mechanic_Build (the only real diff is a 1-byte
  `mov eax,[GameMap]` vs `mov edx,[GameMap]` encoding).
- Stack slot order (declaration order has no effect): RenderTempleSlide, BuildObject, FUN_00412100, RenderPlaneRide (byte locals
  sit in dead parameter slots in the original), FUN_0041db90 (extra local where ours reuses a parameter slot, as FUN_00444a70).
- Tail merging: FUN_0041ef60 (the original keeps each switch case's call and epilogue separate, ours merges them), FUN_00413650
  (the original jumps into the middle of another case's `push` sequence), FUN_0042d610 (same ride-AI switch shape, skipped).
- GameMap row pointer: ours hoists `GameMap[y]` out of the inner loop where the original reloads it: FUN_0045c9c0, and the
  remaining diff in FUN_0045c900, FUN_0046a5b0 and FUN_0046a960; GetObjectUID shows the same diff (not attempted).
- Constants: CheckWorkerOnMouseStatus (the original keeps the constant 1 in ebp; `goto fail` and `for (;;)` compile the same),
  FUN_00459360 (the original keeps `var_4 = 1` in ebx and on the stack, ours folds it), FUN_00421e90 (using FLOAT_004ab43c instead
  of `3.0f` reorders the fmul), FUN_004284d0 (FLOAT_004ab45c plus array bases that differ by one element: layout).
- Scheduling: FUN_0040feb0 (globals loaded early, subtracted late), FUN_0041e660 (narrowed movsx, as FUN_0041e4a0).
- BubbleHelp: frame 8 bytes larger and a different setup order; needs a full rewrite, skipped.
Layout-dependent (from `deadends.py`, not attempted): FUN_0040a930, FUN_0040bab0, FUN_0040d6f0, FUN_00413b50, FUN_00416330,
FUN_00418fe0, lego_sqrtf_init, lego_invsqrtf_init, FUN_004280b0, FUN_00428f00, FUN_004294b0, RenderCarousel, FUN_0042c820,
FUN_00432d00, FUN_00435750, FUN_00438f10, FUN_00442980, FUN_004453a0, __BMPLoader, LoadColourTable, FUN_00452030, stackdump,
FUN_00458ee0, RemObjFromMap, RenderView, PointToIsoPlane, FUN_0045ca90, FUN_0045d770, FUN_0045eaf0, BuildCursorPtr, FUN_00471f10,
DrawPopUpInfo, FUN_004736f0, SaveGame, LoadGame, FUN_00484790, MusicThreadProc.
Register-only (from `difftype.py`, not attempted): GetLegColourOfBloke, GetArmColourOfBloke, LogFlumeEntranceRemoveObject,
RestoreBaseMap, RenderRestaurant2, LoadObjectiveEventList, RenderSpider, RenderSpinningBarrels.
Not attempted yet (mostly large AI/render functions; the first diff lines of the ones in brackets were checked and look like
register allocation or stack slots): FUN_004025d0, LoadPos, FUN_00477bd0, ParseScriptResFile, FUN_0043f0b0, FUN_0043c950,
CoptersUpdate, PrintCertificate, FUN_00407c30, FUN_00417430, FUN_00415220, LogFlumeEntranceAddObject, FUN_004608c0,
PrintProfileDetails, FUN_0043e410, FUN_00401f30, FUN_004316f0, LoadBaseMap, FUN_0040ae90, FUN_00466770, FUN_0042fbb0,
FUN_00442040, RenderFullMap, FUN_004227c0, FUN_00423a10, FUN_004064d0, RES_OpenFile, FUN_00433840, SearchJunglePathConnected,
FUN_004198a0, FUN_00411680, FUN_0043ea30, FUN_00402780 [RES_OpenFileFromVolume, FUN_00402150, FUN_00405bd0, RenderBuildObjectIcon,
FUN_0040bf70, FUN_00406660, DrawNewObjectPopup, FUN_00439950, RenderCursor, FUN_0043a1e0, FUN_0041c940, FUN_0040ca60, FUN_0043bac0].
