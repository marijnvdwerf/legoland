#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "bloke.h"
#include "castle_level.h"
#include "gamemap.h"
#include "globals.h"
#include "map_object.h"
#include "obj_instance.h"
#include "objclass.h"

#include "image_sprite.h"
#include "man3d.h"
#include "math.h"
#include "print_sprite.h"
#include "render3d.h"

// FUNCTION: LEGOLAND 0x00402ca0
void LoadCastleLevelMatteSprite(Element *obj) {
    struct Ride *ride = obj->ride;

    CastleLevelRide = ride;
    if (ride != NULL) {
        ride->flags |= 0x20;
    }
    if (ride->layer != NULL) {
        ride->layer->flags |= 0x2000;
    }
    // STRING: LEGOLAND 0x004b4078
    CastleLevelMatteSprite = LoadSprite("Castle Matte.lls", 1);
}

// FUNCTION: LEGOLAND 0x00402ce0
void KillCastleLevelMatteSprite(void) {
    if (CastleLevelMatteSprite != 0) {
        KillSprite(CastleLevelMatteSprite);
    }
}

// FUNCTION: LEGOLAND 0x00402d00
void RenderCastleLevel(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = obj->ride;
    struct RideNode *node;
    struct Point pos;
    struct Point offset;

    RenderItems_New();
    CastleLevelRiderRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (*tile == node->tile.id) {
            AddBlokeToRenderList(&CastleLevelRiderRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&CastleLevelRiderRenderList);
    if (CastleLevelMatteSprite != NULL) {
        pos = GetScreenCoordsForObject((TileId *)tile, ride);
        offset = GetRenderOffsetForLayer((struct Sprite *)ride->layer, 2);
        AdjustOffsetForViewMode(&offset);
        PrintSprite(CastleLevelMatteSprite, pos.x + offset.x, pos.y + offset.y, clip, 0);
    }
}

// FUNCTION: LEGOLAND 0x00402dc0
void CastleLevel1Update(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node;
    struct RideNode *next;
    struct Bloke *bloke;
    unsigned int x;
    unsigned int y;
    int dx;
    int dy;
    char dir;

    for (node = ride->riders; node != NULL; node = next) {
        next = node->next;
        bloke = node->rider;
        x = node->tile.pos.x + ride->x;
        y = node->tile.pos.y + ride->y;
        if (bloke->low_level_action != 0) {
            continue;
        }
        switch (bloke->param_action) {
        case 0:
            bloke->flags |= 8;
            bloke->dest.x = (x - 7) << 8;
            bloke->dest.y = (y << 8) - 0x100;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->low_level_action = 7;
            bloke->target_dir = dir + 0x10;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 1:
            dx = rand() & 3;
            dy = rand() & 3;
            if (dx == 1) {
                dx = 0;
            } else if (dx == 0) {
                dx = -0x80;
            } else if (dx == 2) {
                dx = 0x80;
            }
            if (dy == 1) {
                dy = 0;
            } else if (dy == 0) {
                dy = -0x80;
            } else if (dy == 2) {
                dy = 0x80;
            }
            bloke->dest.x = ((x - 7) << 8) + dx;
            bloke->dest.y = (y << 8) + dy - 0x100;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->low_level_action = 7;
            bloke->target_dir = dir + 0x10;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 2:
            node->rider->field_58 = (rand() & 0x1f) + 0x10;
            bloke->dir = rand() & 7;
            bloke->field_3a = (rand() & 0xff) + 1;
            bloke->param_action++;
            break;
        case 3:
            if (--bloke->field_3a <= 0) {
                bloke->param_action++;
            }
            if (--bloke->field_58 <= 0) {
                bloke->field_58 = (rand() & 0x1f) + 0x10;
                bloke->dir = rand() & 7;
            }
            break;
        case 4:
            bloke->dest.x = (x << 8) - 0x680;
            bloke->dest.y = (y << 8) - 0x80;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->low_level_action = 7;
            bloke->target_dir = dir + 0x10;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 5:
            bloke->dest.x = (x << 8) + 0x80;
            bloke->dest.y = (y << 8) + 0x80;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->low_level_action = 7;
            bloke->target_dir = dir + 0x10;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 6:
            RemoveBlokeFromRide(ride, node);
            bloke->flags &= ~8;
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x00402ff0
void CastleLevelSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = CastleLevelRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&((struct Ride *)EditMode.unk8)->footprint);
}

// FUNCTION: LEGOLAND 0x00403030
void CastleLevelRemoveObject(Element *obj, TileId tile, struct Cursor *cursor) {
    StandardRemoveObject(obj, tile, cursor);
    RemoveAllBlokesFromRide(obj->ride, tile);
}

// FUNCTION: LEGOLAND 0x00403060
void CastleLevelAddObject(struct Element *param1, int *param2) {
    AddBasicObject(param1, param2);
}

// FUNCTION: LEGOLAND 0x00403080
void CastleLevel1_GetInterfaces(struct Element *name, struct Ride *ci) {
    // STRING: LEGOLAND 0x004b408c
    if (_stricmp("CASTLE LEVEL 1", name->name) == 0) {
        ci->cb_a4 = LoadCastleLevelMatteSprite;
        ci->cb_ac = KillCastleLevelMatteSprite;
        ci->cb_8c = CastleLevelSetEditMode;
        ci->cb_98 = CastleLevelAddObject;
        ci->cb_9c = CastleLevelRemoveObject;
        ci->cb_pre_render = CastleLevel1Update;
        ci->cb_b0 = RenderCastleLevel;
    }
}
