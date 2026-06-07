#include "legoland.h"

#include "build.h"
#include "objclass.h"
#include "globals.h"

// FUNCTION: LEGOLAND 0x00450b90
void AddObjectToBuildList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00450c00
void FUN_00450c00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00450c40
int GetBuildTime(struct CostInfo *objClass) {
    int cost = GetObjCost(objClass);
    if (cost < 50) {
        return 50;
    }
    if (cost > 150) {
        return 150;
    }
    return cost;
}

// FUNCTION: LEGOLAND 0x00450c70
unsigned int FUN_00450c70(void) {
    return 0;
}

// FUNCTION: LEGOLAND 0x00450c80
void ProcessBuildingTimes(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00450cf0
void GetBuildAnimFrame(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00450d90
void DoBuildEffects(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00450f10
void ClearBuildObjList(void) {
    int entry = (int)DAT_006664f8;
    int end = (int)&DAT_006670f8;
    while (entry < end) {
        *(unsigned int *)entry = 0;
        entry += 12;
    }
}
