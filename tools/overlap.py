#!/usr/bin/env python3
"""Find globals in globals.h whose [addr, addr+size) ranges overlap their neighbour.

Each entry in globals.h is a pair:
    // 0x<addr>
    extern <type> <name>;
We parse addr + a size estimate from the type, sort by addr, and report any
entry whose end (addr+size) runs past the NEXT entry's addr. An overlap means
either (a) we under-sized a global (array/struct too small), or (b) the two
"globals" are really fields of one struct that should be folded together.

Usage:
  uv run --no-project tools/overlap.py            # list overlapping clusters
  uv run --no-project tools/overlap.py --all      # dump every global with size
"""
import re, sys, pathlib

HDR = "src/legoland/globals.h"

# size of a single (non-array, non-pointer) base type
BASE = {
    "char": 1, "unsigned char": 1, "signed char": 1, "BYTE": 1, "bool": 1,
    "short": 2, "unsigned short": 2, "WORD": 2,
    "int": 4, "unsigned int": 4, "signed int": 4, "long": 4, "unsigned long": 4,
    "unsigned": 4, "float": 4, "DWORD": 4, "UINT": 4, "BOOL": 4, "LONG": 4,
    "HWND": 4, "HINSTANCE": 4, "HANDLE": 4, "HDC": 4, "HMODULE": 4, "FARPROC": 4,
    "double": 8, "__int64": 8, "LARGE_INTEGER": 8,
    "GUID": 16, "IID": 16, "CLSID": 16,
    "POINT": 8, "RECT": 16, "SIZE": 8,
}

def num(tok):
    return int(tok, 16) if tok.lower().startswith("0x") else int(tok)

def sizeof(decl):
    """size in bytes of `extern <decl>;` body (decl = '<type> <name>[...]')."""
    decl = decl.strip().rstrip(";").strip()
    # array dims
    dims = re.findall(r'\[([0-9xXa-fA-F]+)\]', decl)
    mult = 1
    for d in dims:
        mult *= num(d)
    head = re.sub(r'\[[0-9xXa-fA-F]+\]', '', decl).strip()
    # pointer? element size 4
    if '*' in head:
        return 4 * mult
    # drop the variable name (last identifier)
    m = re.match(r'(.*?)([A-Za-z_]\w*)\s*$', head)
    typ = (m.group(1).strip() if m else head).strip()
    typ = re.sub(r'\bextern\b', '', typ).strip()
    if typ.startswith("struct "):
        return None if mult == 1 else ("STRUCT", mult)  # unknown struct size
    base = BASE.get(typ)
    if base is None:
        return None
    return base * mult

def parse():
    out = []
    lines = pathlib.Path(HDR).read_text().splitlines()
    cur = None
    for l in lines:
        m = re.match(r'\s*//\s*0x([0-9a-fA-F]+)\s*$', l)
        if m:
            cur = int(m.group(1), 16)
            continue
        m = re.match(r'\s*extern\s+(.*;)\s*$', l)
        if m and cur is not None:
            out.append((cur, m.group(1), sizeof(m.group(1))))
            cur = None
    return out

def main():
    g = parse()
    g.sort(key=lambda e: e[0])
    if "--all" in sys.argv:
        for addr, decl, sz in g:
            print(f"0x{addr:08x}  {str(sz):>8}  {decl}")
        return
    unknown = 0
    clusters = []
    for i in range(len(g) - 1):
        addr, decl, sz = g[i]
        naddr = g[i + 1][0]
        if sz is None or isinstance(sz, tuple):
            unknown += 1
            continue
        end = addr + sz
        if end > naddr:
            clusters.append((addr, sz, end, naddr, decl, g[i + 1][1]))
    print(f"{len(g)} globals, {unknown} with unknown size (struct instances)\n")
    print(f"{len(clusters)} overlapping pairs:\n")
    for addr, sz, end, naddr, decl, ndecl in clusters:
        over = end - naddr
        print(f"0x{addr:08x} +{sz:#x} -> 0x{end:08x}  overruns next 0x{naddr:08x} by {over} byte(s)")
        print(f"    this: {decl}")
        print(f"    next: {ndecl}")
    return

if __name__ == "__main__":
    main()
