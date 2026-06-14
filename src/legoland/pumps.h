#pragma once

struct PumpNode {
    unsigned char pad_0[2];
    unsigned short var_2;
    unsigned int var_4;
    unsigned int var_8;
    struct PumpNode *next;
};

struct PumpNode *FUN_00411aa0(unsigned int arg1, unsigned int arg2);
void FUN_00411ba0(unsigned short param_1);
void FUN_00411bd0(void);

void FUN_00411a10();
void FUN_00411a20();
void FUN_00411bf0();
void FUN_00411c70();
void FUN_00411cd0();
