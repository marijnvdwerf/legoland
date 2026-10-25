# RunAppraisal (0x004453a0, `src/legoland/challenge.c`)

**Best: 86.72%** (2026-10-09; was 15.70%) (2026-10-08: rewrite 26.50%, then frame 86.37%; was 15.70%). Kind: C.

## What still differs

After the frame work (2026-10-08): the frame is 0x23d4 like the original, with the same slot order and packing. Two
things still differ there: the order of the ten equal-weight out-values, and n's slot (ours shares total's slot, the
original the row temp's). Per-section scores are struct 94-99.9%; S7 has 14 fewer instructions than the original, and
S2 loses on register choice (exact 62 vs regs 94).

How the frame was found (five probe agents with small MSVC6 tests):
- MSVC6 orders frame slots by a static count of memory references, heaviest at the bottom. Loops do not multiply the
  count. It is counted after CSE but BEFORE tail merging.
- The original repeats the restart code (`i = first; x = ...; AppraisalPageCount++; pagestart = first;` then a jump to
  the section head) at every page test. The compiler merges the copies into one block per section, but the copies
  still count, which makes pagestart and x the heaviest locals. With `pagestart = first` last, the trailing store
  merges into the loop test, as in the original.
- Section 10's text/bar RECT lost its frame home once its bottom is computed through rc.cur.bottom (16 bytes).
- The loop sections have their own pass counter (lpass). The advice count reuses total, and the queued-speech count
  reuses flags (both dead by then), matching the original's slot sharing.

## Tried (don't repeat)

| Date | Who | Change | Result |
|---|---|---|---|
| 2026-10-07 r1 | Haiku agent | Added a padding local to make up the 20 bytes | rejected: fake padding is not allowed |
| 2026-10-07 r2 | Haiku agent | Analysis only (the frame findings above) | no change |
| 2026-10-08 | Haiku 5.5 | Analysis of every `[esp+0x58..0x6c]` access in `orig`; noted that out58..out6c are already used | no change |
| 2026-10-08 | Haiku 5.5 | Moved `rep` above the six `out5x/out6x` declarations (declaration order probe) | frame still `0x23c0`, 15.39%; reverted |
| 2026-10-08 | permuter + Opus 5.5 | volatile passtotacc and tottotacc (both / each alone / plus piVar10): the totals stay in memory like the original, frame unchanged at 0x23c0 | 15.39 -> 15.60 (both); 15.35-15.42 others |
| 2026-10-08 | permuter + Opus 5.5 | each FUN_00444bf0/c70/cd0/d20/d70 output argument gets its own variable (11, the original's slots -9084..-9044, found with tools/frame.py): frame now 0x23d4 like the original | 15.60 -> 15.70 |
| 2026-10-08 | Opus 5.5 + 9 section-writer agents (workflow) | Full rewrite: a skeleton (struct AppraisalRow, the layout/cur rectangles pinned with `if (0)`, page-break macros), ten sections written against a per-section harness (scratchpad ra/h.py), each part's shared restart block written as an explicit label. Then an adversarial behaviour review (12 agents) found one real bug, fixed: section 9 used the nids pointer before section 8 set it | 15.70 -> 26.50 (structure 95.3%) |
| 2026-10-08 | Opus 5.5 | Frame probes on copies: rc declared first; separate subpass/rowp variables in the loop sections | frame order unchanged, score identical |
| 2026-10-08 | Opus 5.5 + 5 frame-probe agents (workflow) | Restart code inlined at every page test (pagestart last), loop-section pass counter, n -> total, nqueued -> flags, RECT bottom through rc.cur.bottom | 26.50 -> 86.37 |
| 2026-10-09 | Opus 5.5 + 7 section agents | Statement order: S0 title row (type/i++ before y), S1 end (x, bottom, totacc, passacc), S10 rectangle field orders (title top/bottom/left/right; text right after top; bar left/right/top). No gain found in S2-S9: the remaining gap is (1) the page-test register pattern (original reuses eax for the test and layout.right), (2) rc.cur.bottom kept in eax across rows (original re-reads memory; ~35 forms tried), (3) the equal-weight out-value order (ties ordered by last read, then a frame-dependent scramble; not reproducible from the out-value lines), (4) S7's surviving restart copy (depends on whole-function size) | 86.37 -> 86.72 |

## Ideas not tried yet

- Give the loop sections' strength-reduced offset its own temporary, as the original does. That should bring the
  row-offset temporary's weight below pagestart's and x's.
- Remove section 10's RECT home: build the two rectangles so that no field is spilled.
- Pack like the original: n and the render-object count into the row-offset temporary's slot, and so on (see
  "Packing" above).

## 2026-10-24 Opus 5.5 (analysis, no change)
- The comparison covers the whole function: reccmp's size is 34704 bytes, both builds have 292 calls with the same
  target histogram and nearly identical instruction-length histograms (8085 vs ~8110 instructions). An earlier estimate
  that only 28.7 KB was compared was wrong.
- verify: 816 removed / 830 added lines in 131 hunks; about half touch `[esp + N]` (slot choice), most of the rest are
  branch displacements and loads that move with them. The first hunk is the classic "constant through a register"
  pattern (`mov ecx, 0x50; mov [esp+0x1c], ecx` vs an immediate store): the original reuses those values later.
- Slot sweep (every pairwise merge of same-typed function-scope locals, 438 candidates, plus 17 block-to-function
  hoists): all exactly tie at the harness score, so the slot problem here is not about which named variables share.
