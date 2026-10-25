#pragma once

#include "legoland.h"

struct Element;
struct Ride;
struct ObjInstance;
struct BestNode;
struct Point;
struct Bloke;

LEGO_EXPORT void *LoadObjectClass(struct Element *cls);

typedef void (*RideCallback)();
typedef int (*RideIntCallback)();
typedef struct RideSpriteInfo *(*RidePtrCallback)(struct Element *element, unsigned short id);

struct LibraryNode {
    /* 0x00 */ struct LibraryNode *next;
    /* 0x04 */ void *module;
    /* 0x08 */ int refcount;
    /* 0x0c */ void (*init)(void *context, void **out);
};

LEGO_EXPORT void IncrementObjectCount(struct Ride *count);
LEGO_EXPORT void DecrementObjectCount(struct Ride *count);
LEGO_EXPORT unsigned int GetObjSalvageValue(struct Ride *ride, unsigned int level);
LEGO_EXPORT struct ObjInstance *CreateObjectInstance(struct Ride *param_1, unsigned short *param_2);
LEGO_EXPORT int GetObjCost(struct Ride *ride);
LEGO_EXPORT int GetObjRepairCost(struct Ride *ride, unsigned int level);
LEGO_EXPORT void BasicObjectDCalcCursor(struct Element *param_1, struct Point *param_2);
LEGO_EXPORT void ClearObjectCounters(void);
LEGO_EXPORT unsigned int ObjCount(struct Element *wrap);
LEGO_EXPORT void FreeBlokeCounters(void);
LEGO_EXPORT void ClearBlokeCounters(unsigned int index);
LEGO_EXPORT void IncrementBlokeCounter(struct Ride *cls, unsigned int index);
LEGO_EXPORT int GetBlokeCounter(struct Ride *cls, int index);
LEGO_EXPORT void BuildObjInfoList(void);
LEGO_EXPORT void CalculateRideCodes(struct Bloke *bloke);
struct RideStats;
LEGO_EXPORT unsigned int CalculateRideCode(unsigned int param_1, struct RideStats *param_2, unsigned int param_3);
LEGO_EXPORT void ResetBestPtr(void);
LEGO_EXPORT int ShuffleObjKeys(struct Point *goal, struct Ride **ride);
LEGO_EXPORT int Calc_Item_Attractiveness(struct Ride *item, struct Bloke *bloke, int param_3);
LEGO_EXPORT void UnLoadObjectLibrary(void *library);
LEGO_EXPORT void SetStandardCallbacks(struct Ride *table);
LEGO_EXPORT int LoadObjectLibrary(void *obj, const char *name);
void ApplyWaterWorksClassOverrides(struct Element *names, struct Ride *info);
void ResetClassRideList(void);
void *GetBestNodeList(void);
LEGO_EXPORT void SetEditObject(struct Ride *obj);
struct BestNode *AddBestNode(void);
struct BestBox {
    int x_min;
    int y_min;
    int x_max;
    int y_max;
    int field_18;
};
struct BestNode {
    struct BestNode *next;
    unsigned int pad_4;
    union {
        struct BestBox box;
        struct {
            unsigned int x_min;
            unsigned int y_min;
            unsigned int x_max;
            unsigned int y_max;
            unsigned int field_18;
        };
    };
    unsigned int field_1c;
    unsigned int field_20;
};
void RemoveBestNode(struct BestNode *node);
typedef struct BestNode BestNode;
struct BestNode *FindBestNodeAtPoint(struct Point *pos);
void CollectAdjacentNodes(int *param_1);
struct BestNode *FindBestNodeAtPixel(int *param_1);
void MergeBestNode(struct BestNode *node);
LEGO_EXPORT void AllocBlokeCounters(unsigned int size);
LEGO_EXPORT void CreateObjectClasses(void);
