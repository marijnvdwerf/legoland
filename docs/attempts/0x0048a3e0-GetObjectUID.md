# GetObjectUID (0x0048a3e0)

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 77.35%. Matching has to come from C now.

## 2026-10-19 Opus 5.5 (plain C, no change: 77.35%)

Scalar x/y instead of the `tile` / `probe` Points (38.52 and 30.61), int `px`/`py` probes (34.30), field-wise probe
copies (34.30-55.01), `tile.x = pos->x >> 8` field-wise (77.95 harness, 97.44 ignoring structure, but no better in
verify). The structure copies `probe = tile` stay.

## 2026-10-24 Opus 5.5 (no change: 77.35 verify / 77.95 harness)
Our code is the original plus 8 instructions: an 8-byte frame (`sub esp, 8` and five `add esp, 8`) and two dead stores
into `probe` (`probe = tile` keeps a memory copy of the field that is modified next: probe.y in the below-probe, probe.x in
the left-probe). Register allocation, branches and the shared tails already match the original.
Every way of removing the memory copy also changes the register allocation (pos/tile.x/tile.y/ride land in
eax/ecx/ebp/ebx instead of ecx/eax/edi/ebp): field-wise probes, `n` scalars, an `int probe[2]`, MapTileAtPoint(&probe) /
by-value Point / (x, y) inline helpers (worker.h GetTileAtPoint style), block-scoped probes, reusing `origin`, scalar tile:
34-59. 625 per-probe combinations of those forms: best 77.95. 4 min permuter: nothing.
Idea not tried: find which extra uses of tile.x/tile.y the original has (the two dead stores add exactly one use of each,
which is what keeps the allocation right).
