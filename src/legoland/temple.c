#include "legoland.h"
#include "crt.h"

#include "gamemap.h"
#include "obj_instance.h"
#include "map_object.h"
#include "globals.h"

typedef void (*TempleHandler)(void);

struct TempleObj {
    unsigned char pad_0[0x8c];
    TempleHandler field_8c;
    unsigned char pad_90[0x8];
    TempleHandler field_98;
    TempleHandler field_9c;
    unsigned char pad_a0[0x4];
    TempleHandler field_a4;
    TempleHandler field_a8;
    TempleHandler field_ac;
    TempleHandler field_b0;
};

struct TempleObject {
    unsigned char pad_0[12];
    unsigned int field_c;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x004169c0
void FUN_004169c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00416a30
void FUN_00416a30(void) {
    if (DAT_004cbf68 != 0) {
        KillSprite(DAT_004cbf68);
    }
    if (DAT_004cbf6c != 0) {
        KillSprite(DAT_004cbf6c);
    }
}

// FUNCTION: LEGOLAND 0x00416a60
void FUN_00416a60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00416b50
void FUN_00416b50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00416dc0
void FUN_00416dc0(void) {
    void *temp = DAT_004cbf5c;
    EditMode = 1;
    DAT_008119b8 = temp;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((void *)((unsigned int)DAT_008119b8 + 0x3c));
}

// FUNCTION: LEGOLAND 0x00416e00
void FUN_00416e00(unsigned int param_1, unsigned int param_2) {
    AddBasicObject(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x00416e20
void FUN_00416e20(struct TempleObject *a1, void *a2, unsigned int a3) {
    StandardRemoveObject((unsigned int)a1, (unsigned int)a2, a3);
    RemoveAllBlokesFromRide(a1->field_c, a2);
}

// FUNCTION: LEGOLAND 0x00416e50
void FUN_00416e50(const char **str, struct TempleObj *obj) {
    // STRING: LEGOLAND 0x004b4ef0
    if (_stricmp("TEMPLE", *str) == 0) {
        obj->field_a4 = FUN_004169c0;
        obj->field_ac = FUN_00416a30;
        obj->field_8c = FUN_00416dc0;
        obj->field_a8 = FUN_00416b50;
        obj->field_b0 = FUN_00416a60;
        obj->field_9c = FUN_00416e20;
        obj->field_98 = FUN_00416e00;
    }
}
