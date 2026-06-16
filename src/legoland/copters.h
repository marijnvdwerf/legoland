#pragma once

#include "legoland.h"
#include "objclass.h"

struct Sprite;

struct CopterLayer {
    /* 0x00 */ unsigned int flags;
    /* 0x04 */ char field_4;
    /* 0x05 */ unsigned char pad_5[3];
    /* 0x08 */ int field_8;
    /* 0x0c */ int field_c;
    /* 0x10 */ int field_10;
    /* 0x14 */ int field_14;
    /* 0x18 */ struct Sprite *field_18;
    /* 0x1c */ char field_1c;
    /* 0x1d */ char field_1d;
    /* 0x1e */ unsigned char pad_1e[2];
};

struct CopterNode {
    /* 0x00 */ unsigned short field_0;
    /* 0x02 */ unsigned char field_2;
    /* 0x03 */ unsigned char field_3;
    /* 0x04 */ struct CopterNode *next;
    /* 0x08 */ unsigned int field_8;
    /* 0x0c */ int field_c;
    /* 0x10 */ unsigned char field_10;
    /* 0x11 */ unsigned char pad_11[3];
    /* 0x14 */ int field_14;
    /* 0x18 */ struct CopterLayer layer[6];
};

void FUN_00403e90(struct CopterNode *node);
void FUN_004049a0(struct CopterNode *node, int param);

void FUN_00405110(struct ClassNode *name, struct CallbackTable *interfaces);
