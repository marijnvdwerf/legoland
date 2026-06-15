#pragma once

#include "legoland.h"

/* Canonical "bloke" record (one 0xac allocation).  Shared by the AI code
   (bloke_ai.c) and the visitor allocator (bloke.c), which previously each
   declared their own private view of the same object.  The gardener/mechanic
   worker code (worker.c) views the same record through struct Worker — a worker
   is just a bloke — but keeps a separate view because some worker fields
   (e.g. the 0x34/0x38 float pair) overlap AI pad bytes that a worker uses as a
   byte (progress@0x36); a single non-union struct cannot name both.
   man3d.c keeps its own thin render-handle view too (its offset 4 is the owned
   Person*, not this list's prev pointer). */
struct Bloke {
    struct Bloke *next;
    struct Bloke *prev;
    unsigned char prev_action;
    unsigned char pad_9[0x1];
    unsigned short prev_param;
    unsigned short action;
    unsigned short field_e;
    unsigned short field_10;
    unsigned char pad_12[0x1c - 0x12];
    unsigned int field_1c;
    unsigned char pad_20[0x2e - 0x20];
    short field_2e;
    unsigned char pad_30[0x54 - 0x30];
    unsigned int field_54;
    unsigned int field_58;
    unsigned char pad_5c[0x60 - 0x5c];
    unsigned char param_action;
    unsigned char pad_61[0x1];
    unsigned short flags;
    unsigned char field_64;
    unsigned char pad_65[0x72 - 0x65];
    unsigned char field_72;
    unsigned char pad_73[0x1];
    unsigned char field_74;
    unsigned char field_75;
    unsigned char pad_76[0x78 - 0x76];
    unsigned short field_78;
    short field_7a;
    unsigned short field_7c;
    unsigned char pad_7e[0x80 - 0x7e];
    unsigned char field_80;
    unsigned char pad_81[0x88 - 0x81];
    unsigned int favourite_attraction_0;
    unsigned int favourite_attraction_1;
    unsigned int favourite_attraction_2;
    unsigned int favourite_food;
    unsigned char pad_98[0xac - 0x98];
};

struct BlokeSampleSource {
    /* 0x00 */ unsigned int field_0;
    /* 0x04 */ struct Bloke *field_4;
    /* 0x08 */ unsigned int field_8;
    /* 0x0c */ unsigned int field_c;
};

void KillAllSamplesFromSource(struct BlokeSampleSource *source);

struct InstancePos;
struct Point;
struct OverTile;
struct Point FUN_004831a0(unsigned int dir, short dist);
LEGO_EXPORT struct Point GetTileInDir(int x, int y, unsigned int dir);
LEGO_EXPORT int OverNewTile(struct OverTile *tile, unsigned int x, unsigned int y);
void FUN_00482b10(void);
void FUN_00482b20(int force);
int FUN_00482b60(struct InstancePos *pos);
void FUN_00482d60(unsigned int index, int value);
void FUN_00482d70(void);
void FUN_00483090(void);
LEGO_EXPORT struct Bloke *MakeBloke(void);
LEGO_EXPORT struct Bloke *NewBlokeWOList(void *param_2);
LEGO_EXPORT struct Bloke *NewBloke(void);
LEGO_EXPORT int GetBlokeNum(struct Bloke *bloke);
LEGO_EXPORT void DestroyBloke(struct Bloke *bloke);
struct Worker;
LEGO_EXPORT void DoLowLevelAI(struct Worker *worker);
