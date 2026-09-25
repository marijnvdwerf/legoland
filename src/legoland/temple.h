#pragma once

#include "legoland.h"

struct Bloke;
struct CallbackTable;
struct ClassNode;

struct TempleLayer {
    /* 0x00 */ unsigned char pad_0[0x10];
    /* 0x10 */ unsigned int flags;
};

struct TempleSeat {
    /* 0x00 */ unsigned char pad_0[0x20];
    /* 0x20 */ int depth;
};

/* One bloke on the ride; linked from TempleRide.list. */
struct TempleNode {
    /* 0x00 */ struct TempleNode *next;
    /* 0x04 */ unsigned char pad_4[0x8 - 0x4];
    /* 0x08 */ struct Bloke *bloke;
    /* 0x0c */ union {
        unsigned short id;
        struct {
            unsigned char x;
            unsigned char y;
        } pos;
    } tile;
    /* 0x10 */ struct TempleSeat *seat;
};

struct TempleRide {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ unsigned int x;
    /* 0x10 */ unsigned int y;
    /* 0x14 */ unsigned char pad_14[0x1c - 0x14];
    /* 0x1c */ unsigned int flags;
    /* 0x20 */ unsigned char pad_20[0x64 - 0x20];
    /* 0x64 */ struct TempleLayer *layer;
    /* 0x68 */ unsigned char pad_68[0xcc - 0x68];
    /* 0xcc */ struct TempleNode *list;
};

struct TempleObject {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ struct TempleRide *ride;
};

void FUN_00416e50(struct ClassNode *str, struct CallbackTable *obj);
