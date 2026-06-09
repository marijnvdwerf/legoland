#include "legoland.h"
#include "crt.h"
#include "globals.h"

#include "gamemap.h"
#include "jungle_cruise.h"
#include "objclass.h"
#include "render3d.h"
#include "llidb.h"
#include "map_object.h"

struct JungleNode {
    unsigned short field_0;
    unsigned char pad_2[1010];
    struct JungleNode *next;
};

struct JungleCursor {
    unsigned char pad_0[0x1c];
    unsigned int field_1c;
    unsigned char pad_20[0x3c - 0x20];
    unsigned char foot;
};

struct JungleHolder {
    unsigned char pad_0[0xc];
    struct JungleCursor *cursor;
};

struct JungleObject {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[0x64 - 0x1c];
    unsigned int field_64;
};

struct JungleContainer {
    unsigned char pad_0[0xc];
    struct JungleObject *object;
};

struct JungleScore {
    unsigned short field_0;
    unsigned char pad_2[58];
    struct JungleScore *next;
    unsigned int field_40;
};

struct JungleEntry {
    unsigned short field_0;
    unsigned char pad_2[14];
    struct JungleEntry *next;
};

struct JungleFishNode {
    unsigned short field_0;
    unsigned int field_4;
    struct JungleFishNode *next;
};

struct JungleLLS {
    unsigned short field_0;
};

struct JungleFishObj {
    unsigned int pad_0[5];
    unsigned int field_14;
    unsigned int field_18;
    unsigned int pad_1c[18];
    unsigned int field_64;
};

struct JungleFishHolder {
    unsigned int pad_0[3];
    struct JungleFishObj *field_c;
};


#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x00432ac0
void FUN_00432ac0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00432b90
void FUN_00432b90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00432cb0
void FUN_00432cb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00432d00
void FUN_00432d00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004332c0
unsigned int FUN_004332c0(unsigned short *param_1) {
    struct JungleNode *node = DAT_00616164;
    unsigned int count = 0;
    if (node != NULL) {
        unsigned short *src = param_1;
        while (node != NULL) {
            unsigned short value = *src;
            node->field_0 = value;
            if (value != 0) {
                count++;
            }
            node = node->next;
        }
    }
    return count;
}

// FUNCTION: LEGOLAND 0x004332f0
void FUN_004332f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004333b0
void FUN_004333b0(struct JungleRide *param_1) {
    FUN_00433840(param_1, param_1->field_3dc, 4);
    param_1->field_3e0 = 4;
    param_1->field_10 = param_1->field_8 + 5;
}

// FUNCTION: LEGOLAND 0x004333e0
void FUN_004333e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004334c0
void FUN_004334c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00433840
void FUN_00433840(struct JungleRide *param_1, unsigned int param_2, unsigned int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x00433ca0
void FUN_00433ca0(struct JungleHolder *param_1) {
    struct JungleCursor *cursor = param_1->cursor;
    DAT_0081cb70 = cursor;
    cursor->field_1c |= 0x400;
    // STRING: LEGOLAND 0x004b720c
    DAT_0081cb68 = LoadSprite("brijmask.lls", 1);
}

// FUNCTION: LEGOLAND 0x00433cd0
void FUN_00433cd0(void) {
    KillSprite(DAT_0081cb68);
}

// FUNCTION: LEGOLAND 0x00433ce0
void FUN_00433ce0(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb70;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint(&DAT_0081cb70->foot);
}

// FUNCTION: LEGOLAND 0x00433d20
void FUN_00433d20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00433d90
void FUN_00433d90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00433fa0
unsigned int FUN_00433fa0(unsigned int param_1, unsigned int param_2) {
    return BasicObjectDCalcCursor(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x00433fc0
void FUN_00433fc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00434040
unsigned int *FUN_00434040(struct JungleContainer *param_1, unsigned short param_2) {
    struct JungleObject *obj = param_1->object;
    DAT_0082c6a0 = obj->field_64;
    DAT_0082c6a4 = obj->field_14;
    DAT_0082c6a8 = obj->field_18;
    DAT_0082c6ac = param_2;
    return &DAT_0082c6a0;
}

// FUNCTION: LEGOLAND 0x00434080
void FUN_00434080(struct JungleHolder *param_1) {
    struct JungleCursor *cursor = param_1->cursor;
    DAT_0081cb74 = (unsigned int)cursor;
    cursor->field_1c |= 0x400;
    // STRING: LEGOLAND 0x004b721c
    DAT_0081cb6c = LoadSprite("mfish2.lls", 1);
}

// FUNCTION: LEGOLAND 0x004340b0
void FUN_004340b0(void) {
    KillSprite(DAT_0081cb6c);
}

// FUNCTION: LEGOLAND 0x004340c0
void FUN_004340c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00434100
void FUN_00434100(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00434330
void FUN_00434330(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00434650
unsigned int FUN_00434650(unsigned int param_1, unsigned int param_2) {
    return BasicObjectDCalcCursor(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x00434670
void FUN_00434670(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00434740
unsigned int *FUN_00434740(struct JungleFishHolder *param_1, unsigned short param_2) {
    struct JungleFishNode *node = (struct JungleFishNode *)DAT_00629c30;
    struct JungleFishObj *obj = param_1->field_c;
    struct JungleLLS *lls1;
    struct JungleLLS *lls2;
    short frame;

    while (node != NULL && node->field_0 != param_2) {
        node = node->next;
    }

    /* TODO: fold SpriteLLS/JungleLLS — GetLLSForSprite takes a SpriteLLS* (stored
       here as the unsigned int sprite handle) and returns a JungleLLS* as unsigned int. */
    lls1 = (struct JungleLLS *)GetLLSForSprite((struct SpriteLLS *)obj->field_64);

    if (node->field_4 != 0) {
        if (lls1->field_0 == 0) {
            node->field_4 = 0;
        }
    } else {
        if (lls1->field_0 == 1) {
            if (rand() % 7 == 0) {
                node->field_4 = 1;
            }
        }
    }

    if (node->field_4 != 0) {
        lls2 = (struct JungleLLS *)GetLLSForSprite((struct SpriteLLS *)DAT_0081cb6c);
        frame = lls1->field_0;
        LLSSetFrame((struct LLS *)lls2, frame); /* TODO: fold struct JungleLLS into struct LLS */
        DAT_0082c6a0 = (unsigned int)DAT_0081cb6c;
    } else {
        DAT_0082c6a0 = obj->field_64;
    }

    DAT_0082c6a4 = obj->field_14;
    DAT_0082c6a8 = obj->field_18;
    DAT_0082c6ac = param_2;

    return &DAT_0082c6a0;
}

// FUNCTION: LEGOLAND 0x00434b40
void FUN_00434b40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00434cb0
void FUN_00434cb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00434e50
void FUN_00434e50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00434f50
void FUN_00434f50(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb60;
    DefaultCursor(&EditCursor);
    DAT_00629c50 = DAT_004b7278;
    DAT_004b7288 = DAT_004b7260;
    SetEditCursorFootPrint(DAT_00629c40);
}

// FUNCTION: LEGOLAND 0x00434f90
void FUN_00434f90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435150
void FUN_00435150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435230
void FUN_00435230(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435470
void FUN_00435470(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435750
void FUN_00435750(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435bd0
void FUN_00435bd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435c70
void FUN_00435c70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00435ec0
void FUN_00435ec0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00436130
void FUN_00436130(unsigned short param_1, unsigned int param_2) {
    struct JungleScore *node = DAT_00629c3c;
    while (node != NULL) {
        if (node->field_0 == param_1) {
            if (node != NULL) {
                node->field_40 += param_2;
            }
            return;
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00436160
void FUN_00436160(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00436190
void FUN_00436190(struct JungleHolder *param_1) {
    DAT_0081cb54 = (unsigned int)param_1->cursor;
}

// FUNCTION: LEGOLAND 0x004361a0
void FUN_004361a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00436200
void FUN_00436200(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00436470
void FUN_00436470(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004365f0
void FUN_004365f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004367b0
void FUN_004367b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00436a40
void FUN_00436a40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00436dc0
void FUN_00436dc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00436f30
void FUN_00436f30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00436fb0
void FUN_00436fb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004371b0
struct JungleEntry *FUN_004371b0(unsigned char param_1, unsigned char param_2) {
    struct JungleEntry *entry;
    unsigned short combined;
    unsigned char temp[2];

    temp[0] = param_1;
    temp[1] = param_2;
    combined = *(unsigned short *)temp;

    entry = (struct JungleEntry *)DAT_0062fd2c;
    if (entry != NULL) {
        while (1) {
            if (entry->field_0 == combined) {
                break;
            }
            entry = entry->next;
            if (entry == NULL) {
                break;
            }
        }
    }
    return entry;
}

// FUNCTION: LEGOLAND 0x004371e0
void FUN_004371e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00437260
void FUN_00437260(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004373c0
void FUN_004373c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00437440
void FUN_00437440(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00437570
void FUN_00437570(void) { STUB(); }
