#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include <stdio.h>
#include "binv.h"
#include "bloke.h"
#include "gamemap.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "objclass.h"
#include "print_sprite.h"
#include "render3d.h"
#include "sound_music.h"
#include "spider_ride.h"
#include "tilemap.h"

struct SpiderState {
    unsigned short id;
    unsigned char field_2;
    unsigned char field_3;
    char frame;
    unsigned char pad_5[3];
    unsigned int field_8;
    unsigned char field_c;
    unsigned char pad_d[3];
    unsigned int field_10;
    unsigned char field_14;
    unsigned char pad_15[3];
    int field_18;
};

struct SlotArray {
    unsigned char pad_0[0x1c];
    unsigned char slots[1];
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x004158f0
int AddSpiderNode(TileId *key) {
    struct SpiderNode *node = (struct SpiderNode *)malloc(sizeof(struct SpiderNode));
    if (node == NULL) {
        return;
    }
    memset(node, 0, sizeof(struct SpiderNode));
    node->tile_id = key->id;
    node->next = SpiderNodeList;
    SpiderNodeList = node;
    return ResetSpiderRide(node);
}

// FUNCTION: LEGOLAND 0x00415930
void RemoveSpiderNode(struct SpiderNode *node) {
    struct SpiderNode *prev;
    struct SpiderNode *cur;

    if (SpiderNodeList == node) {
        SpiderNodeList = node->next;
    } else {
        cur = SpiderNodeList->next;
        prev = SpiderNodeList;
        while (cur != node) {
            prev = prev->next;
            if (prev == NULL) {
                break;
            }
            cur = prev->next;
        }
        if (prev != NULL) {
            prev->next = node->next;
        }
    }
    free(node);
}

// FUNCTION: LEGOLAND 0x00415990
void FreeAllSpiderNodes(void) {
    struct SpiderNode *node = SpiderNodeList;
    while (node != NULL) {
        RemoveSpiderNode(node);
        node = SpiderNodeList;
    }
}

// FUNCTION: LEGOLAND 0x004159b0
struct SpiderNode *FindSpiderNode(TileId *key) {
    struct SpiderNode *cur = SpiderNodeList;

    if (cur != NULL) {
        do {
            if (memcmp(&cur->tile_id, key, 2) == 0) {
                return cur;
            }
            cur = cur->next;
        } while (cur != NULL);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x004159e0
void PlaySpiderRideSfx(const unsigned char *arg0) {
    struct SampleSource params;
    params.type = 0x2;
    params.x = arg0[0];
    params.y = arg0[1];
    PlayInstanceOfSample(SpiderRide_SFX[0].sample, 1, 1, &params);
}

// FUNCTION: LEGOLAND 0x00415a20
void FadeSpiderRideSfx(TileId *tile) {
    struct SampleSource params;
    params.type = 2;
    params.x = tile->pos.x;
    params.y = tile->pos.y;
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
}

// FUNCTION: LEGOLAND 0x00415a60
void StartSpiderRide(struct SpiderState *a) {
    a->field_3 = a->field_2;
    a->field_2 = 0;
    a->field_8 = (a->field_8 & 0xffffbfff) | 0x1;
    a->frame = 0;
    a->field_10 = 0;
    PlaySpiderRideSfx((const unsigned char *)a);
}

// FUNCTION: LEGOLAND 0x00415a90
int ResetSpiderRide(struct SpiderNode *node) {
    struct SpiderState *a = (struct SpiderState *)node;
    a->field_10 = 0;
    a->frame = 0;
    a->field_c = rand() % 2 != 0 ? 4 : 3;
    a->field_8 &= 0xffffbffe;
    a->field_14 = 0;
    a->field_2 = 0;
    FadeSpiderRideSfx((TileId *)node);
}

// FUNCTION: LEGOLAND 0x00415ae0
void RenderSpider(Element *obj, void *param_2, void *param_3, TileId *tile, void *param_5, unsigned int param_6) {
    Ride *ride = obj->ride;
    RideNode *elem = ride->riders;
    char count = 0;
    Bloke *blokes[16] = {0};
    struct SpiderNode *state;
    Point off;
    Point screen;
    char i;
    unsigned short id;

    state = FindSpiderNode(tile);
    if (state == NULL) {
        return;
    }
    screen = GetScreenCoordsForObject(tile, ride);
    if (elem != NULL) {
        do {
            id = tile->id;
            if (id == elem->tile.id) {
                blokes[count++] = elem->rider;
            }
            elem = elem->next;
        } while (elem != NULL);
        if (count != 0) {
            for (i = 0; i < count; i++) {
                if (blokes[i]->param_action == 14) {
                    IP_RenderBlokeIn3DNow(blokes[i]);
                }
            }
            LLSSetFrame(GetLLSForLayer(SpiderRideLayer, 1), state->frame);
            off = GetRenderOffsetForLayer(SpiderRideLayer, 1);
            AdjustOffsetForViewMode(&off);
            PrintSprite(GetSpriteForLayer(SpiderRideLayer, 1), screen.x + off.x, screen.y + off.y, param_6, 0);
            off = GetRenderOffsetForLayer(SpiderRideLayer, 2);
            AdjustOffsetForViewMode(&off);
            PrintSprite(SpiderHutMask2Sprite, screen.x + off.x, screen.y + off.y, param_6, 0);
            *(short *)*ZSpiderSprite->lls = state->frame;
            for (elem = ride->riders; elem != NULL; elem = elem->next) {
                Bloke *b;
                if (tile->id == elem->tile.id && ((b = elem->rider)->flags & 0x80) != 0) {
                    Person *p = b->person;
                    Point base;
                    Point adj;
                    base = SpiderRideDrawOffset;
                    adj.x = 0;
                    adj.y = 0;
                    if (b->field_35 == 1) {
                        adj = SpiderRideDrawAdjust;
                        AdjustOffsetForViewMode(&adj);
                    }
                    p->offset.x = b->screen_x;
                    p->offset.y = b->screen_y;
                    AdjustBlokePosition(&p->offset);
                    AdjustOffsetForViewMode(&base);
                    p->screen.x = b->screen_x + base.x + screen.x;
                    p->screen.y = b->screen_y + base.y + screen.y;
                    p->screen.x += adj.x;
                    p->screen.y += adj.y;
                    AdjustBlokePosition(&p->screen);
                    IP_RenderBlokeIn3DNow(elem->rider);
                }
            }
            off = GetRenderOffsetForLayer(SpiderRideLayer, 2);
            AdjustOffsetForViewMode(&off);
            PrintSprite(SpiderHutMask1Sprite, screen.x + off.x, screen.y + off.y, param_6, 0);
            return;
        }
    }
    {
        LLSSetFrame(GetLLSForLayer(SpiderRideLayer, 1), state->frame);
        off = GetRenderOffsetForLayer(SpiderRideLayer, 1);
        AdjustOffsetForViewMode(&off);
        PrintSprite(GetSpriteForLayer(SpiderRideLayer, 1), screen.x + off.x, screen.y + off.y, param_6, 0);
        off = GetRenderOffsetForLayer(SpiderRideLayer, 2);
        AdjustOffsetForViewMode(&off);
        PrintSprite(GetSpriteForLayer(SpiderRideLayer, 2), screen.x + off.x, screen.y + off.y, param_6, 0);
    }
}

// FUNCTION: LEGOLAND 0x00415e80
void SpiderRideLoad(struct Element *param_1) {
    ActiveSpiderRide = param_1->ride;
    ActiveSpiderRide->flags |= 0x420;
    SpiderRideLayer = ActiveSpiderRide->layer;
    SpiderRideLayer->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b4ea0
    SpiderRunBinV = LoadBinV("Zbuffers\\spiderrun.bnv");
    // STRING: LEGOLAND 0x004b4e88
    SpiderOnBinV = LoadBinV("Zbuffers\\spideron.bnv");
    // STRING: LEGOLAND 0x004b4e70
    SpiderOffBinV = LoadBinV("Zbuffers\\spideroff.bnv");
    // STRING: LEGOLAND 0x004b4e5c
    SpiderHutMask1Sprite = LoadSprite("SpiderHutMask1.lls", 1);
    // STRING: LEGOLAND 0x004b4e48
    SpiderHutMask2Sprite = LoadSprite("SpiderHutMask2.lls", 1);
    // STRING: LEGOLAND 0x004b4e38
    ZSpiderSprite = LoadSprite("z_spider.lls", 1);
    SpiderRideBinVs[0] = SpiderRunBinV;
    SpiderRideDrawOffset.x = -1;
    SpiderRideDrawOffset.y = 2;
    SpiderRideCarAssets[1] = ZSpiderSprite;
    SpiderRideBinVs[1] = SpiderOnBinV;
    SpiderRideCarAssets[0] = SpiderOffBinV;
    HideLayer(SpiderRideLayer, 2);
    StopLayerPlaying(SpiderRideLayer, 2);
    LLSSetFrame(GetLLSForLayer(SpiderRideLayer, 2), 0);
    HideLayer(SpiderRideLayer, 1);
    StopLayerPlaying(SpiderRideLayer, 1);
    LLSSetFrame(GetLLSForLayer(SpiderRideLayer, 1), 0);
    Load_FXList(SpiderRide_SFX, 1);
}

// FUNCTION: LEGOLAND 0x00415fd0
void SpiderRideUnload(struct Element *param_1) {
    ActiveSpiderRide = param_1->ride;

    if (ZSpiderSprite != NULL) {
        KillSprite(ZSpiderSprite);
    }
    if (SpiderRunBinV != 0) {
        FreeBinV(SpiderRunBinV);
    }
    if (SpiderOnBinV != 0) {
        FreeBinV(SpiderOnBinV);
    }
    if (SpiderOffBinV != 0) {
        FreeBinV(SpiderOffBinV);
    }
    KillSprite(SpiderHutMask1Sprite);
    KillSprite(SpiderHutMask2Sprite);
    FreeAllSpiderNodes();
    Kill_FXList(SpiderRide_SFX, 1);
}

// FUNCTION: LEGOLAND 0x00416060
void SpiderSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = (void *)ActiveSpiderRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x004160a0
void SpiderRemoveObject(Element *obj, TileId tile, struct Cursor *cursor) {
    struct SpiderNode *node = FindSpiderNode(&tile);

    if (node != NULL) {
        RemoveSpiderNode(node);
    }
    FadeSpiderRideSfx(&tile);
    StandardRemoveObject(obj, tile, cursor);
    RemoveAllBlokesFromRide(obj->ride, tile);
}

// FUNCTION: LEGOLAND 0x004160f0
void SpiderAddObject(Element *editObj, int *coords) {
    TileId key;

    key.pos.x = coords[0];
    key.pos.y = coords[1];
    AddBasicObject(editObj, coords);
    AddSpiderNode(&key);
}

// FUNCTION: LEGOLAND 0x00416120
struct RideSpriteInfo *GetSpiderRideSpriteInfo(struct Element *a1, unsigned short a2) {
    struct Ride *ride = a1->ride;
    SpiderRideSpriteInfo.sprite = ride->layer;
    SpiderRideSpriteInfo.x = ride->field_14;
    SpiderRideSpriteInfo.y = ride->field_18;
    SpiderRideSpriteInfo.id = a2;
    ride->layer->flags = ride->layer->flags | 0x2000;
    return &SpiderRideSpriteInfo;
}

// FUNCTION: LEGOLAND 0x00416160
void SpiderRide(struct Element *name_ptr, struct Ride *obj) {
    // STRING: LEGOLAND 0x004b4eb8
    if (_stricmp("SPIDER RIDE", name_ptr->name) == 0) {
        obj->cb_a4 = SpiderRideLoad;
        obj->cb_ac = SpiderRideUnload;
        obj->cb_8c = SpiderSetEditMode;
        obj->cb_pre_render = SpiderRideUpdate;
        obj->cb_b0 = RenderSpider;
        obj->cb_9c = SpiderRemoveObject;
        obj->cb_98 = SpiderAddObject;
        obj->cb_sprite = GetSpiderRideSpriteInfo;
        obj->save_hook = SaveSpider;
        obj->load_hook = LoadSpider;
    }
}

// FUNCTION: LEGOLAND 0x004161f0
void UpdateSpiderNode(struct SpiderNode *node) {
    struct SpiderState *s = (struct SpiderState *)node;
    struct RideNode *r = ActiveSpiderRide->riders;
    unsigned int flags = s->field_8;

    if (flags & 1) {
        unsigned char c;
        int v = ++s->field_10;
        c = s->field_c;
        if (c == 0) {
            if (GetAllBlokesOffRide((struct Ride *)ActiveSpiderRide, s->id) == 0) {
                return;
            }
            ResetSpiderRide(node);
            return;
        }
        if (v >= 2) {
            s->field_10 = 0;
            s->frame++;
            if (s->frame >= 0x20) {
                s->frame = 0;
                s->field_c = c - 1;
            }
        }
    } else if (flags & 0x4000) {
        if (s->field_2 == s->field_14) {
            s->field_8 = flags & 0xffffbfff;
            StartSpiderRide(s);
            return;
        }
    } else if (s->field_2 != 0) {
        if (s->field_18 == 0) {
            s->field_8 = flags | 0x4000;
            Ride_SetFlagToNotLetAnyoneOn(s);
        }
        s->field_18--;
    }
    for (; r != NULL; r = r->next) {
        if (s->id == r->tile.id && r->rider->field_35 == 1) {
            sprintf(SpiderBnvInfo.name + 6, "%02d", r->rider->field_36);
            SetBlokePositionFromBNV(SpiderRunBinV, r->rider, SpiderBnvInfo.name, s->frame, -1617787.75f, -1618096.5f, 0);
        }
    }
    *(short *)*ZSpiderSprite->lls = (short)s->frame;
}

// FUNCTION: LEGOLAND 0x00416310
void UpdateSpiderNodes(void) {
    struct SpiderNode *node = SpiderNodeList;
    while (node != NULL) {
        UpdateSpiderNode(node);
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00416330
void SpiderRideUpdate(Element *obj) {
    struct Ride *ride = obj->ride;
    struct RideNode *elem = ride->riders;
    struct RideNode *next;
    struct SpiderState *state;
    struct Bloke *bloke;
    TileId *tile;
    struct Point sc; /* screen position of the ride */
    struct Point off; /* bloke's map position as a screen offset */
    int w, h;
    int ox, oy;
    int ex, ey;
    int coords[3]; /* BNV start position (boarding path) */
    int walk[3]; /* BNV start position (leaving path) */
    char dir;

    UpdateSpiderNodes();
    while (elem != NULL) {
        next = elem->next;
        bloke = elem->rider;
        tile = &elem->tile;
        state = (struct SpiderState *)FindSpiderNode(tile);
        if (state == NULL) {
            return;
        }
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0: {
                int dx, dy;
                state->field_14++;
                state->field_18 = 0xb4;
                bloke->flags |= 8;
                sc = GetScreenCoordsForObject(tile, ride);
                dy = bloke->pos.y;
                dx = bloke->pos.x;
                GetTileDimensions(&w, &h);
                off.x = (dx - dy) * w >> 9;
                off.y = (dx + dy) * h >> 9;
                ox = lpConfig->view_x - (short)Get_XScroll() + off.x;
                oy = off.y + (lpConfig->view_y - (short)Get_YScroll());
                bloke->flags |= 0x80;
                bloke->person->field_30 = 1;
                coords[0] = (ox - SpiderRideDrawOffset.x / 2 - sc.x) * 2;
                coords[1] = (oy - SpiderRideDrawOffset.y / 2 - sc.y) * 2;
                bloke->person->sprite = (struct Sprite *)SpiderRideCarAssets[1];
                bloke->person->depth = GetUnitDepth(-1617787.75f, -1618096.5f);
                bloke->field_35 = 0;
                // STRING: LEGOLAND 0x004b4704
                sprintf(SpiderBnvInfo.name + 6, "%02d", ReserveSpiderRideSeat((struct RideNode *)elem, (struct SlotArray *)state, ActiveSpiderRide->seats));
                bloke->path = NewBNVPath(SpiderRideBinVs[1], 1, SpiderBnvInfo.name, -1617787.75f, -1618096.5f, coords);
                UpdateBlokeFromBNVPath(bloke, bloke->path);
                bloke->field_58 = 0;
                bloke->param_action++;
                break;
            }
            case 1:
                if (UpdateBlokeFromBNVPath(bloke, bloke->path) == 0) {
                    bloke->field_35 = 1;
                    bloke->param_action = 5;
                    free(bloke->path);
                    bloke->path = NULL;
                }
                if (bloke->path != NULL && (int)BNVPath_GetDFrame(bloke->path) >= SpiderBnvInfo.tab1[bloke->field_36]) {
                    bloke->field_35 = 1;
                    bloke->param_action = 5;
                    free(bloke->path);
                    bloke->path = NULL;
                }
                BlokeSetFrame(bloke, bloke->frame);
                break;
            case 5:
                bloke->flags |= 0x80;
                BlokeSitAnim(bloke);
                BlokeSetFrame(bloke, 0);
                bloke->field_35 = 1;
                bloke->person->sprite = (struct Sprite *)SpiderRideCarAssets[1];
                bloke->person->field_30 = 1;
                bloke->person->depth = GetUnitDepth(-1617787.75f, -1618096.5f);
                bloke->param_action++;
                state->field_2++;
                if ((short)(char)state->field_2 == ActiveSpiderRide->seats) {
                    StartSpiderRide(state);
                }
                break;
            case 7:
                walk[0] = bloke->screen_x << 1;
                walk[1] = bloke->screen_y << 1;
                BlokeWalkAnim(bloke);
                BlokeSetFrame(bloke, 0);
                bloke->flags |= 0x80;
                bloke->person->sprite = (struct Sprite *)SpiderRideCarAssets[1];
                bloke->person->field_30 = 1;
                bloke->person->depth = GetUnitDepth(-1617787.75f, -1618096.5f);
                bloke->field_35 = 2;
                sprintf(SpiderBnvInfo.name + 6, "%02d", bloke->field_36);
                bloke->path = NewBNVPath(SpiderRideCarAssets[0], 2, SpiderBnvInfo.name, -1617787.75f, -1618096.5f, walk);
                BNVPath_SetDFrame(bloke, bloke->path, 0);
                UpdateBlokeFromBNVPath(bloke, bloke->path);
                bloke->param_action++;
                break;
            case 8:
                if (UpdateBlokeFromBNVPath(bloke, bloke->path) == 0) {
                    bloke->field_35 = 2;
                    bloke->param_action = 0xd;
                    free(bloke->path);
                    bloke->path = NULL;
                }
                if (bloke->path != NULL && (int)BNVPath_GetDFrame(bloke->path) >= SpiderBnvInfo.tab2[bloke->field_36]) {
                    bloke->field_35 = 2;
                    bloke->param_action = 0xd;
                    free(bloke->path);
                    bloke->path = NULL;
                }
                BlokeSetFrame(bloke, bloke->frame);
                break;
            case 0xd:
                ex = ride->field_24 + tile->pos.x;
                ey = tile->pos.y + ride->field_25;
                ((unsigned char *)state)[0x1b + bloke->field_36] = 0;
                bloke->flags &= 0xff7f;
                bloke->person->sprite = NULL;
                bloke->person->field_30 = 0;
                UnAdjustBlokePosition(&bloke->person->screen);
                ScreenToMapRef(&bloke->person->screen.x, &bloke->pos.x, 0);
                bloke->person->field_34 = 0;
                bloke->pos.x <<= 8;
                bloke->pos.y <<= 8;
                bloke->dest.x = ex << 8;
                bloke->dest.y = ey << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 0xe:
                RemoveBlokeFromRide(ride, elem);
                bloke->flags &= 0xfff7;
                if (--state->field_3 == 0) {
                    state->field_2 = 0;
                    Ride_ClearFlagToNotLetAnyoneOn(state);
                }
                break;
            }
        }
        elem = next;
    }
}

/* Searches cyclically from a random seat for a free slot, marks it occupied, and stores and returns its one-based
 * seat number for the rider. */
// FUNCTION: LEGOLAND 0x00416830
int ReserveSpiderRideSeat(struct RideNode *owner, struct SlotArray *arr, signed char count) {
    int n = count;
    int i = rand() % n;

    if (arr->slots[i] != 0) {
        do {
            i++;
            if (i >= n) {
                i = 0;
            }
        } while (arr->slots[i] != 0);
    }

    arr->slots[i] = 1;
    owner->rider->field_36 = (unsigned char)(i + 1);
    return i + 1;
}

// FUNCTION: LEGOLAND 0x00416880
LEGO_EXPORT int SaveSpider(struct Element *element) {
    struct SpiderNode *node = SpiderNodeList;
    unsigned int one = 1;
    unsigned int zero = 0;

    while (node != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(node, sizeof(struct SpiderNode)) == 0) {
            return 0;
        }
        node = node->next;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x004168f0
LEGO_EXPORT int LoadSpider(struct Element *arg) {
    struct Ride *obj = arg->ride;
    struct SpiderNode *prev = NULL;
    struct RideNode *list;
    struct Person *car;
    struct Bloke *data;
    struct BNVRef *tc;
    unsigned int marker;

    if (!SaveGameRead(&marker, 4)) {
        return 0;
    }
    while (marker != 0) {
        struct SpiderNode *node = (struct SpiderNode *)malloc(sizeof(struct SpiderNode));
        if (!SaveGameRead(node, sizeof(struct SpiderNode))) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            SpiderNodeList = node;
        }
        prev = node;
        if (!SaveGameRead(&marker, 4)) {
            return 0;
        }
    }

    list = obj->riders;
    while (list != NULL) {
        car = list->person;
        if (car->field_30 != 0) {
            car->sprite = SpiderRideCarAssets[car->field_30];
        } else {
            car->sprite = NULL;
            list->person->field_30 = 0;
        }
        data = list->rider;
        tc = data->bnv;
        if (tc != NULL) {
            tc->file = SpiderRideBinVs[tc->index];
        }
        list = list->next;
    }
    return 1;
}
