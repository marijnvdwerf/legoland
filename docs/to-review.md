# To review

Matches that work but use tricks worth a second look before we call them final.
Each entry: where, what the trick is, why it is questionable, what would settle it.


> **2026-10-11:** the user accepted `volatile` (when it is the only way, with a comment) and multi-label
> `goto` cleanup ladders; see CLAUDE.md. Items 1 and 4 below are no longer rule problems: the volatile
> stays, but the `ptmpN` temporaries in item 4 still need meaningful names.

## 1. StartOverlayAnims — volatile cast to force a reload (commit d6074af)

- **Where:** `src/legoland/map_object.c`, two sites:
  `*(volatile unsigned int *)&node->field_10 & 0xff`
- **Trick:** the original reloads `node->field_10` as a full dword in each branch. `volatile` stops the compiler
  from reusing the earlier test load.
- **Why questionable:** it's the same kind of hack that was removed from AdvanceFlumeMover (e8c4858). It's
  not real source; nobody writes volatile there. It also isn't a clean 100%: the OverlayILF/field load order is
  still swapped (the commit says "effective match").
- **To settle:** find a plain-C form that reloads naturally:
  - read the field through a different expression in each branch;
  - a struct copy;
  - a call between test and use;
  - the field typed as a bit-field/char in the struct.
  The AdvanceFlumeMover lesson was to write the field read in each test, not cache it.
  If nothing works, decide whether volatile is acceptable and write the rule into CLAUDE.md.

## 2. JoustRemoveObject, LoadZoomer — unnamed bit-field in the sample source (commit 7370049)

- **Update 2026-10-09:** now one shared type, `struct TileSampleSource` in `sound_music.h`, also used by LoadZoomer
  (97.9% -> 100% with the same initialiser). It's no longer an ad-hoc anonymous struct.

- **Trick:** the local `source` struct now has `unsigned int : 32;` for its second member, so the initialiser
  `{2, x, y}` skips it and the slot stays unwritten, like the original.
- **Why questionable:** it's close to the "no padding or dummy variables" rule. On the other hand it models a
  real field (`SampleSource.bloke`, unused for type 2) and is commented.
- **To settle:** check whether the shared `struct SampleSource` type can be used instead of the anonymous
  struct, with field-by-field assignment that leaves `bloke` unset. (Tried 2026-10-09: SampleSource with field
  stores in all three orders gives 92.5-95%; the stores never sink past the pushes. Only the initialiser does.) If that matches, prefer it. Otherwise accept
  the bit-field and say in CLAUDE.md that it's allowed.

## 3. Global definitions moved/initialised for layout (commits 9e94251, 693de2d)

- **Trick:** globals were reordered in `globals.c` and given `= {0}` / had `= 0` dropped, so MSVC6 puts them in
  .data vs .bss in the original order. This made FUN_004294b0 match and FUN_004284d0 go 85.5% → 89.8%.
  - 9e94251: `DAT_004b6150[36]`, `DAT_006139c8[0xe90]` moved next to `DAT_004b61e0` / `DAT_00614858`.
  - 693de2d: `DAT_00611648`, `DAT_00611688[11]`.
- **Why to look:** this is probably fine, since layout follows the binary's addresses. But initialisers that
  exist only to pick a section can affect other functions referencing the same globals.
- **To settle:**
  - confirm the new order matches the original addresses;
  - run a full verify and diff per-function results against the run before each commit, to check nothing else
    dropped. (Done at the time for both commits: the only per-function change was the target function.)

## 4. Permuter temporaries left in the source (`ptmpN`, often `volatile`)

- **Where:** `grep -n 'ptmp[0-9]' src/legoland/*.c`. As of 2026-10-10 that finds castle.c (3 functions),
  certificate.c, fort.c, gamemain.c and render.c (GenerateNewImageFromZBuffer `volatile int ptmp19`).
- **Trick:** the permuter adds a temporary (`ptmp = expr; use(ptmp);`), sometimes `volatile` or `register`, to
  force a separate load or a register choice. A few candidates were integrated with these still in.
- **Why questionable:** they are dummy variables (the "no padding or dummy variables" rule) and `volatile` is a
  hack. Nobody wrote them in the original.
- **To settle:** for each one:
  1. remove the temporary;
  2. find the plain-C form that gives the same load (struct copy, reading the field in each use, statement
     order);
  3. failing that, accept the lower score.
  In the 2026-10-10 permuter batch every candidate was cut down to its clean statement moves before
  integrating. Most of the hacky gains turned out to come only from `volatile`.

## Also open (not tricks, just unfinished)

- BuildQueueTable (ride_queue.c, 93.5%): only the final `start.x += dx` register choice differs.
- PickQueueTurn 0x401f30 (71.4%): the original frame has 8 unexplained bytes. A dummy variable would be needed,
  which is not allowed.
