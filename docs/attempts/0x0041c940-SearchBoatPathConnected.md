# SearchBoatPathConnected (0x0041c940)

Current full verify: **56.52%**, fresh harness: **59.32%**.

The original keeps the target coordinates in callee-saved registers and reads
the shared found pointer from its parameter slot. Ours keeps found in ebx,
spills target coordinates, and hoists an extra x+5 value. The final recursive
call is tail-eliminated differently as a result. Source was retained; no
random search or volatile experiment was run in this pass. The function is
now annotated. A future attempt should inspect parameter/local lifetimes,
not repeat generic register-order sweeps.
