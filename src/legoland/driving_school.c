#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bricks.h"
#include "driving_school.h"
#include "gamemap.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "objclass.h"
#include "print_sprite.h"
#include "render3d.h"
#include "ride_bloke.h"
#include "ride_queue.h"
#include "roads.h"
#include "sound_music.h"
#include "tilemap.h"

struct DSCursorSource {
    /* 0x00 */ unsigned char pad_0[0x1c];
    /* 0x1c */ unsigned int field_1c;
    /* 0x20 */ unsigned char pad_20[0x3c - 0x20];
    /* 0x3c */ int field_3c;
    /* 0x40 */ int field_40;
};

struct DSHead {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ struct DSCursorSource *field_c;
};

struct Node8 {
    unsigned char pad_0[8];
    void *next;
};

struct DSCarLayer {
    unsigned char pad_0[12];
    struct DSCarSub *field_c;
};

struct DSCarSub {
    unsigned char pad_0[20];
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[72];
    struct DSCarInner *field_64;
};

struct DSCarInner {
    unsigned char pad_0[16];
    unsigned int field_10;
};

struct CountNode {
    unsigned short field_0;
    unsigned char pad_2[2];
    int field_4;
    struct CountNode *next;
};

struct DSRoadElem {
    /* 0x00 */ unsigned char pad_0[0x8];
    /* 0x08 */ unsigned char flags;
    /* 0x09 */ unsigned char pad_9[0xc - 0x9];
    /* 0x0c */ struct ObjectCount *obj;
};

struct DSSampleConfig {
    /* 0x00 */ int field_0;
    /* 0x04 */ int field_4;
};

struct DSObjClass {
    /* 0x00 */ unsigned char pad_0[0x26];
    /* 0x26 */ short field_26;
    /* 0x28 */ unsigned char pad_28[0xc4 - 0x28];
    /* 0xc4 */ unsigned int field_c4;
};

struct DSBlokeNode {
    /* 0x00 */ struct DSBlokeNode *next;
    /* 0x04 */ short field_4;
};

struct DSRenderNode {
    /* 0x00 */ struct DSRenderNode *next;
    /* 0x04 */ unsigned char pad_4[0x8 - 0x4];
    /* 0x08 */ struct Bloke *field_8;
    /* 0x0c */ short field_c;
};

struct DSRenderSub {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ int field_c;
    /* 0x10 */ int field_10;
    /* 0x14 */ int field_14;
    /* 0x18 */ int field_18;
    /* 0x1c */ unsigned char pad_1c[0xcc - 0x1c];
    /* 0xcc */ struct DSRenderNode *field_cc;
};

struct DSRenderRoot {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ struct DSRenderSub *field_c;
};

#include "image_sprite.h"
#include "pumps.h"

// FUNCTION: LEGOLAND 0x004051a0
void FUN_004051a0(short param_1) {
    struct RideQueueEntry *node;
    struct RideQueueEntry *e0;
    struct RideQueueEntry *e1;
    struct RideQueueEntry *e2;
    struct RideQueueEntry *e3;

    for (node = DAT_004c11c4; node != NULL; node = node->field_4) {
        e0 = FUN_004125a0(node->x, node->y - 4);
        e1 = FUN_004125a0(node->x + 4, node->y);
        e2 = FUN_004125a0(node->x, node->y + 4);
        e3 = FUN_004125a0(node->x - 4, node->y);
        if ((node->field_14 & 0xf) == 6) {
            e2 = NULL;
            e3 = NULL;
            e0 = NULL;
        } else if (e0 != NULL && ((short)e0->field_8 != param_1 || e0->field_18 != NULL)) {
            e0 = NULL;
        }
        if (e1 != NULL && ((short)e1->field_8 != param_1 || e1->field_18 != NULL)) {
            e1 = NULL;
        }
        if (e2 != NULL && ((short)e2->field_8 != param_1 || e2->field_18 != NULL)) {
            e2 = NULL;
        }
        if (e3 != NULL && ((short)e3->field_8 != param_1 || e3->field_18 != NULL)) {
            e3 = NULL;
        }
        if (e0 != NULL) {
            e0->field_18 = node;
            e0->field_15 = node->field_15 + 1;
        }
        if (e1 != NULL) {
            e1->field_18 = node;
            e1->field_15 = node->field_15 + 1;
        }
        if (e2 != NULL) {
            e2->field_18 = node;
            e2->field_15 = node->field_15 + 1;
        }
        if (e3 != NULL) {
            e3->field_18 = node;
            e3->field_15 = node->field_15 + 1;
        }
        if (e0 != NULL) {
            e0->field_4 = DAT_004c11c8;
            DAT_004c11c8 = e0;
        }
        if (e1 != NULL) {
            e1->field_4 = DAT_004c11c8;
            DAT_004c11c8 = e1;
        }
        if (e2 != NULL) {
            e2->field_4 = DAT_004c11c8;
            DAT_004c11c8 = e2;
        }
        if (e3 != NULL) {
            e3->field_4 = DAT_004c11c8;
            DAT_004c11c8 = e3;
        }
    }
}

// FUNCTION: LEGOLAND 0x00405310
void FUN_00405310(unsigned int param_1) {
    struct RideQueueEntry *cur;
    struct RideQueueEntry *ret;
    struct RideQueueEntry *tmp;

    cur = DAT_004cbeac;
    while (cur != NULL) {
        if (cur->field_8 == (unsigned short)param_1) {
            cur->field_18 = NULL;
        }
        cur = cur->next;
    }

    ret = FUN_00412650((unsigned short)param_1);
    ret->field_15 = 0;
    ret->field_4 = NULL;

    DAT_004c11c4 = ret;
    DAT_004c11c8 = NULL;

    do {
        FUN_004051a0(param_1);
        tmp = DAT_004c11c8;
        DAT_004c11c4 = tmp;
        DAT_004c11c8 = NULL;
    } while (tmp != NULL);
}

// FUNCTION: LEGOLAND 0x00405370
void FUN_00405370(struct DSHead *param_1) {
    unsigned int lls;

    DAT_0082c694 = param_1->field_c;
    if (LLIDB_FindElement("DSCHOOL MAPPING", (unsigned int *)&param_1, 0) == 0) {
        DAT_0082c67c = LLIDB_LoadData(param_1);
    }
    if (LLIDB_FindElement("DSCHOOL BLUE CAR", (unsigned int *)&param_1, 0) == 0) {
        DAT_00830f9c = LLIDB_LoadData(param_1);
    }
    DAT_0082c694->field_1c |= 0x420;
    Load_FXList(DRIVING_SCHOOL_SFX, 6);
    // STRING: LEGOLAND 0x004b4524
    DAT_0082c6c0 = LoadSprite("DSchool Matte.lls", 1);
    // STRING: LEGOLAND 0x004b4510
    DAT_0082c6bc = LoadPalette((unsigned int)".\\3ddata\\blu.col");
    // STRING: LEGOLAND 0x004b44fc
    DAT_0082c6b8 = LoadPalette((unsigned int)".\\3ddata\\yel.col");
    // STRING: LEGOLAND 0x004b44e8
    DAT_0082c690 = LoadPalette((unsigned int)".\\3ddata\\red.col");
    // STRING: LEGOLAND 0x004b44d8
    DAT_00830f94 = LoadSprite("ds_car&m.lls", 1);
    if (DAT_00830f94 != NULL) {
        lls = GetLLSForSprite((struct SpriteLLS *)DAT_00830f94);
        if (lls != 0) {
            LLSStop(lls);
        }
    }
}

// FUNCTION: LEGOLAND 0x00405460
void FUN_00405460(void) {
    unsigned int local;

    // STRING: LEGOLAND 0x004b454c
    if (LLIDB_FindElement("DSCHOOL MAPPING", &local, 0) == 0) {
        LLIDB_UnLoadData(local);
    }

    // STRING: LEGOLAND 0x004b4538
    if (LLIDB_FindElement("DSCHOOL BLUE CAR", &local, 0) == 0) {
        LLIDB_UnLoadData(local);
    }

    while (DAT_004c10d4 != NULL) {
        FUN_00401c60(DAT_004c10d4);
    }

    FUN_00411bd0();

    while (DAT_004cbeac != NULL) {
        struct RideQueueEntry *next = DAT_004cbeac->next;
        free(DAT_004cbeac);
        DAT_004cbeac = next;
    }

    while (DAT_004c11bc != NULL) {
        void *next = ((struct Node8 *)DAT_004c11bc)->next;
        free(DAT_004c11bc);
        DAT_004c11bc = next;
    }

    Kill_FXList(DRIVING_SCHOOL_SFX, 6);
    KillSprite(DAT_0082c6c0);
    free(DAT_0082c6bc);
    free(DAT_0082c6b8);
    free(DAT_0082c690);
    KillSprite(DAT_00830f94);
    DAT_0082c694 = NULL;
}

// FUNCTION: LEGOLAND 0x00405570
void FUN_00405570(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0082c694;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
    DefaultCursor(&DAT_0082f760);
    memcpy(&DAT_00830b74, DAT_004b4440, 20);
    DAT_00830f88 |= 0x100;
    DefaultCursor(&DAT_0082c6e0);
    memcpy(DAT_0082c6e0.field_1414, DAT_004b4458, 20);
    DAT_0082c6e0.field_1828 |= 0x200;
    DefaultCursor(&DAT_0082df20);
    memcpy(DAT_0082df20.field_1414, DAT_004b4470, 20);
    EditCursor.field_1830 = (unsigned int)&DAT_0082f760;
    DAT_00830f90 = (unsigned int)&DAT_0082c6e0;
}

// FUNCTION: LEGOLAND 0x00405630
void FUN_00405630(unsigned int param_1, unsigned char *param_2) {
    unsigned char *src = param_2;
    unsigned char b0 = param_2[0];
    unsigned char b4 = param_2[4];
    unsigned int id;
    struct CountNode *node;
    struct Cursor *cursor;
    int x;
    int y;
    struct DSRoadElem *elem;

    *(unsigned char *)&param_2 = b0;
    *((unsigned char *)&param_2 + 1) = b4;
    AddBasicObject(param_1, (unsigned int)src);

    node = (struct CountNode *)malloc(sizeof(struct CountNode));
    id = (unsigned int)param_2;
    node->field_0 = id;
    node->next = (struct CountNode *)DAT_004c11bc;
    cursor = (struct Cursor *)EditCursor.field_1830;
    DAT_004c11bc = node;
    y = cursor->field_1408;
    x = cursor->field_1404;

    FUN_004132a0(id, x - 3, y - 4, 6, 1);
    FUN_004132a0(id, x + 1, y - 4, 0, 1);
    x += 5;
    FUN_004132a0(id, x, y - 4, 3, 1);
    FUN_004132a0(id, x, y, 0, 0);
    FUN_004132a0(id, x, y + 4, 0, 0);

    // STRING: LEGOLAND 0x004b455c
    elem = (struct DSRoadElem *)ElemID("Driving School Roads");
    if (elem != NULL && (elem->flags & 4) != 0) {
        IncrementObjectCount(elem->obj);
        IncrementObjectCount(elem->obj);
        IncrementObjectCount(elem->obj);
        IncrementObjectCount(elem->obj);
        IncrementObjectCount(elem->obj);
    }

    node->field_4 = 5;
    FUN_00405310(id);
}

// FUNCTION: LEGOLAND 0x00405740
void FUN_00405740(struct DSHead *param_1, unsigned int param_2, unsigned int param_3) {
    struct DSCursorSource *src = param_1->field_c;
    struct DSCursorSource *c694;
    unsigned int mapx;
    unsigned int mapy;

    memcpy(EditCursor.field_1414, (char *)src + 0x3c, 20);
    EditCursor.field_1830 = 0;
    EditCursor.field_1828 = 0x4408;
    ScreenToMapRef(param_2, &EditCursor.field_1404, param_3);

    c694 = DAT_0082c694;
    mapx = EditCursor.field_1404;
    memcpy(&DAT_00830b74, DAT_004b4440, 20);
    DAT_00830f90 = 0;
    mapy = EditCursor.field_1408;
    DAT_00830f88 = 0x4108;
    DAT_00830b64 = c694->field_3c + mapx;
    DAT_00830b68 = c694->field_40 + mapy;

    memcpy(DAT_0082c6e0.field_1414, DAT_004b4458, 20);
    DAT_0082c6e0.field_1830 = 0;
    DAT_0082c6e0.field_1828 = 0x4208;
    DAT_0082c6e0.field_1404 = c694->field_3c + mapx;
    DAT_0082c6e0.field_1408 = c694->field_40 + mapy;

    memcpy(DAT_0082df20.field_1414, DAT_004b4470, 20);
    DAT_0082df20.field_1830 = 0;
    DAT_0082df20.field_1828 = 0x5008;
    DAT_0082df20.field_1404 = c694->field_3c + mapx;
    DAT_0082df20.field_1408 = c694->field_40 + mapy;

    ValidateCursor(&DAT_0082df20, (unsigned int)src);
    ValidateCursor(&DAT_0082c6e0, (unsigned int)src);
    ValidateCursor(&DAT_0082f760, (unsigned int)src);
    ValidateCursor(&EditCursor, (unsigned int)src);

    EditCursor.field_1830 = (unsigned int)&DAT_0082f760;
    DAT_00830f90 = (unsigned int)&DAT_0082c6e0;
    DAT_0082c6e0.field_1830 = (unsigned int)&DAT_0082df20;
    FUN_0045f4d0(&EditCursor);
}

// FUNCTION: LEGOLAND 0x004058a0
void FUN_004058a0(unsigned int param_1, unsigned int param_2) {
    struct RideQueueEntry *node = DAT_004cbeac;

    BasicObjectDCalcCursor(param_1, param_2);
    DefaultCursor(&DAT_0082f760);
    memcpy(&DAT_00830b74, DAT_004b4bf0, 20);

    while (node != NULL) {
        if (node->field_8 == QueryObj) {
            DAT_00830b64 = node->x;
            DAT_00830b68 = node->y;
            FUN_0045f460(&DAT_0082f760);
            DAT_00830f88 = 0x18;
            BuildCursorPtr(&DAT_0082f760, 0, 0);
            RenderCursor(&DAT_0082f760);
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00405940
void FUN_00405940(int param_1, unsigned int param_2, unsigned int param_3) {
    struct RideQueueEntry *queue = DAT_004cbeac;
    struct CountNode *count = (struct CountNode *)DAT_004c11bc;
    struct DSBlokeNode *blokes = (struct DSBlokeNode *)DAT_004c10d4;
    short id = (short)param_2;

    StandardRemoveObject(param_1, param_2, param_3);
    DefaultCursor(&DAT_0082f760);
    memcpy(&DAT_00830b74, DAT_004b4bf0, 20);

    if ((short)count->field_0 == id) {
        DAT_004c11bc = count->next;
        free(count);
    } else {
        struct CountNode *prev = count;
        struct CountNode *cur = count->next;
        if (cur != NULL) {
            do {
                if ((short)cur->field_0 == id) {
                    prev->next = prev->next->next;
                    free(cur);
                    break;
                }
                prev = cur;
                cur = cur->next;
            } while (cur != NULL);
        }
    }

    FUN_00411ba0(QueryObj);

    while (queue != NULL) {
        struct RideQueueEntry *next = queue->next;
        if (queue->field_8 == QueryObj) {
            if (queue->field_14 & 0x10) {
                queue->field_14 &= 0xef;
                FUN_00413650(queue->field_8, queue->x, queue->y);
                AddBricks(((struct DSObjClass *)DAT_0082c678)->field_26);
            }
            DAT_00830b64 = queue->x;
            DAT_00830b68 = queue->y;
            StandardRemoveObject(((struct DSObjClass *)DAT_0082c684)->field_c4, queue->field_8, (unsigned int)&DAT_0082f760);
            FUN_004133e0(queue->x, queue->y);
        }
        queue = next;
    }

    AddBricks(((struct DSObjClass *)DAT_0082c684)->field_26 * 5);

    while (blokes != NULL) {
        struct DSBlokeNode *next = blokes->next;
        if (blokes->field_4 == (short)QueryObj) {
            FUN_00401c60(blokes);
        }
        blokes = next;
    }

    RemoveAllBlokesFromRide(*(unsigned int *)(param_1 + 0xc), param_2);
}

// FUNCTION: LEGOLAND 0x00405ad0
unsigned int *FUN_00405ad0(struct DSCarLayer *arg1, unsigned short arg2) {
    struct DSCarSub *sub = arg1->field_c;
    struct DSCarInner *inner = sub->field_64;

    DAT_0082c6a0 = (unsigned int)inner;
    DAT_0082c6a4 = sub->field_14;
    DAT_0082c6a8 = sub->field_18;
    DAT_0082c6ac = arg2;

    inner = sub->field_64;
    inner->field_10 |= 0x2000;

    return &DAT_0082c6a0;
}

// FUNCTION: LEGOLAND 0x00405b10
void FUN_00405b10(struct DSRenderRoot *param_1, unsigned int param_2, unsigned int param_3, unsigned char *param_4, unsigned int param_5, unsigned int param_6) {
    struct DSRenderSub *sub = param_1->field_c;
    struct DSRenderNode *node;
    struct Point ref;
    int bounds[4];
    int dx;
    int dy;

    ref.x = param_4[0];
    ref.y = param_4[1];
    for (node = sub->field_cc; node != NULL; node = node->next) {
        if (*(short *)param_4 == node->field_c) {
            unsigned char state = *((unsigned char *)node->field_8 + 0x60);
            if (state <= 1 || state >= 3) {
                IP_RenderBlokeIn3DNow(node->field_8);
            }
        }
    }
    GetTileBounds(&ref, bounds);
    if (DAT_0082c6c0 != NULL) {
        dx = sub->field_14;
        dy = sub->field_18;
        if (dx < 0) {
            dx = -(-dx >> 1);
        } else {
            dx = dx >> 1;
        }
        if (dy < 0) {
            dy = -(-dy >> 1);
        } else {
            dy = dy >> 1;
        }
        PrintSprite(DAT_0082c6c0, bounds[0] + dx, bounds[1] + dy, param_6, 0);
    }
}

// FUNCTION: LEGOLAND 0x00405bd0
void FUN_00405bd0(struct DSRenderRoot *param_1) {
    struct DSRenderSub *sub = param_1->field_c;
    struct DSRenderNode *node = sub->field_cc;
    struct DSSampleConfig params;
    struct DSSampleConfig params2;

    FUN_00402c10();
    FUN_00414440();

    while (node != NULL) {
        int bloke = (int)node->field_8;
        struct DSRenderNode *next = node->next;
        if (*(short *)(bloke + 0xe) == 0) {
            char dir;
            int frame;
            switch (*(unsigned char *)(bloke + 0x60)) {
            case 0:
                *(unsigned int *)(bloke + 0x24) = (sub->field_c + (unsigned int)((unsigned char *)node)[0xc]) * 0x100;
                frame = (((unsigned char *)node)[0xd] - 3 + sub->field_10) * 0x100;
                *(int *)(bloke + 0x28) = frame;
                dir = CalcMoveLine(*(int *)(bloke + 0x68), *(int *)(bloke + 0x6c), *(int *)(bloke + 0x24), frame, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                *(unsigned int *)(bloke + 0x58) = (rand() & 7) + 1;
                *(char *)(bloke + 0x60) += 1;
                break;
            case 1:
                if (*(int *)(bloke + 0x58) == 0) {
                    int count = FUN_00401c40(node->field_c);
                    unsigned int limit = FUN_00413970(node->field_c);
                    if ((unsigned int)(count * 5) < limit) {
                        int res = FUN_00401ae0(node->field_c, bloke);
                        if (res == 0) {
                            struct Sample *sample;
                            unsigned int r;
                            params.field_0 = 1;
                            params.field_4 = bloke;
                            PlayInstanceOfSample(DAT_004b4400, 0, 1, &params);
                            r = rand();
                            r &= 0x80000003;
                            if ((int)r < 0) {
                                r = (r - 1 | 0xfffffffc) + 1;
                            }
                            sample = PlayInstanceOfSample((&DAT_004b4400)[(r + 1) * 3], 1, 1, &params);
                            AdjustPSampleFreq(sample, 10);
                            BlokeSitAnim(bloke);
                            BlokeSetFrame(bloke, 0);
                            *(unsigned char *)(bloke + 0x62) |= 0x80;
                            *(char *)(bloke + 0x60) += 1;
                        } else if (res == -1) {
                            *(unsigned char *)(bloke + 0x60) = 3;
                        } else if (res == -2) {
                            *(unsigned int *)(bloke + 0x58) = (rand() & 0x1f) + 2;
                        }
                    }
                } else {
                    *(int *)(bloke + 0x58) -= 1;
                }
                break;
            case 3:
                *(unsigned short *)(bloke + 0x62) &= 0xff7f;
                BlokeWalkAnim(bloke);
                BlokeSetFrame(bloke, 0);
                *(unsigned int *)(bloke + 0x68) = ((unsigned int)((unsigned char *)node)[0xc] + sub->field_c) * 0x100;
                *(unsigned int *)(bloke + 0x6c) = (((unsigned char *)node)[0xd] - 3 + sub->field_10) * 0x100;
                *(unsigned int *)(bloke + 0x24) = ((unsigned int)((unsigned char *)node)[0xc] + sub->field_c) * 0x100 + 0x80;
                frame = ((unsigned int)((unsigned char *)node)[0xd] + sub->field_10) * 0x100 + 0x80;
                *(int *)(bloke + 0x28) = frame;
                dir = CalcMoveLine(*(int *)(bloke + 0x68), *(int *)(bloke + 0x6c), *(int *)(bloke + 0x24), frame, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = dir + 0x10;
                NewDirForAction(bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 4:
                RemoveBlokeFromRide(sub, node);
                *(unsigned short *)(bloke + 0x62) &= 0xfff7;
                params2.field_0 = 1;
                params2.field_4 = bloke;
                KillAllSamplesFromSource(&params2);
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x00405e70
int FUN_00405e70(void) {
    struct CountNode *countNode;
    struct RideQueueEntry *queue;
    struct PumpNode *pump;
    struct DSBlokeNode *bloke;
    int count;
    int buf[52];

    count = 0;
    for (countNode = (struct CountNode *)DAT_004c11bc; countNode != NULL; countNode = countNode->next) {
        count++;
    }
    SaveGameWrite(&count, 4);
    countNode = (struct CountNode *)DAT_004c11bc;
    while (count != 0) {
        count--;
        SaveGameWrite(countNode, 0xc);
        countNode = countNode->next;
    }

    count = 0;
    for (queue = DAT_004cbeac; queue != NULL; queue = queue->next) {
        count++;
    }
    SaveGameWrite(&count, 4);
    queue = DAT_004cbeac;
    while (count != 0) {
        count--;
        SaveGameWrite(queue, 0x20);
        queue = queue->next;
    }

    count = 0;
    for (pump = (struct PumpNode *)DAT_004cbea4; pump != NULL; pump = pump->next) {
        count++;
    }
    SaveGameWrite(&count, 4);
    pump = (struct PumpNode *)DAT_004cbea4;
    while (count != 0) {
        count--;
        SaveGameWrite(pump, 0x10);
        pump = pump->next;
    }

    count = 0;
    for (bloke = (struct DSBlokeNode *)DAT_004c10d4; bloke != NULL; bloke = bloke->next) {
        count++;
    }
    SaveGameWrite(&count, 4);
    bloke = (struct DSBlokeNode *)DAT_004c10d4;
    while (count != 0) {
        count--;
        memcpy(buf, bloke, 0xd0);
        buf[51] = GetBlokeNum(buf[51]);
        SaveGameWrite(buf, 0xd0);
        bloke = bloke->next;
    }

    return 1;
}

// FUNCTION: LEGOLAND 0x00406020
void FUN_00406020(unsigned short arg1, unsigned int arg2) {
    struct CountNode *current = (struct CountNode *)DAT_004c11bc;
    while (current != NULL) {
        if (current->field_0 == arg1) {
            if (current != NULL) {
                current->field_4 += arg2;
            }
            break;
        }
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x00406050
int FUN_00406050(void) {
    struct CountNode *current = (struct CountNode *)DAT_004c11bc;
    int max = 0;

    while (current != NULL) {
        if (current->field_4 > max) {
            max = current->field_4;
        }
        current = current->next;
    }

    return max;
}

// FUNCTION: LEGOLAND 0x00406070
int FUN_00406070(void) {
    struct CountNode *countPrev;
    struct CountNode *countNode;
    struct RideQueueEntry *queuePrev;
    struct RideQueueEntry *queueNode;
    struct PumpNode *pumpPrev;
    struct PumpNode *pumpNode;
    struct DSBlokeNode *blokePrev;
    struct DSBlokeNode *blokeNode;
    int count;

    DAT_004c11bc = NULL;
    SaveGameRead(&count, 4);
    countPrev = NULL;
    while (count-- != 0) {
        if (countPrev == NULL) {
            countNode = (struct CountNode *)malloc(0xc);
            DAT_004c11bc = countNode;
        } else {
            countNode = (struct CountNode *)malloc(0xc);
            countPrev->next = countNode;
        }
        SaveGameRead(countNode, 0xc);
        countPrev = countNode;
    }

    DAT_004cbeac = NULL;
    SaveGameRead(&count, 4);
    queuePrev = NULL;
    while (count-- != 0) {
        if (queuePrev == NULL) {
            queueNode = (struct RideQueueEntry *)malloc(0x20);
            DAT_004cbeac = queueNode;
        } else {
            queueNode = (struct RideQueueEntry *)malloc(0x20);
            queuePrev->next = queueNode;
        }
        SaveGameRead(queueNode, 0x20);
        queuePrev = queueNode;
    }

    DAT_004cbea4 = NULL;
    SaveGameRead(&count, 4);
    pumpPrev = NULL;
    while (count-- != 0) {
        if (pumpPrev == NULL) {
            pumpNode = (struct PumpNode *)malloc(0x10);
            DAT_004cbea4 = pumpNode;
        } else {
            pumpNode = (struct PumpNode *)malloc(0x10);
            pumpPrev->next = pumpNode;
        }
        SaveGameRead(pumpNode, 0x10);
        pumpPrev = pumpNode;
    }

    DAT_004c10d4 = NULL;
    SaveGameRead(&count, 4);
    blokePrev = NULL;
    while (count-- != 0) {
        if (blokePrev == NULL) {
            blokeNode = (struct DSBlokeNode *)malloc(0xd0);
            DAT_004c10d4 = blokeNode;
        } else {
            blokeNode = (struct DSBlokeNode *)malloc(0xd0);
            blokePrev->next = blokeNode;
        }
        SaveGameRead(blokeNode, 0xd0);
        *(int *)((char *)blokeNode + 0xcc) = GetBlokePtr(*(int *)((char *)blokeNode + 0xcc));
        blokePrev = blokeNode;
    }

    count = -1;
    for (countNode = (struct CountNode *)DAT_004c11bc; countNode != NULL; countNode = countNode->next) {
        FUN_00405310(countNode->field_0);
    }

    return 1;
}
