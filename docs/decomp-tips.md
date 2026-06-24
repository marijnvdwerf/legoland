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
- The `register` keyword and inline `__asm` are NOT needed for matching -- pure C source order changes suffice.

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

## Persistent Constants in Callee-Saved Registers
- When the same constant is used repeatedly (e.g., `0x6002` OR'd into every icon's flags), MSVC6 hoists it into a callee-saved register (EDI) rather than re-materializing each time.
- More zero-uses in a function = more likely to use a dedicated zero register. With 3 zero stores the compiler may use immediates; with 4+ it typically allocates a callee-saved register.

## Lazy Callee-Saved Register Push
- If a variable's first use is inside a conditional block, its callee-saved register gets pushed at that block's entry point, not at function entry. Look for `push %ebx` appearing AFTER `push %esi`/`push %edi` and inside a branch.
- The loop-back jump target is AFTER the `push; mov` sequence, confirming those instructions run only once (loop setup, not per-iteration).

## Parameter Stack Slot Reuse
- When a parameter is only tested once early (via memory-form `testb $imm, N(%esp)` without loading into a register), MSVC6 may reuse its stack slot for a local variable.

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

## Do-While Loops and Post-Increment (FUN_00490680)
- `do {} while (buffer[pos++] != '\r' && pos < size);` (empty body, post-increment in subscript) generates load-inc-cmp: `mov (%eax,%esi,1),%bl; inc %eax; cmp $0xd,%bl`. This is what the original compiler emits.
- `do { pos++; } while (buffer[pos-1] != '\r' && ...);` (separate increment) hoists `mov $0xd,%bl` BEFORE the loop and uses `cmp %bl,mem` inside -- completely different structure.
- `buffer[pos-1] ^ '\r'` generates XOR opcode (0xF3) instead of CMP (0xFB) -- semantically equivalent but different bytes.
- Adding a named `char c` variable shifts register allocation: `file` moves from EBX to EDI, `c` takes CL instead of BL. Removing the variable restores correct allocation.
- SIB byte order (`(%eax,%esi,1)` vs `(%esi,%eax,1)`) is controlled by which operand the compiler treats as "base" vs "index". Post-increment in the subscript expression makes the index variable (pos/EAX) the base.
- `while (buffer[pos++] != '\r')` (while-loop, not do-while) changes register allocation entirely (file→EDI, param_3→EBX). The `do {} while` form preserves the target's allocation.
