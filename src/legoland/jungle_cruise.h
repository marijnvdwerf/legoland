#pragma once

struct JungleRide {
    unsigned char pad_0[8];
    unsigned int field_8;
    unsigned char pad_c[4];
    unsigned int field_10;
    unsigned char pad_14[0x3dc - 0x14];
    unsigned int field_3dc;
    unsigned int field_3e0;
};

void FUN_00433840(struct JungleRide *param_1, unsigned int param_2, unsigned int param_3);
