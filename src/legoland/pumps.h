#pragma once

#include "legoland.h"

struct Cursor;
#include "llidb.h"

struct PumpNode {
    unsigned short id;
    unsigned short var_2;
    unsigned int var_4;
    unsigned int var_8;
    struct PumpNode *next;
};

struct PumpNode *FindPumpAt(unsigned int arg1, unsigned int arg2);
void RemovePump(struct PumpNode *node);
void RemovePumpsWithId(unsigned short param_1);
void RemoveAllPumps(void);

void InitDrivingSchoolPumps(struct Element *param_1);
void DrivingSchoolPumpsSetEditMode();
struct RideQueueEntry *SnapCursorToRoadForPump(struct Cursor *cursor);
void DrivingSchoolPumpsAddObject(Element *obj, int *coords);
void DrivingSchoolPumpsRemoveObject(void *param_1, TileId tile, struct Cursor *cursor);
void DrivingSchoolPumpsCalcCursor(Element *obj, int *screen, unsigned int param_3);
