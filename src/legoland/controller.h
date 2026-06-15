#pragma once

#include "legoland.h"

struct Point;

struct ObjClass {
    /* 0x00 */ unsigned char pad_0[0x1c];
    /* 0x1c */ unsigned int field_1c;
    /* 0x20 */ short type;
    /* 0x22 */ unsigned char pad_22[0x3c - 0x22];
    /* 0x3c */ int field_3c;
    /* 0x40 */ int field_40;
    /* 0x44 */ int field_44;
    /* 0x48 */ int field_48;
    /* 0x4c */ unsigned char pad_4c[0x58 - 0x4c];
    /* 0x58 */ unsigned char *field_58;
    /* 0x5c */ unsigned char pad_5c[0x78 - 0x5c];
    /* 0x78 */ char *name;
    /* 0x7c */ unsigned char pad_7c[0x90 - 0x7c];
    /* 0x90 */ void (*method_90)(unsigned int object, struct Point *pos, int param_3);
    /* 0x94 */ void (*method_94)(unsigned int param_1, void *cursor);
    /* 0x98 */ void (*method_98)(unsigned int classid, struct Point *pos);
    /* 0x9c */ void (*method_9c)(unsigned int classid, unsigned int coords, void *cursor);
    /* 0xa0 */ unsigned char pad_a0[0xc4 - 0xa0];
    /* 0xc4 */ unsigned int field_c4;
};

struct CtrlBuffer {
    /* 0x00 */ int field_0;
    /* 0x04 */ int field_4;
    /* 0x08 */ int field_8;
    /* 0x0c */ int field_c;
    /* 0x10 */ int field_10;
    /* 0x14 */ int field_14;
    /* 0x18 */ unsigned int field_18;
    /* 0x1c */ int field_1c;
    /* 0x20 */ int field_20;
    /* 0x24 */ int field_24;
};

LEGO_EXPORT int SetupControllers(void);
LEGO_EXPORT void ReadGameButtons(void);
