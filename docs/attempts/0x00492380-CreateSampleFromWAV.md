#CreateSampleFromWAV(0x00492380, `src / legoland / sound_sfx.c`)

**Best : 97.49 % **(start 97.49 %)

                      ##Tried
    | Date | Model | Change | Result |
    | -- -| -- -| -- -| -- -|
    | 2026 - 10 - 07 | claude - haiku - 5 - 5 | `if (SoundAvailable != 0) file = RES_OpenFile(...)` instead of early return | 97.49 % (no change) |
    | 2026 - 10 - 07 | claude - haiku - 5 - 5 | `if (file == NULL) return NULL;
` before the do / while | 97.49 % (no change) |
    | 2026 - 10 - 11 | Opus 5.5 | goto cleanup ladder(see above);
early `return 0` / `!SoundAvailable` / body nested in `if (SoundAvailable)` / `return sample` with sample = NULL / decl order / `0x61746164 == chunk` | 71.37 - 83.56(worse) |

    ##What still differs - Original keeps the `SoundAvailable == 0` early return as its own inline epilogue(`jne` over 10 bytes of pops and `ret`).Ours jumps to the shared bottom `return NULL` epilogue,
                                                                                                   because MSVC merges the two
                                                                                                       identical `return NULL` blocks.-
    Success path : ours duplicates `pop ebp;
pop ebx;
add esp;
ret` after the `RES_CloseFile` tail.- (2026 - 10 - 11, Opus 5.5) The failure paths do share one cleanup tail(Release / free data / free format / RES_CloseFile / return 0), but the current do / while (0) + break version already produces it.Rewritten as an explicit `goto fail_release / fail_data / fail_format / fail_close / fail` ladder(same behaviour) : harness 83.56(worse than 97.49) : the early return still merges into the bottom `return NULL` and every jump offset shifts.What really differs is only the early - return epilogue and the success - path epilogue.##Ideas not tried yet - Make the bottom `return NULL` differ from the early one so the compiler cannot merge them.
