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

## Do-While Loops and Post-Increment (FUN_00490680)
- `do {} while (buffer[pos++] != '\r' && pos < size);` (empty body, post-increment in subscript) generates load-inc-cmp: `mov (%eax,%esi,1),%bl; inc %eax; cmp $0xd,%bl`. This is what the original compiler emits.
- `do { pos++; } while (buffer[pos-1] != '\r' && ...);` (separate increment) hoists `mov $0xd,%bl` BEFORE the loop and uses `cmp %bl,mem` inside -- completely different structure.
- `buffer[pos-1] ^ '\r'` generates XOR opcode (0xF3) instead of CMP (0xFB) -- semantically equivalent but different bytes.
- Adding a named `char c` variable shifts register allocation: `file` moves from EBX to EDI, `c` takes CL instead of BL. Removing the variable restores correct allocation.
- SIB byte order (`(%eax,%esi,1)` vs `(%esi,%eax,1)`) is controlled by which operand the compiler treats as "base" vs "index". Post-increment in the subscript expression makes the index variable (pos/EAX) the base.
- `while (buffer[pos++] != '\r')` (while-loop, not do-while) changes register allocation entirely (file→EDI, param_3→EBX). The `do {} while` form preserves the target's allocation.
