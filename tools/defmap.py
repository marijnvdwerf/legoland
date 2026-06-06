#!/usr/bin/env python3
"""Map every defined function (and the TU/header that owns it) for the header refactor.

A function is "defined" in the .c file where its `// FUNCTION: LEGOLAND 0x<addr>`
annotation sits (the next line is the signature). The owning header for symbol S is
`<that-tu>.h`.

Usage:
  uv run --no-project tools/defmap.py                 # TUs by size (ascending) + totals
  uv run --no-project tools/defmap.py <symbol>        # where is <symbol> defined -> header
  uv run --no-project tools/defmap.py --externs <tu>  # cross-TU externs declared in <tu>.c and where each resolves
"""
import re, sys, glob, pathlib

SRC = "src/legoland"

def build():
    name2file = {}
    file_funcs = {}
    for path in sorted(glob.glob(f"{SRC}/*.c")):
        fn = pathlib.Path(path).name
        lines = pathlib.Path(path).read_text().splitlines()
        defs = []
        for i, l in enumerate(lines):
            if not re.match(r'//\s*FUNCTION:\s*LEGOLAND', l):
                continue
            sig = lines[i+1] if i+1 < len(lines) else ""
            m = re.search(r'([A-Za-z_]\w*)\s*\(', sig)
            if m:
                nm = m.group(1)
                name2file[nm] = fn
                defs.append(nm)
        file_funcs[fn] = defs
    return name2file, file_funcs

def header_for(fn):  # castle.c -> castle.h
    return fn[:-2] + ".h"

def main():
    name2file, file_funcs = build()
    if len(sys.argv) == 1:
        rows = sorted(file_funcs.items(), key=lambda kv: len(kv[1]))
        print(f"{'TU':26}{'defined fns':>11}")
        for fn, defs in rows:
            print(f"{fn:26}{len(defs):>11}")
        print(f"\n{len(file_funcs)} TUs, {sum(len(d) for d in file_funcs.values())} defined functions")
        return
    if sys.argv[1] == "--externs":
        tu = sys.argv[2] if not sys.argv[2].endswith(".c") else sys.argv[2]
        tu = tu if tu.endswith(".c") else tu + ".c"
        text = pathlib.Path(f"{SRC}/{tu}").read_text()
        # crude: extern function decls + same-file forward decls
        for m in re.finditer(r'extern\s+[^;{]*?([A-Za-z_]\w*)\s*\([^;{]*\)\s*;', text):
            nm = m.group(1)
            home = name2file.get(nm, "??unknown??")
            print(f"  {nm:28} -> {home}  (include \"{header_for(home)}\")" if home != tu else f"  {nm:28} -> SAME TU")
        return
    sym = sys.argv[1]
    home = name2file.get(sym)
    if home:
        print(f"{sym} defined in {home}  ->  include \"{header_for(home)}\"")
    else:
        print(f"{sym}: NOT FOUND as a defined function (may be a CRT/import stub or undeclared)")

if __name__ == "__main__":
    main()
