# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp>=0.1.7"]
# ///
"""Show per-TU progress, or details for one TU."""

from __future__ import annotations

import os
import re
import sys
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parent
os.environ["PATH"] = str(TOOLS_DIR) + os.pathsep + os.environ.get("PATH", "")

from reccmp.compare.core import Compare
from reccmp.parser.marker import MarkerType, match_marker
from reccmp.project.detect import RecCmpProject


def parse_annotations(source_dir: Path) -> dict[int, tuple[str, str]]:
    annotations = {}
    for src in sorted(source_dir.glob("*.c")):
        lines = src.read_text().splitlines()
        for i, line in enumerate(lines):
            marker = match_marker(line)
            if (
                marker
                and marker.module == "LEGOLAND"
                and marker.type == MarkerType.FUNCTION
            ):
                name = re.search(r"([A-Za-z_]\w*)\s*\(", lines[i + 1]).group(1)
                annotations[marker.offset] = (src.stem, name)
    return annotations


RESET = "\033[0m"
USE_COLOR = sys.stdout.isatty()
_BRAILLE_BASE = 0x2800
_DOT_BITS = [1, 2, 4, 0x40, 8, 0x10, 0x20, 0x80]


def _braille_bar(count: int, max_width: int = 20) -> str:
    n = min(count, max_width * 8)
    result = chr(_BRAILLE_BASE | 0xFF) * (n // 8)
    if n % 8:
        result += chr(_BRAILLE_BASE | sum(_DOT_BITS[: n % 8]))
    return result


def _fg(r: int, g: int, b: int) -> str:
    return f"\033[38;2;{r};{g};{b}m" if USE_COLOR else ""


def classify(ratio: float) -> str:
    return "matched" if ratio == 1.0 else "partial" if ratio > 0 else "unmatched"


def main():
    target = RecCmpProject.from_directory(Path(".")).get("LEGOLAND")
    ratios = {
        d.orig_addr: d.effective_accuracy
        for d in Compare.from_target(target).compare_all()
    }
    functions = {
        address: (tu, name, ratios[address])
        for address, (tu, name) in parse_annotations(target.source_paths[0]).items()
    }
    stats, first = {}, {}
    for address, (tu, _name, ratio) in functions.items():
        s = stats.setdefault(tu, {"matched": 0, "partial": 0, "unmatched": 0})
        s[classify(ratio)] += 1
        first[tu] = min(first.get(tu, address), address)

    if len(sys.argv) > 1:
        tu = sys.argv[1].removesuffix(".c")
        selected = sorted(
            (
                (a, name, ratio)
                for a, (function_tu, name, ratio) in functions.items()
                if function_tu == tu
            ),
            key=lambda function: function[0],
        )
        width = max(len(name) for _address, name, _ratio in selected)
        for address, name, ratio in selected:
            marker = {"matched": "🟢", "partial": "🟡", "unmatched": "🔴"}[
                classify(ratio)
            ]
            print(f"{marker} 0x{address:08x} {name:<{width}} {ratio * 100:7.2f}%")
        s = stats[tu]
        total = sum(s.values())
        print(
            f"{tu}: Matched {s['matched']}  Partial {s['partial']}  Unmatched {s['unmatched']}  {s['matched'] / total * 100:.1f}%"
        )
        return

    w_tu, w_m, w_p, w_u, w_pct = max(len("TU"), *(map(len, stats))), 7, 7, 9, 6
    header = f"{'':2s} {'TU':<{w_tu}}  {'Matched':>{w_m}}  {'Partial':>{w_p}}  {'Unmatched':>{w_u}}  {'%':>{w_pct}}  Remaining"
    sep = f"{'':2s} {'-' * w_tu}  {'-' * w_m}  {'-' * w_p}  {'-' * w_u}  {'-' * w_pct}  ---------"
    print(header)
    print(sep)
    totals = {key: 0 for key in ("matched", "partial", "unmatched")}
    for tu in sorted(stats, key=first.__getitem__):
        s = stats[tu]
        total = sum(s.values())
        pct = s["matched"] / total * 100
        done = s["matched"] == total
        pct_str = f"{pct:>{w_pct}.1f}%"
        values = (
            f"{s['matched']:>{w_m}}  {s['partial']:>{w_p}}  {s['unmatched']:>{w_u}}"
        )
        if done:
            mark = f"{_fg(80, 220, 80)}✓{RESET} " if USE_COLOR else "✓ "
            row = (
                f"{_fg(100, 100, 100)}{tu:<{w_tu}}  {values}  {pct_str}{RESET}"
                if USE_COLOR
                else f"{tu:<{w_tu}}  {values}  {pct_str}"
            )
        else:
            mark = "  "
            bar = (
                f"{_fg(100, 100, 100)}{_braille_bar(total - s['matched'])}{RESET}"
                if USE_COLOR
                else _braille_bar(total - s["matched"])
            )
            row = f"{tu:<{w_tu}}  {values}  {pct_str}  {bar}"
        print(mark + row)
        for key in totals:
            totals[key] += s[key]
    print(sep)
    total = sum(totals.values())
    bar = _braille_bar(total - totals["matched"])
    if USE_COLOR:
        bar = f"{_fg(100, 100, 100)}{bar}{RESET}"
    print(
        f"{'':2s} {'TOTAL':<{w_tu}}  {totals['matched']:>{w_m}}  {totals['partial']:>{w_p}}  {totals['unmatched']:>{w_u}}  {totals['matched'] / total * 100:>{w_pct}.1f}%  {bar}"
    )


if __name__ == "__main__":
    main()
