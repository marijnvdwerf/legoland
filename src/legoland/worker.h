#pragma once

#include "bloke.h"
#include "gamemap.h"
#include "globals.h"
#include "legoland.h"
#include "llidb.h"
#include "math.h"
#include "obj_instance.h"

/* Gardeners and mechanics are ordinary Blokes kept on their own lists
   (GardenerList / MechanicList). */

typedef struct WorkOrder WorkOrder;

/* A gardener's or mechanic's job: build (type 1) or repair (type 2) the object of
   class `element` at `pos`. Gardener orders are listed at GardenerOrderHead..b4, mechanic
   orders at MechanicOrderHead..c4. */
struct WorkOrder {
    /* 0x00 */ WorkOrder *next;
    /* 0x04 */ Element *element;
    /* 0x08 */ Point pos;
    /* 0x10 */ Footprint *footprints;
    /* 0x14 */ int count; /* footprints */
    /* 0x18 */ int assigned;
    /* 0x1c */ union {
        Bloke *worker;
        int worker_index; /* in save games */
    };
    /* 0x20 */ char type; /* 1 build, 2 repair */
    /* 0x21 */ unsigned char pad_21[3];
    /* 0x24 */ int step_x; /* where on the footprint's rim the worker stands */
    /* 0x28 */ int step_y;
    /* 0x2c */ unsigned char walk_dir; /* 1, 7, 5, 3: which rim edge step_x/y walks */
    /* 0x2d */ unsigned char pad_2d[3];
    /* 0x30 */ int no_bricks; /* repair stalled for lack of bricks */
    /* 0x34 */ float bricks; /* bricks owed, paid a whole brick at a time */
    /* 0x38 */ float bricks_per_step;
};

typedef struct RepairOrder RepairOrder;

/* A repair nobody works on (the class needs no worker): paid from the brick
   supply while drawn. List head RepairOrderList. */
struct RepairOrder {
    /* 0x00 */ RepairOrder *next;
    /* 0x04 */ Footprint footprint;
    /* 0x18 */ Point pos;
    /* 0x20 */ float bricks;
    /* 0x24 */ float bricks_per_step;
};

typedef struct WorkerSave WorkerSave;

/* The 0xdc-byte save-game record of a worker and its Person. */
struct WorkerSave {
    /* 0x00 */ unsigned short action;
    /* 0x02 */ unsigned short low_level_action;
    /* 0x04 */ unsigned short pending_action;
    /* 0x06 */ unsigned char pad_6[0x10 - 0x06];
    /* 0x10 */ unsigned int field_1c;
    /* 0x14 */ int field_20;
    /* 0x18 */ Point dest;
    /* 0x20 */ Point goal;
    /* 0x28 */ struct BlokeBlock34 block_34; /* the order is not saved */
    /* 0x50 */ int field_5c;
    /* 0x54 */ unsigned char param_action;
    /* 0x55 */ unsigned char pad_55[1];
    /* 0x56 */ unsigned short flags;
    /* 0x58 */ unsigned char field_64;
    /* 0x59 */ unsigned char speed;
    /* 0x5a */ unsigned char field_82;
    /* 0x5b */ unsigned char pad_5b[1];
    /* 0x5c */ Point pos;
    /* 0x64 */ unsigned short height;
    /* 0x66 */ unsigned char dir;
    /* 0x67 */ unsigned char target_dir;
    /* 0x68 */ unsigned char frame;
    /* 0x69 */ unsigned char field_75;
    /* 0x6a */ unsigned char pad_6a[2];
    /* 0x6c */ Navigator nav;
    /* 0x80 */ unsigned int person_8;
    /* 0x84 */ Vector3 scale;
    /* 0x90 */ Point screen;
    /* 0x98 */ Vector3 rotation;
    /* 0xa4 */ int person_4c;
    /* 0xa8 */ unsigned int anim; /* Person.field_88 */
    /* 0xac */ unsigned int sort_id;
    /* 0xb0 */ int m[9];
    /* 0xd4 */ unsigned int prev_param;
    /* 0xd8 */ unsigned int prev_action;
};

LEGO_EXPORT Bloke *GenerateGardener(int *coords, int param_2);
LEGO_EXPORT Bloke *GenerateMechanic(int *coords, int param_2);
LEGO_EXPORT void RemoveAGardener(Bloke *worker);
LEGO_EXPORT void RefundGardener(void);
LEGO_EXPORT void RefundMechanic(void);
LEGO_EXPORT void RemoveAMechanic(Bloke *worker);
LEGO_EXPORT void IterateNoneWorkersRepairOrders(void);
LEGO_EXPORT void PutWorkerOnRide(Bloke *worker, MapElement *tile);
LEGO_EXPORT WorkOrder *GetGardenerWorkOrderAt(int x, int y);
LEGO_EXPORT WorkOrder *GetMechanicWorkOrderAt(int x, int y);
LEGO_EXPORT void RemoveGardenersWorkOrderAt(int x, int y);
LEGO_EXPORT void RemoveMechanicsWorkOrderAt(int x, int y);
LEGO_EXPORT void RemoveNoneWorkersRepairOrderAT(int x, int y);
LEGO_EXPORT void RemoveRepairOrderAT(Ride *ride, int x, int y);
void EraseWorkOrdersAtTile(Ride *ride, TileId tile);
LEGO_EXPORT void EraseMechanicOrder(WorkOrder *order);
LEGO_EXPORT void EraseGardenerOrder(WorkOrder *order);
void FreeMechanicWorkOrder(WorkOrder *order);
int BuyGardener(void);
int BuyMechanic(void);
void FreeAllWorkOrdersAndWorkers(void);

struct MapRect;
void MarkTilesUnderWorkers(struct MapRect *rect);
Bloke *FindNearestIdleGardener(int *coords);
void AssignGardenerToWorkOrder(Bloke *worker, WorkOrder *order);
WorkOrder *CreateGardenerWorkOrder(Element *element, int *coords, int mode);
struct Point;
LEGO_EXPORT WorkOrder *AddRepairOrderForObject(Ride *ride, struct Point pos);
int GetGardenerCount(void);
int GetMechanicCount(void);
LEGO_EXPORT void ClearAMechanicsWorkList(Bloke *value);
LEGO_EXPORT void ClearAGardenersWorkList(Bloke *param);
LEGO_EXPORT int SetGardenerWorkOrderAtPostion(Bloke *worker, int x, int y);
LEGO_EXPORT int SetMechanicsOrderAtPostion(Bloke *worker, int x, int y);
LEGO_EXPORT void ControlWorkers(void);
LEGO_EXPORT void LoadWorkerInterfaceGFX(void);
void SaveGardeners(void);
void SaveMechanics(void);
void SaveGardenerOrders(void);
void SaveMechanicOrders(void);
void LoadGardeners(void);
void LoadMechanics(void);
void LoadGardenerOrders(void);
void LoadMechanicWorkOrders(void);
LEGO_EXPORT void RenderWorkers(void);
LEGO_EXPORT void RenderWorkerInterfaceGFX(void);
LEGO_EXPORT int WorkOrderBuildObject(Element *element, Point *pos);

/* The map tile at `pos`, or NULL off the map. Inlined by the original: Garderner_Repair
   and Mechanics_Repair only schedule their loads like the binary when the lookup goes
   through this helper. */
static __inline MapElement *GetTileAtPoint(Point *pos) {
    if (pos->x >= 0 && pos->x < lpConfig->width && pos->y >= 0 && pos->y < lpConfig->height) {
        return &GameMap[pos->y][pos->x];
    }
    return NULL;
}
