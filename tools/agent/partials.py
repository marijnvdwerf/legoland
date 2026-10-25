# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp>=0.1.7"]
# ///
import sys, json
sys.path.insert(0, "tools")
sys.argv=[sys.argv[0]]
import progress as P
from pathlib import Path
t = P.RecCmpProject.from_directory(Path(".")).get("LEGOLAND")
c = P.Compare.from_target(t)
r = {d.orig_addr: d.effective_accuracy for d in c.compare_all()}
ann, starts = P.parse_annotations(t.source_paths[0])
asm = P.find_inline_asm(c.orig_bin, starts)
out=[]
for a,(tu,name) in ann.items():
    if a in asm: continue
    if 0 < r[a] < 1: out.append((tu, hex(a), name, round(r[a]*100,2)))
json.dump(sorted(out), open("/tmp/partials.json","w"), indent=0)
print(len(out))
