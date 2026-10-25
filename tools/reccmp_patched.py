"""Run reccmp with the project's fixes (tools/reccmp_fixes.py) applied. Used by tools/verify."""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import reccmp_fixes  # noqa: F401  (patches reccmp on import)
from reccmp.tools.asmcmp import main

if __name__ == "__main__":
    sys.exit(main())
