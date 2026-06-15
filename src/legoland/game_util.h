#pragma once

struct GameObject {
    /* 0x00 */ unsigned char pad_0[8];
    /* 0x08 */ unsigned int flags;
};

struct NerpsArg;

unsigned int FUN_00478be0(unsigned int param_1, unsigned int param_2, unsigned int param_3);

unsigned int LoadObjectClass(struct GameObject *cls);
void FUN_00469ab0(struct GameObject *object);
void FUN_00469900(struct NerpsArg *object, unsigned int a, unsigned int b);
int FUN_0047a480();
void FUN_0046be90(unsigned char param_1, unsigned int param_2);
void FUN_0046c450(unsigned char param_1, unsigned int param_2);
void FUN_0046c4b0(unsigned char param_1, unsigned int param_2);
