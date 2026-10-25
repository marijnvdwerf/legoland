# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp>=0.1.7"]
# ///
# Prints "<addr> <tu> <name> <pct>" for every game FUNCTION. Run from repo/worktree root.
import sys
sys.path.insert(0, "tools")
sys.argv=[sys.argv[0]]
import progress as P
from pathlib import Path
t = P.RecCmpProject.from_directory(Path(".")).get("LEGOLAND")
c = P.Compare.from_target(t)
r = {d.orig_addr: d.effective_accuracy for d in c.compare_all()}
ann, _ = P.parse_annotations(t.source_paths[0])
for a,(tu,name) in sorted(ann.items()):
    print(f"0x{a:08x} {tu} {name} {r.get(a,0)*100:.2f}")
