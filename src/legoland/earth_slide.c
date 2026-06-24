#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "bloke.h"
#include "earth_slide.h"
#include "gamemap.h"
#include "globals.h"
#include "image_sprite.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render3d.h"

// FUNCTION: LEGOLAND 0x0042cd70
void FUN_0042cd70(unsigned short *a1) {
    struct EarthNode *v = (struct EarthNode *)malloc(sizeof(struct EarthNode));
    if (v != NULL) {
        memset(v, 0, sizeof(struct EarthNode));
        v->id = *a1;
        v->next = DAT_006160e8;
        v->field_10 = 0;
        v->field_14 = 0;
        v->field_b = 0;
        v->field_8 = 0;
        v->field_a = 0;
        v->field_4 = 1;
        DAT_006160e8 = v;
    }
}

// FUNCTION: LEGOLAND 0x0042cdc0
void FUN_0042cdc0(struct EarthNode *node) {
    struct EarthNode *cur;
    struct EarthNode *prev;

    if (DAT_006160e8 == node) {
        DAT_006160e8 = node->next;
        free(node);
        return;
    }
    cur = DAT_006160e8->next;
    prev = DAT_006160e8;
    while (cur != node) {
        prev = prev->next;
        if (prev == NULL) {
            goto done;
        }
        cur = prev->next;
    }
    if (prev != NULL) {
        prev->next = node->next;
    }
done:
    free(node);
    return;
}

// FUNCTION: LEGOLAND 0x0042ce20
struct EarthNode *FUN_0042ce20(unsigned short *param_1) {
    struct EarthNode *node = DAT_006160e8;

    if (node != NULL) {
        if (*param_1 == node->id) {
            return node;
        }
        while (1) {
            node = node->next;
            if (node == NULL) {
                break;
            }
            if (*param_1 == node->id) {
                return node;
            }
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0042ce50
void FUN_0042ce50(struct EarthNode *node, struct EarthBlokeElem *elem) {
    struct EarthQueue *q = (struct EarthQueue *)malloc(sizeof(struct EarthQueue));
    if (q != NULL) {
        q->next = NULL;
        q->elem = elem;
        *(unsigned char *)((char *)elem->bloke + 0x62) |= 0x40;
        FUN_0042ce90(node, q);
        node->field_18++;
    }
}

// FUNCTION: LEGOLAND 0x0042ce90
void FUN_0042ce90(struct EarthNode *p, struct EarthQueue *value) {
    if (p->queue_head == NULL && p->queue_tail == NULL) {
        p->queue_head = value;
        p->queue_tail = value;
    } else {
        p->queue_tail->next = value;
        p->queue_tail = value;
    }
}

// FUNCTION: LEGOLAND 0x0042cec0
void FUN_0042cec0(struct Cursor *param_1, unsigned char *param_2, int *param_3) {
    struct EarthNode *node = FUN_0042ce20((unsigned short *)param_2);
    struct EarthQueue *q = node->queue_head;
    int x = *(int *)((char *)param_1 + 0xc) + param_2[0];
    int y = param_2[1] + *(int *)((char *)param_1 + 0x10);
    int i;

    if (q == NULL) {
        param_3[0] = (x + DAT_004b65c0[0]) * 0x100;
        param_3[1] = (DAT_004b65c0[1] + y) * 0x100;
        return;
    }
    i = 0;
    do {
        q = q->next;
        i++;
    } while (q != NULL);
    param_3[0] = (DAT_004b65c0[i * 2] + x) * 0x100;
    param_3[1] = (DAT_004b65c0[i * 2 + 1] + y) * 0x100;
}

// FUNCTION: LEGOLAND 0x0042cf40
int FUN_0042cf40(struct EarthNode *param_1, void *param_2) {
    if (param_1->queue_head != NULL) {
        if (param_1->queue_head->elem->bloke == param_2) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0042cf70
void FUN_0042cf70(struct EarthNode *param_1) {
    unsigned char bx;
    unsigned char by;
    void *bloke;
    struct EarthQueue *q;
    int x;
    int y;
    int *tbl;
    char cv;

    if (param_1->queue_head != NULL) {
        bloke = param_1->queue_head->elem->bloke;
        *(unsigned short *)((char *)bloke + 0x62) &= 0xffbf;
        *(char *)((char *)bloke + 0x60) += 1;
        FUN_0042d040(param_1);
        q = param_1->queue_head;
        if (q != NULL) {
            bx = *(unsigned char *)param_1;
            by = *((unsigned char *)param_1 + 1);
            tbl = &DAT_004b65c0[1];
            x = *(int *)((char *)DAT_006160d0 + 0xc);
            y = *(int *)((char *)DAT_006160d0 + 0x10);
            do {
                bloke = q->elem->bloke;
                *(unsigned int *)((char *)bloke + 0x24) = (tbl[-1] + x + bx) * 0x100;
                *(int *)((char *)bloke + 0x28) = (tbl[0] + by + y) * 0x100;
                cv = CalcMoveLine(*(int *)((char *)bloke + 0x68), *(int *)((char *)bloke + 0x6c),
                    *(int *)((char *)bloke + 0x24), *(int *)((char *)bloke + 0x28), (char *)bloke + 0x98);
                *(unsigned short *)((char *)bloke + 0xe) = 7;
                *(unsigned char *)((char *)bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                q = q->next;
                tbl = tbl + 2;
            } while (q != NULL);
        }
    }
}

// FUNCTION: LEGOLAND 0x0042d040
void FUN_0042d040(struct EarthNode *list) {
    if (list->queue_head != NULL) {
        struct EarthQueue *node = list->queue_head;
        list->queue_head = node->next;
        if (list->queue_tail == node) {
            list->queue_tail = NULL;
        }
        list->field_18--;
    }
}

// FUNCTION: LEGOLAND 0x0042d070
void FUN_0042d070(struct EarthRideObj *param_1, unsigned int param_2, unsigned int param_3, unsigned char *param_4, unsigned int param_5, unsigned int param_6) {
    struct Cursor *ride = param_1->ride;
    struct EarthNode *node = FUN_0042ce20((unsigned short *)param_4);
    union {
        __int64 q;
        int i[2];
    } coords;
    char cv;

    if (node != NULL) {
        cv = node->field_b;
        if (cv <= 0xa || 0xf <= cv) {
            RenderUsingRin((struct RinRender *)DAT_006160d4, (int)cv, (struct ViewportEntry *)ride, param_4);
        }
        RenderBlokesNotInSeats((unsigned int)ride, (unsigned int)param_4);
        if (node->field_4 != 0) {
            coords.q = GetScreenCoordsForObject(param_4, ride);
            PrintSprite(DAT_006160d8, coords.i[0], coords.i[1], param_6, 0);
        }
        coords.q = GetScreenCoordsForObject(param_4, ride);
        PrintSprite(DAT_006160e0, coords.i[0], coords.i[1], param_6, 0);
    }
}

// FUNCTION: LEGOLAND 0x0042d100
void FUN_0042d100(struct EarthRideObj *param_1) {
    int layer;

    DAT_006160d0 = (unsigned int)param_1->ride;
    *(unsigned int *)(DAT_006160d0 + 0x1c) |= 0x20;
    layer = *(int *)(DAT_006160d0 + 0x64);
    if (layer != 0) {
        *(unsigned int *)(layer + 0x10) |= 0x2000;
    }
    DAT_006160e8 = NULL;
    // STRING: LEGOLAND 0x004b663c
    DAT_006160e4 = LoadPos("3ddata\\earth.pos");
    // STRING: LEGOLAND 0x004b6620
    DAT_006160d4 = LoadRin("3ddata\\earthslide.rin", DAT_004b6638);
    if (DAT_006160d4 != NULL) {
        ((int *)DAT_006160d4)[0] = 0xffffff62;
        ((int *)DAT_006160d4)[1] = 0xfffffffa;
        ((int *)DAT_006160d4)[3] = 0;
    }
    if (DAT_006160e4 != NULL) {
        *(int *)((char *)DAT_006160e4 + 0x14) = 0x53;
        *(int *)((char *)DAT_006160e4 + 0x18) = 0xc4;
        *(int *)((char *)DAT_006160e4 + 0x1c) = 3;
        *(int *)((char *)DAT_006160e4 + 0x20) = 0x41;
    }
    // STRING: LEGOLAND 0x004b6600
    DAT_006160d8 = LoadSprite("EarthSlide Entrance Matte.lls", 1);
    // STRING: LEGOLAND 0x004b65e0
    DAT_006160e0 = LoadSprite("EarthSlideEntranceMatte2.lls", 1);
    DAT_006160c8 = 0;
    DAT_006160cc = 0;
}

// FUNCTION: LEGOLAND 0x0042d1f0
void FUN_0042d1f0(struct EarthRideObj *arg1) {
    DAT_006160d0 = (unsigned int)arg1->ride;
    UnLoadRin(DAT_006160d4);
    UnloadPos(DAT_006160e4);
    KillSprite(DAT_006160d8);
    KillSprite(DAT_006160e0);
}

// FUNCTION: LEGOLAND 0x0042d230
void FUN_0042d230(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = (void *)DAT_006160d0;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)EditMode.unk8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x0042d270
void FUN_0042d270(struct EarthRideObj *param_1, unsigned int param_2, unsigned int param_3) {
    struct EarthNode *node = FUN_0042ce20((unsigned short *)&param_2);
    if (node != NULL) {
        FUN_0042cdc0(node);
    }
    StandardRemoveObject((unsigned int)param_1, param_2, param_3);
    RemoveAllBlokesFromRide((unsigned int)param_1->ride, param_2);
}

// FUNCTION: LEGOLAND 0x0042d2c0
void FUN_0042d2c0(unsigned int param_1, unsigned char *param_2) {
    struct {
        unsigned char var_c;
        unsigned char var_10;
        unsigned char var_11;
    } buffer;

    buffer.var_c = param_2[0];
    buffer.var_11 = param_2[4];
    AddBasicObject(param_1, (unsigned int)param_2);
    FUN_0042cd70((unsigned short *)&buffer.var_10);
}

// FUNCTION: LEGOLAND 0x0042d2f0
int FUN_0042d2f0(void) {
    struct EarthNode *node;
    struct EarthQueue *q;
    int count;
    unsigned int value;
    int flag;
    int terminator;

    flag = 1;
    node = DAT_006160e8;
    terminator = 0;
    while (1) {
        if (node == NULL) {
            return SaveGameWrite(&terminator, 4) != 0;
        }
        if (SaveGameWrite(&flag, 4) == 0 || SaveGameWrite(node, 0x24) == 0) {
            break;
        }
        count = 0;
        for (q = node->queue_head; q != NULL; q = q->next) {
            count++;
        }
        if (SaveGameWrite(&count, 4) == 0) {
            return 0;
        }
        for (q = node->queue_head; q != NULL; q = q->next) {
            value = FUN_0042d3e0(*(struct EarthBlokeElem **)(DAT_006160d0 + 0xcc), q->elem);
            if (SaveGameWrite(&value, 4) == 0) {
                return 0;
            }
        }
        node = node->next;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0042d3e0
unsigned int FUN_0042d3e0(struct EarthBlokeElem *param_1, struct EarthBlokeElem *param_2) {
    unsigned int count = 0;
    struct EarthBlokeElem *node = param_1;

    while (node != NULL && node != param_2) {
        node = node->next;
        count++;
    }
    return count;
}

// FUNCTION: LEGOLAND 0x0042d400
int FUN_0042d400(void) {
    struct EarthNode *node;
    struct EarthNode *prev;
    struct EarthQueue *q;
    int flag;
    int count;
    int value;

    if (SaveGameRead(&flag, 4) == 0) {
        return 0;
    }
    prev = NULL;
    while (flag != 0) {
        if (prev == NULL) {
            node = (struct EarthNode *)malloc(sizeof(struct EarthNode));
            DAT_006160e8 = node;
        } else {
            node = (struct EarthNode *)malloc(sizeof(struct EarthNode));
            prev->next = node;
        }
        if (SaveGameRead(node, 0x24) == 0) {
            return 0;
        }
        if (SaveGameRead(&count, 4) == 0) {
            return 0;
        }
        node->queue_head = NULL;
        node->queue_tail = NULL;
        while (count != 0) {
            count--;
            if (node->queue_tail == NULL) {
                q = (struct EarthQueue *)malloc(sizeof(struct EarthQueue));
                node->queue_tail = q;
                node->queue_head = q;
            } else {
                q = (struct EarthQueue *)malloc(sizeof(struct EarthQueue));
                node->queue_tail->next = q;
                node->queue_tail = node->queue_tail->next;
            }
            if (SaveGameRead(&value, 4) == 0) {
                return 0;
            }
            node->queue_tail->elem = FUN_0042d540(*(struct EarthBlokeElem **)(DAT_006160d0 + 0xcc), value);
        }
        if (node->queue_tail != NULL) {
            node->queue_tail->next = NULL;
        }
        if (SaveGameRead(&flag, 4) == 0) {
            return 0;
        }
        prev = node;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0042d540
struct EarthBlokeElem *FUN_0042d540(struct EarthBlokeElem *param_1, unsigned int param_2) {
    struct EarthBlokeElem *result = param_1;
    unsigned int n = param_2;

    n = n - 1;
    if (n == 0) {
        return result;
    }
    n = n + 1;
    while (n != 0) {
        result = result->next;
        n = n - 1;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0042d560
void FUN_0042d560(unsigned short *param_1) {
    int v14;
    unsigned int flags;
    char nb;

    if ((*(unsigned int *)((char *)param_1 + 0x10) & 1) != 0) {
        v14 = *(int *)((char *)param_1 + 0x14);
        *(int *)((char *)param_1 + 0x14) = v14 + 1;
        if (2 < v14 + 1) {
            nb = *(char *)((char *)param_1 + 0xb) + 1;
            *(int *)((char *)param_1 + 0x14) = 0;
            *(char *)((char *)param_1 + 0xb) = nb;
            if (*(int *)((char *)DAT_006160d4 + 0x14) <= (int)nb) {
                flags = *(unsigned int *)((char *)param_1 + 0x10) & 0xfffffffe;
                *(unsigned char *)((char *)param_1 + 0xb) = 0;
                *(unsigned int *)((char *)param_1 + 0x10) = flags;
                GetAllBlokesOffRide((struct Ride *)DAT_006160d0, *param_1);
                *(unsigned int *)((char *)param_1 + 0x4) = 1;
                return;
            }
        }
        Put3DBlokesOnRide((struct ViewportEntry *)DAT_006160d0, (unsigned char *)param_1, (int)*(char *)((char *)param_1 + 0xb), (int *)DAT_006160e4);
    }
    Put3DBlokesOnRide2((struct RideObject *)DAT_006160d0, (struct RideObject *)param_1);
}

// FUNCTION: LEGOLAND 0x0042d5f0
void FUN_0042d5f0(void) {
    struct EarthNode *node = DAT_006160e8;
    while (node != NULL) {
        FUN_0042d560((unsigned short *)node);
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x0042d610
void FUN_0042d610(struct EarthRideObj *param_1) {
    struct Cursor *ride = param_1->ride;
    struct EarthBlokeElem *elem;
    struct EarthBlokeElem *next;
    struct EarthNode *node;
    void *bloke;
    unsigned int idx;
    unsigned char by;
    int bx;
    int byy;
    char cx;
    char cy;
    char cv;
    int local_8;
    int local_4;

    elem = *(struct EarthBlokeElem **)((char *)ride + 0xcc);
    FUN_0042d5f0();
    while (1) {
        if (elem == NULL) {
            return;
        }
        next = elem->next;
        bloke = elem->bloke;
        node = FUN_0042ce20((unsigned short *)&elem->id_x);
        if (node == NULL) {
            return;
        }
        idx = elem->id_x;
        by = elem->id_y;
        bx = *(int *)((char *)ride + 0xc) + idx;
        cx = *(char *)((char *)ride + 0x24);
        byy = *(int *)((char *)ride + 0x10) + by;
        cy = *(char *)((char *)ride + 0x25);
        if (*(short *)((char *)bloke + 0xe) == 0) {
            switch (*(unsigned char *)((char *)bloke + 0x60)) {
            case 0:
                *(unsigned char *)((char *)bloke + 0x62) |= 8;
                FUN_0042cec0(ride, &elem->id_x, &local_8);
                FUN_0042ce50(node, elem);
                *(int *)((char *)bloke + 0x24) = local_8;
                *(int *)((char *)bloke + 0x28) = local_4;
                cv = CalcMoveLine(*(int *)((char *)bloke + 0x68), *(int *)((char *)bloke + 0x6c),
                    *(int *)((char *)bloke + 0x24), local_4, (char *)bloke + 0x98);
                *(short *)((char *)bloke + 0xe) = 7;
                *(unsigned char *)((char *)bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)((char *)bloke + 0x60) += 1;
                break;
            case 1:
                if ((*(unsigned int *)((char *)node + 0x10) & 0x8000) == 0 && FUN_0042cf40(node, bloke) != 0) {
                    FUN_0042cf70(node);
                    cv = *(char *)((char *)node + 9) + 1;
                    *(char *)((char *)node + 9) = cv;
                    if (cv == 1) {
                        *(unsigned int *)((char *)node + 0x10) |= 0x8000;
                    }
                    bx = bx * 0x100 + -0x180;
                    byy = byy * 0x100 + 0x380;
                    *(int *)((char *)bloke + 0x24) = bx;
                    *(int *)((char *)bloke + 0x28) = byy;
                    cv = CalcMoveLine(*(int *)((char *)bloke + 0x68), *(int *)((char *)bloke + 0x6c), bx, byy, (char *)bloke + 0x98);
                    *(short *)((char *)bloke + 0xe) = 7;
                    *(unsigned char *)((char *)bloke + 0x73) = cv + 0x10;
                    NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                    *(char *)((char *)bloke + 0x60) += 1;
                }
                break;
            case 2:
                bx = (bx + -3) * 0x100;
                byy = (byy + 3) * 0x100;
                *(int *)((char *)bloke + 0x24) = bx;
                *(int *)((char *)bloke + 0x28) = byy;
                cv = CalcMoveLine(*(int *)((char *)bloke + 0x68), *(int *)((char *)bloke + 0x6c), bx, byy, (char *)bloke + 0x98);
                *(short *)((char *)bloke + 0xe) = 7;
                *(unsigned char *)((char *)bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)((char *)bloke + 0x60) += 1;
                break;
            case 3:
                bx = (bx + -4) * 0x100;
                byy = (byy + 3) * 0x100;
                *(int *)((char *)bloke + 0x24) = bx;
                *(int *)((char *)bloke + 0x28) = byy;
                cv = CalcMoveLine(*(int *)((char *)bloke + 0x68), *(int *)((char *)bloke + 0x6c), bx, byy, (char *)bloke + 0x98);
                *(short *)((char *)bloke + 0xe) = 7;
                *(unsigned char *)((char *)bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)((char *)bloke + 0x60) += 1;
                break;
            case 4:
                *(int *)((char *)bloke + 0x68) = (bx + -4) * 0x100;
                *(int *)((char *)bloke + 0x6c) = (byy + 3) * 0x100;
                idx = rand();
                *(unsigned int *)((char *)bloke + 0x58) = (idx & 0x1f) + 4;
                *(char *)((char *)bloke + 0x60) += 1;
                break;
            case 5:
                if (*(int *)((char *)bloke + 0x58) == 0) {
                    *(char *)((char *)elem->bloke + 0x60) += 1;
                }
                *(int *)((char *)bloke + 0x58) += -1;
                break;
            case 6:
                *(unsigned char *)((char *)bloke + 0x62) |= 0x80;
                BlokeSitAnim(bloke);
                BlokeSetFrame(bloke, 0);
                cv = *(char *)((char *)node + 8) + 1;
                *(char *)((char *)node + 8) = cv;
                *(char *)((char *)node + 9) += -1;
                if (cv == 1) {
                    *(unsigned char *)((char *)node + 10) = 1;
                    *(unsigned char *)((char *)node + 0xb) = 0;
                    *(unsigned int *)((char *)node + 0x10) |= 1;
                    *(int *)((char *)node + 0x14) = 0;
                    *(int *)((char *)node + 0x4) = 0;
                }
                *(int *)((char *)bloke + 0x58) = 8;
                *(char *)((char *)bloke + 0x60) += 1;
                Put3DBlokesOnRide((struct ViewportEntry *)DAT_006160d0, (unsigned char *)node, (int)*(char *)((char *)node + 0xb), (int *)DAT_006160e4);
                break;
            case 8:
                BlokeWalkAnim((struct Bloke *)bloke);
                *(unsigned short *)((char *)bloke + 0x62) &= 0xff7f;
                *(unsigned int *)((char *)bloke + 0x68) = (cx + idx) * 0x100 + 0x80;
                *(unsigned int *)((char *)bloke + 0x6c) = (cy + by) * 0x100 + 0x80;
                RemoveBlokeFromRide(ride, elem);
                *(unsigned short *)((char *)bloke + 0x62) &= 0xfff7;
                cv = *(char *)((char *)node + 10) + -1;
                *(char *)((char *)node + 10) = cv;
                if (cv == 0) {
                    *(unsigned char *)((char *)node + 8) = 0;
                    *(int *)((char *)node + 0x4) = 1;
                    *(unsigned int *)((char *)node + 0x10) &= 0xffff7fff;
                }
                if ((short)*(char *)((char *)node + 0x18) != *(short *)((char *)ride + 0x2e)) {
                    Ride_ClearFlagToNotLetAnyoneOn((unsigned char *)node);
                }
                break;
            }
        }
        elem = next;
    }
}
