# ParseScriptResFile (0x00478280)

Baseline from current main: full verify / harness **86.03%**.

## Signed callback result discrepancy

The current result variable is unsigned, which removes the negative-result
checks. The original has signed `jl` branches after command/default dispatch
and before the final check handler. ScriptCommandFn already returns int.
Changing the local to int restores those checks, but full verify falls to
69.44% (harness 72.68), with a four-byte-larger frame and different cursor
spills and epilogue sharing. The verifier rejected it; no body change kept.
Do not treat the higher percentage as proof the current unsigned type is
semantically correct. This is a known unresolved mismatch.

## Forms checked (harness)

- Signed status, with the in-loop negative return changed to break: 72.68.
- Initialize status at its declaration: 72.68.
- One named shared error-exit label: 73.74.
- Initialize the command cursor in the scan loop / per token line: 48.39.
- Index the command table without a cursor: 44.50.
- Use the command parameter as the mutable cursor with a saved table base: 68.27.

The original keeps the scan cursor in ebp and reloads tokens[0] for each
comparison. Ours saves the cursor in a local stack slot and holds tokens[0]
in ebp. Avoid blind declaration permutations until the lifetime discrepancy
has been understood. The implicit strchr declaration warning is pre-existing.
