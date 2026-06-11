#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "binv.h"
#include "gamemap.h"
#include "llidb.h"
#include "map_object.h"
#include "spinning_barrels.h"

typedef void (*BarrelVtblFn)(void);

struct BarrelSource {
    unsigned short field_0;
};

struct BarrelCarNode {
    /* 0x00 */ unsigned char pad_0[0x10];
    /* 0x10 */ unsigned int field_10;
    /* 0x14 */ unsigned int field_14;
    /* 0x18 */ unsigned int field_18;
    /* 0x1c */ unsigned char pad_1c[0x48];
    /* 0x64 */ struct BarrelCarNode *next;
};

struct BarrelRide {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ struct BarrelCarNode *car;
    /* 0x10 */ unsigned char pad_10[0x7c];
    /* 0x8c */ BarrelVtblFn var_8c;
    unsigned char pad_1[8];
    BarrelVtblFn var_98;
    BarrelVtblFn var_9c;
    BarrelVtblFn var_a0;
    BarrelVtblFn var_a4;
    BarrelVtblFn var_a8;
    BarrelVtblFn var_ac;
    BarrelVtblFn var_b0;
    unsigned char pad_2[4];
    BarrelVtblFn var_b8;
    BarrelVtblFn var_bc;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x0043bdb0
void FUN_0043bdb0(void *param1) {
    struct BarrelNode *block = (struct BarrelNode *)malloc(0x34);
    if (block == NULL) {
        return;
    }
    memset(block, 0, 0x34);
    block->field_4 = ((struct BarrelSource *)param1)->field_0;
    block->field_8 = 0;
    block->next = DAT_0062fe08;
    DAT_0062fe08 = block;
    FUN_0043c2f0(block);
}

// FUNCTION: LEGOLAND 0x0043be00
void FUN_0043be00(struct BarrelNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x0043be40
void FUN_0043be40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043be70
void FUN_0043be70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043c2f0
void FUN_0043c2f0(struct BarrelNode *node) {
    node->field_14 = 0;
    node->field_8 = 0;
    node->field_18 = 0;
    node->field_6 = 0;
    node->field_c = node->field_c & 0xffffbffe;
    node->field_10 = 3;
}

// FUNCTION: LEGOLAND 0x0043c320
void FUN_0043c320(struct BarrelNode *node) {
    unsigned int packed = node->field_c;
    unsigned char prev = node->field_6;
    packed &= 0xffffbfff;
    node->field_7 = prev;
    packed |= 0x1;
    node->field_6 = 0;
    node->field_c = packed;
}

// FUNCTION: LEGOLAND 0x0043c340
void FUN_0043c340(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043c490
void FUN_0043c490(void) {
    EditMode = 1;
    DAT_008119b8 = (void *)DAT_0062fde4;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((unsigned int *)((unsigned int)DAT_008119b8 + 0x3c));
}

// FUNCTION: LEGOLAND 0x0043c4d0
void FUN_0043c4d0(void) {
    while (DAT_0062fe08 != NULL) {
        FUN_0043be00(DAT_0062fe08);
    }
}

// FUNCTION: LEGOLAND 0x0043c4f0
void FUN_0043c4f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043c540
void FUN_0043c540(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043c570
unsigned int *FUN_0043c570(struct BarrelRide *ride, unsigned short param2) {
    struct BarrelCarNode *target = ride->car;

    DAT_0062fdb0 = (unsigned int)target->next;
    DAT_0062fdb4 = target->field_14;
    DAT_0062fdb8 = target->field_18;
    DAT_0062fdbc = param2;

    target = target->next;
    target->field_10 |= 0x2000;
    return &DAT_0062fdb0;
}

// FUNCTION: LEGOLAND 0x0043c5b0
void FUN_0043c5b0(void) {
    KillSprite(DAT_0062fdd0);
    KillSprite(DAT_0062fdcc);
    KillSprite(DAT_0062fe04);
    if (DAT_0062fde8 != NULL) {
        FreeBinV(DAT_0062fde8);
    }
    if (DAT_0062fdfc != NULL) {
        FreeBinV(DAT_0062fdfc);
    }
    if (DAT_0062fdc8 != NULL) {
        FreeBinV(DAT_0062fdc8);
    }
    FUN_0043c4d0();
}

// FUNCTION: LEGOLAND 0x0043c620
LEGO_EXPORT int SaveSBarrel(void) {
    struct BarrelNode *node = DAT_0062fe08;
    unsigned int marker = 1;
    unsigned int terminator = 0;

    if (node != NULL) {
        do {
            if (SaveGameWrite(&marker, 4)) {
                if (SaveGameWrite(node, 0x34)) {
                    node = node->next;
                } else {
                    return 0;
                }
            } else {
                return 0;
            }
        } while (node != NULL);
    }

    return SaveGameWrite(&terminator, 4) != 0;
}

struct BarrelTypeC {
    void *field_0;
    unsigned int field_4;
};

struct BarrelData {
    unsigned char pad_0[0x54];
    struct BarrelTypeC *field_54;
};

struct BarrelCar {
    unsigned char pad_0[0x2c];
    void *field_2c;
    unsigned int field_30;
};

struct BarrelListNode {
    struct BarrelListNode *next;
    unsigned char pad_4[4];
    struct BarrelData *field_8;
    unsigned char pad_c[4];
    struct BarrelCar *field_10;
};

struct BarrelGameObject {
    unsigned char pad_0[0xcc];
    struct BarrelListNode *field_cc;
};

struct BarrelLoadArg {
    unsigned char pad_0[0xc];
    struct BarrelGameObject *field_c;
};

// FUNCTION: LEGOLAND 0x0043c690
LEGO_EXPORT int LoadSBarrel(struct BarrelLoadArg *arg) {
    struct BarrelGameObject *obj = arg->field_c;
    struct BarrelNode *prev = NULL;
    struct BarrelListNode *list;
    struct BarrelCar *car;
    struct BarrelData *data;
    struct BarrelTypeC *tc;
    unsigned int marker;

    if (!SaveGameRead(&marker, 4)) {
        return 0;
    }
    while (marker != 0) {
        struct BarrelNode *node = (struct BarrelNode *)malloc(0x34);
        if (!SaveGameRead(node, 0x34)) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            DAT_0062fe08 = node;
        }
        prev = node;
        if (!SaveGameRead(&marker, 4)) {
            return 0;
        }
    }

    list = obj->field_cc;
    while (list != NULL) {
        car = list->field_10;
        if (car->field_30 != 0) {
            car->field_2c = DAT_0062fe00[car->field_30];
        } else {
            car->field_2c = NULL;
            list->field_10->field_30 = 0;
        }
        data = list->field_8;
        tc = data->field_54;
        if (tc != NULL) {
            tc->field_0 = DAT_0062fdf0[tc->field_4];
        }
        list = list->next;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0043c760
void FUN_0043c760(const char **str, struct BarrelRide *ride) {
    // STRING: LEGOLAND 0x004b7978
    if (_stricmp("SPINNING BARRELS RIDE", *str) == 0) {
        ride->var_a4 = FUN_0043c340;
        ride->var_8c = FUN_0043c490;
        ride->var_a8 = FUN_0043c950;
        ride->var_b0 = FUN_0043be70;
        ride->var_9c = FUN_0043c4f0;
        ride->var_98 = FUN_0043c540;
        ride->var_ac = FUN_0043c5b0;
        ride->var_a0 = (BarrelVtblFn)FUN_0043c570;
        ride->var_bc = (BarrelVtblFn)SaveSBarrel;
        ride->var_b8 = LoadSBarrel;
    }
}

// FUNCTION: LEGOLAND 0x0043c7f0
void FUN_0043c7f0(struct BarrelNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x0043c930
void FUN_0043c930(void) {
    struct BarrelNode *node = DAT_0062fe08;
    while (node != NULL) {
        FUN_0043c7f0(node);
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x0043c950
void FUN_0043c950(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043ce10
unsigned char FUN_0043ce10(int param1, int param2, signed char param3) {
    int rnd;
    int idx;

    rnd = rand();
    idx = rnd % param3;

    while (((unsigned char *)param2)[idx + 0x21] != 0) {
        idx += 1;
        if (idx >= param3) {
            idx = 0;
        }
    }

    ((unsigned char *)param2)[idx + 0x21] = 1;
    *(unsigned char *)(*(int *)((unsigned char *)param1 + 0x8) + 0x36) = (unsigned char)(idx + 1);
    return (unsigned char)(idx + 1);
}
