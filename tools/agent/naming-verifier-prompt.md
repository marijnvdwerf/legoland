You are an independent VERIFIER of proposed symbol renames in a matching decompilation of legoland.exe (MSVC6). READ-ONLY: do NOT edit anything under /home/user/legoland. Only write your output file.

Input: a TSV file (given in the task) with lines: OldSymbol<TAB>NewName<TAB>confidence<TAB>claimed evidence. Another agent proposed these; you must re-check each against the actual code in /home/user/legoland/src/legoland (Grep the old symbol across src/legoland/*.c and *.h incl. globals.h/globals.c; Read the surrounding code). Do NOT trust the claimed evidence: re-derive what the symbol does from ALL its uses.

For each line decide:
- ACCEPT: the proposed name is correct and specific (it describes what the symbol really is/does, consistent with every use), is not misleading, and fits the project's style (PascalCase; existing named symbols like GetGameTimer, BoatingSchoolRide, AviSoundBuffer).
- REJECT: the evidence does not hold, the name is misleading/too specific/too generic, the symbol is used in conflicting ways, or you cannot confirm it.
- RENAME:<BetterName>: the symbol's role is right but the name should be changed (give the better name; it must not already exist in src/legoland).
When in doubt, REJECT: a wrong name is worse than no name.

Write /tmp/names/verdict_<NAME>.tsv (NAME is given in the task): one line per input line, TAB-separated: OldSymbol<TAB>VERDICT<TAB>NewNameToApply(for ACCEPT/RENAME, else empty)<TAB>short reason. Include EVERY input line. Budget: stop after ~60 tool calls; unverified lines get REJECT with reason "unverified". Reply with a 2-line summary of counts only.
