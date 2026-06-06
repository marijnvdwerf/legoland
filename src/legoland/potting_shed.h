#pragma once

// Per-TU header for potting_shed.c — Potting Shed ride interface hooks.

struct PSCarInner {
    unsigned char pad_0[0x10];
    unsigned int field_10;
};

struct PSCarSub {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[0x48];
    struct PSCarInner *field_64;
};

struct PSCarLayer {
    unsigned char pad_0[0xc];
    struct PSCarSub *field_c;
};

int FUN_0043ce60(int a1);
unsigned int FUN_0043ceb0(unsigned int param_1, unsigned int param_2);
void FUN_0043ced0(void);
void FUN_0043cf00(void);
void FUN_0043d0b0(void);
void FUN_0043d1c0(void);
void FUN_0043d1d0(void);
unsigned int *FUN_0043d210(struct PSCarLayer *param1, unsigned short param2);
