#pragma once

#include "legoland.h"
#include "math.h"

/* Canonical "bloke" record (one 0xac allocation).  Shared by the AI code
   (bloke_ai.c) and the visitor allocator (bloke.c), which previously each
   declared their own private view of the same object.  The gardener/mechanic
   worker code (worker.c) views the same record through struct Worker — a worker
   is just a bloke — but keeps a separate view because some worker fields
   (e.g. the 0x34/0x38 float pair) overlap AI pad bytes that a worker uses as a
   byte (progress@0x36); a single non-union struct cannot name both.
   man3d.c keeps its own thin render-handle view too (its offset 4 is the owned
   Person*, not this list's prev pointer). */
struct Person;
struct Element;
struct BinVFile;

/* A bloke's reference into a loaded BNV file: the file pointer is re-resolved
   from the owning ride's table by index after a save game is loaded. */
struct BNVRef {
    struct BinVFile *file;
    int index;
};
typedef struct BNVRef BNVRef;

struct Bloke {
    struct Bloke *next;
    union {
        struct Bloke *prev;
        struct Person *person; /* ride code: the bloke's 3D render person */
    };
    unsigned char prev_action;
    unsigned char pad_9[0x1];
    unsigned short prev_param;
    unsigned short action;
    unsigned short field_e;
    unsigned short field_10;
    unsigned char pad_12[0x14 - 0x12];
    unsigned int field_14;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned char pad_20[0x24 - 0x20];
    struct Point dest;
    int field_2c;
    int field_30;
    unsigned char pad_34[0x35 - 0x34];
    unsigned char field_35;
    unsigned char field_36;
    unsigned char field_37;
    short field_38;
    short field_3a;
    short screen_x;
    short screen_y;
    unsigned short field_40;
    unsigned char pad_42[0x46 - 0x42];
    unsigned short field_46;
    unsigned char pad_48[0x4a - 0x48];
    short field_4a;
    unsigned short field_4c;
    unsigned char pad_4e[0x50 - 0x4e];
    int field_50;
    union {
        unsigned int field_54;
        struct BNVRef *bnv; /* ride code: BNV file slot of the bloke's path */
    };
    int field_58;
    unsigned int field_5c;
    unsigned char param_action;
    unsigned char pad_61[0x1];
    unsigned short flags;
    unsigned char field_64;
    unsigned char pad_65[0x68 - 0x65];
    struct Point pos;
    unsigned short field_70;
    unsigned char field_72;
    unsigned char field_73;
    unsigned char field_74;
    unsigned char field_75;
    unsigned char pad_76[0x78 - 0x76];
    unsigned short field_78;
    short field_7a;
    unsigned short field_7c;
    unsigned char field_7e;
    unsigned char field_7f;
    unsigned char field_80;
    unsigned char field_81;
    unsigned char field_82;
    unsigned char field_83;
    unsigned char field_84;
    unsigned char pad_85[0x88 - 0x85];
    struct Element *favourite_attraction_0;
    struct Element *favourite_attraction_1;
    struct Element *favourite_attraction_2;
    struct Element *favourite_food;
    struct Navigator nav;
    unsigned char pad_a4[0xac - 0xa4];
};
typedef struct Bloke Bloke;

struct InstancePos;
struct Point;
struct OverTile;
struct ActionState;
struct BNVPerson;
struct BNVPath;
struct BNVBloke;
LEGO_EXPORT int NewDirForAction(struct Bloke *bloke, unsigned char dir);
LEGO_EXPORT struct Bloke *GetBlokePtr(int index);
int CheckForPeople(struct MapRect *rect);
LEGO_EXPORT void SetBlokePositionFromBNV(struct BinVFile *file, struct Bloke *bloke, char *name, int frame, float near_z, float far_z, float *orient);
LEGO_EXPORT struct BNVPath *NewBNVPath(struct BinVFile *file, unsigned int param_2, char *name, float param_4, float param_5, int *coords);
LEGO_EXPORT int UpdateBlokeFromBNVPath(struct BNVBloke *bloke, struct BNVPath *path);
struct Point FUN_004831a0(unsigned char dir, short dist);
LEGO_EXPORT struct Point GetTileInDir(int x, int y, unsigned int dir);
LEGO_EXPORT int OverNewTile(struct OverTile *tile, unsigned int x, unsigned int y);
void FUN_00482b10(void);
void FUN_00482b20(int force);
int FUN_00482b60(struct InstancePos *pos);
struct Person;
LEGO_EXPORT char *GetVisitorName(struct Bloke *bloke);
int FUN_00482cb0(struct Bloke *bloke);
struct BlokeNameView;
void FUN_00482c60(struct Bloke *bloke);
short FUN_00482df0(struct Bloke *bloke, int index, int mul);
void FUN_00482d60(unsigned int index, int value);
void FUN_00482d70(void);
void FUN_00483090(void);
LEGO_EXPORT struct Bloke *MakeBloke(int param_1);
LEGO_EXPORT struct Bloke *NewBlokeWOList(void *param_2);
LEGO_EXPORT struct Bloke *NewBloke(void);
LEGO_EXPORT int GetBlokeNum(struct Bloke *bloke);
LEGO_EXPORT void DestroyBloke(struct Bloke *bloke);
struct Worker;
LEGO_EXPORT void DoLowLevelAI(struct Worker *worker);
struct MapRect;
struct BinVFile;
struct BinVObject;
struct Vertex;
