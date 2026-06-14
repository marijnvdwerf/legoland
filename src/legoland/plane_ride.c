#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "binv.h"
#include "gamemap.h"
#include "llidb.h"
#include "map_object.h"
#include "obj_instance.h"
#include "plane_ride.h"
#include "sound_music.h"
#include "sound_sfx.h"

struct PlaneRideNode {
    unsigned char pad_0[0x20];
    struct PlaneRideNode *next;
};

struct PlaneRideObject {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

struct PlaneRideBlockData {
    unsigned char pad_0[0x10];
    unsigned int field_10;
};

struct PlaneRideBlock {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[0x48];
    struct PlaneRideBlockData *field_64;
};

struct PlaneRideRoot {
    unsigned char pad_0[0xc];
    struct PlaneRideBlock *field_c;
};

#include "image_sprite.h"
#include "ride_interfaces.h"

// FUNCTION: LEGOLAND 0x0043d880
void FUN_0043d880(void *param_1) {
    void *node = malloc(0x24);
    if (node == NULL) {
        return;
    }
    memset(node, 0, 0x24);
    *(unsigned short *)node = *(unsigned short *)param_1;
    ((unsigned int *)node)[8] = (unsigned int)DAT_0062fe9c;
    DAT_0062fe9c = node;
    FUN_0043d9f0(node);
}

// FUNCTION: LEGOLAND 0x0043d8c0
void FUN_0043d8c0(void *param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x0043d940
void FUN_0043d940(void) {
    while (DAT_0062fe9c != NULL) {
        FUN_0043d8c0(DAT_0062fe9c);
    }
}

// FUNCTION: LEGOLAND 0x0043d960
unsigned int FUN_0043d960(struct PlaneRideObject **arg) { STUB(); }

// FUNCTION: LEGOLAND 0x0043d990
void FUN_0043d990(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043d9f0
void FUN_0043d9f0(void *param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x0043da60
void FUN_0043da60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043dda0
void FUN_0043dda0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043dee0
void FUN_0043dee0(struct PlaneRideObject *input) {
    DAT_0062fe58 = input->field_c;
    if (DAT_0081cae0) {
        KillSprite(DAT_0081cae0);
    }
    if (DAT_0062fe90) {
        FreeBinV(DAT_0062fe90);
    }
    if (DAT_0062fe94) {
        FreeBinV(DAT_0062fe94);
    }
    if (DAT_0062fe78) {
        FreeBinV(DAT_0062fe78);
    }
    FUN_0043d940();
    Kill_FXList(DAT_004b79d0, 2);
}

// FUNCTION: LEGOLAND 0x0043df50
void FUN_0043df50(void) {
    EditMode = 1;
    DAT_008119b8 = (void *)DAT_0062fe58;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x0043df90
void FUN_0043df90(struct PlaneRideObject *a1, struct PlaneRideObject *a2, struct PlaneRideObject *a3) {
    unsigned int temp = FUN_0043d960(&a2);
    if (temp != 0) {
        FUN_0043d8c0((void *)temp);
    }
    StandardRemoveObject((unsigned int)a1, (unsigned int)a2, (unsigned int)a3);
    RemoveAllBlokesFromRide(a1->field_c, a2);
}

// FUNCTION: LEGOLAND 0x0043dfe0
void FUN_0043dfe0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043e010
unsigned int *FUN_0043e010(struct PlaneRideRoot *param1, unsigned short param2) {
    struct PlaneRideBlock *block = param1->field_c;

    DAT_0062fe60 = (unsigned int)block->field_64;
    DAT_0062fe64 = block->field_14;
    DAT_0062fe68 = block->field_18;
    DAT_0062fe6c = param2;
    block->field_64->field_10 |= 0x2000;

    return &DAT_0062fe60;
}

// FUNCTION: LEGOLAND 0x0043e050
unsigned int FUN_0043e050(int param2, int param1, signed char n) {
    int count = n;
    int eax = rand();
    int index = eax % count;
    signed char slot = *((signed char *)param1 + index + 0x1c);
    int ecx;

    while (slot != 0) {
        index++;
        if (index >= count) {
            index = 0;
        }
        slot = *((signed char *)param1 + index + 0x1c);
    }

    *((signed char *)param1 + index + 0x1c) = 1;
    ecx = *(int *)(param2 + 0x8);
    *((signed char *)ecx + 0x36) = (signed char)(index + 1);
    return index + 1;
}

// FUNCTION: LEGOLAND 0x0043e0a0
LEGO_EXPORT int SaveZoomer(void) {
    struct PlaneRideNode *current = DAT_0062fe9c;
    unsigned int flag = 1;
    unsigned int terminator = 0;

    while (current != NULL) {
        if (!SaveGameWrite(&flag, 4)) {
            return 0;
        }
        if (!SaveGameWrite(current, 36)) {
            return 0;
        }
        current = current->next;
    }

    if (SaveGameWrite(&terminator, 4)) {
        return 1;
    }
    return 0;
}

struct ZoomerSampleParams {
    int field_0;
    unsigned char pad_4[4];
    unsigned int field_8;
    unsigned int field_c;
};

struct ZoomerTypeC {
    void *field_0;
    unsigned int field_4;
};

struct ZoomerData {
    unsigned char pad_0[0x54];
    struct ZoomerTypeC *field_54;
};

struct ZoomerCar {
    unsigned char pad_0[0x2c];
    void *field_2c;
    unsigned int field_30;
};

struct ZoomerListNode {
    struct ZoomerListNode *next;
    unsigned char pad_4[4];
    struct ZoomerData *field_8;
    unsigned char pad_c[4];
    struct ZoomerCar *field_10;
};

struct ZoomerGameObject {
    unsigned char pad_0[0xcc];
    struct ZoomerListNode *field_cc;
};

struct ZoomerLoadArg {
    unsigned char pad_0[0xc];
    struct ZoomerGameObject *field_c;
};

// FUNCTION: LEGOLAND 0x0043e110
LEGO_EXPORT int LoadZoomer(struct ZoomerLoadArg *arg) {
    struct ZoomerGameObject *obj = arg->field_c;
    struct PlaneRideNode *prev = NULL;
    struct ZoomerListNode *list;
    struct ZoomerCar *car;
    struct ZoomerData *data;
    struct ZoomerTypeC *tc;
    struct ZoomerSampleParams params;
    unsigned int marker;

    if (!SaveGameRead(&marker, 4)) {
        return 0;
    }
    while (marker != 0) {
        struct PlaneRideNode *node = (struct PlaneRideNode *)malloc(0x24);
        if (!SaveGameRead(node, 0x24)) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            DAT_0062fe9c = node;
        }
        params.field_8 = ((unsigned char *)node)[0];
        params.field_0 = 2;
        params.field_c = ((unsigned char *)node)[1];
        PauseSingleSample(PlayInstanceOfSample(*(void **)(DAT_004b79d0 + 8), 1, 1, &params));
        prev = node;
        if (!SaveGameRead(&marker, 4)) {
            return 0;
        }
    }

    list = obj->field_cc;
    while (list != NULL) {
        car = list->field_10;
        if (car->field_30 != 0) {
            car->field_2c = (&DAT_0062fe94)[car->field_30];
        } else {
            car->field_2c = NULL;
            list->field_10->field_30 = 0;
        }
        data = list->field_8;
        tc = data->field_54;
        if (tc != NULL) {
            tc->field_0 = DAT_0062fe84[tc->field_4];
        }
        list = list->next;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0043e220
void FUN_0043e220(struct ClassNode *name, struct CallbackTable *iface) {
    // STRING: LEGOLAND 0x004b7a6c
    if (_stricmp("PLANE RIDE", name->name) == 0) {
        iface->cb_a4 = FUN_0043dda0;
        iface->cb_ac = FUN_0043dee0;
        iface->cb_8c = FUN_0043df50;
        iface->cb_a8 = FUN_0043e410;
        iface->cb_b0 = FUN_0043da60;
        iface->cb_9c = FUN_0043df90;
        iface->cb_98 = FUN_0043dfe0;
        iface->cb_a0 = FUN_0043e010;
        iface->cb_b8 = LoadZoomer;
        iface->cb_bc = SaveZoomer;
    }
}

// FUNCTION: LEGOLAND 0x0043e2b0
void FUN_0043e2b0(void *param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x0043e3f0
void FUN_0043e3f0(void) {
    struct PlaneRideNode *current = DAT_0062fe9c;

    while (current != NULL) {
        FUN_0043e2b0(current);
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x0043e410
void FUN_0043e410(void) { STUB(); }
