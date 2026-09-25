#pragma once

#include "legoland.h"

struct Bloke;
struct Cursor;

/* One bloke inside the hut; linked from HutObject.list. */
struct HutNode {
    /* 0x00 */ struct HutNode *next;
    /* 0x04 */ struct HutNode *prev;
    /* 0x08 */ struct Bloke *bloke;
    /* 0x0c */ unsigned short id;
};

struct HutObject {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ unsigned int x;
    /* 0x10 */ unsigned int y;
    /* 0x14 */ unsigned char pad_14[0x1c - 0x14];
    /* 0x1c */ unsigned int field_1c;
    /* 0x20 */ unsigned char pad_20[0x64 - 0x20];
    /* 0x64 */ unsigned int field_64;
    /* 0x68 */ unsigned char pad_68[0xcc - 0x68];
    /* 0xcc */ struct HutNode *list;
};

struct HutContext {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ struct HutObject *field_c;
};

unsigned int FUN_0043d250(struct HutContext *ctx);
unsigned int FUN_0043d2a0(unsigned int param1, unsigned int param2);
void FUN_0043d2c0(struct HutContext *obj, unsigned int tile, struct Cursor *cursor);
void FUN_0043d2f0(struct HutContext *obj);
void FUN_0043d580(struct HutContext *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile);
void FUN_0043d730();
void FUN_0043d740();
unsigned int *FUN_0043d780(void *ptr, unsigned short arg2);
void FUN_0043d7c0(struct HutObject *hut, unsigned int tile, int flag);
