# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp"]
# ///
"""Show per-TU progress table: matched / unmatched / stub counts.

uv run tools/progress.py
"""

from __future__ import annotations

import os
import sys
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parent
os.environ["PATH"] = str(TOOLS_DIR) + os.pathsep + os.environ.get("PATH", "")

from reccmp.compare.core import Compare
from reccmp.parser.marker import MarkerType, match_marker
from reccmp.project.detect import RecCmpProject


def parse_annotations(source_dir: Path) -> dict[int, tuple[str, MarkerType]]:
    addr_to_tu: dict[int, tuple[str, MarkerType]] = {}
    for src in sorted(source_dir.glob("*.c")):
        with open(src) as f:
            lines = f.readlines()
        for i, line in enumerate(lines):
            m = match_marker(line)
            if m and m.module == "LEGOLAND":
                marker_type = m.type
                # For FUNCTION markers, check whether the body is just STUB().
                # Scan the next few lines for STUB() before the closing brace.
                if marker_type == MarkerType.FUNCTION:
                    for j in range(i + 1, min(i + 6, len(lines))):
                        body_line = lines[j]
                        if "STUB()" in body_line:
                            marker_type = MarkerType.STUB
                            break
                        # Stop scanning if we hit the closing brace or another marker
                        if body_line.strip() == "}" or match_marker(body_line):
                            break
                addr_to_tu[m.offset] = (src.stem, marker_type)
    return addr_to_tu


RESET = "\033[0m"
USE_COLOR = sys.stdout.isatty()

# Braille dots encode 2 columns × 4 rows. Dot bit positions:
#   1  4
#   2  5
#   3  6
#   7  8
# For a horizontal bar, fill left-to-right, top-to-bottom.
_BRAILLE_BASE = 0x2800
_DOT_BITS = [0x01, 0x02, 0x04, 0x40, 0x08, 0x10, 0x20, 0x80]


def _braille_bar(count: int, max_width: int = 20) -> str:
    """Render `count` braille dots in a horizontal bar of at most max_width chars."""
    if count == 0:
        return ""
    dots_per_char = 8
    total_dots = max_width * dots_per_char
    n = min(count, total_dots)
    full_chars = n // dots_per_char
    remainder = n % dots_per_char
    result = chr(_BRAILLE_BASE | 0xFF) * full_chars
    if remainder > 0:
        bits = 0
        for i in range(remainder):
            bits |= _DOT_BITS[i]
        result += chr(_BRAILLE_BASE | bits)
    return result


def _fg(r: int, g: int, b: int) -> str:
    return f"\033[38;2;{r};{g};{b}m" if USE_COLOR else ""


def dim() -> str:
    return _fg(100, 100, 100)


def green_check() -> str:
    return f"{_fg(80, 220, 80)}✓{RESET}" if USE_COLOR else "✓"


def reset() -> str:
    return RESET if USE_COLOR else ""


def main():
    project = RecCmpProject.from_directory(Path("."))
    target = project.get("LEGOLAND")
    cmp = Compare.from_target(target)

    addr_to_tu = parse_annotations(target.source_paths[0])

    tu_stats: dict[str, dict[str, int]] = {}
    tu_min_addr: dict[str, int] = {}
    for diff in cmp.compare_all():
        entry = addr_to_tu.get(diff.orig_addr)
        if entry is None:
            continue
        tu, marker_type = entry
        stats = tu_stats.setdefault(tu, {"matched": 0, "unmatched": 0, "stub": 0})
        if tu not in tu_min_addr or diff.orig_addr < tu_min_addr[tu]:
            tu_min_addr[tu] = diff.orig_addr
        if marker_type == MarkerType.STUB:
            stats["stub"] += 1
        elif diff.effective_ratio == 1.0:
            stats["matched"] += 1
        else:
            stats["unmatched"] += 1

    col_tu = "TU"
    col_m = "Matched"
    col_u = "Unmatched"
    col_s = "Stub"
    col_pct = "%"

    w_tu = max(len(col_tu), *(len(tu) for tu in tu_stats))
    w_m = max(len(col_m), 4)
    w_u = max(len(col_u), 4)
    w_s = max(len(col_s), 4)
    w_pct = max(len(col_pct), 6)

    header = f"{'':2s} {col_tu:<{w_tu}}  {col_m:>{w_m}}  {col_u:>{w_u}}  {col_s:>{w_s}}  {col_pct:>{w_pct}}  Remaining"
    sep = f"{'':2s} {'-' * w_tu}  {'-' * w_m}  {'-' * w_u}  {'-' * w_s}  {'-' * w_pct}  ---------"

    print(header)
    print(sep)

    total_m = total_u = total_s = 0
    for tu in sorted(tu_stats, key=lambda t: tu_min_addr.get(t, 0)):
        s = tu_stats[tu]
        game_total = s["matched"] + s["unmatched"] + s["stub"]
        pct = s["matched"] / game_total * 100 if game_total else 0
        is_done = s["unmatched"] == 0 and s["stub"] == 0 and game_total > 0
        pct_str = f"{pct:>{w_pct}.1f}%"

        if is_done:
            mark = f"{green_check()} "
            row = f"{dim()}{tu:<{w_tu}}  {s['matched']:>{w_m}}  {s['unmatched']:>{w_u}}  {s['stub']:>{w_s}}  {pct_str}{reset()}"
        else:
            mark = "  "
            remaining = s["unmatched"] + s["stub"]
            bar = f"{_fg(100, 100, 100)}{_braille_bar(remaining)}{reset()}" if remaining else ""
            row = f"{tu:<{w_tu}}  {s['matched']:>{w_m}}  {s['unmatched']:>{w_u}}  {s['stub']:>{w_s}}  {pct_str}  {bar}"

        print(f"{mark}{row}")
        total_m += s["matched"]
        total_u += s["unmatched"]
        total_s += s["stub"]

    print(sep)
    game_total = total_m + total_u + total_s
    pct = total_m / game_total * 100 if game_total else 0
    pct_str = f"{pct:>{w_pct}.1f}%"
    remaining = total_u + total_s
    bar = f"{_fg(100, 100, 100)}{_braille_bar(remaining)}{reset()}" if remaining else ""
    print(
        f"{'':2s} {'TOTAL':<{w_tu}}  {total_m:>{w_m}}  {total_u:>{w_u}}  {total_s:>{w_s}}  {pct_str}  {bar}"
    )


if __name__ == "__main__":
    main()
