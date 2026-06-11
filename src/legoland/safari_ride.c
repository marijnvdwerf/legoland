#include "legoland.h"
#include <stdlib.h>
#include <string.h>
#include "globals.h"

#include "gamemap.h"
#include "binv.h"
#include "sound_music.h"
#include "llidb.h"
#include "map_object.h"
#include "safari_ride.h"

#include "image_sprite.h"

struct SafariBlockData {
    unsigned char pad_0[0x10];
    unsigned int field_10;
};

struct SafariBlock {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[0x48];
    struct SafariBlockData *field_64;
};

struct SafariRoot {
    unsigned char pad_0[0xc];
    struct SafariBlock *field_c;
};

struct SafariObject {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

// FUNCTION: LEGOLAND 0x004149c0
void FUN_004149c0(struct SafariNode *param) {
    struct SafariNode *s = (struct SafariNode *)malloc(sizeof(struct SafariNode));
    if (s) {
        memset(s, 0, sizeof(struct SafariNode));
        s->field_0 = param->field_0;
        s->next = (struct SafariNode *)DAT_004cbf0c;
        DAT_004cbf0c = s;
        FUN_00414b10(s);
    }
}

// FUNCTION: LEGOLAND 0x00414a00
void FUN_00414a00(struct SafariNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00414a60
void FUN_00414a60(void) {
    while (DAT_004cbf0c != NULL) {
        FUN_00414a00(DAT_004cbf0c);
    }
}

// FUNCTION: LEGOLAND 0x00414a80
void FUN_00414a80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414ab0
void FUN_00414ab0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414b10
void FUN_00414b10(struct SafariNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00414b80
void FUN_00414b80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414d90
void FUN_00414d90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414ea0
void FUN_00414ea0(struct SafariObject *a1) {
    DAT_004cbec4 = a1->field_c;
    FUN_00414a60();
    Kill_FXList(SAFARI_SFX, 1);
    FreeBinV(DAT_004cbef8);
    FreeBinV(DAT_004cbefc);
    FreeBinV(DAT_004cbf00);
    KillSprite(DAT_004cbf08);
}

// FUNCTION: LEGOLAND 0x00414f00
void FUN_00414f00(void) {
    EditMode = 1;
    DAT_008119b8 = (void *)DAT_004cbec4;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00414f40
void FUN_00414f40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414fc0
void FUN_00414fc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414ff0
unsigned int *FUN_00414ff0(struct SafariRoot *p1, unsigned short arg2) {
    struct SafariBlock *pB = p1->field_c;
    DAT_004cbed0 = (unsigned int)pB->field_64;
    DAT_004cbed4 = pB->field_14;
    DAT_004cbed8 = pB->field_18;
    DAT_004cbedc = arg2;
    pB->field_64->field_10 |= 0x2000;
    return &DAT_004cbed0;
}

// FUNCTION: LEGOLAND 0x00415030
void FUN_00415030(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004150c0
void FUN_004150c0(struct SafariNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00415200
void FUN_00415200(void) {
    struct SafariNode *current = (struct SafariNode *)DAT_004cbf0c;
    if (current != NULL) {
        while (current != NULL) {
            FUN_004150c0(current);
            current = current->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x00415220
void FUN_00415220(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00415760
void FUN_00415760(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004157b0
LEGO_EXPORT int SaveSafariRide(void) {
    struct SafariNode *current = (struct SafariNode *)DAT_004cbf0c;
    unsigned int value1 = 1;
    unsigned int value0 = 0;

    while (current != NULL) {
        if (SaveGameWrite(&value1, 0x4) == 0) {
            return 0;
        }
        if (SaveGameWrite(current, 0x28) == 0) {
            return 0;
        }
        current = current->next;
    }

    if (SaveGameWrite(&value0, 0x4) == 0) {
        return 0;
    }
    return 1;
}

struct SafariTypeC {
    void *field_0;
    unsigned int field_4;
};

struct SafariData {
    unsigned char pad_0[0x54];
    struct SafariTypeC *field_54;
};

struct SafariCar {
    unsigned char pad_0[0x2c];
    void *field_2c;
    unsigned int field_30;
};

struct SafariListNode {
    struct SafariListNode *next;
    unsigned char pad_4[4];
    struct SafariData *field_8;
    unsigned char pad_c[4];
    struct SafariCar *field_10;
};

struct SafariGameObject {
    unsigned char pad_0[0xcc];
    struct SafariListNode *field_cc;
};

struct SafariLoadArg {
    unsigned char pad_0[0xc];
    struct SafariGameObject *field_c;
};

// FUNCTION: LEGOLAND 0x00415820
LEGO_EXPORT int LoadSafariRide(struct SafariLoadArg *arg) {
    struct SafariGameObject *obj = arg->field_c;
    struct SafariNode *prev = NULL;
    struct SafariListNode *list;
    struct SafariCar *car;
    struct SafariData *data;
    struct SafariTypeC *tc;
    unsigned int marker;

    if (!SaveGameRead(&marker, 4)) {
        return 0;
    }
    while (marker != 0) {
        struct SafariNode *node = (struct SafariNode *)malloc(0x28);
        if (!SaveGameRead(node, 0x28)) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            DAT_004cbf0c = node;
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
            car->field_2c = DAT_004cbf04[car->field_30];
        } else {
            car->field_2c = NULL;
            list->field_10->field_30 = 0;
        }
        data = list->field_8;
        tc = data->field_54;
        if (tc != NULL) {
            tc->field_0 = (&DAT_004cbef8)[tc->field_4];
        }
        list = list->next;
    }
    return 1;
}
