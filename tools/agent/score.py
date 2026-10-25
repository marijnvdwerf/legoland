# /// script
# requires-python = ">=3.11"
# dependencies = ["reccmp>=0.1.7"]
# ///
"""Print match % for given function addresses."""
import os, sys, logging
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
    print(a, r.name if r else "?", (f"{r.accuracy*100:.2f}" + (" EFFECTIVE" if r.is_effective_match else "")) if r else "none")
