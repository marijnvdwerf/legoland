#include "legoland.h"

#include "gamemap.h"
#include "money.h"
#include "obj_instance.h"

struct Building {
    unsigned char pad_0[0x1c];
    unsigned int flags;
};

struct MapObject {
    unsigned char pad_0[0xc];
    struct Building *building;
};

struct RideBuilding {
    unsigned char pad_0[0x10];
    unsigned int field_10;
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[0x64 - 0x1c];
    struct RideBuilding *field_64;
};

struct RideObject {
    unsigned char pad_0[0xc];
    struct RideBuilding *building;
};

extern unsigned int LoadSprite(const char *filename, unsigned int param_2);
extern void KillSprite(unsigned int sprite);
extern void SetEditCursorFootPrint(void *param_1);
extern void AddBasicObject(struct MapObject *obj, void *param_2);
extern void StandardRemoveObject(struct RideObject *obj, void *param_2, void *param_3);

extern unsigned int DAT_0081cb18;
extern unsigned int DAT_0081cb20;
extern unsigned int DAT_0081cb28;
extern unsigned int DAT_0081cb48;
extern unsigned int DAT_0081cb50;
extern struct Building *DAT_0081cb3c;
extern struct Building *DAT_0081cb4c;
extern struct Building *DAT_0081cb40;
extern struct Building *DAT_0081cb44;
extern unsigned int DAT_0082c6a0;
extern unsigned int DAT_0082c6a4;
extern unsigned int DAT_0082c6a8;
extern unsigned short DAT_0082c6ac;

extern unsigned int EditMode;
extern void *DAT_008119b8;
extern struct Cursor EditCursor;

void FUN_00439230(unsigned int param_1, void *param_2);

// FUNCTION: LEGOLAND 0x00439200
void FUN_00439200(struct MapObject *obj) {
    struct Building *building = obj->building;
    DAT_0081cb3c = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b751c
    DAT_0081cb18 = LoadSprite("Lego Shop 1 Matte.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x00439230
void FUN_00439230(unsigned int param_1, void *param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x004392b0
void FUN_004392b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00439320
void FUN_00439320(struct MapObject *obj, void *param_2) {
    struct Building *building = obj->building;
    AddBasicObject(obj, param_2);
    FUN_00439230((unsigned int)building, param_2);
}

// FUNCTION: LEGOLAND 0x00439350
void FUN_00439350(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004393a0
void FUN_004393a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004393e0
void FUN_004393e0(void) {
    if (DAT_0081cb18 != 0) {
        KillSprite(DAT_0081cb18);
    }
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00439400
void FUN_00439400(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00439460
void FUN_00439460(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004396d0
void FUN_004396d0(struct MapObject *obj) {
    struct Building *building = obj->building;
    DAT_0081cb4c = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b7534
    DAT_0081cb20 = LoadSprite("Lego Shop 2 Matte.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x00439700
void FUN_00439700(void) {
    if (DAT_0081cb20 != 0) {
        KillSprite(DAT_0081cb20);
    }
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00439720
void FUN_00439720(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb4c;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00439760
void FUN_00439760(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00439950
void FUN_00439950(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00439c20
void FUN_00439c20(struct MapObject *obj) {
    struct Building *building = obj->building;
    DAT_0081cb40 = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b7564
    DAT_0081cb48 = LoadSprite("LegMediaShopMask1.LLS", 1);
    // STRING: LEGOLAND 0x004b754c
    DAT_0081cb50 = LoadSprite("LegMediaShopMask2.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x00439c60
void FUN_00439c60(struct MapObject *obj, void *param_2) {
    struct Building *building = obj->building;
    AddBasicObject(obj, param_2);
    FUN_00439230((unsigned int)building, param_2);
}

// FUNCTION: LEGOLAND 0x00439c90
void FUN_00439c90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00439ce0
void FUN_00439ce0(void) {
    KillSprite(DAT_0081cb48);
    KillSprite(DAT_0081cb50);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00439d00
void FUN_00439d00(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb40;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00439d40
void FUN_00439d40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00439ef0
void FUN_00439ef0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043a0f0
void FUN_0043a0f0(struct MapObject *obj) {
    struct Building *building = obj->building;
    DAT_0081cb44 = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b757c
    DAT_0081cb28 = LoadSprite("Explorers Institute Matte.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0043a120
void FUN_0043a120(void) {
    KillSprite(DAT_0081cb28);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x0043a140
void FUN_0043a140(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb44;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x0043a180
void FUN_0043a180(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043a1e0
void FUN_0043a1e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043a390
unsigned int *FUN_0043a390(struct RideObject *obj, unsigned short param_2) {
    struct RideBuilding *building = obj->building;
    DAT_0082c6a0 = (unsigned int)building->field_64;
    DAT_0082c6a4 = building->field_14;
    DAT_0082c6a8 = building->field_18;
    DAT_0082c6ac = param_2;
    building->field_64->field_10 |= 0x2000;
    return &DAT_0082c6a0;
}

// FUNCTION: LEGOLAND 0x0043a3d0
void FUN_0043a3d0(struct RideObject *obj, void *param_2, void *param_3) {
    StandardRemoveObject(obj, param_2, param_3);
    RemoveAllBlokesFromRide((unsigned int)obj->building, param_2);
}

// FUNCTION: LEGOLAND 0x0043a400
void FUN_0043a400(void) { STUB(); }
