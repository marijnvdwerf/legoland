# /// script
# requires-python = ">=3.11"
# dependencies = []
# ///
"""Download the self-contained build toolchain into ./toolchain (gitignored).

Pulls the original MSVC6 compiler, the `wibo` PE loader, and wibo's msvcrt
support DLLs. URLs are pinned here on purpose — we deliberately do NOT read
decompme's values.yaml at runtime.

    uv run setup.py            # download anything missing
    uv run setup.py --force    # re-download everything
"""

from __future__ import annotations

import argparse
import os
import platform
import shutil
import subprocess
import sys
import tarfile
import tempfile
import urllib.request
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent
TOOLCHAIN = ROOT / "toolchain"

# --- Pinned URLs (sourced once from decompme/compilers values.yaml; not fetched at runtime) ---
MSVC6_URL = "https://github.com/OmniBlade/decomp.me/releases/download/msvcwin9x/msvc6.0.tar.gz"
MSVCRT_DLLS_URL = "https://files.decomp.dev/msvcrt_20251015.zip"

# --- wibo: built from source so our kernel32 patch (string-literal compile support) is applied. ---
WIBO_REPO = "https://github.com/decompals/wibo"
WIBO_TAG = "1.1.0"
WIBO_PATCH = ROOT / "tools" / "wibo.patch"
WIBO_BUILD_DIR = TOOLCHAIN / "wibo-src"  # gitignored clone/build tree
# cmake preset (configure + build) and resulting binary path within WIBO_BUILD_DIR, per platform.
WIBO_PRESET = {"Darwin": "release-macos", "Linux": "release"}
WIBO_BUILT = {"Darwin": "build/release/wibo", "Linux": "build/release/wibo"}


def download(url: str, dest: Path) -> None:
    dest.parent.mkdir(parents=True, exist_ok=True)
    print(f"  ↓ {url}")
    req = urllib.request.Request(url, headers={"User-Agent": "legoland-setup"})
    with urllib.request.urlopen(req) as resp, open(dest, "wb") as out:
        shutil.copyfileobj(resp, out)


def extract_into(archive: Path, dest: Path) -> None:
    """Extract archive into dest, flattening a single wrapping top-level dir if present."""
    dest.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory() as td:
        tmp = Path(td)
        if archive.name.endswith((".tar.gz", ".tgz")):
            with tarfile.open(archive) as tf:
                tf.extractall(tmp)  # noqa: S202 - trusted pinned source
        elif archive.suffix == ".zip":
            with zipfile.ZipFile(archive) as zf:
                zf.extractall(tmp)
        else:
            raise ValueError(f"unknown archive type: {archive.name}")
        entries = [p for p in tmp.iterdir() if not p.name.startswith(".")]
        roots = [tmp / e.name for e in entries]
        src = roots[0] if len(roots) == 1 and roots[0].is_dir() else tmp
        for item in src.iterdir():
            target = dest / item.name
            if target.exists():
                shutil.rmtree(target) if target.is_dir() else target.unlink()
            shutil.move(str(item), str(target))


def step_msvc6(force: bool) -> None:
    dest = TOOLCHAIN / "msvc6"
    if dest.exists() and not force:
        print("msvc6: present, skipping")
        return
    if force and dest.exists():
        shutil.rmtree(dest)
    print("msvc6: downloading + extracting")
    with tempfile.NamedTemporaryFile(suffix=".tar.gz", delete=False) as tmp:
        archive = Path(tmp.name)
    try:
        download(MSVC6_URL, archive)
        extract_into(archive, dest)
    finally:
        archive.unlink(missing_ok=True)


def step_dlls(force: bool) -> None:
    dest = TOOLCHAIN / "dlls"
    if dest.exists() and not force:
        print("dlls: present, skipping")
        return
    if force and dest.exists():
        shutil.rmtree(dest)
    print("dlls: downloading + extracting")
    with tempfile.NamedTemporaryFile(suffix=".zip", delete=False) as tmp:
        archive = Path(tmp.name)
    try:
        download(MSVCRT_DLLS_URL, archive)
        extract_into(archive, dest)
    finally:
        archive.unlink(missing_ok=True)


def run(cmd: list[str], cwd: Path | None = None) -> None:
    print("  $ " + " ".join(cmd))
    subprocess.run(cmd, cwd=cwd, check=True)


def step_wibo(force: bool) -> None:
    """Build wibo from source at WIBO_TAG with tools/wibo.patch applied.

    The patch adds kernel32 lstrcpynA/lstrcpynW (so the MSVC6 cl.exe can compile
    string literals) plus GetTempFileNameW. We build instead of downloading the
    release asset because that prebuilt binary lacks these imports.
    """
    dest = TOOLCHAIN / "wibo"
    if dest.exists() and not force:
        print("wibo: present, skipping")
        return

    system = platform.system()
    preset = WIBO_PRESET.get(system)
    built = WIBO_BUILT.get(system)
    if preset is None or built is None:
        sys.exit(f"wibo: no build preset for platform {system!r}")
    for tool in ("git", "cmake", "ninja"):
        if shutil.which(tool) is None:
            sys.exit(f"wibo: required build tool {tool!r} not found on PATH")

    # Fresh clone every build so the patch applies cleanly and is reproducible.
    if WIBO_BUILD_DIR.exists():
        shutil.rmtree(WIBO_BUILD_DIR)
    print(f"wibo: cloning {WIBO_REPO} @ {WIBO_TAG}")
    run(["git", "clone", "--quiet", "--depth", "1", "--branch", WIBO_TAG, WIBO_REPO, str(WIBO_BUILD_DIR)])

    print(f"wibo: applying {WIBO_PATCH.relative_to(ROOT)}")
    run(["git", "apply", str(WIBO_PATCH)], cwd=WIBO_BUILD_DIR)

    print(f"wibo: building (preset {preset})")
    run(["cmake", "--preset", preset], cwd=WIBO_BUILD_DIR)
    run(["cmake", "--build", "--preset", preset], cwd=WIBO_BUILD_DIR)

    binary = WIBO_BUILD_DIR / built
    if not binary.exists():
        sys.exit(f"wibo: build finished but binary missing at {binary}")
    shutil.copy2(binary, dest)
    dest.chmod(0o755)


def write_clangd() -> None:
    """Write .clangd so the editor can parse the MSVC6 headers (machine-local path)."""
    inc = TOOLCHAIN / "msvc6" / "Include"
    (ROOT / ".clangd").write_text(
        "CompileFlags:\n"
        "  Compiler: clang\n"
        "  Add:\n"
        "    - --target=i686-pc-windows-msvc\n"
        "    - -fms-extensions\n"
        "    - -fms-compatibility\n"
        "    - -fms-compatibility-version=12.00\n"
        "    - -fdeclspec\n"
        "    - -isystem\n"
        f"    - {inc}\n"
    )


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--force", action="store_true", help="re-download everything")
    args = ap.parse_args()

    TOOLCHAIN.mkdir(exist_ok=True)
    step_msvc6(args.force)
    step_dlls(args.force)
    step_wibo(args.force)
    write_clangd()

    print("\nToolchain ready under ./toolchain")
    for sub in ("msvc6", "dlls"):
        p = TOOLCHAIN / sub
        n = sum(1 for _ in p.rglob("*")) if p.exists() else 0
        print(f"  {sub:8} {n} files")
    wibo = TOOLCHAIN / "wibo"
    print(f"  wibo     {'ok' if wibo.exists() and os.access(wibo, os.X_OK) else 'MISSING'}")


if __name__ == "__main__":
    main()
