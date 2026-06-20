#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "legoland.h"

#include "clipping.h"
#include "freeplay.h"
#include "icon.h"
#include "llidb.h"

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

struct NerpsArg;

void FUN_004663f0(void);
int FUN_00478b20(unsigned int arg);
void FUN_00469900(struct NerpsArg *object, unsigned int a, unsigned int b);

unsigned int FUN_00499380(void);
void FUN_00499410(void);
void FUN_0047f810(void);
void ResetMapAI(void);
int FUN_0047afb0(int param_1);
void FUN_00457870(int param_1);
void AllocBlokeCounters(unsigned int size);
void FUN_00458940(void);
void FUN_00489ee0(void);
void UpdateMenu(void);
void FUN_004663c0(void);
void FUN_00490600(unsigned int param_1);
unsigned int FUN_004911c0(const char *a, const char *b);
void FUN_00458bb0(unsigned int param_1);
void FUN_004993c0(void);
void UpdateSoundVols(void);

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x0048a8a0
LEGO_EXPORT void InitFreePlayScreen(void) { STUB(); }

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
void FUN_0048ac60(void) { STUB(); }

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
LEGO_EXPORT void FreePlayObjectList(void) { STUB(); }

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
