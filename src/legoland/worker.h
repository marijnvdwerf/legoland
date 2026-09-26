#pragma once

#include "bloke.h"
#include "gamemap.h"
#include "legoland.h"
#include "llidb.h"
#include "math.h"
#include "obj_instance.h"

/* Gardeners and mechanics are ordinary Blokes kept on their own lists
   (GardenerList / MechanicList). */

typedef struct WorkOrder WorkOrder;

/* A gardener's or mechanic's job: build (type 1) or repair (type 2) the object of
   class `element` at `pos`. Gardener orders are listed at DAT_0079a8b0..b4, mechanic
   orders at DAT_0079a8c0..c4. */
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
   supply while drawn. List head DAT_0079a8d4. */
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
    /* 0x02 */ unsigned short field_e;
    /* 0x04 */ unsigned short field_10;
    /* 0x06 */ unsigned char pad_6[0x10 - 0x06];
    /* 0x10 */ unsigned int field_1c;
    /* 0x14 */ int field_20;
    /* 0x18 */ Point dest;
    /* 0x20 */ Point goal;
    /* 0x28 */ unsigned int block_34[10]; /* Bloke 0x34..0x5c; [7] is the order */
    /* 0x50 */ int field_5c;
    /* 0x54 */ unsigned char param_action;
    /* 0x55 */ unsigned char pad_55[1];
    /* 0x56 */ unsigned short flags;
    /* 0x58 */ unsigned char field_64;
    /* 0x59 */ unsigned char field_7f;
    /* 0x5a */ unsigned char field_82;
    /* 0x5b */ unsigned char pad_5b[1];
    /* 0x5c */ Point pos;
    /* 0x64 */ unsigned short field_70;
    /* 0x66 */ unsigned char field_72;
    /* 0x67 */ unsigned char field_73;
    /* 0x68 */ unsigned char field_74;
    /* 0x69 */ unsigned char field_75;
    /* 0x6a */ unsigned char pad_6a[2];
    /* 0x6c */ Navigator nav;
    /* 0x80 */ unsigned int person_8;
    /* 0x84 */ unsigned int person_10;
    /* 0x88 */ unsigned int person_14;
    /* 0x8c */ unsigned int person_18;
    /* 0x90 */ unsigned int person_1c;
    /* 0x94 */ unsigned int person_20;
    /* 0x98 */ float person_40;
    /* 0x9c */ float person_44;
    /* 0xa0 */ float person_48;
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
LEGO_EXPORT void RemoveGardenersWorkOrderAt(unsigned int x, unsigned int y);
LEGO_EXPORT void RemoveMechanicsWorkOrderAt(unsigned int x, unsigned int y);
LEGO_EXPORT void RemoveNoneWorkersRepairOrderAT(int x, int y);
LEGO_EXPORT void RemoveRepairOrderAT(Ride *ride, unsigned int x, unsigned int y);
void FUN_0049b270(int param_1, unsigned int param_2);
void FUN_00499eb0(WorkOrder *order);
int FUN_0049a120(void);
int FUN_0049a160(void);
void FUN_0049cfc0(void);

void FUN_0049cf00(struct MapRect *rect);
Bloke *FUN_00499c40(int *coords);
void FUN_00499ac0(Bloke *worker, WorkOrder *order);
WorkOrder *FUN_00499780(Element *element, int *coords, int mode);
LEGO_EXPORT WorkOrder *AddRepairOrderForObject(Ride *ride, int x, int y);
int FUN_00499550(void);
int FUN_00499560(void);
