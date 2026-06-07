#include "legoland.h"
#include "globals.h"

#include "clipping.h"
#include "freeplay.h"
#include "icon.h"
#include "llidb.h"

struct ElemRecord {
    unsigned char pad_0[8];
    unsigned char flags;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x0048a8a0
LEGO_EXPORT void InitFreePlayScreen(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048ab60
void FUN_0048ab60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048abb0
void FUN_0048abb0(void) { STUB(); }

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
void FUN_0048af40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048afa0
void FUN_0048afa0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048b000
void FUN_0048b000(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048b110
LEGO_EXPORT void Add2FreePlayPanelLists(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048b2a0
LEGO_EXPORT void FreePlayObjectList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048b4a0
void FUN_0048b4a0(int arg) { STUB(); }

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
        ReferenceSprite((struct Sprite *)esi[-1]); /* TODO-fold: esi walks the slot array as int*; ReferenceSprite's canonical param is struct Sprite* */
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
