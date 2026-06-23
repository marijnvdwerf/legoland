# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp"]
# ///
"""Show per-TU progress table: matched / unmatched / stub counts.

    uv run tools/progress.py
"""

from __future__ import annotations

import os
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
            for line in f:
                m = match_marker(line)
                if m and m.module == "LEGOLAND":
                    addr_to_tu[m.offset] = (src.stem, m.type)
    return addr_to_tu


def main():
    project = RecCmpProject.from_directory(Path("."))
    target = project.get("LEGOLAND")
    cmp = Compare.from_target(target)

    addr_to_tu = parse_annotations(target.source_paths[0])

    tu_stats: dict[str, dict[str, int]] = {}
    for diff in cmp.compare_all():
        entry = addr_to_tu.get(diff.orig_addr)
        if entry is None:
            continue
        tu, marker_type = entry
        stats = tu_stats.setdefault(tu, {"matched": 0, "unmatched": 0, "stub": 0})
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

    header = f"{'':2s} {col_tu:<{w_tu}}  {col_m:>{w_m}}  {col_u:>{w_u}}  {col_s:>{w_s}}  {col_pct:>{w_pct}}"
    sep = f"{'':2s} {'-' * w_tu}  {'-' * w_m}  {'-' * w_u}  {'-' * w_s}  {'-' * w_pct}"

    print(header)
    print(sep)

    total_m = total_u = total_s = 0
    for tu in sorted(tu_stats):
        s = tu_stats[tu]
        game_total = s["matched"] + s["unmatched"]
        pct = s["matched"] / game_total * 100 if game_total else 0
        mark = "✓" if s["unmatched"] == 0 and game_total > 0 else " "
        print(
            f"{mark:2s} {tu:<{w_tu}}  {s['matched']:>{w_m}}  {s['unmatched']:>{w_u}}  {s['stub']:>{w_s}}  {pct:>{w_pct}.1f}%"
        )
        total_m += s["matched"]
        total_u += s["unmatched"]
        total_s += s["stub"]

    print(sep)
    game_total = total_m + total_u
    pct = total_m / game_total * 100 if game_total else 0
    print(
        f"{'':2s} {'TOTAL':<{w_tu}}  {total_m:>{w_m}}  {total_u:>{w_u}}  {total_s:>{w_s}}  {pct:>{w_pct}.1f}%"
    )


if __name__ == "__main__":
    main()
