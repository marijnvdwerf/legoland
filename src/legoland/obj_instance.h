#pragma once

#include "gamemap.h"
#include "legoland.h"

struct Bloke;
struct Sprite;
struct Person;

/* Map tile id as stored per bloke on a ride: x/y bytes, compared as one 16-bit value. */

/* One bloke on a ride; linked from Ride.riders. */
struct RideNode {
    /* 0x00 */ struct RideNode *next;
    /* 0x04 */ struct RideNode *prev;
    /* 0x08 */ struct Bloke *rider;
    /* 0x0c */ TileId tile;
    /* 0x10 */ struct Person *person;
};
typedef struct RideNode RideNode;

/* A placed ride/attraction (the object behind RideObject.ride). */
struct Ride {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ unsigned int x;
    /* 0x10 */ unsigned int y;
    /* 0x14 */ int field_14;
    /* 0x18 */ int field_18;
    /* 0x1c */ unsigned int flags;
    /* 0x20 */ short type;
    /* 0x22 */ unsigned char pad_22[0x24 - 0x22];
    /* 0x24 */ char field_24;
    /* 0x25 */ char field_25;
    /* 0x26 */ unsigned char pad_26[0x2a - 0x26];
    /* 0x2a */ short range;
    /* 0x2c */ unsigned char pad_2c[0x36 - 0x2c];
    /* 0x36 */ short value;
    /* 0x38 */ unsigned char pad_38[0x3c - 0x38];
    /* 0x3c */ struct Footprint footprint;
    /* 0x50 */ unsigned char pad_50[0x64 - 0x50];
    /* 0x64 */ struct Sprite *layer;
    /* 0x68 */ unsigned char pad_68[0xc4 - 0x68];
    /* 0xc4 */ unsigned int field_c4;
    /* 0xc8 */ unsigned char pad_c8[0xcc - 0xc8];
    /* 0xcc */ struct RideNode *riders;
};
typedef struct Ride Ride;

/* Map object of a placed ride; the parameter of the per-class ride callbacks. */
struct MapObject;
struct RideObject {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ union {
        struct Ride *ride;
        struct MapObject *obj; /* map_object.c's view of the same instance */
    };
    /* 0x10 */ unsigned int field_10;
};
typedef struct RideObject RideObject;
struct ObjClassNode;
struct ObjInstance;
struct ObjClassKey;
struct ResFile;
struct InstanceNode;

LEGO_EXPORT void AddInstanceToList(struct InstanceNode *node);
LEGO_EXPORT void RemoveInstanceFromList(struct InstanceNode *node);

char *FUN_00489e60(struct ResFile *file, char *dest, int maxlen);

int FUN_00489f00(const struct ObjClassKey *key);
int FUN_00489f50(const struct ObjClassKey *key);
int FUN_00489f90(const struct ObjClassKey *key);
unsigned short FUN_00489fd0(const struct ObjClassKey *key);

LEGO_EXPORT struct ObjInstance *GetInstanceOfClass(struct ObjClassNode *cls, const unsigned short *uid);
struct ClassOffset;
LEGO_EXPORT int GetObjectUID(int *param_1, struct ClassOffset *param_2);
void FUN_00489ee0(void);
void FUN_0048a040(void);
LEGO_EXPORT void RemoveBlokeFromRide(struct Ride *ride, struct RideNode *node);
LEGO_EXPORT void RemoveAllBlokesFromRide(struct Ride *ride, TileId tile);
LEGO_EXPORT int GetAllBlokesOffRide(struct Ride *ride, unsigned short uid);

LEGO_EXPORT void HandleRideAI(int arg_1, int arg_2, int arg_3);
