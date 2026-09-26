#include <string.h>
#include "legoland.h"

#include "gamemap.h"
#include "globals.h"
#include "map_object.h"
#include "obj_instance.h"
#include "objclass.h"
#include "temple.h"

#include "bloke.h"
#include "image_sprite.h"
#include "man3d.h"
#include "math.h"
#include "print_sprite.h"
#include "render3d.h"
#include "ride_queue.h"

// FUNCTION: LEGOLAND 0x004169c0
void FUN_004169c0(struct RideObject *obj) {
    DAT_004cbf5c = obj->ride;
    if (DAT_004cbf5c != NULL) {
        DAT_004cbf5c->flags |= 0x20;
        if (DAT_004cbf5c->layer != NULL) {
            DAT_004cbf5c->layer->flags |= 0x2000;
            DAT_004cbf64 = DAT_004cbf5c->layer;
        }
    }
    // STRING: LEGOLAND 0x004b4edc
    DAT_004cbf68 = LoadSprite("temple_matte1.lls", 1);
    // STRING: LEGOLAND 0x004b4ec8
    DAT_004cbf6c = LoadSprite("temple_matte2.lls", 1);
}

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
void FUN_00416a60(struct RideObject *obj, unsigned int param_2, unsigned int param_3, unsigned short *coords, unsigned int param_5, unsigned int clip) {
    struct Ride *ride = obj->ride;
    struct RideNode *node;
    struct Point pos;
    struct Point offset;

    RenderItems_New();
    DAT_004cbf70 = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (*coords == node->tile.id) {
            AddBlokeToRenderList(&DAT_004cbf70, (struct BlokeRenderSrc *)node, node->person->field_20);
        }
    }
    RenderBlokeList((struct BlokeListHead *)&DAT_004cbf70);
    pos = GetScreenCoordsForObject((unsigned char *)coords, ride);
    offset = GetRenderOffsetForLayer(DAT_004cbf64, 0);
    AdjustOffsetForViewMode(&offset);
    PrintSprite(DAT_004cbf68, offset.x + pos.x, offset.y + pos.y, clip, 0);
    offset = GetRenderOffsetForLayer(DAT_004cbf64, 3);
    AdjustOffsetForViewMode(&offset);
    PrintSprite(DAT_004cbf6c, offset.x + pos.x, offset.y + pos.y, clip, 0);
}

// FUNCTION: LEGOLAND 0x00416b50
void FUN_00416b50(struct RideObject *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node;
    struct RideNode *next;
    struct Bloke *bloke;
    unsigned int x;
    unsigned int y;
    char dir;

    for (node = ride->riders; node != NULL; node = next) {
        next = node->next;
        bloke = node->rider;
        x = ride->x + node->tile.pos.x;
        y = ride->y + node->tile.pos.y;
        if (bloke->field_e == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                bloke->dest.x = (x - 2) << 8;
                bloke->dest.y = (y - 4) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->field_73 = dir + 0x10;
                bloke->field_e = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 1:
                bloke->dest.x = (x - 2) << 8;
                bloke->dest.y = (y << 8) - 0x680;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->field_73 = dir + 0x10;
                bloke->field_e = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 2:
                bloke->dest.x = (x << 8) - 0x260;
                bloke->dest.y = (y - 8) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->field_73 = dir + 0x10;
                bloke->field_e = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 3:
                bloke->dest.x = (x - 2) << 8;
                bloke->dest.y = (y << 8) - 0x980;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->field_73 = dir + 0x10;
                bloke->field_e = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 4:
                bloke->dest.x = (x << 8) - 0x260;
                bloke->dest.y = (y - 12) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->field_73 = dir + 0x10;
                bloke->field_e = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 5:
                bloke->dest.x = (x - 2) << 8;
                bloke->dest.y = (y << 8) - 0x980;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->field_73 = dir + 0x10;
                bloke->field_e = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 6:
                bloke->dest.x = (x << 8) - 0x260;
                bloke->dest.y = (y - 8) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->field_73 = dir + 0x10;
                bloke->field_e = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 7:
                bloke->dest.x = (x - 2) << 8;
                bloke->dest.y = (y << 8) - 0x680;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->field_73 = dir + 0x10;
                bloke->field_e = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 8:
                bloke->dest.y = (y - 4) << 8;
                bloke->dest.x = (x - 2) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->field_73 = dir + 0x10;
                bloke->field_e = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 9:
                bloke->dest.x = (x << 8) + 0x80;
                bloke->dest.y = (y << 8) + 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->field_73 = dir + 0x10;
                bloke->field_e = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 10:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= ~8;
                break;
            }
        }
    }
}

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
void FUN_00416e20(struct RideObject *a1, TileId tile, unsigned int a3) {
    StandardRemoveObject((unsigned int)a1, tile, a3);
    RemoveAllBlokesFromRide(a1->ride, tile);
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
