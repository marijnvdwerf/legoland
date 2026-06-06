#include "legoland.h"

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
    struct WorkOrder *next;
    unsigned char pad_4[4];
    int var_8;
    int var_c;
    int *var_10;
    unsigned char pad_14[4];
    int var_18;
    int var_1c;
    unsigned char var_20;
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

extern void *DAT_0079abfc;
extern void *DAT_0079a8a8;
extern void *DAT_0079a8ac;
extern void *DAT_0079a8b0;
extern void *DAT_0079a8b4;
extern int DAT_0079a8b8;
extern int DAT_0079a8bc;
extern void *DAT_0079a8c0;
extern void *DAT_0079a8c4;
extern int DAT_0079a8c8;
extern int DAT_0079a8cc;
extern unsigned int DAT_0079a8d0;
extern void *DAT_0079a8d4;
extern unsigned int DAT_0080ff64;
extern int DAT_00832bd0;
extern unsigned int DAT_00832bd8;
extern float FLOAT_004ab480;
extern struct LegoConfig *lpConfig;

extern void *FUN_004a020e(unsigned int count, unsigned int size);
extern void FUN_0049e4d0(void *block);

// FUNCTION: LEGOLAND 0x00499530
void LoadWorkerInterfaceGFX(void) {
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
    struct WorkOrder *node = FUN_004a020e(0x3c, 1);
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
    struct WorkOrder *node = FUN_004a020e(0x3c, 1);
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
void FUN_00499620(void *order) { STUB(); }

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
    struct Worker *current = DAT_0079a8a8;
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
    FUN_0049e4d0(order->var_10);
    FUN_0049e4d0(order);
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
    struct Worker *current = DAT_0079a8a8;
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
    struct Worker *node = DAT_0079a8ac;
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
void ControlWorkers(void) {
    FUN_0049a010();
    FUN_00499fb0();
}

// FUNCTION: LEGOLAND 0x0049a080
void RenderWorkers(void) {
    struct Worker *esi = DAT_0079a8a8;
    if (esi) {
        do {
            if ((esi->flags & 0x20) == 0) {
                SortBlokeIn3D((struct Bloke *)esi);
            }
            esi = esi->next;
        } while (esi);
    }

    esi = DAT_0079a8ac;
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
void PutWorkerOnRide(void) { STUB(); }

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
void RefundGardener(void) {
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
void RefundMechanic(void) {
    AddBricks(30);
}

// FUNCTION: LEGOLAND 0x0049a1a0
void GenerateGardener(void *object, int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x0049a2d0
void RemoveAGardener(struct Worker *worker) { STUB(); }

// FUNCTION: LEGOLAND 0x0049a340
void GenerateMechanic(void *object, int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x0049a430
void RemoveAMechanic(struct Worker *worker) { STUB(); }

// FUNCTION: LEGOLAND 0x0049a480
void Gardener_Idle(struct Worker *worker) {
    worker->state = 0xe;
    FUN_00499d00(worker);
}

// FUNCTION: LEGOLAND 0x0049a4a0
void FUN_0049a4a0(struct Worker *worker) {
    worker->state = 0xe;
}

// FUNCTION: LEGOLAND 0x0049a4b0
void Mechanic_Idle(struct Worker *worker) {
    worker->state = 0xe;
    FUN_00499d30(worker);
}

// FUNCTION: LEGOLAND 0x0049a4d0
void FUN_0049a4d0(struct Worker *worker) {
    worker->state = 0xe;
}

// FUNCTION: LEGOLAND 0x0049a4e0
void Gardener_Build(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049a7f0
void Mechanic_Build(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049ab30
void WorkOrderBuildObject(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049ac50
int FUN_0049ac50(int param) { STUB(); }

// FUNCTION: LEGOLAND 0x0049b0b0
int RenderWorkerInterfaceGFX(void) {
    FUN_0049ac50(0);
    FUN_0049ac50(1);
    return IterateNoneWorkersRepairOrders();
}

// FUNCTION: LEGOLAND 0x0049b0d0
unsigned int FUN_0049b0d0(struct Worker *a, void *b) {
    unsigned char *counter = (unsigned char *)a + 0x11;
    *counter = *counter + 1;

    if (*counter == (*((unsigned char *)b + 0x2c) >> 2)) {
        int result = FindObjectsPower(b);
        *(unsigned short *)((char *)a + 0xc) &= 0xfdff;
        if (result > 0) {
            DAT_00832bd0 += result;
            if (DAT_00832bd8) {
                FUN_0045a060();
            }
        }
    }

    return *counter;
}

// FUNCTION: LEGOLAND 0x0049b130
struct WorkOrder *GetGardenerWorkOrderAt(int x, int y) {
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
struct WorkOrder *GetMechanicWorkOrderAt(int pos_x, int pos_y) {
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
void EraseMechanicOrder(struct WorkOrder *order) {
    if (order->var_18 != 0) {
        NewLongTermAction((struct Bloke *)order->var_1c, 0x11);
    }
    if (*(unsigned int *)((char *)order + 4) == DAT_0080ff64) {
        if (DAT_0079a8d0 != 0) {
            DAT_0079a8d0 = 0;
        }
    }
    if (order->var_20 == 1) {
        FUN_0045e4a0(*(int *)((char *)order + 4), (char *)order + 8);
    }
    FUN_00499eb0(order);
}

// FUNCTION: LEGOLAND 0x0049b230
void EraseGardenerOrder(struct WorkOrder *order) {
    if (order->var_18 != 0) {
        NewLongTermAction((struct Bloke *)order->var_1c, 0x10);
    }
    if (order->var_20 == 1) {
        FUN_0045e4a0(*(int *)((char *)order + 4), (char *)order + 8);
    }
    FUN_00499e30(order);
}

// FUNCTION: LEGOLAND 0x0049b270
void FUN_0049b270(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049b2c0
void SetGardenerWorkOrderAtPostion(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049b350
void FUN_0049b350(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049b430
void SetMechanicsOrderAtPostion(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049b510
void ClearAMechanicsWorkList(int value) {
    struct WorkOrder *head = DAT_0079a8c0;
    if (head == 0) {
        return;
    }
    {
        struct WorkOrder *node = head;
        while (node != 0) {
            if (node->var_1c == value) {
                node->var_18 = 0;
                *(int *)((char *)node + 0x30) = 0;
                node->var_1c = 0;
                DAT_0079a8c8++;
            }
            node = node->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x0049b550
void ClearAGardenersWorkList(int param) {
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
void RemoveRepairOrderAT(struct Worker *worker, unsigned int x, unsigned int y) {
    if ((*(unsigned int *)((char *)worker + 0x1c) & 0x200000) && (*(unsigned int *)((char *)lpConfig + 0x38) != 0)) {
        RemoveGardenersWorkOrderAt(x, y);
    } else if ((*(unsigned int *)((char *)worker + 0x1c) & 0x400000) && (*(unsigned int *)((char *)lpConfig + 0x34) != 0)) {
        RemoveMechanicsWorkOrderAt(x, y);
    } else {
        RemoveNoneWorkersRepairOrderAT(x, y);
    }
}

// FUNCTION: LEGOLAND 0x0049b5f0
void RemoveGardenersWorkOrderAt(unsigned int x, unsigned int y) {
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
void RemoveMechanicsWorkOrderAt(unsigned int x, unsigned int y) {
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
void RemoveNoneWorkersRepairOrderAT(unsigned int x, unsigned int y) {
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
int IterateNoneWorkersRepairOrders(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049b930
void AddRepairOrderForObject(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049ba10
void Garderner_Repair(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0049bd20
void Mechanics_Repair(void) { STUB(); }

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
    while (DAT_0079a8a8 != NULL) {
        RemoveAGardener(DAT_0079a8a8);
    }
    while (DAT_0079a8ac != NULL) {
        RemoveAMechanic(DAT_0079a8ac);
    }
}
