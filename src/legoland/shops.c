#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "gamemap.h"
#include "map_object.h"
#include "math.h"
#include "money.h"
#include "obj_instance.h"
#include "objclass.h"
#include "path_control.h"
#include "shops.h"
#include "tilemap.h"
#include "western_town.h"

struct Building {
    unsigned char pad_0[0x1c];
    unsigned int flags;
};

struct PathArea {
    unsigned char pad_0[0x3c];
    int x0;
    int y0;
    int x1;
    int y1;
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

#include "image_sprite.h"

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
void FUN_00439230(struct PathArea *area, struct Point *origin) {
    int xend = area->x1 + origin->x;
    int x = area->x0 + origin->x;
    int y = area->y0 + origin->y;
    int yend = area->y1 + origin->y;

    if (x > xend)
        return;

    do {
        if (y <= yend) {
            struct Point coord;
            do {
                coord.x = x;
                coord.y = y;
                AddPathTileGFX(&coord, *(unsigned short *)PathSprite);
                y++;
            } while (y <= yend);
        }
        y = area->y0 + origin->y;
        x++;
    } while (x <= xend);
}

// FUNCTION: LEGOLAND 0x004392b0
void FUN_004392b0(struct PathArea *area, struct Point *origin) {
    int xend = area->x1 + origin->x;
    int x = area->x0 + origin->x;
    int y = area->y0 + origin->y;
    int yend = area->y1 + origin->y;

    if (x > xend)
        return;

    do {
        if (y <= yend) {
            int coord[2];
            do {
                coord[0] = x;
                coord[1] = y;
                RemoveRollerCoasterPath(coord);
                y++;
            } while (y <= yend);
        }
        y = area->y0 + origin->y;
        x++;
    } while (x <= xend);
}

// FUNCTION: LEGOLAND 0x00439320
void FUN_00439320(struct MapObject *obj, void *param_2) {
    struct Building *building = obj->building;
    AddBasicObject((unsigned int)obj, (unsigned int)param_2);
    FUN_00439230((unsigned int)building, param_2);
}

// FUNCTION: LEGOLAND 0x00439350
void FUN_00439350(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004393a0
void FUN_004393a0(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb3c;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

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
    AddBasicObject((unsigned int)obj, (unsigned int)param_2);
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
    StandardRemoveObject((unsigned int)obj, (unsigned int)param_2, (unsigned int)param_3);
    RemoveAllBlokesFromRide((unsigned int)obj->building, param_2);
}

// FUNCTION: LEGOLAND 0x0043a400
void FUN_0043a400(struct ClassNode *name, struct CallbackTable *ci) {
    // STRING: LEGOLAND 0x004b75fc
    if (_stricmp("GENERAL STORE", name->name) == 0) {
        ci->cb_a4 = FUN_004375d0;
        ci->cb_ac = FUN_00437610;
        ci->cb_8c = FUN_00437630;
        ci->cb_a0 = FUN_0043a390;
        ci->cb_a8 = FUN_004378e0;
        ci->cb_9c = FUN_0043a3d0;
        ci->cb_b0 = FUN_00437670;
        return;
    }
    // STRING: LEGOLAND 0x004b75f4
    if (_stricmp("SHERIFF", name->name) == 0) {
        ci->cb_a4 = FUN_00437ba0;
        ci->cb_ac = FUN_00437bd0;
        ci->cb_8c = FUN_00437bf0;
        ci->cb_a0 = FUN_0043a390;
        ci->cb_a8 = FUN_00437c90;
        ci->cb_9c = FUN_0043a3d0;
        ci->cb_b0 = FUN_00437c30;
        return;
    }
    // STRING: LEGOLAND 0x004b75e8
    if (_stricmp("JAIL CELL", name->name) == 0) {
        ci->cb_a4 = FUN_00438070;
        ci->cb_ac = FUN_004380f0;
        ci->cb_8c = FUN_00438110;
        ci->cb_a0 = FUN_0043a390;
        ci->cb_a8 = FUN_00438430;
        ci->cb_98 = FUN_00437f60;
        ci->cb_9c = FUN_00438020;
        ci->cb_b0 = FUN_00438150;
        ci->cb_b8 = LoadJailCells;
        ci->cb_bc = SaveJailCells;
        return;
    }
    // STRING: LEGOLAND 0x004b75e0
    if (_stricmp("BANK", name->name) == 0) {
        ci->cb_a4 = FUN_00438870;
        ci->cb_ac = FUN_004388a0;
        ci->cb_8c = FUN_004388c0;
        ci->cb_a0 = FUN_0043a390;
        ci->cb_a8 = FUN_00438960;
        ci->cb_9c = FUN_0043a3d0;
        ci->cb_b0 = FUN_00438900;
        return;
    }
    // STRING: LEGOLAND 0x004b75d8
    if (_stricmp("SALOON", name->name) == 0) {
        ci->cb_a4 = FUN_00438c60;
        ci->cb_ac = FUN_00438ca0;
        ci->cb_8c = FUN_00438cc0;
        ci->cb_a0 = FUN_0043a390;
        ci->cb_a8 = FUN_00438f10;
        ci->cb_9c = FUN_0043a3d0;
        ci->cb_b0 = FUN_00438d00;
        return;
    }
    // STRING: LEGOLAND 0x004b75c4
    if (_stricmp("EXPLORERS INSTITUTE", name->name) == 0) {
        ci->cb_a4 = FUN_0043a0f0;
        ci->cb_ac = FUN_0043a120;
        ci->cb_8c = FUN_0043a140;
        ci->cb_a0 = FUN_0043a390;
        ci->cb_a8 = FUN_0043a1e0;
        ci->cb_9c = FUN_0043a3d0;
        ci->cb_b0 = FUN_0043a180;
        return;
    }
    // STRING: LEGOLAND 0x004b75b8
    if (_stricmp("LEGO SHOP 1", name->name) == 0) {
        ci->cb_a4 = FUN_00439200;
        ci->cb_98 = FUN_00439320;
        ci->cb_9c = FUN_00439350;
        ci->cb_ac = FUN_004393e0;
        ci->cb_8c = FUN_004393a0;
        ci->cb_a0 = FUN_0043a390;
        ci->cb_a8 = FUN_00439460;
        ci->cb_b0 = FUN_00439400;
        return;
    }
    // STRING: LEGOLAND 0x004b75ac
    if (_stricmp("LEGO SHOP 2", name->name) == 0) {
        ci->cb_a4 = FUN_004396d0;
        ci->cb_ac = FUN_00439700;
        ci->cb_8c = FUN_00439720;
        ci->cb_a0 = FUN_0043a390;
        ci->cb_a8 = FUN_00439950;
        ci->cb_9c = FUN_0043a3d0;
        ci->cb_b0 = FUN_00439760;
        return;
    }
    // STRING: LEGOLAND 0x004b759c
    if (_stricmp("LEGO MEDIA SHOP", name->name) == 0) {
        ci->cb_a4 = FUN_00439c20;
        ci->cb_98 = FUN_00439c60;
        ci->cb_9c = FUN_00439c90;
        ci->cb_ac = FUN_00439ce0;
        ci->cb_8c = FUN_00439d00;
        ci->cb_a0 = FUN_0043a390;
        ci->cb_a8 = FUN_00439ef0;
        ci->cb_b0 = FUN_00439d40;
    }
}
