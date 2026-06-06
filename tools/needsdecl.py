#!/usr/bin/env python3
"""Which functions of a TU actually NEED a declaration (i.e. were pre-declared).

A function needs a header decl ONLY if it was pre-declared somewhere:
  - cross-TU: referenced (called / address-taken) from a .c other than its own, or
  - in-file forward: referenced in its own .c at a line BEFORE its definition.
Everything else is defined-and-used-in-order and needs no declaration at all.

Usage:
  uv run --no-project tools/needsdecl.py <tu>     # per-function verdict for one TU
  uv run --no-project tools/needsdecl.py          # summary: declared-but-unneeded per header
"""
import re, sys, glob, pathlib

SRC = "src/legoland"

def defs_in(path):
    """name -> def line number, for functions defined in this .c"""
    out = {}
    lines = pathlib.Path(path).read_text().splitlines()
    for i, l in enumerate(lines):
        if re.match(r'//\s*FUNCTION:\s*LEGOLAND', l):
            sig = lines[i+1] if i+1 < len(lines) else ""
            m = re.search(r'([A-Za-z_]\w*)\s*\(', sig)
            if m:
                out[m.group(1)] = i + 1
    return out, lines

def referenced(name, text, exclude_def_line=None, lines=None, before=None):
    """is `name` used as a call / &addr / assignment target in text?"""
    pat = re.compile(r'(?<![A-Za-z0-9_])' + re.escape(name) + r'(?![A-Za-z0-9_])')
    for m in pat.finditer(text):
        ln = text.count("\n", 0, m.start())
        if before is not None and ln >= before:
            continue
        # skip the definition line itself
        if exclude_def_line is not None and ln == exclude_def_line:
            continue
        # context: a real use is followed by ( or , or ; or ) or is &name
        tail = text[m.end():m.end()+1]
        head = text[max(0, m.start()-1):m.start()]
        if tail in "(" or head == "&" or tail in ";),":
            return True
    return False

def analyze(tu):
    cpath = f"{SRC}/{tu}.c"
    d, lines = defs_in(cpath)
    own = "\n".join(lines)
    others = {p: pathlib.Path(p).read_text() for p in glob.glob(f"{SRC}/*.c") if p != cpath}
    verdicts = {}
    for name, defln in d.items():
        cross = any(referenced(name, t) for t in others.values())
        fwd = referenced(name, own, exclude_def_line=defln, before=defln)
        verdicts[name] = ("cross-TU" if cross else "") + ("+fwd" if fwd else "") or "NOT NEEDED"
    return verdicts

def main():
    if len(sys.argv) == 2:
        tu = sys.argv[1].replace(".c", "").replace(".h", "")
        v = analyze(tu)
        need = [n for n, r in v.items() if r != "NOT NEEDED"]
        print(f"{tu}: {len(need)}/{len(v)} functions need a declaration")
        for n, r in v.items():
            print(f"  {'KEEP ' if r!='NOT NEEDED' else 'drop '} {n:32} {r}")
        return
    # summary over all TUs that currently have a header
    for hp in sorted(glob.glob(f"{SRC}/*.h")):
        tu = pathlib.Path(hp).stem
        if not pathlib.Path(f"{SRC}/{tu}.c").exists():
            continue
        v = analyze(tu)
        need = sum(1 for r in v.values() if r != "NOT NEEDED")
        # count actual function prototypes declared in the .h (not structs/typedefs)
        protos = [n for n in v if re.search(r'(?<![A-Za-z0-9_])' + re.escape(n) + r'\s*\(',
                                            pathlib.Path(hp).read_text())]
        extra = len(protos) - need
        flag = "  <-- OVER-DECLARED" if extra > 0 else ""
        print(f"  {tu+'.h':28} declares {len(protos):2} / needs {need:2}{flag}")

if __name__ == "__main__":
    main()
