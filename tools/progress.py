# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp>=0.1.7"]
# ///
"""Show per-TU progress, details for one TU, or `--worst [N]` (the N lowest-scoring functions)."""

from __future__ import annotations

import os
import re
import sys
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parent
os.environ["PATH"] = str(TOOLS_DIR) + os.pathsep + os.environ.get("PATH", "")

import capstone
from capstone import x86
import reccmp_fixes  # noqa: F401  (project fix for a reccmp false positive; see the module)
from reccmp.compare.core import Compare
from reccmp.parser.marker import MarkerType, match_marker
from reccmp.project.detect import RecCmpProject

# Instructions MSVC6 never emits from C. A function whose original code contains one
# was hand-written assembly and needs inline __asm to byte-match.
INLINE_ASM = {
    "pushal", "popal", "rdtsc", "fnstcw", "fldcw", "fist", "fistp", "shrd", "shld",
    "movsw", "rep stosw", "rep movsw", "lodsb", "lodsw", "lodsd", "cld", "std",
    "loop", "wait", "xchg", "bswap",
}  # fmt: skip


def parse_annotations(source_dir: Path) -> tuple[dict[int, tuple[str, str]], list[int]]:
    """Return FUNCTION annotations, and the start of every annotated function (incl. STUBs)."""
    annotations, starts = {}, []
    for src in sorted(source_dir.glob("*.c")):
        lines = src.read_text(encoding="latin-1").splitlines()
        for i, line in enumerate(lines):
            marker = match_marker(line)
            if not marker or marker.module != "LEGOLAND":
                continue
            if marker.type in (MarkerType.FUNCTION, MarkerType.STUB):
                starts.append(marker.offset)
            if marker.type == MarkerType.FUNCTION:
                name = re.search(r"([A-Za-z_]\w*)\s*\(", re.sub(r"__declspec\(\w+\)", "", lines[i + 1])).group(1)
                annotations[marker.offset] = (src.stem, name)
    return annotations, sorted(set(starts))


def find_inline_asm(image, starts: list[int]) -> set[int]:
    """Addresses of original functions that contain inline asm."""
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    found = set()
    for start, end in zip(starts, starts[1:]):
        code = image.read(start, end - start)
        # MSVC places jump tables after the code: stop before the first one
        for ins in md.disasm(code, start):
            for op in ins.operands:
                if op.type == x86.X86_OP_MEM and start < op.mem.disp < end:
                    end = op.mem.disp
        if any(
            ins.mnemonic in INLINE_ASM
            or (ins.mnemonic == "and" and ins.op_str.endswith(", 0xffffffff"))
            for ins in md.disasm(code[: end - start], start)
        ):
            found.add(start)
    return found


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
    compare = Compare.from_target(target)
    ratios = {d.orig_addr: d.effective_accuracy for d in compare.compare_all()}
    annotations, starts = parse_annotations(target.source_paths[0])
    functions = {
        address: (tu, name, ratios[address])
        for address, (tu, name) in annotations.items()
    }
    inline_asm = find_inline_asm(compare.orig_bin, starts) & functions.keys()
    stats, first = {}, {}
    for address, (tu, _name, ratio) in functions.items():
        s = stats.setdefault(tu, {"matched": 0, "partial": 0, "unmatched": 0})
        s[classify(ratio)] += 1
        first[tu] = min(first.get(tu, address), address)

    if len(sys.argv) > 1 and sys.argv[1] == "--worst":
        print_worst(
            functions, inline_asm, int(sys.argv[2]) if len(sys.argv) > 2 else 30
        )
        return

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
            asm = "  inline asm" if address in inline_asm else ""
            print(f"{marker} 0x{address:08x} {name:<{width}} {ratio * 100:7.2f}%{asm}")
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
    print_split(functions, inline_asm)


def print_worst(functions: dict, inline_asm: set[int], count: int):
    """The lowest-scoring functions that are not yet 100%."""
    worst = sorted(
        (
            (ratio, address, tu, name)
            for address, (tu, name, ratio) in functions.items()
            if ratio < 1.0
        ),
    )[:count]
    width = max(len(name) for _, _, _, name in worst)
    for ratio, address, tu, name in worst:
        asm = "  inline asm" if address in inline_asm else ""
        print(f"{ratio * 100:6.2f}%  0x{address:08x}  {tu:<16} {name:<{width}}{asm}")


def print_split(functions: dict, inline_asm: set[int]):
    """Totals split by whether the original function used inline asm."""
    groups = {
        "Pure C": [r for a, (_, _, r) in functions.items() if a not in inline_asm],
        "Inline asm": [r for a, (_, _, r) in functions.items() if a in inline_asm],
        "All": [r for _, _, r in functions.values()],
    }
    print()
    print(
        f"   {'':<10}  {'Funcs':>5}  {'Matched':>7}  {'Partial':>7}  {'Unmatched':>9}  {'%':>6}  {'Avg sim.':>8}"
    )
    for label, ratios in groups.items():
        count = {c: 0 for c in ("matched", "partial", "unmatched")}
        for ratio in ratios:
            count[classify(ratio)] += 1
        n = max(len(ratios), 1)
        print(
            f"   {label:<10}  {len(ratios):>5}  {count['matched']:>7}  {count['partial']:>7}  {count['unmatched']:>9}  {count['matched'] / n * 100:>5.1f}%  {sum(ratios) / n * 100:>7.2f}%"
        )
    print(
        "   Inline asm: the original uses instructions MSVC6 never emits from C;"
        " match these with __asm."
    )


if __name__ == "__main__":
    main()
