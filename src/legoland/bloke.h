#pragma once

#include "legoland.h"
#include "math.h"

/* Canonical "bloke" record (one 0xac allocation).  Shared by the AI code
   (bloke_ai.c) and the visitor allocator (bloke.c), which previously each
   declared their own private view of the same object.  Gardeners and mechanics
   (worker.c) are blokes too.
   man3d.c keeps its own thin render-handle view too (its offset 4 is the owned
   Person*, not this list's prev pointer). */
struct Person;
struct Element;
struct BinVFile;
struct WorkOrder;

/* A bloke's reference into a loaded BNV file: the file pointer is re-resolved
   from the owning ride's table by index after a save game is loaded. */
struct BNVRef {
    struct BinVFile *file;
    int index;
};
typedef struct BNVRef BNVRef;

/* A bloke walking along an animated BNV path (rides with scripted movement). */
struct BNVPath {
    /* 0x00 */ struct BinVFile *file;
    /* 0x04 */ unsigned int field_4;
    /* 0x08 */ char name[0x14];
    /* 0x1c */ float field_1c;
    /* 0x20 */ float field_20;
    /* 0x24 */ float x;
    /* 0x28 */ float y;
    /* 0x2c */ unsigned char pad_2c[0x30 - 0x2c];
    /* 0x30 */ float dx;
    /* 0x34 */ float dy;
    /* 0x38 */ unsigned char pad_38[0x3c - 0x38];
    /* 0x3c */ float field_3c;
    /* 0x40 */ unsigned int frame_index;
    /* 0x44 */ unsigned int field_44;
};
typedef struct BNVPath BNVPath;

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
    struct Element *target; /* class of the ride/shop the bloke is heading for */
    struct Element *last_ride; /* class of the last ride the bloke went on */
    unsigned int field_1c;
    int field_20;
    Point dest;
    Point goal; /* where the bloke is walking to */
    unsigned char field_34;
    unsigned char field_35;
    unsigned char field_36;
    unsigned char field_37;
    short field_38;
    short field_3a;
    short screen_x;
    short screen_y;
    unsigned short field_40;
    unsigned char pad_42[0x44 - 0x42];
    short field_44;
    union {
        short field_46;
        TileId brolly; /* Shark Cafe brolly tile being walked to */
    };
    unsigned char pad_48[0x4a - 0x48];
    short field_4a;
    unsigned short field_4c;
    unsigned char pad_4e[0x50 - 0x4e];
    union {
        int field_50;
        struct WorkOrder *order; /* gardeners and mechanics */
    };
    union {
        unsigned int field_54;
        struct BNVRef *bnv; /* ride code: BNV file slot of the bloke's path */
    };
    int field_58;
    int field_5c;
    unsigned char param_action;
    unsigned char pad_61[0x1];
    unsigned short flags;
    unsigned char field_64;
    unsigned char pad_65[0x68 - 0x65];
    Point pos;
    unsigned short field_70;
    unsigned char field_72;
    unsigned char field_73;
    unsigned char field_74;
    unsigned char field_75;
    unsigned char pad_76[0x78 - 0x76];
    short field_78;
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
    Navigator nav;
};
typedef struct Bloke Bloke;

/* A per-state low-level AI handler (indexed by Bloke.field_e). */
typedef void (*BlokeAction)(Bloke *bloke);

/* Low-level AI handlers (PTR_FUN_004bd34c). */
void FUN_004838a0(Bloke *bloke);
void FUN_004838c0(Bloke *bloke);
void FUN_00483ef0(Bloke *bloke);
void FUN_00484090(Bloke *bloke);
void FUN_00483d10(Bloke *bloke);
void FUN_004838e0(Bloke *bloke);
void FUN_00484220(Bloke *bloke);
void FUN_004845d0(Bloke *bloke);
void FUN_00484630(Bloke *bloke);
void FUN_00484790(Bloke *bloke);
void FUN_00483e20(Bloke *bloke);
void FUN_00484470(Bloke *bloke);
void FUN_00484520(Bloke *bloke);
void FUN_004848e0(Bloke *bloke);
void FUN_00483d90(Bloke *bloke);
void FUN_00484350(Bloke *bloke);

struct OverTile;
struct ActionState;
struct BNVPerson;
LEGO_EXPORT int NewDirForAction(Bloke *bloke, unsigned char dir);
LEGO_EXPORT Bloke *GetBlokePtr(int index);
int CheckForPeople(struct MapRect *rect);
LEGO_EXPORT void SetBlokePositionFromBNV(struct BinVFile *file, Bloke *bloke, char *name, int frame, float near_z, float far_z, float *orient);
LEGO_EXPORT BNVPath *NewBNVPath(struct BinVFile *file, unsigned int param_2, char *name, float param_4, float param_5, int *coords);
LEGO_EXPORT int UpdateBlokeFromBNVPath(Bloke *bloke, BNVPath *path);
Point FUN_004831a0(unsigned char dir, short dist);
LEGO_EXPORT Point GetTileInDir(Point pos, unsigned char dir);
LEGO_EXPORT int OverNewTile(struct OverTile *tile, unsigned int x, unsigned int y);
void FUN_00482b10(void);
void FUN_00482b20(int force);
int FUN_00482b60(Point *pos);
struct Person;
LEGO_EXPORT char *GetVisitorName(Bloke *bloke);
int FUN_00482cb0(Bloke *bloke);
struct BlokeNameView;
void FUN_00482c60(Bloke *bloke);
int FUN_00482df0(Bloke *bloke, int index, int mul);
void FUN_00482d60(unsigned int index, int value);
void FUN_00482d70(void);
void FUN_00483090(void);
LEGO_EXPORT Bloke *MakeBloke(int param_1);
LEGO_EXPORT Bloke *NewBlokeWOList(int type);
LEGO_EXPORT Bloke *NewBloke(void);
LEGO_EXPORT int GetBlokeNum(Bloke *bloke);
LEGO_EXPORT void DestroyBloke(Bloke *bloke);
LEGO_EXPORT void DoLowLevelAI(Bloke *bloke);
struct MapRect;
struct BinVFile;
struct BinVObject;
struct Vertex;
