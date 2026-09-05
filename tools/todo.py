#!/usr/bin/env -S uv run --script
# /// script
# requires-python = ">=3.10"
# dependencies = [
#   "tree-sitter==0.26.0",
#   "tree-sitter-c==0.24.2",
#   "reccmp",
# ]
# ///
"""List reachable, unfinished decompilation targets grouped by TU."""

from __future__ import annotations

from collections import defaultdict
from pathlib import Path

import callgraph
from progress import parse_annotations
from reccmp.compare.core import Compare
from reccmp.project.detect import RecCmpProject
from reccmp.types import ImageId


def main() -> None:
    functions = callgraph.build_index()
    reachable = set()

    def walk(name: str) -> None:
        if name in reachable:
            return
        reachable.add(name)
        for child in functions[name].calls:
            walk(child)

    walk("WinMain")
    addresses = {
        name: address
        for address, (_tu, name) in parse_annotations(callgraph.SRC_DIR).items()
    }
    target = RecCmpProject.from_directory(callgraph.ROOT).get("LEGOLAND")
    compare = Compare.from_target(target)
    original_functions = sorted(compare.get_functions(), key=lambda f: f.orig_addr)
    sizes = {
        function.orig_addr: function.size(ImageId.ORIG)
        or (
            (
                original_functions[index + 1].orig_addr
                if index + 1 < len(original_functions)
                else compare.orig_bin.sections[0].virtual_range.stop
            )
            - function.orig_addr
        )
        for index, function in enumerate(original_functions)
    }
    todos = defaultdict(list)
    for name in reachable:
        function = functions[name]
        if function.kind == "stub":
            address = addresses[name]
            todos[Path(function.file).stem].append((address, name, sizes[address]))

    name_width = max(
        len(name) for entries in todos.values() for _address, name, _size in entries
    )
    size_width = max(
        len(str(size))
        for entries in todos.values()
        for _address, _name, size in entries
    )
    for tu in sorted(
        todos, key=lambda tu: min(address for address, _name, _size in todos[tu])
    ):
        print(tu)
        for _address, name, size in sorted(todos[tu]):
            print(f"  {name:<{name_width}} {size:>{size_width}}")


if __name__ == "__main__":
    main()
