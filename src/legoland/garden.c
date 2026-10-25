#include "legoland.h"

#include <string.h>
#include "gamemap.h"
#include "garden.h"
#include "globals.h"
#include "llidb.h"
#include "map_object.h"
#include "objclass.h"

#pragma intrinsic(strcmp)

// FUNCTION: LEGOLAND 0x00432480
void LoadHedgeImages(struct Element *arg0) {
    struct Ride *temp = arg0->ride;
    HedgeObjectClass = temp;
    temp->flags |= 0x404;
    // STRING: LEGOLAND 0x004b7114
    if (LLIDB_FindElement("HEDGE IMAGES", &HedgeImagesHandle, 0) != 0) {
        return;
    }
    HedgeImagesData = LLIDB_LoadData((void *)HedgeImagesHandle);
}

// FUNCTION: LEGOLAND 0x004324c0
void UnloadHedgeImages(void) {
    LLIDB_UnLoadData(HedgeImagesHandle);
}

// FUNCTION: LEGOLAND 0x004324d0
void SetHedgeEditCursor(void) {
    void *var = HedgeObjectClass;
    EditMode.unk0 = 1;
    EditMode.unk8 = var;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00432510
void UpdateHedgeTileImage(int x, int y) {
    int pos[2];
    int mask = 0;

    pos[0] = x;
    pos[1] = y - 1;
    if (GetObjectClassAndInstance(pos, 0) == HedgeObjectClass) {
        mask = 1;
    }
    pos[0] = x + 1;
    pos[1] = y;
    if (GetObjectClassAndInstance(pos, 0) == HedgeObjectClass) {
        mask |= 2;
    }
    pos[0] = x;
    pos[1] = y + 1;
    if (GetObjectClassAndInstance(pos, 0) == HedgeObjectClass) {
        mask |= 4;
    }
    pos[0] = x - 1;
    pos[1] = y;
    if (GetObjectClassAndInstance(pos, 0) == HedgeObjectClass) {
        mask |= 8;
    }
    if (mask == 0) {
        Set_UserFlags(x << 8, y << 8, 0xe);
    } else {
        Set_UserFlags(x << 8, y << 8, mask - 1);
    }
}

// FUNCTION: LEGOLAND 0x004325e0
void HedgeAddObject(Element *obj, int *param_2) {
    TileId packed;
    int pos[2];
    packed.pos.x = param_2[0];
    packed.pos.y = param_2[1];
    AddObjectToMap(obj, packed, 0);
    UpdateHedgeTileImage(param_2[0], param_2[1]);
    pos[0] = param_2[0];
    pos[1] = param_2[1] - 1;
    if (GetObjectClassAndInstance(pos, 0) == HedgeObjectClass) {
        UpdateHedgeTileImage(pos[0], pos[1]);
    }
    pos[0] = param_2[0] + 1;
    pos[1] = param_2[1];
    if (GetObjectClassAndInstance(pos, 0) == HedgeObjectClass) {
        UpdateHedgeTileImage(pos[0], pos[1]);
    }
    pos[0] = param_2[0];
    pos[1] = param_2[1] + 1;
    if (GetObjectClassAndInstance(pos, 0) == HedgeObjectClass) {
        UpdateHedgeTileImage(pos[0], pos[1]);
    }
    pos[0] = param_2[0] - 1;
    pos[1] = param_2[1];
    if (GetObjectClassAndInstance(pos, 0) == HedgeObjectClass) {
        UpdateHedgeTileImage(pos[0], pos[1]);
    }
}

// FUNCTION: LEGOLAND 0x00432700
void HedgeRemoveObject(Element *obj, TileId tile, struct Cursor *cursor) {
    int pos[2];
    int x = tile.pos.x;
    int y = tile.pos.y;

    StandardRemoveObject(obj, tile, cursor);
    pos[0] = x;
    pos[1] = y - 1;
    if (GetObjectClassAndInstance(pos, 0) == HedgeObjectClass) {
        UpdateHedgeTileImage(pos[0], pos[1]);
    }
    pos[0] = x + 1;
    pos[1] = y;
    if (GetObjectClassAndInstance(pos, 0) == HedgeObjectClass) {
        UpdateHedgeTileImage(pos[0], pos[1]);
    }
    pos[0] = x;
    pos[1] = y + 1;
    if (GetObjectClassAndInstance(pos, 0) == HedgeObjectClass) {
        UpdateHedgeTileImage(pos[0], pos[1]);
    }
    pos[0] = x - 1;
    pos[1] = y;
    if (GetObjectClassAndInstance(pos, 0) == HedgeObjectClass) {
        UpdateHedgeTileImage(pos[0], pos[1]);
    }
}

// FUNCTION: LEGOLAND 0x00432810
struct RideSpriteInfo *GetHedgeSpriteInfo(struct Element *unused, TileId tile) {
    struct SpriteSet *t;
    int i;

    i = Get_UserFlags(tile.pos.x << 8, tile.pos.y << 8) & 0xffff;
    t = HedgeImagesData;
    RideSpriteInfoBuffer.sprite = t->sprites[(unsigned char)i];
    RideSpriteInfoBuffer.x = t->offset_x[(unsigned char)i] >> 1;
    RideSpriteInfoBuffer.y = t->offset_y[(unsigned char)i] >> 1;
    return &RideSpriteInfoBuffer;
}

// FUNCTION: LEGOLAND 0x00432870
void LoadFlowerImages(struct Element *param) {
    struct Ride *temp = param->ride;
    FlowersRide = temp;
    temp->flags |= 0x404;
    // STRING: LEGOLAND 0x004b7124
    if (LLIDB_FindElement("FLOWERS 1", &FlowerImagesHandle, 0) != 0) {
        return;
    }
    FlowerImagesData = LLIDB_LoadData((void *)FlowerImagesHandle);
}

// FUNCTION: LEGOLAND 0x004328b0
void UnloadFlowerImages(void) {
    LLIDB_UnLoadData(FlowerImagesHandle);
}

// FUNCTION: LEGOLAND 0x004328c0
void FlowersSetEditMode(void) {
    void *var = FlowersRide;
    EditMode.unk0 = 1;
    EditMode.unk8 = var;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00432900
void FlowersAddObject(struct Element *param_1, int *param_2) {
    TileId packed;
    packed.pos.x = param_2[0];
    packed.pos.y = param_2[1];
    AddObjectToMap(param_1, packed, 0);
    if (FlowerImagesData != 0) {
        int r = rand();
        Set_UserFlags(param_2[0] << 8, param_2[1] << 8, (unsigned short)(r % FlowerImagesData->count));
    }
}

// FUNCTION: LEGOLAND 0x00432960
struct RideSpriteInfo *GetFlowerSpriteInfo(struct Element *unused, TileId tile) {
    struct SpriteSet *t;
    int i;

    i = Get_UserFlags(tile.pos.x << 8, tile.pos.y << 8) & 0xffff;
    t = FlowerImagesData;
    RideSpriteInfoBuffer.sprite = t->sprites[(unsigned char)i];
    RideSpriteInfoBuffer.x = t->offset_x[(unsigned char)i] >> 1;
    RideSpriteInfoBuffer.y = t->offset_y[(unsigned char)i] >> 1;
    return &RideSpriteInfoBuffer;
}

// FUNCTION: LEGOLAND 0x004329c0
void GardenGetInterfaces(struct Element *head, struct Ride *iface) {
    // STRING: LEGOLAND 0x004b7138
    if (strcmp(head->name, "HEDGE") == 0) {
        iface->cb_a4 = LoadHedgeImages;
        iface->cb_8c = SetHedgeEditCursor;
        iface->cb_98 = HedgeAddObject;
        iface->cb_9c = HedgeRemoveObject;
        iface->cb_sprite = (struct RideSpriteInfo * (*)(struct Element *, unsigned short)) GetHedgeSpriteInfo; /* takes the tile id as a TileId */
        iface->cb_ac = UnloadHedgeImages;
        return;
    }
    // STRING: LEGOLAND 0x004b7130
    if (strcmp(head->name, "FLOWERS") == 0) {
        iface->cb_a4 = LoadFlowerImages;
        iface->cb_8c = FlowersSetEditMode;
        iface->cb_98 = FlowersAddObject;
        iface->cb_sprite = (struct RideSpriteInfo * (*)(struct Element *, unsigned short)) GetFlowerSpriteInfo; /* takes the tile id as a TileId */
        iface->cb_ac = UnloadFlowerImages;
    }
}
