#pragma once

// Per-TU header for temple.c — Temple ride interface hooks.

typedef void (*TempleHandler)(void);

struct TempleObj {
    unsigned char pad_0[0x8c];
    TempleHandler field_8c;
    unsigned char pad_90[0x8];
    TempleHandler field_98;
    TempleHandler field_9c;
    unsigned char pad_a0[0x4];
    TempleHandler field_a4;
    TempleHandler field_a8;
    TempleHandler field_ac;
    TempleHandler field_b0;
};

struct TempleObject {
    unsigned char pad_0[12];
    unsigned int field_c;
};

void FUN_004169c0(void);
void FUN_00416a30(void);
void FUN_00416a60(void);
void FUN_00416b50(void);
void FUN_00416dc0(void);
void FUN_00416e00(unsigned int param_1, unsigned int param_2);
void FUN_00416e20(struct TempleObject *a1, void *a2, unsigned int a3);
void FUN_00416e50(const char **str, struct TempleObj *obj);
