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
WIBO_RELEASE = "https://github.com/decompals/wibo/releases/download/1.1.0"
WIBO_ASSET = {"Darwin": "wibo-macos", "Linux": "wibo-x86_64"}


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


def step_wibo(force: bool) -> None:
    dest = TOOLCHAIN / "wibo"
    if dest.exists() and not force:
        print("wibo: present, skipping")
        return
    asset = WIBO_ASSET.get(platform.system())
    if asset is None:
        sys.exit(f"wibo: no asset for platform {platform.system()!r}")
    print(f"wibo: downloading {asset}")
    download(f"{WIBO_RELEASE}/{asset}", dest)
    dest.chmod(0o755)


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--force", action="store_true", help="re-download everything")
    args = ap.parse_args()

    TOOLCHAIN.mkdir(exist_ok=True)
    step_msvc6(args.force)
    step_dlls(args.force)
    step_wibo(args.force)

    print("\nToolchain ready under ./toolchain")
    for sub in ("msvc6", "dlls"):
        p = TOOLCHAIN / sub
        n = sum(1 for _ in p.rglob("*")) if p.exists() else 0
        print(f"  {sub:8} {n} files")
    wibo = TOOLCHAIN / "wibo"
    print(f"  wibo     {'ok' if wibo.exists() and os.access(wibo, os.X_OK) else 'MISSING'}")


if __name__ == "__main__":
    main()
