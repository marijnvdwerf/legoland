#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "legoland.h"

#include "bricks.h"
#include "clipping.h"
#include "draw.h"
#include "freeplay.h"
#include "game_util.h"
#include "icon.h"
#include "interface.h"
#include "llidb.h"
#include "map_object.h"
#include "obj_instance.h"
#include "objclass.h"
#include "objectives.h"
#include "options.h"
#include "saveload.h"
#include "screens.h"
#include "sound_music.h"
#include "sound_sfx.h"
#include "stream.h"
#include "string.h"
#include "title.h"

struct PanelNode {
    struct PanelNode *next;
    void *buffer1;
    char **name;
    char **after;
    void *buffer2;
    struct Sprite *a;
    int key;
};

// LLIDB element fields read by InitFreePlayScreen (real Element is opaque).

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x0048a8a0
LEGO_EXPORT void InitFreePlayScreen(void) {
    struct IconNode *icon;
    struct IconNode *bar;
    struct Element *elem;
    struct Element *path_idx;
    unsigned int count;
    unsigned int i;

    FrontEndSubScreenActive = 0;
    Empty_0048b6c0();
    FreePlayCostTotal = 0;
    FreePlaySelectedCount = 0;

    // STRING: LEGOLAND 0x004beb34
    SPRITE_TitleScreenBk = LoadSprite("FreePlayScreenBK.lls", 0);
    // STRING: LEGOLAND 0x004beb24
    FreePlayDown1Sprite = LoadSprite("FP_Down1.lls", 4);
    // STRING: LEGOLAND 0x004beb14
    FreePlayDown2Sprite = LoadSprite("FP_Down2.lls", 4);
    // STRING: LEGOLAND 0x004beb04
    FreePlayDown3Sprite = LoadSprite("FP_Down3.lls", 4);
    // STRING: LEGOLAND 0x004beaf4
    FreePlayDown4Sprite = LoadSprite("FP_Down4.lls", 4);
    // STRING: LEGOLAND 0x004beae8
    FreePlayUp1Sprite = LoadSprite("FP_Up1.lls", 4);
    // STRING: LEGOLAND 0x004beadc
    FreePlayUp2Sprite = LoadSprite("FP_Up2.lls", 4);
    // STRING: LEGOLAND 0x004bead0
    FreePlayUp3Sprite = LoadSprite("FP_Up3.lls", 4);
    // STRING: LEGOLAND 0x004beac4
    FreePlayUp4Sprite = LoadSprite("FP_Up4.lls", 4);
    // STRING: LEGOLAND 0x004beab0
    FreePlayTickSprite = LoadSprite("FreePlay_Tick.lls", 4);
    // STRING: LEGOLAND 0x004beaa0
    FreePlayCoverSprite = LoadSprite("FP_Cover.lls", 4);

    // STRING: LEGOLAND 0x004bea88
    icon = LoadSpriteIcon("GoBack_on_FreePlay.lls", 4, 0xd, 0x137, 7);
    icon->string_id = 0x26;
    icon->string = GetString(0x26);
    icon->flags |= 0x6002;
    icon->event_handler = (void *)ReturnToTitleClicked;
    CancelKeyHandler = ReturnToTitleClicked;

    // STRING: LEGOLAND 0x004bea70
    icon = LoadSpriteIcon("Accept_On_FreePlay.lls", 4, 0x1e4, 0x14b, 7);
    icon->string_id = 0x32;
    icon->string = GetString(0x32);
    icon->flags |= 0x6002;
    icon->event_handler = (void *)FreePlayStartIconHandler;
    OkKeyHandler = FreePlayStartIconHandler;
    icon->flags |= 0x400;
    FreePlayStartIcon = icon;

    // STRING: LEGOLAND 0x004bea60
    bar = LoadSpriteIcon("Bar_Rides.lls", 4, 0xd1, 0x156, 7);
    bar->render_func = (void *)RenderFreePlayBar;
    bar->flags |= 0x400a;

    count = LLIDB_GetCount();
    LLIDB_FindElement(PathControlName, &path_idx, 0);

    for (i = 0; (int)i < (int)count; i++) {
        LLIDB_GetElement(i, (struct Element **)&elem);
        if ((elem->flags & 0x10) == 0) continue;
        if ((elem->flags & 0x1) == 0) continue;
        if (path_idx == elem) continue;
        if (elem->field_10 == 0) continue;
        elem->field_10--;
        if (elem->field_10 != 0) continue;
        if ((elem->flags & 0x1) == 0) continue;
        elem->flags &= 0xfffcfff0;
        if ((elem->flags & 0xfff0) == 0x10 || (elem->flags & 0xfff0) == 0x1010) {
            LLIDB_UnLoadODF(elem);
        }
    }

    LLIDB_ClearOnLevel();
    InitFreePlayLists();

    FreePlayObjectList(0xc8, 0x2a, 0x41, 0xec, 0xc8);
    FreePlayObjectList(0x1f4, 0xbb, 0x41, 0xec, 0x1f4);
    FreePlayObjectList(0x190, 0x14c, 0x41, 0xec, 0x190);
    FreePlayObjectList(0x12c, 0x1dd, 0x41, 0xec, 0x12c);

    RestoreFreePlaySelection();
}

// FUNCTION: LEGOLAND 0x0048ab60
void GiveSelectedFreePlayObjects(void) {
    struct IconNode *node;

    node = (struct IconNode *)IconListHead;
    if (node == 0) {
        return;
    }
    do {
        DrawWatchSprite();
        if (node->field_18 == 1) {
            if (EnableObjectClass(node->field_1c) != 0) {
                GiveRewardObject(ElemID(node->field_1c), 0, 1);
            }
        }
        node = node->next;
    } while (node != 0);
}

// FUNCTION: LEGOLAND 0x0048abb0
void StartFreePlayGame(void) {
    char buf[0x34];

    QueryClass = 0;
    // STRING: LEGOLAND 0x004beb4c
    sprintf(buf, "FreePlayTest.txt");
    PauseGameTimer();
    ResetGameTimer();
    MarkGameTimer();
    CastlePlacedFlag = 0;
    ResetMapAI();
    LevelScriptLoadResult = LoadLevelScript(buf);
    SetBricksLimited(0);
    GiveSelectedFreePlayObjects();
    AllocBlokeCounters(lpConfig->max_blokes);
    ResetEditState();
    MapStats.level_end = 0;
    ClearObjInstanceTable();
    UpdateMenu();
    UnloadWatchSprite();
    SetIntervalScreenIsBriefing(1);
    ShowIntervalScreen(BriefingFileName, 0);
    SetMapLoaded(1);
    ResumeGameTimer();
    UpdateSoundVols();
}

// FUNCTION: LEGOLAND 0x0048ac60
unsigned char FreePlayStartIconHandler(unsigned int param_1, unsigned int param_2) {
    if (FreePlayStartIcon != 0 && (FreePlayStartIcon->flags & 0x400) != 0) {
        return 0;
    }

    if (FrontEndInputEnabled != 0 && (param_2 & 0x2) != 0) {
        CurrentProfile.field_45 = 2;
        LoadWatchSprite(0x127, 0x170);
        SpeechCloseFile();
        SpeechDelayFrames = 0x4;
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        StartFreePlayGame();
        CleanUpFreePlay();
        KillTitleScreenSprites();
        RemoveIconGroup(0x7);
        EditMode.unk4 = 0x3;
        InitGameInterface(0x1);
        SetDefaultMouseHandlers();
        CurrentProfile.field_45 = 2;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048ad00
LEGO_EXPORT void InitFreePlayLists(void) {
    int t2 = 0;
    struct Element *t1 = 0;
    char **t3;
    char *t4;
    struct Element *build = 0;
    struct Element *common = 0;
    struct Element *legoland = 0;
    struct Element *adventurers = 0;
    struct Element *castle = 0;
    struct Element *western = 0;
    unsigned char i;
    unsigned char *p;
    int idx;
    struct Sprite *sprite;
    struct ClipQueryResult *entry;

    if (LLIDB_FindElement(BuildMenuName, &build, 0) != 0) {
        exit(1);
    }
    if (LLIDB_FindElement("COMMON THEME", &common, 0) != 0) {
        exit(1);
    }
    if (LLIDB_FindElement("LEGOLAND THEME", &legoland, 0) != 0) {
        exit(1);
    }
    if (LLIDB_FindElement("ADVENTURERS THEME", &adventurers, 0) != 0) {
        exit(1);
    }
    if (LLIDB_FindElement("CASTLE THEME", &castle, 0) != 0) {
        exit(1);
    }
    if (LLIDB_FindElement("WESTERN THEME", &western, 0) != 0) {
        exit(1);
    }
    for (i = 0, p = CurrentProfile.field_46; i < 200; i++, p++) {
        if (*p == 0) {
            continue;
        }
        idx = 0;
        for (entry = FreePlayObjectTable; (INT_PTR)entry < (INT_PTR)&FreePlayObjectTable[0x86]; entry++, idx++) {
            if (entry->id == i && LLIDB_FindElement(entry->name, &t1, 0) == 0) {
                break;
            }
        }
        if (idx == 0x86) {
            continue;
        }
        sprite = LoadObjDescFile(t1, &t4, &t2, &t3);
        if (sprite == 0) {
            continue;
        }
        if (LoadedObjDescElement->name == legoland->name || LoadedObjDescElement->name == common->name) {
            Add2FreePlayPanelLists(sprite, &t1->name, t4, t2, t3, 200);
        } else if (LoadedObjDescElement->name == adventurers->name) {
            Add2FreePlayPanelLists(sprite, &t1->name, t4, t2, t3, 300);
        } else if (LoadedObjDescElement->name == castle->name) {
            Add2FreePlayPanelLists(sprite, &t1->name, t4, t2, t3, 400);
        } else if (LoadedObjDescElement->name == western->name) {
            Add2FreePlayPanelLists(sprite, &t1->name, t4, t2, t3, 500);
        } else {
            KillSprite(sprite);
        }
        free(t4);
    }
}

// FUNCTION: LEGOLAND 0x0048aef0
unsigned int CanAffordFreePlayObject(const char *arg1, struct Element *arg2) {
    unsigned int result;
    struct Element *elem;

    result = GetFreePlayObjectCost(arg1, 0);
    if ((int)(FreePlayCostTotal + result) <= 0x4e20) {
        if (arg2) {
            elem = ElemID(BuildMenuName);
            if (arg2 != elem) {
                if ((arg2->flags & 0x4) == 0) {
                    return 0;
                }
            }
        }
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0048af40
void SelectFreePlayObject(const char *param) {
    struct ClipQueryResult *node;

    node = 0;
    FreePlayCostTotal += GetFreePlayObjectCost(param, &node);
    if (node != 0) {
        node->field_c = 1;
    }

    if (FreePlaySelectedCount++ == 0) {
        FreePlayStartIcon->flags &= ~0x400u;
    }
}

// FUNCTION: LEGOLAND 0x0048afa0
void DeselectFreePlayObject(const char *param) {
    struct ClipQueryResult *node;

    node = 0;
    FreePlayCostTotal -= GetFreePlayObjectCost(param, &node);
    if (node != 0) {
        node->field_c = 0;
    }

    if (--FreePlaySelectedCount == 0) {
        FreePlayStartIcon->flags |= 0x400;
    }
}

// FUNCTION: LEGOLAND 0x0048b000
unsigned char FreePlayObjectIconHandler(struct IconNode *icon, unsigned int param_2) {
    struct IconNode *node;
    struct Element *elem;

    if ((param_2 & 0x2) != 0) {
        do {
            if (icon->field_18 == 0) {
                if (CanAffordFreePlayObject((const char *)icon->field_1c, (struct Element *)icon->field_20p) != 0) {
                    if (FreePlayRestoringSelection == 0) {
                        PlayInstanceOfSample(GameFX[FX_CLICK1].sample, 0, 1, 0);
                    }
                    SelectFreePlayObject((const char *)icon->field_1c);
                    icon->field_18 = 1;
                    ElemID((const char *)icon->field_1c)->flags |= 0x4;
                } else {
                    PlayInstanceOfSample(GameFX[FX_RASP].sample, 0, 1, 0);
                }
            } else {
                elem = ElemID((const char *)icon->field_1c);
                for (node = IconListHead; node != 0; node = node->next) {
                    if (node->field_20p != 0 && node->field_18 == 1 && (struct Element *)node->field_20p == elem) {
                        DeselectFreePlayObject((const char *)node->field_1c);
                        node->field_18 = 0;
                    }
                }
                PlayInstanceOfSample(GameFX[FX_BUTTON14].sample, 0, 1, 0);
                DeselectFreePlayObject((const char *)icon->field_1c);
                icon->field_18 = 0;
                ElemID((const char *)icon->field_1c)->flags &= ~0x4u;
            }
        } while (0);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048b110
LEGO_EXPORT void Add2FreePlayPanelLists(struct Sprite *a, char **name, char *name2, int key, char **after, int type) {
    struct PanelNode *cur = 0;
    struct PanelNode *prev = 0;
    struct PanelNode **head = 0;
    struct PanelNode *node = (struct PanelNode *)malloc(sizeof(struct PanelNode));

    if (type == 200) {
        cur = FreePlayPanelList1;
        head = &FreePlayPanelList1;
    } else if (type == 300) {
        cur = FreePlayPanelList4;
        head = &FreePlayPanelList4;
    } else if (type == 400) {
        cur = FreePlayPanelList3;
        head = &FreePlayPanelList3;
    } else if (type == 500) {
        cur = FreePlayPanelList2;
        head = &FreePlayPanelList2;
    }
    node->buffer2 = malloc(strlen(name2) + 1);
    strcpy(node->buffer2, name2);
    node->buffer1 = malloc(strlen(*name) + 1);
    strcpy(node->buffer1, *name);
    node->name = name;
    node->a = a;
    node->after = after;
    node->key = key;
    node->next = 0;
    if (after != 0) {
        while (cur != 0) {
            if (cur->name == after) {
                break;
            }
            cur = cur->next;
        }
        if (cur == 0) {
            free(node->buffer1);
            free(node->buffer2);
            free(node);
            return;
        }
        prev = cur;
        cur = cur->next;
    } else {
        while (cur != 0) {
            if (cur->key > key) {
                break;
            }
            prev = cur;
            cur = cur->next;
        }
    }
    if (prev != 0) {
        prev->next = node;
        node->next = cur;
    } else {
        *head = node;
        node->next = cur;
    }
}

// Builds one free-play object list panel (icon group `a`, at x `b`): picks the page's up/down
// arrow sprites and object list from `e` (200, 500, 400 or 300), adds the bar icons, one icon per
// object (0x42 pixels apart) and the full-screen icon, then turns icons a+4 / a+3 into the down /
// up scroll arrows. With no objects it only shows the cover sprite. Returns 1 if the panel was built.
// FUNCTION: LEGOLAND 0x0048b2a0
LEGO_EXPORT unsigned int FreePlayObjectList(int a, int b, int c, int d, int e) {
    struct InterfacePanel *group;
    struct IconNode *icon;
    struct PanelNode *list;
    struct Sprite *sprite2;
    struct Sprite *sprite;
    unsigned int y;

    group = (struct InterfacePanel *)malloc(sizeof(struct InterfacePanel));
    if (group == NULL) {
        return 0;
    }
    if (e == 0xc8) {
        sprite = FreePlayDown1Sprite;
        sprite2 = FreePlayUp1Sprite;
        list = FreePlayPanelList1;
    } else if (e == 0x1f4) {
        sprite = FreePlayDown2Sprite;
        sprite2 = FreePlayUp2Sprite;
        list = FreePlayPanelList2;
    } else if (e == 0x190) {
        sprite = FreePlayDown3Sprite;
        sprite2 = FreePlayUp3Sprite;
        list = FreePlayPanelList3;
    } else if (e == 0x12c) {
        sprite = FreePlayDown4Sprite;
        sprite2 = FreePlayUp4Sprite;
        list = FreePlayPanelList4;
    }
    if (list == NULL) {
        InsertIcon(b - 3, 0x15, 7, FreePlayCoverSprite);
        return 0;
    }
    icon = AddGBarIcons(group, b, c, 1, d, a);
    group->icon = icon;
    b = icon->x;
    group->clip_left = b;
    group->content_left = b;
    y = icon->y;
    group->clip_top = y;
    group->content_top = y;
    group->clip_right = icon->width + icon->x;
    group->clip_bottom = icon->y + icon->height;
    group->field_4 = 1;
    group->group = (short)a;
    SetNewGroup_Callbacks(0, (void *)RenderFreePlayIcons, (void *)FreePlayObjectIconHandler);
    do {
        AddFreePlayIcon(group, (struct InfoSource *)list, b, y, a, 1, list->after);
        list = list->next;
        y += 0x42;
    } while (list != NULL);
    AddFullScreenIcon(a + 6);
    group->field_14 = b;
    group->content_bottom = y;
    icon = FindIcon(a + 4);
    if (icon != NULL) {
        SetIconSprite(icon, sprite);
        icon->x -= 9;
        icon->string_id = 0x94;
        icon->string = GetString(0x94);
    }
    icon = FindIcon(a + 3);
    if (icon != NULL) {
        SetIconSprite(icon, sprite2);
        icon->x -= 9;
        icon->string_id = 0x95;
        icon->string = GetString(0x95);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048b4a0
void FreeFreePlayPanel(int arg) {
    struct PanelNode **head;
    struct PanelNode *cur;
    struct PanelNode *next;

    RemoveIconGroup((unsigned short)(arg + 6));

    if (arg == 0xc8) {
        cur = FreePlayPanelList1;
        head = &FreePlayPanelList1;
    } else if (arg == 0x12c) {
        cur = FreePlayPanelList4;
        head = &FreePlayPanelList4;
    } else if (arg == 0x190) {
        cur = FreePlayPanelList3;
        head = &FreePlayPanelList3;
    } else if (arg == 0x1f4) {
        cur = FreePlayPanelList2;
        head = &FreePlayPanelList2;
    } else {
        return;
    }

    if (cur == 0) {
        return;
    }
    if (cur->next != 0) {
        do {
            next = cur->next;
            free(cur->buffer1);
            free(cur->buffer2);
            free(cur);
            cur = next;
        } while (next != 0);
    }
    *head = 0;
}

// FUNCTION: LEGOLAND 0x0048b540
LEGO_EXPORT void CleanUpFreePlay(void) {
    if (FreePlayDown1Sprite) {
        KillSprite(FreePlayDown1Sprite);
        FreePlayDown1Sprite = NULL;
    }
    if (FreePlayDown2Sprite) {
        KillSprite(FreePlayDown2Sprite);
        FreePlayDown2Sprite = NULL;
    }
    if (FreePlayDown3Sprite) {
        KillSprite(FreePlayDown3Sprite);
        FreePlayDown3Sprite = NULL;
    }
    if (FreePlayDown4Sprite) {
        KillSprite(FreePlayDown4Sprite);
        FreePlayDown4Sprite = NULL;
    }
    if (FreePlayUp1Sprite) {
        KillSprite(FreePlayUp1Sprite);
        FreePlayUp1Sprite = NULL;
    }
    if (FreePlayUp2Sprite) {
        KillSprite(FreePlayUp2Sprite);
        FreePlayUp2Sprite = NULL;
    }
    if (FreePlayUp3Sprite) {
        KillSprite(FreePlayUp3Sprite);
        FreePlayUp3Sprite = NULL;
    }
    if (FreePlayUp4Sprite) {
        KillSprite(FreePlayUp4Sprite);
        FreePlayUp4Sprite = NULL;
    }
    if (FreePlayTickSprite) {
        KillSprite(FreePlayTickSprite);
        FreePlayTickSprite = NULL;
    }
    if (FreePlayCoverSprite) {
        KillSprite(FreePlayCoverSprite);
        FreePlayCoverSprite = NULL;
    }
    DestroyIconGroup(0xc8);
    DestroyIconGroup(0x12c);
    DestroyIconGroup(0x190);
    DestroyIconGroup(0x1f4);
    FreeFreePlayPanel(0xc8);
    FreeFreePlayPanel(0x12c);
    FreeFreePlayPanel(0x190);
    FreeFreePlayPanel(0x1f4);
}

/* Empty in the original (does nothing). */
// FUNCTION: LEGOLAND 0x0048b6c0
void Empty_0048b6c0(void) {
    return;
}

/* Normalizes unlocked profile level flags to 1, ensures the first level is unlocked, and records the highest
 * nonzero level slot (zero-based) for the progress-screen default selection. */
// FUNCTION: LEGOLAND 0x0048b6d0
void UpdateUnlockedLevelSelection(void) {
    int i;

    CurrentProfile.flags[4] = 1;
    for (i = 0; i < 15; i++) {
        if (CurrentProfile.flags[4 + i] != 0) {
            CurrentProfile.flags[4 + i] = 1;
            HighestUnlockedLevelIndex = i;
        }
    }
}

struct FreePlayLoadSlot {
    const char *name0;
    const char *name1;
    int pad_8[3];
    struct Sprite *sprite0;
    struct Sprite *sprite1;
};

// FUNCTION: LEGOLAND 0x0048b700
void LoadProgressScreenSprites(void) {
    int i;

    for (i = 0; i < 10; i++) {
        ProgressScreenTables.levels[i].sprite0 = LoadSprite(ProgressScreenTables.levels[i].name0, 4);
        ProgressScreenTables.levels[i].sprite1 = LoadSprite(ProgressScreenTables.levels[i].name1, 4);
    }
}

// FUNCTION: LEGOLAND 0x0048b740
void ReferenceProgressLevelSprites(void) {
    struct Sprite **esi;

    esi = &ProgressScreenTables.levels[0].sprite1;
    do {
        ReferenceSprite(esi[-1]);
        ReferenceSprite(esi[0]);
        esi = (struct Sprite **)((char *)esi + sizeof(ProgressScreenTables.levels[0]));
    } while ((INT_PTR)esi < (INT_PTR)&ProgressScreenTables.levels[10].sprite1);
}

// FUNCTION: LEGOLAND 0x0048b770
void KillProgressScreenSprites(void) {
    struct FreePlaySpriteSlot *slot;

    RemoveIconGroup(0x1c);
    RemoveIconGroup(0x23);
    slot = (struct FreePlaySpriteSlot *)&ProgressScreenTables.levels[0].sprite0;
    while ((INT_PTR)slot < (INT_PTR)&ProgressScreenTables.levels[10].sprite0) {
        while (KillSprite(slot->sprite0) == 0) {
        }
        while (KillSprite(slot->sprite1) == 0) {
        }
        slot->sprite1 = NULL;
        slot->sprite0 = NULL;
        slot++;
    }
}
