#include <stdio.h>
#include <stdlib.h>

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

struct ElemRecord {
    unsigned char pad_0[8];
    unsigned char flags;
};

struct PanelNode {
    struct PanelNode *next;
    void *buffer1;
    unsigned char pad_8[8];
    void *buffer2;
};

struct GameListNode {
    struct GameListNode *next;
    unsigned char pad_4[0x14];
    unsigned char field_18;
    unsigned char pad_19[3];
    unsigned int field_1c;
};

// LLIDB element fields read by InitFreePlayScreen (real Element is opaque).
struct FreePlayElement {
    unsigned char pad_0[8];
    unsigned int flags;
    unsigned char pad_c[4];
    unsigned int counter;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x0048a8a0
LEGO_EXPORT void InitFreePlayScreen(void) {
    struct SpriteIcon *icon;
    struct SpriteIcon *bar;
    struct FreePlayElement *elem;
    unsigned int path_idx;
    unsigned int count;
    unsigned int i;

    DAT_00668e38 = 0;
    FUN_0048b6c0();
    DAT_007cb3a0 = 0;
    DAT_00798650 = 0;

    // STRING: LEGOLAND 0x004beb34
    SPRITE_TitleScreenBk = LoadSprite("FreePlayScreenBK.lls", 0);
    // STRING: LEGOLAND 0x004beb24
    DAT_007cb3b4 = LoadSprite("FP_Down1.lls", 4);
    // STRING: LEGOLAND 0x004beb14
    DAT_007cb3ac = LoadSprite("FP_Down2.lls", 4);
    // STRING: LEGOLAND 0x004beb04
    DAT_007cb3b0 = LoadSprite("FP_Down3.lls", 4);
    // STRING: LEGOLAND 0x004beaf4
    DAT_007cb3a8 = LoadSprite("FP_Down4.lls", 4);
    // STRING: LEGOLAND 0x004beae8
    DAT_007cb3c4 = LoadSprite("FP_Up1.lls", 4);
    // STRING: LEGOLAND 0x004beadc
    DAT_007cb3c0 = LoadSprite("FP_Up2.lls", 4);
    // STRING: LEGOLAND 0x004bead0
    DAT_007cb3cc = LoadSprite("FP_Up3.lls", 4);
    // STRING: LEGOLAND 0x004beac4
    DAT_007cb3c8 = LoadSprite("FP_Up4.lls", 4);
    // STRING: LEGOLAND 0x004beab0
    DAT_007cb398 = LoadSprite("FreePlay_Tick.lls", 4);
    // STRING: LEGOLAND 0x004beaa0
    DAT_007cb3d4 = LoadSprite("FP_Cover.lls", 4);

    // STRING: LEGOLAND 0x004bea88
    icon = LoadSpriteIcon("GoBack_on_FreePlay.lls", 4, 0xd, 0x137, 7);
    icon->field_3c = 0x26;
    icon->field_38 = GetString(0x26);
    icon->field_34 |= 0x6002;
    icon->event_handler = (unsigned char (*)(unsigned int, unsigned int))FUN_0048fb80;
    DAT_006687c0 = (unsigned int)FUN_0048fb80;

    // STRING: LEGOLAND 0x004bea70
    icon = LoadSpriteIcon("Accept_On_FreePlay.lls", 4, 0x1e4, 0x14b, 7);
    icon->field_3c = 0x32;
    icon->field_38 = GetString(0x32);
    icon->field_34 |= 0x6002;
    icon->event_handler = FUN_0048ac60;
    DAT_006687bc = (unsigned int)FUN_0048ac60;
    icon->field_34 |= 0x400;
    DAT_0079864c = icon;

    // STRING: LEGOLAND 0x004bea60
    bar = LoadSpriteIcon("Bar_Rides.lls", 4, 0xd1, 0x156, 7);
    bar->field_28 = (void *)RenderFreePlayBar;
    bar->field_34 |= 0x400a;

    count = LLIDB_GetCount();
    // STRING: LEGOLAND 0x004b8a70
    LLIDB_FindElement("PATH CONTROL", &path_idx, 0);

    for (i = 0; (int)i < (int)count; i++) {
        LLIDB_GetElement(i, (struct Element **)&elem);
        if ((elem->flags & 0x10) == 0) continue;
        if ((elem->flags & 0x1) == 0) continue;
        if (path_idx == (unsigned int)elem) continue;
        if (elem->counter == 0) continue;
        elem->counter--;
        if (elem->counter != 0) continue;
        if ((elem->flags & 0x1) == 0) continue;
        elem->flags &= 0xfffcfff0;
        if ((elem->flags & 0xfff0) == 0x10 || (elem->flags & 0xfff0) == 0x1010) {
            FUN_0047c6a0((struct LLIDBHead *)elem);
        }
    }

    LLIDB_ClearOnLevel();
    InitFreePlayLists();

    FreePlayObjectList(0xc8, 0x2a, 0x41, 0xec, 0xc8);
    FreePlayObjectList(0x1f4, 0xbb, 0x41, 0xec, 0x1f4);
    FreePlayObjectList(0x190, 0x14c, 0x41, 0xec, 0x190);
    FreePlayObjectList(0x12c, 0x1dd, 0x41, 0xec, 0x12c);

    FUN_0048a790();
}

// FUNCTION: LEGOLAND 0x0048ab60
void FUN_0048ab60(void) {
    struct GameListNode *node;

    node = (struct GameListNode *)DAT_006687c8;
    if (node == 0) {
        return;
    }
    do {
        FUN_004663f0();
        if (node->field_18 == 1) {
            if (FUN_00478b20(node->field_1c) != 0) {
                FUN_00469900((struct NerpsArg *)ElemID((const char *)node->field_1c), 0, 1);
            }
        }
        node = node->next;
    } while (node != 0);
}

// FUNCTION: LEGOLAND 0x0048abb0
void FUN_0048abb0(void) {
    char buf[0x34];

    QueryClass = 0;
    // STRING: LEGOLAND 0x004beb4c
    sprintf(buf, "FreePlayTest.txt");
    FUN_00499380();
    FUN_00499410();
    FUN_0047f810();
    DAT_0079a8d0 = 0;
    ResetMapAI();
    DAT_00667c4c = FUN_0047afb0((int)buf);
    FUN_00457870(0);
    FUN_0048ab60();
    AllocBlokeCounters(lpConfig->field_1a);
    FUN_00458940();
    DAT_00832ba0 = 0;
    FUN_00489ee0();
    UpdateMenu();
    FUN_004663c0();
    FUN_00490600(1);
    FUN_004911c0((const char *)&DAT_0066861c, 0);
    FUN_00458bb0(1);
    FUN_004993c0();
    UpdateSoundVols();
}

// FUNCTION: LEGOLAND 0x0048ac60
unsigned char FUN_0048ac60(unsigned int param_1, unsigned int param_2) {
    if (DAT_0079864c != 0 && (DAT_0079864c->field_34 & 0x400) != 0) {
        return 0;
    }

    if (DAT_004bef9c != 0 && (param_2 & 0x2) != 0) {
        DAT_0080ffe5 = 2;
        FUN_00466360(0x127, 0x170);
        FUN_00498920();
        DAT_006687b0 = 0x4;
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        FUN_0048abb0();
        CleanUpFreePlay();
        KillTitleScreenSprites();
        RemoveIconGroup(0x7);
        DAT_008119b4 = 0x3;
        InitGameInterface(0x1);
        FUN_00474880();
        DAT_0080ffe5 = 2;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048ad00
LEGO_EXPORT void InitFreePlayLists(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048aef0
unsigned int FUN_0048aef0(unsigned int arg1, struct ElemRecord *arg2) {
    unsigned int result;
    struct ElemRecord *elem;

    result = FUN_0048a840(arg1, 0);
    if ((int)(DAT_007cb3a0 + result) <= 0x4e20) {
        if (arg2) {
            // STRING: LEGOLAND 0x004bb49c
            elem = (struct ElemRecord *)ElemID("BUILD MENU"); /* TODO: fold — ElemID handle (uint) viewed as ElemRecord* */
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
void FUN_0048af40(unsigned int param) {
    struct ClipQueryResult *node;

    node = 0;
    DAT_007cb3a0 += FUN_0048a840(param, &node);
    if (node != 0) {
        node->field_c = 1;
    }

    if (DAT_00798650++ == 0) {
        DAT_0079864c->field_34 &= ~0x400u;
    }
}

// FUNCTION: LEGOLAND 0x0048afa0
void FUN_0048afa0(unsigned int param) {
    struct ClipQueryResult *node;

    node = 0;
    DAT_007cb3a0 -= FUN_0048a840(param, &node);
    if (node != 0) {
        node->field_c = 0;
    }

    if (--DAT_00798650 == 0) {
        DAT_0079864c->field_34 |= 0x400;
    }
}

// FUNCTION: LEGOLAND 0x0048b000
void FUN_0048b000(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048b110
LEGO_EXPORT void Add2FreePlayPanelLists(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048b2a0
LEGO_EXPORT unsigned int FreePlayObjectList(int a, int b, int c, int d, int e) { STUB(); }

// FUNCTION: LEGOLAND 0x0048b4a0
void FUN_0048b4a0(int arg) {
    struct PanelNode **head;
    struct PanelNode *cur;
    struct PanelNode *next;

    RemoveIconGroup((unsigned short)(arg + 6));

    if (arg == 0xc8) {
        cur = DAT_007cb3d0;
        head = &DAT_007cb3d0;
    } else if (arg == 0x12c) {
        cur = DAT_007cb39c;
        head = &DAT_007cb39c;
    } else if (arg == 0x190) {
        cur = DAT_007cb3b8;
        head = &DAT_007cb3b8;
    } else if (arg == 0x1f4) {
        cur = DAT_007cb3a4;
        head = &DAT_007cb3a4;
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
    if (DAT_007cb3b4) {
        KillSprite(DAT_007cb3b4);
        DAT_007cb3b4 = NULL;
    }
    if (DAT_007cb3ac) {
        KillSprite(DAT_007cb3ac);
        DAT_007cb3ac = NULL;
    }
    if (DAT_007cb3b0) {
        KillSprite(DAT_007cb3b0);
        DAT_007cb3b0 = NULL;
    }
    if (DAT_007cb3a8) {
        KillSprite(DAT_007cb3a8);
        DAT_007cb3a8 = NULL;
    }
    if (DAT_007cb3c4) {
        KillSprite(DAT_007cb3c4);
        DAT_007cb3c4 = NULL;
    }
    if (DAT_007cb3c0) {
        KillSprite(DAT_007cb3c0);
        DAT_007cb3c0 = NULL;
    }
    if (DAT_007cb3cc) {
        KillSprite(DAT_007cb3cc);
        DAT_007cb3cc = NULL;
    }
    if (DAT_007cb3c8) {
        KillSprite(DAT_007cb3c8);
        DAT_007cb3c8 = NULL;
    }
    if (DAT_007cb398) {
        KillSprite(DAT_007cb398);
        DAT_007cb398 = NULL;
    }
    if (DAT_007cb3d4) {
        KillSprite(DAT_007cb3d4);
        DAT_007cb3d4 = NULL;
    }
    FUN_0046fb40(0xc8);
    FUN_0046fb40(0x12c);
    FUN_0046fb40(0x190);
    FUN_0046fb40(0x1f4);
    FUN_0048b4a0(0xc8);
    FUN_0048b4a0(0x12c);
    FUN_0048b4a0(0x190);
    FUN_0048b4a0(0x1f4);
}

// FUNCTION: LEGOLAND 0x0048b6c0
void FUN_0048b6c0(void) {
    return;
}

// FUNCTION: LEGOLAND 0x0048b6d0
void FUN_0048b6d0(void) {
    int i;

    DAT_0080ffa0.field_34[0] = 1;
    for (i = 0; i < 15; i++) {
        if (DAT_0080ffa0.field_34[i] != 0) {
            DAT_0080ffa0.field_34[i] = 1;
            DAT_007cb394 = i;
        }
    }
}

// FUNCTION: LEGOLAND 0x0048b700
void FUN_0048b700(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048b740
void FUN_0048b740(void) {
    int *esi;

    esi = (int *)&DAT_004beb94[0].field_4;
    do {
        ReferenceSprite((struct Sprite *)esi[-1]);
        ReferenceSprite((struct Sprite *)esi[0]);
        esi += 7;
    } while ((long)esi < (long)DAT_004becb0);
}

// FUNCTION: LEGOLAND 0x0048b770
void FUN_0048b770(void) {
    struct FreePlaySpriteSlot *slot;

    RemoveIconGroup(0x1c);
    RemoveIconGroup(0x23);
    slot = DAT_004beb94;
    while ((int)slot < (int)&DAT_004becac) {
        while (KillSprite(slot->field_0) == 0) {
        }
        while (KillSprite(slot->field_4) == 0) {
        }
        slot->field_4 = NULL;
        slot->field_0 = NULL;
        slot++;
    }
}
