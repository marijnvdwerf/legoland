#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "binv.h"
#include "llidb.h"
#include "sound_music.h"
#include "spider_ride.h"

struct FadeParams {
    unsigned int field_0;
    unsigned char pad_4[4];
    unsigned int field_8;
    unsigned int field_c;
};

struct SpiderState {
    unsigned char field_0;
    unsigned char field_1;
    unsigned char field_2;
    unsigned char field_3;
    unsigned char field_4;
    unsigned char pad_5[3];
    unsigned int field_8;
    unsigned char pad_c[4];
    unsigned int field_10;
};

struct CarNode {
    /* 0x00 */ unsigned char pad_0[0x10];
    /* 0x10 */ unsigned int field_10;
    /* 0x14 */ unsigned int field_14;
    /* 0x18 */ unsigned int field_18;
    /* 0x1c */ unsigned char pad_1c[0x48];
    /* 0x64 */ struct CarNode *next;
};

struct RideObject {
    const char *name;
    unsigned char pad_4[0x88];
    void (*field_8c)(void);
    unsigned char pad_90[0x8];
    void (*field_98)(void);
    void (*field_9c)(void);
    void (*field_a0)(void);
    void (*field_a4)(void);
    void (*field_a8)(void);
    void (*field_ac)(void);
    void (*field_b0)(void);
    unsigned char pad_b4[4];
    void (*field_b8)(void);
    void (*field_bc)(void);
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x004158f0
int FUN_004158f0(struct SpiderNode *arg0) {
    struct SpiderNode *node = (struct SpiderNode *)malloc(0x30);
    if (node == NULL) {
        return;
    }
    memset(node, 0, 0x30);
    node->field_0 = arg0->field_0;
    node->next = (struct SpiderNode *)DAT_004cbf58;
    DAT_004cbf58 = node;
    return FUN_00415a90(node);
}

// FUNCTION: LEGOLAND 0x00415930
void FUN_00415930(struct SpiderNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00415990
void FUN_00415990(void) {
    struct SpiderNode *node = (struct SpiderNode *)DAT_004cbf58;
    while (node != NULL) {
        FUN_00415930(node);
        node = (struct SpiderNode *)DAT_004cbf58;
    }
}

// FUNCTION: LEGOLAND 0x004159b0
void FUN_004159b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004159e0
void FUN_004159e0(const unsigned char *arg0) {
    struct SampleParams params;
    params.field_0 = 0x2;
    params.field_8 = arg0[0];
    params.field_c = arg0[1];
    PlayInstanceOfSample(DAT_004b4d90, 1, 1, &params);
}

// FUNCTION: LEGOLAND 0x00415a20
void FUN_00415a20(const unsigned char *src) {
    struct FadeParams params;
    params.field_0 = 2;
    params.field_8 = src[0];
    params.field_c = src[1];
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
}

// FUNCTION: LEGOLAND 0x00415a60
void FUN_00415a60(struct SpiderState *a) {
    a->field_3 = a->field_2;
    a->field_2 = 0;
    a->field_8 = (a->field_8 & 0xffffbfff) | 0x1;
    a->field_4 = 0;
    a->field_10 = 0;
    FUN_004159e0((const unsigned char *)a);
}

// FUNCTION: LEGOLAND 0x00415a90
int FUN_00415a90(struct SpiderNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00415ae0
void FUN_00415ae0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00415e80
void FUN_00415e80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00415fd0
void FUN_00415fd0(struct CarNode *param_1) {
    DAT_004cbf20 = ((unsigned int *)param_1)[3];

    if (DAT_0082c668 != NULL) {
        KillSprite(DAT_0082c668);
    }
    if (DAT_004cbf10 != 0) {
        FreeBinV(DAT_004cbf10);
    }
    if (DAT_004cbf24 != 0) {
        FreeBinV(DAT_004cbf24);
    }
    if (DAT_004cbf18 != 0) {
        FreeBinV(DAT_004cbf18);
    }
    KillSprite(DAT_004cbf14);
    KillSprite(DAT_004cbf1c);
    FUN_00415990();
    Kill_FXList(SpiderRide_SFX, 1);
}

// FUNCTION: LEGOLAND 0x00416060
void FUN_00416060(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004160a0
void FUN_004160a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004160f0
void FUN_004160f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00416120
unsigned int *FUN_00416120(unsigned int *a1, unsigned short a2) {
    struct CarNode *car = *(struct CarNode **)((unsigned char *)a1 + 0xc);
    DAT_004cbf40 = (unsigned int)car->next;
    DAT_004cbf44 = car->field_14;
    DAT_004cbf48 = car->field_18;
    DAT_004cbf4c = a2;
    car = car->next;
    car->field_10 = car->field_10 | 0x2000;
    return &DAT_004cbf40;
}

// FUNCTION: LEGOLAND 0x00416160
void SpiderRide(char **name_ptr, struct RideObject *obj) {
    // STRING: LEGOLAND 0x004b4eb8
    if (_stricmp("SPIDER RIDE", *name_ptr) == 0) {
        obj->field_a4 = FUN_00415e80;
        obj->field_ac = FUN_00415fd0;
        obj->field_8c = FUN_00416060;
        obj->field_a8 = FUN_00416330;
        obj->field_b0 = FUN_00415ae0;
        obj->field_9c = FUN_004160a0;
        obj->field_98 = FUN_004160f0;
        obj->field_a0 = FUN_00416120;
        obj->field_bc = SaveSpider;
        obj->field_b8 = LoadSpider;
    }
}

// FUNCTION: LEGOLAND 0x004161f0
void FUN_004161f0(struct SpiderNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00416310
void FUN_00416310(void) {
    struct SpiderNode *node = (struct SpiderNode *)DAT_004cbf58;
    while (node != NULL) {
        FUN_004161f0(node);
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00416330
void FUN_00416330(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00416830
void FUN_00416830(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00416880
LEGO_EXPORT int SaveSpider(void) {
    struct SpiderNode *node = (struct SpiderNode *)DAT_004cbf58;
    unsigned int one = 1;
    unsigned int zero = 0;

    while (node != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(node, 0x30) == 0) {
            return 0;
        }
        node = node->next;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

struct SpiderTypeC {
    void *field_0;
    unsigned int field_4;
};

struct SpiderData {
    unsigned char pad_0[0x54];
    struct SpiderTypeC *field_54;
};

struct SpiderCar2 {
    unsigned char pad_0[0x2c];
    void *field_2c;
    unsigned int field_30;
};

struct SpiderListNode {
    struct SpiderListNode *next;
    unsigned char pad_4[4];
    struct SpiderData *field_8;
    unsigned char pad_c[4];
    struct SpiderCar2 *field_10;
};

struct SpiderGameObject {
    unsigned char pad_0[0xcc];
    struct SpiderListNode *field_cc;
};

struct SpiderLoadArg {
    unsigned char pad_0[0xc];
    struct SpiderGameObject *field_c;
};

// FUNCTION: LEGOLAND 0x004168f0
LEGO_EXPORT int LoadSpider(struct SpiderLoadArg *arg) {
    struct SpiderGameObject *obj = arg->field_c;
    struct SpiderNode *prev = NULL;
    struct SpiderListNode *list;
    struct SpiderCar2 *car;
    struct SpiderData *data;
    struct SpiderTypeC *tc;
    unsigned int marker;

    if (!SaveGameRead(&marker, 4)) {
        return 0;
    }
    while (marker != 0) {
        struct SpiderNode *node = (struct SpiderNode *)malloc(0x30);
        if (!SaveGameRead(node, 0x30)) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            DAT_004cbf58 = node;
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
            car->field_2c = DAT_004cbf38[car->field_30];
        } else {
            car->field_2c = NULL;
            list->field_10->field_30 = 0;
        }
        data = list->field_8;
        tc = data->field_54;
        if (tc != NULL) {
            tc->field_0 = DAT_004cbf30[tc->field_4];
        }
        list = list->next;
    }
    return 1;
}
