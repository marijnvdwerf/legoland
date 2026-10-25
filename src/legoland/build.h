#pragma once

#include "legoland.h"

// Per-TU header for build.c — building/construction list helpers.

struct Ride;

LEGO_EXPORT int AddObjectToBuildList(struct Ride *obj, TileId coords);
LEGO_EXPORT void ClearBuildObjList(void);
void RemoveObjectFromBuildList(TileId coords);
LEGO_EXPORT void ProcessBuildingTimes(void);
LEGO_EXPORT int GetBuildTime(struct Ride *objClass);
LEGO_EXPORT void DoBuildEffects(struct Ride *ride, TileId coords);
LEGO_EXPORT int GetBuildAnimFrame(struct Ride *ride, TileId coords);
