You are helping name symbols in a matching decompilation of legoland.exe (a theme-park game, MSVC6). Unnamed functions are `FUN_<addr>` and unnamed globals are `DAT_<addr>`. Your job is to PROPOSE names backed by evidence. READ-ONLY: do NOT edit anything under /home/user/legoland. Only write your output file.

Repo: /home/user/legoland (source in src/legoland/*.c, globals in globals.h/globals.c, declarations in per-TU .h). Read CLAUDE.md for context. Use Grep/Read freely.

Your candidate list is in the file given in the task (tab-separated: symbol, info). Work through it.

## Naming rules
- Match the existing style: look at already-named symbols (e.g. GetGameTimer, SetMoodDelta, BoatingSchoolRide, OverrideFrame, PerfCounterState, AviSoundBuffer). Functions: PascalCase verb phrases. Globals: PascalCase nouns, no Hungarian prefixes. Use existing naming of related symbols in the same TU for consistency (e.g. ride prefixes).
- Only propose a name when EVIDENCE proves it: a string literal it uses/prints, a Win32/DirectX/KLIB API it wraps, a unique access pattern (e.g. "only assigned X in Y and read in Z"), a struct it clearly operates on, a clear role as getter/setter/init/free, or being the single caller/callee of already-named functions. If you are not confident, SKIP it. Quality over quantity: a wrong name is worse than no name. Aim to name roughly half; fewer is fine.
- The new name must not already exist: grep `\bNewName\b` across src/legoland before proposing, and don't propose the same new name twice. It must be a valid C identifier and not a C/Windows keyword or macro/CRT symbol.
- For FUN_ symbols, look at the body AND at the callers/callees (Grep for the symbol across src/legoland). For DAT_ symbols, find ALL reads and writes (Grep across src/legoland, including globals.c/globals.h) and describe the usage.
- Do not rename anything that is a CRT/import stub (crt.c, imports.c).

## Output
Write /tmp/names/out_<NAME>.tsv (NAME is given in the task), one line per proposed rename, TAB-separated, no header:
OldSymbol<TAB>NewName<TAB>high|medium<TAB>one-line evidence with file:line references
Only write `high` or `medium` lines. Then reply with a 3-line summary (counts only). Budget: stop after ~45 tool calls total.

Also read /home/user/legoland/docs/naming-skipped.tsv (symbols already tried in an earlier round; none of your candidates are in it) and the list of already-applied names via `git log` is not needed. Candidates sorted by importance; a widely-used symbol with a provable name is the most valuable. Include a `called-from`/`refs` hint when judging.
