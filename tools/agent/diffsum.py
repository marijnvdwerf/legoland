# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp>=0.1.7"]
# ///
"""For each addr: accuracy, #orig instrs, #differing lines, and a compact list of differing mnemonics."""
import os, sys, logging, json
from pathlib import Path
TOOLS = Path(__file__).resolve().parent.parent
os.environ["PATH"] = str(TOOLS) + os.pathsep + os.environ.get("PATH", "")
logging.disable(logging.CRITICAL)
from reccmp.compare.core import Compare
from reccmp.project.detect import RecCmpProject
t = RecCmpProject.from_directory(Path(".")).get("LEGOLAND")
c = Compare.from_target(t)
for a in sys.argv[1:]:
    r = c.compare_address(int(a, 16))
    if r is None or r.rdiff is None:
        continue
    d = r.rdiff
    no = len(d.orig_inst)
    nd = 0
    kinds = {}
    for tag, i1, i2, j1, j2 in d.codes:
        if tag == "equal": continue
        nd += max(i2 - i1, j2 - j1)
        for k in range(i1, i2):
            m = d.orig_inst[k][1].split()[0] if d.orig_inst[k][1] else ""
            kinds[m] = kinds.get(m, 0) + 1
    ks = " ".join(f"{k}:{v}" for k, v in sorted(kinds.items(), key=lambda x: -x[1])[:6])
    print(f"{a} {r.name:32} {r.accuracy*100:6.2f} n={no:4} diff={nd:4} {ks}")
