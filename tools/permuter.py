# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp>=0.1.7"]
# ///
"""Random-search permuter for near-miss functions (the idea of decomp-permuter, for MSVC6).

    uv run tools/permuter.py 0x0045ff00                 # 10 minutes on RenderCursor
    uv run tools/permuter.py 0x0045ff00 --minutes 60 --seed 3
    uv run tools/permuter.py 0x0045ff00 --score-only    # just score the current source

It compiles only the function's translation unit (a temporary copy outside the repo, same /O2 flags
as the build), cuts the function out of the .obj, and scores it against the original with a
reccmp-like instruction diff: calls and data references become symbol names (from the source
annotations, the exe's imports and the .obj's relocations), float constants become their value and
strings their text, and jumps become displacements. Each round applies 1-3 random source mutations
that keep the meaning (operand swaps, comparison flips, ++/+= forms, if/else swaps, statement and
declaration reordering, ...) to the best version so far and keeps whatever scores at least as well.

Improvements are written to permuter_out/<addr>/ (gitignored). Nothing in src/ is modified: confirm a
candidate by putting it in the source, building, and running `./tools/verify -v <addr>`. The scorer
is close to reccmp's but not identical, and some mutations (statement reordering) can change
behaviour, so read every candidate before keeping it.
"""

from __future__ import annotations

import argparse
import bisect
import difflib
import hashlib
import os
import random
import re
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
SRC = ROOT / "src" / "legoland"
CL = ROOT / "cmake" / "wrappers" / "cl"
sys.path.insert(0, str(TOOLS))

import capstone  # noqa: E402
from capstone import x86  # noqa: E402
from reccmp.formats.detect import detect_image  # noqa: E402

from progress import parse_annotations  # noqa: E402

MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
MD.detail = True
JUMPS = {
    "jmp", "loop", "jecxz", "ja", "jae", "jb", "jbe", "je", "jg", "jge", "jl", "jle", "jne", "jno", "jnp",
    "jns", "jo", "jp", "js",
}  # fmt: skip


def bare(name: str) -> str:
    """Compare symbol names without C decoration: _f, _f@8, @f@8, __imp__f@8 -> f."""
    if name.startswith("__imp_"):
        name = name[6:]
    name = name.lstrip("_@")
    return re.sub(r"@\d+$", "", name)


# ---------------------------------------------------------------- shared listing


SENTINEL = 0x7EADBEEF


def render(raw: bytes, address: int, field: int | None, name: str | None) -> str:
    """Disassemble one instruction; if `field` is given, print the 4-byte value at that offset as `name`."""
    if field is not None:
        b = bytearray(raw)
        b[field : field + 4] = struct.pack("<I", SENTINEL)
        ins = next(MD.disasm(bytes(b), address))
        text = f"{ins.mnemonic} {ins.op_str}".strip()
        return text.replace(f"{SENTINEL:#x}", name)
    ins = next(MD.disasm(raw, address))
    return f"{ins.mnemonic} {ins.op_str}".strip()


def fp_size(ins) -> int:
    if not ins.mnemonic.startswith("f"):
        return 0
    return 8 if "qword" in ins.op_str else 4 if "dword" in ins.op_str else 0


def listing(code: bytes, names: dict, tables: dict, entry_bias: int) -> list[str]:
    """Instruction strings for `code` (disassembled at address 0).

    names:  instruction offset -> (field offset in instruction, name) for operands that are addresses,
            or (None, name) to replace the whole operand of a call/jmp
    tables: data offset inside the function -> "dword" (jump table) or "byte" (index table)
    entry_bias: subtracted from jump-table entries to make them function-relative
    """
    code_end = min(tables) if tables else len(code)
    out = []
    for ins in MD.disasm(code[:code_end], 0):
        raw = code[ins.address : ins.address + ins.size]
        if ins.address in names:
            field, name = names[ins.address]
            if field is None:
                out.append(f"{ins.mnemonic} {name}")
            else:
                out.append(render(raw, ins.address, field, name))
        elif ins.mnemonic in JUMPS or ins.mnemonic == "call":
            op = ins.operands[0] if ins.operands else None
            if op is not None and op.type == x86.X86_OP_IMM:
                out.append(f"{ins.mnemonic} {op.imm - (ins.address + ins.size):+#x}")
            else:
                out.append(render(raw, ins.address, None, None))
        else:
            out.append(render(raw, ins.address, None, None))
    while out and out[-1] in ("nop", "int3"):
        out.pop()
    starts = sorted(tables) + [len(code)]
    for k, t in enumerate(starts[:-1]):
        nxt = starts[k + 1]
        if tables[t] == "dword":
            for o in range(t, nxt - 3, 4):
                v = struct.unpack_from("<I", code, o)[0] - entry_bias
                if not (0 <= v < len(code)):
                    break
                out.append(f"case {v:#x}")
        else:
            data = code[t:nxt].rstrip(b"\x90\xcc")
            out.append("bytes " + data.hex())
    return out


# ---------------------------------------------------------------- original side


def annotations():
    funcs, globals_, strings = {}, {}, set()
    for c in sorted(SRC.glob("*.c")):
        lines = c.read_text(encoding="latin-1").split("\n")
        for i, line in enumerate(lines):
            m = re.match(r"\s*// (FUNCTION|STUB|LIBRARY|GLOBAL|STRING): LEGOLAND (0x[0-9a-f]+)", line)
            if not m:
                continue
            kind, addr = m.group(1), int(m.group(2), 16)
            nxt = lines[i + 1] if i + 1 < len(lines) else ""
            if kind == "STRING":
                strings.add(addr)
            elif kind == "GLOBAL":
                decl = nxt.split("=")[0]
                n = re.search(r"\(\s*\*\s*(\w+)", decl) or re.search(r"(\w+)\s*(\[[^\]]*\]\s*)*;?\s*$", decl.strip())
                if n:
                    globals_[addr] = n.group(1)
            else:
                n = re.search(r"([A-Za-z_]\w*)\s*\(", re.sub(r"__declspec\(\w+\)", "", nxt))
                if n:
                    funcs[addr] = n.group(1)
    return funcs, globals_, strings


def reccmp_symbols() -> dict[int, tuple[str, int, int]]:
    """Original address -> (name, entity type, size) from reccmp's database, cached until the PDB changes.
    This names everything the way reccmp does (CRT functions, floats, strings, imports)."""
    import json

    cache = ROOT / "permuter_out" / ".orig_symbols.json"
    pdb = ROOT / "build" / "legoland.pdb"
    if cache.exists() and pdb.exists() and cache.stat().st_mtime > pdb.stat().st_mtime:
        return {int(k): tuple(v) for k, v in json.loads(cache.read_text()).items()}
    print("building the original-symbol cache from reccmp (once per build)...", flush=True)
    import logging

    logging.disable(logging.CRITICAL)
    os.environ["PATH"] = str(TOOLS) + os.pathsep + os.environ.get("PATH", "")
    from reccmp.compare.core import Compare
    from reccmp.project.detect import RecCmpProject

    compare = Compare.from_target(RecCmpProject.from_directory(ROOT).get("LEGOLAND"))
    syms = {}
    for e in compare._db.get_all():
        if e.orig_addr is not None and e.name:
            syms[e.orig_addr] = (e.name, int(e.entity_type or 0), int(e.size(0) or e.any_size() or 0))
    cache.parent.mkdir(parents=True, exist_ok=True)
    cache.write_text(json.dumps({str(k): v for k, v in syms.items()}))
    return syms


FLOAT_TYPE, STRING_TYPE, IMPORT_TYPE = 6, 4, 7


class Original:
    def __init__(self):
        self.image = detect_image(str(ROOT / "external" / "legoland.exe"))
        self.syms = reccmp_symbols()
        funcs, globals_, strings = annotations()  # fallback for anything reccmp has no name for
        self.c_globals = set(globals_)  # addresses the source declares as globals
        for a, n in {**globals_, **funcs}.items():
            self.syms.setdefault(a, (n, 0, 0))
        for a in strings:
            self.syms.setdefault(a, ("", STRING_TYPE, 0))
        self.addrs = sorted(self.syms)
        self.imports = {imp.addr: imp.name for imp in getattr(self.image, "imports", []) if imp.name}
        _, self.starts = parse_annotations(SRC)
        self._cache = {}

    def read(self, addr, n):
        try:
            return bytes(self.image.read(addr, n))
        except Exception:
            return b""

    def string_at(self, addr):
        return '"' + self.read(addr, 256).split(b"\0")[0].decode("latin-1") + '"'

    def float_at(self, addr, size):
        raw = self.read(addr, size)
        return f"<f:{struct.unpack('<f' if size == 4 else '<d', raw)[0]!r}>" if len(raw) == size else "<addr>"

    def name(self, addr, fp):
        if addr in self.imports:
            return bare(self.imports[addr])
        e = self.syms.get(addr)
        if e is not None:
            name, typ, size = e
            if typ == FLOAT_TYPE and addr not in self.c_globals:
                return self.float_at(addr, fp or size or 4)
            if typ == STRING_TYPE:
                return self.string_at(addr)
            if typ == IMPORT_TYPE or "::" in name:
                return bare(name.split("::")[-1])
            return bare(name)
        if fp:
            return self.float_at(addr, fp)
        s = self.read(addr, 256).split(b"\0")[0]
        if len(s) >= 2 and all(32 <= c < 127 or c in (9, 10, 13) for c in s):
            return '"' + s.decode("latin-1") + '"'
        i = bisect.bisect_right(self.addrs, addr - 1) - 1  # addr - 1: an end pointer belongs to its array
        if i >= 0:
            base = self.addrs[i]
            name, typ, size = self.syms[base]
            if addr - base <= max(size, 1) and typ not in (FLOAT_TYPE, STRING_TYPE):
                return f"{bare(name)}+{addr - base:#x}"
        return "<addr>"

    def in_symbol(self, v):
        """A plain constant inside a known symbol (or exactly at its end: loop-end pointers) is an address,
        unless it is a round number like 0x800000 or 0x7fffff, which only lands inside a symbol by chance."""
        if (v & 0xFFFF) in (0, 0xFFFF):
            return False
        i = bisect.bisect_right(self.addrs, v - 1) - 1  # v - 1 so an end pointer finds its array
        if i < 0:
            return False
        base = self.addrs[i]
        name, typ, size = self.syms[base]
        return size > 0 and base <= v <= base + size

    def function_listing(self, addr, size):
        """Listing of `size` bytes at `addr` (reccmp also reads the original with our function's size),
        never past the next annotated function."""
        nxt = self.starts[self.starts.index(addr) + 1]
        size = min(size, nxt - addr)
        if size in self._cache:
            return self._cache[size]
        end = addr + size
        code = self.read(addr, size)
        names, tables = {}, {}
        for ins in MD.disasm(code, addr):
            off = ins.address - addr
            raw = code[off : off + ins.size]
            if ins.mnemonic == "call" or ins.mnemonic in JUMPS:
                op = ins.operands[0] if ins.operands else None
                if op is not None and op.type == x86.X86_OP_IMM:
                    if ins.mnemonic == "call" or not (addr <= op.imm < nxt):
                        names[off] = (None, self.name(op.imm, 0))  # calls are always named (even recursion)
                    continue  # jumps inside the function are printed as displacements
            for op in ins.operands:
                v = None
                if op.type == x86.X86_OP_MEM and 0x400000 <= op.mem.disp < 0x900000:
                    v = op.mem.disp
                elif op.type == x86.X86_OP_IMM and 0x400000 <= op.imm < 0x900000:
                    # a plain constant can fall inside a symbol's range by chance (the reccmp false positive
                    # tools/reccmp_fixes.py handles): only an exact symbol address counts as an address
                    if op.imm in self.syms or op.imm in self.imports or (addr <= op.imm < nxt) or self.in_symbol(op.imm):
                        v = op.imm
                if v is None:
                    continue
                field = raw.find(struct.pack("<I", v))
                if addr <= v < end:  # data inside the function: jump/index table
                    tables[v - addr] = "dword" if ins.mnemonic == "jmp" else "byte"
                    names[off] = (field, "<table>")
                elif field >= 0:
                    names[off] = (field, self.name(v, fp_size(ins)))
        self._cache[size] = listing(code, names, tables, entry_bias=addr)
        return self._cache[size]


# ---------------------------------------------------------------- candidate side (COFF .obj)


class Obj:
    def __init__(self, data: bytes):
        self.data = data
        nsec = struct.unpack_from("<H", data, 2)[0]
        symptr, nsym = struct.unpack_from("<II", data, 8)
        optsize = struct.unpack_from("<H", data, 16)[0]
        strtab = symptr + nsym * 18
        self.sections = []
        for k in range(nsec):
            o = 20 + optsize + 40 * k
            name = data[o : o + 8].rstrip(b"\0").decode("latin-1")
            size, rawptr, relptr = struct.unpack_from("<III", data, o + 16)
            nrel = struct.unpack_from("<H", data, o + 32)[0]
            relocs = [struct.unpack_from("<IIH", data, relptr + 10 * r) for r in range(nrel)]
            self.sections.append((name, size, rawptr, relocs))
        self.symbols = {}
        k = 0
        while k < nsym:
            o = symptr + 18 * k
            if data[o : o + 4] == b"\0\0\0\0":
                off = struct.unpack_from("<I", data, o + 4)[0]
                name = data[strtab + off : data.index(b"\0", strtab + off)].decode("latin-1")
            else:
                name = data[o : o + 8].rstrip(b"\0").decode("latin-1")
            value, secnum, typ, cls, naux = struct.unpack_from("<IhHBB", data, o + 8)
            self.symbols[k] = (name, value, secnum, typ, cls)
            k += 1 + naux

    def section_bytes(self, secnum):
        _, size, rawptr, _ = self.sections[secnum - 1]
        return self.data[rawptr : rawptr + size]

    def function(self, cname):
        want = bare(cname)
        for name, value, secnum, typ, cls in self.symbols.values():
            if secnum > 0 and typ & 0x20 and bare(name) == want:
                others = sorted(v for n, v, s, t, c in self.symbols.values() if s == secnum and v > value and t & 0x20)
                size = others[0] - value if others else self.sections[secnum - 1][1] - value
                return secnum, value, size
        raise KeyError(cname)

    def data_name(self, secnum, offset, fp):
        """Name for a reference into one of this object's own data sections."""
        named = [(v, n) for n, v, s, t, c in self.symbols.values() if s == secnum and not n.startswith(".") and v <= offset]
        v, n = max(named) if named else (None, None)
        raw = self.section_bytes(secnum)
        if n is not None and n.startswith("__real@"):
            size = 4 if n.startswith("__real@4") else 8
            return f"<f:{struct.unpack('<f' if size == 4 else '<d', raw[v : v + size])[0]!r}>"
        if n is None or n.startswith(("??_C@", "$SG")):
            return '"' + raw[offset:].split(b"\0")[0].decode("latin-1") + '"'
        return bare(n) + (f"+{offset - v:#x}" if offset != v else "")

    def symbol_name(self, idx, addend, fp):
        name, value, secnum, typ, cls = self.symbols[idx]
        if secnum > 0 and (name.startswith((".", "$SG", "??_C@", "__real@"))):
            return self.data_name(secnum, value + addend, fp)
        return bare(name) + (f"+{addend:#x}" if addend else "")


def candidate_listing(obj: Obj, cname: str):
    secnum, start, size = obj.function(cname)
    section = obj.sections[secnum - 1]
    raw = obj.section_bytes(secnum)
    code = raw[start : start + size]
    insns = list(MD.disasm(code, 0))
    starts = [i.address for i in insns]
    names, tables = {}, {}
    for va, symidx, rtype in section[3]:
        if not (start <= va < start + size):
            continue
        off = va - start
        addend = struct.unpack_from("<i", raw, va)[0]
        sname, svalue, ssec, _, _ = obj.symbols[symidx]
        k = bisect.bisect_right(starts, off) - 1
        ins = insns[k] if k >= 0 and off < starts[k] + insns[k].size else None
        if ssec == secnum and rtype == 6 and start <= svalue + addend < start + size:
            # a reference to data inside this function (a table), or a table entry
            if ins is not None and ins.mnemonic in ("jmp", "mov", "movzx", "movsx", "lea", "cmp", "add") and ins.address + ins.size <= min(tables, default=size):
                tables[svalue + addend - start] = "dword" if ins.mnemonic == "jmp" else "byte"
                names[ins.address] = (off - ins.address, "<table>")
            continue
        if ins is None:
            continue
        if rtype == 0x14:  # REL32 call/jmp to a symbol
            names[ins.address] = (None, obj.symbol_name(symidx, 0, 0))
        else:
            names[ins.address] = (off - ins.address, obj.symbol_name(symidx, addend, fp_size(ins)))
    # jump-table entries are section offsets: make them function-relative
    return listing(code, names, tables, entry_bias=start), size

# ---------------------------------------------------------------- compiling


def function_region(lines, addr):
    marker = f"// FUNCTION: LEGOLAND 0x{addr:08x}"
    i = lines.index(marker)
    end = i + 1
    if not lines[end].rstrip().endswith("}"):
        while lines[end] != "}":
            end += 1
    return i + 1, end + 1  # signature line .. closing brace (inclusive), excluding the marker


def compile_tu(text: str, workdir: Path) -> bytes | None:
    src = workdir / "perm.c"
    obj = workdir / "perm.obj"
    src.write_text(text, encoding="latin-1")
    if obj.exists():
        obj.unlink()
    r = subprocess.run(
        [str(CL), f"/I{SRC}", "/nologo", "/W3", "/O2", "/Z7", "/c", f"/Fo{obj}", str(src)],
        capture_output=True,
        text=True,
        cwd=ROOT,
    )
    # lossy conversions (C4244, e.g. a float put in an int temporary) change behaviour: count them
    compile_tu.lossy = r.stdout.count("C4244")
    if r.returncode != 0 or not obj.exists():
        return None
    return obj.read_bytes()


def score(orig: list[str], cand: list[str]) -> float:
    if orig == cand:
        return 1.0
    return difflib.SequenceMatcher(None, orig, cand, autojunk=False).ratio()


_REG = re.compile(r"\b(?:e?[abcd]x|[abcd][lh]|e?[sd]i|e?bp)\b")
_STACK = re.compile(r"\[esp(?: [+-] 0x[0-9a-f]+)?\]")


def guide(orig: list[str], cand: list[str]) -> float:
    """What the search climbs: exact similarity plus partial credit for lines that differ only in register
    choice (and, less, in stack offsets). One register swap changes dozens of lines, so the exact score
    alone has no slope; with this a variant that gets the structure right first still ranks higher."""
    exact = score(orig, cand)
    if exact == 1.0:
        return 1.0
    o2, c2 = [_REG.sub("R", l) for l in orig], [_REG.sub("R", l) for l in cand]
    o3, c3 = [_STACK.sub("[S]", l) for l in o2], [_STACK.sub("[S]", l) for l in c2]
    return 0.5 * exact + 0.3 * score(o2, c2) + 0.2 * score(o3, c3)


# ---------------------------------------------------------------- mutations

OPERAND = r"(?:\(\w+\s*\*?\)\s*)?[A-Za-z_][\w]*(?:(?:->|\.)\w+|\[[^\[\]]+\])*|\d+[uUlL]*|0x[0-9a-fA-F]+[uUlL]*"
COMMUTATIVE = r"\+|\*|&(?!&)|\|(?!\|)|\^|==|!="
FLIP = {"<": ">", ">": "<", "<=": ">=", ">=": "<="}


_LOOSE_BEFORE = ("(", ",", "[", "?", ":", "{", ";", "&&", "||", "return", "=")
_LOOSE_AFTER = (")", ",", "]", ";", "?", ":", "&&", "||")


def _isolated(text, m, op=""):
    """True when the matched binary expression isn't an operand of a tighter (or equal) operator next to it,
    so rewriting it can't change how the surrounding expression parses (a * b + c must not become a * c + b)."""
    before, after = text[: m.start()].rstrip(), text[m.end() :].lstrip()
    if before.endswith(("==", "!=", "<=", ">=")) and op in ("==", "!="):
        return False
    if not before.endswith(_LOOSE_BEFORE) or before.endswith(("<<=", ">>=")):
        return False
    if after.startswith(_LOOSE_AFTER):
        return True
    # + and * bind tighter than comparisons, so a following comparison is fine for them
    return op in ("+", "*") and re.match(r"(==|!=|<=|>=|<(?!<)|>(?!>))", after) is not None


def _sub_random(text, pattern, repl, rng, flags=0, op_group=None):
    matches = list(re.finditer(pattern, text, flags))
    if op_group is not None:
        matches = [m for m in matches if _isolated(text, m, m.group(op_group))]
    if not matches:
        return None
    m = rng.choice(matches)
    new = repl(m)
    if new is None or new == m.group(0):
        return None
    return text[: m.start()] + new + text[m.end() :]


def m_swap_commutative(body, rng):
    pat = rf"(?<![\w\]\)])({OPERAND})\s*({COMMUTATIVE})\s*({OPERAND})(?![\w\[(])"
    return _sub_random(body, pat, lambda m: f"{m.group(3)} {m.group(2)} {m.group(1)}", rng, op_group=2)


def m_flip_compare(body, rng):
    pat = rf"(?<![\w\]\)])({OPERAND})\s*(<=|>=|<(?!<)|>(?!>))\s*({OPERAND})(?![\w\[(])"
    return _sub_random(body, pat, lambda m: f"{m.group(3)} {FLIP[m.group(2)]} {m.group(1)}", rng, op_group=2)


def m_incdec(body, rng):
    def repl(m):
        ind, a, b, op, var = m.group(1), m.group(2), m.group(3), m.group(4), m.group(5)
        v = a or var
        o = (op or b)[0]
        forms = [f"{v}{o}{o};", f"{o}{o}{v};", f"{v} {o}= 1;", f"{v} = {v} {o} 1;"]
        return ind + rng.choice(forms)

    pat = r"(?m)^(\s*)(?:(\w+)(\+\+|--)|(\+\+|--)(\w+));"
    return _sub_random(body, pat, repl, rng)


def m_compound(body, rng):
    def repl(m):
        ind, v, rhs_v, op, rest = m.groups()
        # v = v - a / 2 - b is not v -= a / 2 - b: only fold a single operand (or a parenthesised one)
        single = re.fullmatch(rf"\s*(?:{OPERAND}|\([^()]*\))\s*", rest)
        return f"{ind}{v} {op}= {rest};" if v == rhs_v and single else None

    pat = r"(?m)^(\s*)(\w+(?:->\w+|\.\w+)?) = (\w+(?:->\w+|\.\w+)?) ([-+*&|^]) ([^;]+);"
    out = _sub_random(body, pat, repl, rng)
    if out is not None:
        return out
    pat2 = r"(?m)^(\s*)(\w+(?:->\w+|\.\w+)?) ([-+*&|^])= ([^;]+);"
    # v -= a + b is v = v - (a + b): keep the parentheses unless the right side is a single operand
    def unfold(m):
        rest = m.group(4) if re.fullmatch(rf"\s*(?:{OPERAND}|\([^()]*\))\s*", m.group(4)) else f"({m.group(4)})"
        return f"{m.group(1)}{m.group(2)} = {m.group(2)} {m.group(3)} {rest};"
    return _sub_random(body, pat2, unfold, rng)


def _in_condition(text, m):
    """True when the match is a whole operand of an if/while condition or of && / || (its value is only tested)."""
    before, after = text[: m.start()].rstrip(), text[m.end() :].lstrip()
    if not (after.startswith((")", "&&", "||"))):
        return False
    return before.endswith(("&&", "||")) or re.search(r"\b(if|while)\s*\($", before) is not None


def m_zero_test(body, rng):
    choices = [
        (r"\b(\w+(?:->\w+|\.\w+)?) != 0\b", lambda m: m.group(1)),
        (r"\b(\w+(?:->\w+|\.\w+)?) == 0\b", lambda m: f"!{m.group(1)}"),
        (r"\b(\w+(?:->\w+|\.\w+)?) != NULL\b", lambda m: m.group(1)),
        (r"\b(\w+(?:->\w+|\.\w+)?) == NULL\b", lambda m: f"!{m.group(1)}"),
        (r"\b0 != (\w+)", lambda m: f"{m.group(1)} != 0"),
    ]
    pat, repl = rng.choice(choices)
    matches = [m for m in re.finditer(pat, body) if _in_condition(body, m)]
    if not matches:
        return None
    m = rng.choice(matches)
    return body[: m.start()] + repl(m) + body[m.end() :]


def _statements(lines):
    """Indices of simple one-line statements (not declarations, control flow or braces)."""
    out = []
    for k, l in enumerate(lines):
        s = l.strip()
        if not s.endswith(";") or s.startswith(("return", "break", "continue", "goto", "case", "default", "//")):
            continue
        # must start a statement: the previous code line ends one (not "y1 =" continued, not a braceless if/else/for/while)
        prev = next((lines[j].strip() for j in range(k - 1, -1, -1) if lines[j].strip() and not lines[j].strip().startswith("//")), "{")
        if not prev.endswith((";", "{", "}", ":")) or re.match(r"(\}\s*)?(else|do)\b", prev) and not prev.endswith("{"):
            continue
        if re.match(r"(?:\}\s*else\s+)?(?:if|for|while)\b", prev) and not prev.endswith("{"):
            continue
        if "{" in s or "}" in s or s.startswith(("if", "for", "while", "do", "switch", "else")):
            continue
        out.append(k)
    return out


_CALL = re.compile(r"\b(?!sizeof\b|if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*\s*\(")


def _idents(s):
    return set(re.findall(r"[A-Za-z_]\w*", s))


def _written(stmt):
    """Identifiers in the target of an assignment / ++ / -- statement (conservative: every name in it)."""
    s = stmt.strip()
    m = re.match(r"(.*?)\s*(?:<<|>>|[-+*/%&|^])?=(?!=)", s)
    if m:
        return _idents(m.group(1))
    m = re.match(r"(?:\+\+|--)?\s*(.*?)\s*(?:\+\+|--)?;$", s)
    return _idents(m.group(1)) if m and ("++" in s or "--" in s) else set()


def m_swap_statements(body, rng):
    lines = body.split("\n")
    st = set(_statements(lines))
    pairs = [k for k in st if k + 1 in st and (len(lines[k]) - len(lines[k].lstrip())) == (len(lines[k + 1]) - len(lines[k + 1].lstrip()))]
    pairs = [k for k in pairs if not re.match(r"\s*(?:const\s+|unsigned\s+|signed\s+|struct\s+\w+\s*\*?|int|char|short|long|float|double|void)\b[^=]*;", lines[k])]
    if not pairs:
        return None
    k = rng.choice(pairs)
    a, b = lines[k], lines[k + 1]
    # a call may read or write anything: never move a statement across one
    if _CALL.search(a) or _CALL.search(b):
        return None
    # no dependency either way: neither statement may mention anything the other writes
    wa, wb = _written(a), _written(b)
    if wa & _idents(b) or wb & _idents(a):
        return None
    lines[k], lines[k + 1] = b, a
    return "\n".join(lines)


DECL = re.compile(r"^\s+(?:const\s+|volatile\s+|register\s+)?(?:unsigned\s+|signed\s+)?(?:struct\s+\w+|\w+)\s*\**\s*\w+(\[[^\]]*\])*\s*;\s*$")


def m_swap_decls(body, rng):
    lines = body.split("\n")
    ds = [k for k, l in enumerate(lines) if DECL.match(l)]
    pairs = [k for k in ds if k + 1 in ds]
    if not pairs:
        return None
    k = rng.choice(pairs)
    lines[k], lines[k + 1] = lines[k + 1], lines[k]
    return "\n".join(lines)


def m_register(body, rng):
    lines = body.split("\n")
    ds = [k for k, l in enumerate(lines) if DECL.match(l) and "[" not in l]
    if not ds:
        return None
    k = rng.choice(ds)
    l = lines[k]
    lines[k] = l.replace("register ", "") if "register " in l else re.sub(r"^(\s+)", r"\1register ", l, count=1)
    return "\n".join(lines)


def _block_end(lines, k):
    """Index of the line that closes the brace opened at the end of lines[k]."""
    depth = 0
    for j in range(k, len(lines)):
        # on the opening line only the last "{" counts ("} else {" opens a block, its "}" closes another)
        for ch in (lines[j][lines[j].rfind("{") :] if j == k else lines[j]):
            if ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
                if depth == 0 and j > k:
                    return j  # first brace closing the block, even on a "} else {" line
    return None


def m_swap_if_else(body, rng):
    lines = body.split("\n")
    cands = []
    for k, l in enumerate(lines):
        m = re.match(r"^(\s*)if \((.*)\) \{$", l)
        if not m:
            continue
        e = _block_end(lines, k)
        if e is None or not re.match(r"^\s*\} else \{$", lines[e]):
            continue
        e2 = _block_end(lines, e)
        if e2 is None or lines[e2].strip() != "}":
            continue
        cands.append((k, e, e2, m.group(1), m.group(2)))
    if not cands:
        return None
    k, e, e2, ind, cond = rng.choice(cands)
    then_body, else_body = lines[k + 1 : e], lines[e + 1 : e2]
    new = [f"{ind}if (!({cond})) {{"] + else_body + [f"{ind}}} else {{"] + then_body + [f"{ind}}}"]
    return "\n".join(lines[:k] + new + lines[e2 + 1 :])


def m_split_and(body, rng):
    lines = body.split("\n")
    cands = []
    for k, l in enumerate(lines):
        m = re.match(r"^(\s*)if \(([^()]*(?:\([^()]*\)[^()]*)*) && (.*)\) \{$", l)
        if not m:
            continue
        e = _block_end(lines, k)
        if e is None or lines[e].strip() != "}":
            continue
        cands.append((k, e, m.group(1), m.group(2), m.group(3)))
    if not cands:
        return None
    k, e, ind, a, b = rng.choice(cands)
    inner = ["    " + x if x else x for x in lines[k + 1 : e]]
    new = [f"{ind}if ({a}) {{", f"{ind}    if ({b}) {{"] + inner + [f"{ind}    }}", f"{ind}}}"]
    return "\n".join(lines[:k] + new + lines[e + 1 :])


def m_brace_statement(body, rng):
    lines = body.split("\n")
    st = _statements(lines)
    if not st:
        return None
    k = rng.choice(st)
    ind = lines[k][: len(lines[k]) - len(lines[k].lstrip())]
    lines[k] = f"{ind}{{ {lines[k].strip()} }}"
    return "\n".join(lines)


def _decl_insert_at(lines):
    """Index after the function's local declarations (first line inside the body that isn't one)."""
    k = 1  # lines[0] is the signature with the opening brace
    while k < len(lines) and (DECL.match(lines[k]) or not lines[k].strip()):
        k += 1
    return k


_temp_counter = [0]


def m_temp(body, rng):
    """Move a member/array access into a new int temporary on the line before its statement."""
    lines = body.split("\n")
    st = _statements(lines)
    cands = []
    for k in st:
        rhs = lines[k].split("=", 1)[1] if "=" in lines[k] else lines[k]
        for m in re.finditer(r"[A-Za-z_]\w*(?:->\w+|\.\w+|\[[^\[\]]+\])+", rhs):
            # never the target of ++/-- or an address taken with &: the temporary would be modified instead
            if re.match(r"\s*(\+\+|--)", rhs[m.end() :]) or re.search(r"(\+\+|--|&)\s*(\(\s*[\w\s*]+\)\s*)?$", rhs[: m.start()]):
                continue
            cands.append((k, m.group(0)))
    if not cands:
        return None
    k, expr = rng.choice(cands)
    _temp_counter[0] += 1
    t = f"ptmp{_temp_counter[0]}"
    ind = lines[k][: len(lines[k]) - len(lines[k].lstrip())]
    lines[k] = lines[k].replace(expr, t, 1) if "=" not in lines[k] else lines[k].split("=", 1)[0] + "=" + lines[k].split("=", 1)[1].replace(expr, t, 1)
    lines.insert(k, f"{ind}{t} = {expr};")
    lines.insert(_decl_insert_at(lines), f"    int {t};")
    return "\n".join(lines)


def m_unsigned(body, rng):
    lines = body.split("\n")
    ds = [k for k, l in enumerate(lines) if re.match(r"^\s+(?:register\s+)?(?:unsigned\s+)?(?:int|char|short|long)\s+\w+\s*;", l)]
    if not ds:
        return None
    k = rng.choice(ds)
    l = lines[k]
    # making a variable unsigned changes division, modulo and right shifts of negative values: leave those alone
    name = re.search(r"(\w+)\s*;", l).group(1)
    if "unsigned " not in l and re.search(rf"\b{name}\b\s*(?:[/%]|>>)|(?:[/%]|>>)\s*\(?\s*\b{name}\b|\b{name}\b\s*[-+]\s*\w+\s*\)\s*[/%]", body):
        return None
    lines[k] = l.replace("unsigned ", "", 1) if "unsigned " in l else re.sub(r"\b(int|char|short|long)\b", r"unsigned \1", l, count=1)
    return "\n".join(lines)


def m_for_to_while(body, rng):
    lines = body.split("\n")
    cands = []
    for k, l in enumerate(lines):
        m = re.match(r"^(\s*)for \(([^;]*); ([^;]*); ([^)]*)\) \{$", l)
        if not m:
            continue
        e = _block_end(lines, k)
        if e is None or lines[e].strip() != "}" or any("continue" in x for x in lines[k:e]):
            continue
        cands.append((k, e, m.groups()))
    if not cands:
        return None
    k, e, (ind, init, cond, inc) = rng.choice(cands)
    new = ([f"{ind}{init};"] if init.strip() else []) + [f"{ind}while ({cond or '1'}) {{"] + lines[k + 1 : e]
    new += ([f"{ind}    {inc};"] if inc.strip() else []) + [f"{ind}}}"]
    return "\n".join(lines[:k] + new + lines[e + 1 :])


def m_cast_compare(body, rng):
    pat = rf"(?<![\w\]\)])({OPERAND})(\s*(?:<=|>=|<(?!<)|>(?!>)|==|!=)\s*)({OPERAND})(?![\w\[(])"
    cast = rng.choice(["(int)", "(unsigned int)", "(unsigned)"])
    side = rng.randint(1, 2)

    def repl(m):
        # an unsigned compare against 0 or a negative literal changes the result (x < (unsigned)0 is never true)
        if cast != "(int)" and any(re.fullmatch(r"-?0[uUlL]*|-\s*\w+", g.strip()) for g in (m.group(1), m.group(3))):
            return None
        return f"{cast}{m.group(1)}{m.group(2)}{m.group(3)}" if side == 1 else f"{m.group(1)}{m.group(2)}{cast}{m.group(3)}"

    return _sub_random(body, pat, repl, rng, op_group=2)


def _decl_block(lines):
    """Indices of the function's top-level declaration lines (right after the signature)."""
    out = []
    k = 1
    while k < len(lines):
        s = lines[k].strip()
        if not s or s.startswith("//"):
            k += 1
            continue
        if DECL.match(lines[k]) or INIT_DECL.match(lines[k]):
            out.append(k)
            k += 1
            continue
        break
    return out


INIT_DECL = re.compile(r"^\s+(?:const\s+|volatile\s+|register\s+)?(?:unsigned\s+|signed\s+)?(?:struct\s+\w+|\w+)\s*\**\s*(\w+)\s*=\s*[^;]+;\s*$")
_DECL_NAME = re.compile(r"(\w+)\s*(?:\[[^\]]*\])*\s*(?:=[^;]*)?;\s*$")


def _decl_name(line):
    m = _DECL_NAME.search(line)
    return m.group(1) if m else None


def m_swap_decls_init(body, rng):
    """Swap two adjacent declarations, initialised ones included, when neither initialiser reads the other."""
    lines = body.split("\n")
    ds = _decl_block(lines)
    pairs = []
    for k in ds:
        if k + 1 not in ds:
            continue
        a, b = _decl_name(lines[k]), _decl_name(lines[k + 1])
        if not a or not b:
            continue
        rhs_a = lines[k].split("=", 1)[1] if "=" in lines[k] else ""
        rhs_b = lines[k + 1].split("=", 1)[1] if "=" in lines[k + 1] else ""
        # an initialiser that calls something may have side effects: keep their order
        if re.search(rf"\b{a}\b", rhs_b) or re.search(rf"\b{b}\b", rhs_a) or (_CALL.search(rhs_a) and _CALL.search(rhs_b)):
            continue
        pairs.append(k)
    if not pairs:
        return None
    k = rng.choice(pairs)
    lines[k], lines[k + 1] = lines[k + 1], lines[k]
    return "\n".join(lines)


def m_volatile(body, rng):
    """Toggle volatile on a local scalar: forces it into a stack slot (or lets it back into a register)."""
    lines = body.split("\n")
    ds = [k for k in _decl_block(lines) if "[" not in lines[k] and "struct" not in lines[k] and "*" not in lines[k] and "register " not in lines[k]]
    if not ds:
        return None
    k = rng.choice(ds)
    l = lines[k]
    lines[k] = l.replace("volatile ", "", 1) if "volatile " in l else re.sub(r"^(\s+)", r"\1volatile ", l, count=1)
    return "\n".join(lines)


def m_split_init(body, rng):
    """int x = e;  ->  int x;  ... x = e; as the first statement (moves where the value is first computed)."""
    lines = body.split("\n")
    ds = _decl_block(lines)
    cands = [k for k in ds if INIT_DECL.match(lines[k]) and "const " not in lines[k] and "[" not in lines[k]]
    if not cands or not ds:
        return None
    k = rng.choice(cands)
    name = _decl_name(lines[k])
    decl, rhs = lines[k].split("=", 1)
    ind = lines[k][: len(lines[k]) - len(lines[k].lstrip())]
    # later initialisers that read it must keep seeing the value: only split when none do
    if any(re.search(rf"\b{name}\b", lines[j].split("=", 1)[1]) for j in ds if j > k and "=" in lines[j]):
        return None
    lines[k] = decl.rstrip() + ";"
    last = max(ds)
    lines.insert(last + 1, f"{ind}{name} ={rhs}")
    return "\n".join(lines)


MUTATIONS = [
    (m_swap_decls_init, 2),
    (m_volatile, 1),
    (m_split_init, 2),
    (m_swap_commutative, 5),
    (m_flip_compare, 3),
    (m_incdec, 2),
    (m_compound, 2),
    (m_zero_test, 2),
    (m_swap_statements, 4),
    (m_swap_decls, 1),
    (m_register, 2),
    (m_swap_if_else, 2),
    (m_split_and, 1),
    (m_brace_statement, 1),
    (m_temp, 3),
    (m_unsigned, 2),
    (m_for_to_while, 1),
    (m_cast_compare, 2),
]


def mutate(body, rng):
    funcs = [f for f, w in MUTATIONS for _ in range(w)]
    for _ in range(rng.randint(1, 3)):
        for _attempt in range(10):
            new = rng.choice(funcs)(body, rng)
            if new is not None:
                body = new
                break
    return body


def decl_orders(body, rng, limit):
    """Bodies with the top-level declarations in every valid order (a random sample when there are more than
    limit). An order is valid when no initialiser reads a variable declared after it and initialisers that
    call something keep their relative order."""
    import itertools
    import math

    lines = body.split("\n")
    ds = _decl_block(lines)
    if len(ds) < 2 or ds != list(range(ds[0], ds[0] + len(ds))):
        return []
    decls = [lines[k] for k in ds]
    names = [_decl_name(l) for l in decls]
    rhs = [l.split("=", 1)[1] if "=" in l else "" for l in decls]
    calls = [i for i, r in enumerate(rhs) if _CALL.search(r)]

    def valid(perm):
        pos = {i: n for n, i in enumerate(perm)}
        for i in range(len(decls)):
            for j in range(len(decls)):
                if i != j and names[j] and re.search(rf"\b{names[j]}\b", rhs[i]) and pos[j] > pos[i]:
                    return False
        return all(pos[a] < pos[b] for a, b in zip(calls, calls[1:]))

    idx = list(range(len(decls)))
    if math.factorial(len(decls)) <= limit:
        perms = [p for p in itertools.permutations(idx) if valid(p)]
    else:
        perms, tries = set(), 0
        while len(perms) < limit and tries < limit * 20:
            tries += 1
            p = idx[:]
            rng.shuffle(p)
            if valid(p):
                perms.add(tuple(p))
        perms = list(perms)
    out = []
    for p in perms:
        new = lines[:]
        new[ds[0] : ds[0] + len(ds)] = [decls[i] for i in p]
        out.append("\n".join(new))
    rng.shuffle(out)
    return out


def register_combos(body):
    """body with register added to every subset of its plain local declarations (scalars, pointers, small
    structs; no arrays, no volatile)."""
    import itertools

    lines = body.split("\n")
    ks = [k for k in _decl_block(lines) if "[" not in lines[k] and "volatile " not in lines[k] and "static " not in lines[k]]
    ks = ks[:8]  # 256 combinations at most
    plain = [re.sub(r"^(\s+)register ", r"\1", lines[k]) for k in ks]
    out = []
    for mask in itertools.product((0, 1), repeat=len(ks)):
        new = lines[:]
        for bit, k, l in zip(mask, ks, plain):
            new[k] = re.sub(r"^(\s+)", r"\1register ", l, count=1) if bit else l
        out.append("\n".join(new))
    return out


# ---------------------------------------------------------------- main


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("addr", type=lambda s: int(s, 16))
    ap.add_argument("--minutes", type=float, default=10)
    ap.add_argument("--seed", type=int, default=None)
    ap.add_argument("--score-only", action="store_true", help="score the current source and show the diff")
    ap.add_argument("--decl-orders", action="store_true", help="try every order of the top-level declarations (sampled when there are too many), then every register combination on the 5 best orders")
    args = ap.parse_args()

    orig = Original()
    marker = f"// FUNCTION: LEGOLAND 0x{args.addr:08x}"
    src = next(c for c in SRC.glob("*.c") if marker in c.read_text(encoding="latin-1"))
    lines = src.read_text(encoding="latin-1").split("\n")
    a, b = function_region(lines, args.addr)
    cname = re.search(r"([A-Za-z_]\w*)\s*\(", re.sub(r"__declspec\(\w+\)", "", lines[a])).group(1)
    head, tail = "\n".join(lines[:a]) + "\n", "\n" + "\n".join(lines[b:])
    base_body = "\n".join(lines[a:b])

    work = Path(tempfile.mkdtemp(prefix="permuter-"))
    outdir = ROOT / "permuter_out" / f"0x{args.addr:08x}"

    def evaluate(body):
        data = compile_tu(head + body + tail, work)
        if data is None or compile_tu.lossy > base_lossy[0]:
            return None, None, None, None
        try:
            cand, size = candidate_listing(Obj(data), cname)
        except KeyError:
            return None, None, None, None
        target = orig.function_listing(args.addr, size)
        return score(target, cand), cand, target, guide(target, cand)

    base_lossy = [99999]
    best, cand, target, best_guide = evaluate(base_body)
    base_lossy[0] = compile_tu.lossy
    if best is None:
        sys.exit(f"{cname}: the current source does not compile on its own")
    print(f"{cname} ({src.name}) base score {best * 100:.2f}% (guide {best_guide * 100:.2f}%, {len(target)} instructions)", flush=True)
    if args.score_only:
        for line in difflib.unified_diff(target, cand, "original", "ours", lineterm="", n=1):
            print(line)
        return

    rng = random.Random(args.seed)
    stage, ranked = 1, []
    if args.decl_orders:
        orders = decl_orders(base_body, rng, limit=int(args.minutes * 60 / 0.35 * 0.85))
        print(f"{len(orders)} declaration orders to try", flush=True)
        ranked.append((best_guide, base_body))
    best_body, current, guide_body = base_body, base_body, base_body
    # every variant ever compiled for this function (any run): never compile one twice
    outdir.mkdir(parents=True, exist_ok=True)
    seen_file = outdir / "seen.txt"
    seen = set(seen_file.read_text().split()) if seen_file.exists() else set()
    known_before = len(seen)
    seen.add(hashlib.sha1(base_body.encode()).hexdigest())
    seen_log = seen_file.open("a")
    deadline = time.time() + args.minutes * 60
    tries = compiled = failed = skipped = 0
    base_score = best
    while time.time() < deadline and best < 1.0:
        tries += 1
        if args.decl_orders:
            if not orders and stage == 1:
                # stage 2: every register combination on the best few orders
                stage = 2
                for _, b in sorted(ranked, reverse=True)[:5]:
                    orders.extend(register_combos(b))
                rng.shuffle(orders)
                print(f"[{compiled}] orders done; {len(orders)} register combinations on the 5 best orders", flush=True)
            if not orders:
                break
            body = orders.pop()
        else:
            r = rng.random()
            start = current if r < 0.4 else guide_body if r < 0.8 else best_body
            body = mutate(start, rng)
        h = hashlib.sha1(body.encode()).hexdigest()
        if h in seen:
            skipped += 1
            continue
        seen.add(h)
        seen_log.write(h + "\n")
        s, _, _, g = evaluate(body)
        compiled += 1
        if s is None:
            failed += 1
            continue
        if args.decl_orders and stage == 1:
            ranked.append((g, body))
        if s > best:
            best, best_body = s, body
            path = outdir / f"{s * 100:07.3f}.c"
            path.write_text(marker + "\n" + body + "\n", encoding="latin-1")
            print(f"[{compiled}] {s * 100:.2f}%  -> {path.relative_to(ROOT)}", flush=True)
        # the search follows the graded score: partial register/stack progress counts
        if g > best_guide:
            best_guide, guide_body, current = g, body, body
        elif g == best_guide:
            current = body  # walk along the plateau
        if compiled % 50 == 0:
            seen_log.flush()
            print(f"[{compiled}] still {best * 100:.2f}% ({failed} did not compile, {skipped} already tried)", flush=True)
    seen_log.close()
    summary = (
        f"{time.strftime('%Y-%m-%d %H:%M')} seed={args.seed} minutes={args.minutes:g} "
        f"base={base_score * 100:.2f}% best={best * 100:.2f}% guide={best_guide * 100:.2f}% compiled={compiled} failed={failed} "
        f"skipped_already_tried={skipped} variants_known={len(seen)} (was {known_before})"
    )
    with (outdir / "runs.log").open("a") as f:
        f.write(summary + "\n")
    print("done: " + summary)


if __name__ == "__main__":
    main()
