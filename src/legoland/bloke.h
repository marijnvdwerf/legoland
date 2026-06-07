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
    unsigned char pad_58[0x60 - 0x58];
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
    unsigned char pad_7a[0x7c - 0x7a];
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

void FUN_00482b10(void);
void FUN_00482d60(unsigned int index, int value);
void FUN_00482d70(void);
void FUN_00483090(void);
LEGO_EXPORT struct Bloke *MakeBloke(int param_1);
struct Worker;
LEGO_EXPORT void DoLowLevelAI(struct Worker *worker);
