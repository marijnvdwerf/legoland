#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "gamemap.h"
#include "globals.h"
#include "llidb.h"
#include "map_object.h"
#include "render3d.h"
#include "ride_interfaces.h"
#include "ride_queue.h"

struct GoldArray {
    unsigned char pad_0[0x14];
    unsigned int field_14[6];
};

struct GoldItem {
    /* 0x00 */ unsigned char pad_0[8];
    /* 0x08 */ struct GoldSub *field_8;
    /* 0x0c */ unsigned char field_c;
};

struct GoldSub {
    unsigned char pad_0[0x36];
    unsigned char field_36;
};

struct GoldRandSub {
    unsigned char pad_0[0x58];
    unsigned int field_58;
};

struct GoldRandItem {
    unsigned char pad_0[8];
    struct GoldRandSub *field_8;
};

struct GoldNode {
    unsigned char pad_0[0xc];
    struct GoldNode *next;
    unsigned char pad_10[0x1c];
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x00406920
void FUN_00406920(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00406960
void FUN_00406960(struct GoldNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x004069c0
void FUN_004069c0(void) {
    while (DAT_004c1204 != NULL) {
        FUN_00406960(DAT_004c1204);
    }
}

// FUNCTION: LEGOLAND 0x004069e0
void *FUN_004069e0(void *param) { STUB(); }

// FUNCTION: LEGOLAND 0x00406a10
void FUN_00406a10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00406ab0
void FUN_00406ab0(void) {
    if (DAT_004c11f8 != NULL) {
        KillSprite(DAT_004c11f8);
    }
    if (DAT_004c11fc != NULL) {
        KillSprite(DAT_004c11fc);
    }
    if (DAT_004c1200 != NULL) {
        KillSprite(DAT_004c1200);
    }
    if (DAT_004c11f4 != NULL) {
        KillSprite(DAT_004c11f4);
    }
    if (DAT_004c11e4 != NULL) {
        FUN_00412290(DAT_004c11e4);
    }
    FUN_004069c0();
}

// FUNCTION: LEGOLAND 0x00406b10
void FUN_00406b10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00406e90
unsigned int FUN_00406e90(void *param) {
    struct GoldArray *a = (struct GoldArray *)FUN_004069e0(param);
    unsigned int i;

    if (a == NULL) {
        return 0;
    }
    for (i = 0; i < 6; i++) {
        if (a->field_14[i] == 0) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00406ec0
void FUN_00406ec0(struct GoldItem *item, void *param2) {
    struct GoldArray *a = (struct GoldArray *)FUN_004069e0(param2);
    unsigned int i;

    if (a == NULL) {
        return;
    }
    for (i = 0; i < 6; i++) {
        if (a->field_14[i] == 0) {
            a->field_14[i] = 1;
            item->field_8->field_36 = (unsigned char)i;
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x00406f00
struct GoldArray *FUN_00406f00(struct GoldItem *item) {
    unsigned int idx = item->field_8->field_36;
    struct GoldArray *a = (struct GoldArray *)FUN_004069e0(&item->field_c);

    if (a != NULL) {
        a->field_14[idx] = 0;
    }
    return a;
}

// FUNCTION: LEGOLAND 0x00406f30
void FUN_00406f30(int param) {
    /* param is a RideObject* carried as int (also passed to FUN_00406e90 as void*) */
    if (FUN_00406e90((void *)param) == 0) {
        Ride_SetFlagToNotLetAnyoneOn((struct RideObject *)param);
    } else {
        Ride_ClearFlagToNotLetAnyoneOn((struct RideObject *)param);
    }
}

// FUNCTION: LEGOLAND 0x00406f60
void FUN_00406f60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00407000
void FUN_00407000(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004070b0
void FUN_004070b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00407170
void FUN_00407170(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00407230
void FUN_00407230(struct GoldRandItem *param) {
    int value = rand() % 31;
    param->field_8->field_58 = value + 15;
}

// FUNCTION: LEGOLAND 0x00407250
void FUN_00407250(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004072b0
void FUN_004072b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004075b0
void FUN_004075b0(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_004c11f0;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((unsigned char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x004075f0
void FUN_004075f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004076e0
void FUN_004076e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00407800
LEGO_EXPORT int SaveGoldWash(void) {
    struct GoldNode *node;
    unsigned int one;
    unsigned int zero;

    one = 1;
    zero = 0;
    node = DAT_004c1204;
    while (node != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(node, 0x2c) == 0) {
            return 0;
        }
        node = node->next;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00407870
LEGO_EXPORT int LoadGoldWash(void) {
    unsigned int count;
    struct GoldNode *node;
    struct GoldNode *prev = NULL;

    if (!SaveGameRead(&count, 4)) {
        return 0;
    }
    while (count != 0) {
        node = malloc(0x2c);
        if (!SaveGameRead(node, 0x2c)) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            DAT_004c1204 = node;
        }
        prev = node;
        if (!SaveGameRead(&count, 4)) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004078f0
void FUN_004078f0(struct ClassNode *str, struct CallbackTable *module) {
    // STRING: LEGOLAND 0x004b4670
    if (_stricmp("GOLD RUSH", str->name) == 0) {
        module->cb_a4 = FUN_00406a10;
        module->cb_ac = FUN_00406ab0;
        module->cb_8c = FUN_004075b0;
        module->cb_a8 = FUN_004072b0;
        module->cb_b0 = FUN_00406b10;
        module->cb_98 = FUN_004075f0;
        module->cb_9c = FUN_004076e0;
        module->cb_b8 = LoadGoldWash;
        module->cb_bc = SaveGoldWash;
    }
}
