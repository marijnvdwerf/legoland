#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "gamemap.h"
#include "globals.h"
#include "llidb.h"
#include "map_object.h"
#include "render3d.h"
#include "objclass.h"
#include "sound_music.h"

struct CatapultNode {
    unsigned short field_0;
    unsigned char pad_2[2];
    struct CatapultNode *next;
};

struct CatapultItem {
    unsigned char field_0;
    unsigned char field_1;
    unsigned char pad_2[6];
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
struct CatapultNode *FUN_004030f0(const unsigned short *arg) {
    struct CatapultNode *node = (struct CatapultNode *)malloc(0x3c);
    if (node != NULL) {
        memset(node, 0, 0x3c);
        node->field_0 = *arg;
        node->next = DAT_004c1118;
        DAT_004c1118 = node;
    }
}

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
void FUN_00403430(struct CatapultItem *node) {
    struct SampleParams params;

    if (node->field_8 != 0) {
        return;
    }

    node->field_8 = 1;
    node->field_d = 0;
    params.field_0 = 2;
    params.field_8 = node->field_0;
    params.field_c = node->field_1;
    PlayInstanceOfSample(*(void **)(Catapult_SFX + 0x2c), 0, 1, &params);
}

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
void FUN_004034c0(unsigned char *data, unsigned int index) {
    struct SampleParams params;
    void **samples = (void **)(Catapult_SFX + 8);

    if (data[index + 0x20] != 0) {
        return;
    }

    data[index + 0x20] = 1;
    data[index + 0x24] = 0;
    params.field_0 = 2;
    params.field_8 = data[0];
    params.field_c = data[1];
    {
        int r = rand() % 3;
        PlayInstanceOfSample(samples[r * 3], 0, 1, &params);
    }
}

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
    struct CatapultSaveBuf {
        unsigned int data[15];
    };
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
void FUN_00403bb0(struct ClassNode *name, struct CallbackTable *ci) {
    // STRING: LEGOLAND 0x004b412c
    if (_stricmp("CATAPULT", name->name) == 0) {
        ci->cb_a4 = FUN_004031e0;
        ci->cb_ac = FUN_00403250;
        ci->cb_8c = FUN_00403930;
        ci->cb_a8 = FUN_00403820;
        ci->cb_b0 = FUN_00403270;
        ci->cb_9c = FUN_004039a0;
        ci->cb_98 = FUN_00403970;
        ci->cb_a0 = FUN_004039e0;
        ci->cb_bc = Catapult_Save;
        ci->cb_b8 = Catapult_Load;
    }
}
