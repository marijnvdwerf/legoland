#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "debug_alloc.h"
#include "draw.h"
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
#include "tilemap.h"
#include "water_works.h"

struct WaterRect {
    int x0;
    int y0;
    int x1;
    int y1;
};

struct AdjustOffset {
    int x;
    int y;
};

#include "image_sprite.h"
#include "power.h"

// FUNCTION: LEGOLAND 0x00417a90
unsigned int AcquireWaterWorksSfx(void) {
    if (WaterWorksSfxRefCount++ == 0) {
        Load_FXList(WATERWORKS_SFX, 3);
    }
}

// FUNCTION: LEGOLAND 0x00417ac0
void ReleaseWaterWorksSfx(void) {
    WaterWorksSfxRefCount--;
    if (WaterWorksSfxRefCount == 0) {
        Kill_FXList(WATERWORKS_SFX, 3);
    }
}

// FUNCTION: LEGOLAND 0x00417ae0
void WaterWorksEntranceUnload(void) { ReleaseWaterWorksSfx(); }

// FUNCTION: LEGOLAND 0x00417af0
void PlayWaterWorksSfxAtTile(unsigned char *tile, int sfx, unsigned int looping) {
    struct SampleSource source;

    source.type = 2;
    source.x = tile[0];
    source.y = tile[1];
    PlayInstanceOfSample(WATERWORKS_SFX[sfx].sample, looping, 1, &source);
}

// FUNCTION: LEGOLAND 0x00417b40
void FadeWaterWorksSfxAtTile(unsigned char *tile) {
    struct SampleSource source;

    source.type = 2;
    source.x = tile[0];
    source.y = tile[1];
    UnSourceAndFadeAllSamplesFromSource(&source, -400);
}

// FUNCTION: LEGOLAND 0x00417b80
void InsertWaterNodeAtHead(struct WaterNode **head, struct WaterNode *node) {
    if (node != NULL) {
        node->next = *head;
        *head = node;
    }
}

// FUNCTION: LEGOLAND 0x00417ba0
struct WaterNode *FindWaterNodeByKey(struct WaterNode *list, short *key) {
    if (list == NULL) {
        return NULL;
    }
    while (memcmp(&list->key, key, sizeof(list->key)) != 0) {
        list = list->next;
        if (list == NULL) {
            return NULL;
        }
    }
    return list;
}

// FUNCTION: LEGOLAND 0x00417bd0
void UnlinkWaterNode(struct WaterNode **head, struct WaterNode *node) {
    struct WaterNode *cur;

    cur = *head;
    if (cur == node) {
        *head = node->next;
        return;
    }
    while (cur->next != node) {
        cur = cur->next;
        if (cur == NULL) {
            break;
        }
    }
    if (cur != NULL) {
        cur->next = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00417c00
unsigned int WaterWorksEntranceLoad(struct Element *element) {
    WaterWorksEntranceElement = element;
    WaterWorksEntranceRide = element->ride;
    return AcquireWaterWorksSfx();
}

// FUNCTION: LEGOLAND 0x00417c20
void WaterWorksEntranceAddObject(struct Element *editObj, int *coords) {
    struct SampleSource source;

    AddBasicObject(editObj, coords);
    source.x = coords[0];
    source.type = 2;
    source.y = coords[1];
    PlayInstanceOfSample(WATERWORKS_SFX[1].sample, 1, 1, &source);
}

// FUNCTION: LEGOLAND 0x00417c70
void WaterWorksEntranceRemoveObject(struct Element *element, unsigned int tile, struct Cursor *cursor) {
    RemoveSoundObject(element, tile, cursor);
}

// FUNCTION: LEGOLAND 0x00417c90
unsigned int WaterWorksEntranceExists(void) {
    /* water works pieces can only be placed once the entrance has been built */
    return GetFirstObjectMatching(WaterWorksEntranceElement) != 0;
}

// FUNCTION: LEGOLAND 0x00417cb0
void AddWaterBlockNode(unsigned short key) {
    struct WaterNode *node;

    node = (struct WaterNode *)malloc(sizeof(struct WaterNode));
    if (node != NULL) {
        memset(node, 0, sizeof(struct WaterNode));
        node->key = key;
        InsertWaterNodeAtHead(&WaterBlockList, node);
    }
}

// FUNCTION: LEGOLAND 0x00417cf0
void FreeWaterBlockNode(struct WaterNode *node) {
    UnlinkWaterNode(&WaterBlockList, node);
    free(node);
}

// FUNCTION: LEGOLAND 0x00417d10
struct WaterNode *FindWaterBlockNode(unsigned short *key) {
    return FindWaterNodeByKey(WaterBlockList, key);
}

// FUNCTION: LEGOLAND 0x00417d30
void WaterBlockLoad(struct Element *element) {
    AcquireWaterWorksSfx();
    WaterBlockRide = element->ride;
    WaterBlockRide->flags |= 0x420;
    WaterBlockRide->footprint.x0 = 0;
    WaterBlockRide->footprint.x1 = 0;
    WaterBlockRide->footprint.y0 = 0;
    WaterBlockRide->footprint.y1 = 0;
    WaterBlockRide->field_14 = 0;
    WaterBlockRide->field_18 = 0;
    // STRING: LEGOLAND 0x004b501c
    if (LLIDB_FindElement("WATER WORKS IMAGE LIST", &WaterWorksImageListHandle, 0) == 0) {
        WaterWorksImageListData = LLIDB_LoadData((void *)WaterWorksImageListHandle);
    }
    // STRING: LEGOLAND 0x004b500c
    TopwaterSprite = LoadSprite("topwater.lls", 1);
}

// FUNCTION: LEGOLAND 0x00417dd0
void WaterWorksWaterBlockCalcCursor(struct Element *element, int *screen, unsigned int unused) {
    struct Ride *context;

    context = element->ride;
    EditCursor.footprint = context->footprint;
    EditCursor.next = NULL;
    ScreenToMapRef(screen, &EditCursor.tile_x, unused);
    ValidateCursor(&EditCursor, context);
    if (WaterWorksEntranceExists() != 0) {
        return;
    }
    SetCursorBlocked(&EditCursor, 0xc);
    PropagateCursorBlocked(&EditCursor);
}

// FUNCTION: LEGOLAND 0x00417e40
void WaterBlockUnload(void) {
    if (TopwaterSprite != 0) {
        KillSprite(TopwaterSprite);
    }
    LLIDB_UnLoadData(WaterWorksImageListHandle);
    ReleaseWaterWorksSfx();
}

// FUNCTION: LEGOLAND 0x00417e70
int ListHasPointInRect(struct Bloke *list, struct WaterRect *rect) {
    struct Bloke *current;

    current = list;
    while (current != NULL) {
        if ((current->pos.x >> 8) >= rect->x0 && (current->pos.x >> 8) <= rect->x1) {
            if ((current->pos.y >> 8) >= rect->y0 && (current->pos.y >> 8) <= rect->y1) {
                return 1;
            }
        }
        current = current->next;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00417ec0
unsigned int IsPointInBlokeGardenerOrMechanicList(unsigned char *point) {
    struct WaterRect rect;

    rect.x0 = point[0];
    rect.x1 = point[0];
    rect.y0 = point[1];
    rect.y1 = point[1];
    if (ListHasPointInRect(FirstBloke, &rect) != 0) {
        return 1;
    }
    if (ListHasPointInRect(GardenerList, &rect) != 0) {
        return 1;
    }
    return ListHasPointInRect(MechanicList, &rect) != 0;
}

// FUNCTION: LEGOLAND 0x00417f40
void TriggerWaterBlocksUnderBlokes(void) {
    struct WaterNode *node;

    node = WaterBlockList;
    while (node != NULL) {
        if (IsPointInBlokeGardenerOrMechanicList((unsigned char *)&node->key) != 0) {
            if (node->state == 0) {
                node->state = 1;
                node->frame = 0;
                PlayWaterWorksSfxAtTile((unsigned char *)&node->key, 0, 0);
            }
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00417f90
void WaterWorksWaterBlockUpdate(Element *obj) {
    struct WaterNode *node;
    Ride *ride;
    RideNode *rnode;
    RideNode *next;
    Bloke *bloke;
    TileId *tile;
    union {
        struct Sprite *spr;
        struct LLS *lls;
    } u;
    int limit;
    int rx;
    int ry;
    int idx;

    char dir;

    TriggerWaterBlocksUnderBlokes();
    node = WaterBlockList;
    while (node != NULL) {
        if (node->state == 1) {
            limit = 0;
            node->frame++;
            idx = node->block_index;
            u.spr = WaterWorksImageListData->sprites[(unsigned char)idx];
            if (u.spr != NULL) {
                u.lls = GetLLSForSprite(u.spr);
                if (u.lls != NULL) {
                    limit = u.lls->frame_count;
                } else {
                    // STRING: LEGOLAND 0x004b504c
                    DBPrintf("Bad Sprite in Waterworks block\n");
                }
            } else {
                // STRING: LEGOLAND 0x004b5034
                DBPrintf("Bad Block Index (%d)\n", idx);
            }
            if ((int)node->frame >= limit) {
                node->state = 0;
                node->frame = 0;
            }
        } else {
            u.spr = WaterWorksImageListData->sprites[node->block_index];
            u.lls = GetLLSForSprite(u.spr);
            if (u.lls != NULL) {
                node->frame = 0;
            } else {
                DBPrintf("Bad Sprite in Waterworks block\n");
            }
        }
        node = node->next;
    }
    ride = obj->ride;
    rnode = ride->riders;
    while (rnode != NULL) {
        bloke = rnode->rider;
        tile = &rnode->tile;
        next = rnode->next;
        rx = tile->pos.x + ride->x;
        ry = tile->pos.y + ride->y;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                ry = ry * 0x100 + 0x80;
                bloke->dest.x = rx * 0x100 + -0x80;
                bloke->dest.y = ry;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 1:
                RemoveBlokeFromRide(ride, rnode);
                bloke->flags &= 0xfff7;
                break;
            }
        }
        rnode = next;
    }
}

// FUNCTION: LEGOLAND 0x00418110
struct RideSpriteInfo *GetWaterWorksWaterBlockSpriteInfo(struct Element *element, unsigned short id) {
    struct WaterNode *node;
    int idx;

    node = FindWaterBlockNode(&id);
    if (node != NULL) {
        idx = node->block_index;
        WaterWorksSpriteInfo.id = id;
        WaterWorksSpriteInfo.sprite = WaterWorksImageListData->sprites[(unsigned char)idx];
        WaterWorksSpriteInfo.x = WaterWorksImageListData->offset_x[(unsigned char)idx] >> 1;
        WaterWorksSpriteInfo.y = WaterWorksImageListData->offset_y[(unsigned char)idx] >> 1;
        WaterWorksSpriteInfo.field_10 = 0;
        ((struct Sprite *)WaterWorksSpriteInfo.sprite)->flags |= 0x2000;
        SetOverrideFrame(node->frame);
    }
    return &WaterWorksSpriteInfo;
}

// FUNCTION: LEGOLAND 0x004181a0
void RenderWaterBlock(struct Element *element, int unused2, int unused3, unsigned short *tile_id) {
    struct RideNode *node;

    node = ((struct Ride *)element->ride)->riders;
    while (node != NULL) {
        if (*tile_id == node->tile.id) {
            IP_RenderBlokeIn3DNow(node->rider);
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x004181e0
void WaterBlockAddObject(struct Element *editObj, unsigned char *coords) {
    unsigned short key;
    struct WaterNode *node;

    *(unsigned char *)&key = coords[0];
    *((unsigned char *)&key + 1) = coords[4];
    AddBasicObject(editObj, (int *)coords);
    AddWaterBlockNode(key);
    node = FindWaterBlockNode(&key);
    if (node != NULL) {
        node->block_index = (char)(rand() % WaterWorksImageListData->count);
    }
}

// FUNCTION: LEGOLAND 0x00418230
void WaterBlockRemoveObject(struct Element *element, unsigned int tile, struct Cursor *cursor) {
    struct WaterNode *node;

    StandardRemoveObject(element, *(TileId *)&tile, cursor);
    node = FindWaterBlockNode((unsigned short *)&tile);
    if (node != NULL) {
        FreeWaterBlockNode(node);
    }
}

// FUNCTION: LEGOLAND 0x00418260
void AddShowerNode(unsigned short key) {
    struct WaterNode *node;

    node = (struct WaterNode *)malloc(sizeof(struct WaterNode));
    if (node != NULL) {
        memset(node, 0, sizeof(struct WaterNode));
        node->key = key;
        InsertWaterNodeAtHead(&ShowerList, node);
    }
}

// FUNCTION: LEGOLAND 0x004182a0
void FreeShowerNode(struct WaterNode *node) {
    UnlinkWaterNode(&ShowerList, node);
    free(node);
}

// FUNCTION: LEGOLAND 0x004182c0
struct WaterNode *FindShowerNode(unsigned short *key) {
    return FindWaterNodeByKey(ShowerList, key);
}

// FUNCTION: LEGOLAND 0x004182e0
void ShowerLoad(struct Element *element) {
    AcquireWaterWorksSfx();
    ShowerRide = element->ride;
    ShowerRide->flags |= 0x420;
    ShowerLayer = ShowerRide->layer;
    ShowerLayer->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b506c
    ShowerSprite = LoadSprite("shower.lls", 1);
}

// FUNCTION: LEGOLAND 0x00418330
void ShowerUnload(void) {
    if (ShowerSprite != 0) {
        KillSprite(ShowerSprite);
    }
    ReleaseWaterWorksSfx();
}

// FUNCTION: LEGOLAND 0x00418350
void TriggerShowersUnderBlokes(void) {
    struct WaterNode *node;

    node = ShowerList;
    while (node != NULL) {
        if (IsPointInBlokeGardenerOrMechanicList((unsigned char *)&node->key) != 0) {
            if (node->state == 0) {
                node->state = 1;
                node->frame = 0;
                PlayWaterWorksSfxAtTile((unsigned char *)&node->key, 0, 0);
            }
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x004183a0
void WaterWorksShowerUpdate(void) {
    struct WaterNode *node;
    struct LLS *lls;
    int limit;

    TriggerShowersUnderBlokes();
    for (node = ShowerList; node != NULL; node = node->next) {
        if (node->state == 1 && (unsigned char)(node->frame += 1) >= 0xb) {
            node->state = 2;
            node->frame = 0;
        }
        if (node->state == 2) {
            limit = 0;
            node->frame++;
            lls = GetLLSForSprite((struct Sprite *)ShowerSprite);
            if (lls != NULL) {
                limit = lls->frame_count;
            }
            if ((int)node->frame >= limit) {
                if (IsPointInBlokeGardenerOrMechanicList((unsigned char *)&node->key) != 0) {
                    node->state = 2;
                    node->frame = 0xb;
                } else {
                    node->state = 3;
                    node->frame = 0x24;
                }
            }
        }
        if (node->state == 3 && (unsigned char)(node->frame += 1) >= 0x2c) {
            node->state = 0;
            node->frame = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00418450
void RenderShower(struct Element *element, unsigned int unused2, unsigned int unused3, unsigned char *tile, void *unused5, unsigned int tint) {
    struct WaterNode *node;
    struct Ride *ride;
    struct AdjustOffset offset;
    struct Point sc;
    struct LLS *lls;

    ride = element->ride;
    node = FindShowerNode((unsigned short *)tile);
    if (node != NULL && node->state == 2) {
        offset.x = -8;
        offset.y = 0x14;
        AdjustOffsetForViewMode((struct Point *)&offset);
        sc = GetScreenCoordsForObject((TileId *)tile, ride);
        lls = GetLLSForSprite((struct Sprite *)ShowerSprite);
        LLSSetFrame(lls, node->frame);
        PrintSprite(ShowerSprite, sc.x + offset.x, sc.y + offset.y, tint, 0);
    }
}

// FUNCTION: LEGOLAND 0x004184e0
void ShowerAddObject(struct Element *editObj, unsigned char *coords) {
    unsigned short key;

    *(unsigned char *)&key = coords[0];
    *((unsigned char *)&key + 1) = coords[4];
    AddBasicObject(editObj, (int *)coords);
    AddShowerNode(key);
}

// FUNCTION: LEGOLAND 0x00418510
void ShowerRemoveObject(struct Element *element, unsigned int tile, struct Cursor *cursor) {
    struct WaterNode *node;

    StandardRemoveObject(element, *(TileId *)&tile, cursor);
    node = FindShowerNode((unsigned short *)&tile);
    if (node != NULL) {
        FreeShowerNode(node);
    }
}

// FUNCTION: LEGOLAND 0x00418540
struct RideSpriteInfo *GetWaterWorksShowerSpriteInfo(struct Element *element, unsigned short id) {
    struct Ride *ride;
    struct WaterNode *node;

    ride = (struct Ride *)element->ride;
    WaterWorksSpriteInfo.id = id;
    WaterWorksSpriteInfo.sprite = ride->layer;
    WaterWorksSpriteInfo.x = ride->field_14;
    WaterWorksSpriteInfo.y = ride->field_18;
    WaterWorksSpriteInfo.field_10 = 0;
    node = FindShowerNode(&id);
    if (node != NULL) {
        if (node->state == 2) {
            SetOverrideFrame(0xb);
        } else {
            SetOverrideFrame(node->frame);
        }
    }
    return &WaterWorksSpriteInfo;
}

// FUNCTION: LEGOLAND 0x004185c0
void WaterWorksShowerCalcCursor(struct Element *element, int *screen, unsigned int unused) {
    struct Ride *context;

    context = element->ride;
    ScreenToMapRef(screen, &EditCursor.tile_x, unused);
    EditCursor.footprint = context->footprint;
    EditCursor.next = NULL;
    ValidateCursor(&EditCursor, context);
    if (WaterWorksEntranceExists() != 0) {
        return;
    }
    SetCursorBlocked(&EditCursor, 0xc);
    PropagateCursorBlocked(&EditCursor);
}

// FUNCTION: LEGOLAND 0x00418630
void AddElephantFountainNode(unsigned short key) {
    struct WaterNode *node;

    node = (struct WaterNode *)malloc(sizeof(struct WaterNode));
    if (node != NULL) {
        memset(node, 0, sizeof(struct WaterNode));
        node->key = key;
        InsertWaterNodeAtHead(&ElephantFountainList, node);
    }
}

// FUNCTION: LEGOLAND 0x00418670
void FreeElephantFountainNode(struct WaterNode *node) {
    UnlinkWaterNode(&ElephantFountainList, node);
    free(node);
}

// FUNCTION: LEGOLAND 0x00418690
struct WaterNode *FindElephantFountainNode(unsigned short *key) {
    return FindWaterNodeByKey(ElephantFountainList, key);
}

// FUNCTION: LEGOLAND 0x004186b0
void ElephantFountainLoad(struct Element *element) {
    AcquireWaterWorksSfx();
    ElephantFountainRide = element->ride;
    ElephantFountainRide->flags |= 0x20;
    ElephantFountainLayer = ElephantFountainRide->layer;
    // STRING: LEGOLAND 0x004b5078
    WwElsquirtSprite = LoadSprite("ww_elsquirt.lls", 1);
}

// FUNCTION: LEGOLAND 0x004186f0
void ElephantFountainUnload(void) {
    if (WwElsquirtSprite != 0) {
        KillSprite(WwElsquirtSprite);
    }
    ReleaseWaterWorksSfx();
}

// FUNCTION: LEGOLAND 0x00418710
unsigned int IsBlokeNearElephantFountain(unsigned char *tile, struct Footprint *footprint) {
    struct WaterRect rect;

    rect.x0 = tile[0] + footprint->x0;
    rect.x1 = tile[0] + footprint->x0 + 3;
    rect.y0 = tile[1] + footprint->y1;
    rect.y1 = tile[1] + footprint->y1 + 3;
    if (ListHasPointInRect(FirstBloke, &rect) != 0) {
        return 1;
    }
    if (ListHasPointInRect(GardenerList, &rect) != 0) {
        return 1;
    }
    return ListHasPointInRect(MechanicList, &rect) != 0;
}

// FUNCTION: LEGOLAND 0x004187b0
void TriggerElephantFountainsNearBlokes(void) {
    struct WaterNode *node;

    node = ElephantFountainList;
    while (node != NULL) {
        if (IsBlokeNearElephantFountain((unsigned char *)&node->key, &ElephantFountainRide->footprint) != 0) {
            if (node->state == 0) {
                node->state = 1;
                node->frame = 0;
            }
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x004187f0
void WaterWorksElephantFountainUpdate(void) {
    struct WaterNode *node;
    struct LLS *lls;
    int limit;
    struct AdjustOffset offset;
    int y;
    struct Point sc;

    TriggerElephantFountainsNearBlokes();
    for (node = ElephantFountainList; node != NULL; node = node->next) {
        if (node->state == 1) {
            limit = 0;
            lls = GetLLSForSprite((struct Sprite *)WwElsquirtSprite);
            if (lls != NULL) {
                limit = lls->frame_count;
            }
            node->frame++;
            if ((int)node->frame >= limit) {
                node->state = 0;
                node->frame = 0;
            }
            if (node->state == 1) {
                offset.x = -0x75;
                offset.y = 0x8a;
                AdjustOffsetForViewMode((struct Point *)&offset);
                sc = GetScreenCoordsForObject((TileId *)&node->key, ElephantFountainRide);
                SetOverrideFrame(node->frame);
                y = sc.y + offset.y;
                SortSprite(WwElsquirtSprite, sc.x + offset.x, y, y + 0x20, 0, 0);
                ClearOverrideFrame();
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x004188c0
void RenderWaterWorksElephantFountain(void) {}

// FUNCTION: LEGOLAND 0x004188d0
void ElephantFountainAddObject(struct Element *editObj, unsigned char *coords) {
    unsigned short key;

    *(unsigned char *)&key = coords[0];
    *((unsigned char *)&key + 1) = coords[4];
    AddBasicObject(editObj, (int *)coords);
    AddElephantFountainNode(key);
    PlayWaterWorksSfxAtTile((unsigned char *)&key, 2, 1);
}

// FUNCTION: LEGOLAND 0x00418910
void ElephantFountainRemoveObject(struct Element *element, unsigned int tile, struct Cursor *cursor) {
    struct WaterNode *node;

    StandardRemoveObject(element, *(TileId *)&tile, cursor);
    node = FindElephantFountainNode((unsigned short *)&tile);
    if (node != NULL) {
        FreeElephantFountainNode(node);
    }
    FadeWaterWorksSfxAtTile((unsigned char *)&tile);
}

// FUNCTION: LEGOLAND 0x00418950
void WaterWorksElephantFountainCalcCursor(struct Element *element, int *screen, unsigned int unused) {
    struct Ride *context;

    context = element->ride;
    EditCursor.footprint = context->footprint;
    ScreenToMapRef(screen, &EditCursor.tile_x, unused);
    EditCursor.next = NULL;
    ValidateCursor(&EditCursor, context);
    if (WaterWorksEntranceExists() != 0) {
        return;
    }
    SetCursorBlocked(&EditCursor, 0xc);
    PropagateCursorBlocked(&EditCursor);
}

// FUNCTION: LEGOLAND 0x004189c0
void CrocodileFountainAddObject(struct Element *editObj, int *coords) {
    struct SampleSource source;

    AddBasicObject(editObj, coords);
    source.x = coords[0];
    source.type = 2;
    source.y = coords[1];
    PlayInstanceOfSample(WATERWORKS_SFX[2].sample, 1, 1, &source);
}

// FUNCTION: LEGOLAND 0x00418a10
void CrocodileFountainRemoveObject(struct Element *element, unsigned int tile, struct Cursor *cursor) {
    RemoveSoundObject(element, tile, cursor);
}

// FUNCTION: LEGOLAND 0x00418a30
void WaterWorksCrocodileFountainCalcCursor(struct Element *element, int *screen, unsigned int unused) {
    struct Ride *context;

    context = element->ride;
    memcpy(&EditCursor.footprint, &context->footprint, sizeof(struct Footprint));
    EditCursor.next = NULL;
    ScreenToMapRef(screen, &EditCursor.tile_x, unused);
    ValidateCursor(&EditCursor, context);
    if (WaterWorksEntranceExists() != 0) {
        return;
    }
    SetCursorBlocked(&EditCursor, 0xc);
    PropagateCursorBlocked(&EditCursor);
}

// FUNCTION: LEGOLAND 0x00418aa0
LEGO_EXPORT int Save_WaterBlock(struct Element *element) {
    struct WaterNode *current;
    unsigned int one;
    unsigned int zero;

    one = 1;
    zero = 0;
    current = WaterBlockList;
    while (current != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(current, sizeof(struct WaterNode)) == 0) {
            return 0;
        }
        current = current->next;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00418b10
LEGO_EXPORT int Load_WaterBlock(struct Element *element) {
    struct WaterNode *node;
    struct WaterNode *prev;
    unsigned int count;

    prev = NULL;
    if (SaveGameRead(&count, 4) == 0) {
        return 0;
    }
    while (count != 0) {
        node = (struct WaterNode *)malloc(sizeof(struct WaterNode));
        if (SaveGameRead(node, sizeof(struct WaterNode)) == 0) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            WaterBlockList = node;
        }
        prev = node;
        if (SaveGameRead(&count, 4) == 0) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00418b90
LEGO_EXPORT int Save_ElephantF(struct Element *element) {
    struct WaterNode *current;
    unsigned int one;
    unsigned int zero;

    one = 1;
    zero = 0;
    current = ElephantFountainList;
    while (current != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(current, sizeof(struct WaterNode)) == 0) {
            return 0;
        }
        current = current->next;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00418c00
LEGO_EXPORT int Load_ElephantF(struct Element *element) {
    struct WaterNode *node;
    struct WaterNode *prev;
    unsigned int count;

    prev = NULL;
    if (SaveGameRead(&count, 4) == 0) {
        return 0;
    }
    while (count != 0) {
        node = (struct WaterNode *)malloc(sizeof(struct WaterNode));
        if (SaveGameRead(node, sizeof(struct WaterNode)) == 0) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            ElephantFountainList = node;
        }
        prev = node;
        if (SaveGameRead(&count, 4) == 0) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00418c80
void WaterWorksGetInterfaces(struct Element *element, struct Ride *ci) {
    // STRING: LEGOLAND 0x004b50f4
    if (_stricmp("WATER WORKS ENTRANCE", element->name) == 0) {
        ci->cb_a4 = (void (*)())WaterWorksEntranceLoad;
        ci->cb_ac = WaterWorksEntranceUnload;
        ci->cb_98 = WaterWorksEntranceAddObject;
        ci->cb_9c = WaterWorksEntranceRemoveObject;
        return;
    }
    // STRING: LEGOLAND 0x004b50dc
    if (_stricmp("WATER WORKS WATER BLOCK", element->name) == 0) {
        ci->cb_a4 = WaterBlockLoad;
        ci->cb_98 = WaterBlockAddObject;
        ci->cb_9c = WaterBlockRemoveObject;
        ci->cb_pre_render = WaterWorksWaterBlockUpdate;
        ci->cb_ac = WaterBlockUnload;
        ci->cb_sprite = GetWaterWorksWaterBlockSpriteInfo;
        ci->cb_b0 = RenderWaterBlock;
        ci->cb_90 = WaterWorksWaterBlockCalcCursor;
        ci->save_hook = Save_WaterBlock;
        ci->load_hook = Load_WaterBlock;
        return;
    }
    // STRING: LEGOLAND 0x004b50c8
    if (_stricmp("WATER WORKS SHOWER", element->name) == 0) {
        ci->cb_a4 = ShowerLoad;
        ci->cb_98 = ShowerAddObject;
        ci->cb_9c = ShowerRemoveObject;
        ci->cb_ac = ShowerUnload;
        ci->cb_sprite = GetWaterWorksShowerSpriteInfo;
        ci->cb_b0 = RenderShower;
        ci->cb_pre_render = WaterWorksShowerUpdate;
        ci->cb_90 = WaterWorksShowerCalcCursor;
        return;
    }
    // STRING: LEGOLAND 0x004b50a8
    if (_stricmp("WATER WORKS ELEPHANT FOUNTAIN", element->name) == 0) {
        ci->cb_a4 = ElephantFountainLoad;
        ci->cb_98 = ElephantFountainAddObject;
        ci->cb_9c = ElephantFountainRemoveObject;
        ci->cb_ac = ElephantFountainUnload;
        ci->cb_b0 = RenderWaterWorksElephantFountain;
        ci->cb_pre_render = WaterWorksElephantFountainUpdate;
        ci->cb_90 = WaterWorksElephantFountainCalcCursor;
        ci->save_hook = Save_ElephantF;
        ci->load_hook = Load_ElephantF;
        return;
    }
    // STRING: LEGOLAND 0x004b5088
    if (_stricmp("WATER WORKS CROCODILE FOUNTAIN", element->name) == 0) {
        ci->cb_90 = WaterWorksCrocodileFountainCalcCursor;
        ci->cb_98 = CrocodileFountainAddObject;
        ci->cb_9c = CrocodileFountainRemoveObject;
    }
}
