# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp>=0.1.7"]
# ///
"""Side-by-side full asm of original vs recompiled for given function addresses."""
import os, sys
from pathlib import Path
TOOLS = Path(__file__).resolve().parent.parent
os.environ["PATH"] = str(TOOLS) + os.pathsep + os.environ.get("PATH", "")
import logging
logging.disable(logging.CRITICAL)
from reccmp.compare.core import Compare
from reccmp.project.detect import RecCmpProject

addrs = [int(a, 16) for a in sys.argv[1:]]
t = RecCmpProject.from_directory(Path(".")).get("LEGOLAND")
c = Compare.from_target(t)
for a in addrs:
    r = c.compare_address(a)
    if r is None or r.rdiff is None:
        print(hex(a), "no result"); continue
    d = r.rdiff
    print(f"== {r.name} {r.accuracy*100:.2f}%" + (" EFFECTIVE" if r.is_effective_match else ""))
    for tag, i1, i2, j1, j2 in d.codes:
        n = max(i2 - i1, j2 - j1)
        for k in range(n):
            o = d.orig_inst[i1 + k] if i1 + k < i2 else ("", "")
            rr = d.recomp_inst[j1 + k] if j1 + k < j2 else ("", "")
            mark = " " if tag == "equal" else ("~" if tag == "replace" else ("-" if tag == "delete" else "+"))
            ri = rr[1].split(" \t(")[0]
            print(f"{mark} {o[0]:>8} {o[1]:<44} | {ri}")
