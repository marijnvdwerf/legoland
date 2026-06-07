#!/usr/bin/env -S uv run --no-project python3
"""Sort src/legoland/globals.c entries by global offset (ascending).

reccmp matches data references by the `// GLOBAL: LEGOLAND 0x<addr>` annotation
address, NOT by definition order in the file. Reordering the definitions is
therefore byte-neutral: the recompiled binary still matches per-function. We sort
globals.c so it mirrors globals.h (which is already offset-sorted), keeping
overlap analysis easy.

Usage:
    uv run --no-project tools/sort_globals.py

Idempotent: running it twice produces no change, so it is safe to re-run after
future global additions.

How it works:
  1. Read src/legoland/globals.c (path is relative to the repo root, i.e. the
     parent directory of this script's `tools/` dir).
  2. The preamble = every line BEFORE the first `// GLOBAL:` line; preserved verbatim.
  3. Entries are parsed by chunking: a chunk starts at a `// GLOBAL: LEGOLAND 0x<addr>`
     line and extends up to (but not including) the next `// GLOBAL:` line or EOF.
     Trailing blank lines are stripped from each chunk. This robustly keeps any
     multi-line definition with its annotation.
  4. Chunks are stably sorted by the hex address parsed from their first line.
  5. Output = preamble + one blank line + sorted chunks, each separated by a single
     newline (uniform spacing; no double blank lines).

Safety assertions abort WITHOUT writing if violated:
  - the count of `// GLOBAL:` annotations is unchanged,
  - the multiset of all non-preamble, non-blank lines is identical before/after
    (only reordering happened: nothing edited, dropped, or duplicated),
  - output addresses are non-decreasing.
"""

import collections
import pathlib
import re
import sys

GLOBAL_RE = re.compile(r"^// GLOBAL: LEGOLAND (0x[0-9a-fA-F]+)\b")

REPO_ROOT = pathlib.Path(__file__).resolve().parent.parent
GLOBALS_C = REPO_ROOT / "src" / "legoland" / "globals.c"


def is_global_line(line: str) -> bool:
    return GLOBAL_RE.match(line) is not None


def parse_addr(line: str) -> int:
    m = GLOBAL_RE.match(line)
    if m is None:
        raise ValueError(f"not a // GLOBAL: line: {line!r}")
    return int(m.group(1), 16)


def strip_trailing_blank(lines):
    out = list(lines)
    while out and out[-1].strip() == "":
        out.pop()
    return out


def nonblank(lines):
    return [ln for ln in lines if ln.strip() != ""]


def main() -> int:
    text = GLOBALS_C.read_text()
    lines = text.splitlines()  # drops line terminators; no empty trailing element

    # 1. Find the first // GLOBAL: line.
    first_global = next(
        (i for i, ln in enumerate(lines) if is_global_line(ln)), None
    )
    if first_global is None:
        print("error: no // GLOBAL: line found", file=sys.stderr)
        return 1

    preamble = lines[:first_global]
    body = lines[first_global:]

    # 2. Chunk the body: each chunk starts at a // GLOBAL: line.
    chunks = []
    cur = None
    for ln in body:
        if is_global_line(ln):
            if cur is not None:
                chunks.append(cur)
            cur = [ln]
        else:
            cur.append(ln)
    if cur is not None:
        chunks.append(cur)

    chunks = [strip_trailing_blank(c) for c in chunks]

    # 3. Stable sort by address from each chunk's first line.
    sorted_chunks = sorted(chunks, key=lambda c: parse_addr(c[0]))

    # 4. Reassemble: preamble + blank line + chunks separated by a single newline.
    out_lines = list(preamble)
    # Ensure exactly one blank line between preamble and the first chunk.
    while out_lines and out_lines[-1].strip() == "":
        out_lines.pop()
    out_lines.append("")
    for i, c in enumerate(sorted_chunks):
        out_lines.extend(c)
        if i != len(sorted_chunks) - 1:
            out_lines.append("")
    out_text = "\n".join(out_lines) + "\n"

    # --- SAFETY ASSERTIONS (abort before writing if violated) ---
    in_global_count = sum(1 for ln in lines if is_global_line(ln))
    out_global_count = sum(1 for ln in out_text.splitlines() if is_global_line(ln))
    assert out_global_count == in_global_count, (
        f"// GLOBAL: count changed: in={in_global_count} out={out_global_count}"
    )

    # Key invariant: only reordering. Compare multiset of non-preamble, non-blank
    # lines before and after.
    in_body_lines = collections.Counter(nonblank(body))
    out_body_lines = collections.Counter(
        nonblank(out_text.splitlines()[len(preamble):])
    )
    if in_body_lines != out_body_lines:
        added = out_body_lines - in_body_lines
        removed = in_body_lines - out_body_lines
        raise AssertionError(
            "non-preamble line multiset changed (not a pure reorder):\n"
            f"  added: {dict(added)}\n  removed: {dict(removed)}"
        )

    # Preamble must be preserved verbatim.
    out_preamble = out_text.splitlines()[:len(preamble)]
    assert out_preamble == preamble, "preamble was modified"

    # Output addresses must be non-decreasing.
    addrs = [parse_addr(ln) for ln in out_text.splitlines() if is_global_line(ln)]
    assert addrs == sorted(addrs), "output addresses are not non-decreasing"

    if out_text == text:
        print(f"sort_globals: already sorted ({in_global_count} globals); no change")
        return 0

    GLOBALS_C.write_text(out_text)
    print(f"sort_globals: sorted {in_global_count} globals by offset; wrote {GLOBALS_C}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
