# Manager handover: getting more functions to match

For a Claude session acting as *manager*: it does not decompile itself, it briefs
workers (GPT-5.6 Luna subagents), reads their results critically, and commits. This is
what worked and what didn't in the session that produced commits `47fd6bb`..`736efe7`.

## Ground rules the owner set (do not relitigate)

- Manager only. Workers do the edits. Only Luna (`model: gpt-5.6-luna`) subagents, for
  everything including read-only exploration. Every prompt says "do not spawn
  subagents of your own".
- **Never start, resume, or stop an agent without being told.** Propose, then wait. One
  worker at a time on `src/` and `build/` — two writers race on the binary that
  `verify` reads.
- Workers stay inside `port3`. No reading around the filesystem; no decomp.me / MCP
  scratches ("luna works better in the tree"). Progress comes from the edit → build →
  `verify -v` loop, not from ingesting files.
- Banned in matched C, on top of `CLAUDE.md`: `goto`, `register`, `volatile`. Matches
  come from plain C shape only. No bare `{ ... }` block scopes: they never produced a
  match and no human writes them.
- Do not invent padding or struct hacks to force stack offsets. Stack comes last: get
  structure, basic blocks, branches, call order, statement order, then locals right.
- Do not point workers at Ghidra. Give them the asm file and matched neighbours; research
  paralysis is not progress. Workers iterate to 100% or explicitly declare exhaustion per
  function, and report that state rather than sending asm diffs to the manager.
- Never revert a worker's partial to `STUB()`; fix types in place instead. Prefer semantic
  partials over stubs.
- Commit when asked. Logical commits, one per concern (annotation fixes / TU matches).

## Where the work is

- `./tools/verify` → strip ANSI (`sed 's/\x1b\[[0-9;]*m//g'`) → lines like
  `  NAME (0xADDR) is NN.NN% similar to the original`. Save a baseline copy to `/tmp`
  before each pass; workers must prove no 100% function regressed against it.
- `uv run tools/callgraph.py [root] [--runtime]` — source call tree from WinMain, with
  🟢 (1.0), 🟡 (partial), and 🔴 (0%) nodes. Runtime/CRT calls are hidden unless
  `--runtime`; it takes about three seconds.
- `uv run tools/progress.py [tu]` — the table now classifies by effective ratio
  (Matched / Partial / Unmatched), rather than the old `STUB()` heuristic. With a TU
  argument it lists every function with marker, address, and percentage.
- `uv run tools/todo.py` — reachable 🔴 functions grouped by TU with original byte size;
  use smallest-first target picking. These replace the old grep one-liners.
- State at handover: total **74.58%**, 0 `[ERROR]` lines. `string.c` 12/13 (parked:
  `FUN_00498d00` 78.7%). `image_sprite.c` 35/38 (parked: `FUN_004978b0` 80.9%,
  `ReloadImageBitmapAndBuildSprites` 91.0%, `RemakeAllDetailDependentSprites` 98.9%).
  The current function-count table is 2,019 matched, 575 partial, 659 unmatched (62.1%
  by function weighting; verify's headline is effective byte-weighted).
- This session matched `draw.c`'s `InitHostSystemGPU`, `FUN_004661d0`, and `InitScreen`
  (all 100%), plus `screens.c`'s `FUN_004585c0`, `FUN_00458a50`, and `FUN_00459520`
  (100%*). Four screens partials remain parked at 91%, 85%, 60%, and 59%; see
  `docs/handover-screens.md` for function-level hypotheses, not duplicated here.
- Still parked in `draw.c`: `FUN_004659a0` 48%, `FUN_00466360` 79%, and `PushSetTarget`
  80%. One worker exhausted source-shape variants; remaining differences are
  register-allocation/ordering residue.
- The session's cast-removal sweep made `DDRAWENV` a real struct (including DX6-sized
  `DDCAPS` tails), replaced DirectDraw vtable calls with interface macros, and retyped
  RECT, surface, HWND, profile-buffer, element, and related globals/signatures. The
  `DirectDrawCreate` import stub now has its real `__stdcall` three-argument signature;
  stubs in `imports.c`/`crt.c` remain stubs. Every type change was verify-gated across
  all TUs. Surviving casts have evidence: Sprite dimensions stay unsigned short,
  GetTicks stays unsigned long, and the SortNode RECT overlay is intentional.

## How to brief a worker (what actually moved the needle)

Plain prose, goal first, explicit paths, verification at the end — no XML scaffolding
(see the `prompting-codex` skill). Include, every time:

1. **Goal as a result**: "these N functions in `src/legoland/<tu>.c` reach 100% with no
   other function regressing", smallest gap first.
2. **The loop**: `cmake --build build` then
   `./tools/verify -v 0x<addr> 2>&1 | sed 's/\x1b\[[0-9;]*m//g'`; `-` = original,
   `+` = ours. Header edits need `cmake --build build --clean-first`.
3. **Read the diff, not the percentage.** The % is a distance metric; it drops when a
   correct fix shifts layout. Workers should use the diff while iterating, keep variants
   that remove pairs even if the % falls, and continue to 100% or explicitly declare
   exhaustion per function. Do not ask the manager to interpret asm diffs; report only
   the final match/exhaustion state and any semantic type findings.
4. **Write it the way the file's author would.** Point at the already-matching functions
   in the same TU for idioms (declare-and-init, plain `while` with compound conditions,
   one reusable `i`, do-while when the first iteration is unconditional, standard CRT
   calls). Natural form first, then let the diff say where reality differs. Code
   contorted to the asm (duplicated stores, hoisted `i = 0`) is a last resort and must be
   justified by a diff pair (`FUN_00499040`'s duplicated NUL store *is* justified: the
   natural form was tested and the original really has two exits).
5. **Rules**: `CLAUDE.md` + `DECOMPILING.md`, plus no goto/register/volatile, decls in
   the TU's `.h`, `clang-format -i`, don't commit, don't touch `crt.c`/`imports.c`/
   `external/`/`toolchain/`.
6. **Concrete hypotheses if you have them** — from reading the diff yourself first (see
   next section). A worker with two specific, testable leads finishes in one pass; a
   worker told "figure it out" burns its budget reading files.
7. **Verification**: full verify to a `/tmp` file, zero `[ERROR]`, every other function in
   the TU still 100%, nothing that was 100% in the baseline dropped, `grep -n -E
   'goto|register|volatile'` empty, `git diff --stat` shows only the intended files.

## Read the diff yourself before and after each pass

Five minutes on `verify -v` saves a whole worker pass. Patterns seen this session and what
they meant:

| asm symptom | C cause that fixed it |
|---|---|
| Wrong global/data symbol on one side | wrong `DAT_*` referenced (`GetSprite`) |
| Two `add esp, N` cleanups vs one merged | statement between two calls differs; e.g. log-only on NULL path, unlink otherwise, one shared `free()` after the `if` (`FUN_004975b0`) |
| Same var read via two registers / extra `push ebx` | two index vars where the original has one reused (`FUN_00499040`) |
| `mov byte [x],0` interleaved with `rep stos` | aggregate initializer `char buf[N] = {0}`, not `memset` |
| `cmp bl,'"'; jne →inc; jmp; xor bl,bl; inc` (else path falls into the increment) | one fetch + one test (`c = i < n ? p[i++] : 0; if (c != '"') q++;`), not two branches each incrementing |
| Callee names differ (`getgamedir`, `fopen_wrapper`, `heap_free`) | **red herring** — those are `../port2` asm aliases; port3's own diff showed `_getcwd`/`fopen`/`free` already matching. Trust port3's `verify -v`, not port2 label names. |
| Whole-function EBX↔EDI (or EBX↔EBP) swap between two locals, plus rotated vs top-tested loop, plus a hoisted entry check | allocator tie-break; **not understood**. Declaration order, `total`/`file_size` merge, `for(;;)`+break, ternary fetch — none changed it. Parked: `FUN_00498d00`, `FUN_004978b0`. Don't spend a pass on these until someone finds the trigger; the likely lever is what stops MSVC6 rotating the loop (the duplicated bottom compare is the extra use that tips the allocator). |
| `mov ebp, imm` before vs after a `je` | single-pair scheduling residual (`RemakeAllDetailDependentSprites`); guard-vs-mask order variants didn't move it. |
| Byte index table plus jump table over a dense range | One source `switch` with shared bodies; do not declare the compiler-generated table as data (`FUN_00458ee0`). |
| `sub esp,N; mov [esp],a; mov [esp+4],b; call f` with no cleanup | `/O2` cdecl arguments in a pre-reserved outgoing area, not local struct stores (`HandleRideAI`). |
| `mov eax,[lit]; mov [dst],eax; mov al,[lit+4]; mov [dst+4],al` | Inlined five-byte literal copy; `lit+4` is its NUL, not a global. |
| `RECT r = {0}` where target assigns fields | Aggregate initializer pulls in ESI/`rep stos`; initialize the fields in the branch instead (`InitScreen`). |
| Parameter in ESI/EBX across a call vs fresh `[esp+8]` reads | Callee-saved register means live across a call or reused; source likely uses the parameter multiple times in the former. |

## Failure modes to watch for

- **Context-degraded workers ignore instructions.** After ~100+ tool calls a Luna worker
  shipped the same code twice, skipped an explicit change, and reported bare percentages
  after being told not to. Judge each report against what you asked; if two instructions
  in a row are ignored, tell the owner and propose a fresh worker rather than resuming.
- **"Completed" notifications are not death.** A worker can report and keep running (its
  task-id notifies again). `ListAgents` is the truth. Don't kill a worker because it's
  listed running after a report — check with the owner.
- **Resuming keeps context; it is cheap.** `SendMessage` to the same agent id with only the
  delta instruction works well when the worker is still coherent (the `image_sprite.c`
  worker took two follow-ups this way and closed a function each time).
- **Verify the worker's claims yourself** (`./tools/verify`, `git diff --stat`,
  `grep goto|register|volatile`) before committing. Cheap, and this session caught a
  stale claim once.
- Two workers rebuilding `build/` at once cause transient build breaks and inconsistent
  verify numbers. The one-worker rule is correct; if parallel work is unavoidable, tell
  each worker to sleep-and-retry on errors in files it does not own, and never verify
  while the other is mid-edit.
- Workers degrade after 300+ tool calls. This session's screens worker reached 464 and
  eventually tried a variadic prototype, a hard-coded address table, and caused a
  five-point regression. Keep a `/tmp` copy of the ORIGINAL pre-pass verify output and
  compare against it, not a worker's mid-session baseline. Workers also claim side-fixes
  that were already 100%; check before crediting them. "Explicitly exhausted" is the
  acceptable stop state; "briefly continuing" or silently moving on is not.
- **STRING annotations are load-bearing.** `legoland.exe` has no `.reloc`, so reccmp only
  learns original string addresses from `// STRING:` lines. Removing them cost 0.39pt
  across 227 functions. One per address, text must match the bytes exactly (an
  annotation with wrong text is a *source* bug — fix the literal).

## Suggested next targets

`uv run tools/todo.py` currently starts with these reachable 🔴 targets (original size):

| TU | Function | Bytes |
|---|---|---:|
| certificate | `FUN_00451e20` | 80 |
| build | `ProcessBuildingTimes` | 112 |
| mapscreen | `DrawMapScreen` | 128 |
| man3d | `SetPersonRotation` | 144 |
| gamemain | `FUN_004781b0` | 64 |
| gamemain | `FUN_004781f0` | 144 |

The table is grouped by TU, so use the smallest reachable function rather than blindly
following its display order. Other useful queues are the `lpVtbl`→interface-macro sweep
across roughly 40 remaining sites in `render.c`/`gfx.c`/`title.c`/`text.c`, and the goto
sweep across 28 TUs (about 105 gotos; exclude `challenge.c`, one giant off-limits
function). A fresh worker can revisit the four parked `screens.c` partials using
`docs/handover-screens.md`. TUs with many unmatched bodies remain `map_object`,
`jungle_cruise`, `boating_school`, `tilemap`, and `eatery`; leave the allocator-swap
class parked.
