#pragma once

#include "legoland.h"

struct CostInfo;
struct ObjCountWrap;
struct ObjectCount;
struct ObjInstance;
struct EditObject;
struct BestNode;
struct InstancePos;
struct Bloke;
struct ObjClassNames;
struct ObjectInfo;

typedef void (*RideCallback)();
typedef int (*RideIntCallback)();
typedef unsigned int *(*RidePtrCallback)();

struct CallbackTable {
    /* 0x00 */ unsigned char pad_0[0x88];
    /* 0x88 */ struct LibraryNode *library;
    /* 0x8c */ RideCallback cb_8c;
    /* 0x90 */ RideCallback cb_90;
    /* 0x94 */ RideCallback cb_94;
    /* 0x98 */ RideCallback cb_98;
    /* 0x9c */ RideCallback cb_9c;
    /* 0xa0 */ RidePtrCallback cb_a0;
    /* 0xa4 */ RideCallback cb_a4;
    /* 0xa8 */ RideCallback cb_a8;
    /* 0xac */ RideCallback cb_ac;
    /* 0xb0 */ RideCallback cb_b0;
    /* 0xb4 */ RideCallback cb_b4;
    /* 0xb8 */ RideIntCallback cb_b8;
    /* 0xbc */ RideIntCallback cb_bc;
    /* 0xc0 */ RideIntCallback cb_c0;
    /* 0xc4 */ void *context;
};

struct ClassNode {
    /* 0x00 */ char *name;
    /* 0x04 */ unsigned char pad_4[8];
    /* 0x0c */ struct CallbackTable *iface;
};

struct LibraryNode {
    /* 0x00 */ struct LibraryNode *next;
    /* 0x04 */ void *module;
    /* 0x08 */ int refcount;
    /* 0x0c */ void (*init)(void *context, int *out);
};

LEGO_EXPORT void IncrementObjectCount(struct ObjectCount *count);
LEGO_EXPORT void DecrementObjectCount(struct ObjectCount *count);
LEGO_EXPORT unsigned int GetObjSalvageValue(unsigned int param_1, unsigned int param_2);
LEGO_EXPORT struct ObjInstance *CreateObjectInstance(unsigned int param_1, unsigned short *param_2);
LEGO_EXPORT int GetObjCost(struct CostInfo *info);
LEGO_EXPORT unsigned int GetObjRepairCost(unsigned int param_1, unsigned int param_2);
struct Point;
LEGO_EXPORT void BasicObjectDCalcCursor(unsigned int param_1, struct Point *param_2);
LEGO_EXPORT void ClearObjectCounters(void);
LEGO_EXPORT unsigned int ObjCount(struct ObjCountWrap *wrap);
LEGO_EXPORT void FreeBlokeCounters(void);
LEGO_EXPORT void ClearBlokeCounters(unsigned int index);
struct ObjectClass;
LEGO_EXPORT void IncrementBlokeCounter(struct ObjectClass *cls, unsigned int index);
LEGO_EXPORT int GetBlokeCounter(struct ObjectClass *cls, int index);
LEGO_EXPORT void BuildObjInfoList(void);
LEGO_EXPORT void CalculateRideCodes(unsigned int param_1);
LEGO_EXPORT void ResetBestPtr(void);
LEGO_EXPORT int ShuffleObjKeys(int *param_1, void **param_2);
LEGO_EXPORT unsigned int Calc_Item_Attractiveness(unsigned int param_1, unsigned int param_2, unsigned int param_3);
LEGO_EXPORT void UnLoadObjectLibrary(void *library);
LEGO_EXPORT void SetStandardCallbacks(struct CallbackTable *table);
LEGO_EXPORT int LoadObjectLibrary(void *obj, const char *name);
void FUN_00480aa0(struct ObjClassNames *names, struct ObjectInfo *info);
void FUN_00481170(void);
void *FUN_00481720(void);
LEGO_EXPORT void SetEditObject(struct EditObject *obj);
struct BestNode *FUN_00481730(void);
struct BestNode *FUN_00481790(struct InstancePos *pos);
void FUN_00481b10(struct BestNode *node);
