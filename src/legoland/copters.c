#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "bloke.h"
#include "copters.h"
#include "gamemap.h"
#include "globals.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "obj_instance.h"
#include "objclass.h"
#include "print_sprite.h"
#include "render3d.h"
#include "ride_queue.h"
#include "sound_music.h"
#include "sound_sfx.h"

struct CopterSource {
    unsigned short field_0;
};

// Same object as CopterNode, viewed with the two id bytes split apart.
struct CopterSfxNode {
    /* 0x00 */ unsigned char tile_x;
    /* 0x01 */ unsigned char tile_y;
    /* 0x02 */ unsigned char field_2;
    /* 0x03 */ unsigned char field_3;
    /* 0x04 */ struct CopterNode *next;
    /* 0x08 */ unsigned int field_8;
    /* 0x0c */ int field_c;
    /* 0x10 */ unsigned char field_10;
    /* 0x11 */ unsigned char pad_11[3];
    /* 0x14 */ int field_14;
    /* 0x18 */ struct CopterLayer layer[6];
};

#include "image_sprite.h"

// GLOBAL: LEGOLAND 0x004b4198
static struct PathPair Copters_PathPairs0[6] = {{0, -1}, {1, 0}, {1, 0}, {1, 0}, {-1, -1}, {-1, -1}};
// GLOBAL: LEGOLAND 0x004b41c8
struct PathTable CopterPathTable0 = {6, Copters_PathPairs0};
// GLOBAL: LEGOLAND 0x004b41d0
static struct PathPair Copters_PathPairs1[7] = {{0, -1}, {-1, 0}, {0, -1}, {0, -4}, {-3, -1}, {1, 0}, {1, 0}};
// GLOBAL: LEGOLAND 0x004b4208
struct PathTable CopterPathTable2 = {7, Copters_PathPairs1};
// GLOBAL: LEGOLAND 0x004b4210
static struct PathPair Copters_PathPairs2[6] = {{0, -1}, {-1, -1}, {2, -4}, {2, -2}, {-2, 0}, {0, -1}};
// GLOBAL: LEGOLAND 0x004b4240
struct PathTable CopterPathTable1 = {6, Copters_PathPairs2};
// GLOBAL: LEGOLAND 0x004b4248
static struct PathPair Copters_PathPairs3[5] = {{0, -1}, {4, -1}, {0, -1}, {-1, -1}, {-1, -2}};
// GLOBAL: LEGOLAND 0x004b4270
struct PathTable CopterPathTable3 = {5, Copters_PathPairs3};
// GLOBAL: LEGOLAND 0x004b4278
static struct PathPair Copters_PathPairs4[4] = {{0, -1}, {-1, -1}, {0, -1}, {0, -1}};
// GLOBAL: LEGOLAND 0x004b4298
struct PathTable CopterPathTable4 = {4, Copters_PathPairs4};

// GLOBAL: LEGOLAND 0x004b4170
static const char *Copters_LLSNames[10] = {
    "mcop_gs.lls",
    "mcop_b2s.lls",
    "mcop_rs.lls",
    "mcop_b1s.lls",
    "mcop_ys.lls",
    "mcop_b1m.lls",
    "mcop_gm.lls",
    "mcop_rm.lls",
    "mcop_ym.lls",
    "mcop_b2m.lls",
};

// FUNCTION: LEGOLAND 0x00403c40
void CoptersAddNode(struct CopterSource *src) {
    struct CopterNode *node = (struct CopterNode *)malloc(sizeof(struct CopterNode));
    if (node != NULL) {
        memset(node, 0, sizeof(struct CopterNode));
        node->field_0 = src->field_0;
        node->next = CopterNodeList;
        CopterNodeList = node;
    }
    CoptersInitNode(node);
}

// FUNCTION: LEGOLAND 0x00403c80
void CoptersRemoveNode(struct CopterNode *node) {
    struct CopterNode *prev;
    struct CopterNode *cur;

    if (CopterNodeList == node) {
        CopterNodeList = node->next;
    } else {
        cur = CopterNodeList->next;
        prev = CopterNodeList;
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

// FUNCTION: LEGOLAND 0x00403ce0
void CoptersRemoveAllNodes(void) {
    while (CopterNodeList != NULL) {
        CoptersRemoveNode(CopterNodeList);
    }
}

// FUNCTION: LEGOLAND 0x00403d00
struct CopterNode *CoptersFindNode(struct CopterSource *src) {
    struct CopterNode *node;

    node = CopterNodeList;
    if (node == NULL) {
        return NULL;
    }
    while (memcmp(&node->field_0, &src->field_0, sizeof(node->field_0)) != 0) {
        node = node->next;
        if (node == NULL) {
            return NULL;
        }
    }
    return node;
}

// FUNCTION: LEGOLAND 0x00403d30
int CoptersQueueTableToIndex(struct RideNode *item) {
    struct Bloke *sub = item->rider;
    struct QueueTable **ptr = &CopterQueueTables[0];
    int index = 0;

    while (1) {
        if (sub->queue == *ptr) {
            break;
        }
        ptr++;
        index++;
        if ((INT_PTR)ptr < (INT_PTR)&CopterModelSprites) {
            continue;
        }
        index = -1;
        break;
    }

    sub->queue = (struct QueueTable *)(INT_PTR)index;
    return index;
}

// FUNCTION: LEGOLAND 0x00403d60
void CoptersIndexToQueueTable(struct RideNode *item) {
    struct Bloke *sub = item->rider;
    int index = (int)(INT_PTR)sub->queue;

    if (index < 0 || index >= 6) {
        sub->queue = NULL;
    } else {
        sub->queue = CopterQueueTables[index];
    }
}

// FUNCTION: LEGOLAND 0x00403d90
void CoptersLoad(struct Element *param_1) {
    struct Ride *ride;
    int i;

    ride = param_1->ride;
    ActiveCopterRide = ride;
    ride->flags |= 0x420;
    CopterModelLayers = ActiveCopterRide->layer;
    ((struct Sprite *)CopterModelLayers)->flags |= 0x2000;
    for (i = 0; i < 10; i++) {
        CopterModelSprites[i] = LoadSprite(Copters_LLSNames[i], 1);
    }
    // STRING: LEGOLAND 0x004b43d0
    CoptersPos = LoadPos("3ddata\\copters.pos");
    CopterQueueTables[0] = BuildQueueTable(&CopterPathTable0);
    CopterQueueTables[2] = BuildQueueTable(&CopterPathTable2);
    CopterQueueTables[1] = BuildQueueTable(&CopterPathTable1);
    CopterQueueTables[3] = BuildQueueTable(&CopterPathTable3);
    CopterQueueTables[4] = BuildQueueTable(&CopterPathTable4);
    CoptersRenderList0 = &CoptersRenderListHead0;
    CoptersRenderList1 = &CoptersRenderListHead1;
    CoptersRenderList2 = &CoptersRenderListHead2;
    CoptersRenderList3 = &CoptersRenderListHead3;
    CoptersRenderList4 = &CoptersRenderListHead4;
    // STRING: LEGOLAND 0x004b43bc
    CopterBaseMatteSprite = LoadSprite("cop_base Matte.lls", 1);
    Load_FXList(Helicopter_SFX, 4);
}

// FUNCTION: LEGOLAND 0x00403e90
void CoptersInitNode(struct CopterNode *node) {
    struct Sprite *handle;
    struct LLS *lls;
    char frame;

    node->layer[1].idle_layer = 10;
    node->layer[1].active_layer = 3;
    node->layer[1].field_10 = 2;
    node->layer[1].field_14 = 7;
    node->layer[1].flags = 0;
    handle = GetSpriteForLayer((struct Sprite *)CopterModelLayers, 3);
    if (handle != 0) {
        lls = GetLLSForSprite((struct Sprite *)handle);
        if (lls != NULL) {
            frame = (char)lls->frame_count;
            node->layer[1].frame_count = frame;
            node->layer[1].frame = frame - 1;
        }
    }
    node->layer[0].idle_layer = 2;
    node->layer[0].active_layer = 1;
    node->layer[0].field_10 = 0;
    node->layer[0].field_14 = 6;
    node->layer[0].flags = 0;
    handle = GetSpriteForLayer((struct Sprite *)CopterModelLayers, 1);
    if (handle != 0) {
        lls = GetLLSForSprite((struct Sprite *)handle);
        if (lls != NULL) {
            frame = (char)lls->frame_count;
            node->layer[0].frame_count = frame;
            node->layer[0].frame = frame - 1;
        }
    }
    node->layer[2].active_layer = 0xb;
    node->layer[2].idle_layer = 4;
    node->layer[2].field_10 = 4;
    node->layer[2].field_14 = 8;
    node->layer[2].flags = 0;
    handle = GetSpriteForLayer((struct Sprite *)CopterModelLayers, 0xb);
    if (handle != 0) {
        lls = GetLLSForSprite((struct Sprite *)handle);
        if (lls != NULL) {
            frame = (char)lls->frame_count;
            node->layer[2].frame_count = frame;
            node->layer[2].frame = frame - 1;
        }
    }
    node->layer[3].active_layer = 6;
    node->layer[3].idle_layer = 5;
    node->layer[3].field_10 = 3;
    node->layer[3].field_14 = 5;
    node->layer[3].flags = 0;
    handle = GetSpriteForLayer((struct Sprite *)CopterModelLayers, 6);
    if (handle != 0) {
        lls = GetLLSForSprite((struct Sprite *)handle);
        if (lls != NULL) {
            frame = (char)lls->frame_count;
            node->layer[3].frame_count = frame;
            node->layer[3].frame = frame - 1;
        }
    }
    node->layer[4].idle_layer = 8;
    node->layer[4].active_layer = 7;
    node->layer[4].field_10 = 1;
    node->layer[4].field_14 = 9;
    node->layer[4].flags = 0;
    handle = GetSpriteForLayer((struct Sprite *)CopterModelLayers, 7);
    if (handle != 0) {
        lls = GetLLSForSprite((struct Sprite *)handle);
        if (lls != NULL) {
            frame = (char)lls->frame_count;
            node->layer[4].frame_count = frame;
            node->layer[4].frame = frame - 1;
        }
    }
    ResetCoptersRide(node, 1);
}

// FUNCTION: LEGOLAND 0x00404040
void CoptersUnload(void) {
    struct Sprite **current;

    if (CopterBaseMatteSprite != NULL) {
        KillSprite(CopterBaseMatteSprite);
    }

    current = CopterModelSprites;
    while ((INT_PTR)current < (INT_PTR)&CoptersRenderList2) {
        if (*current != NULL) {
            KillSprite(*current);
        }
        current++;
    }

    if (CopterQueueTables[0] != 0) {
        FreeIfNotNull((struct Sprite *)CopterQueueTables[0]);
    }
    if (CopterQueueTables[1] != 0) {
        FreeIfNotNull((struct Sprite *)CopterQueueTables[1]);
    }
    if (CopterQueueTables[3] != 0) {
        FreeIfNotNull((struct Sprite *)CopterQueueTables[3]);
    }
    if (CopterQueueTables[4] != 0) {
        FreeIfNotNull((struct Sprite *)CopterQueueTables[4]);
    }
    if (CopterQueueTables[2] != 0) {
        FreeIfNotNull((struct Sprite *)CopterQueueTables[2]);
    }

    CoptersRemoveAllNodes();
    Kill_FXList(Helicopter_SFX, 4);
    UnloadPos(CoptersPos);
}

// FUNCTION: LEGOLAND 0x004040f0
void CoptersRenderLayer(struct CopterNode *node, int index, unsigned int param_3) {
    struct CopterLayer *layer;
    struct LLS *lls;
    struct Point off;
    struct Point sc;
    struct Sprite *sprite;
    int a;
    int b;

    layer = &node->layer[index];
    if (layer->flags & 1) {
        b = layer->active_layer;
        a = layer->field_14;
    } else {
        b = layer->idle_layer;
        a = layer->field_10;
    }
    lls = GetLLSForLayer(CopterModelLayers, layer->idle_layer);
    if (lls != NULL) {
        LLSStop((struct LLS *)lls);
        LLSSetFrame(lls, 0);
    }
    lls = GetLLSForLayer(CopterModelLayers, layer->field_10);
    if (lls != NULL) {
        LLSStop((struct LLS *)lls);
        LLSSetFrame(lls, 0);
    }
    lls = GetLLSForLayer(CopterModelLayers, layer->active_layer);
    if (lls != NULL) {
        LLSStop((struct LLS *)lls);
        LLSSetFrame(lls, 0);
    }
    sc = GetScreenCoordsForObject((TileId *)node, (struct Ride *)ActiveCopterRide);
    off = GetRenderOffsetForLayer(ActiveCopterRide->layer, b);
    AdjustOffsetForViewMode(&off);
    sprite = GetSpriteForLayer(ActiveCopterRide->layer, b);
    if (sprite != NULL) {
        lls = GetLLSForSprite((struct Sprite *)sprite);
        if (lls != NULL) {
            LLSSetFrame(lls, layer->frame);
        }
    }
    PrintSprite(sprite, sc.x + off.x, sc.y + off.y, param_3, NULL);
    if (layer->rider != NULL) {
        IP_RenderBlokeIn3DNow(layer->rider->rider);
    }
    sprite = CopterModelSprites[a];
    if (sprite != NULL) {
        lls = GetLLSForSprite((struct Sprite *)sprite);
        if (lls != NULL) {
            LLSSetFrame(lls, layer->frame);
        }
        PrintSprite(sprite, sc.x + off.x, sc.y + off.y, param_3, NULL);
    }
}

// FUNCTION: LEGOLAND 0x00404290
void CoptersRender(Element *obj, int unused, int unused2, TileId *tile, void *unused3, unsigned int clip) {
    struct Ride *ride = obj->ride;
    struct CopterNode *node;
    struct Point pos;

    node = CoptersFindNode((struct CopterSource *)tile);
    if (node != NULL) {
        struct RideNode *rn;
        struct Point base;
        int d;
        int key;

        RenderItems_New();
        CoptersRenderListHead0 = NULL;
        CoptersRenderListHead1 = NULL;
        CoptersRenderListHead2 = NULL;
        CoptersRenderListHead3 = NULL;
        CoptersRenderListHead4 = NULL;
        CoptersRenderListHead5 = NULL;
        base.y = node->field_1 + ActiveCopterRide->y;
        for (rn = ActiveCopterRide->riders; rn != NULL; rn = rn->next) {
            if ((rn->rider->flags & 0x80) == 0) {
                key = rn->person->field_20;
                d = base.y - (rn->rider->pos.y >> 8);
                if (d >= 5) {
                    AddBlokeToRenderList(CoptersRenderList1, rn, key);
                }
                if (d >= 3 && d <= 4) {
                    AddBlokeToRenderList(CoptersRenderList3, rn, key);
                }
                if (d >= 0 && d <= 2) {
                    AddBlokeToRenderList(CoptersRenderList0, rn, key);
                }
            }
        }
        CoptersRenderLayer(node, 0, clip);
        CoptersRenderLayer(node, 2, clip);
        RenderBlokeList((struct RenderItemNode **)CoptersRenderList2);
        RenderBlokeList((struct RenderItemNode **)CoptersRenderList1);
        CoptersRenderLayer(node, 3, clip);
        CoptersRenderLayer(node, 4, clip);
        RenderBlokeList((struct RenderItemNode **)CoptersRenderList3);
        RenderBlokeList((struct RenderItemNode **)CoptersRenderList4);
        CoptersRenderLayer(node, 1, clip);
        RenderBlokeList((struct RenderItemNode **)CoptersRenderList0);
    }
    pos = GetScreenCoordsForObject(tile, ride);
    {
        struct Point off = GetRenderOffsetForLayer(ride->layer, 0);
        AdjustOffsetForViewMode(&off);
        PrintSprite(CopterBaseMatteSprite, pos.x + off.x, pos.y + off.y, clip, 0);
    }
}

// FUNCTION: LEGOLAND 0x00404450
void CoptersSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = ActiveCopterRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00404490
struct RideSpriteInfo *GetCoptersSpriteInfo(struct Element *editobj, unsigned short uid) {
    struct Ride *ride = editobj->ride;

    CoptersSpriteInfo.sprite = ride->layer;
    CoptersSpriteInfo.x = ride->field_14;
    CoptersSpriteInfo.y = ride->field_18;
    CoptersSpriteInfo.id = uid;
    ride->layer->flags |= 0x2000;

    if (CoptersFindNode((struct CopterSource *)&uid) != 0) {
        HideLayer(CopterModelLayers, 1);
        HideLayer(CopterModelLayers, 2);
        HideLayer(CopterModelLayers, 3);
        HideLayer(CopterModelLayers, 0xa);
        HideLayer(CopterModelLayers, 6);
        HideLayer(CopterModelLayers, 5);
        HideLayer(CopterModelLayers, 7);
        HideLayer(CopterModelLayers, 8);
        HideLayer(CopterModelLayers, 0xb);
        HideLayer(CopterModelLayers, 4);
    }

    return &CoptersSpriteInfo;
}

// FUNCTION: LEGOLAND 0x00404580
void CoptersRemoveObject(Element *obj, TileId tile, struct Cursor *cursor) {
    struct CopterSfxNode *node;
    struct SampleSource src;
    unsigned int x;
    unsigned int y;

    node = (struct CopterSfxNode *)CoptersFindNode((struct CopterSource *)&tile);
    if (node != NULL) {
        CoptersRemoveNode((struct CopterNode *)node);
    }
    StandardRemoveObject(obj, tile, cursor);
    RemoveAllBlokesFromRide(obj->ride, tile);
    x = node->tile_x;
    y = node->tile_y;
    src.type = 2;
    src.x = x;
    src.y = y;
    UnSourceAndFadeAllSamplesFromSource(&src, -200);
}

// FUNCTION: LEGOLAND 0x00404600
void CoptersAddObject(Element *obj, int *coords) {
    TileId tile;

    tile.pos.x = coords[0];
    tile.pos.y = coords[1];
    AddBasicObject(obj, coords);
    CoptersAddNode((struct CopterSource *)&tile);
}

// FUNCTION: LEGOLAND 0x00404630
void CoptersPlaceRider(struct CopterNode *node, int index) {
    struct Point p2;
    struct Point p3;
    volatile struct Point p1; /* volatile: last resort, the natural forms tried are in docs/attempts */
    struct Point base;
    struct CopterLayer *entry;
    struct Sprite *sprite;
    struct PosFrame *frame;
    struct Person *person;
    float scale;
    int tmp; /* float bits in, 16.16 fixed point out */
    int yoff;
    int layer;
    int s;
    int i;
    int j;
    int r; /* matrix row */

    entry = &node->layer[index];
    base = GetScreenCoordsForObject((TileId *)node, ActiveCopterRide);
    if (entry->rider != NULL) {
        s = entry->active_layer;
        layer = 1;
        switch (index) {
        case 1:
            layer = 0;
            yoff = 0xeb;
            break;
        case 3:
            layer = 1;
            yoff = 0xe6;
            break;
        case 4:
            layer = 2;
            yoff = 0xe6;
            break;
        case 0:
            layer = 3;
            yoff = 0xd7;
            break;
        case 2:
            layer = 4;
            yoff = 0xe1;
            break;
        }
        p1 = GetRenderOffsetForLayer(CopterModelLayers, s);
        sprite = GetSpriteForLayer(CopterModelLayers, s);
        AdjustOffsetForViewMode(&p1);
        p2.y = (int)CoptersPos->entries[layer][entry->frame].pos[1] + yoff;
        AdjustOffsetForViewMode(&p2);
        p2.x = sprite->width >> 1;
        p3.x = p1.x + base.x + p2.x;
        p3.y = p1.y + base.y + p2.y;
        AdjustBlokePosition(&p3);
        person = entry->rider->person;
        SetPersonPosition(person, p3.x, p3.y);
        scale = 65536.0f;
        frame = &CoptersPos->entries[layer][entry->frame];
        frame->mat[2][0] = frame->mat[0][2] * frame->mat[1][1] - frame->mat[1][2] * frame->mat[0][1];
        frame->mat[2][1] = frame->mat[1][2] * frame->mat[0][0] - frame->mat[1][0] * frame->mat[0][2];
        frame->mat[2][2] = frame->mat[1][0] * frame->mat[0][1] - frame->mat[0][0] * frame->mat[1][1];
        for (i = 0; i < 3; i++) {
            r = CoptersMatrixRowOrder[i];
            for (j = 0; j < 3; j++) {
                /* float -> 16.16 fixed, rounded by fistp (inline asm in the original too) */
                *(float *)&tmp = CoptersPos->entries[layer][entry->frame].mat[r][CoptersMatrixColOrder[j]];
                __asm {
                fld dword ptr tmp
                fmul scale
                fistp tmp
                }
                person->m[j * 3 + i] = CoptersMatrixColSign[j] * CoptersMatrixRowSign[r] * tmp;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00404860
void CoptersAnimateLayer(struct CopterNode *node, int index) {
    struct CopterLayer *layer = &node->layer[index];

    if (layer->flags & 1) {
        layer->frame = layer->frame + 1;
        if (layer->frame < layer->frame_count) {
            return;
        }
        layer->frame = 0;
        layer->field_1d = layer->field_1d - 1;
        if (layer->field_1d >= 0) {
            return;
        }
        layer->flags &= 0xfffffffe;
    }
}

// FUNCTION: LEGOLAND 0x004048a0
unsigned int CoptersResumeSample(void *param) {
    ResumeSinglyPausedSample((struct Sample *)param);
    return 0;
}

/* Starts a copter ride: flags every occupied seat layer, sets running flag 1 (clearing 0x4000), resets the layer
 * frames, plays the start sound and a looping rotor sound that stays paused until CoptersResumeSample resumes it
 * 0xb54 ms later. */
// FUNCTION: LEGOLAND 0x004048b0
void StartCoptersRide(struct CopterSfxNode *node) {
    struct FXItem *fx = Helicopter_SFX;
    struct SampleSource params;
    struct Sample *sample;

    if (node->layer[1].rider != 0) node->layer[1].flags |= 1;
    if (node->layer[0].rider != 0) node->layer[0].flags |= 1;
    if (node->layer[2].rider != 0) node->layer[2].flags |= 1;
    if (node->layer[3].rider != 0) node->layer[3].flags |= 1;
    if (node->layer[4].rider != 0) node->layer[4].flags |= 1;

    node->field_3 = node->field_2;
    node->field_2 = 0;
    node->field_8 = (node->field_8 & ~0x4000u) | 1;
    node->field_c = 2;
    node->layer[1].field_1d = 3;
    node->layer[0].field_1d = 3;
    node->layer[2].field_1d = 3;
    node->layer[3].field_1d = 3;
    node->layer[4].field_1d = 3;
    node->layer[1].frame = 0;
    node->layer[0].frame = 0;
    node->layer[2].frame = 0;
    node->layer[3].frame = 0;
    node->layer[4].frame = 0;

    params.x = node->tile_x;
    params.y = node->tile_y;
    params.type = 2;

    PlayInstanceOfSample(fx[0].sample, 0, 1, &params);
    sample = PlayInstanceOfSample(fx[1].sample, 1, 1, &params);
    PauseSingleSample(sample);
    AddSFX_Callback(sample, 0xb54, (unsigned int (*)(struct Sample *))CoptersResumeSample);
}

/* Resets a copter ride: clears seat layers, riders and counters, rewinds the frames to their last one and clears
 * flags 0x4001. With param 0 it also fades the ride's positional sounds and plays the stop sound. */
// FUNCTION: LEGOLAND 0x004049a0
void ResetCoptersRide(struct CopterNode *node, int param) {
    struct FXItem *fx = Helicopter_SFX;
    struct SampleSource params;
    struct CopterSfxNode *n = (struct CopterSfxNode *)node;

    n->layer[1].flags &= ~1u;
    n->layer[0].flags &= ~1u;
    n->layer[2].flags &= ~1u;
    n->layer[3].flags &= ~1u;
    n->layer[4].flags &= ~1u;
    n->field_c = 0;
    n->field_10 = 0;
    n->field_2 = 0;
    n->layer[1].frame = n->layer[1].frame_count - 1;
    n->layer[0].frame = n->layer[0].frame_count - 1;
    n->layer[2].frame = n->layer[2].frame_count - 1;
    n->layer[3].frame = n->layer[3].frame_count - 1;
    n->layer[4].frame = n->layer[4].frame_count - 1;
    n->layer[1].rider = 0;
    n->layer[0].rider = 0;
    n->layer[2].rider = 0;
    n->layer[3].rider = 0;
    n->layer[4].rider = 0;
    n->field_8 &= ~0x4001u;
    if (param == 0) {
        params.x = n->tile_x;
        params.type = 2;
        params.y = n->tile_y;
        UnSourceAndFadeAllSamplesFromSource(&params, -200);
        PlayInstanceOfSample(fx[2].sample, 0, 1, &params);
    }
}

// FUNCTION: LEGOLAND 0x00404a90
void CoptersUpdateNode(struct CopterNode *node) {
    node->field_c = node->field_c - 1;
    if (node->field_c < 0) {
        node->field_c = 2;
        CoptersAnimateLayer(node, 1);
        CoptersAnimateLayer(node, 0);
        CoptersAnimateLayer(node, 2);
        CoptersAnimateLayer(node, 3);
        CoptersAnimateLayer(node, 4);
        if ((node->field_8 & 1) && !(node->layer[1].flags & 1) && !(node->layer[0].flags & 1) &&
            !(node->layer[2].flags & 1) && !(node->layer[3].flags & 1) &&
            !(node->layer[4].flags & 1)) {
            if (GetAllBlokesOffRide(ActiveCopterRide, node->field_0) != 0) {
                ResetCoptersRide(node, 0);
            }
            return;
        }
    }
    CoptersPlaceRider(node, 1);
    CoptersPlaceRider(node, 0);
    CoptersPlaceRider(node, 2);
    CoptersPlaceRider(node, 3);
    CoptersPlaceRider(node, 4);
    if (!(node->field_8 & 1)) {
        if (node->field_8 & 0x4000) {
            if (node->field_2 == node->field_10) {
                node->field_8 &= ~0x4000u;
                StartCoptersRide((struct CopterSfxNode *)node);
                return;
            }
        } else if (node->field_2 != 0) {
            if (node->field_14 == 0) {
                node->field_8 |= 0x4000;
                Ride_SetFlagToNotLetAnyoneOn(node);
            } else {
                node->field_14 = node->field_14 - 1;
            }
        }
    }
    Put3DBlokesOnRide2(ActiveCopterRide, node);
}

// FUNCTION: LEGOLAND 0x00404bc0
void CoptersUpdateNodes(void) {
    struct CopterNode *node = CopterNodeList;
    if (node == NULL) {
        return;
    }
    do {
        CoptersUpdateNode(node);
        node = node->next;
    } while (node != NULL);
}

// FUNCTION: LEGOLAND 0x00404be0
void CoptersUpdate(struct Element *elem) {
    unsigned int x;
    unsigned int y;
    struct Bloke *b;
    struct CopterNode *cn;
    struct RideNode *link;
    struct Ride *ride = elem->ride;
    struct RideNode *next;
    struct RideNode *node;
    TileId *tile;
    struct RideNode *cur;
    struct QueueTable *spr;
    struct QueueTable *spr2;
    int qi;

    CoptersUpdateNodes();
    node = ride->riders;
    while (node != NULL) {
        cur = node;
        next = cur->next;
        b = cur->rider;
        tile = &cur->tile;
        cn = CoptersFindNode((struct CopterSource *)tile);
        if (cn == NULL) {
            break;
        }
        x = ride->x + tile->pos.x;
        y = tile->pos.y + ride->y;
        if (b->low_level_action == 0) {
            switch (b->param_action) {
            case 0:
                b->flags |= 8;
                link = node;
                cn->layer[CoptersFindChainIndex(link, (struct CopterSource *)&node->tile)].rider = node;
                cn->field_10++;
                cn->field_14 = 0xb4;
                b->field_58 = 0;
                b->param_action++;
                break;
            case 1:
                link = node;
                switch (CoptersFindChainIndex(link, (struct CopterSource *)&node->tile)) {
                case 1:
                    spr = CopterQueueTables[0];
                    break;
                case 0:
                    spr = CopterQueueTables[2];
                    break;
                case 2:
                    spr = CopterQueueTables[1];
                    break;
                case 3:
                    spr = CopterQueueTables[3];
                    break;
                case 4:
                    spr = CopterQueueTables[4];
                    break;
                }
                StartQueueTableForward((struct QueueTable *)spr, (struct Bloke *)b);
                CoptersQueueTableToIndex((struct RideNode *)link);
                break;
            case 2:
                qi = (int)(INT_PTR)GetBlokeQueueTable((struct Bloke *)b); /* the queue index */
                MoveBlokeAlongQueueTable((struct QueueTable *)CopterQueueTables[qi], x, y, b);
                break;
            case 3:
            case 7:
                b->param_action++;
                break;
            case 4:
                b->param_action++;
                cn->field_2++;
                if ((short)(signed char)cn->field_2 == ActiveCopterRide->seats) {
                    StartCoptersRide((struct CopterSfxNode *)cn);
                }
                break;
            case 5:
                b->flags |= 0x80;
                BlokeSitAnim(b);
                BlokeSetFrame(b, 0);
                break;
            case 6:
                BlokeWalkAnim(b);
                BlokeSetFrame(b, 0);
                b->flags &= 0xff7f;
                b->param_action++;
                break;
            case 8:
                link = node;
                switch (CoptersFindChainIndex(link, (struct CopterSource *)&node->tile)) {
                case 1:
                    spr2 = CopterQueueTables[0];
                    break;
                case 0:
                    spr2 = CopterQueueTables[2];
                    break;
                case 2:
                    spr2 = CopterQueueTables[1];
                    break;
                case 3:
                    spr2 = CopterQueueTables[3];
                    break;
                case 4:
                    spr2 = CopterQueueTables[4];
                    break;
                }
                StartQueueTableBackward((struct QueueTable *)spr2, (struct Bloke *)b);
                CoptersQueueTableToIndex((struct RideNode *)link);
                break;
            case 9:
                qi = (int)(INT_PTR)GetBlokeQueueTable((struct Bloke *)b); /* the queue index */
                MoveBlokeAlongQueueTable((struct QueueTable *)CopterQueueTables[qi], x, y, b);
                break;
            case 10:
                b->dest.x = (x << 8) + 0x80;
                b->dest.y = (y << 8) + 0x80;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                break;
            case 11:
                RemoveBlokeFromRide(ride, node);
                b->flags &= 0xfff7;
                cn->field_3--;
                if (cn->field_3 == 0) {
                    Ride_ClearFlagToNotLetAnyoneOn(cn);
                }
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x00404f20
unsigned int CoptersFindChainIndex(struct RideNode *node, struct CopterSource *id) {
    struct RideNode *cur;
    unsigned int idx = 0;

    for (cur = ActiveCopterRide->riders; cur != NULL; cur = cur->next) {
        struct CopterSource *p = (struct CopterSource *)&cur->tile;
        if (memcmp(p, id, 2) == 0) {
            if (cur == node) {
                return idx;
            }
            idx++;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00404f60
LEGO_EXPORT int Copters_Save(struct Element *element) {
    struct CopterNode *node;
    struct RideNode *cur;
    struct RideNode *rider;
    struct RideNode *saved[6];
    int index;
    int i;
    unsigned int one;
    unsigned int zero;

    one = 1;
    zero = 0;
    node = CopterNodeList;
    if (CopterNodeList != NULL) {
        while (node != NULL) {
            if (SaveGameWrite(&one, 4) == 0) {
                return 0;
            }
            for (i = 0; i < 6; i++) {
                rider = node->layer[i].rider;
                index = 0;
                saved[i] = rider;
                for (cur = ActiveCopterRide->riders; cur != NULL; cur = cur->next) {
                    if (cur == rider) {
                        break;
                    }
                    index++;
                }
                if (cur != NULL) {
                    node->layer[i].rider = (struct RideNode *)(INT_PTR)(index + 1);
                } else {
                    node->layer[i].rider = NULL;
                }
            }
            if (SaveGameWrite(node, sizeof(struct CopterNode)) == 0) {
                return 0;
            }
            for (i = 0; i < 6; i++) {
                node->layer[i].rider = saved[i];
            }
            node = node->next;
        }
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00405050
LEGO_EXPORT int Copters_Load(struct Element *element) {
    int count;
    struct CopterNode *prev = NULL;
    struct CopterNode *node;
    struct RideNode *chain;
    int n;
    int i;

    if (!SaveGameRead(&count, 4)) {
        return 0;
    }

    while (count != 0) {
        node = (struct CopterNode *)malloc(sizeof(struct CopterNode));
        if (!SaveGameRead(node, sizeof(struct CopterNode))) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            CopterNodeList = node;
        }
        prev = node;

        for (i = 6; i != 0; i--) {
            /* the saved rider: 1 + its index in the ride's list, or 0 */
            n = (int)(INT_PTR)node->layer[6 - i].rider;
            chain = ActiveCopterRide->riders;
            if (n != 0) {
                while (--n != 0) {
                    chain = chain->next;
                }
                node->layer[6 - i].rider = chain;
            } else {
                node->layer[6 - i].rider = NULL;
            }
        }

        if (!SaveGameRead(&count, 4)) {
            return 0;
        }
    }

    return 1;
}

// FUNCTION: LEGOLAND 0x00405110
void CoptersRide(struct Element *name, struct Ride *interfaces) {
    // STRING: LEGOLAND 0x004b43e4
    if (_stricmp("COPTERS", name->name) == 0) {
        interfaces->cb_a4 = CoptersLoad;
        interfaces->cb_8c = CoptersSetEditMode;
        interfaces->cb_98 = CoptersAddObject;
        interfaces->cb_9c = CoptersRemoveObject;
        interfaces->cb_pre_render = CoptersUpdate;
        interfaces->cb_sprite = GetCoptersSpriteInfo;
        interfaces->cb_b0 = CoptersRender;
        interfaces->cb_ac = CoptersUnload;
        interfaces->save_hook = Copters_Save;
        interfaces->load_hook = Copters_Load;
    }
}
