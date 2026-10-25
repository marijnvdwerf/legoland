#include "progress.h"
#include "draw.h"
#include "freeplay.h"
#include "globals.h"
#include "icon.h"
#include "interface.h"
#include "legoland.h"
#include "options.h"
#include "profile_io.h"
#include "screens.h"
#include "sound_music.h"
#include "string.h"

#include "image_sprite.h"
#include "stream.h"
#include "text.h"
#include "timer.h"

// FUNCTION: LEGOLAND 0x0048b7e0
LEGO_EXPORT void InitProgressScreen(void) {
    struct IconNode *icon;
    unsigned int flags;
    int i;

    if ((int)lpConfig->level < 1) {
        lpConfig->level = 1;
    }
    if (ProgressLevelPicked == 0) {
        UpdateUnlockedLevelSelection();
        if (MapStats.level_end == 0) {
            if (ProgressScreenToggled == 0) {
                lpConfig->level = HighestUnlockedLevelIndex + 1;
            }
        }
        if (MapStats.level_end == 1 && (int)lpConfig->level <= 0xf) {
            CurrentProfile.flags[3 + lpConfig->level] = 1;
            UpDateCurrentProfile();
        }
    }
    if ((int)lpConfig->level <= 5) {
        InitTutorialScreen();
        return;
    }
    if ((int)lpConfig->level > 0xf) {
        FrontEndSubScreenActive = 1;
        SPRITE_TitleScreenBk = NULL;
        EditMode.unk4 = 2;
        FrontEndScreenMode.unk4 = 0xffffffff;
        FrontEndScreenMode.unk8 = 8;
        GamePad &= ~0x20;
        return;
    }
    ProgressShowsTutorials = 0;
    // STRING: LEGOLAND 0x004bef44
    SPRITE_TitleScreenBk = LoadSprite("Progress_ScreenBK.lls", 4);
    flags = 0x6002;
    if (ProgressLevelPicked == 0) {
        if (MapStats.level_end == 1 && lpConfig->level == 6) {
            MapStats.level_end = 2;
        }
        LoadProgressScreenSprites();
        // STRING: LEGOLAND 0x004bef2c
        icon = LoadSpriteIcon("Accept_on_Progress.lls", 4, 0x20e, 0x16f, 0x23);
        icon->string_id = 0x262;
        icon->string = GetString(0x262);
        icon->flags |= flags;
        icon->event_handler = (void *)ProgressPlayClicked;
        // STRING: LEGOLAND 0x004bef14
        icon = LoadSpriteIcon("GoBack_on_Progress.lls", 4, 0x208, 0xb, 0x23);
        icon->string_id = 0x26;
        icon->string = GetString(0x26);
        icon->flags |= flags;
        icon->event_handler = (void *)ProgressGoBackClicked;
        if (MapStats.level_end != 1) {
            // STRING: LEGOLAND 0x004beef8
            icon = LoadSpriteIcon("Tutorial_On_Progress.lls", 4, 0x174, 0x16d, 0x23);
            icon->string_id = 0x258;
            icon->string = GetString(0x258);
            icon->flags |= flags;
            icon->event_handler = (void *)ToggleProgressTutorialScreenClicked;
        }
    }
    ReferenceProgressLevelSprites();
    RemoveIconGroup(0x1c);
    if (MapStats.level_end == 1) {
        flags = 0x200a;

        for (i = 0; i < 10; i++) {
            if (i + 5 == (int)lpConfig->level - 1) {
                icon = InsertIcon(ProgressScreenTables.levels[i].x, ProgressScreenTables.levels[i].y, 0x1c, ProgressScreenTables.levels[i].sprite0);
                if (icon) {
                    icon->render_func = (void *)RenderFlashingSpriteIcon;
                    icon->string_id = ProgressScreenTables.levels[i].id;
                    icon->string = GetString(icon->string_id);
                    icon->field_18 = i + 5;
                    icon->event_handler = (void *)ProgressLevelClicked;
                    icon->flags |= flags;
                }
            } else if (i + 5 < (int)lpConfig->level - 1) {
                icon = InsertIcon(ProgressScreenTables.levels[i].x, ProgressScreenTables.levels[i].y, 0x1c, ProgressScreenTables.levels[i].sprite1);
                if (icon) {
                    icon->string_id = ProgressScreenTables.levels[i].id;
                    icon->string = GetString(icon->string_id);
                    icon->flags |= 0x2000;
                }
            }
        }
    } else {
        for (i = 0; i < 10; i++) {
            if (i + 5 == (int)lpConfig->level - 1) {
                icon = InsertIcon(ProgressScreenTables.levels[i].x, ProgressScreenTables.levels[i].y, 0x1c, ProgressScreenTables.levels[i].sprite0);
                if (icon) {
                    icon->render_func = (void *)RenderFlashingSpriteIcon;
                    icon->string_id = ProgressScreenTables.levels[i].id;
                    icon->string = GetString(icon->string_id);
                    icon->field_18 = i + 5;
                    icon->event_handler = (void *)ProgressLevelClicked;
                    icon->flags |= 0x600a;
                }
            } else if (CurrentProfile.flags[4 + i + 5] == 1) {
                icon = InsertIcon(ProgressScreenTables.levels[i].x, ProgressScreenTables.levels[i].y, 0x1c, ProgressScreenTables.levels[i].sprite1);
                if (icon) {
                    icon->string_id = ProgressScreenTables.levels[i].id;
                    icon->string = GetString(icon->string_id);
                    icon->field_18 = i + 5;
                    icon->event_handler = (void *)ProgressLevelClicked;
                    icon->flags |= flags;
                }
            }
        }
    }
    OkKeyHandler = ProgressPlayClicked;
    CancelKeyHandler = ProgressGoBackClicked;
}

// FUNCTION: LEGOLAND 0x0048bb60
unsigned char ProgressLevelClicked(unsigned char *arg0, unsigned int arg1, unsigned int arg2, unsigned int arg3) {
    ProgressLevelPicked = 1;
    if ((arg1 & 2) != 0) {
        if ((int)(GetTicks() - ProgressLastClickTicks) < 500 && arg0[0x18] == ProgressScreenTables.last_clicked) {
            if (ProgressShowsTutorials != 0) {
                return TutorialPlayClicked(arg0, arg1, arg2, arg3);
            }
            return ProgressPlayClicked(arg0, arg1, arg2, arg3);
        }
        ProgressLastClickTicks = GetTicks();
        ProgressScreenTables.last_clicked = arg0[0x18];
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        lpConfig->level = arg0[0x18] + 1;
        FrontEndScreenMode.unk4 = 0xffffffff;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048bc20
unsigned char ProgressPlayClicked(unsigned char *arg0, unsigned int arg1, unsigned int arg2, unsigned int arg3) {
    if ((arg1 & 2) != 0) {
        do {
            CancelKeyHandler = 0;
            OkKeyHandler = 0;
            SpeechCloseFile();
            SpeechDelayFrames = 4;
            PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
            KillProgressScreenSprites();
            if ((int)lpConfig->level <= 0xf) {
                LoadWatchSprite(0xfa, 0x181);
                FrontEndSubScreenActive = 0;
                InitGameInterface(1);
                EditMode.unk4 = 3;
                SetDefaultMouseHandlers();
                StartLevelGame();
                UnloadWatchSprite();
                ProgressLevelPicked = 0;
                ProgressScreenToggled = 0;
            } else {
                ProgressLevelPicked = 0;
                GamePad &= ~0x20;
                ProgressScreenToggled = 0;
                EditMode.unk4 = 2;
                FrontEndScreenMode.unk4 = 0xffffffff;
                FrontEndScreenMode.unk8 = 8;
                FrontEndSubScreenActive = 1;
            }
        } while (0);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048bd00
void LoadTutorialSprites(void) {
    struct ProgressEntry *e;

    e = &ProgressScreenTables.tutorials[0];
    do {
        e->sprite0 = LoadSprite(e->name0, 4);
        e->sprite1 = LoadSprite(e->name1, 4);
        e++;
    } while ((INT_PTR)&e->sprite0 < (INT_PTR)&TutorialEndSprite0);
}
// FUNCTION: LEGOLAND 0x0048bd40
void ReferenceTutorialSprites(void) {
    int *esi;

    esi = (int *)&ProgressScreenTables.tutorials[0].sprite1;
    do {
        ReferenceSprite((struct Sprite *)esi[-1]);
        ReferenceSprite((struct Sprite *)esi[0]);
        esi += 7;
    } while ((INT_PTR)esi < (INT_PTR)&TutorialEndSprite1);
}
// FUNCTION: LEGOLAND 0x0048bd70
void KillTutorialScreenSprites(void) {
    struct FreePlaySpriteSlot *slot;

    RemoveIconGroup(0x1c);
    RemoveIconGroup(0x23);
    slot = (struct FreePlaySpriteSlot *)&ProgressScreenTables.tutorials[0].sprite0;
    while ((INT_PTR)slot < (INT_PTR)&TutorialEndSprite0) {
        while (KillSprite(slot->sprite0) == 0) {
        }
        while (KillSprite(slot->sprite1) == 0) {
        }
        slot->sprite1 = NULL;
        slot->sprite0 = NULL;
        slot++;
    }
}

// FUNCTION: LEGOLAND 0x0048bde0
void InitTutorialScreen(void) {
    struct IconNode *icon;
    struct IconNode **mapping;
    struct ProgressEntry *entry;
    int i;

    // STRING: LEGOLAND 0x004bef88
    SPRITE_TitleScreenBk = LoadSprite("TutorialBK.lls", 0);
    ProgressShowsTutorials = 1;
    if (ProgressLevelPicked == 0) {
        LoadTutorialSprites();
        // STRING: LEGOLAND 0x004bef70
        icon = LoadSpriteIcon("Accept_on_Report.lls", 4, 0x20a, 0x16c, 0x23);
        icon->string_id = 0x262;
        icon->string = GetString(0x262);
        icon->flags |= 0x6002;
        icon->event_handler = (void *)TutorialPlayClicked;
        OkKeyHandler = TutorialPlayClicked;
        // STRING: LEGOLAND 0x004bef5c
        icon = LoadSpriteIcon("GoBack_on_Tut.lls", 4, 0x20a, 0xf5, 0x23);
        icon->string_id = 0x26;
        icon->string = GetString(0x26);
        if (CurrentProfile.flags[9] == 1) {
            icon->string_id = 0x26c;
            icon->string = GetString(0x26c);
            icon->event_handler = (void *)ToggleProgressTutorialScreenClicked;
        } else {
            icon->string_id = 0x26;
            icon->string = GetString(0x26);
            icon->event_handler = (void *)ProgressGoBackClicked;
        }
        icon->flags |= 0x6002;
        CancelKeyHandler = icon->event_handler;
    }
    ReferenceTutorialSprites();
    RemoveIconGroup(0x1c);
    i = 0;
    mapping = TutorialIcons;
    entry = &ProgressScreenTables.tutorials[0];
    do {
        if (i == lpConfig->level - 1) {
            icon = InsertIcon(entry->x, entry->y, 0x1c, entry->sprite0);
        } else if (CurrentProfile.flags[4 + i] == 1) {
            icon = InsertIcon(entry->x, entry->y, 0x1c, entry->sprite1);
        } else {
            icon = 0;
        }
        *mapping = icon;
        if (icon) {
            icon->string = GetString(0x276);
            icon->string_id = entry->id;
            icon->field_18 = i;
            icon->event_handler = (void *)ProgressLevelClicked;
            icon->flags |= 0x6002;
        }
        i++;
        mapping++;
        entry++;
    } while ((INT_PTR)mapping < (INT_PTR)&HighestUnlockedLevelIndex);
}

// FUNCTION: LEGOLAND 0x0048bf90
unsigned char TutorialPlayClicked(unsigned char *arg0, unsigned int arg1, unsigned int arg2, unsigned int arg3) {
    if ((arg1 & 2) != 0) {
        CancelKeyHandler = 0;
        OkKeyHandler = 0;
        SpeechCloseFile();
        SpeechDelayFrames = 4;
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        KillTutorialScreenSprites();
        LoadWatchSprite(0x186, 0x18b);
        FrontEndSubScreenActive = 0;
        InitGameInterface(1);
        EditMode.unk4 = 3;
        SetDefaultMouseHandlers();
        StartLevelGame();
        UnloadWatchSprite();
        ProgressLevelPicked = 0;
        ProgressScreenToggled = 0;
    }
    return 1;
}

/* Clears the key handlers and progress selection state, releases the current progress or tutorial graphics, and
 * invokes the return-to-title action. */
// FUNCTION: LEGOLAND 0x0048c020
unsigned char ProgressGoBackClicked(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4) {
    unsigned int temp;

    if ((a2 & 2) != 0) {
        CancelKeyHandler = 0;
        OkKeyHandler = 0;
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        temp = ProgressShowsTutorials;
        ProgressLevelPicked = 0;
        ProgressScreenToggled = 0;
        if (temp != 0) {
            KillTutorialScreenSprites();
        } else {
            KillProgressScreenSprites();
        }
        return ReturnToTitleClicked(a1, a2, a3, a4);
    }
    return 1;
}

/* Clears progress selection and invalidates the current screen, then switches from tutorial graphics to level one
 * or from progress graphics to tutorial level six. */
// FUNCTION: LEGOLAND 0x0048c090
unsigned char ToggleProgressTutorialScreenClicked(void *param1, unsigned char param2) {
    if (param2 & 2) {
        ProgressLevelPicked = 0;
        ProgressScreenToggled = 1;
        FrontEndScreenMode.unk4 = 0xffffffff;
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        if (ProgressShowsTutorials != 0) {
            KillTutorialScreenSprites();
            lpConfig->level = 6;
        } else {
            KillProgressScreenSprites();
            lpConfig->level = 1;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048c100
void DrawTutorialList(void) {
    RECT rc;
    struct IconNode **mapping;
    int *entry;
    int i;
    char *text;

    if (ProgressShowsTutorials != 0) {
        rc.top = 0x45;
        rc.bottom = 0x6c;
        rc.left = 10;
        rc.right = 0x1d6;
        NewPrintCent(GetString(0x28a), 3, rc, 0);
        i = 0;
        mapping = TutorialIcons;
        entry = &ProgressScreenTables.tutorials[0].x;
        do {
            text = GetString(entry[-1]);
            rc.left = entry[0] + 0x32;
            rc.top = entry[1] + 6;
            rc.right = rc.left + 0x190;
            rc.bottom = rc.top + 0x16;
            if (i == (int)lpConfig->level - 1) {
                DrawTextOnRenderSurface(text, 2, rc, 0);
            } else if (CurrentProfile.flags[4 + i] == 1) {
                DrawTextOnRenderSurface(text, 2, rc, 0x323232);
            } else {
                DrawTextOnRenderSurface(text, 2, rc, 0xa0a0a0);
            }
            if (CurrentProfile.flags[4 + i] == 1 && MousePos.x >= rc.left && MousePos.x < rc.right && MousePos.y >= rc.top && MousePos.y < rc.bottom) {
                Hover.type = 2;
                Hover.ptr = (struct Bloke *)*mapping;
            }
            i++;
            entry += 7;
            mapping++;
        } while ((INT_PTR)entry < (INT_PTR)&ProgressScreenTables.tail[12]);
    }
}
