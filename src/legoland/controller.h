#pragma once

#include "legoland.h"

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
