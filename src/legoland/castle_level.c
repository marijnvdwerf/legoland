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
void FUN_00402ca0(struct RideObject *obj) {
    struct Ride *ride = obj->ride;

    DAT_004c10dc = ride;
    if (ride != NULL) {
        ride->flags |= 0x20;
    }
    if (ride->layer != NULL) {
        ride->layer->flags |= 0x2000;
    }
    // STRING: LEGOLAND 0x004b4078
    DAT_004c10e4 = LoadSprite("Castle Matte.lls", 1);
}

// FUNCTION: LEGOLAND 0x00402ce0
void FUN_00402ce0(void) {
    if (DAT_004c10e4 != 0) {
        KillSprite(DAT_004c10e4);
    }
}

// FUNCTION: LEGOLAND 0x00402d00
void FUN_00402d00(struct RideObject *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, unsigned int param_5, unsigned int clip) {
    struct Ride *ride = obj->ride;
    struct RideNode *node;
    struct Point pos;
    struct Point offset;

    RenderItems_New();
    DAT_004c10e8 = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (*tile == node->tile.id) {
            AddBlokeToRenderList(&DAT_004c10e8, (struct BlokeRenderSrc *)node, node->person->field_20);
        }
    }
    RenderBlokeList((struct BlokeListHead *)&DAT_004c10e8);
    if (DAT_004c10e4 != NULL) {
        pos = GetScreenCoordsForObject((unsigned char *)tile, ride);
        offset = GetRenderOffsetForLayer((struct LayerOffsetHolder *)ride->layer, 2);
        AdjustOffsetForViewMode(&offset);
        PrintSprite(DAT_004c10e4, pos.x + offset.x, pos.y + offset.y, clip, 0);
    }
}

// FUNCTION: LEGOLAND 0x00402dc0
void FUN_00402dc0(struct RideObject *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node;
    struct RideNode *next;
    struct Bloke *bloke;
    unsigned int x;
    unsigned int y;
    int dx;
    int dy;
    struct Point to;
    char dir;

    for (node = ride->riders; node != NULL; node = next) {
        next = node->next;
        bloke = node->rider;
        x = node->tile.pos.x + ride->x;
        y = node->tile.pos.y + ride->y;
        if (bloke->field_e != 0) {
            continue;
        }
        switch (bloke->param_action) {
        case 0:
            bloke->flags |= 8;
            bloke->dest.x = (x - 7) << 8;
            bloke->dest.y = (y << 8) - 0x100;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->field_e = 7;
            bloke->field_73 = dir + 0x10;
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
            bloke->field_e = 7;
            bloke->field_73 = dir + 0x10;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 2:
            node->rider->field_58 = (rand() & 0x1f) + 0x10;
            bloke->field_72 = rand() & 7;
            bloke->field_3a = (rand() & 0xff) + 1;
            bloke->param_action++;
            break;
        case 3:
            if (--bloke->field_3a <= 0) {
                bloke->param_action++;
            }
            if (--bloke->field_58 <= 0) {
                bloke->field_58 = (rand() & 0x1f) + 0x10;
                bloke->field_72 = rand() & 7;
            }
            break;
        case 4:
            to.x = (x << 8) - 0x680;
            to.y = (y << 8) - 0x80;
            bloke->dest = to;
            dir = CalcMoveLine(bloke->pos, to, &bloke->nav);
            bloke->field_e = 7;
            bloke->field_73 = dir + 0x10;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 5:
            to.x = (x << 8) + 0x80;
            to.y = (y << 8) + 0x80;
            bloke->dest = to;
            dir = CalcMoveLine(bloke->pos, to, &bloke->nav);
            bloke->field_e = 7;
            bloke->field_73 = dir + 0x10;
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
void FUN_00402ff0(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = DAT_004c10dc;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&((struct EditCursorData *)EditMode.unk8)->field_3c);
}

// FUNCTION: LEGOLAND 0x00403030
void FUN_00403030(struct RideObject *obj, TileId tile, struct Cursor *cursor) {
    StandardRemoveObject((struct RideObject *)obj, tile, cursor);
    RemoveAllBlokesFromRide(obj->ride, tile);
}

// FUNCTION: LEGOLAND 0x00403060
void FUN_00403060(unsigned int param1, unsigned int param2) {
    AddBasicObject(param1, param2);
}

// FUNCTION: LEGOLAND 0x00403080
void FUN_00403080(struct ClassNode *name, struct CallbackTable *ci) {
    // STRING: LEGOLAND 0x004b408c
    if (_stricmp("CASTLE LEVEL 1", name->name) == 0) {
        ci->cb_a4 = FUN_00402ca0;
        ci->cb_ac = FUN_00402ce0;
        ci->cb_8c = FUN_00402ff0;
        ci->cb_98 = FUN_00403060;
        ci->cb_9c = FUN_00403030;
        ci->cb_a8 = FUN_00402dc0;
        ci->cb_b0 = FUN_00402d00;
    }
}
