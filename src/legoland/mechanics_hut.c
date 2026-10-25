#include <stdlib.h>
#include "legoland.h"

#include "bloke.h"
#include "bloke_ai.h"
#include "gamemap.h"
#include "globals.h"
#include "man3d.h"
#include "map_object.h"
#include "mechanics_hut.h"
#include "obj_instance.h"

#include "image_sprite.h"
#include "print_sprite.h"
#include "render3d.h"
#include "worker.h"

// FUNCTION: LEGOLAND 0x0043d250
void LoadMechHutMaskSprite(Element *ctx) {
    MechanicsHutRide = ctx->ride;
    MechanicsHutRide->flags |= 0x420;
    MechanicsHutLayer = MechanicsHutRide->layer;
    MechanicsHutRide->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b79a4
    MechHutMaskSprite = LoadSprite("MechHutMask.lls", 1);
}

// FUNCTION: LEGOLAND 0x0043d2a0
unsigned int MechanicsHutAddObject(struct Element *param1, int *param2) {
    return AddBasicObject(param1, param2);
}

// FUNCTION: LEGOLAND 0x0043d2c0
void MechanicsHutRemoveObject(Element *obj, unsigned int tile, struct Cursor *cursor) {
    StandardRemoveObject(obj, *(TileId *)&tile, cursor);
    DetachWorkersFromRemovedHome(obj->ride, tile, 0);
}

// FUNCTION: LEGOLAND 0x0043d2f0
void MechanicsHutUpdate(Element *obj) {
    struct Ride *hut = obj->ride;
    struct RideNode *node;
    struct RideNode *next;
    struct Bloke *bloke;
    char dir;

    for (node = hut->riders; node != NULL; node = next) {
        bloke = node->rider;
        next = node->next;
        if (bloke->low_level_action != 0) {
            continue;
        }
        switch (bloke->param_action) {
        case 0:
            bloke->flags |= 8;
            bloke->dest.y += 0x300;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->target_dir = dir + 0x10;
            bloke->low_level_action = 7;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 1:
            bloke->dest.x += 0x400;
            bloke->dest.y += 0x100;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->target_dir = dir + 0x10;
            bloke->low_level_action = 7;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 2:
            bloke->dest.y += 0x400;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->target_dir = dir + 0x10;
            bloke->low_level_action = 7;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 3:
            bloke->flags &= 0xffd7;
            RemoveBlokeFromList((struct Ride *)hut, (struct RideNode *)node);
            free(node);
            NewLongTermAction(bloke, 0x11);
            break;
        case 100:
            bloke->flags |= 8;
            bloke->dest.y -= 0x300;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->target_dir = dir + 0x10;
            bloke->low_level_action = 7;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 101:
            bloke->dest.x -= 0x480;
            bloke->dest.y -= 0x100;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->target_dir = dir + 0x10;
            bloke->low_level_action = 7;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 102:
            bloke->dest.y -= 0x300;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->target_dir = dir + 0x10;
            bloke->low_level_action = 7;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 103:
            RemoveBlokeFromList((struct Ride *)hut, (struct RideNode *)node);
            bloke->field_36 = 100;
            RefundMechanic();
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x0043d580
void RenderMechanicsHut(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile) {
    struct Ride *hut = obj->ride;
    struct RideNode *node = hut->riders;
    struct Bloke *blokes[30] = {0};
    char count = 0;
    char i;
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
        if (blokes[i]->param_action == 100) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 101) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 103) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    pos = GetScreenCoordsForObject((TileId *)tile, hut);
    PrintSprite(MechHutMaskSprite, pos.x, pos.y, 0, 0);
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 102) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
}

// FUNCTION: LEGOLAND 0x0043d730
void KillMechHutMaskSprite(void) {
    KillSprite(MechHutMaskSprite);
}

// FUNCTION: LEGOLAND 0x0043d740
void MechanicsHutSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = MechanicsHutRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&((struct Ride *)EditMode.unk8)->footprint);
}

// FUNCTION: LEGOLAND 0x0043d780
struct RideSpriteInfo *GetMechanicsHutSpriteInfo(struct Element *ptr, unsigned short arg2) {
    struct Ride *ride = ptr->ride;

    MechanicsHutSpriteInfo.sprite = ride->layer;
    MechanicsHutSpriteInfo.x = ride->field_14;
    MechanicsHutSpriteInfo.y = ride->field_18;
    MechanicsHutSpriteInfo.id = arg2;

    ride->layer->flags |= 0x2000;

    return &MechanicsHutSpriteInfo;
}

/* Detaches the workers attached to a removed home tile. Active workers are unlinked/freed from the home list, moved
 * to the home entrance, and returned to gardener/mechanic idle according to flag; pending workers are unlinked and
 * marked with field_36 = 100. */
// FUNCTION: LEGOLAND 0x0043d7c0
void DetachWorkersFromRemovedHome(struct Ride *hut, unsigned int tile, int flag) {
    struct RideNode *node;
    struct RideNode *next;
    struct Bloke *bloke;
    struct Point at;

    at.x = hut->x + (unsigned char)tile;
    at.y = hut->y + ((unsigned char *)&tile)[1];

    for (node = hut->riders; node != NULL; node = next) {
        next = node->next;
        if (node->tile.id == (unsigned short)tile) {
            bloke = node->rider;
            if (bloke->param_action < 100) {
                RemoveBlokeFromList((struct Ride *)hut, (struct RideNode *)node);
                free(node);
                bloke->flags &= 0xffd7;
                bloke->pos.x = at.x << 8;
                bloke->pos.y = at.y << 8;
                if (flag != 0) {
                    NewLongTermAction(bloke, 0x10);
                } else {
                    NewLongTermAction(bloke, 0x11);
                }
            } else {
                RemoveBlokeFromList((struct Ride *)hut, (struct RideNode *)node);
                bloke->field_36 = 100;
            }
        }
    }
}
