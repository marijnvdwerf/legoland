# /// script
# requires-python = ">=3.11"
# dependencies = []
# ///
"""Render the objdiff-format report.json (from tools/report.py) as a progress treemap SVG.

Each translation unit is a tile sized by its code bytes and coloured by its size-weighted match
percentage; a header shows the overall numbers.

    uv run tools/report.py -o report.json
    uv run tools/progress_svg.py report.json -o docs/progress.svg
"""

from __future__ import annotations

import argparse
import html
import json

W, H = 960, 560
HEAD = 92  # header height
PAD = 2


def colour(p: float) -> str:
    """Red below 80%, then orange -> yellow -> light green up to 99.99%; complete units deep green.
    The scale is stretched over 80-100% because that is where the units differ."""
    if p >= 100.0:
        return "#1f9d55"
    stops = [(0.0, (198, 40, 40)), (80.0, (214, 72, 48)), (90.0, (232, 145, 40)), (96.0, (214, 196, 52)),
             (99.99, (110, 190, 90))]
    for (p0, c0), (p1, c1) in zip(stops, stops[1:]):
        if p <= p1:
            t = (p - p0) / (p1 - p0) if p1 > p0 else 0
            c = [round(a + (b - a) * t) for a, b in zip(c0, c1)]
            return "#%02x%02x%02x" % tuple(c)
    return "#5aaf50"


def squarify(items, x, y, w, h):
    """Squarified treemap. items: list of (value, payload), values > 0, sorted descending."""
    out = []
    items = list(items)
    total = sum(v for v, _ in items)
    if not items or total <= 0:
        return out

    def worst(row, side, scale):
        s = sum(v for v, _ in row) * scale
        if s <= 0:
            return float("inf")
        mx = max(v for v, _ in row) * scale
        mn = min(v for v, _ in row) * scale
        return max(side * side * mx / (s * s), (s * s) / (side * side * mn))

    while items:
        scale = (w * h) / sum(v for v, _ in items)
        side = min(w, h)
        row = [items[0]]
        i = 1
        while i < len(items) and worst(row + [items[i]], side, scale) <= worst(row, side, scale):
            row.append(items[i])
            i += 1
        rs = sum(v for v, _ in row) * scale
        if w >= h:  # lay the row as a column on the left
            cw = rs / h
            cy = y
            for v, p in row:
                ch = v * scale / cw
                out.append((x, cy, cw, ch, p))
                cy += ch
            x += cw
            w -= cw
        else:  # lay the row along the top
            rh = rs / w
            cx = x
            for v, p in row:
                cw2 = v * scale / rh
                out.append((cx, y, cw2, rh, p))
                cx += cw2
            y += rh
            h -= rh
        items = items[i:]
    return out


def render(report: dict) -> str:
    m = report["measures"]
    units = [u for u in report["units"] if int(u["measures"].get("total_code", 0)) > 0]
    units.sort(key=lambda u: -int(u["measures"]["total_code"]))
    tiles = squarify([(int(u["measures"]["total_code"]), u) for u in units], 0, HEAD, W, H - HEAD)

    parts = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
        'font-family="Segoe UI, Helvetica, Arial, sans-serif">',
        f'<rect width="{W}" height="{H}" fill="#0d1117"/>',
    ]
    fuzzy = float(m.get("fuzzy_match_percent", 0))
    fn, tf = int(m.get("matched_functions", 0)), int(m.get("total_functions", 0))
    code = float(m.get("matched_code_percent", 0))
    cu, tu = int(m.get("complete_units", 0)), int(m.get("total_units", 0))
    parts.append('<text x="16" y="34" fill="#f0f6fc" font-size="24" font-weight="700">legoland.exe decompilation</text>')
    stats = [
        ("functions matched", f"{fn:,} / {tf:,}", f"{fn / tf * 100:.1f}%" if tf else ""),
        ("fuzzy match", f"{fuzzy:.2f}%", "size-weighted"),
        ("code matched", f"{code:.2f}%", "bytes in 100% functions"),
        ("units complete", f"{cu} / {tu}", "translation units"),
    ]
    sx = 16
    for label, big, small in stats:
        parts.append(f'<text x="{sx}" y="66" fill="#f0f6fc" font-size="20" font-weight="600">{html.escape(big)}</text>')
        parts.append(f'<text x="{sx}" y="84" fill="#8b949e" font-size="11">{html.escape(label)} · {html.escape(small)}</text>')
        sx += 236
    # legend
    for k, (p, lab) in enumerate(((75, "&lt;80"), (90, "90"), (96, "96"), (99.9, "99"), (100, "100"))):
        parts.append(f'<rect x="{W - 180 + k * 34}" y="16" width="30" height="10" fill="{colour(p)}"/>')
        parts.append(f'<text x="{W - 180 + k * 34}" y="40" fill="#8b949e" font-size="10">{lab}%</text>')

    for x, y, w, h, u in tiles:
        um = u["measures"]
        p = float(um.get("fuzzy_match_percent", 0))
        name = u["name"]
        tip = f'{name}: {p:.2f}% fuzzy, {int(um.get("matched_functions", 0))}/{int(um.get("total_functions", 0))} functions'
        parts.append(
            f'<g><title>{html.escape(tip)}</title>'
            f'<rect x="{x + PAD / 2:.1f}" y="{y + PAD / 2:.1f}" width="{max(w - PAD, 0):.1f}" height="{max(h - PAD, 0):.1f}" '
            f'rx="3" fill="{colour(p)}"/></g>'
        )
        if w > 46 and h > 22:
            fs = 12 if w > 90 and h > 34 else 9
            fit = int((w - 8) / (fs * 0.6))  # characters that fit the tile width
            label = name if len(name) <= fit else name[: max(fit - 1, 1)] + "…"
            parts.append(f'<text x="{x + 5:.1f}" y="{y + fs + 4:.1f}" fill="#0d1117" font-size="{fs}" font-weight="600">{html.escape(label)}</text>')
            if h > 34 and w > 60:
                parts.append(f'<text x="{x + 5:.1f}" y="{y + 2 * fs + 8:.1f}" fill="#0d1117" font-size="{fs - 1}">{p:.1f}%</text>')
    parts.append("</svg>")
    return "\n".join(parts) + "\n"


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("report", nargs="?", default="report.json")
    ap.add_argument("-o", "--output", default="docs/progress.svg")
    args = ap.parse_args()
    with open(args.report) as f:
        report = json.load(f)
    svg = render(report)
    with open(args.output, "w") as f:
        f.write(svg)
    print("wrote", args.output)


if __name__ == "__main__":
    main()
