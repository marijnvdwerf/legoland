#pragma once

#include "legoland.h"

// Per-TU header for build.c — building/construction list helpers.

struct ObjClass;

LEGO_EXPORT int AddObjectToBuildList(struct ObjClass *obj, short param_2);
LEGO_EXPORT void ClearBuildObjList(void);
