# LoadPos (0x0043f660)

## 2026-10-19: back to C

The rule that allowed a documented naked `__asm` transcription for compiler choices C cannot reach was withdrawn.
The function is the plain C that was the `#else` branch again: 42.19%. Matching has to come from C now.
Then the best C from before the asm version was put back (it is better than the plain `#else` C; its `volatile`
or extra local is commented in the source): 66.91%.

## 2026-10-19 agent notes (66.91% -> 100.00%)

What the diff was: the old C walked the frames with a `struct PosFrame *f` (plus a volatile copy) and the matrix
with a `float *walk` in hand-made do/while countdown loops. The original is plain indexed C: `for` loops over
i and j with `frames = pos->entries[i]`, every access as `frames[j].pos[k]` / `frames[j].mat[row][col]`, and the
nine matrix reads in two nested `for` loops. MSVC6 strength-reduces that itself into the original's pointer to
`frames[j].mat` (ebp) and the countdown loops (the row counter in the dead `path` slot).

| Change | Harness |
|---|---|
| plain f pointer, do/while or for loops (no volatile) | 65.10 |
| `f = &pos->entries[i][j]` per frame | 50.39 |
| `frames[j]` indexing, `walk` pointer over the matrix | 81.60 (100 ignoring registers: ebx/ebp swapped); 36 declaration orders: no change |
| the same with `walk++` in the call | 74.90 |
| `frames[j]` indexing and `&frames[j].mat[row][col]` (no walk) | 100 |
