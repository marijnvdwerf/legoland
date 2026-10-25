# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp>=0.1.7"]
# ///
"""Classify diffs: count control-flow mismatches (jcc kind flips, ret/epilogue moves) vs register-only renames."""
import os, sys, logging, re
from pathlib import Path
TOOLS = Path(__file__).resolve().parent.parent
os.environ["PATH"] = str(TOOLS) + os.pathsep + os.environ.get("PATH", "")
logging.disable(logging.CRITICAL)
from reccmp.compare.core import Compare
from reccmp.project.detect import RecCmpProject
t = RecCmpProject.from_directory(Path(".")).get("LEGOLAND")
c = Compare.from_target(t)
REG = re.compile(r"\b(e?[abcd]x|[abcd][lh]|e?[sd]i|e?bp)\b")
def norm(s):
    s = s.split(" \t(")[0]
    s = REG.sub("R", s)
    s = re.sub(r"^(j\w+) .*", r"\1", s)
    return s
for a in sys.argv[1:]:
    r = c.compare_address(int(a, 16))
    if r is None or r.rdiff is None: continue
    d = r.rdiff
    o = [norm(x[1]) for x in d.orig_inst]
    n = [norm(x[1]) for x in d.recomp_inst]
    from collections import Counter
    co, cn = Counter(o), Counter(n)
    cf = 0
    for k in set(co) | set(cn):
        if k.startswith("j") or k.startswith("ret") or k in ("nop",):
            continue
        cf += abs(co[k] - cn[k])
    jo = Counter(x for x in o if x.startswith("j") or x == "ret")
    jn = Counter(x for x in n if x.startswith("j") or x == "ret")
    jd = sum(abs(jo[k] - jn[k]) for k in set(jo) | set(jn))
    print(f"{a} {r.name:32} {r.accuracy*100:6.2f} n={len(o):4} multiset_diff={cf:3} jumpdiff={jd:3}")
