#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "bloke.h"
#include "fort.h"
#include "gamemap.h"
#include "globals.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "objclass.h"
#include "print_sprite.h"
#include "render3d.h"

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x00406240
void LoadFortMaskSprite(Element *elem) {
    struct Ride *ride = elem->ride;
    FortRide = ride;
    if (ride != NULL) {
        ride->flags |= 0x20;
        if (FortRide->layer != NULL) {
            FortRide->layer->flags |= 0x2000;
            FortLayer = FortRide->layer;
        }
    }
    // STRING: LEGOLAND 0x004b4590
    FortMaskSprite = LoadSprite("fortmask.lls", 1);
}

// FUNCTION: LEGOLAND 0x004062a0
void KillFortMaskSprite(void) {
    struct Sprite *sprite = FortMaskSprite;
    if (sprite != NULL) {
        KillSprite(sprite);
    }
}

// FUNCTION: LEGOLAND 0x004062c0
void RenderFort(Element *elem, unsigned int param_2, unsigned int param_3, TileId *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = elem->ride;
    struct Point pos = GetScreenCoordsForObject(tile, ride);
    int base = ride->x + tile->pos.x;
    struct RideNode *node;
    struct LLS *lls;
    struct Sprite *sprite;
    int frame;

    RenderItems_New();
    FortRiderRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (tile->id == node->tile.id && (node->rider->pos.x >> 8) <= base - 4) {
            AddBlokeToRenderList(&FortRiderRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&FortRiderRenderList);
    if (FortLayer != NULL) {
        struct Point off;
        sprite = GetSpriteForLayer(FortLayer, 2);
        if (sprite != NULL) {
            lls = GetLLSForSprite((struct Sprite *)sprite);
            if (lls != NULL) {
                LLSStop((struct LLS *)lls);
            }
        }
        off = GetRenderOffsetForLayer(FortLayer, 2);
        AdjustOffsetForViewMode(&off);
        PrintSprite(GetSpriteForLayer(FortLayer, 2), pos.x + off.x, pos.y + off.y, clip, 0);
    }
    RenderItems_New();
    FortRiderRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (tile->id == node->tile.id && (node->rider->pos.x >> 8) > base - 4) {
            AddBlokeToRenderList(&FortRiderRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&FortRiderRenderList);
    if (FortMaskSprite != NULL) {
        struct Point off2;
        off2.x = 0x173;
        off2.y = -0x7b;
        AdjustOffsetForViewMode(&off2);
        lls = NULL;
        sprite = GetSpriteForLayer(FortLayer, 2);
        if (sprite != NULL) {
            lls = GetLLSForSprite((struct Sprite *)sprite);
        }
        if (lls != NULL) {
            frame = lls->frame;
        } else {
            frame = clip;
        }
        lls = GetLLSForSprite((struct Sprite *)FortMaskSprite);
        if (lls != NULL) {
            LLSSetFrame(lls, frame);
        }
        PrintSprite(FortMaskSprite, pos.x + off2.x, pos.y + off2.y, clip, 0);
    }
}

// FUNCTION: LEGOLAND 0x004064d0
void FortWanderUpdate(RideNode *node, Bloke *bloke) {
    TileId *tile = &node->tile;
    int h, r;
    float fx, fy;
    char dir;

    switch ((short)bloke->field_40) {
    case 1:
        if (--bloke->field_58 <= 0) {
            bloke->field_40 = bloke->field_42;
        }
        break;
    case 2:
        r = (FortWanderRect.right - FortWanderRect.left) << 8;
        h = (FortWanderRect.bottom - FortWanderRect.top) << 8;
        fx = (rand() & 0xff) * 0.003921569f;
        fy = (rand() & 0xff) * 0.003921569f;
        bloke->dest.x = (int)(((tile->pos.x + FortWanderRect.left) << 8) + r * fx);
        bloke->dest.y = (int)(((tile->pos.y + FortWanderRect.top) << 8) + h * fy);
        dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
        bloke->low_level_action = 7;
        bloke->target_dir = dir + 0x10;
        NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
        bloke->field_40 = 3;
        break;
    case 3:
        r = rand() & 3;
        if (r == 0 || r == 1) {
            bloke->dir = rand() & 0xf;
            bloke->field_58 = (rand() & 0xf) + 3;
            bloke->field_40 = 1;
            bloke->field_42 = 3;
        }
        if (r == 2) {
            bloke->field_40 = r;
            return;
        }
        if (r == 3) {
            bloke->param_action++;
        }
        break;
    }
}

/* Update the fort animation and advance each rider after its current movement finishes.
 * Keep the walking setup in each case, with target_dir assigned before low_level_action:
 * MSVC6 then shares the case 2/3 call tail and batches both calls' stack cleanup exactly
 * as in the original. An explicit shared label inserts an extra stack adjustment. */
// FUNCTION: LEGOLAND 0x00406660
void FortUpdate(Element *elem) {
    struct Ride *ride = elem->ride;
    struct Sprite *sprite;
    RideNode *node;
    LLS *animation;
    RideNode *next;
    int x, y;
    Bloke *bloke;
    int tile_x;
    unsigned char dir;

    /* Layer 2 advances one frame per update and loops at the end of its LLS. */
    sprite = GetSpriteForLayer(FortLayer, 2);
    if (sprite != NULL) {
        animation = GetLLSForSprite(sprite);
        if (animation != NULL) {
            if (++animation->frame >= animation->frame_count) {
                animation->frame = 0;
            }
        }
    }
    node = ride->riders;
    if (node != NULL) {
        do {
            /* State 4 removes this node, so save the following rider before updating it. */
            next = node->next;
            bloke = node->rider;
            x = ride->x;
            /* A full-width int preserves the tile's unsigned byte coordinate. */
            tile_x = node->tile.pos.x;
            y = ride->y;
            x = x + tile_x;
            y = y + node->tile.pos.y;
            if (bloke->low_level_action == 0) {
                switch (bloke->param_action) {
                case 0:
                    /* Walk four tiles left of the ride-offset tile, then begin wandering. */
                    bloke->flags |= 8;
                    bloke->dest.x = (x - 4) << 8;
                    bloke->dest.y = y << 8;
                    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->target_dir = 0x10 + dir;
                    bloke->low_level_action = 7;
                    NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                    bloke->field_40 = 2;
                    bloke->param_action++;
                    break;
                case 1:
                    /* The wandering substate advances param_action when the visit ends. */
                    FortWanderUpdate(node, bloke);
                    break;
                case 2:
                    /* Return to the assigned tile's centre in 8.8 fixed-point coordinates. */
                    bloke->dest.x = (tile_x << 8) + 0x80;
                    bloke->dest.y = (node->tile.pos.y << 8) + 0x80;
                    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->target_dir = 0x10 + dir;
                    bloke->low_level_action = 7;
                    NewDirForAction(bloke, ((unsigned char)(0x10 + dir) >> 5) + 3);
                    bloke->param_action++;
                    break;
                case 3:
                    /* Walk to the centre of the tile offset by the ride's x/y coordinates. */
                    bloke->dest.x = (x << 8) + 0x80;
                    bloke->dest.y = (y << 8) + 0x80;
                    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->target_dir = 0x10 + dir;
                    bloke->low_level_action = 7;
                    NewDirForAction(bloke, ((unsigned char)(0x10 + dir) >> 5) + 3);
                    bloke->param_action++;
                    break;
                case 4:
                    /* Release the rider and clear the flag set on entry. */
                    RemoveBlokeFromRide(ride, node);
                    bloke->flags = bloke->flags & (~8);
                    break;
                }
            }
            node = next;
        } while (node != NULL);
    }
}

// FUNCTION: LEGOLAND 0x00406820
void FortSetEditMode(void) {
    EditMode.unk8 = FortRide;
    EditMode.unk0 = 1;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&((struct Ride *)EditMode.unk8)->footprint);
}

// FUNCTION: LEGOLAND 0x00406860
unsigned int FortAddObject(struct Element *param1, int *param2) {
    return AddBasicObject(param1, param2);
}

// FUNCTION: LEGOLAND 0x00406880
void FortRemoveObject(Element *elem, TileId tile, struct Cursor *cursor) {
    StandardRemoveObject(elem, tile, cursor);
    RemoveAllBlokesFromRide(elem->ride, tile);
}

// FUNCTION: LEGOLAND 0x004068b0
void FortGetInterfaces(struct Element *name, struct Ride *ci) {
    // STRING: LEGOLAND 0x004b45a0
    if (_stricmp("FORT", name->name) != 0) {
        return;
    }
    ci->cb_a4 = LoadFortMaskSprite;
    ci->cb_ac = KillFortMaskSprite;
    ci->cb_8c = FortSetEditMode;
    ci->cb_pre_render = FortUpdate;
    ci->cb_b0 = RenderFort;
    ci->cb_9c = FortRemoveObject;
    ci->cb_98 = (void (*)())FortAddObject;
}
