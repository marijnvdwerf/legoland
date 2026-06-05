#!/usr/bin/env python3
"""Print the integration allowlist for a TU + address range.

Allowlist = functions that are (a) currently STUB() in the .c file and
(b) byte-matched in port2 (current_score == 0). Agents may integrate ONLY these.

Usage: uv run --no-project tools/allowlist.py src/legoland/nerps.c 0x46bce0 0x46ce00
"""
import re, sys, csv, pathlib

CSV = "/Users/marijn/Projects/legoland/port2/project/functions.csv"

def main():
    path, lo, hi = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16)
    score = {}
    with open(CSV) as f:
        for row in csv.DictReader(f):
            score[int(row["offset"])] = (int(row["current_score"]), int(row["max_score"]))
    src = pathlib.Path(path).read_text().splitlines()
    allow, partial, stub_unmatched = [], [], []
    for i, l in enumerate(src):
        m = re.match(r'//\s*FUNCTION:\s*LEGOLAND\s+(0x[0-9a-fA-F]+)', l)
        if not m:
            continue
        a = int(m.group(1), 16)
        if not (lo <= a <= hi):
            continue
        is_stub = any('STUB();' in src[j] for j in range(i + 1, min(i + 4, len(src))))
        if not is_stub:
            continue
        s = score.get(a)
        if s and s[0] == 0:
            allow.append(a)
        elif s:
            partial.append((a, s))
        else:
            stub_unmatched.append(a)
    print(f"# {path}  range {hex(lo)}-{hex(hi)}")
    print(f"# ALLOWLIST (STUB now + port2-matched): {len(allow)}")
    print(" ".join(hex(a) for a in allow))
    print(f"# OFF-LIMITS partial in port2: {len(partial)} -> "
          + " ".join(f"{hex(a)}({s[0]}/{s[1]})" for a, s in partial))
    print(f"# OFF-LIMITS not in port2: {len(stub_unmatched)} -> "
          + " ".join(hex(a) for a in stub_unmatched))

if __name__ == "__main__":
    main()
