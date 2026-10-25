#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "gamemap.h"
#include "jungle_cruise.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "money.h"
#include "obj_instance.h"
#include "objclass.h"
#include "path_control.h"
#include "print_sprite.h"
#include "render3d.h"
#include "shops.h"
#include "tilemap.h"
#include "western_town.h"

struct Coords {
    int x;
    int y;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x00439200
void LoadLegoShop1MatteSpriteAndMoneySFX(struct Element *obj) {
    struct Ride *building = obj->ride;
    LegoShop1Building = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b751c
    LegoShop1MatteSprite = LoadSprite("Lego Shop 1 Matte.LLS", 1);
    LoadMoneySFX();
}

/* Adds the current path graphics to every tile in the shop footprint translated by the supplied origin. */
// FUNCTION: LEGOLAND 0x00439230
void AddShopFootprintPathGraphics(struct Ride *area, struct Point *origin) {
    int xend = area->footprint.x1 + origin->x;
    int x = area->footprint.x0 + origin->x;
    int y = area->footprint.y0 + origin->y;
    int yend = area->footprint.y1 + origin->y;

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
        y = area->footprint.y0 + origin->y;
        x++;
    } while (x <= xend);
}

/* Removes roller-coaster paths from every tile in the shop footprint translated by the supplied origin. */
// FUNCTION: LEGOLAND 0x004392b0
void RemoveShopFootprintCoasterPaths(struct Ride *area, struct Point *origin) {
    int xend = area->footprint.x1 + origin->x;
    int x = area->footprint.x0 + origin->x;
    int y = area->footprint.y0 + origin->y;
    int yend = area->footprint.y1 + origin->y;

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
        y = area->footprint.y0 + origin->y;
        x++;
    } while (x <= xend);
}

// FUNCTION: LEGOLAND 0x00439320
void LegoShop1AddObject(struct Element *obj, void *param_2) {
    struct Ride *building = obj->ride;
    AddBasicObject(obj, param_2);
    AddShopFootprintPathGraphics((struct Ride *)building, param_2);
}

// FUNCTION: LEGOLAND 0x00439350
void LegoShop1RemoveObject(struct Element *obj, TileId coords, void *cursor) {
    void *ride = obj->data;
    struct Point local;

    StandardRemoveObject(obj, coords, cursor);
    RemoveAllBlokesFromRide((struct Ride *)ride, coords);

    local.x = coords.pos.x;
    local.y = coords.pos.y;
    RemoveShopFootprintCoasterPaths((struct Ride *)ride, &local);
}

// FUNCTION: LEGOLAND 0x004393a0
void LegoShop1SetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = LegoShop1Building;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x004393e0
void UnloadLegoShop1MatteSpriteAndMoneySFX(void) {
    if (LegoShop1MatteSprite != 0) {
        KillSprite(LegoShop1MatteSprite);
    }
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00439400
void RenderLegoShop1(struct Element *obj, unsigned int param2, unsigned int param3, unsigned short *ride, void *param5, unsigned int param1) {
    struct Ride *building = obj->ride;
    struct RideNode *node = building->riders;
    int count = 0;

    while (node != 0) {
        if (*ride == node->tile.id) {
            RenderBlokeIn3D(node->rider);
            count++;
        }
        node = node->next;
    }

    if (count != 0) {
        struct Point q = GetScreenCoordsForObject((TileId *)ride, building);
        struct Coords *coords = (struct Coords *)&q;
        PrintSprite(LegoShop1MatteSprite, coords->x, coords->y, param1, (int *)node);
    }
}

// FUNCTION: LEGOLAND 0x00439460
void LegoShop1Update(Element *obj) {
    Ride *ride = obj->ride;
    RideNode *node = ride->riders;
    RideNode *next;
    Bloke *bloke;
    TileId *tile;
    unsigned int x;
    unsigned int y;
    unsigned char dir;

    while (node != NULL) {
        next = node->next;
        bloke = node->rider;
        tile = &node->tile;
        x = tile->pos.x + ride->x;
        y = tile->pos.y + ride->y;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                x = (x - 5) << 8;
                y <<= 8;
                bloke->dest.x = x;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 1:
                bloke->dest.x = (rand() % 2 + x - 5) << 8;
                bloke->dest.y = (rand() % 2 + y + 3) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->field_58 = rand() % 50;
                bloke->param_action++;
                break;
            case 2:
                bloke->field_58--;
                if (bloke->field_58 == 0) {
                    bloke->param_action++;
                } else if (bloke->field_58 % 10 == 0) {
                    bloke->dir++;
                    if (bloke->dir > 8) {
                        bloke->dir = 0;
                    }
                }
                break;
            case 3:
                bloke->dest.x = (x - 5) << 8;
                bloke->dest.y = (rand() % 3 + y) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->field_58 = rand() % 30;
                bloke->param_action++;
                break;
            case 4:
                BrowseAndBuyItem(node, obj, tile, 1);
                break;
            case 5:
                x <<= 8;
                bloke->dest.x = x;
                y <<= 8;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 6:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= ~8;
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x004396d0
void LoadLegoShop2MatteSpriteAndMoneySFX(struct Element *obj) {
    struct Ride *building = obj->ride;
    LegoShop2Building = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b7534
    LegoShop2MatteSprite = LoadSprite("Lego Shop 2 Matte.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x00439700
void UnloadLegoShop2MatteSpriteAndMoneySFX(void) {
    if (LegoShop2MatteSprite != 0) {
        KillSprite(LegoShop2MatteSprite);
    }
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00439720
void LegoShop2SetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = LegoShop2Building;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00439760
void RenderLegoShop2(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int param_6) {
    struct Ride *shop = obj->ride;
    struct RideNode *node = shop->riders;
    struct Bloke *blokes[10] = {0};
    char count = 0;
    char i;
    struct Point off;
    struct Point pos;

    while (node != NULL) {
        if (*tile == node->tile.id) {
            blokes[count++] = node->rider;
        }
        node = node->next;
    }
    if (count == 0) {
        return;
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 4) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 5) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 3) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 2) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 9) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 10) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 11) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    off = GetRenderOffsetForLayer(shop->layer, 0);
    pos = GetScreenCoordsForObject((TileId *)tile, shop);
    AdjustOffsetForViewMode(&off);
    PrintSprite(LegoShop2MatteSprite, pos.x + off.x, pos.y + off.y, param_6, 0);
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 1) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 12) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
}

// FUNCTION: LEGOLAND 0x00439950
void LegoShop2Update(Element *obj) {
    Ride *ride = obj->ride;
    RideNode *node = ride->riders;
    RideNode *next;
    Bloke *bloke;
    TileId *tile;
    unsigned int x;
    unsigned int y;
    unsigned char dir;
    int r;

    while (node != NULL) {
        next = node->next;
        tile = &node->tile;
        bloke = node->rider;
        x = tile->pos.x + ride->x;
        y = tile->pos.y + ride->y;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                x = (x << 8) - 0x80;
                y <<= 8;
                bloke->dest.x = x;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 2:
                x = (x << 8) - 0x160;
                y = (y << 8) - 0x60;
                bloke->dest.x = x;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 3:
                /* walk to one of three browse spots: (x - 3, y), (x - 1, y - 2) or (x - 3, y - 2.5);
                 * case 5 puts the visitor back on the same three */
                r = rand() % 3;
                if (r == 2) {
                    x = (x - 3) << 8;
                    y <<= 8;
                    bloke->dest.x = x;
                    bloke->dest.y = y;
                    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->low_level_action = 7;
                    bloke->target_dir = dir + 0x10;
                    NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                    bloke->param_action++;
                    break;
                } else if (r == 1) {
                    x = (x << 8) - 0x100;
                    y = (y - 2) << 8;
                    bloke->dest.x = x;
                    bloke->dest.y = y;
                } else {
                    x = (x - 3) << 8;
                    y = (y << 8) - 0x280;
                    bloke->dest.x = x;
                    bloke->dest.y = y;
                }
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 4:
                BrowseAndBuyItem(node, obj, tile, 1);
                break;
            case 5:
                r = rand() % 3;
                if (r == 2) {
                    x = (x - 3) << 8;
                    y <<= 8;
                    bloke->pos.x = x;
                    bloke->pos.y = y;
                    bloke->field_58 = rand() % 50;
                    bloke->param_action = 8;
                } else if (r == 1) {
                    x = (x << 8) - 0x100;
                    y = (y - 2) << 8;
                    bloke->pos.x = x;
                    bloke->pos.y = y;
                    bloke->field_58 = rand() % 50;
                    bloke->param_action = 8;
                } else {
                    x = (x - 3) << 8;
                    y = (y << 8) - 0x280;
                    bloke->pos.x = x;
                    bloke->pos.y = y;
                    bloke->param_action = 9;
                }
                break;
            case 8:
                bloke->field_58--;
                if (bloke->field_58 == 0) {
                    bloke->param_action++;
                }
                break;
            case 1:
            case 9:
                x = (x << 8) - 0x100;
                y = (y << 8) - 0x80;
                bloke->dest.x = x;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 10:
                x = (x << 8) - 0x80;
                y <<= 8;
                bloke->dest.x = x;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 11:
                x <<= 8;
                y <<= 8;
                bloke->dest.x = x;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
            case 12:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= ~8;
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x00439c20
void LoadLegMediaShopMaskSpritesAndMoneySFX(struct Element *obj) {
    struct Ride *building = obj->ride;
    LegoMediaShopBuilding = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b7564
    LegMediaShopMask1Sprite = LoadSprite("LegMediaShopMask1.LLS", 1);
    // STRING: LEGOLAND 0x004b754c
    LegMediaShopMask2Sprite = LoadSprite("LegMediaShopMask2.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x00439c60
void LegoMediaShopAddObject(struct Element *obj, void *param_2) {
    struct Ride *building = obj->ride;
    AddBasicObject(obj, param_2);
    AddShopFootprintPathGraphics((struct Ride *)building, param_2);
}

// FUNCTION: LEGOLAND 0x00439c90
void LegoMediaShopRemoveObject(struct Element *obj, TileId coords, void *cursor) {
    void *ride = obj->data;
    struct Point local;

    StandardRemoveObject(obj, coords, cursor);
    RemoveAllBlokesFromRide((struct Ride *)ride, coords);

    local.x = coords.pos.x;
    local.y = coords.pos.y;
    RemoveShopFootprintCoasterPaths((struct Ride *)ride, &local);
}

// FUNCTION: LEGOLAND 0x00439ce0
void UnloadLegMediaShopMaskSpritesAndMoneySFX(void) {
    KillSprite(LegMediaShopMask1Sprite);
    KillSprite(LegMediaShopMask2Sprite);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00439d00
void LegoMediaShopSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = LegoMediaShopBuilding;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00439d40
void RenderLegoMediaShop(struct Element *obj, unsigned int param2, unsigned int param3, unsigned short *ride, void *param5, unsigned int param1) {
    struct Ride *building = obj->ride;
    char count = 0;
    char i;
    struct RideNode *node = building->riders;
    struct Bloke *blokes[10] = {0};
    struct Point pos;

    while (node != 0) {
        if (*ride == node->tile.id) {
            blokes[count++] = node->rider;
        }
        node = node->next;
    }
    if (count == 0) {
        return;
    }
    pos = GetScreenCoordsForObject((TileId *)ride, building);
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 2) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 3) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 4) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 5) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    PrintSprite(LegMediaShopMask2Sprite, pos.x, pos.y, param1, 0);
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 0) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 1) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 6) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    PrintSprite(LegMediaShopMask1Sprite, pos.x, pos.y, param1, 0);
}

/* Advances the LEGO Media shop visitor state machine: enters, chooses a browsing position, buys an item, and walks
 * back out before removing the ride node. */
// FUNCTION: LEGOLAND 0x00439ef0
void LegoMediaShopUpdate(Element *obj) {
    Ride *ride = obj->ride;
    RideNode *node = ride->riders;
    RideNode *next;
    Bloke *bloke;
    TileId *tile;
    unsigned int x;
    unsigned int y;
    unsigned char dir;
    char r;

    while (node != NULL) {
        next = node->next;
        bloke = node->rider;
        tile = &node->tile;
        x = tile->pos.x + ride->x;
        y = tile->pos.y + ride->y;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                x = (x - 2) << 8;
                y <<= 8;
                bloke->dest.x = x;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 1:
                r = rand() % 3;
                if (r == 2) {
                    x = (x - 4) << 8;
                    y <<= 8;
                    bloke->dest.x = x;
                    bloke->dest.y = y;
                } else if (r == 1) {
                    bloke->dest.x = (x - 3) << 8;
                    y = (y - 2) << 8;
                    bloke->dest.y = y;
                } else {
                    x = (x - 4) << 8;
                    y = (y - 2) << 8;
                    bloke->dest.x = x;
                    bloke->dest.y = y;
                }
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 2:
                BrowseAndBuyItem(node, obj, tile, 1);
                break;
            case 3:
                r = rand() % 3;
                if (r == 2) {
                    bloke->pos.x = (x - 4) << 8;
                    bloke->pos.y = y << 8;
                    bloke->param_action++;
                } else if (r == 1) {
                    bloke->pos.x = (x - 3) << 8;
                    bloke->pos.y = (y - 2) << 8;
                    bloke->param_action++;
                } else {
                    bloke->pos.x = (x - 4) << 8;
                    bloke->pos.y = (y - 2) << 8;
                    bloke->param_action++;
                }
                break;
            case 4:
                x = (x - 2) << 8;
                y <<= 8;
                bloke->dest.x = x;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 5:
                x <<= 8;
                bloke->dest.x = x;
                y <<= 8;
                bloke->dest.y = y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 6:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= ~8;
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x0043a0f0
void LoadExplorersInstituteMatteSpriteAndMoneySFX(struct Element *obj) {
    struct Ride *building = obj->ride;
    ExplorersInstituteBuilding = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b757c
    ExplorersInstituteMatteSprite = LoadSprite("Explorers Institute Matte.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0043a120
void UnloadExplorersInstituteMatteSpriteAndMoneySFX(void) {
    KillSprite(ExplorersInstituteMatteSprite);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x0043a140
void ExplorersInstituteSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = ExplorersInstituteBuilding;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x0043a180
void RenderExplorersInstitute(struct Element *obj, unsigned int param2, unsigned int param3, unsigned short *ride, void *param5, unsigned int param1) {
    struct Ride *building = obj->ride;
    struct RideNode *node = building->riders;
    int count = 0;

    while (node != 0) {
        if (*ride == node->tile.id) {
            RenderBlokeIn3D(node->rider);
            count++;
        }
        node = node->next;
    }

    if (count != 0) {
        struct Point q = GetScreenCoordsForObject((TileId *)ride, building);
        struct Coords *coords = (struct Coords *)&q;
        PrintSprite(ExplorersInstituteMatteSprite, coords->x, coords->y, param1, (int *)node);
    }
}

// FUNCTION: LEGOLAND 0x0043a1e0
void ExplorersInstituteUpdate(struct Element *obj) {
    unsigned int ptmp54;
    int ptmp55;
    volatile int ptmp911;
    struct Ride *ride = obj->ride;
    struct RideNode *elem = ride->riders;
    struct RideNode *next;
    int x;
    struct Bloke *bloke;
    unsigned int y;

    while (elem != NULL) {
        next = elem->next;
        bloke = elem->rider;
        if ((int)0 == bloke->low_level_action) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                bloke->dest.x = elem->tile.pos.x << 8;
                bloke->dest.y = (1 + elem->tile.pos.y) << 8;
                bloke->target_dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
                bloke->low_level_action = 7;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 1:
                bloke->dest.x = elem->tile.pos.x << 8;
                bloke->dest.y = (elem->tile.pos.y + 2) << 8;
                bloke->target_dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
                bloke->low_level_action = 7;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                bloke->field_58 = rand() % 70 + 20;
                break;
            case 2:
                if (bloke->field_58 == 0) {
                    bloke->param_action++;
                }
                bloke->field_58--;
                break;
            case 3:
                bloke->dest.x = elem->tile.pos.x << 8;
                bloke->dest.y = (1 + elem->tile.pos.y) << 8;
                bloke->target_dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
                bloke->low_level_action = 7;
                ptmp55 = bloke->target_dir;
                NewDirForAction(bloke, (ptmp55 >> 5) + 3);
                bloke->param_action++;
                break;
            case 4:
                ptmp911 = elem->tile.pos.y;
                bloke->dest.y = ((ride->y + ptmp911) << 8) + 0x80;
                ptmp54 = elem->tile.pos.x;
                bloke->dest.x = ((ride->x + ptmp54) << 8) + 0x80;
                bloke->target_dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
                bloke->low_level_action = 7;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 5:
                RemoveBlokeFromRide(ride, elem);
                bloke->flags &= 0xfff7;
                break;
            }
        }
        elem = next;
    }
}

// FUNCTION: LEGOLAND 0x0043a390
struct RideSpriteInfo *GetShopSpriteInfo(struct Element *obj, unsigned short param_2) {
    struct Ride *building = obj->ride;
    RideSpriteInfoBuffer.sprite = building->layer;
    RideSpriteInfoBuffer.x = building->field_14;
    RideSpriteInfoBuffer.y = building->field_18;
    RideSpriteInfoBuffer.id = param_2;
    building->layer->flags |= 0x2000;
    return &RideSpriteInfoBuffer;
}

// FUNCTION: LEGOLAND 0x0043a3d0
void RemoveObjectAndBlokes(struct Element *obj, TileId tile, void *param_3) {
    StandardRemoveObject(obj, tile, param_3);
    RemoveAllBlokesFromRide(obj->ride, tile);
}

// FUNCTION: LEGOLAND 0x0043a400
void ShopsGetInterfaces(struct Element *name, struct Ride *ci) {
    // STRING: LEGOLAND 0x004b75fc
    if (_stricmp("GENERAL STORE", name->name) == 0) {
        ci->cb_a4 = LoadGStoreMatteSpritesAndMoneySFX;
        ci->cb_ac = KillGStoreMatteSpritesAndMoneySFX;
        ci->cb_8c = GeneralStoreSetEditMode;
        ci->cb_sprite = GetShopSpriteInfo;
        ci->cb_pre_render = GeneralStoreUpdate;
        ci->cb_9c = RemoveObjectAndBlokes;
        ci->cb_b0 = RenderGeneralStore;
        return;
    }
    // STRING: LEGOLAND 0x004b75f4
    if (_stricmp("SHERIFF", name->name) == 0) {
        ci->cb_a4 = LoadSherifshutMatteSpriteAndMoneySFX;
        ci->cb_ac = KillSherifshutMatteSpriteAndMoneySFX;
        ci->cb_8c = SheriffSetEditMode;
        ci->cb_sprite = GetShopSpriteInfo;
        ci->cb_pre_render = SheriffUpdate;
        ci->cb_9c = RemoveObjectAndBlokes;
        ci->cb_b0 = RenderSheriff;
        return;
    }
    // STRING: LEGOLAND 0x004b75e8
    if (_stricmp("JAIL CELL", name->name) == 0) {
        ci->cb_a4 = JailCellLoad;
        ci->cb_ac = JailCellUnload;
        ci->cb_8c = JailCellSetEditMode;
        ci->cb_sprite = GetShopSpriteInfo;
        ci->cb_pre_render = JailCellUpdate;
        ci->cb_98 = JailCellAddObject;
        ci->cb_9c = JailCellRemoveObject;
        ci->cb_b0 = RenderJailCell;
        ci->load_hook = LoadJailCells;
        ci->save_hook = SaveJailCells;
        return;
    }
    // STRING: LEGOLAND 0x004b75e0
    if (_stricmp("BANK", name->name) == 0) {
        ci->cb_a4 = LoadBankMatteSpriteAndMoneySFX;
        ci->cb_ac = KillBankMatteSpriteAndMoneySFX;
        ci->cb_8c = BankSetEditMode;
        ci->cb_sprite = GetShopSpriteInfo;
        ci->cb_pre_render = BankUpdate;
        ci->cb_9c = RemoveObjectAndBlokes;
        ci->cb_b0 = RenderBank;
        return;
    }
    // STRING: LEGOLAND 0x004b75d8
    if (_stricmp("SALOON", name->name) == 0) {
        ci->cb_a4 = LoadSaloonMatteSpritesAndMoneySFX;
        ci->cb_ac = KillSaloonMatteSpritesAndMoneySFX;
        ci->cb_8c = SaloonSetEditMode;
        ci->cb_sprite = GetShopSpriteInfo;
        ci->cb_pre_render = SaloonUpdate;
        ci->cb_9c = RemoveObjectAndBlokes;
        ci->cb_b0 = RenderSaloon;
        return;
    }
    // STRING: LEGOLAND 0x004b75c4
    if (_stricmp("EXPLORERS INSTITUTE", name->name) == 0) {
        ci->cb_a4 = LoadExplorersInstituteMatteSpriteAndMoneySFX;
        ci->cb_ac = UnloadExplorersInstituteMatteSpriteAndMoneySFX;
        ci->cb_8c = ExplorersInstituteSetEditMode;
        ci->cb_sprite = GetShopSpriteInfo;
        ci->cb_pre_render = ExplorersInstituteUpdate;
        ci->cb_9c = RemoveObjectAndBlokes;
        ci->cb_b0 = RenderExplorersInstitute;
        return;
    }
    // STRING: LEGOLAND 0x004b75b8
    if (_stricmp("LEGO SHOP 1", name->name) == 0) {
        ci->cb_a4 = LoadLegoShop1MatteSpriteAndMoneySFX;
        ci->cb_98 = LegoShop1AddObject;
        ci->cb_9c = LegoShop1RemoveObject;
        ci->cb_ac = UnloadLegoShop1MatteSpriteAndMoneySFX;
        ci->cb_8c = LegoShop1SetEditMode;
        ci->cb_sprite = GetShopSpriteInfo;
        ci->cb_pre_render = LegoShop1Update;
        ci->cb_b0 = RenderLegoShop1;
        return;
    }
    // STRING: LEGOLAND 0x004b75ac
    if (_stricmp("LEGO SHOP 2", name->name) == 0) {
        ci->cb_a4 = LoadLegoShop2MatteSpriteAndMoneySFX;
        ci->cb_ac = UnloadLegoShop2MatteSpriteAndMoneySFX;
        ci->cb_8c = LegoShop2SetEditMode;
        ci->cb_sprite = GetShopSpriteInfo;
        ci->cb_pre_render = LegoShop2Update;
        ci->cb_9c = RemoveObjectAndBlokes;
        ci->cb_b0 = RenderLegoShop2;
        return;
    }
    // STRING: LEGOLAND 0x004b759c
    if (_stricmp("LEGO MEDIA SHOP", name->name) == 0) {
        ci->cb_a4 = LoadLegMediaShopMaskSpritesAndMoneySFX;
        ci->cb_98 = LegoMediaShopAddObject;
        ci->cb_9c = LegoMediaShopRemoveObject;
        ci->cb_ac = UnloadLegMediaShopMaskSpritesAndMoneySFX;
        ci->cb_8c = LegoMediaShopSetEditMode;
        ci->cb_sprite = GetShopSpriteInfo;
        ci->cb_pre_render = LegoMediaShopUpdate;
        ci->cb_b0 = RenderLegoMediaShop;
    }
}
