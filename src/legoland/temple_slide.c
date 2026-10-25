#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "binv.h"
#include "bloke.h"
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
#include "temple_slide.h"

struct TempleRide {
    unsigned char pad_0[4];
    unsigned int var_4;
    unsigned char pad_8[4];
    unsigned int var_c;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x00416ec0
void AddSlideNode(TileId *key) {
    struct SlideNode *node = (struct SlideNode *)malloc(sizeof(struct SlideNode));
    if (node != NULL) {
        memset(node, 0, sizeof(struct SlideNode));
        node->key = key->id;
        node->next = SlideNodeList;
        SlideNodeList = node;
        ClearTempleRideVar4AndFlag1((struct TempleRide *)node);
    }
}

// FUNCTION: LEGOLAND 0x00416f00
void RemoveSlideNode(struct SlideNode *node) {
    struct SlideNode *prev;
    struct SlideNode *cur;

    if (SlideNodeList == node) {
        SlideNodeList = node->next;
    } else {
        cur = SlideNodeList->next;
        prev = SlideNodeList;
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

// FUNCTION: LEGOLAND 0x00416f60
struct SlideNode *FindSlideNode(void *arg) {
    struct SlideNode *cur = SlideNodeList;

    if (cur != NULL) {
        do {
            if (memcmp(&cur->key, arg, 2) == 0) {
                return cur;
            }
            cur = cur->next;
        } while (cur != NULL);
    }
    return NULL;
}

/* Sets bit 0 of var_c. */
// FUNCTION: LEGOLAND 0x00416f90
void SetTempleRideFlag1(struct TempleRide *arg) {
    arg->var_c |= 1;
}

// FUNCTION: LEGOLAND 0x00416fa0
void RenderTempleSlide(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = obj->ride;
    struct RideNode *node;
    struct Point pos;
    struct Point off1;

    FindSlideNode(tile);
    pos = GetScreenCoordsForObject((TileId *)tile, ride);
    for (node = ride->riders; node != NULL; node = node->next) {
        if (*tile == node->tile.id && (node->rider->flags & 0x80) == 0) {
            IP_RenderBlokeIn3DNow(node->rider);
        }
    }
    PrintSprite(TempSlideMatteSprite, pos.x, pos.y, clip, 0);
    RenderItems_New();
    TempleSlideRiderRenderList = NULL;
    for (node = ride->riders; node != NULL; node = node->next) {
        if (*tile == node->tile.id && (node->rider->flags & 0x80) != 0) {
            struct Bloke *bloke = node->rider;
            struct Person *person;

            struct Point off2;

            off1 = TempleSlideDrawOffset;
            AdjustOffsetForViewMode(&off1);
            off2.x = TempleSlideDrawOffset2X;
            off2.y = TempleSlideDrawOffset2Y;
            AdjustOffsetForViewMode(&off2);
            person = bloke->person;
            person->offset.x = bloke->screen_x;
            person->offset.y = bloke->screen_y;
            person->offset.x += off2.x;
            person->offset.y += off2.y;
            AdjustBlokePosition(&person->offset);
            person->screen.x = bloke->screen_x + off1.x + off2.x + pos.x;
            person->screen.y = bloke->screen_y + off1.y + off2.y + pos.y;
            AdjustBlokePosition(&person->screen);
            AddBlokeToRenderList(&TempleSlideRiderRenderList, node, node->person->field_20);
        }
    }
    RenderBlokeList((struct RenderItemNode **)&TempleSlideRiderRenderList);
}

/* Clears var_4 and bit 0 of var_c. */
// FUNCTION: LEGOLAND 0x00417130
void ClearTempleRideVar4AndFlag1(struct TempleRide *arg) {
    arg->var_4 = 0;
    arg->var_c &= 0xfffffffe;
}

// FUNCTION: LEGOLAND 0x00417150
void TempleSlideLoad(struct Element *arg) {
    ActiveTempleSlideRide = arg->ride;
    ActiveTempleSlideRide->flags |= 0x20;
    TempleSlideLayer = ActiveTempleSlideRide->layer;
    TempleSlideLayer->flags |= 0x2000;
    ZTempSlideSprite = LoadSprite(
        // STRING: LEGOLAND 0x004b4f7c
        "z_tempslide.lls", 1);
    TempSlideBinV = LoadBinV(
        // STRING: LEGOLAND 0x004b4f64
        "Zbuffers\\tempslide.bnv");
    GetLLSForSprite((struct Sprite *)ActiveTempleSlideRide->layer);
    TempleSlideDrawOffset.x = 0;
    TempleSlideDrawOffset.y = 1;
    TempleSlideDrawOffset2X = 13;
    TempleSlideDrawOffset2Y = 93;
    TempSlideMatteSprite = LoadSprite(
        // STRING: LEGOLAND 0x004b4f50
        "tempslide_matte.lls", 1);
    TempleSlideBinVs[0] = TempSlideBinV;
}

// FUNCTION: LEGOLAND 0x00417200
void TempleSlideUnload(struct Element *arg) {
    ActiveTempleSlideRide = arg->ride;
    if (TempSlideMatteSprite != 0) {
        KillSprite(TempSlideMatteSprite);
    }
    KillSprite(ZTempSlideSprite);
    FreeBinV(TempSlideBinV);
}

// FUNCTION: LEGOLAND 0x00417240
void TempleSlideSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = (void *)ActiveTempleSlideRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00417280
void RemoveSlideObject(struct Element *obj, TileId tile, struct Cursor *cursor) {
    struct SlideNode *node = FindSlideNode(&tile);
    if (node != NULL) {
        RemoveSlideNode(node);
    }
    StandardRemoveObject(obj, tile, cursor);
    RemoveAllBlokesFromRide((struct Ride *)obj->ride, tile);
}

// FUNCTION: LEGOLAND 0x004172d0
void TempleSlideAddObject(Element *obj, int *coords) {
    TileId tile;
    tile.pos.x = (unsigned char)coords[0];
    tile.pos.y = (unsigned char)coords[1];
    AddBasicObject(obj, coords);
    AddSlideNode(&tile);
}

// FUNCTION: LEGOLAND 0x00417300
struct RideSpriteInfo *GetTempleSlideSpriteInfo(struct Element *ctx, unsigned short param) {
    struct Ride *track = ctx->ride;
    struct Sprite *car = track->layer;

    TempleSlideSpriteInfo.sprite = car;
    TempleSlideSpriteInfo.x = track->field_14;
    TempleSlideSpriteInfo.y = track->field_18;
    TempleSlideSpriteInfo.id = param;

    car = track->layer;
    car->flags |= 0x2000;

    return &TempleSlideSpriteInfo;
}

// FUNCTION: LEGOLAND 0x00417340
void UpdateSlideAvailability(void *arg) {
    unsigned int *array = FindSlideNode(arg)->slots;
    int i = 0;

    while (i < 4) {
        if (array[i] == 0) {
            Ride_ClearFlagToNotLetAnyoneOn(arg);
            return;
        }
        i++;
    }
    Ride_SetFlagToNotLetAnyoneOn(arg);
}

/* Reserves the first free entrance lane among slots zero and three, updates ride availability, and returns its
 * index. Falls back to slot zero when both are busy; returns minus one if the slide is absent. */
// FUNCTION: LEGOLAND 0x00417380
int ReserveTempleSlideLane(void *arg) {
    int avail[4];
    int count;
    int pick;
    struct SlideNode *node = FindSlideNode(arg);

    if (node != NULL) {
        count = 0;
        if (node->slots[0] == 0) {
            avail[count] = count;
            count = 1;
        }
        if (node->slots[3] == 0) {
            avail[count++] = 3;
        }
        if (count != 0) {
            pick = avail[0];
            node->slots[pick] = 1;
            UpdateSlideAvailability(arg);
            return pick;
        }
        node->slots[0] = 1;
        UpdateSlideAvailability(arg);
        return 0;
    }
    return -1;
}

// FUNCTION: LEGOLAND 0x00417400
void ReleaseSlideSlot(unsigned int index, void *arg) {
    struct SlideNode *result = FindSlideNode(arg);

    if (result != NULL) {
        result->slots[index] = 0;
    }
    UpdateSlideAvailability(arg);
}

// FUNCTION: LEGOLAND 0x00417430
void TempleSlideUpdate(Element *obj) {
    Ride *ride;
    Bloke *bloke;
    TileId *tile;
    int x;
    int y;
    unsigned char dir;
    int f;
    short sx;
    short sy;
    int off[4];
    Point coords;
    Point d;
    RideNode *next;
    int th;
    int tw;
    RideNode *node;

    ride = obj->ride;
    node = ride->riders;
    while (node != NULL) {
        next = node->next;
        tile = &node->tile;
        bloke = node->rider;
        x = ride->x + tile->pos.x;
        y = tile->pos.y + ride->y;
        if (bloke->low_level_action == 0) {
            switch (bloke->param_action) {
            case 0:
                bloke->flags |= 8;
                bloke->param_action++;
                bloke->field_36 = ReserveTempleSlideLane(tile);
                if (bloke->field_36 > 1) {
                    x = (x << 8) + 0x380;
                    y = (y << 8) - 0x280;
                    bloke->dest.x = x;
                    bloke->dest.y = y;
                    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->low_level_action = 7;
                    bloke->target_dir = dir + 0x10;
                    NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                    bloke->param_action++;
                } else {
                    y -= 5;
                    x = (x << 8) + 0x80;
                    y <<= 8;
                    bloke->dest.x = x;
                    bloke->dest.y = y;
                    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                    bloke->low_level_action = 7;
                    bloke->target_dir = dir + 0x10;
                    NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                    bloke->param_action = 3;
                }
                break;
            case 2:
                bloke->dest.x = (ride->footprint.x0 + tile->pos.x + 4) << 8;
                bloke->dest.y = (ride->footprint.y0 + tile->pos.y + 2) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 3:
                coords = GetScreenCoordsForObject(tile, ride);
                {
                    int py = bloke->pos.y;
                    int px = bloke->pos.x;
                    GetTileDimensions(&tw, &th);
                    y = (px + py) * th >> 9;
                    x = (px - py) * tw >> 9;
                }
                sx = Get_XScroll();
                x += lpConfig->view_x - sx;
                sy = Get_YScroll();
                y += lpConfig->view_y - sy;
                x = x - TempleSlideDrawOffset.x / 2 - coords.x;
                bloke->flags |= 0x80;
                y = y - TempleSlideDrawOffset.y / 2 - coords.y;
                off[0] = x * 2;
                off[1] = y * 2;
                bloke->person->sprite = ZTempSlideSprite;
                bloke->person->field_30 = 1;
                bloke->person->depth = GetUnitDepth(-1617664.875f, -1617913.0f);
                bloke->field_35 = 0;
                bloke->path = NewBNVPath(TempleSlideBinVs[0], 0, TempleSlidePathNames[bloke->field_36], -1617664.875f, -1617913.0f, off);
                BNVPath_SetDFrame(bloke, bloke->path, 0);
                UpdateBlokeFromBNVPath(bloke, bloke->path);
                bloke->param_action++;
                break;
            case 4:
                if (UpdateBlokeFromBNVPath(bloke, bloke->path) == 0) {
                    bloke->field_35 = 1;
                    bloke->param_action = 5;
                    free(bloke->path);
                    bloke->path = NULL;
                }
                BlokeSetFrame(bloke, bloke->frame);
                if (bloke->path != NULL) {
                    f = BNVPath_GetDFrame(bloke->path);
                    if (f == TempleSlideStartFrames[bloke->field_36]) {
                        BlokeWalkAnim(bloke);
                        bloke->field_44 = bloke->speed;
                        bloke->speed = 0x18;
                    } else if (f == TempleSlideEndFrames[bloke->field_36]) {
                        bloke->field_35 = 1;
                        bloke->param_action = 5;
                        free(bloke->path);
                        bloke->path = NULL;
                    } else if (f >= TempleSlideStartFrames[bloke->field_36] && f <= (int)TempleSlideEndFrames[bloke->field_36]) {
                        BlokeSetFrame(bloke, 0);
                    }
                }
                break;
            case 5:
                BlokeWalkAnim(bloke);
                BlokeSetFrame(bloke, 0);
                bloke->flags &= 0xff7f;
                bloke->person->sprite = NULL;
                bloke->person->field_30 = 0;
                bloke->speed = (unsigned char)bloke->field_44;
                switch (bloke->field_36) {
                case 0:
                    d.x = -0x280;
                    d.y = 0x700;
                    break;
                case 1:
                    d.x = -0x180;
                    d.y = 0x180;
                    break;
                case 2:
                    d.x = -0x180;
                    d.y = 0;
                    break;
                case 3:
                    d.x = -0x280;
                    d.y = -0x500;
                    break;
                }
                x = ride->field_24 + tile->pos.x;
                y = ride->field_25 + tile->pos.y;
                bloke->pos.x = d.x + (x << 8);
                bloke->pos.y = d.y + (y << 8);
                bloke->dest.x = (x << 8) + 0x80;
                bloke->dest.y = (y << 8) + 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = 0x10 + dir;
                NewDirForAction(bloke, (bloke->target_dir >> 5) + 3);
                bloke->param_action++;
                break;
            case 6:
                ReleaseSlideSlot(bloke->field_36, tile);
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= 0xfff7;
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x004178c0
LEGO_EXPORT int SaveTempleSlide(struct Element *element) {
    int one = 1;
    int zero = 0;
    struct SlideNode *p;

    p = SlideNodeList;
    while (p != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(p, sizeof(struct SlideNode)) == 0) {
            return 0;
        }
        p = p->next;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00417930
LEGO_EXPORT int LoadTempleSlide(struct Element *obj) {
    struct Ride *ride = obj->ride;
    struct SlideNode *last = NULL;
    struct RideNode *node;
    unsigned int marker;

    if (!SaveGameRead(&marker, 4)) {
        return 0;
    }

    while (marker != 0) {
        struct SlideNode *save = malloc(sizeof(struct SlideNode));
        if (!SaveGameRead(save, sizeof(struct SlideNode))) {
            return 0;
        }
        save->next = NULL;
        if (last != NULL) {
            last->next = save;
        } else {
            SlideNodeList = save;
        }
        last = save;
        if (!SaveGameRead(&marker, 4)) {
            return 0;
        }
    }

    for (node = ride->riders; node != NULL; node = node->next) {
        struct Person *render = node->person;
        struct Bloke *bloke;
        struct BNVRef *path;

        if (render->field_30 != 0) {
            /* The original reads [field_30 * 4 + 0x4cbfcc] (just past TempleSlideDrawOffset.y). field_30 is
             * only ever 1 here (the rider is on the slide), which reads ZTempSlideSprite. */
            render->sprite = (&ZTempSlideSprite)[render->field_30 - 1];
        } else {
            render->field_2c = 0;
            node->person->field_30 = 0;
        }

        bloke = node->rider;
        path = bloke->bnv;
        if (path != NULL) {
            path->file = TempleSlideBinVs[path->index];
        }
    }

    return 1;
}

// FUNCTION: LEGOLAND 0x00417a00
LEGO_EXPORT void TempleSlide_GetInterfaces(struct Element *ctx, struct Ride *interfaces) {
    // STRING: LEGOLAND 0x004b4f8c
    if (_stricmp("TEMPLE SLIDE", ctx->name) == 0) {
        interfaces->cb_a4 = TempleSlideLoad;
        interfaces->cb_ac = TempleSlideUnload;
        interfaces->cb_8c = TempleSlideSetEditMode;
        interfaces->cb_pre_render = TempleSlideUpdate;
        interfaces->cb_b0 = RenderTempleSlide;
        interfaces->cb_9c = RemoveSlideObject;
        interfaces->cb_98 = TempleSlideAddObject;
        interfaces->cb_sprite = GetTempleSlideSpriteInfo;
        interfaces->save_hook = SaveTempleSlide;
        interfaces->load_hook = LoadTempleSlide;
    }
}
