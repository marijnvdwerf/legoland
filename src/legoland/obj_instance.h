#pragma once

#include "gamemap.h"
#include "legoland.h"
#include "llidb.h"

struct Bloke;
struct Element;
struct FXSpriteList;
struct Sprite;
struct Person;

/* Map tile id as stored per bloke on a ride: x/y bytes, compared as one 16-bit value. */

/* One placed instance of an object class; linked from Ride.instances. */
struct ObjInstance {
    /* 0x00 */ struct ObjInstance *next;
    /* 0x04 */ struct ObjInstance *prev;
    /* 0x08 */ struct Ride *ride; /* the instance's class */
    /* 0x0c */ unsigned short flags;
    /* 0x0e */ union {
        unsigned short uid; /* anchor tile id */
        TileId anchor;
    };
    /* 0x10 */ unsigned int field_10;
};
typedef struct ObjInstance ObjInstance;

/* One bloke on a ride; linked from Ride.riders. */
struct RideNode {
    /* 0x00 */ struct RideNode *next;
    /* 0x04 */ struct RideNode *prev;
    /* 0x08 */ struct Bloke *rider;
    /* 0x0c */ TileId tile;
    /* 0x10 */ struct Person *person;
};
typedef struct RideNode RideNode;

/* A placed ride/attraction (the object behind Element.ride). */
struct LibraryNode;
struct RideSpriteInfo;
struct Ride {
    /* 0x00 */ struct Ride *next; /* ObjectClassList */
    /* 0x04 */ struct ObjInstance *instances;
    /* 0x08 */ unsigned int field_8;
    /* 0x0c */ union {
        struct {
            unsigned int x;
            unsigned int y;
        };
        struct Point pos;
    };
    /* 0x14 */ int field_14;
    /* 0x18 */ int field_18;
    /* 0x1c */ unsigned int flags;
    /* 0x20 */ short type;
    /* 0x22 */ unsigned char pad_22[0x24 - 0x22];
    /* 0x24 */ char field_24;
    /* 0x25 */ char field_25;
    /* 0x26 */ short cost; /* bricks to build */
    /* 0x28 */ short field_28; /* shops: the price of an item */
    /* 0x2a */ short range;
    /* 0x2c */ unsigned char durability; /* repair levels (MapElement.field_11 when new) */
    /* 0x2d */ unsigned char pad_2d;
    /* 0x2e */ short seats; /* riders allowed per tile */
    /* 0x30 */ unsigned char pad_30[0x32 - 0x30];
    /* 0x32 */ signed char anim_dx; /* build-animation draw offset */
    /* 0x33 */ signed char anim_dy;
    /* 0x34 */ unsigned char pad_34[0x36 - 0x34];
    /* 0x36 */ short value;
    /* 0x38 */ short intensity; /* compared with a bloke's thrill preference */
    /* 0x3a */ short field_3a;
    /* 0x3c */ struct Footprint footprint;
    /* 0x50 */ void *desc; /* the ODF's description block */
    /* 0x54 */ void *script; /* the ODF's script block */
    /* 0x58 */ struct Element *parent_element;
    /* 0x5c */ struct Element *theme_element;
    /* 0x60 */ struct Element *menu_element;
    /* 0x64 */ struct Sprite *layer;
    /* 0x68 */ struct Sprite *icon; /* drawn over pending work orders */
    /* 0x6c */ struct Sprite *anim; /* build-animation sprite */
    /* 0x70 */ struct Element *handle; /* the element of its TSF sprites */
    /* 0x74 */ union {
        struct FXSpriteList *sprites; /* ODF objects: the TSF's sprites */
        unsigned short *tiles; /* basic objects: their tile ids */
    };
    /* 0x78 */ char *name;
    /* 0x7c */ char *field_7c;
    /* 0x80 */ void *data_80;
    /* 0x84 */ unsigned char pad_84[0x88 - 0x84];
    /* 0x88 */ struct LibraryNode *library;
    /* 0x8c */ void (*cb_8c)();
    /* 0x90 */ void (*cb_90)();
    /* 0x94 */ void (*cb_94)();
    /* 0x98 */ void (*cb_98)();
    /* 0x9c */ void (*cb_9c)();
    /* 0xa0 */ struct RideSpriteInfo *(*cb_sprite)(struct Element *element, unsigned short id); /* sprite to draw for a placed object */
    /* 0xa4 */ void (*cb_a4)();
    /* 0xa8 */ void (*cb_pre_render)();
    /* 0xac */ void (*cb_ac)();
    /* 0xb0 */ void (*cb_b0)();
    /* 0xb4 */ void (*cb_b4)();
    /* 0xb8 */ int (*load_hook)(struct Element *element); /* run after a save game restores this ride */
    /* 0xbc */ int (*save_hook)(struct Element *element); /* run when this ride is saved */
    /* 0xc0 */ int (*cb_c0)();
    /* 0xc4 */ struct Element *element; /* this ride's LLIDB element */
    /* 0xc8 */ unsigned char *counters; /* per-bloke visit counts */
    /* 0xcc */ struct RideNode *riders;
};
typedef struct Ride Ride;

/* Map object of a placed ride; the parameter of the per-class ride callbacks. */
struct ObjInstance;
struct Point;
struct ResFile;
struct InstanceNode;

LEGO_EXPORT void AddInstanceToList(struct InstanceNode *node);
LEGO_EXPORT void RemoveInstanceFromList(struct InstanceNode *node);

char *ReadResFileLine(struct ResFile *file, char *dest, int maxlen);

int AddObjInstance(const struct Point *pos);
int RemoveObjInstance(const struct Point *pos);
int IncObjInstanceCount(const struct Point *pos);
unsigned short GetObjInstanceCount(const struct Point *pos);

LEGO_EXPORT struct ObjInstance *GetInstanceOfClass(struct Ride *ride, const TileId *tile);
LEGO_EXPORT TileId GetObjectUID(struct Point *pos, struct Ride *ride);
void ClearObjInstanceTable(void);
void FreeAllObjectClassInstances(void);
LEGO_EXPORT void RemoveBlokeFromRide(struct Ride *ride, struct RideNode *node);
LEGO_EXPORT void RemoveAllBlokesFromRide(struct Ride *ride, TileId tile);
LEGO_EXPORT int GetAllBlokesOffRide(struct Ride *ride, unsigned short uid);

LEGO_EXPORT void HandleRideAI(void);
