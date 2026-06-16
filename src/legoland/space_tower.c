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
#include "print_sprite.h"
#include "render3d.h"
#include "objclass.h"
#include "sound_music.h"
#include "space_tower.h"

struct SeatRider {
    /* 0x00 */ unsigned char pad_0[8];
    /* 0x08 */ struct Bloke *render;
};

struct SpaceTowerSeat {
    /* 0x00 */ unsigned int flags;
    /* 0x04 */ unsigned char pad_4[4];
    /* 0x08 */ int pos;
    /* 0x0c */ int state;
    /* 0x10 */ int field_10;
    /* 0x14 */ int field_14;
    /* 0x18 */ struct SeatRider *field_18;
    /* 0x1c */ struct SeatRider *field_1c;
    /* 0x20 */ signed char delta;
    /* 0x21 */ unsigned char pad_21[3];
};

struct RideObject {
    /* 0x00 */ unsigned short var_0;
    /* 0x02 */ unsigned char var_2;
    /* 0x03 */ unsigned char var_3;
    /* 0x04 */ unsigned char var_4;
    /* 0x05 */ unsigned char pad_5[3];
    /* 0x08 */ struct RideObject *next;
    /* 0x0c */ unsigned int var_c;
    /* 0x10 */ unsigned int var_10;
    /* 0x14 */ struct SpaceTowerSeat seats[4];
    /* 0xa4 */ unsigned char flags_a4[8];
    /* 0xac */ unsigned char var_ac;
    /* 0xad */ unsigned char var_ad;
    /* 0xae */ unsigned char pad_ae[2];
    /* 0xb0 */ unsigned int var_b0;
    /* 0xb4 */ unsigned char pad_b4[24];
    /* 0xcc */ unsigned int var_cc;
};

struct AnimEntry {
    /* 0x00 */ int count;
};

struct AnimLayout {
    /* 0x00 */ int count;
    /* 0x04 */ struct AnimEntry **entries;
};

struct SpaceTowerRideNode {
    /* 0x00 */ struct SpaceTowerRideNode *next;
    /* 0x04 */ unsigned char pad_4[4];
    /* 0x08 */ struct Bloke *bloke;
    /* 0x0c */ union {
        unsigned short id;
        struct {
            unsigned char x;
            unsigned char y;
        } coord;
    };
    /* 0x10 */ unsigned char pad_10[2];
};

struct SpaceTowerRide {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ int field_c;
    /* 0x10 */ int field_10;
    /* 0x14 */ unsigned char pad_14[0x1c - 0x14];
    /* 0x1c */ unsigned int flags;
    /* 0x20 */ unsigned char pad_20[0x64 - 0x20];
    /* 0x64 */ void *layers;
    /* 0x68 */ unsigned char pad_68[0xcc - 0x68];
    /* 0xcc */ struct SpaceTowerRideNode *list;
};

struct SpaceTowerCtx {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ struct SpaceTowerRide *ride;
};

struct ListNode {
    unsigned char pad_0[8];
    struct ListNode *next;
};

struct FadeParams {
    unsigned int field_0;
    unsigned char pad_4[4];
    unsigned int field_8;
    unsigned int field_c;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x0043a7a0
__int64 FUN_0043a7a0(int *param_1, int param_2, int param_3) {
    int *node;
    union {
        __int64 q;
        struct {
            int low;
            int high;
        } parts;
    } result;
    int *cur;
    int index;
    int remaining;

    result.parts.low = 0;
    result.parts.high = 0;
    index = 0;
    if (param_2 + 1 > 0) {
        param_1 = *(int **)((int)param_1 + 4);
        do {
            cur = (int *)*param_1;
            if (index == param_2) {
                remaining = param_3 + 1;
            } else {
                remaining = *cur;
            }
            if (remaining > 0) {
                node = (int *)cur[1];
                do {
                    result.parts.low += node[2] + node[0] * 0x100;
                    result.parts.high += node[1] * 0x100 + node[3];
                    node += 4;
                    remaining += -1;
                } while (remaining != 0);
            }
            index += 1;
            param_1 += 1;
        } while (index < param_2 + 1);
    }
    return result.q;
}

// FUNCTION: LEGOLAND 0x0043a820
void FUN_0043a820(struct AnimEntry *param_1, struct SpaceTowerRideNode *param_2) {
    struct Bloke *bloke;
    struct SpaceTowerRide *ride;
    __int64 base;
    char dir;

    bloke = param_2->bloke;
    base = FUN_0043a7a0((int *)DAT_004b7758[bloke->field_50].field_4, bloke->field_4a, bloke->field_38);
    ride = (struct SpaceTowerRide *)DAT_0062fd74;
    bloke->field_24 = (int)base + (param_2->coord.x + ride->field_c) * 0x100;
    bloke->field_28 = (int)(base >> 0x20) + (param_2->coord.y + ride->field_10) * 0x100;
    dir = CalcMoveLine(bloke->field_68, bloke->field_6c, bloke->field_24, bloke->field_28, bloke->field_98);
    bloke->field_e = 7;
    bloke->field_73 = dir + 0x10;
    NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
    param_2->bloke->field_38 = param_2->bloke->field_38 + 1;
}

// FUNCTION: LEGOLAND 0x0043a8c0
void FUN_0043a8c0(struct SpaceTowerRideNode *param_1) {
    struct Bloke *bloke;
    struct AnimLayout *layout;
    struct AnimEntry *entry;

    bloke = param_1->bloke;
    layout = (struct AnimLayout *)DAT_004b7758[bloke->field_50].field_4;
    entry = layout->entries[bloke->field_4a];
    if ((int)bloke->field_4a >= layout->count) {
        bloke->param_action = bloke->param_action + '\x01';
        return;
    }
    if ((int)bloke->field_38 >= entry->count) {
        bloke->field_38 = 0;
        param_1->bloke->field_4a = param_1->bloke->field_4a + 1;
        bloke = param_1->bloke;
        if (bloke->field_4a == (short)bloke->field_4c || (int)bloke->field_4a == layout->count) {
            bloke->param_action = bloke->param_action + '\x01';
            return;
        }
    }
    if ((int)bloke->field_38 != entry->count) {
        FUN_0043a820(entry, param_1);
    }
}

// FUNCTION: LEGOLAND 0x0043a940
void FUN_0043a940(struct SpaceTowerSeat *seat) {
    int pos;

    if ((seat->flags & 1) != 0) {
        switch (seat->state) {
        case 1:
            if (seat->field_10 == 0) {
                seat->field_10 = -1;
                return;
            }
            seat->state = 2;
            break;
        case 2:
            if (seat->field_14 == 0) {
                pos = seat->pos + seat->delta;
                seat->pos = pos;
                if (200 < pos) {
                    seat->pos = 200;
                    seat->field_14 = 1;
                    return;
                }
            } else {
                pos = seat->pos - 2;
                seat->pos = pos;
                if (pos < 0) {
                    seat->pos = 0;
                    seat->field_14 = 0;
                    seat->state = 0;
                    return;
                }
            }
            break;
        }
    }
    return;
}

// FUNCTION: LEGOLAND 0x0043a9b0
void FUN_0043a9b0(struct RideObject *arg) {
    struct SpaceTowerSeat *seat;
    int i;

    i = 4;
    seat = arg->seats;
    do {
        i = i + -1;
        seat->flags = seat->flags & 0xfffffffe;
        seat->state = 0;
        seat = seat + 1;
    } while (i != 0);
    i = 0;
    do {
        if (arg->flags_a4[i] != '\0') {
            arg->seats[i >> 1].flags = arg->seats[i >> 1].flags | 1;
            arg->seats[i >> 1].state = 2;
            arg->seats[i >> 1].field_14 = 0;
            arg->seats[i >> 1].pos = 0;
        }
        i = i + 1;
    } while (i < 8);
    return;
}

// FUNCTION: LEGOLAND 0x0043aa10
void FUN_0043aa10(unsigned char *arg) {
    unsigned int buffer[4];

    buffer[0] = 2;
    buffer[2] = arg[0];
    buffer[3] = arg[1];
    PlayInstanceOfSample(DAT_004b7620, 1, 1, buffer);
}

// FUNCTION: LEGOLAND 0x0043aa50
void FUN_0043aa50(unsigned char *arg) {
    struct FadeParams params;
    params.field_0 = 2;
    params.field_8 = arg[0];
    params.field_c = arg[1];
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
}

// FUNCTION: LEGOLAND 0x0043aa90
void FUN_0043aa90(struct RideObject *arg) {
    unsigned char tmp = arg->var_2;
    arg->var_c |= 1;
    arg->var_3 = tmp;
    arg->var_10 = 0;
    FUN_0043a9b0(arg);
    FUN_0043aa10((unsigned char *)arg);
}

// FUNCTION: LEGOLAND 0x0043aac0
void FUN_0043aac0(struct RideObject *arg) {
    int r;

    arg->var_c = arg->var_c & 0xffffbffe;
    arg->seats[0].flags = arg->seats[0].flags & 0xfffffffe;
    arg->seats[1].flags = arg->seats[1].flags & 0xfffffffe;
    arg->seats[0].pos = 0;
    arg->seats[1].pos = 0;
    arg->seats[2].pos = 0;
    arg->seats[3].pos = 0;
    arg->seats[2].flags = arg->seats[2].flags & 0xfffffffe;
    arg->seats[3].flags = arg->seats[3].flags & 0xfffffffe;
    r = rand();
    arg->seats[0].delta = (char)(r % 3) + '\a';
    r = rand();
    arg->seats[1].delta = (char)(r % 3) + '\a';
    r = rand();
    arg->seats[2].delta = (char)(r % 3) + '\a';
    r = rand();
    arg->var_4 = 0;
    arg->var_2 = 0;
    arg->seats[3].delta = (char)(r % 3) + '\a';
    FUN_0043aa50((unsigned char *)arg);
}

// FUNCTION: LEGOLAND 0x0043ab70
void FUN_0043ab70(unsigned short *param_1) {
    struct RideObject *node;
    unsigned int *fill;
    int i;

    node = (struct RideObject *)malloc(0xb4);
    if (node != NULL) {
        fill = (unsigned int *)node;
        for (i = 0x2d; i != 0; i = i + -1) {
            *fill = 0;
            fill = fill + 1;
        }
        node->var_0 = *param_1;
        node->next = DAT_0062fda8;
        DAT_0062fda8 = node;
        FUN_0043aac0(node);
    }
}

// FUNCTION: LEGOLAND 0x0043abc0
void FUN_0043abc0(struct RideObject *arg) {
    struct RideObject *cur;
    struct RideObject *next;

    if (DAT_0062fda8 == arg) {
        DAT_0062fda8 = arg->next;
        free(arg);
        return;
    }
    next = DAT_0062fda8->next;
    cur = DAT_0062fda8;
    while (next != arg) {
        cur = cur->next;
        if (cur == NULL) {
            goto done;
        }
        next = cur->next;
    }
    if (cur != NULL) {
        cur->next = arg->next;
    }
done:
    free(arg);
}

// FUNCTION: LEGOLAND 0x0043ac20
void FUN_0043ac20(void) {
    while (DAT_0062fda8 != NULL) {
        FUN_0043abc0(DAT_0062fda8);
    }
}

// FUNCTION: LEGOLAND 0x0043ac40
struct RideObject *FUN_0043ac40(unsigned short *param_1) {
    struct RideObject *node;

    node = DAT_0062fda8;
    if (node != NULL) {
        if (node->var_0 == *param_1) {
            return node;
        }
        while (1) {
            node = node->next;
            if (node == NULL) {
                break;
            }
            if (node->var_0 == *param_1) {
                return node;
            }
        }
    }
    return node;
}

// FUNCTION: LEGOLAND 0x0043ac70
void FUN_0043ac70(struct SpaceTowerRideNode *param_1, unsigned short *param_2) {
    struct RideObject *ride;
    unsigned int slot;

    ride = FUN_0043ac40(param_2);
    if (ride != NULL) {
        slot = FUN_0043acb0(param_1, ride);
        param_1->bloke->field_50 = slot;
        param_1->bloke->field_4c = DAT_004b7758[slot].field_0;
    }
}

// FUNCTION: LEGOLAND 0x0043acb0
unsigned int FUN_0043acb0(struct SpaceTowerRideNode *param_1, struct RideObject *param_2) {
    char slot_used;
    unsigned int slot;

    slot = rand();
    slot = slot & 0x80000007;
    if ((int)slot < 0) {
        slot = (slot - 1 | 0xfffffff8) + 1;
    }
    slot_used = param_2->flags_a4[slot];
    while (slot_used != '\0') {
        slot = slot + 1;
        if ((int)slot >= 8) {
            slot = 0;
        }
        slot_used = param_2->flags_a4[slot];
    }
    param_2->flags_a4[slot] = 1;
    param_1->bloke->field_36 = (char)slot;
    return slot;
}

// FUNCTION: LEGOLAND 0x0043ad00
int FUN_0043ad00(unsigned char *param_1, int param_2) {
    short y_scroll;
    unsigned int x;
    unsigned int y;
    int dim;
    int width;
    int height;

    y = param_1[1];
    x = param_1[0];
    switch (param_2) {
    case 0:
    case 3:
        x = x + 2;
        y = y + 2;
        break;
    case 1:
    case 2:
        x = x - 2;
        y = y - 2;
    }
    GetTileDimensions(&width, &height);
    dim = (y * 0x100 + x * 0x100) * height;
    Get_XScroll();
    y_scroll = (short)Get_YScroll();
    return ((unsigned int)lpConfig->field_22 - (int)y_scroll) + (dim >> 9);
}

// FUNCTION: LEGOLAND 0x0043ad90
void FUN_0043ad90(struct RideObject *param_1, int param_2, unsigned int param_3) {
    __int64 coords;
    int off_x;
    int off_y;

    coords = GetScreenCoordsForObject((unsigned char *)param_1, DAT_0062fd74);
    if (DAT_0062fd64[param_2] != NULL) {
        off_x = DAT_0062fd88[param_2].x;
        off_y = DAT_0062fd88[param_2].y - param_1->seats[param_2].pos;
        AdjustOffsetForViewMode((struct AdjustStruct *)&off_x);
        PrintSprite(DAT_0062fd64[param_2], off_x + (int)coords, off_y + (int)(coords >> 0x20), param_3, 0);
    }
}

// FUNCTION: LEGOLAND 0x0043ae20
void FUN_0043ae20(struct RideObject *param_1, int param_2, unsigned int param_3) {
    __int64 coords;
    int off_x;
    int off_y;
    int sprite_id;

    coords = GetScreenCoordsForObject((unsigned char *)param_1, DAT_0062fd74);
    off_x = DAT_0062fd88[param_2].x;
    off_y = DAT_0062fd88[param_2].y - param_1->seats[param_2].pos;
    AdjustOffsetForViewMode((struct AdjustStruct *)&off_x);
    switch (param_2) {
    case 0:
        sprite_id = 6;
        break;
    case 1:
        sprite_id = 4;
        break;
    case 2:
        sprite_id = 0;
        break;
    case 3:
        sprite_id = 2;
        break;
    default:
        sprite_id = off_y;
    }
    PrintSprite(GetSpriteForLayer((struct LayerContainer *)DAT_0062fd60, sprite_id), off_x + (int)coords, (int)(coords >> 0x20) + off_y, param_3, 0);
}

// FUNCTION: LEGOLAND 0x0043aee0
void FUN_0043aee0(struct RideObject *param_1, int param_2, unsigned int param_3) {
    FUN_0043ae20(param_1, param_2, param_3);
    if ((param_1->seats[param_2].flags & 1) != 0) {
        FUN_0043ad00((unsigned char *)param_1, param_2);
        if (param_1->seats[param_2].field_18 != NULL) {
            IP_RenderBlokeIn3DNow(param_1->seats[param_2].field_18->render);
        }
        if (param_1->seats[param_2].field_1c != NULL) {
            IP_RenderBlokeIn3DNow(param_1->seats[param_2].field_1c->render);
        }
        FUN_0043ad90(param_1, param_2, param_3);
    }
}

// FUNCTION: LEGOLAND 0x0043af50
void FUN_0043af50(struct SpaceTowerCtx *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, unsigned int param_5, unsigned int param_6) {
    struct SpaceTowerRide *ride;
    struct SpaceTowerRideNode *node;
    struct RideObject *obj;
    struct Bloke *bloke;
    int coord_x;
    int coord_y;
    union {
        __int64 q;
        struct {
            int low;
            int high;
        } parts;
    } coords;
    struct LayerOffset local_8;
    struct LayerOffset local_10;

    ride = param_1->ride;
    node = ride->list;
    obj = FUN_0043ac40(param_4);
    if (obj != NULL) {
        coords.q = GetScreenCoordsForObject((unsigned char *)param_4, ride);
        coord_x = coords.parts.low;
        coord_y = coords.parts.high;
        *(__int64 *)&local_8 = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_0062fd60, 1);
        AdjustOffsetForViewMode((struct AdjustStruct *)&local_8);
        if (node == NULL) {
            FUN_0043aee0(obj, 1, param_6);
            FUN_0043aee0(obj, 2, param_6);
            PrintSprite(DAT_0062fd7c, local_8.x + coord_x, local_8.y + coord_y, param_6, 0);
            FUN_0043aee0(obj, 0, param_6);
            FUN_0043aee0(obj, 3, param_6);
        } else {
            do {
                if (node->id == *param_4) {
                    bloke = node->bloke;
                    if ((bloke->flags & 0x80) == 0 &&
                        DAT_004b7758[bloke->field_50].field_4 == &DAT_004b7750) {
                        IP_RenderBlokeIn3DNow(bloke);
                    }
                }
                node = node->next;
            } while (node != NULL);
            FUN_0043aee0(obj, 1, param_6);
            FUN_0043aee0(obj, 2, param_6);
            PrintSprite(DAT_0062fd7c, local_8.x + coord_x, local_8.y + coord_y, param_6, 0);
            FUN_0043aee0(obj, 0, param_6);
            FUN_0043aee0(obj, 3, param_6);
            node = ride->list;
            while (node != NULL) {
                if (node->id == *param_4) {
                    bloke = node->bloke;
                    if ((bloke->flags & 0x80) == 0 &&
                        DAT_004b7758[bloke->field_50].field_4 == &DAT_004b76b8) {
                        IP_RenderBlokeIn3DNow(bloke);
                    }
                }
                node = node->next;
            }
            PrintSprite(DAT_0062fd80, local_8.x + coord_x, local_8.y + coord_y, param_6, 0);
        }
        LLSSetFrame(GetLLSForLayer(DAT_0062fd60, 5), (int)obj->var_ad);
        *(__int64 *)&local_10 = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_0062fd60, 5);
        AdjustOffsetForViewMode((struct AdjustStruct *)&local_10);
        PrintSprite(GetSpriteForLayer((struct LayerContainer *)DAT_0062fd60, 5), local_10.x + coord_x, local_10.y + coord_y, param_6, 0);
        LLSSetFrame(GetLLSForLayer(DAT_0062fd60, 3), (int)obj->var_ac);
        *(__int64 *)&local_10 = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_0062fd60, 3);
        AdjustOffsetForViewMode((struct AdjustStruct *)&local_10);
        PrintSprite(GetSpriteForLayer((struct LayerContainer *)DAT_0062fd60, 3), local_10.x + coord_x, local_10.y + coord_y, param_6, 0);
    }
}

// FUNCTION: LEGOLAND 0x0043b2b0
void FUN_0043b2b0(struct SpaceTowerCtx *param_1) {
    struct SpaceTowerRide *ride;

    ride = param_1->ride;
    DAT_0062fd74 = ride;
    ride->flags = ride->flags | 0x420;
    DAT_0062fd60 = ride->layers;
    *(unsigned int *)((char *)DAT_0062fd60 + 0x10) = *(unsigned int *)((char *)DAT_0062fd60 + 0x10) | 0x2000;
    HideLayer(DAT_0062fd60, 5);
    StopLayerPlaying(DAT_0062fd60, 5);
    LLSSetFrame(GetLLSForLayer(DAT_0062fd60, 5), 0);
    HideLayer(DAT_0062fd60, 3);
    StopLayerPlaying(DAT_0062fd60, 3);
    LLSSetFrame(GetLLSForLayer(DAT_0062fd60, 3), 0);
    *(__int64 *)&DAT_0062fd88[0] = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_0062fd60, 6);
    *(__int64 *)&DAT_0062fd88[1] = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_0062fd60, 4);
    *(__int64 *)&DAT_0062fd88[2] = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_0062fd60, 0);
    *(__int64 *)&DAT_0062fd88[3] = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_0062fd60, 2);
    DAT_0062fd64[0] = NULL;
    // STRING: LEGOLAND 0x004b7880
    DAT_0062fd64[1] = LoadSprite("SpaceTower Seat2 Matte.lls", 1);
    // STRING: LEGOLAND 0x004b7864
    DAT_0062fd64[2] = LoadSprite("SpaceTower Seat3 Matte.lls", 1);
    DAT_0062fd64[3] = NULL;
    // STRING: LEGOLAND 0x004b7850
    DAT_0062fd80 = LoadSprite("Spacet Matte1.lls", 1);
    // STRING: LEGOLAND 0x004b783c
    DAT_0062fd7c = LoadSprite("Spacet Matte2.lls", 1);
    Load_FXList(SPACE_TOWER_SFX, 1);
}

// FUNCTION: LEGOLAND 0x0043b420
void FUN_0043b420(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0062fd74;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((void *)((unsigned int)DAT_008119b8 + 0x3c));
}

// FUNCTION: LEGOLAND 0x0043b460
void FUN_0043b460(struct EditObject *param_1, unsigned int param_2, struct Cursor *param_3) {
    struct RideObject *node;

    node = FUN_0043ac40((unsigned short *)&param_2);
    if (node != NULL) {
        FUN_0043abc0(node);
    }
    StandardRemoveObject(param_1, param_2, param_3);
    RemoveAllBlokesFromRide(*(struct Ride **)((char *)param_1 + 0xc), param_2);
    FUN_0043aa50((unsigned char *)&param_2);
}

// FUNCTION: LEGOLAND 0x0043b4b0
void FUN_0043b4b0(struct EditObject *param_1, unsigned char *param_2) {
    unsigned char *src = param_2;
    unsigned char b0 = param_2[0];
    unsigned char b4 = param_2[4];

    *(unsigned char *)&param_2 = b0;
    *((unsigned char *)&param_2 + 1) = b4;
    AddBasicObject(param_1, (int *)src);
    FUN_0043ab70((unsigned short *)&param_2);
}

// FUNCTION: LEGOLAND 0x0043b4e0
void *FUN_0043b4e0(int param_1, unsigned short param_2) {
    int ride;

    ride = *(int *)(param_1 + 0xc);
    DAT_0062fd48 = *(unsigned int *)(ride + 0x64);
    DAT_0062fd4c = *(unsigned int *)(ride + 0x14);
    DAT_0062fd50 = *(unsigned int *)(ride + 0x18);
    DAT_0062fd54 = param_2;
    *(unsigned int *)(*(int *)(ride + 0x64) + 0x10) |= 0x2000;
    HideLayer(DAT_0062fd60, 6);
    HideLayer(DAT_0062fd60, 4);
    HideLayer(DAT_0062fd60, 0);
    HideLayer(DAT_0062fd60, 2);
    HideLayer(DAT_0062fd60, 5);
    return &DAT_0062fd48;
}

// FUNCTION: LEGOLAND 0x0043b570
void FUN_0043b570(void) {
    KillSprite(DAT_0062fd80);
    KillSprite(DAT_0062fd7c);
    KillSprite(DAT_0062fd64[1]);
    KillSprite(DAT_0062fd64[2]);
    FUN_0043ac20();
    Kill_FXList(SPACE_TOWER_SFX, 1);
    ((struct RideObject *)DAT_0062fd74)->var_cc = 0;
}

// FUNCTION: LEGOLAND 0x0043b5d0
LEGO_EXPORT int SpaceTower_Save(void) {
    int *field;
    int *cursor;
    int target;
    int index;
    unsigned int i;
    unsigned int one;
    unsigned int zero;
    char *node;

    one = 1;
    zero = 0;
    node = (char *)DAT_0062fda8;
    while (node != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        i = 0;
        do {
            if (i & 1) {
                field = (int *)(node + 0x30 + ((int)i >> 1) * 0x24);
            } else {
                field = (int *)(node + 0x2c + ((int)i >> 1) * 0x24);
            }
            index = 0;
            cursor = *(int **)((char *)DAT_0062fd74 + 0xcc);
            if (cursor != NULL) {
                target = *field;
                do {
                    if (cursor == (int *)target) {
                        if (cursor != NULL) {
                            *field = index + 1;
                            goto next;
                        }
                        break;
                    }
                    cursor = (int *)*cursor;
                    index = index + 1;
                } while (cursor != NULL);
            }
            *field = 0;
        next:
            i = i + 1;
        } while ((int)i < 8);
        if (SaveGameWrite(node, 0xb4) == 0) {
            return 0;
        }
        node = *(char **)(node + 8);
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x0043b6a0
LEGO_EXPORT void SpaceTower_Load(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043b780
void SpaceTowerRide(struct ClassNode *name, struct CallbackTable *obj) {
    // STRING: LEGOLAND 0x004b789c
    if (_stricmp("SPACE TOWER RIDE", name->name) == 0) {
        obj->cb_a4 = FUN_0043b2b0;
        obj->cb_8c = FUN_0043b420;
        obj->cb_a8 = FUN_0043bac0;
        obj->cb_a0 = FUN_0043b4e0;
        obj->cb_b0 = FUN_0043af50;
        obj->cb_9c = FUN_0043b460;
        obj->cb_98 = FUN_0043b4b0;
        obj->cb_ac = FUN_0043b570;
        obj->cb_bc = SpaceTower_Save;
        obj->cb_b8 = SpaceTower_Load;
    }
}

// FUNCTION: LEGOLAND 0x0043b810
void FUN_0043b810(struct RideObject *arg) { STUB(); }

// FUNCTION: LEGOLAND 0x0043b990
void FUN_0043b990(struct RideObject *esi) {
    esi->var_ad++;
    if ((signed char)esi->var_ad >= 16) {
        esi->var_ad = 0;
    }
    esi->var_ac++;
    if ((signed char)esi->var_ac >= 16) {
        esi->var_ac = 0;
    }
    if (esi->var_c & 1) {
        FUN_0043a940(&esi->seats[0]);
        FUN_0043a940(&esi->seats[1]);
        FUN_0043a940(&esi->seats[2]);
        FUN_0043a940(&esi->seats[3]);
        if (esi->seats[0].state == 0 && esi->seats[1].state == 0 && esi->seats[2].state == 0 && esi->seats[3].state == 0) {
            if (GetAllBlokesOffRide((struct Ride *)DAT_0062fd74, esi->var_0) == 0) {
                return;
            }
            FUN_0043aac0(esi);
            return;
        }
    }
    FUN_0043b810(esi);
    Put3DBlokesOnRide2(DAT_0062fd74, esi);
    if (esi->var_c & 1) {
        return;
    }
    if (esi->var_c & 0x4000) {
        if (esi->var_2 == esi->var_4) {
            esi->var_c &= ~0x4000;
            FUN_0043aa90(esi);
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
void FUN_0043baa0(void) {
    struct ListNode *node;

    node = DAT_0062fda8;
    if (node != NULL) {
        do {
            FUN_0043b990((struct RideObject *)node);
            node = node->next;
        } while (node != NULL);
    }
}

// FUNCTION: LEGOLAND 0x0043bac0
void FUN_0043bac0(void) { STUB(); }
