#pragma once

#include "legoland.h"

struct CostInfo;
struct ObjCountWrap;
struct EditObject;
struct BestNode;
struct InstancePos;
struct Bloke;
struct CallbackTable;
struct ObjClassNames;
struct ObjectInfo;

LEGO_EXPORT int GetObjCost(struct CostInfo *info);
LEGO_EXPORT unsigned int BasicObjectDCalcCursor(unsigned int param_1, unsigned int param_2);
LEGO_EXPORT void ClearObjectCounters(void);
LEGO_EXPORT unsigned int ObjCount(struct ObjCountWrap *wrap);
LEGO_EXPORT void FreeBlokeCounters(void);
LEGO_EXPORT void ClearBlokeCounters(unsigned int index, struct Bloke *bloke);
LEGO_EXPORT void UnLoadObjectLibrary(void *library);
LEGO_EXPORT void SetStandardCallbacks(struct CallbackTable *table);
LEGO_EXPORT int LoadObjectLibrary(void *obj, const char *name);
void FUN_00480aa0(struct ObjClassNames *names, struct ObjectInfo *info);
void FUN_00481170(void);
LEGO_EXPORT void SetEditObject(struct EditObject *obj);
struct BestNode *FUN_00481730(void);
struct BestNode *FUN_00481790(struct InstancePos *pos);
void FUN_00481b10(struct BestNode *node);
