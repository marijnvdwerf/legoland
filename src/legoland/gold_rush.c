#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "bloke.h"
#include "gamemap.h"
#include "globals.h"
#include "gold_rush.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "objclass.h"
#include "path_control.h"
#include "print_sprite.h"
#include "render3d.h"
#include "ride_queue.h"
#include "screens.h"
#include "tilemap.h"

struct GoldArray {
    unsigned char pad_0[0x14];
    unsigned int field_14[6];
};

struct GoldSlot {
    int index;
    float weight;
};

#include "image_sprite.h"

// GLOBAL: LEGOLAND 0x004b45b0
static struct GoldSlot Gold_Slots[6] = {{0, 0.8f}, {0, 0.2f}, {1, 0.8f}, {1, 0.2f}, {2, 0.8f}, {2, 0.2f}};

// GLOBAL: LEGOLAND 0x004b45e0
static struct PathPair Gold_PathPairs[5] = {{-2, 0}, {0, -6}, {-6, 0}, {0, 1}, {3, 0}};
// GLOBAL: LEGOLAND 0x004b4608
struct PathTable GoldRushPathTable = {5, Gold_PathPairs};

// GLOBAL: LEGOLAND 0x004b4610
static struct Point Gold_Points[3] = {{0x400, 0x60}, {0x400, 0x3d0}, {0x400, 0x700}};

// FUNCTION: LEGOLAND 0x00406920
void AddGoldWashNode(struct GoldNode *src) {
    struct GoldNode *node = (struct GoldNode *)malloc(sizeof(struct GoldNode));
    if (node != NULL) {
        memset(node, 0, sizeof(struct GoldNode));
        node->key = src->key;
        node->next = GoldWashList;
        GoldWashList = node;
    }
}

// FUNCTION: LEGOLAND 0x00406960
void RemoveGoldWashNode(struct GoldNode *node) {
    struct GoldNode *prev;
    struct GoldNode *cur;

    if (GoldWashList == node) {
        GoldWashList = node->next;
    } else {
        cur = GoldWashList->next;
        prev = GoldWashList;
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

// FUNCTION: LEGOLAND 0x004069c0
void FreeGoldWashList(void) {
    while (GoldWashList != NULL) {
        RemoveGoldWashNode(GoldWashList);
    }
}

// FUNCTION: LEGOLAND 0x004069e0
void *FindGoldWashNode(void *param) {
    struct GoldNode *cur = GoldWashList;

    if (cur != NULL) {
        do {
            if (memcmp(&cur->key, param, 2) == 0) {
                return cur;
            }
            cur = cur->next;
        } while (cur != NULL);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00406a10
void GoldRushLoad(struct Element *obj) {
    struct Ride *inner = obj->ride;
    GoldRushRide = inner;
    if (inner != NULL) {
        inner->flags |= 0x20;
        if (((struct Ride *)GoldRushRide)->layer != NULL) {
            ((struct Ride *)GoldRushRide)->layer->flags |= 0x2000;
            GoldRushLayer = (struct Sprite *)((struct Ride *)GoldRushRide)->layer;
        }
    }

    GoldRushQueueTable = (struct Sprite *)BuildQueueTable(&GoldRushPathTable);
    // STRING: LEGOLAND 0x004b465c
    GoldWashMatte1Sprite = LoadSprite("goldwashmatte1.lls", 1);
    // STRING: LEGOLAND 0x004b464c
    GoldWashSprite = LoadSprite("goldwash.lls", 1);
    // STRING: LEGOLAND 0x004b4638
    GoldWashMatte2Sprite = LoadSprite("goldwashmatte2.lls", 1);
    // STRING: LEGOLAND 0x004b4628
    GoldMaskSprite = LoadSprite("goldmask.lls", 1);
}

// FUNCTION: LEGOLAND 0x00406ab0
void GoldRushUnload(void) {
    if (GoldWashMatte1Sprite != NULL) {
        KillSprite(GoldWashMatte1Sprite);
    }
    if (GoldWashSprite != NULL) {
        KillSprite(GoldWashSprite);
    }
    if (GoldWashMatte2Sprite != NULL) {
        KillSprite(GoldWashMatte2Sprite);
    }
    if (GoldMaskSprite != NULL) {
        KillSprite(GoldMaskSprite);
    }
    if (GoldRushQueueTable != NULL) {
        FreeIfNotNull(GoldRushQueueTable);
    }
    FreeGoldWashList();
}

// FUNCTION: LEGOLAND 0x00406b10
void RenderGoldRush(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = obj->ride;
    struct RideNode *node;
    struct Point pos;
    int frame;
    struct Sprite *spr;
    struct LLS *lls;

    pos = GetScreenCoordsForObject((TileId *)tile, ride);
    RenderItems_New();
    GoldRushBlokeRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (*tile == node->tile.id && node->rider->pos.x <= (((unsigned char *)tile)[0] << 8) + 0x780 && node->rider->pos.y <= (((unsigned char *)tile)[1] << 8) - 0x280) {
            AddBlokeToRenderList(&GoldRushBlokeRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&GoldRushBlokeRenderList);
    if (GoldMaskSprite != NULL) {
        struct Point off;
        frame = 0;
        spr = GetSpriteForLayer(GoldRushLayer, 1);
        if (spr != NULL) {
            lls = GetLLSForSprite((struct Sprite *)spr);
            if (lls != NULL) {
                frame = *(short *)lls % 8;
            }
        }
        lls = GetLLSForSprite((struct Sprite *)GoldMaskSprite);
        if (lls != NULL) {
            LLSSetFrame(lls, frame);
        }
        off = GetRenderOffsetForLayer(GoldRushLayer, 1);
        AdjustOffsetForViewMode(&off);
        PrintSprite(GoldMaskSprite, pos.x + off.x, pos.y + off.y, clip, 0);
    }
    RenderItems_New();
    GoldRushBlokeRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (*tile == node->tile.id && node->rider->pos.x <= (((unsigned char *)tile)[0] << 8) + 0x780 && node->rider->pos.y >= (((unsigned char *)tile)[1] << 8) - 0x280) {
            AddBlokeToRenderList(&GoldRushBlokeRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&GoldRushBlokeRenderList);
    if (GoldWashMatte2Sprite != NULL) {
        struct Point off;
        off = GetRenderOffsetForLayer(GoldRushLayer, 1);
        AdjustOffsetForViewMode(&off);
        PrintSprite(GoldWashMatte2Sprite, pos.x + off.x, pos.y + off.y, clip, 0);
    }
    RenderItems_New();
    GoldRushBlokeRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (*tile == node->tile.id && node->rider->pos.x > (((unsigned char *)tile)[0] << 8) + 0x780) {
            AddBlokeToRenderList(&GoldRushBlokeRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&GoldRushBlokeRenderList);
    if (GoldWashMatte1Sprite != NULL) {
        struct Point off;
        off = GetRenderOffsetForLayer(GoldRushLayer, 1);
        AdjustOffsetForViewMode(&off);
        PrintSprite(GoldWashMatte1Sprite, pos.x + off.x, pos.y + off.y, clip, 0);
    }
    RenderItems_New();
    GoldRushBlokeRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (*tile == node->tile.id && (node->rider->pos.x >> 8) > ((unsigned char *)tile)[0] + 8) {
            AddBlokeToRenderList(&GoldRushBlokeRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&GoldRushBlokeRenderList);
    if (GoldRushLayer != NULL) {
        struct Point off;
        off = GetRenderOffsetForLayer(GoldRushLayer, 3);
        AdjustOffsetForViewMode(&off);
        PrintSprite(GetSpriteForLayer(GoldRushLayer, 3), pos.x + off.x, pos.y + off.y, clip, 0);
    }
}

// FUNCTION: LEGOLAND 0x00406e90
unsigned int GoldWashHasFreeSlot(void *param) {
    struct GoldArray *a = (struct GoldArray *)FindGoldWashNode(param);
    unsigned int i;

    if (a == NULL) {
        return 0;
    }
    for (i = 0; i < 6; i++) {
        if (a->field_14[i] == 0) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00406ec0
void TakeGoldWashSlot(struct RideNode *item, void *param2) {
    struct GoldArray *a = (struct GoldArray *)FindGoldWashNode(param2);
    unsigned int i;

    if (a == NULL) {
        return;
    }
    for (i = 0; i < 6; i++) {
        if (a->field_14[i] == 0) {
            a->field_14[i] = 1;
            item->rider->field_36 = (unsigned char)i;
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x00406f00
struct GoldArray *ReleaseGoldWashSlot(struct RideNode *item) {
    unsigned int idx = item->rider->field_36;
    struct GoldArray *a = (struct GoldArray *)FindGoldWashNode(&item->tile.pos.x);

    if (a != NULL) {
        a->field_14[idx] = 0;
    }
    return a;
}

// FUNCTION: LEGOLAND 0x00406f30
void UpdateGoldWashAvailability(void *param) {
    if (GoldWashHasFreeSlot(param) == 0) {
        Ride_SetFlagToNotLetAnyoneOn(param);
    } else {
        Ride_ClearFlagToNotLetAnyoneOn(param);
    }
}

/* Gold rush: walks the rider to the centre of its wash slot point (Gold_Points of its slot, offset by the ride tile
 * p) and faces the move. */
// FUNCTION: LEGOLAND 0x00406f60
void WalkGoldRushRiderToSlotCentre(struct RideNode *item, unsigned char *p) {
    struct Bloke *b = item->rider;
    int slot = Gold_Slots[b->field_36].index;

    b->dest.x = Gold_Points[slot].x;
    b->dest.y = Gold_Points[slot].y;
    b->dest.x += p[0] << 8;
    b->dest.y += p[1] << 8;
    b->dest.x += 0x80;
    b->dest.y += 0x80;
    b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
    b->low_level_action = 7;
    NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
}

/* Gold rush: walks the rider to the approach point 2.5 tiles to the -x side of its slot point, shifted further by
 * the slot weight (weight * 2 tiles). */
// FUNCTION: LEGOLAND 0x00407000
void WalkGoldRushRiderToSlotApproach(struct RideNode *item, unsigned char *p) {
    struct Bloke *b = item->rider;
    struct GoldSlot *s = &Gold_Slots[b->field_36];
    int slot = s->index;
    float w = s->weight;

    b->dest.x = Gold_Points[slot].x - 0x280;
    b->dest.y = Gold_Points[slot].y;
    b->dest.x += p[0] << 8;
    b->dest.y += (p[1] << 8) + 0x80;
    b->dest.x += 0x80 - (int)(w * 512.0f);
    b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
    b->low_level_action = 7;
    NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
}

// Sends a gold-rush rider to its panning spot: the slot's point, offset by the tile in `p` and by
// the slot's weight, then starts the walk (low-level action 7) facing the new direction.
// FUNCTION: LEGOLAND 0x004070b0
void SendGoldRushRiderToSlot(struct RideNode *item, unsigned char *p) {
    struct Bloke *b = item->rider;
    struct GoldSlot *s = &Gold_Slots[b->field_36];
    int slot = s->index;
    float w = s->weight;

    b->dest.x = Gold_Points[slot].x - 0x280;
    b->dest.y = Gold_Points[slot].y;
    b->dest.x += p[0] << 8;
    b->dest.y += p[1] << 8;
    b->dest.x += 0x80 - (int)(w * 512.0f);
    b->dest.y -= 0x50;
    b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
    b->low_level_action = 7;
    NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
}

/* Gold rush: after panning, moves the rider half a tile down and walks it back to the same approach point as
 * WalkGoldRushRiderToSlotApproach. */
// FUNCTION: LEGOLAND 0x00407170
void ReturnGoldRushRiderToSlotApproach(struct RideNode *item, unsigned char *p) {
    struct Bloke *b = item->rider;
    struct GoldSlot *s = &Gold_Slots[b->field_36];
    int slot = s->index;
    float w = s->weight * 512.0f;

    b->pos.y += 0x80;
    b->dest.x = Gold_Points[slot].x - 0x280;
    b->dest.y = Gold_Points[slot].y;
    b->dest.x += p[0] << 8;
    b->dest.y += (p[1] << 8) + 0x80;
    b->dest.x += 0x80 - (int)w;
    b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
    b->low_level_action = 7;
    NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
}

// FUNCTION: LEGOLAND 0x00407230
void SetRandomGoldPanTime(struct RideNode *param) {
    int value = rand() % 31;
    param->rider->field_58 = value + 15;
}

/* Gold rush: one tick of a visitor panning at a wash slot (pan animation, facing direction 1). When field_58 runs
 * out, releases the slot and moves on to the next step. */
// FUNCTION: LEGOLAND 0x00407250
void GoldRushWashPan(struct RideNode *ref) {
    struct Bloke *b = ref->rider;

    b->flags |= 0x100;
    BlokePanWithPan((struct Bloke *)b);
    BlokeAnimNextFrame((struct Bloke *)b);
    b->dir = 1;
    b->field_58 = b->field_58 - 1;
    if (b->field_58 <= 0) {
        b->flags &= 0xfeff;
        BlokeWalkWithPan((struct Bloke *)b);
        ReleaseGoldWashSlot((struct RideNode *)ref);
        UpdateGoldWashAvailability(&ref->tile.pos.x);
        b->param_action++;
    }
}

// FUNCTION: LEGOLAND 0x004072b0
void GoldRushUpdate(struct Element *elem) {
    struct Ride *ride = elem->ride;
    struct RideNode *node = ride->riders;
    struct RideNode *next;
    struct Bloke *b;
    unsigned char *t;
    int x;
    int y;

    while (node != NULL) {
        next = node->next;
        t = (unsigned char *)&node->tile;
        b = node->rider;
        x = t[0] + ride->x;
        y = t[1] + ride->y;
        if (b->low_level_action == 0) {
            switch (b->param_action) {
            case 0:
                b->flags |= 8;
                TakeGoldWashSlot((struct RideNode *)node, t);
                UpdateGoldWashAvailability(t);
                StartQueueTableForward((struct QueueTable *)GoldRushQueueTable, (struct Bloke *)b);
                break;
            case 1:
                MoveBlokeAlongQueueTable((struct QueueTable *)GoldRushQueueTable, x, y, b);
                if ((b->pos.x >> 8) == t[0] + 5 && (b->pos.y >> 8) == t[1] - 3) {
                    BlokeWalkWithPan(b);
                }
                break;
            case 2:
                b->param_action++;
                b->dest.x = (t[0] << 8) + 0x480;
                b->dest.y = (t[1] << 8) - 0x180;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                break;
            case 3:
            case 8:
                WalkGoldRushRiderToSlotCentre((struct RideNode *)node, t);
                b->param_action++;
                break;
            case 4:
                WalkGoldRushRiderToSlotApproach((struct RideNode *)node, t);
                b->param_action++;
                break;
            case 5:
                SendGoldRushRiderToSlot((struct RideNode *)node, t);
                SetRandomGoldPanTime((struct RideNode *)node);
                b->param_action++;
                break;
            case 6:
                GoldRushWashPan((struct RideNode *)node);
                break;
            case 7:
                ReturnGoldRushRiderToSlotApproach((struct RideNode *)node, t);
                b->param_action++;
                break;
            case 9:
                b->dest.x = (t[0] << 8) + 0x480;
                b->dest.y = (t[1] << 8) - 0x180;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                break;
            case 10:
                StartQueueTableBackward((struct QueueTable *)GoldRushQueueTable, (struct Bloke *)b);
                break;
            case 11:
                MoveBlokeAlongQueueTable((struct QueueTable *)GoldRushQueueTable, x, y, b);
                if ((b->pos.x >> 8) == t[0] + 5 && (b->pos.y >> 8) == t[1] - 3) {
                    BlokeWalkAnim(b);
                }
                break;
            case 12:
                b->dest.x = (x << 8) + 0x80;
                b->dest.y = (y << 8) + 0x80;
                b->target_dir = CalcMoveLine(b->pos, b->dest, &b->nav) + 0x10;
                b->low_level_action = 7;
                NewDirForAction(b, (unsigned char)((b->target_dir >> 5) + 3));
                b->param_action++;
                break;
            case 13:
                RemoveBlokeFromRide(ride, node);
                b->flags &= 0xfff7;
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x004075b0
void GoldRushSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = GoldRushRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x004075f0
void GoldRushAddObject(struct Element *editObj, struct Point *pos) {
    TileId id;
    struct Point p;
    struct Ride *ride;

    id.pos.x = (unsigned char)pos->x;
    id.pos.y = (unsigned char)pos->y;
    ride = editObj->ride;
    AddBasicObject(editObj, &pos->x);
    AddGoldWashNode((struct GoldNode *)&id);

    p.x = pos->x + ride->x - 1;
    p.y = pos->y + ride->y;
    AddPathTileGFX(&p, *(unsigned short *)PathSprite);

    p.x = pos->x + ride->x - 2;
    p.y = pos->y + ride->y;
    AddPathTileGFX(&p, *(unsigned short *)PathSprite);

    p.x = pos->x + ride->x - 2;
    p.y = pos->y + ride->y - 1;
    AddPathTileGFX(&p, *(unsigned short *)PathSprite);

    p.x = pos->x + ride->x - 2;
    p.y = pos->y + ride->y - 2;
    AddPathTileGFX(&p, *(unsigned short *)PathSprite);
}

// FUNCTION: LEGOLAND 0x004076e0
void GoldRushRemoveObject(struct Element *editObj, TileId coords, struct Cursor *cursor) {
    int p[2];
    struct Ride *ride = editObj->ride;
    void *found = FindGoldWashNode(&coords);

    if (found != NULL) {
        RemoveGoldWashNode((struct GoldNode *)found);
    }

    StandardRemoveObject(editObj, coords, cursor);
    RemoveAllBlokesFromRide((struct Ride *)ride, coords);

    p[0] = coords.pos.x + ride->x - 1;
    p[1] = coords.pos.y + ride->y;
    RemoveRollerCoasterPath(p);

    p[0] = coords.pos.x + ride->x - 2;
    p[1] = coords.pos.y + ride->y;
    RemoveRollerCoasterPath(p);

    p[0] = coords.pos.x + ride->x - 2;
    p[1] = coords.pos.y + ride->y - 1;
    RemoveRollerCoasterPath(p);

    p[0] = coords.pos.x + ride->x - 2;
    p[1] = coords.pos.y + ride->y - 2;
    RemoveRollerCoasterPath(p);
}

// FUNCTION: LEGOLAND 0x00407800
LEGO_EXPORT int SaveGoldWash(struct Element *element) {
    struct GoldNode *node;
    unsigned int one;
    unsigned int zero;

    one = 1;
    zero = 0;
    node = GoldWashList;
    while (node != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(node, sizeof(struct GoldNode)) == 0) {
            return 0;
        }
        node = node->next;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00407870
LEGO_EXPORT int LoadGoldWash(struct Element *element) {
    unsigned int count;
    struct GoldNode *node;
    struct GoldNode *prev = NULL;

    if (!SaveGameRead(&count, 4)) {
        return 0;
    }
    while (count != 0) {
        node = malloc(sizeof(struct GoldNode));
        if (!SaveGameRead(node, sizeof(struct GoldNode))) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            GoldWashList = node;
        }
        prev = node;
        if (!SaveGameRead(&count, 4)) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004078f0
void GoldRush_GetInterfaces(struct Element *str, struct Ride *module) {
    // STRING: LEGOLAND 0x004b4670
    if (_stricmp("GOLD RUSH", str->name) == 0) {
        module->cb_a4 = GoldRushLoad;
        module->cb_ac = GoldRushUnload;
        module->cb_8c = GoldRushSetEditMode;
        module->cb_pre_render = GoldRushUpdate;
        module->cb_b0 = RenderGoldRush;
        module->cb_98 = GoldRushAddObject;
        module->cb_9c = GoldRushRemoveObject;
        module->load_hook = LoadGoldWash;
        module->save_hook = SaveGoldWash;
    }
}
