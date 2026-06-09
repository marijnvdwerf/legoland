#!/usr/bin/env python3
# /// script
# requires-python = ">=3.11"
# dependencies = ["tree-sitter>=0.22", "tree-sitter-c"]
# ///
"""Find .c files that contain function pre-declarations (prototypes without a body).

These belong in headers, not .c files. Reports every function prototype
in a .c file — including extern declarations and forward declarations,
even if the function is defined later in the same file.

Usage:
  uv run --no-project tools/needsdecl.py          # summary per file
  uv run --no-project tools/needsdecl.py <tu>     # details for one TU
"""
import sys, glob, pathlib
import tree_sitter_c as tsc
from tree_sitter import Language, Parser

C_LANG = Language(tsc.language())
SRC = "src/legoland"


def parse(path):
    src = pathlib.Path(path).read_bytes()
    return Parser(C_LANG).parse(src), src


def _find_func_name(node):
    """If this is a function_declarator with a plain identifier name, return it."""
    if node.type == "function_declarator":
        decl = node.child_by_field_name("declarator")
        if decl and decl.type == "identifier":
            return decl.text.decode()
        return None
    if node.type in ("pointer_declarator", "parenthesized_declarator"):
        inner = node.child_by_field_name("declarator")
        if inner:
            return _find_func_name(inner)
    return None


def _decl_func_name(decl_node):
    """Extract function name from a declaration node, if it's a function prototype."""
    for child in decl_node.children:
        name = _find_func_name(child)
        if name:
            return name
    return None


def find_predecls(path):
    """Return [(name, line, source_text), ...] for function pre-declarations."""
    tree, src = parse(path)
    lines = src.decode(errors="replace").splitlines()
    results = []

    def walk(node):
        if node.type in ("compound_statement", "struct_specifier",
                         "union_specifier", "enum_specifier"):
            return

        if node.type == "declaration":
            if any(c.type == "storage_class_specifier" and c.text == b"typedef"
                   for c in node.children):
                return
            name = _decl_func_name(node)
            if name:
                line = node.start_point[0]
                results.append((name, line + 1, lines[line].strip()))
            return

        for child in node.children:
            walk(child)

    walk(tree.root_node)
    return results


def main():
    if len(sys.argv) == 2:
        tu = sys.argv[1].replace(".c", "").replace(".h", "")
        predecls = find_predecls(f"{SRC}/{tu}.c")
        if not predecls:
            print(f"{tu}: clean")
            return
        print(f"{tu}: {len(predecls)} pre-declarations")
        for name, line, text in predecls:
            print(f"  line {line:4}  {text}")
        return

    dirty = []
    for path in sorted(glob.glob(f"{SRC}/*.c")):
        predecls = find_predecls(path)
        if predecls:
            tu = pathlib.Path(path).stem
            dirty.append((tu, len(predecls)))

    if not dirty:
        print("All .c files are clean.")
        return

    for tu, count in dirty:
        print(f"  {tu + '.c':28} {count:3} pre-declarations")
    print(f"\n{len(dirty)} files with pre-declarations, {sum(c for _, c in dirty)} total")


if __name__ == "__main__":
    main()
