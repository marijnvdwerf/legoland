# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp"]
# ///
"""Generate an objdiff-format report.json from reccmp comparison results.

Uses the reccmp library to compare functions and reccmp's marker parser
to group results by translation unit.

    uv run tools/report.py                  # -> report.json
    uv run tools/report.py -o progress.json # custom output path
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path

from reccmp.compare.core import Compare
from reccmp.parser.marker import MarkerType, match_marker
from reccmp.project.detect import RecCmpProject


def parse_annotations(source_dir: Path) -> dict[int, str]:
    """Use reccmp's marker parser to map orig addresses to TU names."""
    addr_to_tu: dict[int, str] = {}
    for src in sorted(source_dir.glob("*.c")):
        with open(src) as f:
            for line in f:
                m = match_marker(line)
                if m and m.module == "LEGOLAND" and m.type == MarkerType.FUNCTION:
                    addr_to_tu[m.offset] = src.stem
    return addr_to_tu


def build_report(cmp: Compare, addr_to_tu: dict[int, str]) -> dict:
    """Build an objdiff v2 report from reccmp comparison results."""

    tu_funcs: dict[str, list[dict]] = {}

    for diff in cmp.compare_all():
        tu = addr_to_tu.get(diff.orig_addr)
        if tu is None:
            continue

        match = cmp._db.get_one_match(diff.orig_addr)
        size = match.any_size() if match else 0

        tu_funcs.setdefault(tu, []).append(
            {
                "addr": diff.orig_addr,
                "name": diff.name,
                "size": size,
                "ratio": diff.effective_ratio,
                "stub": diff.is_stub,
            }
        )

    units = []
    total_m = _empty_measures()

    for tu in sorted(tu_funcs):
        funcs = [f for f in tu_funcs[tu] if not f["stub"]]
        if not funcs:
            continue

        unit_code = sum(f["size"] for f in funcs)
        unit_matched = sum(f["size"] for f in funcs if f["ratio"] == 1.0)
        unit_matched_fns = sum(1 for f in funcs if f["ratio"] == 1.0)
        unit_fuzzy_w = sum(f["ratio"] * 100.0 * f["size"] for f in funcs)
        unit_fuzzy = unit_fuzzy_w / unit_code if unit_code else 0.0
        complete = all(f["ratio"] == 1.0 for f in funcs)

        um = {
            "fuzzy_match_percent": unit_fuzzy,
            "total_code": unit_code,
            "matched_code": unit_matched,
            "matched_code_percent": unit_matched / unit_code * 100 if unit_code else 0,
            "total_data": 0,
            "matched_data": 0,
            "matched_data_percent": 100.0,
            "total_functions": len(funcs),
            "matched_functions": unit_matched_fns,
            "matched_functions_percent": unit_matched_fns / len(funcs) * 100,
            "complete_code": unit_code if complete else 0,
            "complete_code_percent": 100.0 if complete else 0.0,
            "complete_data": 0,
            "complete_data_percent": 100.0,
            "total_units": 1,
            "complete_units": 1 if complete else 0,
        }

        units.append(
            {
                "name": tu,
                "measures": um,
                "sections": [
                    {
                        "name": ".text",
                        "size": unit_code,
                        "fuzzy_match_percent": unit_fuzzy,
                        "metadata": {},
                    }
                ],
                "functions": [
                    {
                        "name": f["name"],
                        "size": f["size"],
                        "fuzzy_match_percent": f["ratio"] * 100.0,
                        "metadata": {"virtual_address": f["addr"]},
                    }
                    for f in funcs
                ],
                "metadata": {
                    "complete": complete,
                    "source_path": f"src/legoland/{tu}.c",
                },
            }
        )

        _add_measures(total_m, um)

    total_m["fuzzy_match_percent"] = (
        total_m["fuzzy_match_percent"] / total_m["total_code"]
        if total_m["total_code"]
        else 0
    )
    _calc_percents(total_m, len(units))

    return {"measures": total_m, "units": units, "version": 2, "categories": []}


def _empty_measures() -> dict:
    return {
        "fuzzy_match_percent": 0.0,
        "total_code": 0,
        "matched_code": 0,
        "total_data": 0,
        "matched_data": 0,
        "total_functions": 0,
        "matched_functions": 0,
        "complete_code": 0,
        "complete_data": 0,
        "complete_units": 0,
    }


def _add_measures(total: dict, unit: dict):
    total["fuzzy_match_percent"] += unit["fuzzy_match_percent"] * unit["total_code"]
    total["total_code"] += unit["total_code"]
    total["matched_code"] += unit["matched_code"]
    total["total_functions"] += unit["total_functions"]
    total["matched_functions"] += unit["matched_functions"]
    total["complete_code"] += unit["complete_code"]
    total["complete_units"] += unit["complete_units"]


def _calc_percents(m: dict, n_units: int):
    tc = m["total_code"]
    m["matched_code_percent"] = m["matched_code"] / tc * 100 if tc else 0
    m["matched_data_percent"] = 100.0
    tf = m["total_functions"]
    m["matched_functions_percent"] = m["matched_functions"] / tf * 100 if tf else 0
    m["complete_code_percent"] = m["complete_code"] / tc * 100 if tc else 0
    m["complete_data_percent"] = 100.0
    m["total_units"] = n_units


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("-o", "--output", default="report.json")
    args = parser.parse_args()

    project = RecCmpProject.from_directory(Path("."))
    target = project.get("LEGOLAND")
    cmp = Compare.from_target(target)

    addr_to_tu = parse_annotations(target.source_paths[0])
    report = build_report(cmp, addr_to_tu)

    with open(args.output, "w") as f:
        json.dump(report, f, indent=2)

    m = report["measures"]
    print(
        f"{m['total_functions']} functions, "
        f"{m['matched_functions']} matched ({m['matched_functions_percent']:.1f}%), "
        f"fuzzy {m['fuzzy_match_percent']:.1f}%, "
        f"{m['total_units']} TUs ({m['complete_units']} complete)"
    )


if __name__ == "__main__":
    main()
