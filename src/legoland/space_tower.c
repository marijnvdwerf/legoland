#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "gamemap.h"
#include "globals.h"
#include "llidb.h"
#include "map_object.h"
#include "obj_instance.h"
#include "render3d.h"
#include "objclass.h"
#include "sound_music.h"
#include "space_tower.h"

struct SpaceTowerSeat {
    /* 0x00 */ unsigned int flags;
    /* 0x04 */ unsigned char pad_4[4];
    /* 0x08 */ int pos;
    /* 0x0c */ int state;
    /* 0x10 */ int field_10;
    /* 0x14 */ int field_14;
    /* 0x18 */ unsigned char pad_18[8];
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
void FUN_0043a820(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043a8c0
void FUN_0043a8c0(void) { STUB(); }

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
void FUN_0043ac70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043acb0
void FUN_0043acb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043ad00
void FUN_0043ad00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043ad90
void FUN_0043ad90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043ae20
void FUN_0043ae20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043aee0
void FUN_0043aee0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043af50
void FUN_0043af50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043b2b0
void FUN_0043b2b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043b420
void FUN_0043b420(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0062fd74;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((void *)((unsigned int)DAT_008119b8 + 0x3c));
}

// FUNCTION: LEGOLAND 0x0043b460
void FUN_0043b460(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043b4b0
void FUN_0043b4b0(void) { STUB(); }

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
    KillSprite(DAT_0062fd68);
    KillSprite(DAT_0062fd6c);
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
