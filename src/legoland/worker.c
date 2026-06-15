#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "bloke_ai.h"
#include "bricks.h"
#include "controller.h"
#include "debug_alloc.h"
#include "gamemap.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "objclass.h"
#include "print_sprite.h"
#include "sound_music.h"
#include "tilemap.h"
#include "worker.h"

struct LegoConfig;

struct WorkOrder {
    /* 0x00 */ struct WorkOrder *next;
    /* 0x04 */ struct EditObject *var_4;
    /* 0x08 */ int var_8;
    /* 0x0c */ int var_c;
    /* 0x10 */ int *var_10;
    /* 0x14 */ int var_14;
    /* 0x18 */ int var_18;
    /* 0x1c */ int var_1c;
    /* 0x20 */ unsigned char var_20;
    /* 0x21 */ unsigned char pad_21[3];
    /* 0x24 */ int var_24;
    /* 0x28 */ int var_28;
    /* 0x2c */ unsigned char var_2c;
    /* 0x2d */ unsigned char pad_2d[3];
    /* 0x30 */ int var_30;
    /* 0x34 */ float var_34;
    /* 0x38 */ float var_38;
};

struct Rect {
    int minx;
    int miny;
    int maxx;
    int maxy;
};

struct RepairOrder {
    /* 0x00 */ struct RepairOrder *next;
    /* 0x04 */ int footprint[5];
    /* 0x18 */ int var_18;
    /* 0x1c */ int var_1c;
    /* 0x20 */ float var_20;
    /* 0x24 */ float var_24;
};

struct BlokeSave {
    /* 0x00 */ unsigned short field_0;
    /* 0x02 */ unsigned short field_2;
    /* 0x04 */ unsigned short field_4;
    /* 0x06 */ unsigned char pad_6[0x10 - 0x06];
    /* 0x10 */ unsigned int field_10;
    /* 0x14 */ unsigned int field_14;
    /* 0x18 */ unsigned int field_18;
    /* 0x1c */ unsigned int field_1c;
    /* 0x20 */ unsigned int field_20;
    /* 0x24 */ unsigned int field_24;
    /* 0x28 */ unsigned int field_28[10];
    /* 0x50 */ unsigned int field_50;
    /* 0x54 */ unsigned char field_54;
    /* 0x55 */ unsigned char pad_55[1];
    /* 0x56 */ unsigned short field_56;
    /* 0x58 */ unsigned char field_58;
    /* 0x59 */ unsigned char field_59;
    /* 0x5a */ unsigned char field_5a;
    /* 0x5b */ unsigned char pad_5b[1];
    /* 0x5c */ unsigned int field_5c;
    /* 0x60 */ unsigned int field_60;
    /* 0x64 */ unsigned short field_64;
    /* 0x66 */ unsigned char field_66;
    /* 0x67 */ unsigned char field_67;
    /* 0x68 */ unsigned char field_68;
    /* 0x69 */ unsigned char field_69;
    /* 0x6a */ unsigned char pad_6a[2];
    /* 0x6c */ unsigned int field_6c[5];
    /* 0x80 */ unsigned int field_80;
    /* 0x84 */ unsigned int field_84;
    /* 0x88 */ unsigned int field_88;
    /* 0x8c */ unsigned int field_8c;
    /* 0x90 */ unsigned int field_90;
    /* 0x94 */ unsigned int field_94;
    /* 0x98 */ unsigned int field_98;
    /* 0x9c */ unsigned int field_9c;
    /* 0xa0 */ unsigned int field_a0;
    /* 0xa4 */ unsigned int field_a4;
    /* 0xa8 */ unsigned int field_a8;
    /* 0xac */ unsigned int field_ac;
    /* 0xb0 */ unsigned int field_b0[9];
    /* 0xd4 */ unsigned int field_d4;
    /* 0xd8 */ unsigned int field_d8;
};

// FUNCTION: LEGOLAND 0x00499530
LEGO_EXPORT void LoadWorkerInterfaceGFX(void) {
    // STRING: LEGOLAND 0x004b9bd0
    DAT_0079abfc = (void *)ElemID("NORMAL PATH TILES"); /* TODO: fold — ElemID handle (uint) stored as void* */
}

// FUNCTION: LEGOLAND 0x00499550
int FUN_00499550(void) {
    return DAT_0079a8bc;
}

// FUNCTION: LEGOLAND 0x00499560
int FUN_00499560(void) {
    return DAT_0079a8cc;
}

// FUNCTION: LEGOLAND 0x00499570
void *FUN_00499570(void) {
    struct WorkOrder *node = calloc(0x3c, 1);
    // STRING: LEGOLAND 0x004bff88
    DBPrintf("Allocated Workorder %x\n", node);
    if (DAT_0079a8b4 == 0) {
        DAT_0079a8b4 = node;
        DAT_0079a8b0 = node;
        DAT_0079a8b8++;
    } else {
        *(struct WorkOrder **)DAT_0079a8b4 = node;
        DAT_0079a8b4 = node;
        DAT_0079a8b8++;
    }
    return node;
}

// FUNCTION: LEGOLAND 0x004995d0
void *FUN_004995d0(void) {
    struct WorkOrder *node = calloc(0x3c, 1);
    if (DAT_0079a8c4 == 0) {
        DAT_0079a8c4 = node;
        DAT_0079a8c8++;
        DAT_0079a8c0 = node;
        return node;
    }
    *(struct WorkOrder **)DAT_0079a8c4 = node;
    DAT_0079a8c8++;
    DAT_0079a8c4 = node;
    return node;
}

// FUNCTION: LEGOLAND 0x00499620
void FUN_00499620(void *order) {
    struct WorkOrder *o = (struct WorkOrder *)order;
    struct Rect *rect;

    if (o->var_2c == 1) {
        rect = (struct Rect *)o->var_10;
        if (o->var_24 < (rect->maxx - rect->minx) + 1) {
            o->var_24++;
        } else {
            o->var_2c = 7;
        }
    }
    if (o->var_2c == 7) {
        rect = (struct Rect *)o->var_10;
        if (o->var_28 < (rect->maxy - rect->miny) + 1) {
            o->var_28++;
        } else {
            o->var_2c = 5;
        }
    }
    if (o->var_2c == 5) {
        if (o->var_24 != -1) {
            o->var_24--;
            return;
        }
        o->var_2c = 3;
        return;
    }
    if (o->var_2c == 3) {
        if (o->var_28 != -1) {
            o->var_28--;
            return;
        }
        o->var_2c = 1;
    }
}

// FUNCTION: LEGOLAND 0x004996a0
int FUN_004996a0(struct WorkOrder *order) {
    struct Rect *rect;
    struct MapElement *row;
    struct MapElement *cell;
    int x;
    int y;
    int maxy;

    rect = (struct Rect *)order->var_10;
    maxy = rect->maxy;
    x = order->var_24 + order->var_8 + rect->minx + maxy;
    y = order->var_c - order->var_28 + maxy;

    if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
        row = GameMap[y];
        cell = (struct MapElement *)((char *)row + x * 20);
        if (row != 0) {
            if ((cell->flags & 0x88) == 0) {
                return 1;
            }
            return cell->field_0 == (unsigned int)DAT_007fd624;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00499720
void FUN_00499720(struct WorkOrder *order) {
    int saved_24;
    int saved_28;
    int done;

    FUN_00499620(order);
    saved_24 = order->var_24;
    saved_28 = order->var_28;

    while (1) {
        done = FUN_004996a0(order);
        FUN_00499620(order);
        if (order->var_24 == saved_24 && order->var_28 == saved_28) {
            break;
        }
        if (done != 0) {
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x00499760
void FUN_00499760(struct WorkOrder *order, float value) {
    order->var_38 = value;
    order->var_34 = value * FLOAT_004ab480;
}

// FUNCTION: LEGOLAND 0x00499780
struct WorkOrder *FUN_00499780(struct EditObject *obj, int *coords, int mode) {
    struct WorkOrder *order;
    struct ObjClass *cls;
    unsigned char modeb;
    float ratio;

    if (DAT_0079a8b8 >= 0xe1) {
        return 0;
    }

    order = FUN_00499570();
    modeb = (unsigned char)mode;
    order->var_4 = obj;
    order->var_8 = coords[0];
    order->var_c = coords[1];
    order->var_18 = 0;
    order->var_10 = malloc(0x14);
    cls = *(struct ObjClass **)((char *)obj + 0xc);
    order->var_14 = 1;
    order->var_20 = modeb;
    order->var_2c = 1;
    order->var_28 = 0xffffffff;

    memcpy(order->var_10, &cls->field_3c, 20);

    FUN_00499720(order);

    if (mode == 2) {
        ratio = (float)GetObjCost((struct CostInfo *)cls) / (float)*((unsigned char *)cls + 0x2c);
        FUN_00499760(order, ratio);
    }

    return order;
}

struct FootNode {
    /* 0x00 */ int rect[4];
    /* 0x10 */ struct FootNode *next;
};

// FUNCTION: LEGOLAND 0x00499830
struct WorkOrder *FUN_00499830(struct EditObject *obj, int *coords, int mode) {
    struct WorkOrder *order;
    struct ObjClass *cls;
    struct Cursor *cursor;
    struct FootNode *node;
    int *dst;
    int *src;
    int dx;
    int dy;
    int x;
    int y;
    int i;
    unsigned char level;

    if (0xe0 < DAT_0079a8c8) {
        return 0;
    }
    order = FUN_004995d0();
    order->var_4 = obj;
    order->var_8 = coords[0];
    order->var_c = coords[1];
    order->var_18 = 0;
    cls = *(struct ObjClass **)((char *)obj + 0xc);
    if (mode == 2) {
        order->var_10 = malloc(0x14);
        order->var_14 = 1;
        src = &cls->field_3c;
        dst = order->var_10;
        for (i = 5; i != 0; i--) {
            *dst++ = *src++;
        }
    } else {
        cursor = &EditCursor;
        order->var_14 = 0;
        do {
            if ((cursor->field_1828 & 0x3000) == 0) {
                for (node = (struct FootNode *)&cursor->field_1414; node != 0; node = node->next) {
                    order->var_14++;
                }
            }
            cursor = (struct Cursor *)cursor->field_1830;
        } while (cursor != 0);
        order->var_10 = malloc(order->var_14 * 0x14);
        order->var_14 = 0;
        cursor = &EditCursor;
        do {
            if ((cursor->field_1828 & 0x3000) == 0) {
                dx = cursor->field_1404 - order->var_8;
                dy = cursor->field_1408 - order->var_c;
                for (node = (struct FootNode *)&cursor->field_1414; node != 0; node = node->next) {
                    src = (int *)node;
                    dst = (int *)((char *)order->var_10 + order->var_14 * 0x14);
                    for (i = 5; i != 0; i--) {
                        *dst++ = *src++;
                    }
                    dst = (int *)((char *)order->var_10 + order->var_14 * 0x14);
                    dst[0] += dx;
                    dst[1] += dy;
                    dst[2] += dx;
                    dst[3] += dy;
                    order->var_14++;
                }
            }
            cursor = (struct Cursor *)cursor->field_1830;
        } while (cursor != 0);
    }
    order->var_20 = (unsigned char)mode;
    order->var_2c = 1;
    order->var_28 = 0xffffffff;
    FUN_00499720(order);
    if (mode == 2) {
        struct MapElement *cell;
        x = coords[0];
        if (x < 0 || lpConfig->width <= x || (y = coords[1], y < 0) || lpConfig->height <= y) {
            cell = 0;
        } else {
            cell = (struct MapElement *)((char *)GameMap[y] + x * 0x14);
        }
        level = *((unsigned char *)cell + 0x11);
        FUN_00499760(order, (float)GetObjRepairCost((unsigned int)cls, level) /
                                (float)(int)(*((unsigned char *)cls + 0x2c) - level));
    }
    return order;
}

// FUNCTION: LEGOLAND 0x00499a70
unsigned __int64 FUN_00499a70(struct WorkOrder *order) {
    int *rect;
    int minx;
    int miny;
    int dx;
    int dy;
    union {
        unsigned __int64 q;
        int i[2];
    } result;

    rect = order->var_10;
    minx = rect[0];
    miny = rect[1];
    dx = rect[2] - minx;
    dy = rect[3] - miny;
    if (dx >= dy) {
        result.i[0] = (dx + (order->var_8 + minx) * 2) * 0x80;
        result.i[1] = (order->var_c + miny) * 0x100;
    } else {
        result.i[0] = (order->var_8 + minx) * 0x100;
        result.i[1] = (dy + (order->var_c + miny) * 2) * 0x80;
    }
    return result.q;
}

// FUNCTION: LEGOLAND 0x00499ac0
void FUN_00499ac0(struct Worker *worker, struct WorkOrder *order) {
    worker->var_50 = order;
    if (order->var_20 == 1) {
        worker->var_2c = (order->var_10[2] + order->var_8 * 2 + order->var_10[0]) * 0x80;
        worker->var_30 = (order->var_10[3] + order->var_c * 2 + order->var_10[1]) * 0x80;
    } else {
        worker->var_2c = (order->var_24 + order->var_8 + order->var_10[0]) * 0x100;
        worker->var_30 = ((order->var_10[3] - order->var_28) + order->var_c) * 0x100;
    }
    order->var_1c = (int)worker;
    order->var_18 = 1;
    if (order->var_20 == 1) {
        NewLongTermAction((struct Bloke *)worker, 0x12);
        return;
    }
    NewLongTermAction((struct Bloke *)worker, 0x15);
}

// FUNCTION: LEGOLAND 0x00499b60
void FUN_00499b60(struct Worker *worker, struct WorkOrder *order) {
    int var_30;
    union {
        unsigned __int64 q;
        int i[2];
    } key;

    worker->var_50 = order;
    if (order->var_20 == 1) {
        key.q = FUN_00499a70(order);
        var_30 = key.i[1];
        worker->var_2c = key.i[0];
    } else {
        worker->var_2c = (order->var_24 + order->var_8 + order->var_10[0]) * 0x100;
        var_30 = ((order->var_c - order->var_28) + order->var_10[3]) * 0x100;
    }
    worker->var_30 = var_30;
    order->var_1c = (int)worker;
    order->var_18 = 1;
    if (order->var_20 == 1) {
        NewLongTermAction((struct Bloke *)worker, 0x13);
        return;
    }
    NewLongTermAction((struct Bloke *)worker, 0x16);
}

// FUNCTION: LEGOLAND 0x00499be0
void *FUN_00499be0(void *list, void *params) {
    struct Worker *result = 0;
    int bestDist = 0x7fffffff;

    if (list != 0) {
        struct Worker *cur = list;
        do {
            int *fields = (int *)cur;
            if (fields[6] == 0) {
                int dx = (((int *)params)[0x1a] >> 8) - fields[2];
                int dy = (((int *)params)[0x1b] >> 8) - fields[3];
                int dist2 = dx * dx + dy * dy;
                if (dist2 < bestDist) {
                    bestDist = dist2;
                    result = cur;
                }
            }
            cur = cur->next;
        } while (cur != 0);
    }

    return result;
}

// FUNCTION: LEGOLAND 0x00499c40
void *FUN_00499c40(int *point) {
    struct Worker *current = GardenerList;
    struct Worker *best = NULL;
    int minDist = 0x7fffffff;

    while (current != NULL) {
        if (current->flags_c == 0x10) {
            int *fields = (int *)current;
            int dx = (fields[0x1a] >> 8) - point[0];
            int dy = (fields[0x1b] >> 8) - point[1];
            int dist = dx * dx + dy * dy;
            if (dist < minDist) {
                minDist = dist;
                best = current;
            }
            current = current->next;
            continue;
        }
        current = current->next;
    }
    return best;
}

// FUNCTION: LEGOLAND 0x00499d00
int FUN_00499d00(void *arg) {
    struct WorkOrder *p = FUN_00499be0(DAT_0079a8b0, arg);
    if (p == 0) {
        return 0;
    }
    FUN_00499ac0(arg, p);
    return *(signed char *)&p->var_20;
}

// FUNCTION: LEGOLAND 0x00499d30
int FUN_00499d30(void *arg) {
    struct WorkOrder *p = FUN_00499be0(DAT_0079a8c0, arg);
    if (p == 0) {
        return 0;
    }
    FUN_00499b60(arg, p);
    return *(signed char *)&p->var_20;
}

// FUNCTION: LEGOLAND 0x00499d60
void FUN_00499d60(struct WorkOrder *order) {
    struct WorkOrder *cur;
    struct WorkOrder *next;

    cur = DAT_0079a8b0;
    if (DAT_0079a8b0 == 0) {
        return;
    }
    // STRING: LEGOLAND 0x004c0034
    DBPrintf("unlinking gardener order %x at (%d, %d)\n", order, order->var_8, order->var_c);
    if (DAT_0079a8b4 == order) {
        // STRING: LEGOLAND 0x004c0014
        DBPrintf("   Last in list,  Next = %x\n", order->next);
    }
    if (DAT_0079a8b0 == order) {
        // STRING: LEGOLAND 0x004bfff4
        DBPrintf("   First in list, Last = %x\n", DAT_0079a8b4);
        DAT_0079a8b0 = order->next;
        if (DAT_0079a8b0 == 0) {
            DAT_0079a8b4 = 0;
        }
    } else {
        do {
            next = cur->next;
            if (next == order) {
                if (cur != 0) {
                    cur->next = order->next;
                    if (order->next == 0) {
                        DAT_0079a8b4 = cur;
                    }
                    goto done;
                }
                break;
            }
            cur = next;
        } while (next != 0);
        // STRING: LEGOLAND 0x004bffa0
        DBPrintf("    Work order not found (%x) at (%d,%d)", order->var_8, order->var_c);
        return;
    }
done:
    // STRING: LEGOLAND 0x004bffcc
    DBPrintf("    Work orders START (%x), END (%x)\n", DAT_0079a8b0, DAT_0079a8b4);
}

// FUNCTION: LEGOLAND 0x00499e30
void FUN_00499e30(struct WorkOrder *order) {
    FUN_00499d60(order);
    free(order->var_10);
    free(order);
    DAT_0079a8b8--;
}

// FUNCTION: LEGOLAND 0x00499e60
void FUN_00499e60(struct WorkOrder *order) {
    order->var_18 = 0;
    if (order->next == 0) {
        return;
    }
    if (DAT_0079a8b0 == 0) {
        return;
    }
    FUN_00499d60(order);
    ((struct WorkOrder *)DAT_0079a8b4)->next = order;
    DAT_0079a8b4 = order;
    if (DAT_0079a8b0 == 0) {
        DAT_0079a8b0 = order;
    }
    order->next = 0;
}

// FUNCTION: LEGOLAND 0x00499eb0
void FUN_00499eb0(struct WorkOrder *order) {
    struct WorkOrder *cur;
    struct WorkOrder *next;

    if (DAT_0079a8c0 == 0) {
        return;
    }
    cur = DAT_0079a8c0;
    if (cur == order) {
        DAT_0079a8c8--;
        DAT_0079a8c0 = order->next;
        if (DAT_0079a8c0 == 0) {
            DAT_0079a8c4 = 0;
        }
        free(order->var_10);
        free(order);
        return;
    }
    while (next = cur->next, next != order) {
        cur = next;
        if (next == 0) {
            return;
        }
    }
    if (cur != 0) {
        DAT_0079a8c8--;
        cur->next = order->next;
        if (order->next == 0) {
            DAT_0079a8c4 = cur;
        }
        free(order->var_10);
        free(order);
    }
}

// FUNCTION: LEGOLAND 0x00499f40
void FUN_00499f40(struct WorkOrder *order) {
    struct WorkOrder *scan;
    struct WorkOrder *prev;
    struct WorkOrder *head;
    struct WorkOrder *onext;

    scan = DAT_0079a8c0;
    onext = order->next;
    order->var_18 = 0;
    if (onext != 0 && DAT_0079a8c0 != 0) {
        head = onext;
        if (DAT_0079a8c0 != order) {
            do {
                prev = scan;
                head = DAT_0079a8c0;
                if (prev == 0) {
                    goto relink;
                }
                scan = prev->next;
            } while (prev->next != order);
            if (prev != 0) {
                prev->next = onext;
                head = DAT_0079a8c0;
            }
        }
    relink:
        DAT_0079a8c0 = head;
        ((struct WorkOrder *)DAT_0079a8c4)->next = order;
        DAT_0079a8c4 = order;
        if (DAT_0079a8c0 == 0) {
            DAT_0079a8c0 = order;
        }
        order->next = 0;
    }
}

// FUNCTION: LEGOLAND 0x00499fb0
void FUN_00499fb0(void) {
    struct Worker *current = GardenerList;
    if (current != NULL) {
        struct Worker *next;
        do {
            current->ticks++;
            next = current->next;
            if (current->state == 0) {
                /* Worker is the same record as struct Bloke, but it cannot be
                   merged into one non-union struct: worker uses 0x34/0x38 as a
                   (speed_x, speed_y) float pair (FUN_00499760) while the bloke
                   view uses 0x36 as a byte (progress) — the float overlaps the
                   byte, so the cast to the AI view stays. */
                DoHighLevelAI((struct Bloke *)current);
            }
            if (current->state != 0) {
                DoLowLevelAI(current);
            }
            UpdatePerson((struct Person *)current);
            if (current->progress == 100) {
                RemoveAGardener(current);
            }
            current = next;
        } while (current != NULL);
    }
}

// FUNCTION: LEGOLAND 0x0049a010
void FUN_0049a010(void) {
    struct Worker *node = MechanicList;
    while (node != 0) {
        struct Worker *next = node->next;
        node->ticks++;
        if (node->state == 0) {
            DoHighLevelAI((struct Bloke *)node);
        }
        if (node->state != 0) {
            DoLowLevelAI(node);
        }
        UpdatePerson((struct Person *)node);
        if (node->progress == 100) {
            RemoveAMechanic(node);
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x0049a070
LEGO_EXPORT void ControlWorkers(void) {
    FUN_0049a010();
    FUN_00499fb0();
}

// FUNCTION: LEGOLAND 0x0049a080
LEGO_EXPORT void RenderWorkers(void) {
    struct Worker *esi = GardenerList;
    if (esi) {
        do {
            if ((esi->flags & 0x20) == 0) {
                SortBlokeIn3D((struct Bloke *)esi);
            }
            esi = esi->next;
        } while (esi);
    }

    esi = MechanicList;
    if (esi) {
        do {
            if (esi->var_46 != 0 && (esi->flags & 0x20) == 0) {
                SortBlokeIn3D((struct Bloke *)esi);
            }
            esi = esi->next;
        } while (esi);
    }
}

struct RidePassenger {
    /* 0x00 */ struct RidePassenger *next;
    /* 0x04 */ int field_4;
    /* 0x08 */ struct Worker *worker;
    /* 0x0c */ short field_c;
    /* 0x0e */ unsigned char pad_e[2];
    /* 0x10 */ struct Person *person;
};

// FUNCTION: LEGOLAND 0x0049a0d0
LEGO_EXPORT void PutWorkerOnRide(struct Worker *worker, int *object) {
    struct RidePassenger *passenger;

    passenger = malloc(sizeof(struct RidePassenger));
    if (passenger != 0) {
        passenger->next = 0;
        passenger->field_4 = 0;
        passenger->worker = 0;
        passenger->field_c = 0;
        passenger->person = 0;
        passenger->worker = worker;
        passenger->person = worker->field_4;
        worker->flags |= 0x20;
        passenger->field_c = (short)object[1];
        PutBlokeInList(*(struct BlokeList **)(*(char **)object + 0xc), (struct Bloke *)passenger);
    }
}

// FUNCTION: LEGOLAND 0x0049a120
int FUN_0049a120(void) {
    if (GetBrickCount() < 30) {
        return 0;
    }
    if (DAT_0079a8bc >= 15) {
        return 0;
    }
    UseBricks(30);
    return 1;
}

// FUNCTION: LEGOLAND 0x0049a150
LEGO_EXPORT void RefundGardener(void) {
    AddBricks(30);
}

// FUNCTION: LEGOLAND 0x0049a160
int FUN_0049a160(void) {
    if (GetBrickCount() < 30) {
        return 0;
    }
    if (DAT_0079a8cc >= 15) {
        return 0;
    }
    UseBricks(30);
    return 1;
}

// FUNCTION: LEGOLAND 0x0049a190
LEGO_EXPORT void RefundMechanic(void) {
    AddBricks(30);
}

// FUNCTION: LEGOLAND 0x0049a1a0
LEGO_EXPORT struct Worker *GenerateGardener(int *coords, int param_2) {
    struct Worker *worker;
    int x;
    int y;
    int *cell;
    unsigned short action;

    if (DAT_0079a8bc >= 0xf) {
        return 0;
    }
    // STRING: LEGOLAND 0x004c00c4
    DBPrintf("Generating Gardener\n");
    worker = (struct Worker *)NewBlokeWOList((void *)2);
    cell = 0;
    if (worker == 0) {
        // STRING: LEGOLAND 0x004c0060
        DBPrintf("   Failed to Generate Gardener\n");
        return 0;
    }
    worker->var_70 = 0;
    worker->var_72 = 0;
    worker->var_74 = 0;
    worker->var_75 = 1;
    worker->var_7f = 0x18;
    worker->next = GardenerList;
    DAT_0079a8bc++;
    GardenerList = worker;
    if (param_2 == 0) {
        // STRING: LEGOLAND 0x004c0080
        DBPrintf("   Gardener Generated at (%d,%d)\n", coords[0], coords[1]);
        action = 0x10;
        worker->var_68 = coords[0] << 8;
        worker->var_6c = coords[1] << 8;
    } else {
        x = coords[0];
        if (-1 < x && x < (int)lpConfig->width && (y = coords[1], -1 < y) &&
            y < (int)lpConfig->height) {
            cell = (int *)((char *)GameMap[y] + x * 0x14);
        }
        // STRING: LEGOLAND 0x004c00a4
        DBPrintf("   Gardener Generated in hut\n");
        PutWorkerOnRide(worker, cell);
        x = coords[0];
        coords[0] = x - 2;
        worker->var_68 = (x - 2) * 0x100;
        worker->var_24 = (x - 2) * 0x100;
        coords[1]++;
        worker->var_6c = coords[1] << 8;
        worker->var_28 = coords[1] << 8;
        action = 5;
    }
    NewLongTermAction((struct Bloke *)worker, action);
    DAT_00668610 |= 0x80;
    return worker;
}

// FUNCTION: LEGOLAND 0x0049a2d0
LEGO_EXPORT void RemoveAGardener(struct Worker *worker) {
    struct Worker *prev;
    struct Worker *cur;
    struct Worker *next;

    prev = GardenerList;
    if (GardenerList == 0) {
        return;
    }
    if (GardenerList == worker) {
        GardenerList = ((struct Worker *)GardenerList)->next;
        free(prev);
        DAT_0079a8bc--;
        return;
    }
    next = ((struct Worker *)GardenerList)->next;
    while (cur = next, cur != 0) {
        if (cur == worker) {
            prev->next = cur->next;
            free(cur);
            DAT_0079a8bc--;
            return;
        }
        prev = cur;
        next = cur->next;
    }
    DAT_00668610 |= 0x80;
}

// FUNCTION: LEGOLAND 0x0049a340
LEGO_EXPORT struct Worker *GenerateMechanic(int *coords, int param_2) {
    struct Worker *worker;
    int x;
    int y;
    int *cell;

    if (DAT_0079a8cc > 0xe) {
        return 0;
    }
    worker = (struct Worker *)NewBlokeWOList((void *)3);
    worker->var_70 = 0;
    worker->var_72 = 0;
    worker->var_74 = 0;
    worker->var_75 = 1;
    worker->var_7f = 0x18;
    worker->next = MechanicList;
    worker->var_46 = 1;
    DAT_0079a8cc++;
    MechanicList = worker;
    if (param_2 != 0) {
        x = coords[0];
        if (x < 0 || x >= (int)lpConfig->width || (y = coords[1], y < 0) ||
            y >= (int)lpConfig->height) {
            cell = 0;
        } else {
            cell = (int *)((char *)GameMap[y] + x * 0x14);
        }
        PutWorkerOnRide(worker, cell);
        x = coords[0] * 0x100 - 0x80;
        worker->var_68 = x;
        worker->var_24 = x;
        y = coords[1];
        worker->var_6c = y << 8;
        worker->var_28 = y << 8;
        NewLongTermAction((struct Bloke *)worker, 5);
        return worker;
    }
    worker->var_68 = coords[0] << 8;
    worker->var_6c = coords[1] << 8;
    NewLongTermAction((struct Bloke *)worker, 0x11);
    return worker;
}

// FUNCTION: LEGOLAND 0x0049a430
LEGO_EXPORT void RemoveAMechanic(struct Worker *worker) {
    struct Worker *cur;
    struct Worker *prev;
    struct Worker *target;

    if (MechanicList == 0) {
        return;
    }
    if (MechanicList == worker) {
        target = MechanicList;
        MechanicList = ((struct Worker *)MechanicList)->next;
    } else {
        cur = ((struct Worker *)MechanicList)->next;
        prev = MechanicList;
        if (((struct Worker *)MechanicList)->next == 0) {
            return;
        }
        while (target = cur, target != worker) {
            cur = target->next;
            prev = target;
            if (target->next == 0) {
                return;
            }
        }
        prev->next = target->next;
    }
    free(target);
    DAT_0079a8cc--;
}

// FUNCTION: LEGOLAND 0x0049a480
LEGO_EXPORT void Gardener_Idle(struct Worker *worker) {
    worker->state = 0xe;
    FUN_00499d00(worker);
}

// FUNCTION: LEGOLAND 0x0049a4a0
void FUN_0049a4a0(struct Worker *worker) {
    worker->state = 0xe;
}

// FUNCTION: LEGOLAND 0x0049a4b0
LEGO_EXPORT void Mechanic_Idle(struct Worker *worker) {
    worker->state = 0xe;
    FUN_00499d30(worker);
}

// FUNCTION: LEGOLAND 0x0049a4d0
void FUN_0049a4d0(struct Worker *worker) {
    worker->state = 0xe;
}

// FUNCTION: LEGOLAND 0x0049a4e0
LEGO_EXPORT void Gardener_Build(struct Worker *worker) {
    char dir;
    int result;
    int dx;
    int dy;
    int dest[2];
    int coords[2];
    struct WorkOrder *order;

    switch (worker->var_60) {
    case 0:
        worker->var_24 = worker->var_2c;
        worker->var_28 = worker->var_30;
        dir = CalcMoveLine(worker->var_68, worker->var_6c, worker->var_2c, worker->var_30,
                           &worker->var_98);
        worker->state = 0xc;
        worker->var_73 = dir + 0x10;
        NewDirForAction(worker, (worker->var_73 >> 5) + 3);
        worker->var_60 = 0xb;
        return;
    case 0xb:
        dy = worker->var_28 - worker->var_6c;
        dx = worker->var_24 - worker->var_68;
        worker->var_60 = (unsigned char)(((0x8fff < dy * dy + dx * dx) - 1U & 7) + 100);
        return;
    case 100:
        result = FUN_00482710(&worker->var_68, &worker->var_2c, dest);
        if (result == 0) {
            FUN_00499e60(worker->var_50);
            worker->flags_c = 0x10;
            worker->var_60 = 100;
            worker->ticks = 0x70;
            worker->state = 4;
            return;
        }
        if (result == 1) {
            worker->var_24 = dest[0];
            worker->var_28 = dest[1];
            dir = CalcMoveLine(worker->var_68, worker->var_6c, dest[0], dest[1], &worker->var_98);
            worker->state = 0xc;
            worker->var_73 = dir + 0x10;
            NewDirForAction(worker, (worker->var_73 >> 5) + 3);
            if ((worker->var_64 & 1) != 0) {
                worker->var_60 = 0x6a;
                return;
            }
        } else if (result == 2) {
            worker->var_24 = dest[0];
            worker->var_28 = dest[1];
            dir = CalcMoveLine(worker->var_68, worker->var_6c, dest[0], dest[1], &worker->var_98);
            worker->state = 0xc;
            worker->var_73 = dir + 0x10;
            NewDirForAction(worker, (worker->var_73 >> 5) + 3);
            worker->var_60 = ~worker->var_64 & 1 | 0x6a;
            return;
        }
        break;
    case 0x6a:
        worker->ticks = 0x70;
        worker->state = 4;
        worker->var_60 = 100;
        return;
    case 0x6b:
        *((unsigned char *)&worker->flags + 1) |= 1;
        BlokeSetAnim(worker, 1);
        BlokeSetFrame(worker, 0x28);
        worker->var_60++;
        return;
    case 0x6c:
        if (PlayBlokeAnim(worker) != 0) {
            BlokeWalkAnim((struct Bloke *)worker);
            worker->flags &= 0xfeff;
            worker->var_60++;
            return;
        }
        break;
    case 0x6d:
        order = worker->var_50;
        worker->flags &= 0xfff7;
        coords[0] = order->var_8;
        coords[1] = order->var_c;
        result = BuildObject(order->var_4, coords);
        if (result == 0) {
            FUN_00499e60(worker->var_50);
            NewLongTermAction((struct Bloke *)worker, 0x10);
            FUN_004735e0(1);
        } else {
            FUN_00499e30(worker->var_50);
            if (FUN_00499d00(worker) == 0) {
                NewLongTermAction((struct Bloke *)worker, 0x10);
                return;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0049a7f0
LEGO_EXPORT void Mechanic_Build(struct Worker *worker) {
    char dir;
    int result;
    int dx;
    int dy;
    int dest[2];
    int coords[2];
    struct WorkOrder *order;
    struct MapElement *cell;
    int x;
    int y;

    switch (worker->var_60) {
    case 0:
        worker->var_24 = worker->var_2c;
        worker->var_28 = worker->var_30;
        dir = CalcMoveLine(worker->var_68, worker->var_6c, worker->var_2c, worker->var_30,
                           &worker->var_98);
        worker->state = 0xc;
        worker->var_73 = dir + 0x10;
        NewDirForAction(worker, (worker->var_73 >> 5) + 3);
        worker->var_60 = 0xb;
        return;
    case 0xb:
        dy = worker->var_28 - worker->var_6c;
        dx = worker->var_24 - worker->var_68;
        worker->var_60 = (unsigned char)(((0xffff < dy * dy + dx * dx) - 1U & 6) + 0x65);
        return;
    case 0x65:
        result = FUN_00482710(&worker->var_68, &worker->var_2c, dest);
        if (result == 0) {
            FUN_00499f40(worker->var_50);
            worker->flags_c = 0x11;
            worker->var_60 = 0x65;
            worker->ticks = 0x70;
            worker->state = 4;
            return;
        }
        if (result == 1) {
            worker->var_24 = dest[0];
            worker->var_28 = dest[1];
            dir = CalcMoveLine(worker->var_68, worker->var_6c, dest[0], dest[1], &worker->var_98);
            worker->state = 0xc;
            worker->var_73 = dir + 0x10;
            NewDirForAction(worker, (worker->var_73 >> 5) + 3);
            if ((worker->var_64 & 1) != 0) {
                worker->var_60 = 0x6a;
                return;
            }
        } else if (result == 2) {
            worker->var_24 = dest[0];
            worker->var_28 = dest[1];
            dir = CalcMoveLine(worker->var_68, worker->var_6c, dest[0], dest[1], &worker->var_98);
            worker->state = 0xc;
            worker->var_73 = dir + 0x10;
            NewDirForAction(worker, (worker->var_73 >> 5) + 3);
            worker->var_60 = ~worker->var_64 & 1 | 0x6a;
            return;
        }
        break;
    case 0x6a:
        worker->ticks = 0x70;
        worker->state = 4;
        worker->var_60 = 0x65;
        return;
    case 0x6b:
        order = worker->var_50;
        *(unsigned char *)&worker->flags |= 8;
        coords[0] = order->var_8;
        coords[1] = order->var_c;
        result = BuildObject(order->var_4, coords);
        if (result != 0) {
            worker->var_46 = 0;
            worker->var_60++;
            return;
        }
        FUN_00499f40(worker->var_50);
        NewLongTermAction((struct Bloke *)worker, 0x11);
        worker->var_50->var_30 = 1;
        return;
    case 0x6c:
        worker->var_50->var_30 = 0;
        x = worker->var_50->var_8;
        y = worker->var_50->var_c;
        if (x < 0 || (int)lpConfig->width <= x || y < 0 || (int)lpConfig->height <= y) {
            cell = 0;
        } else {
            cell = (struct MapElement *)((char *)GameMap[y] + x * 0x14);
        }
        if ((cell->flags & 0x20) == 0) {
            worker->var_60++;
            return;
        }
        break;
    case 0x6d:
        worker->flags &= 0xfff7;
        worker->var_46 = 1;
        FUN_00499eb0(worker->var_50);
        if (FUN_00499d30(worker) == 0) {
            NewLongTermAction((struct Bloke *)worker, 0x11);
        }
    }
}

// FUNCTION: LEGOLAND 0x0049ab30
LEGO_EXPORT unsigned int WorkOrderBuildObject(struct EditObject *obj, int *coords) {
    struct ObjClass *cls;
    struct WorkOrder *order;
    struct Worker *worker;
    unsigned int result;
    int config[4];

    if (obj == (struct EditObject *)DAT_0080ff64 && DAT_0079a8d0 != 0) {
        return 0;
    }
    cls = *(struct ObjClass **)((char *)obj + 0xc);
    if ((cls->field_1c & 0x200000) == 0 || lpConfig->field_38 == 0) {
        if (GetBrickCount() < GetObjCost((struct CostInfo *)cls)) {
            FUN_004735e0(3);
            return 0;
        }
        FUN_0045e300(obj, (struct Point *)coords);
        FUN_0045d770(&EditCursor);
        result = BuildObject(obj, coords);
    } else {
        order = FUN_00499780(obj, coords, 1);
        result = (order != 0);
        if (result == 0) {
            return 0;
        }
        SetObjRectFlags(obj, (struct Point *)coords, 0x800);
        worker = FUN_00499c40(coords);
        if (worker != 0) {
            FUN_00499ac0(worker, order);
        }
    }
    if (result != 0 && DAT_00667cd8 == 0) {
        config[2] = coords[0];
        config[3] = coords[1];
        config[0] = 2;
        PlayInstanceOfSample(DAT_004b9248, 0, 1, config);
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0049ac50
void FUN_0049ac50(int param_1) {
    short sVar1;
    short sVar2;
    int *piVar3;
    struct ObjClass *cls;
    int *fp;
    int iVar6;
    int iVar7;
    unsigned int uVar8;
    unsigned int uVar9;
    int iVar10;
    unsigned int uVar11;
    int iVar12;
    int iVar13;
    int iVar14;
    int iVar15;
    int iVar16;
    struct Sprite *sprite;
    int handle;
    struct WorkOrder *node;
    int local_48;
    unsigned int local_44;
    int point[2];
    int bounds[4];
    int local_c;
    int local_4;

    piVar3 = *(int **)((char *)DAT_0079abfc + 0xc);
    if (param_1 == 0) {
        local_44 = 0x40e040;
        node = DAT_0079a8b0;
    } else {
        local_44 = 0xe04040;
        node = DAT_0079a8c0;
    }
    for (; node != 0; node = node->next) {
        cls = *(struct ObjClass **)((char *)node->var_4 + 0xc);
        local_48 = 0;
        if (0 < node->var_14) {
            do {
                fp = (int *)((char *)node->var_10 + local_48 * 0x14);
                iVar13 = fp[0] + node->var_8;
                iVar12 = fp[3] + node->var_c;
                iVar16 = fp[2] + node->var_8;
                iVar14 = fp[1] + node->var_c;
                local_c = iVar14;
                local_4 = iVar12;
                for (point[1] = iVar14; point[0] = iVar13, point[1] <= iVar12; point[1]++) {
                    for (; point[0] <= iVar16; point[0]++) {
                        GetTileBounds((struct Point *)point, bounds);
                        if (node->var_18 == 0) {
                            uVar11 = 1;
                            sprite = (struct Sprite *)TileSpriteArray[(DAT_00805f48 & 0xff) + *(int *)DAT_00801a6c];
                        } else {
                            sprite = (struct Sprite *)TileSpriteArray[(DAT_00805f48 & 0xff) + *(int *)DAT_00801a6c];
                            uVar11 = local_44;
                        }
                        PrintSprite(sprite, bounds[0], bounds[1], uVar11, 0);
                    }
                }
                if (local_48 == 0) {
                    iVar15 = iVar14;
                    if (iVar14 <= iVar12) {
                        do {
                            uVar11 = (unsigned int)(iVar14 != iVar12) << 2 |
                                     (unsigned int)(iVar14 != iVar15);
                            point[1] = iVar14;
                            iVar12 = local_4;
                            for (point[0] = iVar13; local_4 = iVar12, point[0] <= iVar16;
                                 point[0]++) {
                                uVar9 = ((unsigned int)(point[0] != iVar13) << 2 |
                                         (unsigned int)(point[0] != iVar16))
                                        << 1;
                                GetTileBounds((struct Point *)point, bounds);
                                if (node->var_18 == 0) {
                                    uVar8 = 0x1f1f81;
                                    sprite = (struct Sprite *)TileSpriteArray[(uVar9 | uVar11) + 3 + *piVar3];
                                } else {
                                    uVar8 = local_44 | 0x1f1f80;
                                    sprite = (struct Sprite *)TileSpriteArray[(uVar9 | uVar11) + 3 + *piVar3];
                                }
                                PrintSprite(sprite, bounds[0], bounds[1], uVar8, 0);
                                iVar15 = local_c;
                                iVar12 = local_4;
                            }
                            iVar14 = point[1] + 1;
                        } while (iVar14 <= iVar12);
                    }
                    handle = *(int *)((char *)cls + 0x68);
                    sVar1 = *(short *)(handle + 0x14);
                    sVar2 = *(short *)(handle + 0x16);
                    point[0] = iVar13;
                    point[1] = iVar12;
                    GetTileBounds((struct Point *)point, bounds);
                    iVar14 = bounds[0];
                    point[0] = iVar16;
                    point[1] = iVar15;
                    GetTileBounds((struct Point *)point, bounds);
                    iVar7 = bounds[2];
                    iVar10 = sVar1;
                    if (iVar10 * 2 < bounds[2] - iVar14) {
                        point[0] = iVar13;
                        point[1] = iVar15;
                        GetTileBounds((struct Point *)point, bounds);
                        iVar6 = bounds[1];
                        point[0] = iVar16;
                        point[1] = iVar12;
                        GetTileBounds((struct Point *)point, bounds);
                        PrintSprite(*(void **)((char *)cls + 0x68), ((iVar7 - iVar10) + iVar14) / 2,
                                    ((bounds[3] - sVar2) + iVar6) / 2, 0, 0);
                    }
                    point[0] = iVar13;
                    point[1] = iVar12;
                    GetTileBounds((struct Point *)point, bounds);
                    iVar14 = bounds[0];
                    point[0] = iVar16;
                    point[1] = iVar15;
                    GetTileBounds((struct Point *)point, bounds);
                    iVar7 = bounds[2];
                    point[0] = iVar13;
                    point[1] = iVar15;
                    GetTileBounds((struct Point *)point, bounds);
                    iVar13 = bounds[1];
                    point[0] = iVar16;
                    point[1] = iVar12;
                    GetTileBounds((struct Point *)point, bounds);
                    iVar14 = (iVar7 + iVar14) / 2 + 10;
                    iVar12 = ((bounds[3] - *(short *)((char *)DAT_007fdeb0 + 0x16)) + iVar13) / 2;
                    if (param_1 != 0) {
                        if (node->var_30 == 0) {
                            PrintSprite(DAT_007fe004, iVar14, iVar12, 0, 0);
                        } else {
                            LLSNextFrame(**(struct LLS ***)((char *)DAT_007fdeb0 + 8));
                            PrintSprite(DAT_007fdeb0, iVar14, iVar12, 0, 0);
                        }
                    }
                }
                local_48++;
            } while (local_48 < node->var_14);
        }
    }
}

// FUNCTION: LEGOLAND 0x0049b0b0
LEGO_EXPORT void RenderWorkerInterfaceGFX(void) {
    FUN_0049ac50(0);
    FUN_0049ac50(1);
    IterateNoneWorkersRepairOrders();
}

// FUNCTION: LEGOLAND 0x0049b0d0
unsigned int FUN_0049b0d0(struct Worker *a, void *b) {
    a->counter_11 = a->counter_11 + 1;

    if (a->counter_11 == (*((unsigned char *)b + 0x2c) >> 2)) {
        int result = FindObjectsPower(b);
        a->flags_c &= 0xfdff;
        if (result > 0) {
            DAT_00832bd0 += result;
            if (DAT_00832bd8) {
                FUN_0045a060();
            }
        }
    }

    return a->counter_11;
}

// FUNCTION: LEGOLAND 0x0049b130
LEGO_EXPORT struct WorkOrder *GetGardenerWorkOrderAt(int x, int y) {
    struct WorkOrder *current = DAT_0079a8b0;

    while (current != NULL) {
        struct Rect *rect = (struct Rect *)current->var_10;

        if (rect->minx + current->var_8 > x) {
            current = current->next;
            continue;
        }
        if (x > rect->maxx + current->var_8) {
            current = current->next;
            continue;
        }
        if (rect->miny + current->var_c > y) {
            current = current->next;
            continue;
        }
        if (y <= rect->maxy + current->var_c) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// FUNCTION: LEGOLAND 0x0049b180
LEGO_EXPORT struct WorkOrder *GetMechanicWorkOrderAt(int pos_x, int pos_y) {
    struct WorkOrder *node = DAT_0079a8c0;
    if (node == 0) {
        return 0;
    }

    while (node != 0) {
        struct Rect *box = (struct Rect *)node->var_10;
        if (node->var_8 + box->minx <= pos_x) {
            if (pos_x <= node->var_8 + box->maxx) {
                if (node->var_c + box->miny <= pos_y) {
                    if (pos_y <= node->var_c + box->maxy) {
                        return node;
                    }
                }
            }
        }
        node = node->next;
    }

    return 0;
}

// FUNCTION: LEGOLAND 0x0049b1d0
LEGO_EXPORT void EraseMechanicOrder(struct WorkOrder *order) {
    if (order->var_18 != 0) {
        NewLongTermAction((struct Bloke *)order->var_1c, 0x11);
    }
    if ((unsigned int)order->var_4 == DAT_0080ff64) {
        if (DAT_0079a8d0 != 0) {
            DAT_0079a8d0 = 0;
        }
    }
    if (order->var_20 == 1) {
        FUN_0045e4a0(order->var_4, &order->var_8);
    }
    FUN_00499eb0(order);
}

// FUNCTION: LEGOLAND 0x0049b230
LEGO_EXPORT void EraseGardenerOrder(struct WorkOrder *order) {
    if (order->var_18 != 0) {
        NewLongTermAction((struct Bloke *)order->var_1c, 0x10);
    }
    if (order->var_20 == 1) {
        FUN_0045e4a0(order->var_4, &order->var_8);
    }
    FUN_00499e30(order);
}

// FUNCTION: LEGOLAND 0x0049b270
void FUN_0049b270(int param_1, unsigned int param_2) {
    unsigned int y;
    struct WorkOrder *order;

    y = param_2 >> 8 & 0xff;
    order = GetMechanicWorkOrderAt(param_2 & 0xff, y);
    if (order != 0) {
        EraseMechanicOrder(order);
    }
    order = GetGardenerWorkOrderAt(param_2 & 0xff, y);
    if (order != 0) {
        EraseGardenerOrder(order);
    }
}

// FUNCTION: LEGOLAND 0x0049b2c0
LEGO_EXPORT int SetGardenerWorkOrderAtPostion(struct Worker *worker, int x, int y) {
    struct WorkOrder *order;
    struct Worker *holder;

    order = GetGardenerWorkOrderAt(x, y);
    if (order == 0) {
        if (FUN_00499d00(worker) == 0) {
            NewLongTermAction((struct Bloke *)worker, 0x10);
            return 1;
        }
    } else {
        if (order->var_18 != 0) {
            holder = (struct Worker *)order->var_1c;
            if ((order->var_20 != 1 || 0x6a < holder->var_60) &&
                (order->var_20 != 2 || 8 < holder->var_60)) {
                return 0;
            }
            NewLongTermAction((struct Bloke *)holder, 0x10);
        }
        FUN_00499ac0(worker, order);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0049b350
struct MapElement *FUN_0049b350(struct Worker *worker, int x, int y) {
    const int *entry;
    struct MapElement *tile;
    struct MapElement *target;
    struct ObjClass *cls;
    unsigned char size;
    int tx;
    int ty;

    entry = &DAT_004bff28[1];
    do {
        tx = entry[-1] + x;
        ty = entry[0] + y;
        if (tx >= 0 && tx < (int)lpConfig->width && ty >= 0 && ty < (int)lpConfig->height &&
            (tile = (struct MapElement *)((char *)GameMap[ty] + tx * 0x14), tile != 0) &&
            (tile->flags & 0x88) != 0) {
            unsigned char nx = *((unsigned char *)tile + 4);
            unsigned char ny = *((unsigned char *)tile + 5);
            if ((unsigned int)nx < (unsigned int)lpConfig->width &&
                (unsigned int)ny < (unsigned int)lpConfig->height) {
                target = (struct MapElement *)((char *)GameMap[ny] + nx * 0x14);
            } else {
                target = 0;
            }
            cls = *(struct ObjClass **)((char *)tile->field_0 + 0xc);
            size = *((unsigned char *)cls + 0x2c);
            if (size != 0 && *((unsigned char *)target + 0x11) < size &&
                (cls->field_1c & 0x400000) != 0 && lpConfig->field_34 != 0 &&
                (*((unsigned char *)target + 0xd) & 0x40) == 0) {
                return target;
            }
        }
        entry = entry + 2;
        if (&DAT_004bff28[24] <= entry) {
            return 0;
        }
    } while (1);
}

// FUNCTION: LEGOLAND 0x0049b430
LEGO_EXPORT int SetMechanicsOrderAtPostion(struct Worker *worker, int x, int y) {
    struct WorkOrder *order;
    struct MapElement *target;
    struct Worker *holder;
    struct WorkOrder *repair;

    order = GetMechanicWorkOrderAt(x, y);
    if (order == 0) {
        target = FUN_0049b350(worker, x, y);
        if (target == 0) {
            if (FUN_00499d30(worker) == 0) {
                NewLongTermAction((struct Bloke *)worker, 0x11);
                return 1;
            }
        } else {
            repair = AddRepairOrderForObject(*(struct ObjClass **)((char *)target->field_0 + 0xc),
                                             (char)*((unsigned char *)target + 4),
                                             *((unsigned char *)target + 5));
            if (repair != 0) {
                *((unsigned char *)target + 0xd) |= 0x40;
                FUN_00499b60(worker, repair);
                return 1;
            }
        }
    } else {
        if (order->var_18 != 0) {
            holder = (struct Worker *)order->var_1c;
            if ((order->var_20 != 1 || 0x6a < holder->var_60) &&
                (order->var_20 != 2 || 0x6a < holder->var_60)) {
                return 0;
            }
            NewLongTermAction((struct Bloke *)holder, 0x11);
        }
        FUN_00499b60(worker, order);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0049b510
LEGO_EXPORT void ClearAMechanicsWorkList(int value) {
    struct WorkOrder *head = DAT_0079a8c0;
    if (head == 0) {
        return;
    }
    {
        struct WorkOrder *node = head;
        while (node != 0) {
            if (node->var_1c == value) {
                node->var_18 = 0;
                node->var_30 = 0;
                node->var_1c = 0;
                DAT_0079a8c8++;
            }
            node = node->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x0049b550
LEGO_EXPORT void ClearAGardenersWorkList(int param) {
    struct WorkOrder *current = DAT_0079a8b0;
    if (current == NULL) {
        return;
    }
    while (current != NULL) {
        if (current->var_1c == param) {
            current->var_18 = 0;
            current->var_1c = 0;
            DAT_0079a8b8++;
        }
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x0049b580
LEGO_EXPORT void RemoveRepairOrderAT(struct Worker *worker, unsigned int x, unsigned int y) {
    if ((worker->flags_1c & 0x200000) && (lpConfig->field_38 != 0)) {
        RemoveGardenersWorkOrderAt(x, y);
    } else if ((worker->flags_1c & 0x400000) && (lpConfig->field_34 != 0)) {
        RemoveMechanicsWorkOrderAt(x, y);
    } else {
        RemoveNoneWorkersRepairOrderAT(x, y);
    }
}

// FUNCTION: LEGOLAND 0x0049b5f0
LEGO_EXPORT void RemoveGardenersWorkOrderAt(unsigned int x, unsigned int y) {
    struct WorkOrder *esi = DAT_0079a8b0;
    if (esi != 0) {
        while (esi != 0) {
            if ((unsigned int)esi->var_8 == x && (unsigned int)esi->var_c == y) {
                if (esi->var_18 != 0) {
                    NewLongTermAction((struct Bloke *)esi->var_1c, 0x10);
                }
                FUN_00499e30(esi);
                break;
            }
            esi = esi->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x0049b640
LEGO_EXPORT void RemoveMechanicsWorkOrderAt(unsigned int x, unsigned int y) {
    struct WorkOrder *current = DAT_0079a8c0;
    while (current) {
        if ((unsigned int)current->var_8 == x && (unsigned int)current->var_c == y) {
            if (current->var_18) {
                NewLongTermAction((struct Bloke *)current->var_1c, 0x11);
            }
            FUN_00499eb0(current);
            break;
        }
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x0049b690
void FUN_0049b690(struct Footprint *src, int *coords, float value) {
    struct RepairOrder *order;
    float scaled;

    order = malloc(sizeof(struct RepairOrder));
    order->next = DAT_0079a8d4;
    scaled = FLOAT_004ab480 * value;
    DAT_0079a8d4 = order;
    *(struct Footprint *)order->footprint = *src;
    order->var_18 = coords[0];
    order->var_1c = coords[1];
    order->var_20 = scaled;
    order->var_24 = value;
}

// FUNCTION: LEGOLAND 0x0049b6e0
void FUN_0049b6e0(struct RepairOrder *order) {
    struct RepairOrder *cur;
    struct RepairOrder *prev;

    if (DAT_0079a8d4 == order) {
        DAT_0079a8d4 = order->next;
        free(order);
        return;
    }
    cur = ((struct RepairOrder *)DAT_0079a8d4)->next;
    prev = DAT_0079a8d4;
    while (cur != order) {
        prev = prev->next;
        if (prev == 0) {
            goto done;
        }
        cur = prev->next;
    }
    if (prev != 0) {
        prev->next = order->next;
    }
done:
    free(order);
}

// FUNCTION: LEGOLAND 0x0049b720
LEGO_EXPORT void RemoveNoneWorkersRepairOrderAT(unsigned int x, unsigned int y) {
    struct RepairOrder *current = DAT_0079a8d4;
    while (current != NULL) {
        if ((unsigned int)current->var_18 == x && (unsigned int)current->var_1c == y) {
            FUN_0049b6e0(current);
            return;
        }
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x0049b750
LEGO_EXPORT void IterateNoneWorkersRepairOrders(void) {
    struct RepairOrder *cur;
    struct RepairOrder *order;
    int iVar3;
    int iVar4;
    int iVar5;
    int iVar6;
    int iVar7;
    struct MapElement *tile;
    struct ObjClass *cls;
    int point[2];
    int bounds[4];
    int local_4;
    int cost;

    cur = DAT_0079a8d4;
    while (order = cur, order != 0) {
        cur = order->next;
        iVar3 = order->footprint[1] + order->var_1c;
        iVar6 = order->footprint[0] + order->var_18;
        iVar5 = order->footprint[2] + order->var_18;
        point[1] = order->footprint[3] + order->var_1c;
        point[0] = iVar6;
        local_4 = point[1];
        GetTileBounds((struct Point *)point, bounds);
        iVar4 = bounds[0];
        point[0] = iVar5;
        point[1] = iVar3;
        GetTileBounds((struct Point *)point, bounds);
        iVar7 = bounds[2];
        point[0] = iVar6;
        point[1] = iVar3;
        GetTileBounds((struct Point *)point, bounds);
        iVar3 = bounds[1];
        point[1] = local_4;
        point[0] = iVar5;
        GetTileBounds((struct Point *)point, bounds);
        iVar7 = (iVar7 + iVar4) / 2;
        iVar4 = (bounds[3] + iVar3) / 2;
        if (order->var_20 < (float)DAT_004ab3a8) {
            cost = 0;
        } else {
            cost = FUN_00458930(order->var_20);
        }
        if (GetBrickCount() < cost) {
            LLSNextFrame(**(struct LLS ***)((char *)DAT_007fdeb0 + 8));
            PrintSprite(DAT_007fdeb0, iVar7, iVar4, 0, 0);
        } else {
            UseBricks(cost);
            order->var_20 = (order->var_20 - (float)cost) + order->var_24;
            PrintSprite(DAT_007fe004, iVar7, iVar4, 0, 0);
            iVar4 = order->var_18;
            if (iVar4 < 0 || (int)lpConfig->width <= iVar4 || (iVar3 = order->var_1c, iVar3 < 0) ||
                (int)lpConfig->height <= iVar3) {
                tile = 0;
            } else {
                tile = (struct MapElement *)((char *)GameMap[iVar3] + iVar4 * 0x14);
            }
            DAT_00668610 |= 0x200;
            cls = *(struct ObjClass **)((char *)tile->field_0 + 0xc);
            FUN_0049b0d0((struct Worker *)tile, cls);
            if (*((unsigned char *)cls + 0x2c) <= *((unsigned char *)tile + 0x11)) {
                tile->flags &= 0xbfff;
                *((unsigned char *)tile + 0x11) = *((unsigned char *)cls + 0x2c);
                FUN_0049b6e0(order);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0049b930
LEGO_EXPORT struct WorkOrder *AddRepairOrderForObject(struct ObjClass *cls, int x, int y) {
    unsigned char level;
    int cost;
    struct MapElement *cell;

    if (x < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        cell = 0;
    } else {
        cell = (struct MapElement *)((char *)GameMap[y] + x * 0x14);
    }
    level = *((unsigned char *)cell + 0x11);
    cost = GetObjRepairCost((unsigned int)cls, level);
    if ((cls->field_1c & 0x200000) != 0 && lpConfig->field_38 != 0) {
        return FUN_00499780((struct EditObject *)cls->field_c4, &x, 2);
    }
    if ((cls->field_1c & 0x400000) != 0 && lpConfig->field_34 != 0) {
        return FUN_00499830((struct EditObject *)cls->field_c4, &x, 2);
    }
    FUN_0049b690((struct Footprint *)&cls->field_3c, &x,
                 (float)cost / (float)(int)(*((unsigned char *)cls + 0x2c) - level));
}

// FUNCTION: LEGOLAND 0x0049ba10
LEGO_EXPORT void Garderner_Repair(struct Worker *worker) {
    char dir;
    int result;
    int cost;
    struct WorkOrder *order;
    struct MapElement *tile;
    struct ObjClass *cls;
    int dest[2];

    switch (worker->var_60) {
    case 0:
        result = FUN_00482710(&worker->var_68, &worker->var_2c, dest);
        if (result == 0) {
            FUN_00499720(worker->var_50);
            FUN_00499e60(worker->var_50);
            worker->flags_c = 0x10;
            worker->var_60 = 0;
            worker->ticks = 0x10;
            worker->state = 4;
            return;
        }
        if (result == 1) {
            worker->var_24 = dest[0];
            worker->var_28 = dest[1];
            dir = CalcMoveLine(worker->var_68, worker->var_6c, dest[0], dest[1], &worker->var_98);
            worker->state = 0xc;
            worker->var_73 = dir + 0x10;
            NewDirForAction(worker, (worker->var_73 >> 5) + 3);
            if ((worker->var_64 & 1) != 0) {
                worker->var_60 = 6;
                return;
            }
        } else if (result == 2) {
            worker->var_24 = dest[0];
            worker->var_28 = dest[1];
            dir = CalcMoveLine(worker->var_68, worker->var_6c, dest[0], dest[1], &worker->var_98);
            worker->state = 0xc;
            worker->var_73 = dir + 0x10;
            NewDirForAction(worker, (worker->var_73 >> 5) + 3);
            worker->var_60 = ~worker->var_64 & 1 | 6;
            return;
        }
        break;
    case 6:
        worker->ticks = 0x70;
        worker->state = 4;
        worker->var_60 = 0;
        return;
    case 7:
        worker->flags |= 0x108;
        BlokeSetAnim(worker, 1);
        worker->var_60++;
        worker->var_72 = *((unsigned char *)worker->var_50 + 0x2c);
        return;
    case 8:
        if (PlayBlokeAnim(worker) != 0) {
            BlokeWalkAnim((struct Bloke *)worker);
            worker->flags &= 0xfeff;
            worker->var_60++;
            return;
        }
        break;
    case 9:
        order = worker->var_50;
        *(unsigned char *)&worker->flags |= 8;
        worker->var_72 = *((unsigned char *)order + 0x2c);
        if (order->var_8 < 0 || (int)lpConfig->width <= order->var_8 || order->var_c < 0 ||
            (int)lpConfig->height <= order->var_c) {
            tile = 0;
        } else {
            tile = (struct MapElement *)((char *)GameMap[order->var_c] + order->var_8 * 0x14);
        }
        if ((tile->flags & 0x88) != 0) {
            if (order->var_34 < (float)DAT_004ab3a8) {
                cost = 0;
            } else {
                cost = FUN_00458930(order->var_34);
            }
            if (GetBrickCount() < cost) {
                order->var_30 = 1;
                return;
            }
            UseBricks(cost);
            order->var_34 = (order->var_34 - (float)cost) + order->var_38;
            if (order->var_8 < 0 || (int)lpConfig->width <= order->var_8 ||
                (order->var_c < 0 || (int)lpConfig->height <= order->var_c)) {
                tile = 0;
            } else {
                tile = (struct MapElement *)((char *)GameMap[order->var_c] + order->var_8 * 0x14);
            }
            DAT_00668610 |= 0x200;
            cls = *(struct ObjClass **)((char *)tile->field_0 + 0xc);
            FUN_0049b0d0((struct Worker *)tile, cls);
            if (*((unsigned char *)cls + 0x2c) <= *((unsigned char *)tile + 0x11)) {
                tile->flags &= 0xbfff;
                *((unsigned char *)tile + 0x11) = *((unsigned char *)cls + 0x2c);
                worker->var_60++;
                return;
            }
        }
        break;
    case 10:
        worker->flags &= 0xfff7;
        FUN_00499e30(worker->var_50);
        if (FUN_00499d00(worker) == 0) {
            NewLongTermAction((struct Bloke *)worker, 0x10);
        }
    }
}

// FUNCTION: LEGOLAND 0x0049bd20
LEGO_EXPORT void Mechanics_Repair(struct Worker *worker) {
    char dir;
    int result;
    int cost;
    int dx;
    int dy;
    struct WorkOrder *order;
    struct MapElement *tile;
    struct ObjClass *cls;
    int dest[2];

    switch ((char)worker->var_60) {
    case 0:
        worker->var_24 = worker->var_2c;
        worker->var_28 = worker->var_30;
        dir = CalcMoveLine(worker->var_68, worker->var_6c, worker->var_2c, worker->var_30,
                           &worker->var_98);
        worker->state = 0xc;
        worker->var_73 = dir + 0x10;
        NewDirForAction(worker, (worker->var_73 >> 5) + 3);
        worker->var_60 = 0xb;
        return;
    case 0xb:
        dx = worker->var_28 - worker->var_6c;
        dy = worker->var_24 - worker->var_68;
        worker->var_60 = (unsigned char)(((0xffff < dx * dx + dy * dy) - 1U & 7) + 100);
        return;
    case 0x64:
        result = FUN_00482710(&worker->var_68, &worker->var_2c, dest);
        if (result == 0) {
            FUN_00499720(worker->var_50);
            FUN_00499f40(worker->var_50);
            worker->flags_c = 0x11;
            worker->var_60 = 0;
            worker->ticks = 0x70;
            worker->state = 4;
            return;
        }
        if (result == 1) {
            worker->var_24 = dest[0];
            worker->var_28 = dest[1];
            dir = CalcMoveLine(worker->var_68, worker->var_6c, dest[0], dest[1], &worker->var_98);
            worker->state = 0xc;
            worker->var_73 = dir + 0x10;
            NewDirForAction(worker, (worker->var_73 >> 5) + 3);
            if ((worker->var_64 & 1) != 0) {
                worker->var_60 = 0x6a;
                return;
            }
        } else if (result == 2) {
            worker->var_24 = dest[0];
            worker->var_28 = dest[1];
            dir = CalcMoveLine(worker->var_68, worker->var_6c, dest[0], dest[1], &worker->var_98);
            worker->state = 0xc;
            worker->var_73 = dir + 0x10;
            NewDirForAction(worker, (worker->var_73 >> 5) + 3);
            worker->var_60 = ~worker->var_64 & 1 | 0x6a;
            return;
        }
        break;
    case 0x6a:
        worker->ticks = 0x70;
        worker->state = 4;
        worker->var_60 = 100;
        return;
    case 0x6b:
        order = worker->var_50;
        *(unsigned char *)&worker->flags |= 8;
        worker->var_72 = *((unsigned char *)order + 0x2c);
        if (order->var_8 < 0 || (int)lpConfig->width <= order->var_8 || order->var_c < 0 ||
            (int)lpConfig->height <= order->var_c) {
            tile = 0;
        } else {
            tile = (struct MapElement *)((char *)GameMap[order->var_c] + order->var_8 * 0x14);
        }
        if ((tile->flags & 0x88) == 0) {
            worker->var_60++;
            return;
        }
        if (order->var_34 < (float)DAT_004ab3a8) {
            cost = 0;
        } else {
            cost = FUN_00458930(order->var_34);
        }
        if (GetBrickCount() < cost) {
            order->var_30 = 1;
            FUN_004735e0(2);
            return;
        }
        UseBricks(cost);
        order->var_34 = (order->var_34 - (float)cost) + order->var_38;
        if (order->var_8 < 0 || (int)lpConfig->width <= order->var_8 ||
            (order->var_c < 0 || (int)lpConfig->height <= order->var_c)) {
            tile = 0;
        } else {
            tile = (struct MapElement *)((char *)GameMap[order->var_c] + order->var_8 * 0x14);
        }
        cls = *(struct ObjClass **)((char *)tile->field_0 + 0xc);
        DAT_00668610 |= 0x200;
        FUN_0049b0d0((struct Worker *)tile, cls);
        if (*((unsigned char *)cls + 0x2c) <= *((unsigned char *)tile + 0x11)) {
            tile->flags &= 0xbfff;
            *((unsigned char *)tile + 0x11) = *((unsigned char *)cls + 0x2c);
            worker->var_60++;
            return;
        }
        break;
    case 0x6c:
        worker->flags &= 0xfff7;
        worker->var_46 = 1;
        FUN_00499eb0(worker->var_50);
        if (FUN_00499d30(worker) == 0) {
            NewLongTermAction((struct Bloke *)worker, 0x11);
        }
    }
}

// FUNCTION: LEGOLAND 0x0049c140
void FUN_0049c140(void) {
    struct Worker *cur;
    struct Worker *node;
    struct BlokeList *list;
    void *bnode;
    int count;
    struct BlokeSave rec;
    char *person;

    // STRING: LEGOLAND 0x004b89ac
    list = (struct BlokeList *)*(int *)(ElemID("POTTING SHED") + 0xc);
    count = 0;
    node = GardenerList;
    do {
        do {
            cur = node;
            node = GardenerList;
            if (cur == 0) {
                for (; node != 0; node = node->next) {
                    count++;
                }
                if (SaveGameWrite(&count, 4) != 0) {
                    for (node = GardenerList; node != 0; node = node->next) {
                        rec.field_0 = node->flags_c;
                        rec.field_2 = node->state;
                        rec.field_4 = node->field_10;
                        rec.field_10 = node->flags_1c;
                        rec.field_14 = node->field_20;
                        rec.field_18 = node->var_24;
                        rec.field_1c = node->var_28;
                        rec.field_20 = node->var_2c;
                        rec.field_24 = node->var_30;
                        memcpy(rec.field_28, (char *)node + 0x34, 40);
                        rec.field_50 = node->ticks;
                        rec.field_54 = node->var_60;
                        rec.field_56 = node->flags;
                        rec.field_58 = node->var_64;
                        rec.field_59 = node->var_7f;
                        rec.field_5a = node->var_82;
                        rec.field_5c = node->var_68;
                        rec.field_60 = node->var_6c;
                        rec.field_64 = node->var_70;
                        rec.field_66 = node->var_72;
                        rec.field_67 = node->var_73;
                        rec.field_68 = node->var_74;
                        rec.field_69 = node->var_75;
                        memcpy(rec.field_6c, &node->var_98, 20);
                        person = (char *)node->field_4;
                        rec.field_80 = *(unsigned int *)(person + 8);
                        rec.field_84 = *(unsigned int *)(person + 0x10);
                        rec.field_88 = *(unsigned int *)(person + 0x14);
                        rec.field_8c = *(unsigned int *)(person + 0x18);
                        rec.field_90 = *(unsigned int *)(person + 0x1c);
                        rec.field_94 = *(unsigned int *)(person + 0x20);
                        rec.field_98 = *(unsigned int *)(person + 0x40);
                        rec.field_9c = *(unsigned int *)(person + 0x44);
                        rec.field_a0 = *(unsigned int *)(person + 0x48);
                        rec.field_a4 = *(unsigned int *)(person + 0x4c);
                        rec.field_a8 = *(unsigned int *)(person + 0x88);
                        rec.field_ac = *(unsigned int *)(person + 0x54);
                        memcpy(rec.field_b0, person + 0x58, 36);
                        rec.field_d4 = node->field_a;
                        rec.field_d8 = node->field_8;
                        rec.field_28[7] = 0;
                        if (SaveGameWrite(&rec, 0xdc) == 0) {
                            return;
                        }
                    }
                }
                return;
            }
            node = cur->next;
        } while (cur->flags_c != 5);
        for (bnode = *(void **)((char *)list + 0xcc); bnode != 0; bnode = *(void **)bnode) {
            if (*(struct Worker **)((char *)bnode + 8) == cur) {
                RemoveBlokeFromList(list, (struct Bloke *)bnode);
                break;
            }
        }
        cur->flags &= 0xffd7;
        if (cur->var_60 < 100) {
            cur->var_68 = cur->var_24;
            cur->var_6c = cur->var_28;
            NewLongTermAction((struct Bloke *)cur, 0x10);
        } else {
            RemoveAGardener(cur);
        }
    } while (1);
}

// FUNCTION: LEGOLAND 0x0049c3c0
void FUN_0049c3c0(void) {
    int count;
    struct Worker *node;
    struct Worker *prev;
    char *person;
    struct BlokeSave rec;

    count = 0;
    if (SaveGameRead(&count, 4) == 0) {
        return;
    }
    GardenerList = 0;
    DAT_0079a8bc = count;
    prev = 0;
    if (count == 0) {
        return;
    }
    do {
        count--;
        if (prev == 0) {
            node = malloc(0xac);
            GardenerList = node;
        } else {
            node = malloc(0xac);
            prev->next = node;
        }
        if (SaveGameRead(&rec, 0xdc) == 0) {
            return;
        }
        node->flags_c = rec.field_0;
        node->state = rec.field_2;
        node->field_10 = rec.field_4;
        node->flags_1c = rec.field_10;
        node->field_20 = rec.field_14;
        node->var_24 = rec.field_18;
        node->var_28 = rec.field_1c;
        node->var_2c = rec.field_20;
        node->var_30 = rec.field_24;
        memcpy((char *)node + 0x34, rec.field_28, 40);
        node->ticks = rec.field_50;
        node->var_60 = rec.field_54;
        node->flags = rec.field_56;
        node->var_64 = rec.field_58;
        node->var_7f = rec.field_59;
        node->var_82 = rec.field_5a;
        node->var_68 = rec.field_5c;
        node->var_6c = rec.field_60;
        node->var_70 = rec.field_64;
        node->var_72 = rec.field_66;
        node->var_73 = rec.field_67;
        node->var_74 = rec.field_68;
        node->var_75 = rec.field_69;
        memcpy(&node->var_98, rec.field_6c, 20);
        person = malloc(0x94);
        node->field_4 = (struct Person *)person;
        FUN_0043f810((struct Person *)person);
        *(struct Worker **)(person + 0xc) = node;
        *(unsigned int *)(person + 8) = rec.field_80;
        *(unsigned int *)(person + 0x10) = rec.field_84;
        *(unsigned int *)(person + 0x14) = rec.field_88;
        *(unsigned int *)(person + 0x18) = rec.field_8c;
        *(unsigned int *)(person + 0x1c) = rec.field_90;
        *(unsigned int *)(person + 0x20) = rec.field_94;
        *(unsigned int *)(person + 0x40) = rec.field_98;
        *(unsigned int *)(person + 0x44) = rec.field_9c;
        *(unsigned int *)(person + 0x48) = rec.field_a0;
        *(unsigned int *)(person + 0x4c) = rec.field_a4;
        *(unsigned int *)(person + 0x88) = 0xffffffff;
        *(unsigned int *)(person + 0x54) = rec.field_ac;
        memcpy(person + 0x58, rec.field_b0, 36);
        node->field_a = rec.field_d4;
        node->field_8 = (unsigned char)rec.field_d8;
        *(unsigned int *)(person + 0x2c) = 0;
        *(unsigned int *)(person + 0x50) = 0;
        BlokeSetAnim((struct Bloke *)node, rec.field_a8);
        prev = node;
    } while (count != 0);
    if (node != 0) {
        node->next = 0;
    }
}

// FUNCTION: LEGOLAND 0x0049c630
void FUN_0049c630(void) {
    struct Worker *cur;
    struct Worker *node;
    struct BlokeList *list;
    void *bnode;
    int count;
    struct BlokeSave rec;
    char *person;

    list = (struct BlokeList *)*(int *)(ElemID("MECHANICS HUT") + 0xc);
    count = 0;
    node = MechanicList;
    do {
        do {
            cur = node;
            node = MechanicList;
            if (cur == 0) {
                for (; node != 0; node = node->next) {
                    count++;
                }
                if (SaveGameWrite(&count, 4) != 0) {
                    for (node = MechanicList; node != 0; node = node->next) {
                        rec.field_0 = node->flags_c;
                        rec.field_2 = node->state;
                        rec.field_4 = node->field_10;
                        rec.field_10 = node->flags_1c;
                        rec.field_14 = node->field_20;
                        rec.field_18 = node->var_24;
                        rec.field_1c = node->var_28;
                        rec.field_20 = node->var_2c;
                        rec.field_24 = node->var_30;
                        memcpy(rec.field_28, (char *)node + 0x34, 40);
                        rec.field_50 = node->ticks;
                        rec.field_54 = node->var_60;
                        rec.field_56 = node->flags;
                        rec.field_58 = node->var_64;
                        rec.field_59 = node->var_7f;
                        rec.field_5a = node->var_82;
                        rec.field_5c = node->var_68;
                        rec.field_60 = node->var_6c;
                        rec.field_64 = node->var_70;
                        rec.field_66 = node->var_72;
                        rec.field_67 = node->var_73;
                        rec.field_68 = node->var_74;
                        rec.field_69 = node->var_75;
                        memcpy(rec.field_6c, &node->var_98, 20);
                        person = (char *)node->field_4;
                        rec.field_80 = *(unsigned int *)(person + 8);
                        rec.field_84 = *(unsigned int *)(person + 0x10);
                        rec.field_88 = *(unsigned int *)(person + 0x14);
                        rec.field_8c = *(unsigned int *)(person + 0x18);
                        rec.field_90 = *(unsigned int *)(person + 0x1c);
                        rec.field_94 = *(unsigned int *)(person + 0x20);
                        rec.field_98 = *(unsigned int *)(person + 0x40);
                        rec.field_9c = *(unsigned int *)(person + 0x44);
                        rec.field_a0 = *(unsigned int *)(person + 0x48);
                        rec.field_a4 = *(unsigned int *)(person + 0x4c);
                        rec.field_a8 = *(unsigned int *)(person + 0x88);
                        rec.field_ac = *(unsigned int *)(person + 0x54);
                        memcpy(rec.field_b0, person + 0x58, 36);
                        rec.field_d4 = node->field_a;
                        rec.field_d8 = node->field_8;
                        rec.field_28[7] = 0;
                        if (SaveGameWrite(&rec, 0xdc) == 0) {
                            return;
                        }
                    }
                }
                return;
            }
            node = cur->next;
        } while (cur->flags_c != 5);
        for (bnode = *(void **)((char *)list + 0xcc); bnode != 0; bnode = *(void **)bnode) {
            if (*(struct Worker **)((char *)bnode + 8) == cur) {
                RemoveBlokeFromList(list, (struct Bloke *)bnode);
                break;
            }
        }
        cur->flags &= 0xffd7;
        if (cur->var_60 < 100) {
            cur->var_68 = cur->var_24;
            cur->var_6c = cur->var_28;
            NewLongTermAction((struct Bloke *)cur, 0x11);
        } else {
            RemoveAMechanic(cur);
        }
    } while (1);
}

// FUNCTION: LEGOLAND 0x0049c8b0
void FUN_0049c8b0(void) {
    int count;
    struct Worker *node;
    struct Worker *prev;
    char *person;
    struct BlokeSave rec;

    count = 0;
    if (SaveGameRead(&count, 4) == 0) {
        return;
    }
    MechanicList = 0;
    DAT_0079a8cc = count;
    prev = 0;
    if (count == 0) {
        return;
    }
    do {
        count--;
        if (prev == 0) {
            node = malloc(0xac);
            MechanicList = node;
        } else {
            node = malloc(0xac);
            prev->next = node;
        }
        if (SaveGameRead(&rec, 0xdc) == 0) {
            return;
        }
        node->flags_c = rec.field_0;
        node->state = rec.field_2;
        node->field_10 = rec.field_4;
        node->flags_1c = rec.field_10;
        node->field_20 = rec.field_14;
        node->var_24 = rec.field_18;
        node->var_28 = rec.field_1c;
        node->var_2c = rec.field_20;
        node->var_30 = rec.field_24;
        memcpy((char *)node + 0x34, rec.field_28, 40);
        node->ticks = rec.field_50;
        node->var_60 = rec.field_54;
        node->flags = rec.field_56;
        node->var_64 = rec.field_58;
        node->var_7f = rec.field_59;
        node->var_82 = rec.field_5a;
        node->var_68 = rec.field_5c;
        node->var_6c = rec.field_60;
        node->var_70 = rec.field_64;
        node->var_72 = rec.field_66;
        node->var_73 = rec.field_67;
        node->var_74 = rec.field_68;
        node->var_75 = rec.field_69;
        memcpy(&node->var_98, rec.field_6c, 20);
        person = malloc(0x94);
        node->field_4 = (struct Person *)person;
        FUN_0043f810((struct Person *)person);
        *(struct Worker **)(person + 0xc) = node;
        *(unsigned int *)(person + 8) = rec.field_80;
        *(unsigned int *)(person + 0x10) = rec.field_84;
        *(unsigned int *)(person + 0x14) = rec.field_88;
        *(unsigned int *)(person + 0x18) = rec.field_8c;
        *(unsigned int *)(person + 0x1c) = rec.field_90;
        *(unsigned int *)(person + 0x20) = rec.field_94;
        *(unsigned int *)(person + 0x40) = rec.field_98;
        *(unsigned int *)(person + 0x44) = rec.field_9c;
        *(unsigned int *)(person + 0x48) = rec.field_a0;
        *(unsigned int *)(person + 0x4c) = rec.field_a4;
        *(unsigned int *)(person + 0x88) = 0xffffffff;
        *(unsigned int *)(person + 0x54) = rec.field_ac;
        memcpy(person + 0x58, rec.field_b0, 36);
        node->field_a = rec.field_d4;
        node->field_8 = (unsigned char)rec.field_d8;
        *(unsigned int *)(person + 0x2c) = 0;
        *(unsigned int *)(person + 0x50) = 0;
        BlokeSetAnim((struct Bloke *)node, rec.field_a8);
        prev = node;
    } while (count != 0);
    if (node != 0) {
        node->next = 0;
    }
}

// FUNCTION: LEGOLAND 0x0049cb20
void FUN_0049cb20(void) {
    int count;
    int len;
    struct WorkOrder *node;
    struct WorkOrder buf;
    struct Worker *idx;
    struct Worker *scan;
    char *name;

    count = 0;
    for (node = DAT_0079a8b0; node != 0; node = node->next) {
        count++;
    }
    SaveGameWrite(&count, 4);
    node = DAT_0079a8b0;
    do {
        if (node == 0) {
            return;
        }
        buf = *node;
        idx = (struct Worker *)buf.var_1c;
        if (buf.var_18 != 0) {
            idx = 0;
            scan = GardenerList;
            if (GardenerList == 0) {
                buf.var_18 = 0;
                idx = (struct Worker *)buf.var_1c;
            } else {
                while (scan != (struct Worker *)buf.var_1c) {
                    scan = scan->next;
                    idx = (struct Worker *)((char *)idx + 1);
                    if (scan == 0) {
                        buf.var_18 = 0;
                        idx = (struct Worker *)buf.var_1c;
                        break;
                    }
                }
            }
        }
        buf.var_1c = (int)idx;
        SaveGameWrite(&buf, 0x3c);
        name = *(char **)buf.var_4;
        len = strlen(name) + 1;
        SaveGameWrite(&len, 4);
        SaveGameWrite(*(char **)buf.var_4, len);
        SaveGameWrite(buf.var_10, buf.var_14 * 0x14);
        node = node->next;
    } while (1);
}

// FUNCTION: LEGOLAND 0x0049cc10
void FUN_0049cc10(void) {
    int count;
    int len;
    struct WorkOrder *node;
    struct WorkOrder *prev;
    struct Worker *worker;
    void *fp;
    int i;
    char name[512];

    DAT_0079a8b0 = 0;
    SaveGameRead(&count, 4);
    prev = 0;
    DAT_0079a8b8 = count;
    while (count != 0) {
        count--;
        if (prev == 0) {
            node = malloc(0x3c);
            DAT_0079a8b0 = node;
        } else {
            node = malloc(0x3c);
            prev->next = node;
        }
        SaveGameRead(node, 0x3c);
        SaveGameRead(&len, 4);
        SaveGameRead(name, len);
        name[len] = 0;
        node->var_4 = (struct EditObject *)ElemID(name);
        if (node->var_18 != 0) {
            worker = GardenerList;
            for (i = (int)node->var_1c; i != 0; i--) {
                worker = worker->next;
            }
            node->var_1c = (int)worker;
            worker->var_50 = node;
        }
        fp = malloc(node->var_14 * 0x14);
        node->var_10 = fp;
        SaveGameRead(fp, node->var_14 * 0x14);
        prev = node;
    }
    DAT_0079a8b4 = prev;
}

// FUNCTION: LEGOLAND 0x0049cd10
void FUN_0049cd10(void) {
    int count;
    int len;
    struct WorkOrder *node;
    struct WorkOrder buf;
    struct Worker *idx;
    struct Worker *scan;
    char *name;

    count = 0;
    for (node = DAT_0079a8c0; node != 0; node = node->next) {
        count++;
    }
    SaveGameWrite(&count, 4);
    node = DAT_0079a8c0;
    do {
        if (node == 0) {
            return;
        }
        buf = *node;
        idx = (struct Worker *)buf.var_1c;
        if (buf.var_18 != 0) {
            idx = 0;
            scan = MechanicList;
            if (MechanicList == 0) {
                buf.var_18 = 0;
                idx = (struct Worker *)buf.var_1c;
            } else {
                while (scan != (struct Worker *)buf.var_1c) {
                    scan = scan->next;
                    idx = (struct Worker *)((char *)idx + 1);
                    if (scan == 0) {
                        buf.var_18 = 0;
                        idx = (struct Worker *)buf.var_1c;
                        break;
                    }
                }
            }
        }
        buf.var_1c = (int)idx;
        SaveGameWrite(&buf, 0x3c);
        name = *(char **)buf.var_4;
        len = strlen(name) + 1;
        SaveGameWrite(&len, 4);
        SaveGameWrite(*(char **)buf.var_4, len);
        SaveGameWrite(buf.var_10, buf.var_14 * 0x14);
        node = node->next;
    } while (1);
}

// FUNCTION: LEGOLAND 0x0049ce00
void FUN_0049ce00(void) {
    int count;
    int len;
    struct WorkOrder *node;
    struct WorkOrder *prev;
    struct Worker *worker;
    void *fp;
    int i;
    char name[512];

    DAT_0079a8c0 = 0;
    SaveGameRead(&count, 4);
    prev = 0;
    DAT_0079a8c8 = count;
    while (count != 0) {
        count--;
        if (prev == 0) {
            node = malloc(0x3c);
            DAT_0079a8c0 = node;
        } else {
            node = malloc(0x3c);
            prev->next = node;
        }
        SaveGameRead(node, 0x3c);
        SaveGameRead(&len, 4);
        SaveGameRead(name, len);
        name[len] = 0;
        fp = malloc(node->var_14 * 0x14);
        node->var_10 = fp;
        SaveGameRead(fp, node->var_14 * 0x14);
        node->var_4 = (struct EditObject *)ElemID(name);
        prev = node;
        if (node->var_18 != 0) {
            worker = MechanicList;
            for (i = (int)node->var_1c; i != 0; i--) {
                worker = worker->next;
            }
            node->var_1c = (int)worker;
            worker->var_50 = node;
        }
    }
    DAT_0079a8c4 = prev;
}

// FUNCTION: LEGOLAND 0x0049cf00
void FUN_0049cf00(void) {
    struct Worker *node;
    struct LegoConfig *cfg;
    int x;
    int y;
    struct MapElement *tile;

    cfg = lpConfig;
    for (node = MechanicList; node != 0; node = node->next) {
        if (node->var_68 >= 0 && (x = node->var_68 >> 8, x < (int)cfg->width) &&
            node->var_6c >= 0 && (y = node->var_6c >> 8, y < (int)cfg->height)) {
            tile = (struct MapElement *)((char *)GameMap[y] + x * 0x14);
            *((unsigned char *)tile + 0xd) |= 0x10;
            cfg = lpConfig;
        }
    }
    for (node = GardenerList; node != 0; node = node->next) {
        if (node->var_68 >= 0 && (x = node->var_68 >> 8, x < (int)cfg->width) &&
            node->var_6c >= 0 && (y = node->var_6c >> 8, y < (int)cfg->height)) {
            tile = (struct MapElement *)((char *)GameMap[y] + x * 0x14);
            *((unsigned char *)tile + 0xd) |= 0x10;
            cfg = lpConfig;
        }
    }
}

// FUNCTION: LEGOLAND 0x0049cfc0
void FUN_0049cfc0(void) {
    while (DAT_0079a8b0 != NULL) {
        FUN_00499e30(DAT_0079a8b0);
    }
    while (DAT_0079a8c0 != NULL) {
        FUN_00499eb0(DAT_0079a8c0);
    }
    while (DAT_0079a8d4 != NULL) {
        FUN_0049b6e0(DAT_0079a8d4);
    }
    while (GardenerList != NULL) {
        RemoveAGardener(GardenerList);
    }
    while (MechanicList != NULL) {
        RemoveAMechanic(MechanicList);
    }
}
