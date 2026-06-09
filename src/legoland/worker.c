#include "legoland.h"
#include "crt.h"
#include "globals.h"

#include "bloke_ai.h"
#include "bricks.h"
#include "debug_alloc.h"
#include "gamemap.h"
#include "man3d.h"
#include "bloke.h"
#include "worker.h"
#include "llidb.h"
#include "map_object.h"

struct LegoConfig;

struct WorkOrder {
    /* 0x00 */ struct WorkOrder *next;
    /* 0x04 */ int var_4;
    /* 0x08 */ int var_8;
    /* 0x0c */ int var_c;
    /* 0x10 */ int *var_10;
    /* 0x14 */ unsigned char pad_14[4];
    /* 0x18 */ int var_18;
    /* 0x1c */ int var_1c;
    /* 0x20 */ unsigned char var_20;
    /* 0x21 */ unsigned char pad_21[3];
    /* 0x24 */ int var_24;
    /* 0x28 */ int var_28;
    /* 0x2c */ unsigned char var_2c;
    /* 0x2d */ unsigned char pad_2d[3];
    /* 0x30 */ int var_30;
};

struct Rect {
    int minx;
    int miny;
    int maxx;
    int maxy;
};

struct RepairOrder {
    struct RepairOrder *next;
    unsigned char pad_4[20];
    int var_18;
    int var_1c;
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
int FUN_004996a0(void *order) { STUB(); }

// FUNCTION: LEGOLAND 0x00499720
void FUN_00499720(int *order) {
    int saved_24;
    int saved_28;
    int done;

    FUN_00499620(order);
    saved_24 = order[9];
    saved_28 = order[10];

    while (1) {
        done = FUN_004996a0(order);
        FUN_00499620(order);
        if (order[9] == saved_24 && order[10] == saved_28) {
            break;
        }
        if (done != 0) {
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x00499760
void FUN_00499760(struct Worker *worker, float value) {
    *(float *)((char *)worker + 0x38) = value;
    *(float *)((char *)worker + 0x34) = value * FLOAT_004ab480;
}

// FUNCTION: LEGOLAND 0x00499780
void FUN_00499780(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00499830
void FUN_00499830(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00499a70
void FUN_00499a70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00499ac0
void FUN_00499ac0(void *arg, void *order) { STUB(); }

// FUNCTION: LEGOLAND 0x00499b60
void FUN_00499b60(void *arg, void *order) { STUB(); }

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
        if (*(unsigned short *)((char *)current + 0xc) == 0x10) {
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
void FUN_00499d60(void *order) { STUB(); }

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
void FUN_00499eb0(void *order) { STUB(); }

// FUNCTION: LEGOLAND 0x00499f40
void FUN_00499f40(void) { STUB(); }

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

// FUNCTION: LEGOLAND 0x0049a0d0
LEGO_EXPORT void PutWorkerOnRide(void) { STUB(); }

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
LEGO_EXPORT void GenerateGardener(void *object, int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x0049a2d0
LEGO_EXPORT void RemoveAGardener(struct Worker *worker) { STUB(); }

// FUNCTION: LEGOLAND 0x0049a340
LEGO_EXPORT void GenerateMechanic(void *object, int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x0049a430
LEGO_EXPORT void RemoveAMechanic(struct Worker *worker) { STUB(); }

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
LEGO_EXPORT void Gardener_Build(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049a7f0
LEGO_EXPORT void Mechanic_Build(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049ab30
LEGO_EXPORT void WorkOrderBuildObject(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049ac50
int FUN_0049ac50(int param) { STUB(); }

// FUNCTION: LEGOLAND 0x0049b0b0
LEGO_EXPORT int RenderWorkerInterfaceGFX(void) {
    FUN_0049ac50(0);
    FUN_0049ac50(1);
    return IterateNoneWorkersRepairOrders();
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
void FUN_0049b270(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049b2c0
LEGO_EXPORT void SetGardenerWorkOrderAtPostion(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049b350
void FUN_0049b350(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049b430
LEGO_EXPORT void SetMechanicsOrderAtPostion(void) { STUB(); }

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
void FUN_0049b690(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049b6e0
void FUN_0049b6e0(void *order) { STUB(); }

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
LEGO_EXPORT int IterateNoneWorkersRepairOrders(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049b930
LEGO_EXPORT void AddRepairOrderForObject(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049ba10
LEGO_EXPORT void Garderner_Repair(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049bd20
LEGO_EXPORT void Mechanics_Repair(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049c140
void FUN_0049c140(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049c3c0
void FUN_0049c3c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049c630
void FUN_0049c630(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049c8b0
void FUN_0049c8b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049cb20
void FUN_0049cb20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049cc10
void FUN_0049cc10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049cd10
void FUN_0049cd10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049ce00
void FUN_0049ce00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049cf00
void FUN_0049cf00(void) { STUB(); }

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
