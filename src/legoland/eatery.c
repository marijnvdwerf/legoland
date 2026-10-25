#include <stdlib.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "bloke_ai.h"
#include "eatery.h"
#include "gamemap.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "money.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render3d.h"
#include "sound_music.h"

#include "image_sprite.h"

struct BrollyData {
    /* 0x00 */ unsigned char pad_0[4];
    /* 0x04 */ int count_4;
    /* 0x08 */ struct Sprite **table_8;
    /* 0x0c */ int *table_c;
    /* 0x10 */ int *table_10;
};

struct UserFlagsArg {
    int var_0;
    int var_4;
};

// FUNCTION: LEGOLAND 0x0042e220
void LoadChuckWagonResources(struct Element *obj) {
    struct Ride *fx = obj->ride;
    struct Sprite *inner;
    ChuckWagonRide = fx;
    fx->flags |= 0x20;
    inner = ChuckWagonRide->layer;
    inner->flags |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e250
void UnloadChuckWagonResources(void) { KillMoneySFX(); }

// FUNCTION: LEGOLAND 0x0042e260
void RenderChuckWagon(struct Element *arg, unsigned int param2, unsigned int param3, unsigned short *value) {
    struct Ride *owner = arg->ride;
    struct RideNode *node = owner->riders;
    if (node != NULL) {
        while (node != NULL) {
            if (*value == node->tile.id) {
                IP_RenderBlokeIn3DNow((struct Bloke *)node->rider);
                GetScreenCoordsForObject((TileId *)value, owner);
                break;
            }
            node = node->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x0042e2a0
void ChuckWagonUpdate(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    struct RideNode *next;
    struct Bloke *bloke;
    TileId *tile;
    int x;
    int y;
    char dir;

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
                bloke->dest.x = x << 8;
                bloke->dest.y = (y + 1) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 1:
                bloke->dest.x = (x << 8) - 0x80;
                bloke->dest.y = (y + 1) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
            case 2:
                bloke->dir = 7;
                bloke->param_action++;
                bloke->field_58 = (rand() & 0x1f) + 4;
                break;
            case 3:
                if (bloke->field_58 == 0) {
                    bloke->param_action++;
                    BuyItem((struct Element *)obj, tile, 0);
                }
                bloke->field_58--;
                break;
            case 4:
                bloke->dest.x = (x << 8) + 0x80;
                bloke->dest.y = (y << 8) + 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 5:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= 0xfff7;
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x0042e460
void LoadBrollyImages(struct Element *obj) {
    ActiveEateryRide = obj->ride;
    ActiveEateryRide->flags |= 0x400;
    // STRING: LEGOLAND 0x004b6ec8
    if (LLIDB_FindElement("BROLLY IMAGES", &BrollyImagesHandle, 0) == 0) {
        BrollyImagesData = (struct BrollyData *)LLIDB_LoadData((void *)BrollyImagesHandle);
    }
}

// FUNCTION: LEGOLAND 0x0042e4b0
void UnloadBrollyImages(void) {
    LLIDB_UnLoadData(BrollyImagesHandle);
}

// FUNCTION: LEGOLAND 0x0042e4c0
void BrollySetEditMode(void) {
    struct Ride *temp = ActiveEateryRide;
    EditMode.unk0 = 1;
    EditMode.unk8 = temp;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x0042e500
void BrollyAddObject(struct Element *param_1, unsigned char *param_2) {
    TileId id;
    id.pos.x = param_2[0];
    id.pos.y = param_2[4];
    AddObjectToMap(param_1, id, 0);
    if (BrollyImagesData != NULL) {
        int r = rand();
        Set_UserFlags(*(int *)param_2 << 8, *(int *)(param_2 + 4) << 8, (unsigned short)(r % BrollyImagesData->count_4));
    }
}

// FUNCTION: LEGOLAND 0x0042e560
struct RideSpriteInfo *GetBrollySpriteInfo(struct Element *param_1, unsigned short param_2) {
    unsigned char *b = (unsigned char *)&param_2;
    int i = Get_UserFlags((unsigned int)b[0] << 8, (unsigned int)b[1] << 8) & 0xffff;
    RideSpriteInfoBuffer.sprite = BrollyImagesData->table_8[(unsigned char)i];
    RideSpriteInfoBuffer.x = BrollyImagesData->table_c[(unsigned char)i] >> 1;
    RideSpriteInfoBuffer.y = BrollyImagesData->table_10[(unsigned char)i] >> 1;
    RideSpriteInfoBuffer.field_10 = 0;
    return &RideSpriteInfoBuffer;
}

// FUNCTION: LEGOLAND 0x0042e5d0
void LoadSharkCafeResources(struct Element *obj) {
    struct Ride *fx = obj->ride;
    struct Sprite *inner;
    SharkCafeRide = fx;
    SharkCafeRide->flags |= 0x20;
    inner = SharkCafeRide->layer;
    inner->flags |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e600
void UnloadSharkCafeResources(void) { KillMoneySFX(); }

// FUNCTION: LEGOLAND 0x0042e610
void SharkCafeUpdate(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    struct RideNode *next;
    struct Bloke *bloke;
    TileId *tile;
    int x;
    int y;
    char dir;

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
                bloke->dest.x = (x << 8) - 0x80;
                bloke->dest.y = (y << 8) + 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 1:
                bloke->dir = 7;
                bloke->param_action++;
                bloke->field_58 = (rand() & 0x1f) + 4;
                break;
            case 2:
                if (bloke->field_58 == 0) {
                    bloke->param_action++;
                    BuyItem((struct Element *)obj, tile, 1);
                }
                bloke->field_58--;
                break;
            case 3:
                bloke->dest.x = (x << 8) + 0x80;
                bloke->dest.y = (y << 8) + 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 4:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= 0xfff7;
                NewLongTermAction(bloke, 0xd);
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x0042e770
void LoadFoodcartDrinkResources(struct Element *obj) {
    struct Ride *fx = obj->ride;
    struct Sprite *inner;
    FoodcartDrinkRide = fx;
    fx->flags |= 0x20;
    inner = FoodcartDrinkRide->layer;
    inner->flags |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e7a0
void UnloadFoodcartDrinkResources(void) { KillMoneySFX(); }

// FUNCTION: LEGOLAND 0x0042e7b0
void LoadFoodcartIcecreamResources(struct Element *obj) {
    struct Ride *fx = obj->ride;
    struct Sprite *inner;
    FoodcartIcecreamRide = fx;
    fx->flags |= 0x20;
    inner = FoodcartIcecreamRide->layer;
    inner->flags |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e7e0
void UnloadFoodcartIcecreamResources(void) { KillMoneySFX(); }

// FUNCTION: LEGOLAND 0x0042e7f0
void LoadFoodcartFoodResources(struct Element *obj) {
    struct Ride *fx = obj->ride;
    struct Sprite *inner;
    FoodcartFoodRide = fx;
    fx->flags |= 0x20;
    inner = FoodcartFoodRide->layer;
    inner->flags |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e820
void UnloadFoodcartFoodResources(void) { KillMoneySFX(); }

/* Render callback (cb_b0) of the foodcart drink/food/icecream stalls and the shark cafe: draws every customer
 * standing on the rendered tile. */
// FUNCTION: LEGOLAND 0x0042e830
void RenderFoodStallCustomers(struct Element *arg, unsigned int param2, unsigned int param3, unsigned short *value) {
    struct Ride *owner = arg->ride;
    struct RideNode *node = owner->riders;
    if (node == NULL) {
        return;
    }
    while (node != NULL) {
        if (*value == node->tile.id) {
            IP_RenderBlokeIn3DNow((struct Bloke *)node->rider);
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x0042e870
void LoadCastleBBQResources(struct Element *obj) {
    CastleBBQRide = obj->ride;
    CastleBBQRide->flags |= 0x20;
    CastleBBQLayer = CastleBBQRide->layer;
    CastleBBQLayer->flags |= 0x2000;
    Load_FXList(RESTAURANT_SFX, 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e8b0
void UnloadCastleBBQResources(void) {
    Kill_FXList(RESTAURANT_SFX, 1);
    CastleBBQRide = 0;
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e8d0
void EaterySetEditMode(struct Element *arg) {
    struct Ride *temp = arg->ride;
    EditMode.unk0 = 1;
    EditMode.unk8 = (void *)temp;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x0042e910
void RenderCastleBBQ(struct Element *param_1, unsigned int param_2, unsigned int param_3, short *param_4, void *param_5, unsigned int param_6) {
    struct Ride *ride = param_1->ride;
    struct RideNode *node;
    struct Point coords;
    struct Point offset;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (*param_4 == *(short *)&node->tile) {
            IP_RenderBlokeIn3DNow(node->rider);
        }
    }
    coords = GetScreenCoordsForObject((TileId *)param_4, ride);
    offset = GetRenderOffsetForLayer(CastleBBQLayer, 1);
    AdjustOffsetForViewMode(&offset);
    if (GetSpriteForLayer(CastleBBQLayer, 1) != 0) {
        PrintSprite((struct Sprite *)GetSpriteForLayer(CastleBBQLayer, 1),
            offset.x + coords.x, offset.y + coords.y, param_6, 0);
    }
}

// FUNCTION: LEGOLAND 0x0042e9c0
void CastleBBQAddObject(struct Element *param_1, unsigned int *param_2) {
    struct SampleSource params;
    AddBasicObject(param_1, param_2);
    params.x = *param_2;
    params.y = param_2[1];
    params.type = 2;
    PlayInstanceOfSample(RESTAURANT_SFX[0].sample, 1, 1, &params);
}

// FUNCTION: LEGOLAND 0x0042ea10
void CastleBBQRemoveObject(struct Element *param_1, unsigned int param_2, struct Cursor *param_3) {
    unsigned char *b = (unsigned char *)&param_2;
    struct SampleSource params;
    StandardRemoveObject(param_1, *(TileId *)&param_2, param_3);
    params.x = b[0];
    params.y = b[1];
    params.type = 2;
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
}

// FUNCTION: LEGOLAND 0x0042ea60
void CastleBBQUpdate(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    struct RideNode *next;
    struct Bloke *bloke;
    TileId *tile;
    int x;
    int y;
    char dir;

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
                bloke->dest.x = (x << 8) + 0x280;
                bloke->dest.y = (y << 8) + 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 1:
                bloke->dest.x = (x << 8) + 0x280;
                bloke->dest.y = y << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 2:
                bloke->field_58 = (rand() & 0x1f) + 4;
                bloke->param_action++;
                break;
            case 3:
                if (bloke->field_58 == 0) {
                    bloke->param_action++;
                    PlayMoneySFX(tile, 0, 0);
                }
                bloke->field_58--;
                break;
            case 4:
                bloke->dest.x = (x << 8) + 0x280;
                bloke->dest.y = (y << 8) + 0x80;
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
                bloke->flags &= 0xfff7;
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x0042ec10
void FoodcartDrinkUpdate(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    struct RideNode *next;
    struct Bloke *bloke;
    TileId *tile;
    int x;
    int y;
    char dir;

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
                bloke->dest.x = (x << 8) - 0x80;
                bloke->dest.y = (y << 8) + 0x180;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 1:
                bloke->dir = 7;
                bloke->param_action++;
                bloke->field_58 = (rand() & 0x1f) + 4;
                break;
            case 2:
                if (bloke->field_58 == 0) {
                    bloke->param_action++;
                    BuyItem((struct Element *)obj, tile, 1);
                }
                bloke->field_58--;
                break;
            case 3:
                bloke->dir = 3;
                bloke->dest.x = (x << 8) + 0x80;
                bloke->dest.y = (y << 8) + 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 4:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= 0xfff7;
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x0042ed70
void FoodcartFoodUpdate(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    struct RideNode *next;
    struct Bloke *bloke;
    TileId *tile;
    int x;
    int y;
    char dir;

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
                bloke->dest.x = (x << 8) - 0x80;
                bloke->dest.y = (y << 8) + 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 1:
                bloke->dir = 7;
                bloke->param_action++;
                bloke->field_58 = (rand() & 0x1f) + 4;
                break;
            case 2:
                if (bloke->field_58 == 0) {
                    bloke->param_action++;
                    BuyItem((struct Element *)obj, tile, 1);
                }
                bloke->field_58--;
                break;
            case 3:
                bloke->dest.x = (x << 8) + 0x80;
                bloke->dest.y = (y << 8) + 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 4:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= 0xfff7;
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x0042eec0
struct BrollyNode *AllocBrollyNode(unsigned short *param_1) {
    struct BrollyNode *node = malloc(sizeof(struct BrollyNode));
    if (node != NULL) {
        memset(node, 0, sizeof(struct BrollyNode));
        node->value = *param_1;
        node->next = BrollyNodeList;
        memset(&node->seats, 0, sizeof(node->seats));
        node->field_9 = 0;
        BrollyNodeList = node;
    }
    return node;
}

// FUNCTION: LEGOLAND 0x0042ef10
void Restaurant1AddObject(struct Element *param_1, unsigned char *param_2) {
    unsigned char id[2];
    id[0] = param_2[0];
    id[1] = param_2[4];
    AddBasicObject(param_1, (int *)param_2);
    AllocBrollyNode((unsigned short *)id);
}

// FUNCTION: LEGOLAND 0x0042ef40
struct BrollyNode *FindBrollyNode(unsigned short *param_1) {
    struct BrollyNode *node = BrollyNodeList;
    if (node == NULL) {
        return NULL;
    }
    while (memcmp(&node->value, param_1, 2) != 0) {
        node = node->next;
        if (node == NULL) {
            return NULL;
        }
    }
    return node;
}

// FUNCTION: LEGOLAND 0x0042ef70
void RemoveBrollyNode(struct BrollyNode *node) {
    struct BrollyNode *head = BrollyNodeList;
    if (head == node) {
        BrollyNodeList = node->next;
    } else {
        struct BrollyNode *current = head;
        if (current->next != node) {
            do {
                current = current->next;
            } while (current && current->next != node);
        }
        if (current) {
            current->next = node->next;
        }
    }
    free(node);
}

// FUNCTION: LEGOLAND 0x0042efb0
void Restaurant1RemoveObject(struct Element *param_1, TileId tile, struct Cursor *param_3) {
    struct SampleSource params;
    struct BrollyNode *node = FindBrollyNode(&tile.id);
    if (node != NULL) {
        RemoveBrollyNode(node);
    }
    StandardRemoveObject(param_1, tile, param_3);
    RemoveAllBlokesFromRide(((struct Element *)param_1)->ride, tile);
    params.x = tile.pos.x;
    params.y = tile.pos.y;
    params.type = 2;
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
}

// FUNCTION: LEGOLAND 0x0042f030
void LoadRestaurant1Resources(struct Element *obj) {
    Restaurant1Ride = obj->ride;
    Restaurant1Ride->flags |= 0x20;
    EateryFxInner = Restaurant1Ride->layer;
    EateryFxInner->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b6f2c
    RestMaskMainSprite = LoadSprite("RestMask_Main.lls", 1);
    // STRING: LEGOLAND 0x004b6f14
    RestMaskLevel1aaSprite = LoadSprite("RestMaskLevel1aa.lls", 1);
    // STRING: LEGOLAND 0x004b6f00
    RestMaskLevel1Sprite = LoadSprite("RestMaskLevel1.lls", 1);
    // STRING: LEGOLAND 0x004b6eec
    RestMaskLevel2Sprite = LoadSprite("RestMaskLevel2.lls", 1);
    // STRING: LEGOLAND 0x004b6ed8
    RestMaskLevel3Sprite = LoadSprite("RestMaskLevel3.lls", 1);
    HideLayer(EateryFxInner, 1);
    StopLayerPlaying(EateryFxInner, 1);
    LLSSetFrame((struct LLS *)GetLLSForLayer(EateryFxInner, 1), 0);
    LoadMoneySFX();
}

/* Sends a Restaurant1 customer to step `step` of its seat's walk path (Restaurant1CustomerPath, seat =
 * bloke->field_36): sets the destination relative to tile (x, y), starts the walk (low-level action 7), stores the
 * frame and optionally turns to face the move. */
// FUNCTION: LEGOLAND 0x0042f0f0
void MoveRestaurant1CustomerToStep(struct Bloke *bloke, int x, int y, int step) {
    int idx = (step + bloke->field_36 * 5) * 6;
    int y_add = Restaurant1CustomerPath[idx + 1];
    int dir_flag = Restaurant1CustomerPath[idx + 4];
    int frame = Restaurant1CustomerPath[idx + 5];
    char dir;

    bloke->dest.x = (Restaurant1CustomerPath[idx] + x) * 0x100 + Restaurant1CustomerPath[idx + 2];
    bloke->dest.y = (y + y_add) * 0x100 + Restaurant1CustomerPath[idx + 3];
    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
    bloke->target_dir = dir + 0x10;
    bloke->low_level_action = 7;
    bloke->field_37 = (unsigned char)frame;
    if (dir_flag == 1) {
        NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
    }
}

// FUNCTION: LEGOLAND 0x0042f1a0
void Restaurant1Update(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    struct RideNode *next;
    struct Bloke *bloke;
    TileId *tile;
    struct BrollyNode *state;
    struct Seats *slot;
    struct Seats seats;
    char last_zero = 0;
    char zero_count = 0;
    char i;
    char *p;
    char dir;
    char pick;
    int x;
    int y;

    while (node != NULL) {
        next = node->next;
        tile = &node->tile;
        bloke = node->rider;
        state = FindBrollyNode((unsigned short *)tile);
        if (state == NULL) {
            return;
        }
        slot = &state->seats;
        seats = *slot;
        x = ride->x + tile->pos.x;
        y = tile->pos.y + ride->y;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                bloke->field_37 = 3;
                bloke->dest.x = (x - 6) << 8;
                bloke->dest.y = y << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                for (i = 0, p = (char *)seats.c; i < 3; i++, p++) {
                    if (*p == 0) {
                        zero_count++;
                        last_zero = i;
                    }
                }
                if (zero_count != 0) {
                    if (zero_count == 3) {
                        pick = rand() % 3;
                    } else {
                        pick = last_zero;
                    }
                    seats.c[pick] = 1;
                    bloke->field_36 = pick;
                    bloke->field_46 = 3;
                    bloke->field_5c = 300;
                    bloke->param_action++;
                } else {
                    bloke->field_46 = 4;
                    bloke->field_5c = 500;
                    bloke->param_action++;
                }
                break;
            case 1:
                BuyItem((struct Element *)obj, tile, 1);
                if (bloke->field_46 == 4) {
                    bloke->param_action = 8;
                } else {
                    MoveRestaurant1CustomerToStep(bloke, x, y, 0);
                    bloke->param_action++;
                }
                break;
            case 2:
                MoveRestaurant1CustomerToStep(bloke, x, y, 1);
                bloke->param_action++;
                break;
            case 3:
                MoveRestaurant1CustomerToStep(bloke, x, y, 2);
                bloke->param_action++;
                break;
            case 4:
                bloke->flags |= 0x100;
                bloke->height = 10;
                BlokeSitAnim(bloke);
                BlokeSetFrame(bloke, 0);
                bloke->param_action++;
                break;
            case 5:
                if ((int)bloke->field_5c-- < 0) {
                    bloke->param_action++;
                }
                break;
            case 6:
                bloke->flags &= 0xfeff;
                bloke->height = 0;
                BlokeWalkAnim(bloke);
                MoveRestaurant1CustomerToStep(bloke, x, y, 3);
                bloke->param_action++;
                break;
            case 7:
                MoveRestaurant1CustomerToStep(bloke, x, y, 4);
                bloke->param_action += 2;
                seats.c[bloke->field_36] = 0;
                break;
            case 8:
                if ((int)bloke->field_5c-- < 0) {
                    bloke->param_action++;
                }
            case 9:
                bloke->field_37 = 3;
                bloke->dest.x = x << 8;
                bloke->dest.y = (y + 1) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 10:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= 0xfff7;
                break;
            }
        }
        *slot = seats;
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x0042f4c0
void RenderRestaurant1(struct Element *param_1, unsigned int param_2, unsigned int param_3, short *param_4, void *param_5, unsigned int param_6) {
    struct Ride *ride = param_1->ride;
    struct RideNode *node;
    struct Bloke *blokes[8];
    int count;
    int i;
    struct Bloke **p;
    struct BrollyNode *state;
    struct Point coords;
    struct Point offset;
    struct {
        /* 0x00 */ int field_0;
        /* 0x04 */ struct Element *field_4;
        /* 0x08 */ short field_8;
    } cfg;
    node = ride->riders;
    cfg.field_0 = 0x103;
    cfg.field_4 = ride->element;
    cfg.field_8 = *param_4;
    p = blokes;
    blokes[0] = 0;
    for (i = 7; p = p + 1, i != 0; i = i - 1) {
        *p = NULL;
    }
    count = 0;
    coords = GetScreenCoordsForObject((TileId *)param_4, ride);
    if (node != NULL) {
        short v = *param_4;
        p = blokes;
        do {
            if (v == (short)node->tile.id) {
                count = count + 1;
                *p = node->rider;
                p = p + 1;
            }
            node = node->next;
        } while (node != NULL);
        if (count != 0) {
            for (i = 0; i < count; i = i + 1) {
                if (blokes[i]->field_37 == 1) IP_RenderBlokeIn3DNow(blokes[i]);
            }
            PrintSprite(RestMaskLevel1aaSprite, param_2, param_3, param_6, 0);
            for (i = 0; i < count; i = i + 1) {
                if (blokes[i]->field_37 == 2) IP_RenderBlokeIn3DNow(blokes[i]);
            }
            PrintSprite(RestMaskLevel1Sprite, param_2, param_3, param_6, 0);
            for (i = 0; i < count; i = i + 1) {
                if (blokes[i]->field_37 == 3) IP_RenderBlokeIn3DNow(blokes[i]);
            }
            PrintSprite(RestMaskLevel2Sprite, param_2, param_3, param_6, 0);
            for (i = 0; i < count; i = i + 1) {
                if (blokes[i]->field_37 == 4) IP_RenderBlokeIn3DNow(blokes[i]);
            }
            PrintSprite(RestMaskLevel3Sprite, param_2, param_3, param_6, 0);
            for (i = 0; i < count; i = i + 1) {
                if (blokes[i]->field_37 == 5) IP_RenderBlokeIn3DNow(blokes[i]);
            }
            PrintSprite(RestMaskMainSprite, param_2, param_3, param_6, 0);
        }
    }
    state = FindBrollyNode((unsigned short *)param_4);
    if (state != NULL) {
        char frame = state->field_9 + 1;
        if (frame > 0xf) {
            frame = 0;
        }
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryFxInner, 1), frame);
        offset = GetRenderOffsetForLayer(EateryFxInner, 1);
        AdjustOffsetForViewMode(&offset);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryFxInner, 1),
            coords.x + offset.x, coords.y + offset.y, param_6, (int *)&cfg);
        state->field_9 = frame;
    }
}

// FUNCTION: LEGOLAND 0x0042f720
void KillRestMaskSpritesAndMoneySFX(void) {
    KillSprite(RestMaskMainSprite);
    KillSprite(RestMaskLevel1aaSprite);
    KillSprite(RestMaskLevel1Sprite);
    KillSprite(RestMaskLevel2Sprite);
    KillSprite(RestMaskLevel3Sprite);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042f770
void LoadRestaurant2Resources(struct Element *obj) {
    Load_FXList(OCTOPUS_SFX, 3);
    LoadMoneySFX();
    Restaurant2Ride = obj->ride;
    Restaurant2Ride->flags |= 0x420;
    EateryLayerOwner = Restaurant2Ride->layer;
    EateryLayerOwner->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b6f70
    R2FdoormSprite = LoadSprite("R2Fdoor_m.lls", 1);
    // STRING: LEGOLAND 0x004b6f60
    R2Fdoorm1Sprite = LoadSprite("R2Fdoor_m1.lls", 1);
    // STRING: LEGOLAND 0x004b6f50
    R2BdoormSprite = LoadSprite("R2Bdoor_m.lls", 1);
    // STRING: LEGOLAND 0x004b6f40
    R2TowermSprite = LoadSprite("R2Tower_m.lls", 1);
    HideLayer(EateryLayerOwner, 0);
    StopLayerPlaying(EateryLayerOwner, 0);
    LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 0), 0);
    HideLayer(EateryLayerOwner, 6);
    StopLayerPlaying(EateryLayerOwner, 6);
    LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 6), 0);
    HideLayer(EateryLayerOwner, 2);
    StopLayerPlaying(EateryLayerOwner, 2);
    LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 2), 0);
    HideLayer(EateryLayerOwner, 5);
    HideLayer(EateryLayerOwner, 1);
    StopLayerPlaying(EateryLayerOwner, 1);
    LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 1), 0);
    HideLayer(EateryLayerOwner, 3);
    StopLayerPlaying(EateryLayerOwner, 3);
    LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 3), 0);
}

/* Allocates a zeroed 0x40-byte Restaurant2 state node for the instance at the given tile id (field_38 = 0x143) and
 * pushes it onto SaveBlockList. */
// FUNCTION: LEGOLAND 0x0042f920
void AddRestaurant2Node(unsigned short *param_1) {
    struct SaveBlock *node = malloc(sizeof(struct SaveBlock));
    if (node != NULL) {
        unsigned int *p = (unsigned int *)node;
        int i;
        for (i = 0x10; i != 0; i = i - 1) {
            *p = 0;
            p = p + 1;
        }
        node->value = *param_1;
        node->next = SaveBlockList;
        node->field_6 = 0;
        node->field_7 = 0;
        node->field_8 = 0;
        node->field_9 = 0;
        node->field_c = 0;
        node->field_10 = 0;
        node->field_11 = 0;
        node->field_14 = 0;
        node->field_18 = 0;
        node->field_1c = 0;
        node->field_20 = 0;
        node->field_24 = 0;
        node->field_28 = 0;
        node->field_2c = 0;
        node->field_30 = 0;
        node->field_34 = 0;
        node->field_38 = 0x143;
        node->field_3c = 0;
        SaveBlockList = node;
    }
}

// FUNCTION: LEGOLAND 0x0042f9a0
void Restaurant2AddObject(struct Element *param_1, unsigned char *param_2) {
    unsigned char id[2];
    id[0] = param_2[0];
    id[1] = param_2[4];
    AddBasicObject(param_1, (int *)param_2);
    AddRestaurant2Node((unsigned short *)id);
}

// FUNCTION: LEGOLAND 0x0042f9d0
struct SaveBlock *FindSaveBlock(unsigned short *param) {
    struct SaveBlock *node = SaveBlockList;
    if (node == NULL) {
        return NULL;
    }
    while (memcmp(&node->value, param, 2) != 0) {
        node = node->next;
        if (node == NULL) {
            return NULL;
        }
    }
    return node;
}

// FUNCTION: LEGOLAND 0x0042fa00
void RemoveSaveBlock(struct SaveBlock *param) {
    struct SaveBlock *node;
    struct SaveBlock *prev;

    if (SaveBlockList == param) {
        SaveBlockList = param->next;
    } else {
        node = SaveBlockList->next;
        prev = SaveBlockList;
        while (node != param) {
            prev = prev->next;
            if (prev == NULL) {
                break;
            }
            node = prev->next;
        }
        if (prev != NULL) {
            prev->next = param->next;
        }
    }
    free(param);
}

// FUNCTION: LEGOLAND 0x0042fa40
void Restaurant2RemoveObject(struct Element *arg1, TileId tile, struct Cursor *arg3, unsigned int arg4, unsigned int arg5) {
    struct SaveBlock *result = FindSaveBlock(&tile.id);
    if (result != NULL) {
        RemoveSaveBlock(result);
    }
    StandardRemoveObject(arg1, tile, arg3);
    RemoveAllBlokesFromRide(((struct Element *)arg1)->ride, tile);
}

/* Moves a Restaurant2 rider diagonally by 8 times the step table entry idx: towards -x/-y with the first table for
 * dir 1, towards +x/+y with the second table otherwise. */
// FUNCTION: LEGOLAND 0x0042fa90
void ShiftRestaurant2RiderDiagonally(struct RideNode *node, int dir, int idx) {
    if (dir == 1) {
        node->rider->pos.x += Restaurant2DownSteps[idx] * -8;
        node->rider->pos.y += Restaurant2DownSteps[idx] * -8;
    } else {
        node->rider->pos.x += Restaurant2UpSteps[idx] * 8;
        node->rider->pos.y += Restaurant2UpSteps[idx] * 8;
    }
}

// FUNCTION: LEGOLAND 0x0042fb00
void PlayOctopusStartSfx(unsigned short param_1) {
    unsigned char *b = (unsigned char *)&param_1;
    struct SampleSource params;
    params.x = b[0];
    params.y = b[1];
    params.type = 2;
    PlayInstanceOfSample(OCTOPUS_SFX[0].sample, 0, 1, &params);
    PlayInstanceOfSample(OCTOPUS_SFX[1].sample, 1, 1, &params);
}

// FUNCTION: LEGOLAND 0x0042fb60
void PlayOctopusStopSfx(unsigned short param_1) {
    unsigned char *b = (unsigned char *)&param_1;
    struct SampleSource params;
    params.type = 2;
    params.x = b[0];
    params.y = b[1];
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
    PlayInstanceOfSample(OCTOPUS_SFX[2].sample, 0, 1, &params);
}

// FUNCTION: LEGOLAND 0x0042fbb0
void Restaurant2Update(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node;
    struct RideNode *next;
    struct Bloke *bloke;
    TileId *tile;
    struct SaveBlock *st;
    char dir;
    int x;
    int y;
    /* per-tile restaurant state, copied out of the SaveBlock */
    signed char f6;
    signed char f7;
    signed char f8;
    signed char f9;
    int fc;
    unsigned char f10;
    unsigned char f11;
    int f14;
    int f18;
    int f1c;
    int f20;
    int f24;
    int f28;
    int f2c;
    int f30;
    int f34;
    int f38;
    int f3c;
    int state;
    int lift;

    /* move the blokes queuing/eating in each restaurant */
    node = ride->riders;
    while (node != NULL) {
        tile = &node->tile;
        next = node->next;
        bloke = node->rider;
        st = FindSaveBlock(&tile->id);
        if (st == NULL) {
            return;
        }
        f7 = st->field_7;
        f8 = st->field_8;
        fc = st->field_c;
        f10 = st->field_10;
        f11 = st->field_11;
        f14 = st->field_14;
        f18 = st->field_18;
        f1c = st->field_1c;
        f20 = st->field_20;
        f24 = st->field_24;
        f28 = st->field_28;
        f2c = st->field_2c;
        f30 = st->field_30;
        f38 = st->field_38;
        f3c = st->field_3c;
        x = ride->x + tile->pos.x;
        y = ride->y + tile->pos.y;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                bloke->dest.x = (x << 8) + 0x3c8;
                bloke->dest.y = (y - 2) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->field_5c = 300;
                bloke->field_44 = bloke->speed;
                bloke->speed = 0x15;
                bloke->param_action++;
                fc++;
                break;
            case 1:
                bloke->dest.x = (x << 8) + 0x3c8;
                bloke->dest.y = ((y - 4) << 8) + fc * 100;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 2:
                if (f10 < 3 && f20 != 0) {
                    bloke->dest.y = (y << 8) - 0x39c;
                    bloke->dest.x = (x << 8) + 0x2ce;
                    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->low_level_action = 7;
                    bloke->target_dir = dir + 0x10;
                    NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                    bloke->param_action++;
                    f10++;
                }
                break;
            case 3:
                bloke->dest.x = (x << 8) + 0x16a;
                bloke->dest.y = (y << 8) - 0x39c;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 4:
                bloke->dest.x = (x << 8) + 0x16a;
                bloke->dest.y = (y << 8) - 0x532 + f10 * 100;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                fc--;
                f14++;
                if (f10 == 3 || fc == 0) {
                    f20 = 0;
                    f14 = 3;
                }
                break;
            case 5:
                if (f14 >= 3) {
                    f24 = 1;
                    f14 = 0;
                }
                SetPersonDirection(node->person, 5);
                bloke->param_action++;
                break;
            case 6:
                if (f18 == 2 && f28 == 0) {
                    ShiftRestaurant2RiderDiagonally(node, 1, f7);
                } else if (f28 != 0) {
                    bloke->param_action++;
                }
                break;
            case 7:
                if (f2c != 0) {
                    bloke->dest.x = (x << 8) - 0x79c;
                    bloke->dest.y = (y << 8) - 0xc9c;
                    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->low_level_action = 7;
                    bloke->target_dir = dir + 0x10;
                    NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                    bloke->param_action++;
                }
                break;
            case 8:
                bloke->dest.x = (x - 10) << 8;
                bloke->dest.y = (y << 8) - 0xc9c;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 9:
                if (f10-- == 0) {
                    f2c = 0;
                }
                bloke->param_action++;
                break;
            case 10:
                if (bloke->field_5c-- < 0) {
                    bloke->param_action++;
                } else if (bloke->field_5c == 250) {
                    BuyItem((struct Element *)obj, tile, 1);
                }
                break;
            case 11:
                if (f18 <= 1 && f11 < 3) {
                    bloke->pos.x = (x << 8) - 0xa9c;
                    bloke->pos.y = (y << 8) - 0xd2c + f11 * 100;
                    bloke->param_action++;
                    f11++;
                }
                break;
            case 12:
                if (f11 != 0 && f18 == 0 && fc == 0 && f1c++ > 100) {
                    f18 = 2;
                    f7 = 0;
                    f38 = 0x143;
                    f3c = 0;
                    f2c = 0;
                    f28 = 0;
                }
                if (f18 == 2 && f28 == 0) {
                    ShiftRestaurant2RiderDiagonally(node, 2, f7);
                } else if (f28 != 0) {
                    bloke->param_action++;
                }
                break;
            case 13:
                if (f30 != 0) {
                    bloke->dest.x = (x - 2) << 8;
                    bloke->dest.y = (y << 8) - 0x46a;
                    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->low_level_action = 7;
                    bloke->target_dir = dir + 0x10;
                    NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                    bloke->param_action++;
                }
                break;
            case 14:
                bloke->dest.x = (x - 3) << 8;
                bloke->dest.y = (y << 8) - 0x46a;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->speed = (unsigned char)bloke->field_44;
                bloke->param_action++;
                f11--;
                break;
            case 15:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= 0xfff7;
                break;
            }
        }
        st->field_7 = f7;
        st->field_8 = f8;
        st->field_c = fc;
        st->field_10 = f10;
        st->field_11 = f11;
        st->field_14 = f14;
        st->field_18 = f18;
        st->field_1c = f1c;
        st->field_20 = f20;
        st->field_24 = f24;
        st->field_28 = f28;
        st->field_2c = f2c;
        st->field_30 = f30;
        st->field_38 = f38;
        st->field_3c = f3c;
        node = next;
    }

    /* animate each restaurant's lift/doors state machine */
    for (st = SaveBlockList; st != NULL; st = st->next) {
        f9 = st->field_9;
        f6 = st->field_6;
        f7 = st->field_7;
        f8 = st->field_8;
        fc = st->field_c;
        f10 = st->field_10;
        f11 = st->field_11;
        state = st->field_18;
        f1c = st->field_1c;
        f20 = st->field_20;
        f24 = st->field_24;
        lift = st->field_28;
        f2c = st->field_2c;
        f30 = st->field_30;
        f34 = st->field_34;
        f38 = st->field_38;
        f3c = st->field_3c;
        switch (state) {
        case 0:
            if (fc != 0) {
                if (f8 <= 8) {
                    f8++;
                } else {
                    state = 1;
                    f8 = 8;
                    f20 = 1;
                }
            }
            break;
        case 1:
            if (f24 != 0) {
                f20 = 0;
                if (f8 >= 0) {
                    f8--;
                } else {
                    f24 = 0;
                    state = 2;
                    f6 = 0;
                    f7 = 0;
                    f38 = 0x143;
                    f3c = 0;
                    PlayOctopusStartSfx(st->value);
                }
            }
            break;
        case 2:
            if (f7 <= 0x20) {
                f7++;
                f38 -= (&Restaurant2DownStepsBase)[f7];
                f3c += Restaurant2UpSteps[f7];
            } else {
                lift = 1;
                f1c = 0;
                state = 4;
                f6 = 0;
                PlayOctopusStopSfx(st->value);
            }
            break;
        case 4:
            if (f6 <= 8) {
                f6++;
            } else {
                state = 5;
                f2c = 1;
                f30 = 1;
                f6 = 8;
            }
            break;
        case 5:
            if (f10 <= 0 && f11 <= 0) {
                f30 = 0;
                f2c = 0;
                if (f6 >= 0) {
                    f6--;
                } else {
                    f34 = 1;
                    state = 3;
                    f7 = 0x20;
                    PlayOctopusStartSfx(st->value);
                }
            }
            break;
        case 3:
            if (f34 != 0) {
                if (f7 >= 0) {
                    lift = 0;
                    f7--;
                } else {
                    lift = 0;
                    state = 0;
                    f6 = 0;
                    f34 = 0;
                    f7 = 0;
                    f8 = 0;
                    PlayOctopusStopSfx(st->value);
                }
            }
            break;
        }
        f9++;
        if (f9 > 0x1f) {
            f9 = 0;
        }
        st->field_6 = f6;
        st->field_7 = f7;
        st->field_8 = f8;
        st->field_9 = f9;
        st->field_c = fc;
        st->field_10 = f10;
        st->field_11 = f11;
        st->field_18 = state;
        st->field_1c = f1c;
        st->field_20 = f20;
        st->field_24 = f24;
        st->field_28 = lift;
        st->field_2c = f2c;
        st->field_30 = f30;
        st->field_34 = f34;
        st->field_38 = f38;
        st->field_3c = f3c;
    }
}

// FUNCTION: LEGOLAND 0x004304a0
struct RideSpriteInfo *GetRestaurant2SpriteInfo(struct Element *obj, unsigned short a2) {
    struct Ride *fx = obj->ride;
    struct Sprite *inner = fx->layer;
    EaterySpriteInfo.sprite = inner;
    EaterySpriteInfo.x = fx->field_14;
    EaterySpriteInfo.y = fx->field_18;
    EaterySpriteInfo.id = a2;
    inner = fx->layer;
    inner->flags |= 0x2000;
    return &EaterySpriteInfo;
}

// FUNCTION: LEGOLAND 0x004304e0
void RenderEateryBaseLayers(unsigned short *param_1, struct Ride *param_2, unsigned int param_3) {
    struct SaveBlock *state;
    struct Point coords;
    struct Point off;
    struct HitInfo cfg;
    char c8;
    char c9;
    char c6;
    char c7;
    int sx;
    int sy;
    volatile int coord_y; /* volatile: the original re-reads this from its stack slot */
    cfg.type = 0x103;
    cfg.element = param_2->element;
    cfg.coords = *param_1;
    state = FindSaveBlock(param_1);
    if (state == NULL) {
        return;
    }
    coords = GetScreenCoordsForObject((TileId *)param_1, param_2);
    sx = coords.x;
    c9 = state->field_9;
    coord_y = coords.y;
    sy = coord_y;
    c8 = state->field_8;
    c6 = state->field_6;
    c7 = state->field_7;
    switch (state->field_18) {
    case 0:
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 1), c7);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 1);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 1), off.x + sx, off.y + sy, param_3, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 3), c9);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 3);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 3), off.x + sx, off.y + sy, param_3, (int *)&cfg);
        if (state->field_c == 0) {
            return;
        }
        goto set_l6;
    case 1:
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 1), c7);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 1);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 1), off.x + sx, off.y + sy, param_3, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 3), c9);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 3);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 3), off.x + sx, off.y + sy, param_3, (int *)&cfg);
    set_l6:
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 6), c8);
        return;
    case 2:
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 1), c7);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 1);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 1), off.x + sx, off.y + sy, param_3, (int *)&cfg);
        return;
    case 4:
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 1), c7);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 1);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 1), off.x + sx, off.y + sy, param_3, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 2), c6);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 2);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 2), off.x + sx, off.y + sy, param_3, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 0), c6);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 0);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 0), off.x + sx, off.y + sy, param_3, (int *)&cfg);
        return;
    case 5:
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 1), c7);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 1);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 1), off.x + sx, off.y + sy, param_3, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 2), c6);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 2);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 2), off.x + sx, off.y + sy, param_3, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 0), c6);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 0);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 0), off.x + sx, off.y + sy, param_3, (int *)&cfg);
        return;
    case 3:
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 1), c7);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 1);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 1), off.x + sx, off.y + sy, param_3, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 3), c9);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 3);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 3), off.x + sx, off.y + sy, param_3, (int *)&cfg);
        return;
    }
}

// FUNCTION: LEGOLAND 0x00430b10
void RenderRestaurant2(struct Element *param_1, unsigned int param_2, unsigned int param_3, short *param_4, void *param_5, unsigned int param_6) {
    struct Ride *ride = param_1->ride;
    struct RideNode *node = ride->riders;
    struct Bloke **p;
    int i;
    int count;
    struct SaveBlock *state;
    char c9;
    int s18;
    unsigned int f11;
    int f38;
    int f3c;
    struct Bloke *blokes[30];
    struct Point coords;
    struct Point off;
    struct {
        /* 0x00 */ int field_0;
        /* 0x04 */ struct Element *field_4;
        /* 0x08 */ short field_8;
    } cfg;
    int sx;
    int sy;
    cfg.field_4 = param_1;
    cfg.field_8 = *param_4;
    p = blokes;
    blokes[0] = 0;
    for (i = 0x1d; p = p + 1, i != 0; i = i - 1) {
        *p = NULL;
    }
    cfg.field_0 = 0x103;
    count = 0;
    state = FindSaveBlock(param_4);
    if (state == NULL) {
        return;
    }
    c9 = state->field_9;
    s18 = state->field_18;
    f11 = state->field_11;
    f38 = state->field_38;
    f3c = state->field_3c;
    RenderEateryBaseLayers((unsigned short *)param_4, ride, param_6);
    coords = GetScreenCoordsForObject((TileId *)param_4, ride);
    if (node == NULL) {
        return;
    }
    p = blokes;
    do {
        if (*param_4 == (short)node->tile.id) {
            count = count + 1;
            *p = node->rider;
            p = p + 1;
        }
        node = node->next;
    } while (node != NULL);
    if (count == 0) {
        return;
    }
    sx = coords.x;
    sy = coords.y;
    if (s18 == 0 || s18 == 1) {
        off = GetRenderOffsetForLayer(EateryLayerOwner, 5);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 5), off.x + sx, off.y + sy, param_6, (int *)&cfg);
        for (i = 0; i < count; i = i + 1) {
            if (blokes[i]->param_action == 5) IP_RenderBlokeIn3DNow(blokes[i]);
        }
        for (i = 0; i < count; i = i + 1) {
            if (blokes[i]->param_action == 6) IP_RenderBlokeIn3DNow(blokes[i]);
        }
        off = GetRenderOffsetForLayer(EateryLayerOwner, 6);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 6), off.x + sx, off.y + sy, param_6, (int *)&cfg);
        for (i = 0; i < count; i = i + 1) {
            if (blokes[i]->param_action == 4) IP_RenderBlokeIn3DNow(blokes[i]);
        }
        PrintSprite(R2Fdoorm1Sprite, off.x + sx, off.y + sy, param_6, (int *)&cfg);
    } else if (s18 == 2) {
        if (f11 != 0) {
            for (i = 0; i < count; i = i + 1) {
                if (blokes[i]->param_action == 0xc) IP_RenderBlokeIn3DNow(blokes[i]);
            }
            PrintSprite(R2BdoormSprite, sx, f3c / 2 + sy, param_6, (int *)&cfg);
        }
        PrintSprite(R2TowermSprite, sx, sy, param_6, (int *)&cfg);
        for (i = 0; i < count; i = i + 1) {
            if (blokes[i]->param_action == 6) IP_RenderBlokeIn3DNow(blokes[i]);
        }
        off = GetRenderOffsetForLayer(EateryLayerOwner, 6);
        AdjustOffsetForViewMode(&off);
        PrintSprite(R2FdoormSprite, off.x + sx, f38 / 2 + sy, param_6, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 3), c9);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 3);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 3), off.x + sx, off.y + sy, param_6, (int *)&cfg);
    } else if (s18 == 5 || s18 == 4) {
        for (i = 0; i < count; i = i + 1) {
            if (blokes[i]->param_action == 7) IP_RenderBlokeIn3DNow(blokes[i]);
        }
        for (i = 0; i < count; i = i + 1) {
            if (blokes[i]->param_action == 8) IP_RenderBlokeIn3DNow(blokes[i]);
        }
        for (i = 0; i < count; i = i + 1) {
            if (blokes[i]->param_action == 9) IP_RenderBlokeIn3DNow(blokes[i]);
        }
        off = GetRenderOffsetForLayer(EateryLayerOwner, 0);
        AdjustOffsetForViewMode(&off);
        for (i = 0; i < count; i = i + 1) {
            if (blokes[i]->param_action == 0xd) IP_RenderBlokeIn3DNow(blokes[i]);
        }
        for (i = 0; i < count; i = i + 1) {
            if (blokes[i]->param_action == 0xe) IP_RenderBlokeIn3DNow(blokes[i]);
        }
        for (i = 0; i < count; i = i + 1) {
            if (blokes[i]->param_action == 0xf) IP_RenderBlokeIn3DNow(blokes[i]);
        }
        PrintSprite(R2BdoormSprite, sx, off.y + sy, param_6, (int *)&cfg);
        PrintSprite(R2TowermSprite, sx, sy, param_6, (int *)&cfg);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 2);
        AdjustOffsetForViewMode(&off);
        PrintSprite(R2FdoormSprite, off.x + sx, off.y + sy, param_6, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(EateryLayerOwner, 3), c9);
        off = GetRenderOffsetForLayer(EateryLayerOwner, 3);
        AdjustOffsetForViewMode(&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer(EateryLayerOwner, 3), off.x + sx, off.y + sy, param_6, (int *)&cfg);
    }
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->param_action == 0) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->param_action == 1) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->param_action == 2) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->param_action == 3) IP_RenderBlokeIn3DNow(blokes[i]);
    }
}

// FUNCTION: LEGOLAND 0x00431120
void UnloadRestaurant2Resources(void) {
    Kill_FXList(OCTOPUS_SFX, 3);
    KillMoneySFX();
    KillSprite(R2FdoormSprite);
    KillSprite(R2BdoormSprite);
    KillSprite(R2TowermSprite);
    KillSprite(R2Fdoorm1Sprite);
}

// FUNCTION: LEGOLAND 0x00431170
void FoodcartIcecreamUpdate(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    struct RideNode *next;
    struct Bloke *bloke;
    TileId *tile;
    int x;
    int y;
    char dir;

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
                bloke->dest.x = (x << 8) + 0x80;
                bloke->dest.y = (y << 8) - 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 1:
                bloke->param_action++;
                bloke->field_58 = (rand() & 0x1f) + 4;
                break;
            case 2:
                if (bloke->field_58 == 0) {
                    bloke->param_action++;
                    BuyItem((struct Element *)obj, tile, 1);
                }
                bloke->field_58--;
                break;
            case 3:
                bloke->dest.x = (x << 8) + 0x80;
                bloke->dest.y = (y << 8) + 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 4:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= 0xfff7;
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x004312c0
void EateryRemoveObject(struct Element *param_1, TileId tile, struct Cursor *param_3) {
    StandardRemoveObject(param_1, tile, param_3);
    RemoveAllBlokesFromRide(param_1->ride, tile);
    StopMoneySFX(&tile.pos.x);
}

// FUNCTION: LEGOLAND 0x00431300
void LoadOctopusCafeResources(struct Element *obj) {
    struct Sprite *inner;
    OctopusCafeRide = obj->ride;
    OctopusCafeRide->flags |= 0x20;
    inner = OctopusCafeRide->layer;
    inner->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b7100
    OctTentASprite = LoadSprite("OctTentA.lls", 1);
    // STRING: LEGOLAND 0x004b70f0
    OctTentBSprite = LoadSprite("OctTentB.lls", 1);
    // STRING: LEGOLAND 0x004b70e0
    OctTentCSprite = LoadSprite("OctTentC.lls", 1);
    // STRING: LEGOLAND 0x004b70d0
    OctTentDSprite = LoadSprite("OctTentD.lls", 1);
    // STRING: LEGOLAND 0x004b70c0
    OctTentESprite = LoadSprite("OctTentE.lls", 1);
    // STRING: LEGOLAND 0x004b70b0
    OctTentFSprite = LoadSprite("OctTentF.lls", 1);
    // STRING: LEGOLAND 0x004b70a0
    OctTentGSprite = LoadSprite("OctTentG.lls", 1);
    // STRING: LEGOLAND 0x004b7090
    OctTentHSprite = LoadSprite("OctTentH.lls", 1);
    // STRING: LEGOLAND 0x004b7080
    OctoKioskSprite = LoadSprite("OctoKiosk.lls", 1);
    // STRING: LEGOLAND 0x004b7070
    OctTabAASprite = LoadSprite("OctTabAA.lls", 1);
    // STRING: LEGOLAND 0x004b7060
    OctTabABSprite = LoadSprite("OctTabAB.lls", 1);
    // STRING: LEGOLAND 0x004b7050
    OctTabBASprite = LoadSprite("OctTabBA.lls", 1);
    // STRING: LEGOLAND 0x004b7040
    OctTabBBSprite = LoadSprite("OctTabBB.lls", 1);
    // STRING: LEGOLAND 0x004b7030
    OctTabCASprite = LoadSprite("OctTabCA.lls", 1);
    // STRING: LEGOLAND 0x004b7020
    OctTabCBSprite = LoadSprite("OctTabCB.lls", 1);
    // STRING: LEGOLAND 0x004b7010
    OctTabDASprite = LoadSprite("OctTabDA.lls", 1);
    // STRING: LEGOLAND 0x004b7000
    OctTabDBSprite = LoadSprite("OctTabDB.lls", 1);
    // STRING: LEGOLAND 0x004b6ff0
    OctTabEASprite = LoadSprite("OctTabEA.lls", 1);
    // STRING: LEGOLAND 0x004b6fe0
    OctTabEBSprite = LoadSprite("OctTabEB.lls", 1);
    // STRING: LEGOLAND 0x004b6fd0
    OctTabFASprite = LoadSprite("OctTabFA.lls", 1);
    // STRING: LEGOLAND 0x004b6fc0
    OctTabFBSprite = LoadSprite("OctTabFB.lls", 1);
    // STRING: LEGOLAND 0x004b6fb0
    OctTabGASprite = LoadSprite("OctTabGA.lls", 1);
    // STRING: LEGOLAND 0x004b6fa0
    OctTabGBSprite = LoadSprite("OctTabGB.lls", 1);
    // STRING: LEGOLAND 0x004b6f90
    OctTabHASprite = LoadSprite("OctTabHA.lls", 1);
    // STRING: LEGOLAND 0x004b6f80
    OctTabHBSprite = LoadSprite("OctTabHB.lls", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x004314f0
void OctopusCafeAddObject(struct Element *param_1, struct UserFlagsArg *param_2) {
    AddBasicObject(param_1, (int *)param_2);
    Set_UserFlags(param_2->var_0, param_2->var_4, 0);
}

// FUNCTION: LEGOLAND 0x00431520
void UnloadOctopusCafeResources(void) {
    if (OctTentASprite) {
        KillSprite(OctTentASprite);
    }
    if (OctTentBSprite) {
        KillSprite(OctTentBSprite);
    }
    if (OctTentCSprite) {
        KillSprite(OctTentCSprite);
    }
    if (OctTentDSprite) {
        KillSprite(OctTentDSprite);
    }
    if (OctTentESprite) {
        KillSprite(OctTentESprite);
    }
    if (OctTentFSprite) {
        KillSprite(OctTentFSprite);
    }
    if (OctTentGSprite) {
        KillSprite(OctTentGSprite);
    }
    if (OctTentHSprite) {
        KillSprite(OctTentHSprite);
    }
    if (OctoKioskSprite) {
        KillSprite(OctoKioskSprite);
    }
    if (OctTabAASprite) {
        KillSprite(OctTabAASprite);
    }
    if (OctTabABSprite) {
        KillSprite(OctTabABSprite);
    }
    if (OctTabBASprite) {
        KillSprite(OctTabBASprite);
    }
    if (OctTabBBSprite) {
        KillSprite(OctTabBBSprite);
    }
    if (OctTabCASprite) {
        KillSprite(OctTabCASprite);
    }
    if (OctTabCBSprite) {
        KillSprite(OctTabCBSprite);
    }
    if (OctTabDASprite) {
        KillSprite(OctTabDASprite);
    }
    if (OctTabDBSprite) {
        KillSprite(OctTabDBSprite);
    }
    if (OctTabEASprite) {
        KillSprite(OctTabEASprite);
    }
    if (OctTabEBSprite) {
        KillSprite(OctTabEBSprite);
    }
    if (OctTabFASprite) {
        KillSprite(OctTabFASprite);
    }
    if (OctTabFBSprite) {
        KillSprite(OctTabFBSprite);
    }
    if (OctTabGASprite) {
        KillSprite(OctTabGASprite);
    }
    if (OctTabGBSprite) {
        KillSprite(OctTabGBSprite);
    }
    if (OctTabHASprite) {
        KillSprite(OctTabHASprite);
    }
    if (OctTabHBSprite) {
        KillSprite(OctTabHBSprite);
    }
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x004316f0
void OctopusCafeUpdate(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *node;
    struct RideNode *next;
    struct Bloke *bloke;
    TileId *tile;
    int seat;
    char dir;
    int spot;
    int chair;
    Point off;
    int idx;

    /* walk every bloke in the cafe through its scripted visit */
    for (node = ride->riders; node != NULL; node = next) {
        bloke = node->rider;
        next = node->next;
        tile = &node->tile;
        if (bloke->low_level_action != 0) {
            continue;
        }
        switch (bloke->param_action) {
        case 0:
            /* claim the next seat of this table */
            seat = Get_UserFlags(tile->pos.x << 8, tile->pos.y << 8);
            bloke->flags |= 8;
            bloke->field_36 = (unsigned char)seat;
            bloke->field_40 = 0;
            bloke->field_5c = 50;
            bloke->param_action++;
            Set_UserFlags(tile->pos.x << 8, tile->pos.y << 8, (seat + 1) & 0x1f);
            continue;
        case 1:
        case 2:
            if (bloke->param_action == 2 && bloke->field_5c-- >= 0) {
                continue;
            }
            /* fall through */
        case 3:
        case 4:
            BuyItem((struct Element *)obj, tile, 1);
            idx = OctopusCafeEntryPath[bloke->param_action + (bloke->field_36 >> 1) * 4];
            if (idx == -1) {
                bloke->param_action++;
                continue;
            }
            break;
        case 5:
            if (bloke->field_5c-- < 0) {
                bloke->param_action++;
            }
            continue;
        case 6:
            /* walk to the chair */
            spot = OctopusCafeSpotIndices[(bloke->field_36 >> 1) * 4];
            bloke->pos = bloke->dest;
            chair = OctopusCafeChairIndices[bloke->field_36];
            off.x = OctopusCafeChairOffsets[chair * 4];
            off.y = OctopusCafeChairOffsets[chair * 4 + 1];
            bloke->dest.x = OctopusCafeSpotOffsets[spot * 2] + (tile->pos.x << 8) - 0x80 + off.x;
            bloke->dest.y = (tile->pos.y << 8) + OctopusCafeSpotOffsets[spot * 2 + 1] + 0x80 + off.y;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->target_dir = dir + 0x10;
            bloke->low_level_action = 7;
            NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
            bloke->param_action++;
            continue;
        case 7:
            NewDirForAction(bloke, (bloke->dir - 4) & 7);
            bloke->param_action++;
            continue;
        case 8:
            /* sit down on the chair */
            bloke->pos = bloke->dest;
            bloke->field_40 = 1;
            chair = OctopusCafeChairIndices[bloke->field_36];
            off.x = OctopusCafeChairOffsets[chair * 4 + 2];
            off.y = OctopusCafeChairOffsets[chair * 4 + 3];
            spot = OctopusCafeSpotIndices[(bloke->field_36 >> 1) * 4];
            bloke->dest.x = OctopusCafeSpotOffsets[spot * 2] + (tile->pos.x << 8) - 0x80 + off.x;
            bloke->dest.y = (tile->pos.y << 8) + OctopusCafeSpotOffsets[spot * 2 + 1] + 0x80 + off.y;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->target_dir = dir + 0x10;
            bloke->low_level_action = 7;
            bloke->param_action++;
            bloke->field_5c = 200;
            continue;
        case 9:
            bloke->flags |= 0x100;
            bloke->pos = bloke->dest;
            bloke->height = 10;
            BlokeSitAnim(bloke);
            BlokeSetFrame(bloke, 0);
            bloke->param_action++;
            continue;
        case 10:
            if (bloke->field_5c-- < 0) {
                bloke->param_action++;
            }
            continue;
        case 11:
            /* get up again */
            bloke->flags &= ~0x100;
            bloke->height = 0;
            BlokeWalkAnim(bloke);
            chair = OctopusCafeChairIndices[bloke->field_36];
            off.x = OctopusCafeChairOffsets[chair * 4];
            off.y = OctopusCafeChairOffsets[chair * 4 + 1];
            spot = OctopusCafeSpotIndices[(bloke->field_36 >> 1) * 4];
            bloke->dest.x = OctopusCafeSpotOffsets[spot * 2] + (tile->pos.x << 8) - 0x80 + off.x;
            bloke->dest.y = (tile->pos.y << 8) + OctopusCafeSpotOffsets[spot * 2 + 1] + 0x80 + off.y;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->target_dir = dir + 0x10;
            bloke->low_level_action = 7;
            bloke->param_action++;
            continue;
        case 12:
            bloke->field_40 = 0;
            idx = OctopusCafeSpotIndices[(bloke->field_36 >> 1) * 4];
            break;
        case 13:
        case 14:
        case 15:
        case 16:
            /* walk out along the table's exit path, skipping unused points */
            do {
                idx = OctopusCafeExitPath[(bloke->field_36 >> 1) * 4 - bloke->param_action];
                bloke->param_action++;
            } while (idx == -1);
            bloke->dest.x = OctopusCafeSpotOffsets[idx * 2] - 0x80 + (tile->pos.x << 8);
            bloke->dest.y = (tile->pos.y << 8) + 0x80 + OctopusCafeSpotOffsets[idx * 2 + 1];
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->low_level_action = 7;
            bloke->target_dir = dir + 0x10;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            continue;
        case 17:
            BlokeWalkAnim(bloke);
            bloke->dest.x = ((ride->x + tile->pos.x) << 8) + 0x80;
            bloke->dest.y = ((tile->pos.y + ride->y) << 8) + 0x80;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
            bloke->low_level_action = 7;
            bloke->target_dir = dir + 0x10;
            NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            continue;
        case 18:
            RemoveBlokeFromRide(ride, node);
            bloke->flags &= ~8;
            continue;
        default:
            continue;
        }
        /* walk to waypoint idx of the table */
        bloke->dest.x = OctopusCafeSpotOffsets[idx * 2] - 0x80 + (tile->pos.x << 8);
        bloke->dest.y = (tile->pos.y << 8) + 0x80 + OctopusCafeSpotOffsets[idx * 2 + 1];
        dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
        bloke->target_dir = dir + 0x10;
        bloke->low_level_action = 7;
        NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
        bloke->param_action++;
    }
}

/* Draws the Octopus Cafe in layers: the base sprite, then the first pair of seated blokes and the sprite over them,
 * then the second pair and its overlay sprite (an overlay only when its pair has someone). */
// FUNCTION: LEGOLAND 0x00431c50
void RenderOctopusCafeSeatLayers(unsigned int param_1, unsigned int param_2, struct Sprite *param_3, struct Sprite *param_4, struct Sprite *param_5, struct Bloke *param_6, struct Bloke *param_7, struct Bloke *param_8, struct Bloke *param_9, unsigned int param_10) {
    PrintSprite(param_3, param_1, param_2, param_10, 0);
    if (param_6 != 0 || param_7 != 0) {
        if (param_6 != 0) {
            IP_RenderBlokeIn3DNow(param_6);
        }
        if (param_7 != 0) {
            IP_RenderBlokeIn3DNow(param_7);
        }
        PrintSprite(param_4, param_1, param_2, param_10, 0);
    }
    if (param_8 != 0 || param_9 != 0) {
        if (param_8 != 0) {
            IP_RenderBlokeIn3DNow(param_8);
        }
        if (param_9 != 0) {
            IP_RenderBlokeIn3DNow(param_9);
        }
        PrintSprite(param_5, param_1, param_2, param_10, 0);
    }
}

// FUNCTION: LEGOLAND 0x00431d00
void RenderOctopusCafe(struct Element *param_1, unsigned int param_2, unsigned int param_3, unsigned char *param_4, void *param_5, unsigned int param_6) {
    struct Bloke *buckets[0x20];
    struct Bloke *blokes[0x20];
    int count;
    int i;
    struct Bloke **p;
    struct Bloke *bloke;
    struct RideNode *node;
    TileId *tile = (TileId *)param_4;
    p = buckets;
    buckets[0] = 0;
    for (i = 0x1f; p = p + 1, i != 0; i = i - 1) {
        *p = NULL;
    }
    count = 0;
    node = param_1->ride->riders;
    while (node != NULL) {
        if (tile->id == node->tile.id) {
            bloke = node->rider;
            if (bloke->field_40 != 0) {
                buckets[bloke->field_36] = bloke;
            } else {
                blokes[count] = bloke;
                count = count + 1;
                bloke->field_37 = OctopusCafeDepthMap[(((bloke->pos.x >> 8) - tile->pos.x) * 0xb - (bloke->pos.y >> 8) + tile->pos.y) * 4];
            }
        }
        node = node->next;
    }
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 1) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 2) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    RenderOctopusCafeSeatLayers(param_2, param_3, OctTentESprite, OctTabEASprite, OctTabEBSprite, buckets[14], buckets[15], buckets[12], buckets[13], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 3) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 4) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    RenderOctopusCafeSeatLayers(param_2, param_3, OctTentFSprite, OctTabFASprite, OctTabFBSprite, buckets[17], buckets[18], buckets[16], buckets[19], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 5) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 6) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    RenderOctopusCafeSeatLayers(param_2, param_3, OctTentDSprite, OctTabDASprite, OctTabDBSprite, buckets[10], buckets[11], buckets[8], buckets[9], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 7) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    RenderOctopusCafeSeatLayers(param_2, param_3, OctTentCSprite, OctTabCASprite, OctTabCBSprite, buckets[4], buckets[7], buckets[5], buckets[6], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 8) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 9) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 10) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    PrintSprite(OctoKioskSprite, param_2, param_3, param_6, 0);
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 0xb) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 0xc) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    RenderOctopusCafeSeatLayers(param_2, param_3, OctTentGSprite, OctTabGASprite, OctTabGBSprite, buckets[21], buckets[22], buckets[20], buckets[23], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 0xd) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 0xe) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    RenderOctopusCafeSeatLayers(param_2, param_3, OctTentHSprite, OctTabHASprite, OctTabHBSprite, buckets[24], buckets[25], buckets[26], buckets[27], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 0xf) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 0x10) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    RenderOctopusCafeSeatLayers(param_2, param_3, OctTentBSprite, OctTabBASprite, OctTabBBSprite, buckets[0], buckets[3], buckets[1], buckets[2], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 0x11) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 0x12) IP_RenderBlokeIn3DNow(blokes[i]);
    }
    RenderOctopusCafeSeatLayers(param_2, param_3, OctTentASprite, OctTabAASprite, OctTabABSprite, buckets[28], buckets[29], buckets[30], buckets[31], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (blokes[i]->field_37 == 0x13) IP_RenderBlokeIn3DNow(blokes[i]);
    }
}

// FUNCTION: LEGOLAND 0x004322a0
int Restaurant1_Save(struct Element *element) {
    unsigned int one = 1;
    unsigned int zero = 0;
    struct BrollyNode *node = BrollyNodeList;
    while (node != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(node, sizeof(struct BrollyNode)) == 0) {
            return 0;
        }
        node = node->next;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00432310
int Restaurant1_Load(struct Element *element) {
    unsigned int count;
    struct BrollyNode *prev = NULL;
    struct BrollyNode *node;
    if (!SaveGameRead(&count, 4)) {
        return 0;
    }
    if (count == 0) {
        return 1;
    }
    while (count != 0) {
        node = malloc(sizeof(struct BrollyNode));
        if (!SaveGameRead(node, sizeof(struct BrollyNode))) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            BrollyNodeList = node;
        }
        prev = node;
        if (!SaveGameRead(&count, 4)) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00432390
int Restaurant2_Save(struct Element *element) {
    unsigned int one = 1;
    unsigned int zero = 0;
    struct SaveBlock *node = SaveBlockList;
    while (node != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(node, sizeof(struct SaveBlock)) == 0) {
            return 0;
        }
        node = node->next;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00432400
int Restaurant2_Load(struct Element *element) {
    unsigned int count;
    struct SaveBlock *current;
    struct SaveBlock *prev = NULL;

    if (!SaveGameRead(&count, 4)) {
        return 0;
    }
    if (count == 0) {
        return 1;
    }
    while (count != 0) {
        current = malloc(sizeof(struct SaveBlock));
        if (!SaveGameRead(current, sizeof(struct SaveBlock))) {
            return 0;
        }
        current->next = 0;
        if (prev != NULL) {
            prev->next = current;
        } else {
            SaveBlockList = current;
        }
        prev = current;
        if (!SaveGameRead(&count, 4)) {
            return 0;
        }
    }
    return 1;
}
