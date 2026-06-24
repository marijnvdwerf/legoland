#include "legoland.h"

#include "gamemap.h"
#include "globals.h"
#include "map_object.h"
#include "mechanics_hut.h"

struct HutObject {
    unsigned char pad_0[0x1c];
    unsigned int field_1c;
    unsigned char pad_20[0x64 - 0x20];
    unsigned int field_64;
};

struct HutContext {
    unsigned char pad_0[0xc];
    struct HutObject *field_c;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x0043d250
unsigned int FUN_0043d250(struct HutContext *ctx) {
    DAT_0081caf4 = ctx->field_c;
    ((struct HutObject *)DAT_0081caf4)->field_1c |= 0x420;
    DAT_0062fe50 = ((struct HutObject *)DAT_0081caf4)->field_64;
    ((struct HutObject *)DAT_0081caf4)->field_1c |= 0x2000;
    // STRING: LEGOLAND 0x004b79a4
    DAT_0062fe54 = LoadSprite("MechHutMask.lls", 1);
    return (unsigned int)DAT_0062fe54;
}

// FUNCTION: LEGOLAND 0x0043d2a0
unsigned int FUN_0043d2a0(unsigned int param1, unsigned int param2) {
    return AddBasicObject(param1, param2);
}

// FUNCTION: LEGOLAND 0x0043d2c0
void FUN_0043d2c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043d2f0
void FUN_0043d2f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043d580
void FUN_0043d580(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043d730
void FUN_0043d730(void) {
    KillSprite(DAT_0062fe54);
}

// FUNCTION: LEGOLAND 0x0043d740
void FUN_0043d740(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = DAT_0081caf4;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&((struct EditCursorData *)EditMode.unk8)->field_3c);
}

// FUNCTION: LEGOLAND 0x0043d780
unsigned int *FUN_0043d780(void *ptr, unsigned short arg2) {
    unsigned int *p = (unsigned int *)((unsigned int *)ptr)[3];

    DAT_0062fe30 = p[25];
    DAT_0062fe34 = ((unsigned int *)p)[5];
    DAT_0062fe38 = ((unsigned int *)p)[6];
    DAT_0062fe3c = arg2;

    p = p[25];
    p[4] |= 0x2000;

    return &DAT_0062fe30;
}

// FUNCTION: LEGOLAND 0x0043d7c0
void FUN_0043d7c0(void) { STUB(); }
