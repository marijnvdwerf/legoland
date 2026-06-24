#include <string.h>
#include "legoland.h"

#include "gamemap.h"
#include "globals.h"
#include "map_object.h"
#include "obj_instance.h"
#include "objclass.h"
#include "temple.h"

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
    EditMode.unk0 = 1;
    EditMode.unk8 = temp;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((void *)((unsigned int)EditMode.unk8 + 0x3c));
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
void FUN_00416e50(struct ClassNode *str, struct CallbackTable *obj) {
    // STRING: LEGOLAND 0x004b4ef0
    if (_stricmp("TEMPLE", str->name) == 0) {
        obj->cb_a4 = FUN_004169c0;
        obj->cb_ac = FUN_00416a30;
        obj->cb_8c = FUN_00416dc0;
        obj->cb_a8 = FUN_00416b50;
        obj->cb_b0 = FUN_00416a60;
        obj->cb_9c = FUN_00416e20;
        obj->cb_98 = FUN_00416e00;
    }
}
