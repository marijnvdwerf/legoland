#pragma once

#include "legoland.h"

struct Ride;
struct ObjClassNode;
struct ObjInstance;
struct ObjClassKey;

int FUN_00489f00(const struct ObjClassKey *key);
int FUN_00489f50(const struct ObjClassKey *key);
int FUN_00489f90(const struct ObjClassKey *key);

LEGO_EXPORT struct ObjInstance *GetInstanceOfClass(struct ObjClassNode *cls, const unsigned short *uid);
LEGO_EXPORT int GetObjectUID(int *param_1, int param_2);
void FUN_00489ee0(void);
void FUN_0048a040(void);
LEGO_EXPORT void RemoveBlokeFromRide(void *ride, void *node);
LEGO_EXPORT void RemoveAllBlokesFromRide(unsigned int arg1, void *arg2);
LEGO_EXPORT int GetAllBlokesOffRide(struct Ride *ride, unsigned short uid);
