#include <stdlib.h>
#include <string.h>
#include "legoland.h"

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
#include "sound_music.h"
#include "space_tower.h"

struct RideNode;

struct AnimStep {
    /* 0x00 */ int x;
    /* 0x04 */ int y;
    /* 0x08 */ int dx;
    /* 0x0c */ int dy;
};

struct ListNode {
    unsigned char pad_0[8];
    struct ListNode *next;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x0043a7a0
struct Point GetAnimLayoutOffset(struct AnimLayout *layout, int seg, int step) {
    struct Point sum;
    struct AnimEntry *entry;
    struct AnimEntry **pp;
    struct AnimStep *p;
    int i;
    int n;

    sum.x = 0;
    sum.y = 0;
    pp = layout->entries;
    for (i = 0; i < seg + 1; i++) {
        entry = *pp;
        if (i != seg) {
            n = entry->count;
        } else {
            n = step + 1;
        }
        if (n > 0) {
            p = entry->steps;
            do {
                sum.x += p->dx + (p->x << 8);
                sum.y += (p->y << 8) + p->dy;
                p++;
            } while (--n);
        }
        pp++;
    }
    return sum;
}

/* Builds the next rider destination from the tower animation layout and ride origin, starts movement, and
 * increments the layout waypoint index. */
// FUNCTION: LEGOLAND 0x0043a820
void MoveSpaceTowerRiderToNextWaypoint(struct AnimEntry *param_1, struct RideNode *param_2) {
    struct Bloke *bloke;
    struct Ride *ride;
    struct Point base;
    char dir;

    bloke = param_2->rider;
    base = GetAnimLayoutOffset((struct AnimLayout *)SpaceTowerLayouts[bloke->field_50].anim_layout, bloke->field_4a, bloke->field_38);
    ride = SpaceTowerRideObj;
    base.x += (param_2->tile.pos.x + ride->x) << 8;
    base.y += (param_2->tile.pos.y + ride->y) << 8;
    bloke->dest.x = base.x;
    bloke->dest.y = base.y;
    dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
    bloke->low_level_action = 7;
    bloke->target_dir = dir + 0x10;
    NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
    param_2->rider->field_38++;
}

/* Advances the rider through the current layout entry and its waypoints, advancing the action when the selected
 * entry range is complete. */
// FUNCTION: LEGOLAND 0x0043a8c0
void AdvanceSpaceTowerRiderLayout(struct RideNode *param_1) {
    struct Bloke *bloke;
    struct AnimLayout *layout;
    struct AnimEntry *entry;

    bloke = param_1->rider;
    layout = (struct AnimLayout *)SpaceTowerLayouts[bloke->field_50].anim_layout;
    entry = layout->entries[bloke->field_4a];
    if ((int)bloke->field_4a >= layout->count) {
        bloke->param_action = bloke->param_action + '\x01';
        return;
    }
    if ((int)bloke->field_38 >= entry->count) {
        bloke->field_38 = 0;
        param_1->rider->field_4a = param_1->rider->field_4a + 1;
        bloke = param_1->rider;
        if (bloke->field_4a == (short)bloke->field_4c || (int)bloke->field_4a == layout->count) {
            bloke->param_action = bloke->param_action + '\x01';
            return;
        }
    }
    if ((int)bloke->field_38 != entry->count) {
        MoveSpaceTowerRiderToNextWaypoint(entry, param_1);
    }
}

// FUNCTION: LEGOLAND 0x0043a940
void UpdateSpaceTowerSeat(struct SpaceTowerSeat *seat) {
    if (seat->flags & 1) {
        switch (seat->state) {
        case 1:
            if (seat->field_10 == 0) {
                seat->field_10 = -1;
            } else {
                seat->state = 2;
            }
            break;
        case 2:
            if (seat->descending == 0) {
                seat->pos += seat->delta;
                if (seat->pos > 200) {
                    seat->pos = 200;
                    seat->descending = 1;
                }
            } else {
                seat->pos -= 2;
                if (seat->pos < 0) {
                    seat->pos = 0;
                    seat->descending = 0;
                    seat->state = 0;
                }
            }
            break;
        }
    }
}

/* Clears all four seat states, then activates each seat with at least one occupied rider slot and resets its motion state. */
// FUNCTION: LEGOLAND 0x0043a9b0
void SyncSpaceTowerSeatsFromOccupancy(struct SpaceTowerCar *arg) {
    int i;

    for (i = 0; i < 4; i++) {
        arg->seats[i].flags &= ~1;
        arg->seats[i].state = 0;
    }
    for (i = 0; i < 8; i++) {
        if (arg->flags_a4[i] != 0) {
            arg->seats[i >> 1].flags |= 1;
            arg->seats[i >> 1].state = 2;
            arg->seats[i >> 1].descending = 0;
            arg->seats[i >> 1].pos = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x0043aa10
void PlaySpaceTowerSfx(unsigned char *arg) {
    unsigned int buffer[4];

    buffer[0] = 2;
    buffer[2] = arg[0];
    buffer[3] = arg[1];
    PlayInstanceOfSample(SPACE_TOWER_SFX[0].sample, 1, 1, buffer);
}

// FUNCTION: LEGOLAND 0x0043aa50
void FadeSpaceTowerSfx(unsigned char *arg) {
    struct SampleSource params;
    params.type = 2;
    params.x = arg[0];
    params.y = arg[1];
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
}

// FUNCTION: LEGOLAND 0x0043aa90
void StartSpaceTowerCar(struct SpaceTowerCar *arg) {
    unsigned char tmp = arg->var_2;
    arg->var_c |= 1;
    arg->var_3 = tmp;
    arg->var_10 = 0;
    SyncSpaceTowerSeatsFromOccupancy(arg);
    PlaySpaceTowerSfx((unsigned char *)arg);
}

// FUNCTION: LEGOLAND 0x0043aac0
void ResetSpaceTowerCar(struct SpaceTowerCar *arg) {
    arg->var_c &= 0xffffbffe;
    arg->seats[0].pos = 0;
    arg->seats[0].flags &= 0xfffffffe;
    arg->seats[1].pos = 0;
    arg->seats[1].flags &= 0xfffffffe;
    arg->seats[2].pos = 0;
    arg->seats[3].pos = 0;
    arg->seats[2].flags &= 0xfffffffe;
    arg->seats[3].flags &= 0xfffffffe;
    arg->seats[0].delta = rand() % 3 + 7;
    arg->seats[1].delta = rand() % 3 + 7;
    arg->seats[2].delta = rand() % 3 + 7;
    arg->seats[3].delta = rand() % 3 + 7;
    arg->var_4 = 0;
    arg->var_2 = 0;
    FadeSpaceTowerSfx((unsigned char *)arg);
}

// FUNCTION: LEGOLAND 0x0043ab70
void AddSpaceTowerCar(unsigned short *param_1) {
    struct SpaceTowerCar *node;
    unsigned int *fill;
    int i;

    node = (struct SpaceTowerCar *)malloc(sizeof(struct SpaceTowerCar));
    if (node != NULL) {
        fill = (unsigned int *)node;
        for (i = 0x2d; i != 0; i = i + -1) {
            *fill = 0;
            fill = fill + 1;
        }
        node->var_0 = *param_1;
        node->next = SpaceTowerCarList;
        SpaceTowerCarList = node;
        ResetSpaceTowerCar(node);
    }
}

// FUNCTION: LEGOLAND 0x0043abc0
void RemoveSpaceTowerCar(struct SpaceTowerCar *arg) {
    struct SpaceTowerCar *next;
    struct SpaceTowerCar *prev;

    if (SpaceTowerCarList == arg) {
        SpaceTowerCarList = arg->next;
    } else {
        next = SpaceTowerCarList->next;
        prev = SpaceTowerCarList;
        while (next != arg) {
            prev = prev->next;
            if (prev == NULL) {
                break;
            }
            next = prev->next;
        }
        if (prev != NULL) {
            prev->next = arg->next;
        }
    }
    free(arg);
}

// FUNCTION: LEGOLAND 0x0043ac20
void FreeAllSpaceTowerCars(void) {
    while (SpaceTowerCarList != NULL) {
        RemoveSpaceTowerCar(SpaceTowerCarList);
    }
}

// FUNCTION: LEGOLAND 0x0043ac40
struct SpaceTowerCar *FindSpaceTowerCar(unsigned short *param_1) {
    struct SpaceTowerCar *node;

    node = SpaceTowerCarList;
    if (node == NULL) {
        return NULL;
    }
    while (memcmp(node, param_1, 2) != 0) {
        node = node->next;
        if (node == NULL) {
            return NULL;
        }
    }
    return node;
}

// FUNCTION: LEGOLAND 0x0043ac70
void AssignSpaceTowerSlot(struct RideNode *param_1, unsigned short *param_2) {
    struct SpaceTowerCar *ride;
    unsigned int slot;

    ride = FindSpaceTowerCar(param_2);
    if (ride != NULL) {
        slot = TakeRandomSpaceTowerSlot(param_1, ride);
        param_1->rider->field_50 = slot;
        param_1->rider->field_4c = SpaceTowerLayouts[slot].field_0;
    }
}

// FUNCTION: LEGOLAND 0x0043acb0
unsigned int TakeRandomSpaceTowerSlot(struct RideNode *param_1, struct SpaceTowerCar *param_2) {
    int slot;

    slot = rand() % 8;
    while (param_2->flags_a4[slot] != 0) {
        slot++;
        if (slot >= 8) {
            slot = 0;
        }
    }
    param_2->flags_a4[slot] = 1;
    param_1->rider->field_36 = (char)slot;
    return slot;
}

/* Returns the projected screen Y of a tower seat base, offsetting the ride tile by two tiles according to the seat
 * index and subtracting vertical scroll. */
// FUNCTION: LEGOLAND 0x0043ad00
int GetSpaceTowerSeatScreenY(unsigned char *param_1, int param_2) {
    int y;
    int x;
    struct Point p;
    int w;
    int h;

    x = param_1[0];
    y = param_1[1];
    switch (param_2) {
    case 0:
    case 3:
        x += 2;
        y += 2;
        break;
    case 1:
    case 2:
        x -= 2;
        y -= 2;
        break;
    }
    p.y = y << 8;
    p.x = x << 8;
    GetTileDimensions(&w, &h);
    p.y = ((p.y + p.x) * h) >> 9;
    Get_XScroll();
    return lpConfig->view_y - (short)Get_YScroll() + p.y;
}

// FUNCTION: LEGOLAND 0x0043ad90
void DrawSpaceTowerSeatMatte(struct SpaceTowerCar *param_1, int param_2, unsigned int param_3) {
    struct Point coords;
    struct Point off;
    struct Point pos;

    coords = GetScreenCoordsForObject((TileId *)param_1, SpaceTowerRideObj);
    if (SpaceTowerSeatMatteSprites[param_2] != NULL) {
        off = SpaceTowerLayerOffsets[param_2];
        off.y -= param_1->seats[param_2].pos;
        AdjustOffsetForViewMode(&off);
        pos.x = coords.x + off.x;
        pos.y = coords.y + off.y;
        PrintSprite(SpaceTowerSeatMatteSprites[param_2], pos.x, pos.y, param_3, 0);
    }
}

// FUNCTION: LEGOLAND 0x0043ae20
void DrawSpaceTowerSeatSprite(struct SpaceTowerCar *param_1, int param_2, unsigned int param_3) {
    struct Point coords;
    struct Point off;
    int id;

    coords = GetScreenCoordsForObject((TileId *)param_1, SpaceTowerRideObj);
    off = SpaceTowerLayerOffsets[param_2];
    off.y -= param_1->seats[param_2].pos;
    AdjustOffsetForViewMode(&off);
    switch (param_2) {
    case 0:
        id = 6;
        break;
    case 1:
        id = 4;
        break;
    case 2:
        id = 0;
        break;
    case 3:
        id = 2;
        break;
    }
    PrintSprite(GetSpriteForLayer(SpaceTowerLayers, id), coords.x + off.x, coords.y + off.y, param_3, 0);
}

// FUNCTION: LEGOLAND 0x0043aee0
void RenderSpaceTowerSeat(struct SpaceTowerCar *param_1, int param_2, unsigned int param_3) {
    DrawSpaceTowerSeatSprite(param_1, param_2, param_3);
    if ((param_1->seats[param_2].flags & 1) != 0) {
        GetSpaceTowerSeatScreenY((unsigned char *)param_1, param_2);
        if (param_1->seats[param_2].rider_slot0 != NULL) {
            IP_RenderBlokeIn3DNow(param_1->seats[param_2].rider_slot0->rider);
        }
        if (param_1->seats[param_2].rider_slot1 != NULL) {
            IP_RenderBlokeIn3DNow(param_1->seats[param_2].rider_slot1->rider);
        }
        DrawSpaceTowerSeatMatte(param_1, param_2, param_3);
    }
}

// FUNCTION: LEGOLAND 0x0043af50
void RenderSpaceTower(struct Element *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, void *param_5, unsigned int param_6) {
    struct Ride *ride;
    struct RideNode *node;
    struct SpaceTowerCar *car;
    struct Bloke *bloke;
    struct Point coords;
    struct Point off1;
    struct Point off2;

    ride = param_1->ride;
    node = ride->riders;
    car = FindSpaceTowerCar(param_4);
    if (car == NULL) {
        return;
    }
    coords = GetScreenCoordsForObject((TileId *)param_4, ride);
    off1 = GetRenderOffsetForLayer(SpaceTowerLayers, 1);
    AdjustOffsetForViewMode(&off1);
    if (node != NULL) {
        for (; node != NULL; node = node->next) {
            if (memcmp(&node->tile.id, param_4, 2) == 0) {
                bloke = node->rider;
                if ((bloke->flags & 0x80) == 0 && SpaceTowerLayouts[bloke->field_50].anim_layout == &SpaceTowerBackSeatLayout) {
                    IP_RenderBlokeIn3DNow(bloke);
                }
            }
        }
        RenderSpaceTowerSeat(car, 1, param_6);
        RenderSpaceTowerSeat(car, 2, param_6);
        PrintSprite(SpaceTowerMatte2Sprite, coords.x + off1.x, coords.y + off1.y, param_6, 0);
        RenderSpaceTowerSeat(car, 0, param_6);
        RenderSpaceTowerSeat(car, 3, param_6);
        node = ride->riders;
        for (; node != NULL; node = node->next) {
            if (memcmp(&node->tile.id, param_4, 2) == 0) {
                bloke = node->rider;
                if ((bloke->flags & 0x80) == 0 && SpaceTowerLayouts[bloke->field_50].anim_layout == &SpaceTowerFrontSeatLayout) {
                    IP_RenderBlokeIn3DNow(bloke);
                }
            }
        }
        PrintSprite(SpaceTowerMatte1Sprite, coords.x + off1.x, coords.y + off1.y, param_6, 0);
        LLSSetFrame(GetLLSForLayer(SpaceTowerLayers, 5), (char)car->var_ad);
        off2 = GetRenderOffsetForLayer(SpaceTowerLayers, 5);
        AdjustOffsetForViewMode(&off2);
        PrintSprite(GetSpriteForLayer(SpaceTowerLayers, 5), off2.x + coords.x, off2.y + coords.y, param_6, 0);
        LLSSetFrame(GetLLSForLayer(SpaceTowerLayers, 3), (char)car->var_ac);
        off2 = GetRenderOffsetForLayer(SpaceTowerLayers, 3);
        AdjustOffsetForViewMode(&off2);
        PrintSprite(GetSpriteForLayer(SpaceTowerLayers, 3), off2.x + coords.x, off2.y + coords.y, param_6, 0);
    } else {
        RenderSpaceTowerSeat(car, 1, param_6);
        RenderSpaceTowerSeat(car, 2, param_6);
        PrintSprite(SpaceTowerMatte2Sprite, coords.x + off1.x, coords.y + off1.y, param_6, 0);
        RenderSpaceTowerSeat(car, 0, param_6);
        RenderSpaceTowerSeat(car, 3, param_6);
        LLSSetFrame(GetLLSForLayer(SpaceTowerLayers, 5), (char)car->var_ad);
        off2 = GetRenderOffsetForLayer(SpaceTowerLayers, 5);
        AdjustOffsetForViewMode(&off2);
        PrintSprite(GetSpriteForLayer(SpaceTowerLayers, 5), off2.x + coords.x, off2.y + coords.y, param_6, 0);
        LLSSetFrame(GetLLSForLayer(SpaceTowerLayers, 3), (char)car->var_ac);
        off2 = GetRenderOffsetForLayer(SpaceTowerLayers, 3);
        AdjustOffsetForViewMode(&off2);
        PrintSprite(GetSpriteForLayer(SpaceTowerLayers, 3), off2.x + coords.x, off2.y + coords.y, param_6, 0);
    }
}

// FUNCTION: LEGOLAND 0x0043b2b0
void SpaceTowerRideLoad(struct Element *param_1) {
    SpaceTowerRideObj = param_1->ride;
    SpaceTowerRideObj->flags |= 0x420;
    SpaceTowerLayers = SpaceTowerRideObj->layer;
    SpaceTowerLayers->flags |= 0x2000;
    HideLayer(SpaceTowerLayers, 5);
    StopLayerPlaying(SpaceTowerLayers, 5);
    LLSSetFrame(GetLLSForLayer(SpaceTowerLayers, 5), 0);
    HideLayer(SpaceTowerLayers, 3);
    StopLayerPlaying(SpaceTowerLayers, 3);
    LLSSetFrame(GetLLSForLayer(SpaceTowerLayers, 3), 0);
    SpaceTowerLayerOffsets[0] = GetRenderOffsetForLayer(SpaceTowerLayers, 6);
    SpaceTowerLayerOffsets[1] = GetRenderOffsetForLayer(SpaceTowerLayers, 4);
    SpaceTowerLayerOffsets[2] = GetRenderOffsetForLayer(SpaceTowerLayers, 0);
    SpaceTowerLayerOffsets[3] = GetRenderOffsetForLayer(SpaceTowerLayers, 2);
    SpaceTowerSeatMatteSprites[0] = NULL;
    // STRING: LEGOLAND 0x004b7880
    SpaceTowerSeatMatteSprites[1] = LoadSprite("SpaceTower Seat2 Matte.lls", 1);
    // STRING: LEGOLAND 0x004b7864
    SpaceTowerSeatMatteSprites[2] = LoadSprite("SpaceTower Seat3 Matte.lls", 1);
    SpaceTowerSeatMatteSprites[3] = NULL;
    // STRING: LEGOLAND 0x004b7850
    SpaceTowerMatte1Sprite = LoadSprite("Spacet Matte1.lls", 1);
    // STRING: LEGOLAND 0x004b783c
    SpaceTowerMatte2Sprite = LoadSprite("Spacet Matte2.lls", 1);
    Load_FXList(SPACE_TOWER_SFX, 1);
}

// FUNCTION: LEGOLAND 0x0043b420
void SpaceTowerSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = SpaceTowerRideObj;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x0043b460
void SpaceTowerRemoveObject(struct Element *param_1, TileId tile, struct Cursor *param_3) {
    struct SpaceTowerCar *node;

    node = FindSpaceTowerCar(&tile.id);
    if (node != NULL) {
        RemoveSpaceTowerCar(node);
    }
    StandardRemoveObject(param_1, tile, param_3);
    RemoveAllBlokesFromRide(param_1->ride, tile);
    FadeSpaceTowerSfx(&tile.pos.x);
}

// FUNCTION: LEGOLAND 0x0043b4b0
void SpaceTowerAddObject(struct Element *param_1, int *coords) {
    TileId id;

    id.pos.x = (unsigned char)coords[0];
    id.pos.y = (unsigned char)coords[1];
    AddBasicObject(param_1, coords);
    AddSpaceTowerCar(&id.id);
}

// FUNCTION: LEGOLAND 0x0043b4e0
struct RideSpriteInfo *GetSpaceTowerRideSpriteInfo(struct Element *param_1, unsigned short param_2) {
    struct Ride *ride;

    ride = param_1->ride;
    SpaceTowerSpriteInfo.sprite = ride->layer;
    SpaceTowerSpriteInfo.x = ride->field_14;
    SpaceTowerSpriteInfo.y = ride->field_18;
    SpaceTowerSpriteInfo.id = param_2;
    ride->layer->flags |= 0x2000;
    HideLayer(SpaceTowerLayers, 6);
    HideLayer(SpaceTowerLayers, 4);
    HideLayer(SpaceTowerLayers, 0);
    HideLayer(SpaceTowerLayers, 2);
    HideLayer(SpaceTowerLayers, 5);
    return &SpaceTowerSpriteInfo;
}

// FUNCTION: LEGOLAND 0x0043b570
void SpaceTowerRideUnload(void) {
    KillSprite(SpaceTowerMatte1Sprite);
    KillSprite(SpaceTowerMatte2Sprite);
    KillSprite(SpaceTowerSeatMatteSprites[1]);
    KillSprite(SpaceTowerSeatMatteSprites[2]);
    FreeAllSpaceTowerCars();
    Kill_FXList(SPACE_TOWER_SFX, 1);
    SpaceTowerRideObj->riders = NULL;
}

// FUNCTION: LEGOLAND 0x0043b5d0
LEGO_EXPORT int SpaceTower_Save(struct Element *element) {
    struct SpaceTowerCar *car;
    struct RideNode **field;
    struct RideNode *cur;
    int index;
    int i;
    unsigned int one;
    unsigned int zero;

    car = SpaceTowerCarList;
    one = 1;
    zero = 0;
    if (SpaceTowerCarList != NULL) {
        while (car != NULL) {
            if (SaveGameWrite(&one, 4) == 0) {
                return 0;
            }
            for (i = 0; i < 8; i++) {
                if (i & 1) {
                    field = &car->seats[i >> 1].rider_slot1;
                } else {
                    field = &car->seats[i >> 1].rider_slot0;
                }
                index = 0;
                for (cur = SpaceTowerRideObj->riders; cur != NULL; cur = cur->next) {
                    if (cur == *field) {
                        break;
                    }
                    index++;
                }
                if (cur != NULL) {
                    *field = (struct RideNode *)(INT_PTR)(index + 1);
                } else {
                    *field = NULL;
                }
            }
            if (SaveGameWrite(car, sizeof(struct SpaceTowerCar)) == 0) {
                return 0;
            }
            car = car->next;
        }
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x0043b6a0
LEGO_EXPORT int SpaceTower_Load(struct Element *element) {
    struct SpaceTowerCar *node;
    struct SpaceTowerCar *prev;
    struct RideNode **field;
    struct RideNode *cur;
    int flag;
    int n;
    int i;

    prev = NULL;
    if (SaveGameRead(&flag, 4) == 0) {
        return 0;
    }
    while (flag != 0) {
        node = (struct SpaceTowerCar *)malloc(sizeof(struct SpaceTowerCar));
        if (SaveGameRead(node, sizeof(struct SpaceTowerCar)) == 0) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            SpaceTowerCarList = node;
        }
        prev = node;
        for (i = 0; i < 8; i++) {
            if (i & 1) {
                field = &node->seats[i >> 1].rider_slot1;
            } else {
                field = &node->seats[i >> 1].rider_slot0;
            }
            n = (int)(INT_PTR)*field;
            cur = SpaceTowerRideObj->riders;
            if (n != 0) {
                while (--n != 0) {
                    cur = cur->next;
                }
                *field = cur;
            } else {
                *field = NULL;
            }
        }
        if (SaveGameRead(&flag, 4) == 0) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0043b780
void SpaceTowerRide(struct Element *name, struct Ride *obj) {
    // STRING: LEGOLAND 0x004b789c
    if (_stricmp("SPACE TOWER RIDE", name->name) == 0) {
        obj->cb_a4 = SpaceTowerRideLoad;
        obj->cb_8c = SpaceTowerSetEditMode;
        obj->cb_pre_render = SpaceTowerUpdate;
        obj->cb_sprite = GetSpaceTowerRideSpriteInfo;
        obj->cb_b0 = RenderSpaceTower;
        obj->cb_9c = SpaceTowerRemoveObject;
        obj->cb_98 = SpaceTowerAddObject;
        obj->cb_ac = SpaceTowerRideUnload;
        obj->save_hook = SpaceTower_Save;
        obj->load_hook = SpaceTower_Load;
    }
}

// FUNCTION: LEGOLAND 0x0043b810
void AssignSpaceTowerSeatRiders(struct SpaceTowerCar *param_1) {
    struct RideNode *node;
    int slot;
    int idx;
    int odd;
    struct Point coords;
    struct Point off;
    struct Point dat;
    struct Point pos;
    struct Person *person;

    node = SpaceTowerRideObj->riders;
    param_1->seats[0].rider_slot0 = NULL;
    param_1->seats[0].rider_slot1 = NULL;
    param_1->seats[1].rider_slot0 = NULL;
    param_1->seats[1].rider_slot1 = NULL;
    param_1->seats[2].rider_slot0 = NULL;
    param_1->seats[2].rider_slot1 = NULL;
    param_1->seats[3].rider_slot0 = NULL;
    param_1->seats[3].rider_slot1 = NULL;
    coords = GetScreenCoordsForObject((TileId *)param_1, SpaceTowerRideObj);
    for (; node != NULL; node = node->next) {
        if (memcmp(&node->tile.id, &param_1->var_0, 2) == 0 && (node->rider->flags & 0x80) != 0) {
            slot = node->rider->field_36;
            idx = slot >> 1;
            odd = slot & 1;
            if (odd == 0) {
                param_1->seats[idx].rider_slot0 = node;
            } else {
                param_1->seats[idx].rider_slot1 = node;
            }
            off = SpaceTowerLayerOffsets[idx];
            off.y -= param_1->seats[idx].pos;
            AdjustOffsetForViewMode(&off);
            pos.x = coords.x + off.x;
            pos.y = coords.y + off.y;
            if (odd == 0) {
                dat = SpaceTowerSeats[idx].inner;
            } else {
                dat = SpaceTowerSeats[idx].outer;
            }
            AdjustOffsetForViewMode(&dat);
            pos.x += dat.x;
            pos.y += dat.y;
            AdjustBlokePosition((struct Point *)&pos);
            person = node->person;
            SetPersonPosition(person, pos.x, pos.y);
            SetPersonDirection(person, SpaceTowerSeats[idx].direction);
        }
    }
}

// FUNCTION: LEGOLAND 0x0043b990
void UpdateSpaceTowerCar(struct SpaceTowerCar *esi) {
    esi->var_ad++;
    if ((signed char)esi->var_ad >= 16) {
        esi->var_ad = 0;
    }
    esi->var_ac++;
    if ((signed char)esi->var_ac >= 16) {
        esi->var_ac = 0;
    }
    if (esi->var_c & 1) {
        UpdateSpaceTowerSeat(&esi->seats[0]);
        UpdateSpaceTowerSeat(&esi->seats[1]);
        UpdateSpaceTowerSeat(&esi->seats[2]);
        UpdateSpaceTowerSeat(&esi->seats[3]);
        if (esi->seats[0].state == 0 && esi->seats[1].state == 0 && esi->seats[2].state == 0 && esi->seats[3].state == 0) {
            if (GetAllBlokesOffRide((struct Ride *)SpaceTowerRideObj, esi->var_0) == 0) {
                return;
            }
            ResetSpaceTowerCar(esi);
            return;
        }
    }
    AssignSpaceTowerSeatRiders(esi);
    Put3DBlokesOnRide2(SpaceTowerRideObj, esi);
    if (esi->var_c & 1) {
        return;
    }
    if (esi->var_c & 0x4000) {
        if (esi->var_2 == esi->var_4) {
            esi->var_c &= ~0x4000;
            StartSpaceTowerCar(esi);
        }
        return;
    }
    if (esi->var_2 == 0) {
        return;
    }
    if (esi->var_b0 == 0) {
        esi->var_c |= 0x4000;
        Ride_SetFlagToNotLetAnyoneOn(esi);
    } else {
        esi->var_b0--;
    }
}

// FUNCTION: LEGOLAND 0x0043baa0
void UpdateSpaceTowerCars(void) {
    struct ListNode *node;

    node = (struct ListNode *)SpaceTowerCarList;
    if (node != NULL) {
        do {
            UpdateSpaceTowerCar((struct SpaceTowerCar *)node);
            node = node->next;
        } while (node != NULL);
    }
}

// FUNCTION: LEGOLAND 0x0043bac0
void SpaceTowerUpdate(struct Element *param_1) {
    struct Ride *ride;
    struct RideNode *next;
    struct RideNode *node;
    register struct Bloke *bloke;
    register struct SpaceTowerCar *obj;
    int x;
    unsigned int y;
    TileId *tile;
    int ride_y;
    int ride_x;
    struct Point to;
    char dir;
    char seat;

    int tile_y;
    int coord_y;
    ride = param_1->ride;
    UpdateSpaceTowerCars();
    node = ride->riders;
    while (node != NULL) {
        next = node->next;
        bloke = node->rider;
        obj = FindSpaceTowerCar(&node->tile.id);
        if (!obj) {
            return;
        }
        x = node->tile.pos.x;
        ride_x = x + ride->x;
        tile = &node->tile;
        y = tile->pos.y;
        ride_y = ride->y + y;
        if (!bloke->low_level_action) {
            switch (bloke->param_action) {
            case 0:
                obj->var_4++;
                obj->var_b0 = 200;
                AssignSpaceTowerSlot(node, &tile->id);
                bloke->flags |= 8;
                ride_x <<= 8;
                ride_y = (ride_y + 1) << 8;
                bloke->dest.x = ride_x;
                bloke->dest.y = ride_y;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 1:
                node->rider->field_4a = 0;
                node->rider->field_38 = 0;
                bloke->param_action++;
                break;
            case 3:
                seat = bloke->field_36;
                coord_y = node->tile.pos.y;
                bloke->dest.x = (x + SpaceTowerSeatOffsets[seat].x) << 8;
                bloke->dest.y = (SpaceTowerSeatOffsets[seat].y + coord_y) << 8;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = dir + 0x10;
                NewDirForAction(bloke, (char)SpaceTowerSeats[seat >> 1].direction);
                bloke->param_action++;
                break;
            case 4:
                bloke->flags = bloke->flags | 0x80;
                BlokeSitAnim(bloke);
                BlokeSetFrame(bloke, 0);
                bloke->field_58 = 0;
                bloke->param_action++;
                obj->var_2++;
                SyncSpaceTowerSeatsFromOccupancy(obj);
                if ((short)(char)obj->var_2 == SpaceTowerRideObj->seats) {
                    StartSpaceTowerCar(obj);
                }
                break;
            case 6:
                node->rider->field_4c = 0xffff;
                obj->flags_a4[bloke->field_36] = 0;
                bloke->flags &= 0xff7f;
                BlokeWalkAnim(bloke);
                BlokeSetFrame(bloke, 0);
                bloke->param_action++;
                break;
            case 2:
            case 7:
                AdvanceSpaceTowerRiderLayout(node);
                break;
            case 8:
                bloke->dest.x = (((signed char)ride->field_24 + x) << 8) + 0x80;
                tile_y = tile->pos.y;
                bloke->dest.y = (((signed char)ride->field_25 + tile_y) << 8) + 0x80;
                dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = 0x10 + dir;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 9:
                RemoveBlokeFromRide((struct Ride *)ride, (struct RideNode *)node);
                bloke->flags &= 0xfff7;
                if (--obj->var_3 == 0) {
                    obj->var_2 = 0;
                    Ride_ClearFlagToNotLetAnyoneOn(obj);
                }
                break;
            }
        }
        node = next;
    }
}
