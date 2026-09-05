#!/usr/bin/env -S uv run --script
# /// script
# requires-python = ">=3.10"
# dependencies = [
#   "tree-sitter==0.26.0",
#   "tree-sitter-c==0.24.2",
#   "reccmp",
# ]
# ///
"""Print a source-oriented call graph for the Legoland decompilation."""

from __future__ import annotations

import argparse
import logging
import os
import re
import sys
from collections import OrderedDict
from dataclasses import dataclass, field
from pathlib import Path
from typing import Iterable

import tree_sitter_c
from tree_sitter import Language, Parser

ROOT = Path(__file__).resolve().parents[1]
TOOLS_DIR = Path(__file__).resolve().parent
os.environ["PATH"] = str(TOOLS_DIR) + os.pathsep + os.environ.get("PATH", "")
from reccmp.compare.core import Compare
from reccmp.project.detect import RecCmpProject

SRC_DIR = ROOT / "src" / "legoland"
C_LANGUAGE = Language(tree_sitter_c.language())


@dataclass
class Function:
    name: str
    file: str
    kind: str  # "matched", "partial", "stub", "runtime", or "synthetic"
    calls: list[str] = field(default_factory=list)

    @property
    def marker(self) -> str:
        return {
            "matched": "🟢",
            "partial": "🟡",
            "stub": "🔴",
            "runtime": "  ",
            "synthetic": "  ",
        }[self.kind]


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


def calls(body, source: bytes, macro_names: set[str]) -> list[str]:
    found: list[str] = []
    for node in nodes(body):
        if node.type != "call_expression":
            continue
        target = node.child_by_field_name("function")
        if target is None or target.type != "identifier":
            continue
        name = text(source, target)
        if name not in macro_names:
            found.append(name)
    return found


def match_results() -> dict[int, float]:
    logging.disable(logging.CRITICAL)
    project = RecCmpProject.from_directory(ROOT)
    target = project.get("LEGOLAND")
    compare = Compare.from_target(target)
    return {diff.orig_addr: diff.effective_ratio for diff in compare.compare_all()}


def index_sources(functions: Functions, matches: dict[int, float]) -> None:
    macro_names = macros()
    parser = Parser(C_LANGUAGE)
    for path in sorted(SRC_DIR.glob("*.c")):
        source = path.read_bytes()
        addresses = {
            source.count(b"\n", 0, marker.start()): int(marker.group(1), 16)
            for marker in re.finditer(
                rb"(?m)^// FUNCTION: LEGOLAND (0x[0-9A-Fa-f]+)\s*$", source
            )
        }
        tree = parser.parse(source)
        for definition in nodes(tree.root_node):
            if definition.type != "function_definition":
                continue
            name = function_name(definition.child_by_field_name("declarator"), source)
            body = definition.child_by_field_name("body")
            found = calls(body, source, macro_names) if body is not None else []
            runtime = path.name in {"crt.c", "imports.c"}
            if runtime:
                kind = "runtime"
            else:
                address = addresses.get(definition.start_point[0] - 1)
                if address is None:
                    kind = "synthetic"
                else:
                    matching = matches.get(address)
                    kind = (
                        "synthetic"
                        if matching is None
                        else "matched"
                        if matching == 1
                        else "stub"
                        if matching == 0
                        else "partial"
                    )
            functions[name] = Function(
                name,
                str(path.relative_to(ROOT)),
                kind,
                [] if runtime else found,
            )


def build_index() -> Functions:
    functions: Functions = OrderedDict()
    index_sources(functions, match_results())
    for function in list(functions.values()):
        function.calls = list(dict.fromkeys(function.calls))
        for name in function.calls:
            functions.setdefault(name, Function(name, "", "runtime"))
    return functions


def render(functions: Functions, root: str, show_runtime: bool) -> str:
    lines: list[str] = []
    expanded: set[str] = set()
    active: set[str] = set()

    def walk(name: str, prefix: str, last: bool, root_node: bool) -> None:
        function = functions.get(name, Function(name, "", "runtime"))
        suffix = (
            " (cycle)"
            if name in active
            else (
                " (see above)"
                if name in expanded and function.kind == "matched"
                else ""
            )
        )
        branch = "" if root_node else ("└─ " if last else "├─ ")
        location = f" [{function.file}]" if function.file else ""
        line = f"{function.marker} {function.name}{location}{suffix}"
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
            if show_runtime or functions[child].kind != "runtime"
        ]
        for index, child in enumerate(children):
            walk(child, child_prefix, index == len(children) - 1, False)
        active.remove(name)

    walk(root, "", True, True)
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
