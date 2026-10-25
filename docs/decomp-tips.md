# MSVC6 /O2 Matching Decomp Cheat Sheet

## Register Allocation (RECT fields, struct-by-value args)
- **Field assignment order in source controls register allocation.** MSVC6 assigns registers based on source order of struct field writes, NOT struct layout order.
- After a function call (which clobbers eax/ecx/edx), the compiler picks caller-saved registers first (ecx, edx), then callee-saved (esi, edi). If a callee-saved reg held a now-dead value, it becomes available too.
- To get a specific register for "zero" (e.g., `xor edx` instead of `xor ecx`): put a non-zero field assignment BEFORE the zero field. The non-zero field consumes eax/ecx, pushing zero to edx.
- Example: `rc.top=0x102; rc.bottom=0x130; rc.left=0; rc.right=0x280;` gives `(edx=zero, eax=top, esi=right, ecx=bottom)`.

## Array/Variable Initialization
- `int hit[3] = {1}` (C89 partial initializer) generates `xor eax; mov eax,slot1; mov eax,slot2` (register-based zero stores) + `movl $1,slot0` (immediate).
- Explicit `hit[1]=0; hit[2]=0;` causes MSVC6 to allocate edi as a **persistent zero register** (used for all subsequent `push $0x0` args too), pulling in ebx as a 3rd callee-saved register. This cascades badly.
- `hit[0]=0; hit[2]=0; hit[0]=1;` (dead store trick) does NOT generate stores for hit[1] -- only the surviving stores appear.

## Ternary vs Bitwise for Conditional Assignment
- `expr ? 0x8c : -0x8c` produces: `neg; sbb eax,eax; and $0x118; add $0xffffff74` (correct `add` with negative immediate).
- `(-(unsigned)(expr != 0) & 0x118) - 0x8c` produces extra `neg; neg` pairs and `sub $0x8c` instead of `add $0xffffff74`. The 5-byte `ADD EAX,imm32` (opcode 0x05) vs 6-byte `SUB r/m32,imm32` (opcode 0x81) matters for matching.

## General MSVC6 /O2 Patterns
- Compiler freely reorders independent stores/loads for scheduling. Source order affects register *assignment*, not necessarily instruction *emission* order.
- `push $0x0` (immediate) vs `push %reg` (register holding zero): determined by whether the compiler decided to materialize a zero register. More zero-uses in a function = more likely to use a dedicated zero register.
- `dec %eax` / `inc %eax` generated from `x + -1` / `x + 1` (or `x - 1`).
- MSVC6 C89 mode: no mid-block declarations. All variables must be declared at top of block.
- The `register` keyword and inline `__asm` are NOT needed for matching compiler-generated code -- pure C source
  order changes suffice. `__asm` is only for functions that were hand-written assembly (see below).

## Signedness and Extension
- `signed char` → `int` assignment generates a single `movsbl` (movsx) instruction.
- `unsigned char` → `unsigned int` generates `xor %reg,%reg` + `mov byte` (two instructions, zero-extension).
- The target asm tells you the signedness — if you see `movsbl`, the source type is signed.
- Type choice can affect instruction scheduling even for unrelated instructions (e.g., stack cleanup `add $0x10,%esp` moves before or after loads depending on sign-extension vs zero-extension).
- `(int)(unsigned short)field` gives `xor eax; mov ax` (zero-extend 16→32). Plain `(int)field` on a `short` gives `movsx` (sign-extend). The asm tells you which cast the original used.

## Parameter Types and Load Width
- `unsigned int param_2` (wider type) can produce a narrower byte load `MOV BL, [ESP+0xc]` when only the low byte is used (for bit tests). The compiler generates the minimal load.
- `unsigned char param_2` (narrower type) paradoxically produces a dword load + mask: `MOV [ESP+0xc], %ebx; AND $0xff, %ebx`. The compiler loads the full stack slot and masks to ensure proper width.
- When you see a byte load into a callee-saved register (e.g., `mov bl, [esp+N]`), try declaring the parameter as `unsigned int`.

## Loop Structure: while+break vs do-while
- `while (cond) { ...; if (x) break; ... }` and `do { ... } while (cond)` generate different branch layouts.
- Match the loop structure to the target's jump pattern — a forward `jne` to a label past the loop body suggests `while` + `break`.

## Return Types and Width
- Declaring a function as returning `unsigned short` when it actually returns `unsigned int` causes spurious `and $0xffff,%eax` zero-extension at the call site. Match the return type to avoid this.
- `unsigned char` return type generates `mov $0x1,%al` (8-bit), not `mov $0x1,%eax`.

## Post-Call Register Allocation
- After a `call` (which clobbers eax/ecx/edx), the compiler picks caller-saved registers first, in source order of the assignments: ecx, edx, eax for consecutive values.
- Button callback `param_2 & 2` compiles to `testb $0x2,0x8(%esp)` — direct byte test on stack, no register load.

## Temp Variables vs Inline Calls
- Using `unsigned int x = subcall()` as a temp forces the sub-call to evaluate BEFORE pushing outer call args. Without the temp (inline), the compiler evaluates args right-to-left, pushing constants first, then doing the sub-call with per-call stack cleanup — different code.
- General rule: when a sub-call result is used as a non-last argument to an outer call, a temp variable forces the sub-call to evaluate early.
- If the target calls the same function twice with the same arg (e.g., GetString), duplicate the call inline rather than caching in a temp. A temp forces a callee-saved register (EBX) to keep the value alive, changing the whole register allocation.

## Stack Cleanup Batching
- MSVC6 batches `add %esp` stack cleanup across multiple consecutive calls. Identifying batch boundaries (where the `add esp, 0xNN` appears) is essential for matching.
- Count all pushed bytes across batched calls: e.g., 4×LoadSprite (2 args each = 0x20) + InsertIcon (4 args = 0x10) + GetString (1 arg = 0x04) = 0x34. The single `add $0x34, %esp` appears after the last call in the batch.

## Global Pointer vs Local Pointer
- A local `struct IconNode *icon = InsertIcon(...)` forces MSVC6 to allocate a callee-saved register (ESI) to keep the pointer alive across subsequent calls, adding `push %esi` / `pop %esi` and shifting all ESP-relative parameter accesses by 4.
- If the target asm has NO callee-saved register push and instead reloads from a global (`mov _DAT_xxx, %reg`) before every field access, use the global directly: `PopUpCloseIcon = InsertIcon(...); PopUpCloseIcon->string_id = 0x4e;`
- If the target DOES have `push %esi` and uses `%esi` as the pointer across field writes without reloading, then a local variable is correct.
- The stack offset tells you which case: `param_1` at `[ESP+0x28]` (no extra pushes) vs `[ESP+0x2c]` (one callee-saved push).

## Flag OR Patterns
- Two separate `flags |= 0x2000; flags |= 0x4002;` produce two read-modify-write sequences. MSVC6 does NOT merge them even when it could.
- `|= 0x2000` compiles to `OR DH, 0x20` (byte OR on high byte of low word — narrower encoding). `|= 0x4002` compiles to `OR ECX, 0x4002` (full dword). The encoding width is driven by the constant value.
- Each OR reloads the pointer from the global when there's no local variable holding it.

## Persistent Constants in Callee-Saved Registers
- When the same constant is used repeatedly (e.g., `0x6002` OR'd into every icon's flags), MSVC6 hoists it into a callee-saved register (EDI) rather than re-materializing each time.
- More zero-uses in a function = more likely to use a dedicated zero register. With 3 zero stores the compiler may use immediates; with 4+ it typically allocates a callee-saved register.

## Lazy Callee-Saved Register Push
- If a variable's first use is inside a conditional block, its callee-saved register gets pushed at that block's entry point, not at function entry. Look for `push %ebx` appearing AFTER `push %esi`/`push %edi` and inside a branch.
- The loop-back jump target is AFTER the `push; mov` sequence, confirming those instructions run only once (loop setup, not per-iteration).
- To get the push *after* an early-exit test (`test byte ptr [esp+8], 2; je end; push esi`), wrap the guarded
  body in `do { ... } while (0);` and turn its early `return`s into if/else, with one `return` after the loop:
  `if ((flags & 2) != 0) { do { if (a) { ... } else { ... } } while (0); return 2; } return 1;`.
  Without the wrapper MSVC6 pushes in the prologue (and often preloads the flag byte into `al`). Matched
  FUN_00475080, FUN_004751a0, FUN_0048b000, FUN_0048bc20, FUN_00470000, FUN_0046d980.

## Parameter Stack Slot Reuse
- When a parameter is only tested once early (via memory-form `testb $imm, N(%esp)` without loading into a register), MSVC6 may reuse its stack slot for a local variable.

## Switch Jump Table Residuals
- MSVC6 switch jump tables use internal local labels (`$L115`) in fresh compiles, while the original binary uses `.text`-base + addend relocations. Both resolve to the same addresses at link time, but decomp.me shows a small residual (~45 points). This is unavoidable from pure C — reccmp normalizes these correctly.
- Switch on values 1–N generates `DEC EAX; CMP EAX, N-1; JA default` — subtracts the minimum case, then unsigned range check.

## Switch vs If-Else
- `sub imm8; jcc` with NO intervening `test` = switch statement (compiler reuses flags from the subtraction).
- `add $0xFFFFFFxx; test; jcc` = nested if-else (extra `test` instruction is the giveaway).
- Switch case BODY layout is by complexity, not source order — complex cases are placed later in binary even if they appear first in source. The comparison chain follows source order.

## Branch Layout and Return Patterns
- MSVC6 does NOT factor out shared tail calls after if/else — each branch gets its own copy of shared post-branch calls and its own `ret`. Don't try to factor common calls after the if/else.
- Multiple early-exit conditions jumping to the SAME far label = shared late return block. Use nested `if (cond) { ... return 2; } return 1;` to force this layout.

## Struct-by-Value on Stack
- `int locals[3] = {1}` (C89 partial init) generates `xor eax; mov $1,[esp]; mov eax,[esp+4]; mov eax,[esp+8]` — one zero register reused. Explicit `locals[1]=0; locals[2]=0;` allocates a persistent zero register (EDI) that cascades through the whole function.
- `sub esp, 0x10` for by-value struct args happens INSIDE each branch, not before the branch.
- `push` of shared args (same value across branches) happens BEFORE the conditional jump.

## Clamp Pattern
- `mem = val; if (mem > max) mem = max;` (unconditional store, then conditional overwrite) generates double-write: `mov %ax, mem; jle over; movw $max, mem`.
- `if (val > max) val = max; mem = val;` (clamp first, then store) generates completely different code.

## Preventing Store Hoisting
- When a store (e.g., `DAT = 0`) appears after an if/else block on all non-goto paths, MSVC6 may hoist it BEFORE the branch, creating a spurious early store and changing register allocation.
- Fix: place the store explicitly inside EACH branch rather than after the if/else. The compiler then merges identical stores to the merge point without hoisting.
- This can change which callee-saved register is chosen (e.g., ESI→EBX) because hoisting creates extra register pressure at the branch point.

## Struct Copy vs Individual Globals
- Struct copy (`*param = global_struct`) gives correct register allocation (compiler preloads destination pointer into a callee-saved register) but generates `_StructBase+0x4` symbol references for fields.
- Individual global access (`param[1] = DAT_xxx`) gives correct standalone symbol names but different register allocation.
- This is a known limitation — some functions may not reach 100% if the original used individual globals but the register allocation only matches with struct copy semantics.

## Do-While Loops and Post-Increment (ReadResourceLines)
- `do {} while (buffer[pos++] != '\r' && pos < size);` (empty body, post-increment in subscript) generates load-inc-cmp: `mov (%eax,%esi,1),%bl; inc %eax; cmp $0xd,%bl`. This is what the original compiler emits.
- `do { pos++; } while (buffer[pos-1] != '\r' && ...);` (separate increment) hoists `mov $0xd,%bl` BEFORE the loop and uses `cmp %bl,mem` inside -- completely different structure.
- `buffer[pos-1] ^ '\r'` generates XOR opcode (0xF3) instead of CMP (0xFB) -- semantically equivalent but different bytes.
- Adding a named `char c` variable shifts register allocation: `file` moves from EBX to EDI, `c` takes CL instead of BL. Removing the variable restores correct allocation.
- SIB byte order (`(%eax,%esi,1)` vs `(%esi,%eax,1)`) is controlled by which operand the compiler treats as "base" vs "index". Post-increment in the subscript expression makes the index variable (pos/EAX) the base.
- `while (buffer[pos++] != '\r')` (while-loop, not do-while) changes register allocation entirely (file→EDI, param_3→EBX). The `do {} while` form preserves the target's allocation.

## Uninitialized Locals Read From Dead Argument Slots (FUN_0040b420, FUN_0040f920)
- If the asm "falls back" to an argument that the code no longer uses (e.g. `mov edi,[esp+0x18]` where
  `[esp+0x18]` is a dead parameter slot), the source probably used an **uninitialized** local on that path.
  MSVC reads the variable's home slot, which it had placed in the dead argument slot.
- Write it that way: `int frame; if (lls != NULL) frame = lls->frame; ... LLSSetFrame(x, frame);`.
  Writing the fallback as `(int)param` keeps the parameter alive in a register and changes the allocation.
- If several sections each read such a value, give each block its own short-lived local.

## Block Scope Decides Stack Layout
- Declaration order of locals almost never changes the frame at /O2; block scope does. Declaring a local
  (e.g. `struct Point off;`) inside each block that uses it often fixes the whole layout.
- Two ints that are really a position may need to be one `struct Point`: a struct is never placed in a dead
  argument slot, two scalars can be.

## Functions With an ebp Frame (`push ebp; mov ebp,esp`)
At /O2, MSVC6 omits the frame pointer. A frame in the original means one of:
1. **Inline asm in the function.** Fingerprints: `fistp DWORD` behind `fstp [x]; fld [x]` (fast float->int
   macro), `shrd`/`shld` (fixed-point), `rdtsc` wrapped in `push eax; push edx` (timing macro), `pusha/popa`,
   `xchg`, `fstcw/fldcw`, `push eax; lea eax,[func]; mov [g],eax; pop eax`. MSVC never emits these; the
   function cannot be matched in pure C: write those parts with inline `__asm`.
2. **Unoptimized code** (every local in `[ebp-N]`, loops as `jmp` to the condition, no register
   allocation). Wrap the function in `#pragma optimize("", off)` / `#pragma optimize("", on)`, placing the
   first pragma *above* the `// FUNCTION:` annotation (a line between the annotation and the function makes
   reccmp drop it). In /Od the slot order of locals depends on their **names** (symbol hash), not on
   declaration order.
3. **Optimized body with a frame** and no inline-asm fingerprint: `#pragma optimize("y", off)` (frame
   pointer omission off) reproduces it.

## Walking a Global List: `mov esi,eax` Before the Test (FUN_0040d210, FUN_00408f30)
- When the original loads the list head into eax, tests it, and copies it to the loop register
  (`mov eax,[head]; mov esi,eax; test eax,eax`), write
  `node = head; if (head != NULL) for (; node != NULL; node = node->next) { ... }`.

## Globals Reloaded Everywhere
- If the original reloads a global after every call or store even where nothing could alias it, try
  declaring it `volatile` (TextCellCount, the text cell count, fixed four text.c functions). Check every
  other user still matches — it is not always right (DAT_006687a0 got worse).

## COM Calls (DirectMusic/DirectSound)
- Type the COM objects as `struct X { struct XVtbl *vtable; }` with `__stdcall` function pointers at the
  vtable offsets the asm uses (`call [ecx+0x4c]` = slot 0x13), and call `p->vtable->Method(p, ...)`; retype
  the global that holds the object so no casts are needed. The music thread (MusicThreadProc) types the
  DirectMusic loader/performance/composer/port/segment this way in sound_sfx.h.
- MSVC6's /O2 includes /Gf (string pooling): a literal repeated in a macro is emitted once. Annotate a shared
  message with `// STRING:` above a `#define NAME "..."` line and use NAME in the macro body.

## Scores That Move Without Touching the Function
- FUN_00425e20 (castle.c) changes its register assignment (one commutative `add`) when unrelated
  declarations are added to or retyped in globals.h -- apparently symbol-table order, not its own code. It
  has moved between 89% and 93% this way; after a globals.h change, re-tune its `mid` operand order
  rather than treating the drop as a regression in the new code.
- MusicThreadProc (sound_sfx.c, 11 KB) fell from 100% to 99.89% (an `add esp,4` scheduled one
  instruction later and two import loads merged) when one `extern const char BuildMenuName[];` was added
  in the middle of globals.h; moving the new declarations to the END of globals.h restored it (2026-10-17).
  Keep new globals.h declarations that the matched code does not depend on at the end of the file. After a
  header change, compare the per-function report (`uv run tools/report.py`), not only the totals: one
  large function losing 100% barely moves the fuzzy %, but takes all its bytes out of "code matched".

## Shared Cleanup Paths: `goto fail`
- When several failure checks jump into one cleanup chain that runs the releases in reverse order and
  falls into a single `return`, the original was usually written with named labels:
  `if (FAILED(hr)) goto release_perf; ... release_perf: perf->Release(); release_composer: ...`.
  Nested ifs or duplicated cleanup code give the wrong block layout (SaveGame: 62.77% -> 91.55% with `goto fail`).


## Matching Hand-Written Assembly
- The original's asm was MSVC inline `__asm` inside C functions, so the frame, register saves and the C around
  it were compiled. Rebuild that shape first; transcribe the whole function as `__declspec(naked)` only when
  the C around the asm won't converge (see CLAUDE.md).
- An asm block that names a variable keeps that variable in memory. If the original reloads a parameter at
  every use (or keeps a float temp and an `fistp` result in its slot), name the parameter in the asm
  (`fld dword ptr index` / `fistp index`). `volatile` gets the same reloads in C. MSVC6 packs other locals into
  dead parameter slots, so the original's asm often uses `[ebp+8]` / `[ebp+0xc]` as scratch.
- A float literal in a double expression is folded to a `qword` constant. `(float)(x * d) * 5.0f` keeps the
  4-byte literal (the cast emits no code). reccmp pairs the original's float constants (type FLOAT) with
  compiler literals only, never with a declared global of the same value.
- When code addresses one array through its neighbour (`DAT_00612210 - 8`), the two must be adjacent in our
  image too: initialise both (`= {0}`) and define them next to each other, so they go into .data in order.
- Converting a disassembly listing (capstone) to MSVC inline-asm syntax: `pushal`/`popal` are `pushad`/`popad`;
  `faddp st(1)` must be `faddp st(1), st` and `fadd st(1)` must be `fadd st, st(1)`; string ops take no operands
  (`rep movsw`); MSVC encodes `xchg a, b` with the operands the other way round, so write `xchg b, a`; imports
  are called as `call dword ptr [IsBadReadPtr]`; any function or global the asm names needs a prototype or
  `extern` before it (asm will not use an implicit declaration).
- reccmp treats an immediate as an address when it falls inside any known symbol. A constant like `0x500000`
  can land inside a big array in one image and not the other. `tools/reccmp_fixes.py` (loaded by `tools/verify`
  and `progress.py`) counts such a line as equal when its raw text is identical in both images and one side
  kept the plain number. Lines where both sides resolve to different symbols still count as different.
- A naked transcription must not contain raw original addresses (`mov edi, 0x641004`): they assemble to the
  same bytes but point at the original's layout, not ours. `asm2naked.py` names every address it can and
  reports the rest.

## Switch case order changes tail merging

- MSVC6 merges identical `return N` blocks (cross-jumping), and which copies survive depends on the order the
  `case` labels appear in the source, not on their values. If the original keeps a `return 3` as its own block
  where ours jumps to a shared one (or the jump table points at a different copy), try every order of the
  case blocks: FUN_004092b0 went 84.6% -> 100% with `case 1, case 0, case 2, case 3`.
- Likewise a jump-table-only difference can mean the original split shared labels
  (`case 1: case 2: ... /* fall through */ case 3: case 4:`), as in OctopusCafeUpdate.

## Frame-size and slot diagnostics (2026-10-11)

- **Compare the frame size first** (`sub esp, N`, or `mov eax, N; call __chkstk`). If the original's is
  bigger, the source had more distinct variables than ours: look for a variable reused in two places (a
  `struct Point pt` used for map coordinates in one loop and for corners in another) and give the second use
  its own block-scoped variable. RenderView went 69.8% -> 72.5% that way (+8 of the missing 12 bytes).
  If ours is bigger, a variable the original packs into a dead parameter slot is staying live in ours.
- **Which variable packs into a dead parameter slot** depends on the statement order of the setup code:
  FUN_0041df00 moved `lo = 0` after the struct copy, and `lo` (not `hi`) went into obj's slot
  (67.8% -> 71.4%).
- **A `volatile` that raises the score is a clue, not a fix.** It means the original keeps that value in
  memory, or that the variable needs more weight in the slot ordering. Look for the source shape behind it
  (value live across a call, read through its stack slot, more references) before keeping the volatile.
- **Save/load records:** if the original stores a zero-extended dword where ours stores a word or byte, the
  record field is wider than we declared (BlokeSave.prev_param/prev_action, as WorkerSave already had).
- **`// STRING:` annotations must sit directly above the line with the literal**, not above a `{` block; a
  misplaced one is silently ignored and the string shows as a raw address in the diff.
