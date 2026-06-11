#pragma once

#include "legoland.h"

struct ObjClass {
    /* 0x00 */ unsigned char pad_0[0x3c];
    /* 0x3c */ int field_3c;
    /* 0x40 */ int field_40;
    /* 0x44 */ int field_44;
    /* 0x48 */ int field_48;
    /* 0x4c */ unsigned char pad_4c[0x94 - 0x4c];
    /* 0x94 */ void (*method_94)(unsigned int param_1, void *cursor);
    /* 0x98 */ unsigned char pad_98[0xc4 - 0x98];
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
