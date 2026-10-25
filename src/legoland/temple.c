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
void LoadTempleMatteSprites(Element *obj) {
    TempleBuildingRide = obj->ride;
    if (TempleBuildingRide != NULL) {
        TempleBuildingRide->flags |= 0x20;
        if (TempleBuildingRide->layer != NULL) {
            TempleBuildingRide->layer->flags |= 0x2000;
            TempleLayer = TempleBuildingRide->layer;
        }
    }
    // STRING: LEGOLAND 0x004b4edc
    TempleMatte1Sprite = LoadSprite("temple_matte1.lls", 1);
    // STRING: LEGOLAND 0x004b4ec8
    TempleMatte2Sprite = LoadSprite("temple_matte2.lls", 1);
}

// FUNCTION: LEGOLAND 0x00416a30
void KillTempleMatteSprites(void) {
    if (TempleMatte1Sprite != 0) {
        KillSprite(TempleMatte1Sprite);
    }
    if (TempleMatte2Sprite != 0) {
        KillSprite(TempleMatte2Sprite);
    }
}

// FUNCTION: LEGOLAND 0x00416a60
void RenderTemple(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *coords, void *param_5, unsigned int clip) {
    struct Ride *ride = obj->ride;
    struct RideNode *node;
    struct Point pos;
    struct Point offset;

    RenderItems_New();
    TempleRiderRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (*coords == node->tile.id) {
            AddBlokeToRenderList(&TempleRiderRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&TempleRiderRenderList);
    pos = GetScreenCoordsForObject((TileId *)coords, ride);
    offset = GetRenderOffsetForLayer(TempleLayer, 0);
    AdjustOffsetForViewMode(&offset);
    PrintSprite(TempleMatte1Sprite, offset.x + pos.x, offset.y + pos.y, clip, 0);
    offset = GetRenderOffsetForLayer(TempleLayer, 3);
    AdjustOffsetForViewMode(&offset);
    PrintSprite(TempleMatte2Sprite, offset.x + pos.x, offset.y + pos.y, clip, 0);
}

// FUNCTION: LEGOLAND 0x00416b50
void TempleUpdate(Element *obj) {
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
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                bloke->dest.x = (x - 2) << 8;
                bloke->dest.y = (y - 4) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->target_dir = dir + 0x10;
                bloke->low_level_action = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 1:
                bloke->dest.x = (x - 2) << 8;
                bloke->dest.y = (y << 8) - 0x680;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->target_dir = dir + 0x10;
                bloke->low_level_action = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 2:
                bloke->dest.x = (x << 8) - 0x260;
                bloke->dest.y = (y - 8) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->target_dir = dir + 0x10;
                bloke->low_level_action = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 3:
                bloke->dest.x = (x - 2) << 8;
                bloke->dest.y = (y << 8) - 0x980;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->target_dir = dir + 0x10;
                bloke->low_level_action = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 4:
                bloke->dest.x = (x << 8) - 0x260;
                bloke->dest.y = (y - 12) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->target_dir = dir + 0x10;
                bloke->low_level_action = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 5:
                bloke->dest.x = (x - 2) << 8;
                bloke->dest.y = (y << 8) - 0x980;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->target_dir = dir + 0x10;
                bloke->low_level_action = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 6:
                bloke->dest.x = (x << 8) - 0x260;
                bloke->dest.y = (y - 8) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->target_dir = dir + 0x10;
                bloke->low_level_action = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 7:
                bloke->dest.x = (x - 2) << 8;
                bloke->dest.y = (y << 8) - 0x680;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->target_dir = dir + 0x10;
                bloke->low_level_action = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 8:
                bloke->dest.y = (y - 4) << 8;
                bloke->dest.x = (x - 2) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->target_dir = dir + 0x10;
                bloke->low_level_action = 7;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 9:
                bloke->dest.x = (x << 8) + 0x80;
                bloke->dest.y = (y << 8) + 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->target_dir = dir + 0x10;
                bloke->low_level_action = 7;
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
void TempleSetEditMode(void) {
    void *temp = TempleBuildingRide;
    EditMode.unk0 = 1;
    EditMode.unk8 = temp;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00416e00
void TempleAddObject(struct Element *param_1, int *param_2) {
    AddBasicObject(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x00416e20
void TempleRemoveObject(Element *a1, TileId tile, struct Cursor *a3) {
    StandardRemoveObject(a1, tile, a3);
    RemoveAllBlokesFromRide(a1->ride, tile);
}

// FUNCTION: LEGOLAND 0x00416e50
void Temple_GetInterfaces(struct Element *str, struct Ride *obj) {
    // STRING: LEGOLAND 0x004b4ef0
    if (_stricmp("TEMPLE", str->name) == 0) {
        obj->cb_a4 = LoadTempleMatteSprites;
        obj->cb_ac = KillTempleMatteSprites;
        obj->cb_8c = TempleSetEditMode;
        obj->cb_pre_render = TempleUpdate;
        obj->cb_b0 = RenderTemple;
        obj->cb_9c = TempleRemoveObject;
        obj->cb_98 = TempleAddObject;
    }
}
