# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp>=0.1.7"]
# ///
"""Flag partials whose diff contains layout-dependent operands reccmp cannot normalize."""
import os, sys, logging, re
from pathlib import Path
TOOLS = Path(__file__).resolve().parent.parent
os.environ["PATH"] = str(TOOLS) + os.pathsep + os.environ.get("PATH", "")
logging.disable(logging.CRITICAL)
from reccmp.compare.core import Compare
from reccmp.project.detect import RecCmpProject
t = RecCmpProject.from_directory(Path(".")).get("LEGOLAND")
c = Compare.from_target(t)
raw = re.compile(r"0x(4[0-9a-f]{5}|[5-8][0-9a-f]{5})\b")
for a in sys.argv[1:]:
    r = c.compare_address(int(a, 16))
    if r is None or r.rdiff is None: continue
    d = r.rdiff
    flags = set()
    for tag, i1, i2, j1, j2 in d.codes:
        if tag == "equal": continue
        for k in range(i1, i2):
            o = d.orig_inst[k][1]
            if raw.search(o): flags.add("rawaddr")
            if "*4 + 0x" in o or "*8 + 0x" in o: flags.add("table")
        for k in range(j1, j2):
            x = d.recomp_inst[k][1]
            if re.search(r"cmp \w+, .*\((FLOAT|OFFSET|DATA)\)", x) : flags.add("cmpsym")
            if "EditCursor+" in x: flags.add("editcursor")
    print(a, r.name, f"{r.accuracy*100:.2f}", " ".join(sorted(flags)))
