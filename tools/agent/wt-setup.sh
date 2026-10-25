#!/bin/sh
# Run from a fresh worktree root: links toolchain, configures, builds, writes reccmp configs.
set -e
export PATH="$HOME/.local/bin:$PATH"
[ -e toolchain ] || ln -s "$(git rev-parse --git-common-dir)/../toolchain" toolchain
cp "$(git rev-parse --git-common-dir)/../reccmp-user.yml" . 2>/dev/null || true
sed -i "s#original_path: .*#original_path: $PWD/external/legoland.exe#" reccmp-user.yml 2>/dev/null || true
cmake --preset msvc6 >/dev/null
cmake --build build >/dev/null
printf 'project: .\ntargets:\n  LEGOLAND:\n    path: %s/build/legoland.exe\n    pdb: %s/build/legoland.pdb\n' "$PWD" "$PWD" > reccmp-build.yml
cp reccmp-build.yml build/reccmp-build.yml
echo "worktree ready: $PWD"
