#include "legoland.h"

#include "gamemap.h"
#include "binv.h"
#include "obj_instance.h"
#include "sound_music.h"
#include "llidb.h"
#include "map_object.h"

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

typedef void (*PlaneRideMethod)(void);

struct PlaneRideInterface {
    unsigned char pad_0[0x8c];
    PlaneRideMethod field_8c;
    unsigned char pad_90[8];
    PlaneRideMethod field_98;
    PlaneRideMethod field_9c;
    PlaneRideMethod field_a0;
    PlaneRideMethod field_a4;
    PlaneRideMethod field_a8;
    PlaneRideMethod field_ac;
    PlaneRideMethod field_b0;
    unsigned char pad_b4[4];
    PlaneRideMethod field_b8;
    PlaneRideMethod field_bc;
};

extern void *DAT_0062fe9c;
extern unsigned int DAT_0062fe58;
extern unsigned int DAT_0062fe60;
extern unsigned int DAT_0062fe64;
extern unsigned int DAT_0062fe68;
extern unsigned short DAT_0062fe6c;
extern void *DAT_0062fe78;
extern void *DAT_0062fe90;
extern void *DAT_0062fe94;
extern unsigned int DAT_0081cae0;
extern unsigned char DAT_004beba0;
extern unsigned char DAT_004b79d0[0x18];
extern unsigned int EditMode;
extern void *DAT_008119b8;
extern struct Cursor EditCursor;

extern void *_malloc(unsigned int size);
extern int FUN_0049e4b2(void);
#include "image_sprite.h"
extern int __strcmpi(const char *s1, const char *s2);

void FUN_0043d9f0(void *param_1);
void FUN_0043e410(void);

// FUNCTION: LEGOLAND 0x0043d880
void FUN_0043d880(void *param_1) {
    void *node = _malloc(0x24);
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
    int eax = FUN_0049e4b2();
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
int SaveZoomer(void) {
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

// FUNCTION: LEGOLAND 0x0043e110
void LoadZoomer(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043e220
void FUN_0043e220(const char **name, struct PlaneRideInterface *iface) {
    // STRING: LEGOLAND 0x004b7a6c
    if (__strcmpi("PLANE RIDE", *name) == 0) {
        iface->field_a4 = (PlaneRideMethod)FUN_0043dda0;
        iface->field_ac = (PlaneRideMethod)FUN_0043dee0;
        iface->field_8c = (PlaneRideMethod)FUN_0043df50;
        iface->field_a8 = (PlaneRideMethod)FUN_0043e410;
        iface->field_b0 = (PlaneRideMethod)FUN_0043da60;
        iface->field_9c = (PlaneRideMethod)FUN_0043df90;
        iface->field_98 = (PlaneRideMethod)FUN_0043dfe0;
        iface->field_a0 = (PlaneRideMethod)FUN_0043e010;
        iface->field_b8 = (PlaneRideMethod)LoadZoomer;
        iface->field_bc = (PlaneRideMethod)SaveZoomer;
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
