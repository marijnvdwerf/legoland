#!/usr/bin/env python3
# /// script
# requires-python = ">=3.11"
# dependencies = ["tree-sitter>=0.22", "tree-sitter-c"]
# ///
"""Project linter for the one-header-per-TU convention.

Two checks run over src/legoland/:

  Check 1 — .c files carry no function pre-declarations (prototypes).
    Prototypes belong in headers, not .c files. Reports every function
    prototype in a .c file (extern declarations, forward declarations),
    even if the function is defined later in the same file.

  Check 2 — every prototype in a header foo.h is defined in foo.c.
    A header foo.h is the canonical header for foo.c and must declare only
    the functions DEFINED in foo.c. A prototype for a function that lives in
    a different TU bar.c belongs in bar.h, not foo.h (the anti-pattern: a
    catch-all header re-declaring dozens of other TUs' functions). Each
    header prototype is classified as:
      OK        defined in the accompanying foo.c
      MISLOCATED  defined in a different TU bar.c -> belongs in bar.h
      ORPHAN    defined in no .c at all (typo or missing function)
      NO-TU     the header has no accompanying .c (e.g. legoland.h)

Usage:
  uv run --no-project tools/needsdecl.py          # both checks, tree-wide
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
    """If this declarator (possibly through pointers/parens) names a function, return it."""
    if node is None:
        return None
    if node.type == "function_declarator":
        decl = node.child_by_field_name("declarator")
        if decl and decl.type == "identifier":
            return decl.text.decode()
        # e.g. struct Foo *(*GetThing)(...) — keep unwrapping
        return _find_func_name(decl)
    if node.type in ("pointer_declarator", "parenthesized_declarator"):
        return _find_func_name(node.child_by_field_name("declarator"))
    return None


def _decl_func_name(decl_node):
    """Extract function name from a declaration node, if it's a function prototype."""
    for child in decl_node.children:
        name = _find_func_name(child)
        if name:
            return name
    return None


def find_predecls(path):
    """Return [(name, line, source_text), ...] for function pre-declarations/prototypes.

    Works on .c and .h files. Skips typedefs and the bodies of definitions,
    so a function_definition is never reported here.
    """
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


def find_defs(path):
    """Return the set of function NAMES defined (with a body) in a .c file.

    Handles the LEGO_EXPORT macro prefix: MSVC's __declspec-style macro makes
    tree-sitter split a pointer-returning definition into a stray `declaration`
    plus a `function_definition` whose declarator still carries the real name,
    so we read the name straight off each function_definition's declarator.
    """
    tree, _ = parse(path)
    names = set()

    def walk(node):
        if node.type == "function_definition":
            name = _find_func_name(node.child_by_field_name("declarator"))
            if name:
                names.add(name)
            return  # don't descend into the body
        for child in node.children:
            walk(child)

    walk(tree.root_node)
    return names


def build_def_index():
    """name -> stem of the .c that defines it (e.g. 'LoadBinV' -> 'binv')."""
    index = {}
    for path in sorted(glob.glob(f"{SRC}/*.c")):
        stem = pathlib.Path(path).stem
        for name in find_defs(path):
            index.setdefault(name, stem)
    return index


def classify_header(header_path, def_index):
    """Classify each prototype in a header.

    Returns (ok, mislocated, orphan, no_tu) where each is a list of
    (name, line, text[, owner]) tuples; `owner` present for mislocated.
    """
    stem = pathlib.Path(header_path).stem
    has_tu = pathlib.Path(f"{SRC}/{stem}.c").exists()
    ok, mislocated, orphan, no_tu = [], [], [], []
    for name, line, text in find_predecls(header_path):
        owner = def_index.get(name)
        if not has_tu:
            no_tu.append((name, line, text, owner))
        elif owner == stem:
            ok.append((name, line, text))
        elif owner is None:
            orphan.append((name, line, text))
        else:
            mislocated.append((name, line, text, owner))
    return ok, mislocated, orphan, no_tu


def run_check1():
    """Returns dirty list [(tu, predecls)]; prints nothing."""
    dirty = []
    for path in sorted(glob.glob(f"{SRC}/*.c")):
        predecls = find_predecls(path)
        if predecls:
            dirty.append((pathlib.Path(path).stem, predecls))
    return dirty


def run_check2(def_index):
    """Returns per-header classification dict {stem: (ok, mis, orphan, no_tu)}."""
    out = {}
    for path in sorted(glob.glob(f"{SRC}/*.h")):
        out[pathlib.Path(path).stem] = (path, classify_header(path, def_index))
    return out


def main():
    if len(sys.argv) == 2:
        tu = sys.argv[1].replace(".c", "").replace(".h", "")

        print(f"=== {tu}: .c pre-declarations (check 1) ===")
        cpath = f"{SRC}/{tu}.c"
        if not pathlib.Path(cpath).exists():
            print(f"  (no {tu}.c)")
        else:
            predecls = find_predecls(cpath)
            if not predecls:
                print("  clean")
            else:
                for name, line, text in predecls:
                    print(f"  {tu}.c:{line:<4} {text}")

        print(f"\n=== {tu}: header / TU function-decl match (check 2) ===")
        hpath = f"{SRC}/{tu}.h"
        if not pathlib.Path(hpath).exists():
            print(f"  (no {tu}.h)")
            return
        def_index = build_def_index()
        ok, mislocated, orphan, no_tu = classify_header(hpath, def_index)
        if not (mislocated or orphan or no_tu):
            print(f"  clean ({len(ok)} prototypes, all defined in {tu}.c)")
            return
        for name, line, text, owner in no_tu:
            where = f"defined in {owner}.c" if owner else "defined nowhere"
            print(f"  {tu}.h:{line:<4} declares {name} — {where} "
                  f"(header has no accompanying .c)")
        for name, line, text, owner in mislocated:
            print(f"  {tu}.h:{line:<4} declares {name} — defined in {owner}.c "
                  f"(belongs in {owner}.h)")
        for name, line, text in orphan:
            print(f"  {tu}.h:{line:<4} declares {name} — defined nowhere")
        return

    # ---- tree-wide run: both checks ----
    print("=== check 1: .c files contain no function pre-declarations ===")
    dirty = run_check1()
    if not dirty:
        print("All .c files are clean.")
    else:
        for tu, predecls in dirty:
            print(f"  {tu + '.c':28} {len(predecls):3} pre-declarations")
        print(f"\n{len(dirty)} files with pre-declarations, "
              f"{sum(len(p) for _, p in dirty)} total")

    print("\n=== check 2: every header prototype is defined in its own TU's .c ===")
    def_index = build_def_index()
    results = run_check2(def_index)

    no_tu_rows, mis_rows, orphan_rows = [], [], []
    for stem, (path, (ok, mislocated, orphan, no_tu)) in results.items():
        for name, line, text, owner in no_tu:
            no_tu_rows.append((stem, line, name, owner))
        for name, line, text, owner in mislocated:
            mis_rows.append((stem, line, name, owner))
        for name, line, text in orphan:
            orphan_rows.append((stem, line, name))

    if not (no_tu_rows or mis_rows or orphan_rows):
        print("All headers match their TU.")
        return

    if no_tu_rows:
        print("\n  header has no accompanying .c:")
        for stem, line, name, owner in no_tu_rows:
            where = f"defined in {owner}.c" if owner else "defined nowhere"
            print(f"    {stem}.h:{line:<4} declares {name} — {where}")
    if mis_rows:
        print("\n  declared in the wrong TU's header:")
        for stem, line, name, owner in sorted(mis_rows):
            print(f"    {stem}.h:{line:<4} declares {name} — "
                  f"defined in {owner}.c (belongs in {owner}.h)")
    if orphan_rows:
        print("\n  defined nowhere (orphan decl):")
        for stem, line, name in sorted(orphan_rows):
            print(f"    {stem}.h:{line:<4} declares {name} — defined nowhere")

    total = len(no_tu_rows) + len(mis_rows) + len(orphan_rows)
    print(f"\n{total} header/TU violations "
          f"({len(mis_rows)} mislocated, {len(orphan_rows)} orphan, "
          f"{len(no_tu_rows)} in a .c-less header)")


if __name__ == "__main__":
    main()
