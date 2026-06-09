#!/usr/bin/env python3
# /// script
# requires-python = ">=3.11"
# dependencies = ["tree-sitter>=0.22", "tree-sitter-c"]
# ///
"""Which functions of a TU actually NEED a declaration in a header.

A function needs a header decl ONLY if it was pre-declared somewhere:
  - cross-TU: referenced from a .c other than its own, or
  - in-file forward: referenced in its own .c before its definition.
Everything else is defined-and-used-in-order and needs no declaration.

Usage:
  uv run --no-project tools/needsdecl.py <tu>     # per-function verdict for one TU
  uv run --no-project tools/needsdecl.py          # summary: declared-but-unneeded per header
"""
import re, sys, glob, pathlib
import tree_sitter_c as tsc
from tree_sitter import Language, Parser

C_LANG = Language(tsc.language())

SRC = "src/legoland"

# ── Caching ──────────────────────────────────────────────────────────

_trees = {}
_ident_maps = {}


def _get_tree(path):
    """Parse a .c file with tree-sitter, cached."""
    if path not in _trees:
        src = pathlib.Path(path).read_bytes()
        _trees[path] = Parser(C_LANG).parse(src)
    return _trees[path]


def _get_idents(path):
    """Return {name: [(start_byte, is_func_def_name), ...]} for all identifiers.

    Uses tree-sitter to walk the AST.  `identifier` nodes are real symbol
    references; `field_identifier` (struct member access via -> or .) is a
    different node type and never appears here — no heuristic filtering needed.
    """
    if path not in _ident_maps:
        tree = _get_tree(path)
        result = {}
        stack = [tree.root_node]
        while stack:
            node = stack.pop()
            if node.type == "identifier":
                name = node.text.decode()
                result.setdefault(name, []).append(
                    (node.start_byte, _is_func_def_name(node))
                )
            stack.extend(reversed(node.children))
        _ident_maps[path] = result
    return _ident_maps[path]


# ── AST helpers ──────────────────────────────────────────────────────

def _is_func_def_name(node):
    """True if this identifier is the name in a function_definition's declarator.

    Walks up through pointer_declarator / parenthesized_declarator to handle
    pointer-returning functions like `struct Foo *bar(...)`.
    """
    parent = node.parent
    if not parent or parent.type != "function_declarator":
        return False
    ancestor = parent.parent
    while ancestor and ancestor.type in ("pointer_declarator", "parenthesized_declarator"):
        ancestor = ancestor.parent
    return ancestor is not None and ancestor.type == "function_definition"


# ── Core logic ───────────────────────────────────────────────────────

def defs_in(path):
    """Return {name: def_byte_offset} for annotated functions in this .c file.

    Uses the // FUNCTION: LEGOLAND annotation (regex) to identify which functions
    belong to this TU, then computes the byte offset of the name on the next line.
    This is more robust than pure tree-sitter for definition finding because
    LEGO_EXPORT / __asm can cause parse errors that swallow function_definition
    nodes.
    """
    lines = pathlib.Path(path).read_text().splitlines()
    out = {}
    byte_offset = 0
    for i, line in enumerate(lines):
        if re.match(r"//\s*FUNCTION:\s*LEGOLAND", line):
            sig = lines[i + 1] if i + 1 < len(lines) else ""
            m = re.search(r"([A-Za-z_]\w*)\s*\(", sig)
            if m:
                out[m.group(1)] = byte_offset + len(line) + 1 + m.start(1)
        byte_offset += len(line) + 1
    return out


def has_reference(path, name, *, before_byte=None):
    """Is `name` used as an identifier (not a struct field) in this file?

    Optionally restrict to references before `before_byte` (for forward-decl check).
    Skips the identifier that is the function's own definition name.
    """
    for byte_off, is_def in _get_idents(path).get(name, []):
        if is_def:
            continue
        if before_byte is not None and byte_off >= before_byte:
            continue
        return True
    return False


def analyze(tu):
    cpath = f"{SRC}/{tu}.c"
    defs = defs_in(cpath)
    others = [p for p in glob.glob(f"{SRC}/*.c") if p != cpath]
    verdicts = {}
    for name, def_byte in defs.items():
        cross = any(has_reference(p, name) for p in others)
        fwd = has_reference(cpath, name, before_byte=def_byte)
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
        htext = pathlib.Path(hp).read_text()
        protos = [n for n in v if re.search(r'(?<![A-Za-z0-9_])' + re.escape(n) + r'\s*\(', htext)]
        extra = len(protos) - need
        flag = "  <-- OVER-DECLARED" if extra > 0 else ""
        print(f"  {tu+'.h':28} declares {len(protos):2} / needs {need:2}{flag}")


if __name__ == "__main__":
    main()
