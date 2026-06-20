#pragma once

struct CommandArgs;

struct GameObject {
    /* 0x00 */ unsigned char pad_0[8];
    /* 0x08 */ unsigned int flags;
};

int FUN_00478b20(unsigned int arg);
unsigned int FUN_00478be0(unsigned int param_1, unsigned int param_2, unsigned int param_3);
int FUN_0047a480(struct CommandArgs *arg, int argc);
int FUN_0047afb0(int param_1);
