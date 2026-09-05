#!/usr/bin/env -S uv run --script
# /// script
# requires-python = ">=3.10"
# dependencies = [
#   "tree-sitter==0.26.0",
#   "tree-sitter-c==0.24.2",
# ]
# ///
"""Print a source-oriented call graph for the Legoland decompilation."""

from __future__ import annotations

import argparse
import re
import sys
from collections import OrderedDict
from dataclasses import dataclass, field
from pathlib import Path
from typing import Iterable

import tree_sitter_c
from tree_sitter import Language, Parser

ROOT = Path(__file__).resolve().parents[1]
SRC_DIR = ROOT / "src" / "legoland"
C_LANGUAGE = Language(tree_sitter_c.language())
MAX_RENDER_DEPTH = 64


@dataclass
class Function:
    name: str
    file: str
    kind: str  # "matched", "stub", or "runtime"
    calls: list[str] = field(default_factory=list)

    @property
    def marker(self) -> str:
        return {"matched": "🟢", "stub": "🔴", "runtime": "  "}[self.kind]

    @property
    def display_name(self) -> str:
        return self.name


Functions = OrderedDict[str, Function]


def nodes(node):
    yield node
    for child in node.named_children:
        yield from nodes(child)


def text(source: bytes, node) -> str:
    return source[node.start_byte : node.end_byte].decode("utf-8", errors="replace")


def function_name(node, source: bytes) -> str | None:
    if node is None:
        return None
    if node.type == "identifier":
        return text(source, node)
    name = function_name(node.child_by_field_name("declarator"), source)
    if name is not None:
        return name
    for child in node.named_children:
        name = function_name(child, source)
        if name is not None:
            return name
    return None


def macros() -> set[str]:
    return {
        name
        for path in (*SRC_DIR.glob("*.c"), *SRC_DIR.glob("*.h"))
        for name in re.findall(
            r"(?m)^\s*#\s*define\s+([A-Za-z_]\w*)\s*\(",
            path.read_text(encoding="utf-8"),
        )
    }


def calls(body, source: bytes, macro_names: set[str]) -> tuple[list[str], bool]:
    found: list[str] = []
    stub = False
    for node in nodes(body):
        if node.type != "call_expression":
            continue
        target = node.child_by_field_name("function")
        if target is None or target.type != "identifier":
            continue
        name = text(source, target)
        stub |= name == "STUB"
        if name not in macro_names:
            found.append(name)
    return found, stub


def index_sources(functions: Functions) -> None:
    macro_names = macros()
    parser = Parser(C_LANGUAGE)
    for path in sorted(SRC_DIR.glob("*.c")):
        source = path.read_bytes()
        tree = parser.parse(source)
        for definition in nodes(tree.root_node):
            if definition.type != "function_definition":
                continue
            name = function_name(definition.child_by_field_name("declarator"), source)
            body = definition.child_by_field_name("body")
            found, stub = (
                calls(body, source, macro_names) if body is not None else ([], False)
            )
            runtime = path.name in {"crt.c", "imports.c"}
            functions[name] = Function(
                name,
                str(path.relative_to(ROOT)),
                "runtime" if runtime else "stub" if stub else "matched",
                [] if runtime or stub else found,
            )


def build_index() -> Functions:
    functions: Functions = OrderedDict()
    index_sources(functions)
    for function in list(functions.values()):
        function.calls = list(dict.fromkeys(function.calls))
        for name in function.calls:
            functions.setdefault(name, Function(name, "", "runtime"))
    return functions


def render(functions: Functions, root: str, show_runtime: bool) -> str:
    lines: list[str] = []
    expanded: set[str] = set()
    active: set[str] = set()

    def walk(name: str, prefix: str, last: bool, depth: int, root_node: bool) -> None:
        function = functions.get(name, Function(name, "", "runtime"))
        suffix = (
            " … (depth limit)"
            if depth >= MAX_RENDER_DEPTH
            else " (cycle)"
            if name in active
            else " (see above)"
            if name in expanded and function.kind == "matched"
            else ""
        )
        branch = "" if root_node else ("└─ " if last else "├─ ")
        location = f" [{function.file}]" if function.file else ""
        line = f"{function.marker} {function.display_name}{location}{suffix}"
        if function.kind == "runtime" and sys.stdout.isatty():
            line = f"\x1b[2m{line}\x1b[0m"
        lines.append(f"{prefix}{branch}{line}")
        if suffix:
            return
        expanded.add(name)
        active.add(name)
        child_prefix = prefix if root_node else prefix + ("   " if last else "│  ")
        children = [
            child
            for child in function.calls
            if show_runtime
            or functions.get(child, Function(child, "", "runtime")).kind != "runtime"
        ]
        for index, child in enumerate(children):
            walk(child, child_prefix, index == len(children) - 1, depth + 1, False)
        active.remove(name)

    walk(root, "", True, 0, True)
    return "\n".join(lines)


def parse_args(argv: Iterable[str], default_root: str) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Print the Legoland C-source call graph rooted at a function."
    )
    parser.add_argument(
        "--runtime", action="store_true", help="include runtime/library calls"
    )
    parser.add_argument(
        "root",
        nargs="?",
        default=default_root,
        help=f"root symbol (default: {default_root})",
    )
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] | None = None) -> int:
    functions = build_index()
    default_root = "WinMain" if "WinMain" in functions else "legoland_entry"
    args = parse_args(sys.argv[1:] if argv is None else argv, default_root)
    try:
        print(render(functions, args.root, args.runtime))
    except BrokenPipeError:
        return 0
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
