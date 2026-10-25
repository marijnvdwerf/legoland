#include <stdlib.h>
#include "legoland.h"

#include "bloke.h"
#include "bloke_ai.h"
#include "gamemap.h"
#include "globals.h"
#include "man3d.h"
#include "map_object.h"
#include "obj_instance.h"
#include "potting_shed.h"

#include "image_sprite.h"
#include "mechanics_hut.h"
#include "print_sprite.h"
#include "render3d.h"
#include "worker.h"

// FUNCTION: LEGOLAND 0x0043ce60
void LoadGShedMatteSprite(Element *obj) {
    PottingShedRide = obj->ride;
    PottingShedRide->flags |= 0x420;
    PottingShedLayer = PottingShedRide->layer;
    PottingShedRide->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b7994
    GShedMatteSprite = LoadSprite("gshedmatte.lls", 1);
}

// FUNCTION: LEGOLAND 0x0043ceb0
unsigned int PottingShedAddObject(struct Element *param_1, int *param_2) {
    return AddBasicObject(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x0043ced0
void PottingShedRemoveObject(Element *obj, unsigned int tile, struct Cursor *cursor) {
    StandardRemoveObject(obj, *(TileId *)&tile, cursor);
    DetachWorkersFromRemovedHome(obj->ride, tile, 1);
}

// FUNCTION: LEGOLAND 0x0043cf00
void PottingShedUpdate(Element *obj) {
    struct Ride *shed = obj->ride;
    struct RideNode *node;
    struct RideNode *next;
    struct Bloke *bloke;
    char dir;

    for (node = shed->riders; node != NULL; node = next) {
        bloke = node->rider;
        next = node->next;
        if (bloke->low_level_action != 0) {
            continue;
        }
        switch (bloke->param_action) {
        case 0:
            bloke->flags |= 8;
            bloke->dest.x += 0x900;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->target_dir = dir + 0x10;
            bloke->low_level_action = 7;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 1:
            bloke->flags &= 0xffd7;
            RemoveBlokeFromList((struct Ride *)shed, (struct RideNode *)node);
            NewLongTermAction(bloke, 0x10);
            free(node);
            break;
        case 100:
            bloke->flags |= 8;
            bloke->dest.x -= 0x900;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->target_dir = dir + 0x10;
            bloke->low_level_action = 7;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            break;
        case 101:
            RemoveBlokeFromList((struct Ride *)shed, (struct RideNode *)node);
            bloke->field_36 = 100;
            RefundGardener();
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x0043d0b0
void RenderPottingShed(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile) {
    struct Ride *shed = obj->ride;
    struct RideNode *node = shed->riders;
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
        if (blokes[i]->param_action == 100) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    for (i = 0; i < count; i++) {
        if (blokes[i]->param_action == 101) {
            IP_RenderBlokeIn3DNow(blokes[i]);
        }
    }
    pos = GetScreenCoordsForObject((TileId *)tile, shed);
    PrintSprite(GShedMatteSprite, pos.x, pos.y, 0, 0);
}

// FUNCTION: LEGOLAND 0x0043d1c0
void KillGShedMatteSprite(void) {
    KillSprite(GShedMatteSprite);
}

// FUNCTION: LEGOLAND 0x0043d1d0
void PottingShedSetEditMode(void) {
    void *p = PottingShedRide;
    EditMode.unk0 = 1;
    EditMode.unk8 = p;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x0043d210
struct RideSpriteInfo *GetPottingShedSpriteInfo(struct Element *param1, unsigned short param2) {
    struct Ride *s2 = param1->ride;
    struct Sprite *s3;

    PottingShedSpriteInfo.sprite = s2->layer;
    PottingShedSpriteInfo.x = s2->field_14;
    PottingShedSpriteInfo.y = s2->field_18;
    PottingShedSpriteInfo.id = param2;

    s3 = s2->layer;
    s3->flags |= 0x2000;

    return &PottingShedSpriteInfo;
}
