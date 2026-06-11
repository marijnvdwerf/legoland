#include "legoland.h"
#include <string.h>

#include "gamemap.h"
#include "render3d.h"
#include "sound_music.h"
#include "map_object.h"
#include "llidb.h"
#include "globals.h"

typedef void (*CatapultVtblFn)(void);

struct CatapultInterface {
    unsigned char pad_0[0x8c];
    CatapultVtblFn field_8c;
    CatapultVtblFn field_90;
    CatapultVtblFn field_94;
    CatapultVtblFn field_98;
    CatapultVtblFn field_9c;
    CatapultVtblFn field_a0;
    CatapultVtblFn field_a4;
    CatapultVtblFn field_a8;
    CatapultVtblFn field_ac;
    CatapultVtblFn field_b0;
    CatapultVtblFn pad_b4;
    CatapultVtblFn field_b8;
    CatapultVtblFn field_bc;
};

struct CatapultNode {
    unsigned int field_0;
    struct CatapultNode *next;
};

struct CatapultItem {
    unsigned char pad_0[8];
    unsigned int field_8;
    unsigned char pad_c[1];
    signed char field_d;
};

struct CatapultLayer {
    unsigned char pad_0[0xc];
    struct CatapultSprite *field_c;
};

struct CatapultSprite {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned char pad_20[0x44];
    struct CatapultInner *field_64;
};

struct CatapultInner {
    unsigned char pad_0[0x10];
    unsigned int field_10;
};

#include "image_sprite.h"


// FUNCTION: LEGOLAND 0x004030f0
void FUN_004030f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00403130
void FUN_00403130(struct CatapultNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00403190
void FUN_00403190(void) {
    while (DAT_004c1118 != NULL) {
        FUN_00403130(DAT_004c1118);
    }
}

// FUNCTION: LEGOLAND 0x004031b0
void FUN_004031b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004031e0
void FUN_004031e0(struct CatapultLayer *param1) {
    struct CatapultSprite *sprite;
    struct CatapultInner *inner;

    sprite = param1->field_c;
    DAT_004c10f4 = sprite;
    if (sprite != NULL) {
        sprite->field_1c |= 0x420;
        inner = ((struct CatapultSprite *)DAT_004c10f4)->field_64;
        if (inner != NULL) {
            inner->field_10 |= 0x2000;
            DAT_004c10f0 = ((struct CatapultSprite *)DAT_004c10f4)->field_64;
        }
    }
    HideLayer(DAT_004c10f0, 0);
    HideLayer(DAT_004c10f0, 1);
    Load_FXList(Catapult_SFX, 4);
}

// FUNCTION: LEGOLAND 0x00403250
void FUN_00403250(void) {
    FUN_00403190();
    Kill_FXList(Catapult_SFX, 4);
}

// FUNCTION: LEGOLAND 0x00403270
void FUN_00403270(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00403430
void FUN_00403430(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00403480
void FUN_00403480(struct CatapultItem *item) {
    if (item->field_8 & 0x1) {
        GetSpriteForLayer(DAT_004c10f0, 1);
        item->field_d++;
        if (item->field_d >= 0x20) {
            item->field_8 &= 0xfffffffe;
        }
    }
}

// FUNCTION: LEGOLAND 0x004034c0
void FUN_004034c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00403530
void FUN_00403530(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00403580
void FUN_00403580(void *arg, unsigned int index) {
    unsigned char *p = (unsigned char *)arg + index * 4;
    *(unsigned int *)(p + 0x28) = 0;
    *(unsigned int *)(p + 0x10) = 0;
}

// FUNCTION: LEGOLAND 0x004035a0
void FUN_004035a0(struct CatapultNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00403690
void FUN_00403690(void) {
    struct CatapultNode *node = DAT_004c1118;
    while (node != NULL) {
        FUN_004035a0(node);
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x004036b0
void FUN_004036b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004036f0
void FUN_004036f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004037d0
void FUN_004037d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00403820
void FUN_00403820(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00403930
void FUN_00403930(void) {
    DAT_008119b8 = DAT_004c10f4;
    EditMode = 1;
    DAT_008119b8 = DAT_004c10f4;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((unsigned char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00403970
void FUN_00403970(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004039a0
void FUN_004039a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004039e0
unsigned int *FUN_004039e0(struct CatapultLayer *arg1, unsigned short arg2) {
    struct CatapultSprite *sprite = arg1->field_c;

    DAT_004c1100 = sprite->field_64;
    DAT_004c1104 = sprite->field_14;
    DAT_004c1108 = sprite->field_18;
    DAT_004c110c = arg2;

    sprite->field_64->field_10 |= 0x2000;

    return (unsigned int *)&DAT_004c1100;
}

// FUNCTION: LEGOLAND 0x00403a20
LEGO_EXPORT int Catapult_Save(void) {
    struct CatapultSaveBuf { unsigned int data[15]; };
    struct CatapultNode *node;
    int *field;
    unsigned int *cursor;
    int i;
    int index;
    unsigned int one;
    unsigned int zero;
    struct CatapultSaveBuf scratch;

    one = 1;
    zero = 0;
    node = DAT_004c1118;
    while (node != NULL) {
        scratch = *(struct CatapultSaveBuf *)node;
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        field = (int *)&scratch.data[4];
        i = 4;
        do {
            index = 0;
            for (cursor = *(unsigned int **)((char *)DAT_004c10f4 + 0xcc); cursor != NULL; cursor = (unsigned int *)*cursor) {
                if (cursor == (unsigned int *)*field) {
                    if (cursor != NULL) {
                        *field = index + 1;
                        goto next;
                    }
                    break;
                }
                index = index + 1;
            }
            *field = 0;
        next:
            field++;
            i--;
        } while (i != 0);
        if (SaveGameWrite(&scratch, 0x3c) == 0) {
            return 0;
        }
        node = node->next;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00403af0
LEGO_EXPORT void Catapult_Load(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00403bb0
void FUN_00403bb0(const char **name, struct CatapultInterface *ci) {
    // STRING: LEGOLAND 0x004b412c
    if (_stricmp("CATAPULT", *name) == 0) {
        ci->field_a4 = (CatapultVtblFn)FUN_004031e0;
        ci->field_ac = (CatapultVtblFn)FUN_00403250;
        ci->field_8c = (CatapultVtblFn)FUN_00403930;
        ci->field_a8 = FUN_00403820;
        ci->field_b0 = FUN_00403270;
        ci->field_9c = FUN_004039a0;
        ci->field_98 = FUN_00403970;
        ci->field_a0 = (CatapultVtblFn)FUN_004039e0;
        ci->field_bc = Catapult_Save;
        ci->field_b8 = Catapult_Load;
    }
}
