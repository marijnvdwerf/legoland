You are working on a matching decompilation of legoland.exe (MSVC6 /O2, x86, reccmp-style). Goal: make PARTIALLY-matching functions byte-identical (100%) using pure C.

You are in an ISOLATED GIT WORKTREE (your cwd). Work only there. Do not touch /home/user/legoland itself.

## Setup (run once, from your worktree root)
    export PATH=$HOME/.local/bin:$PATH
    tools/agent/wt-setup.sh          # links toolchain, builds, writes reccmp configs
Read CLAUDE.md and docs/decomp-tips.md (MSVC6 codegen tricks) first. learnings/*.md has a worked example.

## Your batch
Your function list is in the batch file named in the task (format: `addr file name current%`). All are partial matches written in pure C (not inline asm), so 100% should be reachable.

## Loop per function
1. `./tools/verify -v <addr>` shows the asm diff (original vs recompiled). Target asm is also in the original binary; the diff is the source of truth.
2. Edit the C in src/legoland/<file>, rebuild with `cmake --build build` (use `cmake --build build --clean-first` after ANY header change — no header dep tracking), re-run verify. Build is ~1-6 s, verify ~2 s: iterate a lot.
3. Typical fixes: statement/assignment order, types (signed/unsigned, char/short/int widths), temps vs inline exprs, loop shape (for/while/do), if/else vs ternary vs switch, struct field types, pointer vs index access, merging duplicated code, return types, parameter types/count, and correct prototypes of callees.
4. HARD LIMIT: at most ~12 build attempts per function, then move on. Stop entirely after your batch is done or ~60 tool calls total, whichever comes first.

## Rules (from CLAUDE.md — mandatory)
- Pure C only: no inline __asm, no __declspec(naked). goto only with meaningful named labels.
- No forward declarations in .c files — declarations go in the TU's .h (or legoland.h/globals.h for shared ones).
- Keep `// FUNCTION: LEGOLAND 0x...` markers and address order intact. Never touch crt.c / imports.c / external/ / toolchain/.
- Other agents are concurrently editing OTHER TUs. Prefer editing only your batch's .c files and their own .h. If you must change a shared header (legoland.h, globals.h, globals.c, struct defs), keep the change minimal and additive and report it.
- Never regress other functions. Before committing, run:
      tools/agent/regress.sh /tmp/baseline.txt
  It prints REGRESSED lines (any function whose % dropped vs the starting baseline — must be none) and NEWLY MATCHED lines. Fix or revert anything REGRESSED.
- Run `clang-format -i` on modified .c/.h files (if clang-format is installed), rebuild and re-check after formatting.

## Committing
Commit in your worktree (on its current branch) as you go — one commit per newly matched function is ideal, e.g. "Match FUN_00438f10 (73.79% -> 100%)"; improvements without a full match can go in one commit "Improve X, Y (a% -> b%)". Stage specific files only (`git add src/...`) — NEVER `git add -A` (toolchain symlink, reccmp yml, uv.lock must not be committed; run `git checkout uv.lock` if it changed). End commit messages with:

Co-Authored-By: Claude Sonnet <noreply@anthropic.com>

## Final report (keep it short)
- Worktree path and branch name, and `git log --oneline` of your commits.
- Table: addr, name, before %, after %.
- Any shared-header changes you made.
- One-line notes on stubborn functions (what the remaining diff is), if useful.

## IMPORTANT (round 2)
Your worktree may start from an OLDER commit. FIRST run: `git reset --hard claude/legoland-decomp-progress-9j3118` (that branch is the correct base and is the one /tmp/baseline.txt was made from), THEN run wt-setup.sh. Skip clang-format (installed version differs from the repo's and breaks tooling). Commit each improvement as you go.
