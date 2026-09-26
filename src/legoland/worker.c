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
#include "image_sprite.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "objclass.h"
#include "pathfind.h"
#include "popupinfo.h"
#include "print_sprite.h"
#include "screens.h"
#include "sound_music.h"
#include "tilemap.h"
#include "worker.h"

// FUNCTION: LEGOLAND 0x00499530
LEGO_EXPORT void LoadWorkerInterfaceGFX(void) {
    // STRING: LEGOLAND 0x004b9bd0
    DAT_0079abfc = ElemID("NORMAL PATH TILES");
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
WorkOrder *FUN_00499570(void) {
    WorkOrder *node = calloc(sizeof(WorkOrder), 1);
    // STRING: LEGOLAND 0x004bff88
    DBPrintf("Allocated Workorder %x\n", node);
    if (DAT_0079a8b4 == NULL) {
        DAT_0079a8b4 = node;
        DAT_0079a8b0 = node;
        DAT_0079a8b8++;
    } else {
        DAT_0079a8b4->next = node;
        DAT_0079a8b4 = node;
        DAT_0079a8b8++;
    }
    return node;
}

// FUNCTION: LEGOLAND 0x004995d0
WorkOrder *FUN_004995d0(void) {
    WorkOrder *node = calloc(sizeof(WorkOrder), 1);
    if (DAT_0079a8c4 == NULL) {
        DAT_0079a8c4 = node;
        DAT_0079a8c8++;
        DAT_0079a8c0 = node;
        return node;
    }
    DAT_0079a8c4->next = node;
    DAT_0079a8c8++;
    DAT_0079a8c4 = node;
    return node;
}

// FUNCTION: LEGOLAND 0x00499620
void FUN_00499620(WorkOrder *order) {
    if (order->walk_dir == 1) {
        if (order->step_x < (order->footprints->x1 - order->footprints->x0) + 1) {
            order->step_x++;
        } else {
            order->walk_dir = 7;
        }
    }
    if (order->walk_dir == 7) {
        if (order->step_y < (order->footprints->y1 - order->footprints->y0) + 1) {
            order->step_y++;
        } else {
            order->walk_dir = 5;
        }
    }
    if (order->walk_dir == 5) {
        if (order->step_x != -1) {
            order->step_x--;
            return;
        }
        order->walk_dir = 3;
        return;
    }
    if (order->walk_dir == 3) {
        if (order->step_y != -1) {
            order->step_y--;
            return;
        }
        order->walk_dir = 1;
    }
}

// FUNCTION: LEGOLAND 0x004996a0
int FUN_004996a0(WorkOrder *order) {
    MapElement *cell;
    int x = order->step_x + order->pos.x + order->footprints->x0;
    int y = order->footprints->y1 - order->step_y + order->pos.y;

    if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
        cell = &GameMap[y][x];
    } else {
        cell = NULL;
    }
    if (cell == NULL) {
        return 0;
    }
    if ((cell->flags & 0x88) != 0 && cell->field_0->data != DAT_007fd624) {
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00499720
void FUN_00499720(WorkOrder *order) {
    int saved_24;
    int saved_28;
    int done;

    FUN_00499620(order);
    saved_24 = order->step_x;
    saved_28 = order->step_y;

    while (1) {
        done = FUN_004996a0(order);
        FUN_00499620(order);
        if (order->step_x == saved_24 && order->step_y == saved_28) {
            break;
        }
        if (done != 0) {
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x00499760
void FUN_00499760(WorkOrder *order, float value) {
    order->bricks_per_step = value;
    order->bricks = value * 1.5f;
}

// FUNCTION: LEGOLAND 0x00499780
WorkOrder *FUN_00499780(Element *element, int *coords, int mode) {
    WorkOrder *order;
    Ride *ride;
    float cost;

    if (DAT_0079a8b8 >= 0xe1) {
        return NULL;
    }
    order = FUN_00499570();
    order->element = element;
    order->pos.x = coords[0];
    order->pos.y = coords[1];
    order->assigned = 0;
    ride = element->ride;
    order->footprints = malloc(sizeof(Footprint));
    *order->footprints = ride->footprint;
    order->count = 1;
    order->type = mode;
    order->walk_dir = 1;
    order->step_y = -1;
    FUN_00499720(order);
    if (mode == 2) {
        cost = GetObjCost(ride) / (float)ride->durability;
        FUN_00499760(order, cost);
    }
    return order;
}

// FUNCTION: LEGOLAND 0x00499830
WorkOrder *FUN_00499830(Element *element, int *coords, int mode) {
    WorkOrder *order;
    Ride *ride;
    Cursor *cursor;
    Footprint *fp;
    int dx;
    int dy;
    MapElement *cell;
    int level;
    float cost;

    if (DAT_0079a8c8 >= 0xe1) {
        return NULL;
    }
    order = FUN_004995d0();
    order->element = element;
    order->pos.x = coords[0];
    order->pos.y = coords[1];
    order->assigned = 0;
    ride = element->ride;
    if (mode == 2) {
        order->footprints = malloc(sizeof(Footprint));
        order->count = 1;
        *order->footprints = ride->footprint;
    } else {
        order->count = 0;
        for (cursor = &EditCursor; cursor != NULL; cursor = cursor->next) {
            if ((cursor->field_1828 & 0x3000) == 0) {
                for (fp = &cursor->footprint; fp != NULL; fp = fp->next) {
                    order->count++;
                }
            }
        }
        order->footprints = malloc(order->count * sizeof(Footprint));
        order->count = 0;
        for (cursor = &EditCursor; cursor != NULL; cursor = cursor->next) {
            if ((cursor->field_1828 & 0x3000) == 0) {
                dx = cursor->field_1404 - order->pos.x;
                dy = cursor->field_1408 - order->pos.y;
                for (fp = &cursor->footprint; fp != NULL; fp = fp->next) {
                    order->footprints[order->count] = *fp;
                    order->footprints[order->count].x0 += dx;
                    order->footprints[order->count].y0 += dy;
                    order->footprints[order->count].x1 += dx;
                    order->footprints[order->count].y1 += dy;
                    order->count++;
                }
            }
        }
    }
    order->type = mode;
    order->walk_dir = 1;
    order->step_y = -1;
    FUN_00499720(order);
    if (mode == 2) {
        if (coords[0] >= 0 && coords[0] < lpConfig->width && coords[1] >= 0 && coords[1] < lpConfig->height) {
            cell = &GameMap[coords[1]][coords[0]];
        } else {
            cell = NULL;
        }
        level = cell->field_11;
        cost = GetObjRepairCost(ride, level) / (float)(ride->durability - level);
        FUN_00499760(order, cost);
    }
    return order;
}

// FUNCTION: LEGOLAND 0x00499a70
Point FUN_00499a70(WorkOrder *order) {
    Footprint *fp = order->footprints;
    int x0 = fp->x0;
    int y0 = fp->y0;
    int dx = fp->x1 - x0;
    int dy = fp->y1 - y0;
    Point result;

    if (dx >= dy) {
        result.x = (dx + (order->pos.x + x0) * 2) * 0x80;
        result.y = (order->pos.y + y0) * 0x100;
    } else {
        result.x = (order->pos.x + x0) * 0x100;
        result.y = (dy + (order->pos.y + y0) * 2) * 0x80;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00499ac0
void FUN_00499ac0(Bloke *worker, WorkOrder *order) {
    worker->order = order;
    if (order->type == 1) {
        worker->goal.x = (order->footprints->x1 + order->pos.x * 2 + order->footprints->x0) * 0x80;
        worker->goal.y = (order->footprints->y1 + order->pos.y * 2 + order->footprints->y0) * 0x80;
    } else {
        worker->goal.x = (order->step_x + order->pos.x + order->footprints->x0) * 0x100;
        worker->goal.y = ((order->footprints->y1 - order->step_y) + order->pos.y) * 0x100;
    }
    order->worker = worker;
    order->assigned = 1;
    if (order->type == 1) {
        NewLongTermAction(worker, 0x12);
        return;
    }
    NewLongTermAction(worker, 0x15);
}

// FUNCTION: LEGOLAND 0x00499b60
void FUN_00499b60(Bloke *worker, WorkOrder *order) {
    Point goal;

    worker->order = order;
    if (order->type == 1) {
        goal = FUN_00499a70(order);
        worker->goal.x = goal.x;
    } else {
        worker->goal.x = (order->step_x + order->pos.x + order->footprints->x0) * 0x100;
        goal.y = ((order->pos.y - order->step_y) + order->footprints->y1) * 0x100;
    }
    worker->goal.y = goal.y;
    order->worker = worker;
    order->assigned = 1;
    if (order->type == 1) {
        NewLongTermAction(worker, 0x13);
        return;
    }
    NewLongTermAction(worker, 0x16);
}

// FUNCTION: LEGOLAND 0x00499be0
WorkOrder *FUN_00499be0(WorkOrder *list, Bloke *worker) {
    WorkOrder *result = NULL;
    int bestDist = 0x7fffffff;

    if (list != NULL) {
        WorkOrder *cur = list;
        do {
            if (cur->assigned == 0) {
                int dx = (worker->pos.x >> 8) - cur->pos.x;
                int dy = (worker->pos.y >> 8) - cur->pos.y;
                int dist2 = dx * dx + dy * dy;
                if (dist2 < bestDist) {
                    bestDist = dist2;
                    result = cur;
                }
            }
            cur = cur->next;
        } while (cur != NULL);
    }

    return result;
}

// FUNCTION: LEGOLAND 0x00499c40
Bloke *FUN_00499c40(int *coords) {
    Bloke *current = GardenerList;
    Bloke *best = NULL;
    int minDist = 0x7fffffff;

    while (current != NULL) {
        if (current->action == 0x10) {
            int dx = (current->pos.x >> 8) - coords[0];
            int dy = (current->pos.y >> 8) - coords[1];
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
int FUN_00499d00(Bloke *worker) {
    WorkOrder *order = FUN_00499be0(DAT_0079a8b0, worker);
    if (order == NULL) {
        return 0;
    }
    FUN_00499ac0(worker, order);
    return order->type;
}

// FUNCTION: LEGOLAND 0x00499d30
int FUN_00499d30(Bloke *worker) {
    WorkOrder *order = FUN_00499be0(DAT_0079a8c0, worker);
    if (order == NULL) {
        return 0;
    }
    FUN_00499b60(worker, order);
    return order->type;
}

// FUNCTION: LEGOLAND 0x00499d60
void FUN_00499d60(WorkOrder *order) {
    WorkOrder *cur = DAT_0079a8b0;
    WorkOrder *next;

    if (cur == NULL) {
        return;
    }
    // STRING: LEGOLAND 0x004c0034
    DBPrintf("unlinking gardener order %x at (%d, %d)\n", order, order->pos.x, order->pos.y);
    if (DAT_0079a8b4 == order) {
        // STRING: LEGOLAND 0x004c0014
        DBPrintf("   Last in list,  Next = %x\n", order->next);
    }
    if (DAT_0079a8b0 == order) {
        // STRING: LEGOLAND 0x004bfff4
        DBPrintf("   First in list, Last = %x\n", DAT_0079a8b4);
        DAT_0079a8b0 = order->next;
        if (DAT_0079a8b0 == NULL) {
            DAT_0079a8b4 = NULL;
        }
        // STRING: LEGOLAND 0x004bffcc
        DBPrintf("    Work orders START (%x), END (%x)\n", DAT_0079a8b0, DAT_0079a8b4);
        return;
    }
    for (; cur != NULL; cur = cur->next) {
        if (cur->next == order) {
            break;
        }
    }
    if (cur != NULL) {
        next = order->next;
        cur->next = next;
        if (next == NULL) {
            DAT_0079a8b4 = cur;
        }
        DBPrintf("    Work orders START (%x), END (%x)\n", DAT_0079a8b0, DAT_0079a8b4);
        return;
    }
    // STRING: LEGOLAND 0x004bffa0
    DBPrintf("    Work order not found (%x) at (%d,%d)", order->pos.x, order->pos.y);
}

// FUNCTION: LEGOLAND 0x00499e30
void FUN_00499e30(WorkOrder *order) {
    FUN_00499d60(order);
    free(order->footprints);
    free(order);
    DAT_0079a8b8--;
}

// FUNCTION: LEGOLAND 0x00499e60
void FUN_00499e60(WorkOrder *order) {
    order->assigned = 0;
    if (order->next == 0) {
        return;
    }
    if (DAT_0079a8b0 == 0) {
        return;
    }
    FUN_00499d60(order);
    DAT_0079a8b4->next = order;
    DAT_0079a8b4 = order;
    if (DAT_0079a8b0 == 0) {
        DAT_0079a8b0 = order;
    }
    order->next = 0;
}

// FUNCTION: LEGOLAND 0x00499eb0
void FUN_00499eb0(WorkOrder *order) {
    WorkOrder *cur = DAT_0079a8c0;
    WorkOrder *next;

    if (cur == NULL) {
        return;
    }
    if (cur == order) {
        DAT_0079a8c8--;
        DAT_0079a8c0 = order->next;
        if (DAT_0079a8c0 == NULL) {
            DAT_0079a8c4 = NULL;
        }
        free(order->footprints);
        free(order);
        return;
    }
    for (; cur != NULL; cur = cur->next) {
        if (cur->next == order) {
            break;
        }
    }
    if (cur != NULL) {
        DAT_0079a8c8--;
        next = order->next;
        cur->next = next;
        if (next == NULL) {
            DAT_0079a8c4 = cur;
        }
        free(order->footprints);
        free(order);
    }
}

// FUNCTION: LEGOLAND 0x00499f40
void FUN_00499f40(WorkOrder *order) {
    WorkOrder *scan = DAT_0079a8c0;
    WorkOrder *onext = order->next;

    order->assigned = 0;
    if (onext != 0 && DAT_0079a8c0 != 0) {
        if (DAT_0079a8c0 == order) {
            DAT_0079a8c0 = onext;
        } else {
            for (; scan != 0; scan = scan->next) {
                if (scan->next == order) {
                    break;
                }
            }
            if (scan != 0) {
                scan->next = onext;
            }
        }
        DAT_0079a8c4->next = order;
        DAT_0079a8c4 = order;
        if (DAT_0079a8c0 == 0) {
            DAT_0079a8c0 = order;
        }
        order->next = 0;
    }
}

// FUNCTION: LEGOLAND 0x00499fb0
void FUN_00499fb0(void) {
    Bloke *current = GardenerList;
    if (current != NULL) {
        Bloke *next;
        do {
            current->field_5c++;
            next = current->next;
            if (current->field_e == 0) {
                DoHighLevelAI(current);
            }
            if (current->field_e != 0) {
                DoLowLevelAI(current);
            }
            UpdatePerson(current);
            if (current->field_36 == 100) {
                RemoveAGardener(current);
            }
            current = next;
        } while (current != NULL);
    }
}

// FUNCTION: LEGOLAND 0x0049a010
void FUN_0049a010(void) {
    Bloke *node = MechanicList;
    while (node != 0) {
        Bloke *next = node->next;
        node->field_5c++;
        if (node->field_e == 0) {
            DoHighLevelAI(node);
        }
        if (node->field_e != 0) {
            DoLowLevelAI(node);
        }
        UpdatePerson(node);
        if (node->field_36 == 100) {
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
    Bloke *esi = GardenerList;
    if (esi) {
        do {
            if ((esi->flags & 0x20) == 0) {
                SortBlokeIn3D(esi);
            }
            esi = esi->next;
        } while (esi);
    }

    esi = MechanicList;
    if (esi) {
        do {
            if (esi->field_46 != 0 && (esi->flags & 0x20) == 0) {
                SortBlokeIn3D(esi);
            }
            esi = esi->next;
        } while (esi);
    }
}

// FUNCTION: LEGOLAND 0x0049a0d0
LEGO_EXPORT void PutWorkerOnRide(Bloke *worker, MapElement *tile) {
    RideNode *node = malloc(sizeof(RideNode));

    if (node != NULL) {
        memset(node, 0, sizeof(RideNode));
        node->rider = worker;
        node->person = worker->person;
        worker->flags |= 0x20;
        node->tile = tile->anchor;
        PutBlokeInList(tile->field_0->ride, node);
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
LEGO_EXPORT Bloke *GenerateGardener(int *coords, int in_hut) {
    Bloke *worker;
    MapElement *cell;

    if (DAT_0079a8bc >= 15) {
        return NULL;
    }
    // STRING: LEGOLAND 0x004c00c4
    DBPrintf("Generating Gardener\n");
    worker = NewBlokeWOList(2);
    cell = NULL;
    if (worker != NULL) {
        worker->field_70 = 0;
        worker->field_72 = 0;
        worker->field_74 = 0;
        worker->field_75 = 1;
        worker->field_7f = 0x18;
        worker->next = GardenerList;
        DAT_0079a8bc++;
        GardenerList = worker;
        if (in_hut != 0) {
            if (coords[0] >= 0 && coords[0] < lpConfig->width && coords[1] >= 0 && coords[1] < lpConfig->height) {
                cell = &GameMap[coords[1]][coords[0]];
            }
            // STRING: LEGOLAND 0x004c00a4
            DBPrintf("   Gardener Generated in hut\n");
            PutWorkerOnRide(worker, cell);
            coords[0] -= 2;
            coords[1]++;
            worker->dest.x = worker->pos.x = coords[0] << 8;
            worker->dest.y = worker->pos.y = coords[1] << 8;
            NewLongTermAction(worker, 5);
        } else {
            // STRING: LEGOLAND 0x004c0080
            DBPrintf("   Gardener Generated at (%d,%d)\n", coords[0], coords[1]);
            worker->pos.x = coords[0] << 8;
            worker->pos.y = coords[1] << 8;
            NewLongTermAction(worker, 0x10);
        }
        DAT_00668610 |= 0x80;
        return worker;
    }
    // STRING: LEGOLAND 0x004c0060
    DBPrintf("   Failed to Generate Gardener\n");
    return worker;
}

// FUNCTION: LEGOLAND 0x0049a2d0
LEGO_EXPORT void RemoveAGardener(Bloke *worker) {
    Bloke *prev;
    Bloke *cur = GardenerList;

    if (cur == NULL) {
        return;
    }
    if (cur == worker) {
        GardenerList = cur->next;
        free(cur);
        DAT_0079a8bc--;
        return;
    }
    for (prev = cur, cur = cur->next; cur != NULL; prev = cur, cur = cur->next) {
        if (cur == worker) {
            prev->next = cur->next;
            free(cur);
            DAT_0079a8bc--;
            return;
        }
    }
    DAT_00668610 |= 0x80;
}

// FUNCTION: LEGOLAND 0x0049a340
LEGO_EXPORT Bloke *GenerateMechanic(int *coords, int in_hut) {
    Bloke *worker;
    MapElement *cell;

    if (DAT_0079a8cc >= 15) {
        return NULL;
    }
    worker = NewBlokeWOList(3);
    worker->field_70 = 0;
    worker->field_72 = 0;
    worker->field_74 = 0;
    worker->field_75 = 1;
    worker->field_7f = 0x18;
    worker->next = MechanicList;
    worker->field_46 = 1;
    MechanicList = worker;
    DAT_0079a8cc++;
    if (in_hut != 0) {
        if (coords[0] >= 0 && coords[0] < lpConfig->width && coords[1] >= 0 && coords[1] < lpConfig->height) {
            cell = &GameMap[coords[1]][coords[0]];
        } else {
            cell = NULL;
        }
        PutWorkerOnRide(worker, cell);
        worker->dest.x = worker->pos.x = (coords[0] << 8) - 0x80;
        worker->dest.y = worker->pos.y = coords[1] << 8;
        NewLongTermAction(worker, 5);
        return worker;
    }
    worker->pos.x = coords[0] << 8;
    worker->pos.y = coords[1] << 8;
    NewLongTermAction(worker, 0x11);
    return worker;
}

// FUNCTION: LEGOLAND 0x0049a430
LEGO_EXPORT void RemoveAMechanic(Bloke *worker) {
    Bloke *prev;
    Bloke *cur = MechanicList;

    if (cur == NULL) {
        return;
    }
    if (cur == worker) {
        MechanicList = cur->next;
        free(cur);
        DAT_0079a8cc--;
        return;
    }
    for (prev = cur, cur = cur->next; cur != NULL; prev = cur, cur = cur->next) {
        if (cur == worker) {
            prev->next = cur->next;
            free(cur);
            DAT_0079a8cc--;
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x0049a480
LEGO_EXPORT void Gardener_Idle(Bloke *worker) {
    worker->field_e = 0xe;
    FUN_00499d00(worker);
}

// FUNCTION: LEGOLAND 0x0049a4a0
void FUN_0049a4a0(Bloke *worker) {
    worker->field_e = 0xe;
}

// FUNCTION: LEGOLAND 0x0049a4b0
LEGO_EXPORT void Mechanic_Idle(Bloke *worker) {
    worker->field_e = 0xe;
    FUN_00499d30(worker);
}

// FUNCTION: LEGOLAND 0x0049a4d0
void FUN_0049a4d0(Bloke *worker) {
    worker->field_e = 0xe;
}

// FUNCTION: LEGOLAND 0x0049a4e0
LEGO_EXPORT void Gardener_Build(Bloke *worker) {
    int dx;
    int dy;
    Point dest;
    int coords[2];
    WorkOrder *order;

    switch (worker->param_action) {
    case 0:
        worker->dest.x = worker->goal.x;
        worker->dest.y = worker->goal.y;
        worker->field_73 = CalcMoveLine(worker->pos, worker->goal, &worker->nav) + 0x10;
        worker->field_e = 0xc;
        NewDirForAction(worker, (worker->field_73 >> 5) + 3);
        worker->param_action = 0xb;
        return;
    case 0xb:
        dx = worker->dest.x - worker->pos.x;
        dy = worker->dest.y - worker->pos.y;
        worker->param_action = dx * dx + dy * dy < 0x9000 ? 107 : 100;
        return;
    case 100:
        switch (FUN_00482710(&worker->pos.x, &worker->goal.x, &dest.x)) {
        case 0:
            FUN_00499e60(worker->order);
            worker->action = 0x10;
            worker->param_action = 100;
            worker->field_5c = 0x70;
            worker->field_e = 4;
            return;
        case 1:
            worker->dest.x = dest.x;
            worker->dest.y = dest.y;
            worker->field_73 = CalcMoveLine(worker->pos, dest, &worker->nav) + 0x10;
            worker->field_e = 0xc;
            NewDirForAction(worker, (worker->field_73 >> 5) + 3);
            if ((worker->field_64 & 1) != 0) {
                worker->param_action = 0x6a;
            }
            return;
        case 2:
            worker->dest.x = dest.x;
            worker->dest.y = dest.y;
            worker->field_73 = CalcMoveLine(worker->pos, dest, &worker->nav) + 0x10;
            worker->field_e = 0xc;
            NewDirForAction(worker, (worker->field_73 >> 5) + 3);
            worker->param_action = ~worker->field_64 & 1 | 0x6a;
            return;
        }
        return;
    case 0x6a:
        worker->field_5c = 0x70;
        worker->field_e = 4;
        worker->param_action = 100;
        return;
    case 0x6b:
        worker->flags |= 0x100;
        BlokeSetAnim(worker, 1);
        BlokeSetFrame(worker, 0x28);
        worker->param_action++;
        return;
    case 0x6c:
        if (PlayBlokeAnim(worker) != 0) {
            BlokeWalkAnim(worker);
            worker->flags &= 0xfeff;
            worker->param_action++;
        }
        return;
    case 0x6d:
        order = worker->order;
        worker->flags &= 0xfff7;
        coords[0] = order->pos.x;
        coords[1] = order->pos.y;
        if (BuildObject(order->element, coords) != 0) {
            FUN_00499e30(worker->order);
            if (FUN_00499d00(worker) == 0) {
                NewLongTermAction(worker, 0x10);
            }
        } else {
            FUN_00499e60(worker->order);
            NewLongTermAction(worker, 0x10);
            FUN_004735e0(1);
        }
        return;
    }
}

// FUNCTION: LEGOLAND 0x0049a7f0
LEGO_EXPORT void Mechanic_Build(Bloke *worker) {
    int dx;
    int dy;
    Point dest;
    int coords[2];
    WorkOrder *order;
    MapElement *cell;

    switch (worker->param_action) {
    case 0:
        worker->dest.x = worker->goal.x;
        worker->dest.y = worker->goal.y;
        worker->field_73 = CalcMoveLine(worker->pos, worker->goal, &worker->nav) + 0x10;
        worker->field_e = 0xc;
        NewDirForAction(worker, (worker->field_73 >> 5) + 3);
        worker->param_action = 0xb;
        return;
    case 0xb:
        dx = worker->dest.x - worker->pos.x;
        dy = worker->dest.y - worker->pos.y;
        worker->param_action = dx * dx + dy * dy < 0x10000 ? 107 : 101;
        return;
    case 0x65:
        switch (FUN_00482710(&worker->pos.x, &worker->goal.x, &dest.x)) {
        case 0:
            FUN_00499f40(worker->order);
            worker->action = 0x11;
            worker->param_action = 0x65;
            worker->field_5c = 0x70;
            worker->field_e = 4;
            return;
        case 1:
            worker->dest.x = dest.x;
            worker->dest.y = dest.y;
            worker->field_73 = CalcMoveLine(worker->pos, dest, &worker->nav) + 0x10;
            worker->field_e = 0xc;
            NewDirForAction(worker, (worker->field_73 >> 5) + 3);
            if ((worker->field_64 & 1) != 0) {
                worker->param_action = 0x6a;
            }
            return;
        case 2:
            worker->dest.x = dest.x;
            worker->dest.y = dest.y;
            worker->field_73 = CalcMoveLine(worker->pos, dest, &worker->nav) + 0x10;
            worker->field_e = 0xc;
            NewDirForAction(worker, (worker->field_73 >> 5) + 3);
            worker->param_action = ~worker->field_64 & 1 | 0x6a;
            return;
        }
        return;
    case 0x6a:
        worker->field_5c = 0x70;
        worker->field_e = 4;
        worker->param_action = 0x65;
        return;
    case 0x6b:
        order = worker->order;
        worker->flags |= 8;
        coords[0] = order->pos.x;
        coords[1] = order->pos.y;
        if (BuildObject(order->element, coords) != 0) {
            worker->field_46 = 0;
            worker->param_action++;
            return;
        }
        FUN_00499f40(worker->order);
        NewLongTermAction(worker, 0x11);
        worker->order->no_bricks = 1;
        return;
    case 0x6c:
        worker->order->no_bricks = 0;
        coords[0] = worker->order->pos.x;
        coords[1] = worker->order->pos.y;
        if (coords[0] >= 0 && coords[0] < lpConfig->width && coords[1] >= 0 && coords[1] < lpConfig->height) {
            cell = &GameMap[coords[1]][coords[0]];
        } else {
            cell = NULL;
        }
        if ((cell->flags & 0x20) == 0) {
            worker->param_action++;
        }
        return;
    case 0x6d:
        worker->flags &= 0xfff7;
        worker->field_46 = 1;
        FUN_00499eb0(worker->order);
        if (FUN_00499d30(worker) == 0) {
            NewLongTermAction(worker, 0x11);
        }
        return;
    }
}

// FUNCTION: LEGOLAND 0x0049ab30
LEGO_EXPORT int WorkOrderBuildObject(Element *element, Point *pos) {
    Ride *ride = element->ride;
    WorkOrder *order;
    Bloke *worker;
    int result;
    SampleSource source;

    if (element == DAT_0080ff64 && DAT_0079a8d0 != 0) {
        return 0;
    }
    if ((ride->flags & 0x200000) != 0 && lpConfig->field_38 != 0) {
        order = FUN_00499780(element, &pos->x, 1);
        result = order != NULL;
        if (result != 0) {
            SetObjRectFlags(element, pos, 0x800);
            worker = FUN_00499c40(&pos->x);
            if (worker != NULL) {
                FUN_00499ac0(worker, order);
            }
        }
    } else {
        if (GetBrickCount() < GetObjCost(ride)) {
            FUN_004735e0(3);
            return 0;
        }
        FUN_0045e300(element, pos);
        FUN_0045d770(&EditCursor);
        result = BuildObject(element, &pos->x);
    }
    if (result != 0 && DAT_00667cd8 == 0) {
        source.field_8 = pos->x;
        source.type = 2;
        source.field_c = pos->y;
        PlayInstanceOfSample(DAT_004b9248, 0, 1, &source);
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0049ac50
void FUN_0049ac50(int mechanics) {
    int *ids = DAT_0079abfc->data;
    unsigned int colour;
    WorkOrder *order;
    Ride *ride;
    Footprint *fp;
    int i;
    MapRect r;
    int edge;
    int side;
    short width;
    short height;
    int left;
    int right;
    int top;
    int x;
    int y;
    Point pt;
    int bounds[4];

    if (mechanics != 0) {
        colour = 0xe04040;
        order = DAT_0079a8c0;
    } else {
        colour = 0x40e040;
        order = DAT_0079a8b0;
    }
    for (; order != NULL; order = order->next) {
        ride = order->element->ride;
        for (i = 0; i < order->count; i++) {
            fp = &order->footprints[i];
            r.x0 = fp->x0 + order->pos.x;
            r.y0 = fp->y0 + order->pos.y;
            r.y1 = fp->y1 + order->pos.y;
            r.x1 = fp->x1 + order->pos.x;
            for (pt.y = r.y0; pt.y <= r.y1; pt.y++) {
                for (pt.x = r.x0; pt.x <= r.x1; pt.x++) {
                    GetTileBounds(&pt, bounds);
                    if (order->assigned != 0) {
                        PrintSprite(TileSpriteArray[(DAT_00805f48 & 0xff) + *DAT_00801a6c], bounds[0], bounds[1], colour, 0);
                    } else {
                        PrintSprite(TileSpriteArray[(DAT_00805f48 & 0xff) + *DAT_00801a6c], bounds[0], bounds[1], 1, 0);
                    }
                }
            }
            if (i == 0) {
                for (pt.y = r.y0; pt.y <= r.y1; pt.y++) {
                    edge = (pt.y != r.y1) << 2 | (pt.y != r.y0);
                    for (pt.x = r.x0; pt.x <= r.x1; pt.x++) {
                        side = ((pt.x != r.x0) << 2 | (pt.x != r.x1)) << 1;
                        GetTileBounds(&pt, bounds);
                        if (order->assigned != 0) {
                            PrintSprite(TileSpriteArray[((side | edge) + 3 & 0xff) + *ids], bounds[0], bounds[1], colour | 0x1f1f80, 0);
                        } else {
                            PrintSprite(TileSpriteArray[((side | edge) + 3 & 0xff) + *ids], bounds[0], bounds[1], 0x1f1f81, 0);
                        }
                    }
                }
                width = ride->icon->width;
                height = ride->icon->height;
                pt.x = r.x0;
                pt.y = r.y1;
                GetTileBounds(&pt, bounds);
                left = bounds[0];
                pt.x = r.x1;
                pt.y = r.y0;
                GetTileBounds(&pt, bounds);
                right = bounds[2];
                if (right - left > width * 2) {
                    pt.x = r.x0;
                    pt.y = r.y0;
                    GetTileBounds(&pt, bounds);
                    top = bounds[1];
                    pt.x = r.x1;
                    pt.y = r.y1;
                    GetTileBounds(&pt, bounds);
                    PrintSprite(ride->icon, (right - width + left) / 2, (bounds[3] - height + top) / 2, 0, 0);
                }
                pt.x = r.x0;
                pt.y = r.y1;
                GetTileBounds(&pt, bounds);
                left = bounds[0];
                pt.x = r.x1;
                pt.y = r.y0;
                GetTileBounds(&pt, bounds);
                right = bounds[2];
                pt.x = r.x0;
                pt.y = r.y0;
                GetTileBounds(&pt, bounds);
                top = bounds[1];
                pt.x = r.x1;
                pt.y = r.y1;
                GetTileBounds(&pt, bounds);
                height = DAT_007fdeb0->height;
                x = (right + left) / 2 + 10;
                y = (bounds[3] - height + top) / 2;
                if (mechanics != 0) {
                    if (order->no_bricks != 0) {
                        LLSNextFrame(*DAT_007fdeb0->lls);
                        PrintSprite(DAT_007fdeb0, x, y, 0, 0);
                    } else {
                        PrintSprite(DAT_007fe004, x, y, 0, 0);
                    }
                }
            }
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
unsigned int FUN_0049b0d0(MapElement *tile, Ride *ride) {
    tile->field_11 = tile->field_11 + 1;

    if (tile->field_11 == (ride->durability >> 2)) {
        int result = FindObjectsPower(ride);
        tile->flags &= 0xfdff;
        if (result > 0) {
            MapStats.field_3d0 += result;
            if (MapStats.field_3d8) {
                FUN_0045a060();
            }
        }
    }

    return tile->field_11;
}

// FUNCTION: LEGOLAND 0x0049b130
LEGO_EXPORT WorkOrder *GetGardenerWorkOrderAt(int x, int y) {
    WorkOrder *current = DAT_0079a8b0;

    while (current != NULL) {
        Footprint *rect = current->footprints;

        if (rect->x0 + current->pos.x > x) {
            current = current->next;
            continue;
        }
        if (x > rect->x1 + current->pos.x) {
            current = current->next;
            continue;
        }
        if (rect->y0 + current->pos.y > y) {
            current = current->next;
            continue;
        }
        if (y <= rect->y1 + current->pos.y) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// FUNCTION: LEGOLAND 0x0049b180
LEGO_EXPORT WorkOrder *GetMechanicWorkOrderAt(int pos_x, int pos_y) {
    WorkOrder *node = DAT_0079a8c0;
    if (node == 0) {
        return 0;
    }

    while (node != 0) {
        Footprint *box = node->footprints;
        if (node->pos.x + box->x0 <= pos_x) {
            if (pos_x <= node->pos.x + box->x1) {
                if (node->pos.y + box->y0 <= pos_y) {
                    if (pos_y <= node->pos.y + box->y1) {
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
LEGO_EXPORT void EraseMechanicOrder(WorkOrder *order) {
    if (order->assigned != 0) {
        NewLongTermAction(order->worker, 0x11);
    }
    if (order->element == DAT_0080ff64) {
        if (DAT_0079a8d0 != 0) {
            DAT_0079a8d0 = 0;
        }
    }
    if (order->type == 1) {
        FUN_0045e4a0(order->element, &order->pos.x);
    }
    FUN_00499eb0(order);
}

// FUNCTION: LEGOLAND 0x0049b230
LEGO_EXPORT void EraseGardenerOrder(WorkOrder *order) {
    if (order->assigned != 0) {
        NewLongTermAction(order->worker, 0x10);
    }
    if (order->type == 1) {
        FUN_0045e4a0(order->element, &order->pos.x);
    }
    FUN_00499e30(order);
}

// FUNCTION: LEGOLAND 0x0049b270
void FUN_0049b270(Ride *ride, TileId tile) {
    int x = tile.pos.x;
    int y = tile.pos.y;
    WorkOrder *order;

    order = GetMechanicWorkOrderAt(x, y);
    if (order != NULL) {
        EraseMechanicOrder(order);
    }
    order = GetGardenerWorkOrderAt(x, y);
    if (order != NULL) {
        EraseGardenerOrder(order);
    }
}

// FUNCTION: LEGOLAND 0x0049b2c0
LEGO_EXPORT int SetGardenerWorkOrderAtPostion(Bloke *worker, int x, int y) {
    WorkOrder *order = GetGardenerWorkOrderAt(x, y);

    if (order == NULL) {
        if (FUN_00499d00(worker) == 0) {
            NewLongTermAction(worker, 0x10);
            return 1;
        }
    } else {
        if (order->assigned != 0) {
            if (order->type == 1 && order->worker->param_action < 0x6b) {
                NewLongTermAction(order->worker, 0x10);
            } else if (order->type == 2 && order->worker->param_action < 9) {
                NewLongTermAction(order->worker, 0x10);
            } else {
                return 0;
            }
        }
        FUN_00499ac0(worker, order);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0049b350
MapElement *FUN_0049b350(Bloke *worker, int x, int y) {
    const int (*offset)[2];
    MapElement *tile;
    MapElement *target;
    Ride *ride;
    int tx;
    int ty;

    for (offset = DAT_004bff28; offset < &DAT_004bff28[12]; offset++) {
        tx = (*offset)[0] + x;
        ty = (*offset)[1] + y;
        if (tx >= 0 && tx < lpConfig->width && ty >= 0 && ty < lpConfig->height) {
            tile = &GameMap[ty][tx];
        } else {
            tile = NULL;
        }
        if (tile != NULL && (tile->flags & 0x88) != 0) {
            ride = tile->field_0->ride;
            tx = tile->anchor.pos.x;
            ty = tile->anchor.pos.y;
            if (tx >= 0 && tx < lpConfig->width && ty >= 0 && ty < lpConfig->height) {
                target = &GameMap[ty][tx];
            } else {
                target = NULL;
            }
            if (ride->durability != 0 && target->field_11 < ride->durability && (ride->flags & 0x400000) != 0 &&
                lpConfig->field_34 != 0 && (target->flags & 0x4000) == 0) {
                return target;
            }
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0049b430
LEGO_EXPORT int SetMechanicsOrderAtPostion(Bloke *worker, int x, int y) {
    WorkOrder *order;
    MapElement *target;

    order = GetMechanicWorkOrderAt(x, y);
    if (order == NULL) {
        target = FUN_0049b350(worker, x, y);
        if (target != NULL) {
            order = AddRepairOrderForObject(target->field_0->ride, target->anchor.pos.x, target->anchor.pos.y);
            if (order != NULL) {
                target->flags |= 0x4000;
                FUN_00499b60(worker, order);
                return 1;
            }
        } else if (FUN_00499d30(worker) == 0) {
            NewLongTermAction(worker, 0x11);
            return 1;
        }
    } else {
        if (order->assigned != 0) {
            if (order->type == 1 && order->worker->param_action < 0x6b) {
                NewLongTermAction(order->worker, 0x11);
            } else if (order->type == 2 && order->worker->param_action < 0x6b) {
                NewLongTermAction(order->worker, 0x11);
            } else {
                return 0;
            }
        }
        FUN_00499b60(worker, order);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0049b510
LEGO_EXPORT void ClearAMechanicsWorkList(int value) {
    WorkOrder *head = DAT_0079a8c0;
    if (head == 0) {
        return;
    }
    {
        WorkOrder *node = head;
        while (node != 0) {
            if (node->worker == value) {
                node->assigned = 0;
                node->no_bricks = 0;
                node->worker = 0;
                DAT_0079a8c8++;
            }
            node = node->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x0049b550
LEGO_EXPORT void ClearAGardenersWorkList(int param) {
    WorkOrder *current = DAT_0079a8b0;
    if (current == NULL) {
        return;
    }
    while (current != NULL) {
        if (current->worker == param) {
            current->assigned = 0;
            current->worker = 0;
            DAT_0079a8b8++;
        }
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x0049b580
LEGO_EXPORT void RemoveRepairOrderAT(Ride *ride, int x, int y) {
    if ((ride->flags & 0x200000) && (lpConfig->field_38 != 0)) {
        RemoveGardenersWorkOrderAt(x, y);
    } else if ((ride->flags & 0x400000) && (lpConfig->field_34 != 0)) {
        RemoveMechanicsWorkOrderAt(x, y);
    } else {
        RemoveNoneWorkersRepairOrderAT(x, y);
    }
}

// FUNCTION: LEGOLAND 0x0049b5f0
LEGO_EXPORT void RemoveGardenersWorkOrderAt(int x, int y) {
    WorkOrder *esi = DAT_0079a8b0;
    if (esi != 0) {
        while (esi != 0) {
            if (esi->pos.x == x && esi->pos.y == y) {
                if (esi->assigned != 0) {
                    NewLongTermAction(esi->worker, 0x10);
                }
                FUN_00499e30(esi);
                break;
            }
            esi = esi->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x0049b640
LEGO_EXPORT void RemoveMechanicsWorkOrderAt(int x, int y) {
    WorkOrder *current = DAT_0079a8c0;
    while (current) {
        if (current->pos.x == x && current->pos.y == y) {
            if (current->assigned) {
                NewLongTermAction(current->worker, 0x11);
            }
            FUN_00499eb0(current);
            break;
        }
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x0049b690
void FUN_0049b690(Footprint *footprint, int *coords, float rate) {
    RepairOrder *order = malloc(sizeof(RepairOrder));

    order->next = DAT_0079a8d4;
    DAT_0079a8d4 = order;
    order->footprint = *footprint;
    order->pos.x = coords[0];
    order->pos.y = coords[1];
    order->bricks = 1.5f * rate;
    order->bricks_per_step = rate;
}

// FUNCTION: LEGOLAND 0x0049b6e0
void FUN_0049b6e0(RepairOrder *order) {
    RepairOrder *cur;
    RepairOrder *prev;

    if (DAT_0079a8d4 == order) {
        DAT_0079a8d4 = order->next;
    } else {
        cur = DAT_0079a8d4->next;
        prev = DAT_0079a8d4;
        while (cur != order) {
            prev = prev->next;
            if (prev == NULL) {
                break;
            }
            cur = prev->next;
        }
        if (prev != NULL) {
            prev->next = order->next;
        }
    }
    free(order);
}

// FUNCTION: LEGOLAND 0x0049b720
LEGO_EXPORT void RemoveNoneWorkersRepairOrderAT(int x, int y) {
    RepairOrder *current = DAT_0079a8d4;
    while (current != NULL) {
        if (current->pos.x == x && current->pos.y == y) {
            FUN_0049b6e0(current);
            return;
        }
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x0049b750
LEGO_EXPORT void IterateNoneWorkersRepairOrders(void) {
    RepairOrder *order;
    RepairOrder *next;
    MapElement *tile;
    Ride *ride;
    MapRect r;
    int left;
    int right;
    int top;
    int x;
    int y;
    int cost;
    int tx;
    Point pt;
    int bounds[4];

    for (order = DAT_0079a8d4; order != NULL; order = next) {
        next = order->next;
        r.x0 = order->footprint.x0 + order->pos.x;
        r.y0 = order->footprint.y0 + order->pos.y;
        r.x1 = order->footprint.x1 + order->pos.x;
        r.y1 = order->footprint.y1 + order->pos.y;
        pt.x = r.x0;
        pt.y = r.y1;
        GetTileBounds(&pt, bounds);
        left = bounds[0];
        pt.x = r.x1;
        pt.y = r.y0;
        GetTileBounds(&pt, bounds);
        right = bounds[2];
        pt.x = r.x0;
        pt.y = r.y0;
        GetTileBounds(&pt, bounds);
        top = bounds[1];
        pt.x = r.x1;
        pt.y = r.y1;
        GetTileBounds(&pt, bounds);
        x = (right + left) / 2;
        y = (bounds[3] + top) / 2;
        if (order->bricks >= 1.0) {
            cost = order->bricks;
        } else {
            cost = 0;
        }
        if (GetBrickCount() >= cost) {
            UseBricks(cost);
            order->bricks = order->bricks - cost + order->bricks_per_step;
            PrintSprite(DAT_007fe004, x, y, 0, 0);
            tx = order->pos.x;
            if (tx >= 0 && tx < lpConfig->width && order->pos.y >= 0 && order->pos.y < lpConfig->height) {
                tile = &GameMap[order->pos.y][tx];
            } else {
                tile = NULL;
            }
            ride = tile->field_0->ride;
            DAT_00668610 |= 0x200;
            FUN_0049b0d0(tile, ride);
            if (tile->field_11 >= ride->durability) {
                tile->flags &= 0xbfff;
                tile->field_11 = ride->durability;
                FUN_0049b6e0(order);
            }
        } else {
            LLSNextFrame(*DAT_007fdeb0->lls);
            PrintSprite(DAT_007fdeb0, x, y, 0, 0);
        }
    }
}

// FUNCTION: LEGOLAND 0x0049b930
LEGO_EXPORT WorkOrder *AddRepairOrderForObject(Ride *ride, int x, int y) {
    MapElement *cell;
    int level;
    float rate;

    if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
        cell = &GameMap[y][x];
    } else {
        cell = NULL;
    }
    level = cell->field_11;
    rate = GetObjRepairCost(ride, level) / (float)(ride->durability - level);
    if ((ride->flags & 0x200000) != 0 && lpConfig->field_38 != 0) {
        return FUN_00499780(ride->element, &x, 2);
    }
    if ((ride->flags & 0x400000) != 0 && lpConfig->field_34 != 0) {
        return FUN_00499830(ride->element, &x, 2);
    }
    FUN_0049b690(&ride->footprint, &x, rate);
}

// FUNCTION: LEGOLAND 0x0049ba10
LEGO_EXPORT void Garderner_Repair(Bloke *worker) {
    Point dest;
    WorkOrder *order;
    MapElement *tile;
    Ride *ride;
    int cost;
    int x;
    int y;

    switch (worker->param_action) {
    case 0:
        switch (FUN_00482710(&worker->pos.x, &worker->goal.x, &dest.x)) {
        case 0:
            FUN_00499720(worker->order);
            FUN_00499e60(worker->order);
            worker->action = 0x10;
            worker->param_action = 0;
            worker->field_5c = 0x10;
            worker->field_e = 4;
            return;
        case 1:
            worker->dest.x = dest.x;
            worker->dest.y = dest.y;
            worker->field_73 = CalcMoveLine(worker->pos, dest, &worker->nav) + 0x10;
            worker->field_e = 0xc;
            NewDirForAction(worker, (worker->field_73 >> 5) + 3);
            if ((worker->field_64 & 1) != 0) {
                worker->param_action = 6;
            }
            return;
        case 2:
            worker->dest.x = dest.x;
            worker->dest.y = dest.y;
            worker->field_73 = CalcMoveLine(worker->pos, dest, &worker->nav) + 0x10;
            worker->field_e = 0xc;
            NewDirForAction(worker, (worker->field_73 >> 5) + 3);
            worker->param_action = ~worker->field_64 & 1 | 6;
            return;
        }
        return;
    case 6:
        worker->field_5c = 0x70;
        worker->field_e = 4;
        worker->param_action = 0;
        return;
    case 7:
        worker->flags |= 0x108;
        BlokeSetAnim(worker, 1);
        worker->field_72 = worker->order->walk_dir;
        worker->param_action++;
        return;
    case 8:
        if (PlayBlokeAnim(worker) != 0) {
            BlokeWalkAnim(worker);
            worker->flags &= 0xfeff;
            worker->param_action++;
        }
        return;
    case 9:
        order = worker->order;
        worker->flags |= 8;
        worker->field_72 = order->walk_dir;
        x = order->pos.x;
        y = order->pos.y;
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            tile = &GameMap[y][x];
        } else {
            tile = NULL;
        }
        if ((tile->flags & 0x88) == 0) {
            return;
        }
        if (order->bricks >= 1.0) {
            cost = order->bricks;
        } else {
            cost = 0;
        }
        if (GetBrickCount() >= cost) {
            UseBricks(cost);
            order->bricks = order->bricks - cost + order->bricks_per_step;
            if (order->pos.x >= 0 && order->pos.x < lpConfig->width && order->pos.y >= 0 && order->pos.y < lpConfig->height) {
                tile = &GameMap[order->pos.y][order->pos.x];
            } else {
                tile = NULL;
            }
            ride = tile->field_0->ride;
            DAT_00668610 |= 0x200;
            FUN_0049b0d0(tile, ride);
            if (tile->field_11 >= ride->durability) {
                tile->flags &= 0xbfff;
                tile->field_11 = ride->durability;
                worker->param_action++;
            }
            return;
        }
        worker->order->no_bricks = 1;
        return;
    case 10:
        worker->flags &= 0xfff7;
        FUN_00499e30(worker->order);
        if (FUN_00499d00(worker) == 0) {
            NewLongTermAction(worker, 0x10);
        }
        return;
    }
}

// FUNCTION: LEGOLAND 0x0049bd20
LEGO_EXPORT void Mechanics_Repair(Bloke *worker) {
    int dx;
    int dy;
    Point dest;
    WorkOrder *order;
    MapElement *tile;
    Ride *ride;
    int cost;
    int x;
    int y;

    switch (worker->param_action) {
    case 0:
        worker->dest.x = worker->goal.x;
        worker->dest.y = worker->goal.y;
        worker->field_73 = CalcMoveLine(worker->pos, worker->goal, &worker->nav) + 0x10;
        worker->field_e = 0xc;
        NewDirForAction(worker, (worker->field_73 >> 5) + 3);
        worker->param_action = 0xb;
        return;
    case 0xb:
        dx = worker->dest.x - worker->pos.x;
        dy = worker->dest.y - worker->pos.y;
        worker->param_action = dx * dx + dy * dy < 0x10000 ? 107 : 100;
        return;
    case 100:
        switch (FUN_00482710(&worker->pos.x, &worker->goal.x, &dest.x)) {
        case 0:
            FUN_00499720(worker->order);
            FUN_00499f40(worker->order);
            worker->action = 0x11;
            worker->param_action = 0;
            worker->field_5c = 0x70;
            worker->field_e = 4;
            return;
        case 1:
            worker->dest.x = dest.x;
            worker->dest.y = dest.y;
            worker->field_73 = CalcMoveLine(worker->pos, dest, &worker->nav) + 0x10;
            worker->field_e = 0xc;
            NewDirForAction(worker, (worker->field_73 >> 5) + 3);
            if ((worker->field_64 & 1) != 0) {
                worker->param_action = 0x6a;
            }
            return;
        case 2:
            worker->dest.x = dest.x;
            worker->dest.y = dest.y;
            worker->field_73 = CalcMoveLine(worker->pos, dest, &worker->nav) + 0x10;
            worker->field_e = 0xc;
            NewDirForAction(worker, (worker->field_73 >> 5) + 3);
            worker->param_action = ~worker->field_64 & 1 | 0x6a;
            return;
        }
        return;
    case 0x6a:
        worker->field_5c = 0x70;
        worker->field_e = 4;
        worker->param_action = 100;
        return;
    case 0x6b:
        order = worker->order;
        worker->flags |= 8;
        worker->field_72 = order->walk_dir;
        x = order->pos.x;
        y = order->pos.y;
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            tile = &GameMap[y][x];
        } else {
            tile = NULL;
        }
        if ((tile->flags & 0x88) != 0) {
            if (order->bricks >= 1.0) {
                cost = order->bricks;
            } else {
                cost = 0;
            }
            if (GetBrickCount() >= cost) {
                UseBricks(cost);
                order->bricks = order->bricks - cost + order->bricks_per_step;
                if (order->pos.x >= 0 && order->pos.x < lpConfig->width && order->pos.y >= 0 && order->pos.y < lpConfig->height) {
                    tile = &GameMap[order->pos.y][order->pos.x];
                } else {
                    tile = NULL;
                }
                ride = tile->field_0->ride;
                DAT_00668610 |= 0x200;
                FUN_0049b0d0(tile, ride);
                if (tile->field_11 >= ride->durability) {
                    tile->flags &= 0xbfff;
                    tile->field_11 = ride->durability;
                    worker->param_action++;
                }
                return;
            }
            worker->order->no_bricks = 1;
            FUN_004735e0(2);
            return;
        }
        worker->param_action++;
        return;
    case 0x6c:
        worker->flags &= 0xfff7;
        worker->field_46 = 1;
        FUN_00499eb0(worker->order);
        if (FUN_00499d30(worker) == 0) {
            NewLongTermAction(worker, 0x11);
        }
        return;
    }
}

// FUNCTION: LEGOLAND 0x0049c140
void FUN_0049c140(void) {
    Ride *ride = ElemID("POTTING SHED")->ride;
    int count = 0;
    Bloke *worker;
    Bloke *next;
    RideNode *node;
    WorkerSave rec;

    for (worker = GardenerList; worker != NULL; worker = next) {
        next = worker->next;
        if (worker->action == 5) {
            for (node = ride->riders; node != NULL; node = node->next) {
                if (node->rider == worker) {
                    RemoveBlokeFromList(ride, node);
                    break;
                }
            }
            worker->flags &= 0xffd7;
            if (worker->param_action >= 100) {
                RemoveAGardener(worker);
            } else {
                worker->pos = worker->dest;
                NewLongTermAction(worker, 0x10);
            }
        }
    }
    for (worker = GardenerList; worker != NULL; worker = worker->next) {
        count++;
    }
    if (SaveGameWrite(&count, sizeof(count)) == 0) {
        return;
    }
    for (worker = GardenerList; worker != NULL; worker = worker->next) {
        rec.action = worker->action;
        rec.field_e = worker->field_e;
        rec.field_10 = worker->field_10;
        rec.field_1c = worker->field_1c;
        rec.field_20 = worker->field_20;
        rec.dest.x = worker->dest.x;
        rec.dest.y = worker->dest.y;
        rec.goal.x = worker->goal.x;
        rec.goal.y = worker->goal.y;
        memcpy(rec.block_34, &worker->field_34, sizeof(rec.block_34));
        rec.field_5c = worker->field_5c;
        rec.param_action = worker->param_action;
        rec.flags = worker->flags;
        rec.field_64 = worker->field_64;
        rec.field_7f = worker->field_7f;
        rec.field_82 = worker->field_82;
        rec.pos.x = worker->pos.x;
        rec.pos.y = worker->pos.y;
        rec.field_70 = worker->field_70;
        rec.field_72 = worker->field_72;
        rec.field_73 = worker->field_73;
        rec.field_74 = worker->field_74;
        rec.field_75 = worker->field_75;
        rec.nav = worker->nav;
        rec.person_8 = worker->person->field_8;
        rec.scale = worker->person->scale;
        rec.screen = worker->person->screen;
        rec.rotation = worker->person->rotation;
        rec.person_4c = worker->person->field_4c;
        rec.anim = worker->person->field_88;
        rec.sort_id = worker->person->sort_id;
        memcpy(rec.m, worker->person->m, sizeof(rec.m));
        rec.prev_param = worker->prev_param;
        rec.prev_action = worker->prev_action;
        rec.block_34[7] = 0;
        if (SaveGameWrite(&rec, sizeof(rec)) == 0) {
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x0049c3c0
void FUN_0049c3c0(void) {
    int count = 0;
    Bloke *worker = NULL;
    WorkerSave rec;

    if (SaveGameRead(&count, sizeof(count)) == 0) {
        return;
    }
    GardenerList = NULL;
    DAT_0079a8bc = count;
    while (count-- != 0) {
        if (worker != NULL) {
            worker->next = malloc(sizeof(Bloke));
            worker = worker->next;
        } else {
            GardenerList = malloc(sizeof(Bloke));
            worker = GardenerList;
        }
        if (SaveGameRead(&rec, sizeof(rec)) == 0) {
            return;
        }
        worker->action = rec.action;
        worker->field_e = rec.field_e;
        worker->field_10 = rec.field_10;
        worker->field_1c = rec.field_1c;
        worker->field_20 = rec.field_20;
        worker->dest.x = rec.dest.x;
        worker->dest.y = rec.dest.y;
        worker->goal.x = rec.goal.x;
        worker->goal.y = rec.goal.y;
        memcpy(&worker->field_34, rec.block_34, sizeof(rec.block_34));
        worker->field_5c = rec.field_5c;
        worker->param_action = rec.param_action;
        worker->flags = rec.flags;
        worker->field_64 = rec.field_64;
        worker->field_7f = rec.field_7f;
        worker->field_82 = rec.field_82;
        worker->pos.x = rec.pos.x;
        worker->pos.y = rec.pos.y;
        worker->field_70 = rec.field_70;
        worker->field_72 = rec.field_72;
        worker->field_73 = rec.field_73;
        worker->field_74 = rec.field_74;
        worker->field_75 = rec.field_75;
        worker->nav = rec.nav;
        worker->person = malloc(sizeof(Person));
        FUN_0043f810(worker->person);
        worker->person->bloke = worker;
        worker->person->field_8 = rec.person_8;
        worker->person->scale = rec.scale;
        worker->person->screen = rec.screen;
        worker->person->rotation = rec.rotation;
        worker->person->field_4c = rec.person_4c;
        worker->person->field_88 = 0xffffffff;
        worker->person->sort_id = rec.sort_id;
        memcpy(worker->person->m, rec.m, sizeof(rec.m));
        worker->prev_param = rec.prev_param;
        worker->prev_action = rec.prev_action;
        worker->person->field_2c = 0;
        worker->person->field_50 = 0;
        BlokeSetAnim(worker, rec.anim);
    }
    if (worker != NULL) {
        worker->next = NULL;
    }
}

// FUNCTION: LEGOLAND 0x0049c630
void FUN_0049c630(void) {
    Ride *ride = ElemID("MECHANICS HUT")->ride;
    int count = 0;
    Bloke *worker;
    Bloke *next;
    RideNode *node;
    WorkerSave rec;

    for (worker = MechanicList; worker != NULL; worker = next) {
        next = worker->next;
        if (worker->action == 5) {
            for (node = ride->riders; node != NULL; node = node->next) {
                if (node->rider == worker) {
                    RemoveBlokeFromList(ride, node);
                    break;
                }
            }
            worker->flags &= 0xffd7;
            if (worker->param_action >= 100) {
                RemoveAMechanic(worker);
            } else {
                worker->pos = worker->dest;
                NewLongTermAction(worker, 0x11);
            }
        }
    }
    for (worker = MechanicList; worker != NULL; worker = worker->next) {
        count++;
    }
    if (SaveGameWrite(&count, sizeof(count)) == 0) {
        return;
    }
    for (worker = MechanicList; worker != NULL; worker = worker->next) {
        rec.action = worker->action;
        rec.field_e = worker->field_e;
        rec.field_10 = worker->field_10;
        rec.field_1c = worker->field_1c;
        rec.field_20 = worker->field_20;
        rec.dest.x = worker->dest.x;
        rec.dest.y = worker->dest.y;
        rec.goal.x = worker->goal.x;
        rec.goal.y = worker->goal.y;
        memcpy(rec.block_34, &worker->field_34, sizeof(rec.block_34));
        rec.field_5c = worker->field_5c;
        rec.param_action = worker->param_action;
        rec.flags = worker->flags;
        rec.field_64 = worker->field_64;
        rec.field_7f = worker->field_7f;
        rec.field_82 = worker->field_82;
        rec.pos.x = worker->pos.x;
        rec.pos.y = worker->pos.y;
        rec.field_70 = worker->field_70;
        rec.field_72 = worker->field_72;
        rec.field_73 = worker->field_73;
        rec.field_74 = worker->field_74;
        rec.field_75 = worker->field_75;
        rec.nav = worker->nav;
        rec.person_8 = worker->person->field_8;
        rec.scale = worker->person->scale;
        rec.screen = worker->person->screen;
        rec.rotation = worker->person->rotation;
        rec.person_4c = worker->person->field_4c;
        rec.anim = worker->person->field_88;
        rec.sort_id = worker->person->sort_id;
        memcpy(rec.m, worker->person->m, sizeof(rec.m));
        rec.prev_param = worker->prev_param;
        rec.prev_action = worker->prev_action;
        rec.block_34[7] = 0;
        if (SaveGameWrite(&rec, sizeof(rec)) == 0) {
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x0049c8b0
void FUN_0049c8b0(void) {
    int count = 0;
    Bloke *worker = NULL;
    WorkerSave rec;

    if (SaveGameRead(&count, sizeof(count)) == 0) {
        return;
    }
    MechanicList = NULL;
    DAT_0079a8cc = count;
    while (count-- != 0) {
        if (worker != NULL) {
            worker->next = malloc(sizeof(Bloke));
            worker = worker->next;
        } else {
            MechanicList = malloc(sizeof(Bloke));
            worker = MechanicList;
        }
        if (SaveGameRead(&rec, sizeof(rec)) == 0) {
            return;
        }
        worker->action = rec.action;
        worker->field_e = rec.field_e;
        worker->field_10 = rec.field_10;
        worker->field_1c = rec.field_1c;
        worker->field_20 = rec.field_20;
        worker->dest.x = rec.dest.x;
        worker->dest.y = rec.dest.y;
        worker->goal.x = rec.goal.x;
        worker->goal.y = rec.goal.y;
        memcpy(&worker->field_34, rec.block_34, sizeof(rec.block_34));
        worker->field_5c = rec.field_5c;
        worker->param_action = rec.param_action;
        worker->flags = rec.flags;
        worker->field_64 = rec.field_64;
        worker->field_7f = rec.field_7f;
        worker->field_82 = rec.field_82;
        worker->pos.x = rec.pos.x;
        worker->pos.y = rec.pos.y;
        worker->field_70 = rec.field_70;
        worker->field_72 = rec.field_72;
        worker->field_73 = rec.field_73;
        worker->field_74 = rec.field_74;
        worker->field_75 = rec.field_75;
        worker->nav = rec.nav;
        worker->person = malloc(sizeof(Person));
        FUN_0043f810(worker->person);
        worker->person->bloke = worker;
        worker->person->field_8 = rec.person_8;
        worker->person->scale = rec.scale;
        worker->person->screen = rec.screen;
        worker->person->rotation = rec.rotation;
        worker->person->field_4c = rec.person_4c;
        worker->person->field_88 = 0xffffffff;
        worker->person->sort_id = rec.sort_id;
        memcpy(worker->person->m, rec.m, sizeof(rec.m));
        worker->prev_param = rec.prev_param;
        worker->prev_action = rec.prev_action;
        worker->person->field_2c = 0;
        worker->person->field_50 = 0;
        BlokeSetAnim(worker, rec.anim);
    }
    if (worker != NULL) {
        worker->next = NULL;
    }
}

// FUNCTION: LEGOLAND 0x0049cb20
void FUN_0049cb20(void) {
    int count = 0;
    int len;
    int index;
    WorkOrder *order;
    WorkOrder rec;
    Bloke *worker;

    for (order = DAT_0079a8b0; order != NULL; order = order->next) {
        count++;
    }
    SaveGameWrite(&count, sizeof(count));
    for (order = DAT_0079a8b0; order != NULL; order = order->next) {
        rec = *order;
        if (rec.assigned != 0) {
            index = 0;
            for (worker = GardenerList; worker != NULL; worker = worker->next) {
                if (worker == rec.worker) {
                    break;
                }
                index++;
            }
            if (worker == NULL) {
                rec.assigned = 0;
            } else {
                rec.worker_index = index;
            }
        }
        SaveGameWrite(&rec, sizeof(rec));
        len = strlen(rec.element->name);
        SaveGameWrite(&len, sizeof(len));
        SaveGameWrite(rec.element->name, len);
        SaveGameWrite(rec.footprints, rec.count * sizeof(Footprint));
    }
}

// FUNCTION: LEGOLAND 0x0049cc10
void FUN_0049cc10(void) {
    int count;
    int len;
    int i;
    WorkOrder *order = NULL;
    Bloke *worker;
    char name[512];

    DAT_0079a8b0 = NULL;
    SaveGameRead(&count, sizeof(count));
    DAT_0079a8b8 = count;
    while (count-- != 0) {
        if (order != NULL) {
            order->next = malloc(sizeof(WorkOrder));
            order = order->next;
        } else {
            order = malloc(sizeof(WorkOrder));
            DAT_0079a8b0 = order;
        }
        SaveGameRead(order, sizeof(WorkOrder));
        SaveGameRead(&len, sizeof(len));
        SaveGameRead(name, len);
        name[len] = 0;
        order->element = ElemID(name);
        if (order->assigned != 0) {
            worker = GardenerList;
            i = order->worker_index;
            while (i-- != 0) {
                worker = worker->next;
            }
            order->worker = worker;
            worker->order = order;
        }
        order->footprints = malloc(order->count * sizeof(Footprint));
        SaveGameRead(order->footprints, order->count * sizeof(Footprint));
    }
    DAT_0079a8b4 = order;
}

// FUNCTION: LEGOLAND 0x0049cd10
void FUN_0049cd10(void) {
    int count = 0;
    int len;
    int index;
    WorkOrder *order;
    WorkOrder rec;
    Bloke *worker;

    for (order = DAT_0079a8c0; order != NULL; order = order->next) {
        count++;
    }
    SaveGameWrite(&count, sizeof(count));
    for (order = DAT_0079a8c0; order != NULL; order = order->next) {
        rec = *order;
        if (rec.assigned != 0) {
            index = 0;
            for (worker = MechanicList; worker != NULL; worker = worker->next) {
                if (worker == rec.worker) {
                    break;
                }
                index++;
            }
            if (worker == NULL) {
                rec.assigned = 0;
            } else {
                rec.worker_index = index;
            }
        }
        SaveGameWrite(&rec, sizeof(rec));
        len = strlen(rec.element->name);
        SaveGameWrite(&len, sizeof(len));
        SaveGameWrite(rec.element->name, len);
        SaveGameWrite(rec.footprints, rec.count * sizeof(Footprint));
    }
}

// FUNCTION: LEGOLAND 0x0049ce00
void FUN_0049ce00(void) {
    int count;
    int len;
    int i;
    WorkOrder *order = NULL;
    Bloke *worker;
    char name[512];

    DAT_0079a8c0 = NULL;
    SaveGameRead(&count, sizeof(count));
    DAT_0079a8c8 = count;
    while (count-- != 0) {
        if (order != NULL) {
            order->next = malloc(sizeof(WorkOrder));
            order = order->next;
        } else {
            order = malloc(sizeof(WorkOrder));
            DAT_0079a8c0 = order;
        }
        SaveGameRead(order, sizeof(WorkOrder));
        SaveGameRead(&len, sizeof(len));
        SaveGameRead(name, len);
        name[len] = 0;
        order->footprints = malloc(order->count * sizeof(Footprint));
        SaveGameRead(order->footprints, order->count * sizeof(Footprint));
        order->element = ElemID(name);
        if (order->assigned != 0) {
            worker = MechanicList;
            i = order->worker_index;
            while (i-- != 0) {
                worker = worker->next;
            }
            order->worker = worker;
            worker->order = order;
        }
    }
    DAT_0079a8c4 = order;
}

// FUNCTION: LEGOLAND 0x0049cf00
void FUN_0049cf00(MapRect *rect) {
    Bloke *worker;

    for (worker = MechanicList; worker != NULL; worker = worker->next) {
        if (worker->pos.x >= 0 && worker->pos.x >> 8 < lpConfig->width && worker->pos.y >= 0 &&
            worker->pos.y >> 8 < lpConfig->height) {
            GameMap[worker->pos.y >> 8][worker->pos.x >> 8].flags |= 0x1000;
        }
    }
    for (worker = GardenerList; worker != NULL; worker = worker->next) {
        if (worker->pos.x >= 0 && worker->pos.x >> 8 < lpConfig->width && worker->pos.y >= 0 &&
            worker->pos.y >> 8 < lpConfig->height) {
            GameMap[worker->pos.y >> 8][worker->pos.x >> 8].flags |= 0x1000;
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
