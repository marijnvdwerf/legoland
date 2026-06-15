#pragma once

#include "legoland.h"

struct Ride;
struct RideNode;
struct ObjClassNode;
struct ObjInstance;
struct ObjClassKey;
struct ResFile;
struct Bloke;

int IsFavouriteFood(struct Bloke *bloke, unsigned int food);

char *FUN_00489e60(struct ResFile *file, char *dest, int maxlen);

int FUN_00489f00(const struct ObjClassKey *key);
int FUN_00489f50(const struct ObjClassKey *key);
int FUN_00489f90(const struct ObjClassKey *key);

LEGO_EXPORT struct ObjInstance *GetInstanceOfClass(struct ObjClassNode *cls, const unsigned short *uid);
struct ClassOffset;
LEGO_EXPORT int GetObjectUID(int *param_1, struct ClassOffset *param_2);
void FUN_00489ee0(void);
void FUN_0048a040(void);
LEGO_EXPORT void RemoveBlokeFromRide(struct Ride *ride, struct RideNode *node);
LEGO_EXPORT void RemoveAllBlokesFromRide(struct Ride *ride, unsigned int param_2);
LEGO_EXPORT int GetAllBlokesOffRide(struct Ride *ride, unsigned short uid);
