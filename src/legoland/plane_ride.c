#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "binv.h"
#include "bloke.h"
#include "gamemap.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "obj_instance.h"
#include "plane_ride.h"
#include "print_sprite.h"
#include "render3d.h"
#include "sound_music.h"
#include "sound_sfx.h"
#include "tilemap.h"

#include "image_sprite.h"
#include "objclass.h"

// FUNCTION: LEGOLAND 0x0043d880
void AddPlaneRideNode(void *param_1) {
    void *node = malloc(sizeof(struct PlaneRideNode));
    if (node == NULL) {
        return;
    }
    memset(node, 0, sizeof(struct PlaneRideNode));
    *(unsigned short *)node = *(unsigned short *)param_1;
    ((struct PlaneRideNode *)node)->next = PlaneRideNodeList;
    PlaneRideNodeList = node;
    ResetPlaneRide(node);
}

// FUNCTION: LEGOLAND 0x0043d8c0
void RemovePlaneRideNode(struct PlaneRideNode *node) {
    struct PlaneRideNode *prev;
    struct PlaneRideNode *cur;
    struct SampleSource src;

    if (PlaneRideNodeList == node) {
        PlaneRideNodeList = node->next;
    } else {
        cur = PlaneRideNodeList->next;
        prev = PlaneRideNodeList;
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
    src.type = 2;
    src.x = node->b0;
    src.y = node->b1;
    UnSourceAndFadeAllSamplesFromSource(&src, -200);
    free(node);
}

// FUNCTION: LEGOLAND 0x0043d940
void RemoveAllPlaneRideNodes(void) {
    while (PlaneRideNodeList != NULL) {
        RemovePlaneRideNode(PlaneRideNodeList);
    }
}

// FUNCTION: LEGOLAND 0x0043d960
struct PlaneRideNode *FindPlaneRideNode(TileId *arg) {
    struct PlaneRideNode *cur = PlaneRideNodeList;

    if (cur != NULL) {
        do {
            if (memcmp(&cur->b0, arg, 2) == 0) {
                return cur;
            }
            cur = cur->next;
        } while (cur != NULL);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0043d990
void StartPlaneRide(struct PlaneRideNode *node) {
    int cfg[4];

    cfg[0] = 2;
    node->flags &= ~0x4000;
    node->b3 = node->b2;
    node->flags |= 1;
    node->b2 = 0;
    node->b4 = 0;
    node->f10 = 0;
    cfg[2] = node->b0;
    cfg[3] = node->b1;
    PlayInstanceOfSample(PlaneRideFX[0].sample, 1, 1, cfg);
}

// FUNCTION: LEGOLAND 0x0043d9f0
void ResetPlaneRide(struct PlaneRideNode *node) {
    int cfg[4];

    node->f10 = 0;
    node->b4 = 0;
    node->b12 = rand() % 2 != 0 ? 2 : 1;
    node->b14 = 0;
    node->b2 = 0;
    node->flags &= ~0x4001;
    cfg[0] = 2;
    cfg[2] = node->b0;
    cfg[3] = node->b1;
    UnSourceAndFadeAllSamplesFromSource(cfg, -200);
}

// FUNCTION: LEGOLAND 0x0043da60
void RenderPlaneRide(struct Element *element, unsigned int param_2, unsigned int param_3, TileId *tile, void *param_5, unsigned int param_6) {
    struct Ride *ride;
    struct RideNode *r;
    register struct Point coords;
    char i;
    struct Point off;
    struct PlaneRideNode *node;
    struct Bloke *rider;
    int x;
    struct Bloke *riders[4] = {0};
    char n;
    struct Bloke *bloke;
    struct Person *person;

    ride = element->ride;
    r = ride->riders;
    n = 0;
    node = FindPlaneRideNode(tile);
    if (node == NULL) {
        return;
    }
    coords = GetScreenCoordsForObject(tile, ride);
    if (r != NULL) {
        for (; r != NULL; r = r->next) {
            if (tile->id == r->tile.id) {
                riders[n++] = r->rider;
            }
        }
        if (n != 0) {
            i = 0;
            while (n > i) {
                if (riders[i]->param_action == 0xd) {
                    rider = riders[i];
                    IP_RenderBlokeIn3DNow(rider);
                }
                i++;
            }
            for (i = 0; i < n; i++) {
                if (riders[i]->param_action == 0xe) {
                    IP_RenderBlokeIn3DNow(riders[i]);
                }
            }
            LLSSetFrame(GetLLSForLayer(PlaneRideLayer, 1), node->b4);
            off = GetRenderOffsetForLayer(ride->layer, 1);
            AdjustOffsetForViewMode(&off);
            PrintSprite(GetSpriteForLayer(ride->layer, 1), coords.x + off.x, coords.y + off.y, param_6, 0);
            r = ride->riders;
            *(short *)*ZoomerSprite->lls = node->b4;
            while (r != NULL) {
                if (tile->id == r->tile.id) {
                    struct Point off2;
                    bloke = r->rider;
                    if (bloke->flags & 0x80) {
                        person = bloke->person;
                        off2.x = PlaneRideDrawOffsetX;
                        off2.y = PlaneRideDrawOffsetY;
                        person->offset.x = bloke->screen_x;
                        person->offset.y = bloke->screen_y;
                        AdjustBlokePosition(&person->offset);
                        AdjustOffsetForViewMode(&off2);
                        person->screen.x = bloke->screen_x + coords.x + off2.x;
                        person->screen.y = bloke->screen_y + coords.y + off2.y;
                        AdjustBlokePosition(&person->screen);
                        IP_RenderBlokeIn3DNow(r->rider);
                    }
                }
                r = r->next;
            }
            LLSSetFrame(GetLLSForLayer(PlaneRideLayer, 2), node->b5);
            off = GetRenderOffsetForLayer(ride->layer, 2);
            AdjustOffsetForViewMode(&off);
            x = coords.x;
            PrintSprite(GetSpriteForLayer(ride->layer, 2), off.x + x, coords.y + off.y, param_6, 0);
            return;
        }
    }
    LLSSetFrame(GetLLSForLayer(PlaneRideLayer, 1), node->b4);
    off = GetRenderOffsetForLayer(ride->layer, 1);
    AdjustOffsetForViewMode(&off);
    PrintSprite(GetSpriteForLayer(ride->layer, 1), coords.x + off.x, coords.y + off.y, param_6, 0);
    LLSSetFrame(GetLLSForLayer(PlaneRideLayer, 2), node->b5);
    off = GetRenderOffsetForLayer(ride->layer, 2);
    AdjustOffsetForViewMode(&off);
    PrintSprite(GetSpriteForLayer(ride->layer, 2), coords.x + off.x, coords.y + off.y, param_6, 0);
}

// FUNCTION: LEGOLAND 0x0043dda0
void PlaneRideLoad(Element *input) {
    struct LayerResult layer;

    PlaneRide = input->ride;
    PlaneRide->flags |= 0x420;
    PlaneRideLayer = PlaneRide->layer;
    PlaneRideLayer->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b7a54
    ZoomerideBinV = LoadBinV("Zbuffers\\Zoomeride.bnv");
    // STRING: LEGOLAND 0x004b7a3c
    Zoomer0nBinV = LoadBinV("Zbuffers\\Zoomer0n.bnv");
    // STRING: LEGOLAND 0x004b7a24
    Zoomer0ffBinV = LoadBinV("Zbuffers\\Zoomer0ff.bnv");
    // STRING: LEGOLAND 0x004b7a14
    PlaneRideRiderSprite = ZoomerSprite = LoadSprite("z_Zoomer.lls", 1);
    PlaneRideDrawOffsetX = -10;
    PlaneRideDrawOffsetY = -0x6b;
    PlaneRideBinVs[0] = ZoomerideBinV;
    PlaneRideBinVs[1] = Zoomer0nBinV;
    PlaneRideBinVs[2] = Zoomer0ffBinV;
    Load_FXList(PlaneRideFX, 2);
    HideLayer(PlaneRideLayer, 1);
    StopLayerPlaying(PlaneRideLayer, 1);
    LLSSetFrame(GetLLSForLayer(PlaneRideLayer, 1), 0);
    HideLayer(PlaneRideLayer, 2);
    StopLayerPlaying(PlaneRideLayer, 2);
    LLSSetFrame(GetLLSForLayer(PlaneRideLayer, 2), 0);
    GetLayer(PlaneRide->layer, &layer, 1);
}

// FUNCTION: LEGOLAND 0x0043dee0
void PlaneRideUnload(struct Element *input) {
    PlaneRide = input->ride;
    if (ZoomerSprite) {
        KillSprite(ZoomerSprite);
    }
    if (ZoomerideBinV) {
        FreeBinV(ZoomerideBinV);
    }
    if (Zoomer0nBinV) {
        FreeBinV(Zoomer0nBinV);
    }
    if (Zoomer0ffBinV) {
        FreeBinV(Zoomer0ffBinV);
    }
    RemoveAllPlaneRideNodes();
    Kill_FXList(PlaneRideFX, 2);
}

// FUNCTION: LEGOLAND 0x0043df50
void PlaneRideSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = (void *)PlaneRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x0043df90
void PlaneRideRemoveObject(struct Element *a1, TileId a2, struct Cursor *a3) {
    struct PlaneRideNode *temp = FindPlaneRideNode(&a2);
    if (temp != 0) {
        RemovePlaneRideNode(temp);
    }
    StandardRemoveObject(a1, a2, a3);
    RemoveAllBlokesFromRide(a1->ride, a2);
}

// FUNCTION: LEGOLAND 0x0043dfe0
void PlaneRideAddObject(Element *a, int *p) {
    unsigned char c[2];

    c[0] = *(unsigned char *)p;
    c[1] = ((unsigned char *)p)[4];
    AddBasicObject(a, p);
    AddPlaneRideNode(c);
}

// FUNCTION: LEGOLAND 0x0043e010
struct RideSpriteInfo *GetPlaneRideSpriteInfo(struct Element *param1, unsigned short param2) {
    struct Ride *block = param1->ride;

    PlaneRideSpriteInfo.sprite = block->layer;
    PlaneRideSpriteInfo.x = block->field_14;
    PlaneRideSpriteInfo.y = block->field_18;
    PlaneRideSpriteInfo.id = param2;
    block->layer->flags |= 0x2000;

    return &PlaneRideSpriteInfo;
}

/* Searches cyclically from a random seat for a free slot, marks it occupied, and stores and returns its one-based
 * seat number for the rider. */
// FUNCTION: LEGOLAND 0x0043e050
unsigned int ReservePlaneRideSeat(struct RideNode *rn, struct PlaneRideNode *node, signed char n) {
    int count = n;
    int eax = rand();
    int index = eax % count;
    signed char slot = node->slots[index];

    while (slot != 0) {
        index++;
        if (index >= count) {
            index = 0;
        }
        slot = node->slots[index];
    }

    node->slots[index] = 1;
    rn->rider->field_36 = (signed char)(index + 1);
    return index + 1;
}

// FUNCTION: LEGOLAND 0x0043e0a0
LEGO_EXPORT int SaveZoomer(struct Element *element) {
    struct PlaneRideNode *current = PlaneRideNodeList;
    unsigned int flag = 1;
    unsigned int terminator = 0;

    while (current != NULL) {
        if (!SaveGameWrite(&flag, 4)) {
            return 0;
        }
        if (!SaveGameWrite(current, sizeof(struct PlaneRideNode))) {
            return 0;
        }
        current = current->next;
    }

    if (SaveGameWrite(&terminator, 4)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0043e110
LEGO_EXPORT int LoadZoomer(struct Element *arg) {
    struct Ride *obj = arg->ride;
    struct PlaneRideNode *prev = NULL;
    struct RideNode *list;
    struct Person *car;
    struct Bloke *data;
    struct BNVRef *tc;
    unsigned int marker;

    if (!SaveGameRead(&marker, 4)) {
        return 0;
    }
    while (marker != 0) {
        struct PlaneRideNode *node = (struct PlaneRideNode *)malloc(sizeof(struct PlaneRideNode));
        if (!SaveGameRead(node, sizeof(struct PlaneRideNode))) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            PlaneRideNodeList = node;
        }
        {
            struct TileSampleSource params = {2, ((unsigned char *)node)[0], ((unsigned char *)node)[1]};
            PauseSingleSample(PlayInstanceOfSample(PlaneRideFX[0].sample, 1, 1, &params));
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
            car->sprite = (&Zoomer0nBinV)[car->field_30];
        } else {
            car->sprite = NULL;
            list->person->field_30 = 0;
        }
        data = list->rider;
        tc = data->bnv;
        if (tc != NULL) {
            tc->file = PlaneRideBinVs[tc->index];
        }
        list = list->next;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0043e220
void PlaneRide_GetInterfaces(struct Element *name, struct Ride *iface) {
    // STRING: LEGOLAND 0x004b7a6c
    if (_stricmp("PLANE RIDE", name->name) == 0) {
        iface->cb_a4 = PlaneRideLoad;
        iface->cb_ac = PlaneRideUnload;
        iface->cb_8c = PlaneRideSetEditMode;
        iface->cb_pre_render = PlaneRideUpdate;
        iface->cb_b0 = RenderPlaneRide;
        iface->cb_9c = PlaneRideRemoveObject;
        iface->cb_98 = PlaneRideAddObject;
        iface->cb_sprite = GetPlaneRideSpriteInfo;
        iface->load_hook = LoadZoomer;
        iface->save_hook = SaveZoomer;
    }
}

// FUNCTION: LEGOLAND 0x0043e2b0
void UpdatePlaneRideNode(struct PlaneRideNode *node) {
    struct RideNode *r = PlaneRide->riders;
    unsigned int flags;

    if (++node->b5 >= 0x18) {
        node->b5 = 0;
    }
    flags = node->flags;
    if (flags & 1) {
        int v = ++node->f10;
        char c = node->b12;
        if (c == 0) {
            if (GetAllBlokesOffRide(PlaneRide, *(unsigned short *)node) == 0) {
                return;
            }
            ResetPlaneRide(node);
            return;
        }
        if (v >= 2) {
            node->f10 = 0;
            if (++node->b4 >= 0x61) {
                node->b4 = 0;
                node->b12 = c - 1;
            }
        }
    } else {
        unsigned char cur = node->b2;
        if (flags & 0x4000) {
            if (cur == node->b14) {
                node->flags = flags & 0xffffbfff;
                StartPlaneRide(node);
                return;
            }
        } else if (cur != 0) {
            int k = node->f18;
            if (k == 0) {
                node->flags = flags | 0x4000;
                Ride_SetFlagToNotLetAnyoneOn(node);
            } else {
                node->f18 = k - 1;
            }
        }
    }
    for (; r != NULL; r = r->next) {
        if (*(unsigned short *)node == r->tile.id && r->rider->field_35 == 1) {
            // STRING: LEGOLAND 0x004b4704
            sprintf(PlaneRideManboxName + 6, "%02d", r->rider->field_36);
            SetBlokePositionFromBNV(ZoomerideBinV, r->rider, PlaneRideManboxName, node->b4, -1617706.75f, -1617948.625f, 0);
        }
    }
    *(short *)*ZoomerSprite->lls = (short)node->b4;
}

// FUNCTION: LEGOLAND 0x0043e3f0
void UpdatePlaneRideNodes(void) {
    struct PlaneRideNode *current = PlaneRideNodeList;

    while (current != NULL) {
        UpdatePlaneRideNode(current);
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x0043e410
void PlaneRideUpdate(struct Element *elem) {
    struct Ride *ride = elem->ride;
    struct RideNode *rn = ride->riders;
    struct RideNode *next;
    struct PlaneRideNode *node;
    struct Bloke *bloke;
    TileId *pos;
    struct Point sc; /* screen position of the ride */
    int tw, th;
    int ox, d; /* screen x of the bloke, and its offset from the map position */
    int x, y;
    int tx, ty; /* tile the rider walks back to */
    int coords[3]; /* BNV start position (boarding path) */
    struct Point tmp;
    struct Point coords2; /* BNV start position (leaving path) */

    while (NULL != rn) {
        next = rn->next;
        bloke = rn->rider;
        pos = &rn->tile;
        node = FindPlaneRideNode(pos);
        if (!node) {
            return;
        }
        if (0 == bloke->low_level_action) {
            switch (bloke->param_action) {
            case 0:
                node->b14++;
                node->f18 = 0xb4;
                bloke->flags |= 8;
                sc = GetScreenCoordsForObject(pos, ride);
                x = bloke->pos.x;
                y = bloke->pos.y;
                GetTileDimensions(&tw, &th);
                d = (x - y) * tw >> 9;
                x = (x + y) * th >> 9;
                ox = lpConfig->view_x - (short)Get_XScroll() + d;
                x = x + (lpConfig->view_y - (short)Get_YScroll());
                coords[0] = (ox - PlaneRideDrawOffsetX / 2 - sc.x) * 2;
                coords[1] = (x - PlaneRideDrawOffsetY / 2 - sc.y) * 2;
                bloke->flags |= 0x80;
                bloke->person->sprite = PlaneRideRiderSprite;
                bloke->person->field_30 = 1;
                bloke->person->depth = GetUnitDepth(-1617706.75f, -1617948.625f);
                bloke->field_35 = 0;
                // STRING: LEGOLAND 0x004b4704
                sprintf(PlaneRideManboxName + 6, "%02d", ReservePlaneRideSeat(rn, node, (char)PlaneRide->seats));
                bloke->path = NewBNVPath(PlaneRideBinVs[1], 1, PlaneRideManboxName, -1617706.75f, -1617948.625f, coords);
                UpdateBlokeFromBNVPath(bloke, bloke->path);
                bloke->param_action++;
                bloke->field_58 = 0;
                break;
            case 1:
                if (UpdateBlokeFromBNVPath(bloke, bloke->path) == 0) {
                    bloke->field_35 = 1;
                    bloke->param_action = 5;
                    free(bloke->path);
                    bloke->path = NULL;
                }
                if (bloke->path != NULL && BNVPath_GetDFrame(bloke->path) >= 0x3f) {
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
                bloke->person->depth = GetUnitDepth(-1617706.75f, -1617948.625f);
                bloke->param_action++;
                if ((char)++node->b2 == PlaneRide->seats) {
                    StartPlaneRide(node);
                }
                break;
            case 7:
                tmp.x = bloke->screen_x * 2;
                tmp.y = bloke->screen_y * 2;
                BlokeWalkAnim(bloke);
                BlokeSetFrame(bloke, 0);
                UnAdjustBlokePosition(&tmp);
                coords2 = tmp;
                bloke->flags |= 0x80;
                bloke->person->sprite = PlaneRideRiderSprite;
                bloke->person->field_30 = 1;
                bloke->person->depth = GetUnitDepth(-1617706.75f, -1617948.625f);
                bloke->field_35 = 2;
                sprintf(PlaneRideManboxName + 6, "%02d", bloke->field_36);
                bloke->path = NewBNVPath(PlaneRideBinVs[2], 2, PlaneRideManboxName, -1617706.75f, -1617948.625f, &coords2.x);
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
                if (bloke->path != NULL && BNVPath_GetDFrame(bloke->path) >= 0x20) {
                    bloke->field_35 = 2;
                    bloke->param_action = 0xd;
                    free(bloke->path);
                    bloke->path = NULL;
                }
                BlokeSetFrame(bloke, bloke->frame);
                break;
            case 0xd:
                tx = ride->field_24 + pos->pos.x;
                ty = pos->pos.y + ride->field_25;

                node->slots[bloke->field_36 - 1] = 0;
                bloke->flags &= 0xff7f;
                bloke->person->sprite = NULL;
                bloke->person->field_30 = 0;
                UnAdjustBlokePosition(&bloke->person->screen);
                ScreenToMapRef(&bloke->person->screen.x, &bloke->pos.x, 0);
                bloke->person->field_34 = 0;
                bloke->pos.x <<= 8;
                bloke->pos.y <<= 8;
                bloke->dest.x = tx * 256 + 128;
                bloke->dest.y = ty * 256 + 128;
                bloke->target_dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
                bloke->low_level_action = 7;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 0xe:
                bloke->flags = bloke->flags & 0xfff7;
                RemoveBlokeFromRide(ride, rn);
                if (--node->b3 == 0) {
                    node->b2 = 0;
                    Ride_ClearFlagToNotLetAnyoneOn(node);
                }
                break;
            }
        }
        rn = next;
    }
    UpdatePlaneRideNodes();
}
