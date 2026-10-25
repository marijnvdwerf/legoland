# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp>=0.1.7"]
# ///
"""Transcribe original functions as `__declspec(naked)` inline asm.

For hand-written-assembly functions whose C + __asm version won't converge (see CLAUDE.md).
Needs a current build: names come from reccmp's database, so every function or global the
original references must already be annotated in the source.

    uv run tools/asm2naked.py 0x00466d80             # print the transcription
    uv run tools/asm2naked.py 0x00466d80 --apply     # replace the function in its .c file
    uv run tools/asm2naked.py 0x00441980 --name 0x4b7ae0=DAT_004b7ae0:12

`--name ADDR=SYMBOL[:SIZE]` names an address reccmp can't (a global it hasn't paired yet).
Branch targets become labels, absolute addresses become symbols. Anything it can't express
(an unnamed address, a jump table) is reported; a jump table means the function must be C.
After --apply: rebuild (--clean-first if you added globals/prototypes) and run
`./tools/verify -v ADDR`.
"""

from __future__ import annotations

import argparse
import bisect
import os
import re
import sys
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parent
os.environ["PATH"] = str(TOOLS_DIR) + os.pathsep + os.environ.get("PATH", "")
sys.path.insert(0, str(TOOLS_DIR))

import capstone
from capstone import x86
from reccmp.compare.core import Compare
from reccmp.project.detect import RecCmpProject

from progress import parse_annotations

SOURCE_DIR = Path("src/legoland")
# reccmp's names for CRT functions -> the name C code uses
CRT_NAMES = {
    "_sprintf": "sprintf",
    "_malloc": "malloc",
    "_free": "free",
    "_memset": "memset",
    "_memcpy": "memcpy",
}
BRANCHES = {"jmp", "call", "loop", "jecxz"} | set(
    "ja jae jb jbe jc je jg jge jl jle jna jnae jnb jnbe jnc jne jng jnge jnl jnle jno jnp jns jnz jo jp jpe jpo js jz".split()
)
STRING_OPS = re.compile(r"^(rep |repne |repe )?(movs|stos|lods|scas|cmps)[bwd]$")
# capstone mnemonics MSVC's inline assembler spells differently
MNEMONICS = {"pushal": "pushad", "popal": "popad", "pushfl": "pushfd", "popfl": "popfd"}
ADDRESS_RANGE = range(0x400000, 0x900000)


class Symbols:
    """Original address -> symbol name (+ offset), for everything reccmp has paired."""

    def __init__(self, compare: Compare, extra: dict[int, tuple[str, int]]):
        self.syms: dict[int, tuple[str, int]] = {}
        for ent in compare._db.get_all():
            if ent.orig_addr is None or not ent.name:
                continue
            if "::" in ent.name:
                # an import ("KERNEL32.dll::IsBadReadPtr"), even one our build doesn't link yet:
                # `call dword ptr [IsBadReadPtr]` assembles to the same indirect call
                self.syms[ent.orig_addr] = (ent.name.split("::")[-1], 4)
            elif ent.recomp_addr is not None:
                self.syms[ent.orig_addr] = (ent.name, ent.any_size())
        self.syms.update(extra)
        self.addrs = sorted(self.syms)

    def resolve(self, addr: int) -> str | None:
        i = bisect.bisect_right(self.addrs, addr) - 1
        if i < 0:
            return None
        base = self.addrs[i]
        name, size = self.syms[base]
        if not re.fullmatch(r"[A-Za-z_]\w*", name):
            return (
                None  # strings and other unnamed entities can't be referenced from asm
            )
        name = CRT_NAMES.get(name, name)
        if addr == base:
            return name
        if addr < base + max(size or 0, 1):
            return f"{name} + {addr - base:#x}"
        return None


def find_function(addr: int) -> tuple[Path, int]:
    marker = f"// FUNCTION: LEGOLAND 0x{addr:08x}"
    for src in sorted(SOURCE_DIR.glob("*.c")):
        lines = src.read_text(encoding="latin-1").split("\n")
        if marker in lines:
            return src, lines.index(marker)
    sys.exit(f"no // FUNCTION annotation for {addr:#x}")


def transcribe(
    image, symbols: Symbols, start: int, end: int
) -> tuple[list[str], list[str]]:
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    ins = list(md.disasm(image.read(start, end - start), start))
    while ins and ins[-1].mnemonic in ("nop", "int3"):
        ins.pop()
    targets = {
        i.operands[0].imm
        for i in ins
        if i.mnemonic in BRANCHES
        and i.operands
        and i.operands[0].type == x86.X86_OP_IMM
        and start <= i.operands[0].imm < end
    }
    problems, out = [], []
    for i in ins:
        if i.address in targets:
            out.append(f"L{i.address:x}:")
        mn, ops = MNEMONICS.get(i.mnemonic, i.mnemonic), i.op_str
        if STRING_OPS.match(mn):
            ops = ""
        elif mn in BRANCHES and i.operands and i.operands[0].type == x86.X86_OP_IMM:
            t = i.operands[0].imm
            if start <= t < end:
                ops = f"L{t:x}"
            elif (name := symbols.resolve(t)) is not None:
                ops = name
            else:
                problems.append(f"{i.address:#x}: branch to unnamed {t:#x}")
        else:
            for op in i.operands:
                if op.type == x86.X86_OP_MEM and op.mem.disp in ADDRESS_RANGE:
                    d = op.mem.disp
                    if start <= d < end:
                        problems.append(
                            f"{i.address:#x}: jump table / data inside the function (write it in C)"
                        )
                    elif (name := symbols.resolve(d)) is not None:
                        ops = ops.replace(f"{d:#x}", name)
                    else:
                        problems.append(f"{i.address:#x}: unnamed address {d:#x}")
                elif op.type == x86.X86_OP_IMM and op.imm in ADDRESS_RANGE:
                    # a constant that may or may not be an address: only named ones become `offset`
                    if (name := symbols.resolve(op.imm)) is not None:
                        ops = re.sub(rf"\b{op.imm:#x}\b", f"offset {name}", ops)
                    else:
                        problems.append(
                            f"{i.address:#x}: {op.imm:#x} left as a number (name it if it is an address)"
                        )
        ops = ops.replace("xword ptr", "tbyte ptr")
        if re.fullmatch(r"st\(\d\)", ops):
            if mn in ("faddp", "fmulp", "fsubp", "fsubrp", "fdivp", "fdivrp"):
                ops += ", st"  # MSVC wants both operands of the popping forms
            elif mn in ("fadd", "fmul", "fsub", "fsubr", "fdiv", "fdivr"):
                ops = "st, " + ops  # capstone's `fadd st(i)` is st(0) += st(i)
        if mn == "xchg" and all(o.type == x86.X86_OP_REG for o in i.operands):
            ops = ", ".join(
                reversed(ops.split(", "))
            )  # MSVC encodes the operands the other way round
        out.append(f"        {mn} {ops}".rstrip())
    return out, problems


def missing_prototypes(src: Path, line: int, body: list[str]) -> list[str]:
    """Functions the asm calls directly that src can't see a declaration of above `line`
    (asm won't declare them implicitly). Imports (`call dword ptr [X]`) come from system headers."""
    text = src.read_text(encoding="latin-1")
    headers = [SOURCE_DIR / h for h in re.findall(r'#include "([^"]+)"', text)]
    visible = "\n".join(h.read_text(encoding="latin-1") for h in headers if h.exists())
    visible += "\n".join(
        text.split("\n")[:line]
    )  # prototypes and definitions earlier in the file
    called = set(re.findall(r"^\s+call ([A-Za-z_]\w*)$", "\n".join(body), re.M))
    called -= {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp"}
    return sorted(
        n for n in called if not re.search(rf"\b{n}\s*\([^;{{]*\)\s*[;{{]", visible)
    )


def main():
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument("addrs", nargs="+", type=lambda s: int(s, 16))
    parser.add_argument(
        "--name", action="append", default=[], metavar="ADDR=SYMBOL[:SIZE]"
    )
    parser.add_argument(
        "--apply", action="store_true", help="replace the function in its .c file"
    )
    args = parser.parse_args()

    extra = {}
    for spec in args.name:
        addr, _, rest = spec.partition("=")
        name, _, size = rest.partition(":")
        extra[int(addr, 16)] = (name, int(size or "4", 0))

    target = RecCmpProject.from_directory(Path(".")).get("LEGOLAND")
    compare = Compare.from_target(target)
    symbols = Symbols(compare, extra)
    _, starts = parse_annotations(target.source_paths[0])

    for addr in args.addrs:
        end = starts[starts.index(addr) + 1]
        src, i = find_function(addr)
        lines = src.read_text(encoding="latin-1").split("\n")
        sig = lines[i + 1][: lines[i + 1].index("{")].rstrip()
        if "__declspec(naked)" not in sig:
            sig = "__declspec(naked) " + sig
        body, problems = transcribe(compare.orig_bin, symbols, addr, end)
        snippet = [
            "// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.",
            lines[i],
            sig + " {",
            "    __asm {",
            *body,
            "    }",
            "}",
        ]
        for p in problems:
            print(f"{addr:#x}: {p}", file=sys.stderr)
        for n in missing_prototypes(src, i, body):
            print(f"{addr:#x}: {src.name} needs a prototype for {n}", file=sys.stderr)
        if not args.apply:
            print("\n".join(snippet))
            continue
        # replace from the marker (and a "Hand-written assembly" comment right above it) to the end of the body
        first = i - 1 if lines[i - 1].startswith("// Hand-written assembly") else i
        last = i + 1
        if not lines[last].rstrip().endswith("}"):
            while lines[last] != "}":
                last += 1
        lines[first : last + 1] = snippet
        src.write_text("\n".join(lines), encoding="latin-1")
        print(f"{addr:#x}: wrote {len(body)} instructions into {src}")


if __name__ == "__main__":
    main()
