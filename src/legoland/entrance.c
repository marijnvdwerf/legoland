#include <stdlib.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "bloke_ai.h"
#include "bricks.h"
#include "entrance.h"
#include "money.h"
#include "sound_music.h"

#include "image_sprite.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render3d.h"

/* Plays the entrance sound attached to the arriving visitor and, when the entrance fee is nonzero, plays the money
 * sound at the entrance tile. */
// FUNCTION: LEGOLAND 0x0042d970
void PlayEntranceArrivalSounds(TileId *tile, struct Bloke *arg) {
    struct SampleSource config;

    config.type = 1;
    config.bloke = arg;
    PlayInstanceOfSample(ENTRANCE_SFX[0].sample, 0, 1, &config);
    if (MapStats.entrance_fee != 0) {
        PlayMoneySFX(tile, 1, 0);
    }
}

// FUNCTION: LEGOLAND 0x0042d9c0
void RenderEntrance(Element *obj, unsigned int param_2, unsigned int param_3, TileId *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = obj->ride;
    struct RideNode *node;
    struct Point pos;
    struct Point off;
    struct Point off1;
    struct Sprite *spr;
    int lo;
    int hi;

    pos = GetScreenCoordsForObject(tile, ride);
    off = GetRenderOffsetForLayer(ride->layer, 3);
    AdjustOffsetForViewMode(&off);
    hi = (tile->pos.y + ride->footprint.y0) << 8;
    lo = (tile->pos.x + ride->footprint.x1) << 8;

    RenderItems_New();
    BlokeRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (tile->id == node->tile.id && node->rider->pos.x < lo) {
            AddBlokeToRenderList(&BlokeRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&BlokeRenderList);

    off1 = GetRenderOffsetForLayer(ride->layer, 1);
    AdjustOffsetForViewMode(&off1);
    spr = GetSpriteForLayer(ride->layer, 1);
    PrintSprite(spr, pos.x + off1.x, pos.y + off1.y, clip, 0);
    off1 = GetRenderOffsetForLayer(ride->layer, 2);
    AdjustOffsetForViewMode(&off1);
    spr = GetSpriteForLayer(ride->layer, 2);
    PrintSprite(spr, pos.x + off1.x, pos.y + off1.y, 0, 0);

    RenderItems_New();
    BlokeRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (tile->id == node->tile.id && node->rider->pos.x >= lo && node->rider->pos.y >= hi + 0x820 && node->rider->pos.y <= hi + 0x8d0) {
            AddBlokeToRenderList(&BlokeRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&BlokeRenderList);
    if (EntranceMatte4Sprite != NULL) {
        PrintSprite(EntranceMatte4Sprite, pos.x + off.x, pos.y + off.y, 0, 0);
    }

    RenderItems_New();
    BlokeRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (tile->id == node->tile.id && node->rider->pos.x >= lo && node->rider->pos.y >= hi + 0x920 && node->rider->pos.y <= hi + 0x9d0) {
            AddBlokeToRenderList(&BlokeRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&BlokeRenderList);
    if (EntranceMatte3Sprite != NULL) {
        PrintSprite(EntranceMatte3Sprite, pos.x + off.x, pos.y + off.y, 0, 0);
    }

    RenderItems_New();
    BlokeRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (tile->id == node->tile.id && node->rider->pos.x >= lo && node->rider->pos.y >= hi + 0xc20 && node->rider->pos.y <= hi + 0xcd0) {
            AddBlokeToRenderList(&BlokeRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&BlokeRenderList);
    if (EntranceMatte2Sprite != NULL) {
        PrintSprite(EntranceMatte2Sprite, pos.x + off.x, pos.y + off.y, 0, 0);
    }

    RenderItems_New();
    BlokeRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (tile->id == node->tile.id && node->rider->pos.x >= lo && node->rider->pos.y >= hi + 0xd20 && node->rider->pos.y <= hi + 0xdd0) {
            AddBlokeToRenderList(&BlokeRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&BlokeRenderList);
    if (EntranceMatte1Sprite != NULL) {
        PrintSprite(EntranceMatte1Sprite, pos.x + off.x, pos.y + off.y, 0, 0);
    }

    RenderItems_New();
    BlokeRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (tile->id == node->tile.id && node->rider->pos.x >= lo) {
            int y = node->rider->pos.y;
            if ((y < hi + 0x820 || y > hi + 0x8d0) && (y < hi + 0x920 || y > hi + 0x9d0) && (y < hi + 0xc20 || y > hi + 0xcd0) && (y < hi + 0xd20 || y > hi + 0xdd0)) {
                AddBlokeToRenderList(&BlokeRenderList, node, node->person->field_20);
            }
        }
    }
    RenderBlokeList((struct RenderItemNode **)&BlokeRenderList);
    if (Booth1Sprite != NULL) {
        PrintSprite(Booth1Sprite, pos.x + off.x, pos.y + off.y, 0, 0);
    }
}

// FUNCTION: LEGOLAND 0x0042de50
void LoadEntranceResources(Element *param) {
    Load_FXList(ENTRANCE_SFX, 1);
    LoadMoneySFX();
    EntranceRide = param->ride;
    EntranceRide->flags |= 0x20;
    EntranceLayer = EntranceRide->layer;
    EntranceLayer->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b66cc
    EntranceMatte1Sprite = LoadSprite("entrance_matte1.lls", 1);
    // STRING: LEGOLAND 0x004b66b8
    EntranceMatte2Sprite = LoadSprite("entrance_matte2.lls", 1);
    // STRING: LEGOLAND 0x004b66a4
    EntranceMatte3Sprite = LoadSprite("entrance_matte3.lls", 1);
    // STRING: LEGOLAND 0x004b6690
    EntranceMatte4Sprite = LoadSprite("entrance_matte4.lls", 1);
    // STRING: LEGOLAND 0x004b6684
    Booth1Sprite = LoadSprite("booth1.lls", 1);
}

// FUNCTION: LEGOLAND 0x0042def0
void UnloadEntranceResources(Element *param) {
    Kill_FXList(ENTRANCE_SFX, 1);
    KillMoneySFX();
    EntranceRide = param->ride;
    if (Booth1Sprite != 0) {
        KillSprite(Booth1Sprite);
    }
    if (EntranceMatte1Sprite != 0) {
        KillSprite(EntranceMatte1Sprite);
    }
    if (EntranceMatte2Sprite != 0) {
        KillSprite(EntranceMatte2Sprite);
    }
    if (EntranceMatte3Sprite != 0) {
        KillSprite(EntranceMatte3Sprite);
    }
    if (EntranceMatte4Sprite != 0) {
        KillSprite(EntranceMatte4Sprite);
    }
}

// FUNCTION: LEGOLAND 0x0042df70
void EntranceRemoveObject(Element *obj, TileId tile, struct Cursor *cursor) {
    StandardRemoveObject(obj, tile, cursor);
    RemoveAllBlokesFromRide(obj->ride, tile);
}

// FUNCTION: LEGOLAND 0x0042dfa0
void Entrance1Update(Element *elem) {
    Ride *ride = elem->ride;
    RideNode *node;
    RideNode *next;

    for (node = ride->riders; node != NULL; node = next) {
        Bloke *bloke = node->rider;
        TileId *tile;
        int x0, y, x1;
        int *table;

        next = node->next;
        tile = &node->tile;
        x0 = tile->pos.x + ride->footprint.x0;
        x1 = tile->pos.x + ride->footprint.x1 + 6;
        y = tile->pos.y + ride->footprint.y0;
        if (bloke->low_level_action != 0) {
            continue;
        }
        switch (bloke->param_action) {
        case 0:
            Entrance1AlternateBit ^= 1;
            bloke->flags |= 8;
            if (bloke->pos.x <= (x0 << 8)) {
                bloke->field_36 = 2;
                bloke->param_action++;
                bloke->field_3a = (rand() >> 8) + Entrance1AlternateBit & 1;
            }
            if (bloke->pos.x >= (x1 << 8)) {
                bloke->field_36 = 1;
                bloke->param_action++;
                bloke->field_3a = (rand() >> 8) + Entrance1AlternateBit & 1;
            }
            if (bloke->field_36 == 1) {
                bloke->dest.x = x1 << 8;
                bloke->param_action = 0x32;
                table = ENTRANCE_DEST_RIGHT;
            } else {
                bloke->dest.x = x0 << 8;
                table = ENTRANCE_DEST_LEFT;
            }
            bloke->dest.y = table[bloke->field_3a] + (y << 8);
            bloke->target_dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
            bloke->low_level_action = 7;
            NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
            break;
        case 0x32:
            bloke->param_action = 1;
            bloke->dest.x = (x1 - 3) << 8;
            bloke->target_dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
            bloke->low_level_action = 7;
            NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
            break;
        case 1:
            if (bloke->field_36 == 1) {
                AddBricks(MapStats.entrance_fee);
                PlayEntranceArrivalSounds(&node->tile, bloke);
                table = ENTRANCE_DEST_RIGHT;
                bloke->dest.x = (x0 << 8) - 0x80;
            } else {
                table = ENTRANCE_DEST_LEFT;
                bloke->dest.x = x1 << 8;
            }
            bloke->dest.y = table[bloke->field_3a] + (y << 8);
            bloke->target_dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
            bloke->low_level_action = 7;
            NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
            bloke->param_action++;
            break;
        case 2:
            RemoveBlokeFromRide(ride, node);
            bloke->target = 0;
            bloke->last_ride = 0;
            PopLongTermAction(bloke);
            break;
        }
    }
}
