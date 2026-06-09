#pragma once

struct KLIBAUDIO_Vtbl {
    unsigned char pad_0[0x30];
    void(__stdcall *func_30)(void *self, int a1, int a2, int a3);
    unsigned char pad_34[0x3c - 0x34];
    void(__stdcall *func_3c)(void *self, unsigned int a1);
};

struct KLIBAUDIO_Stop {
    unsigned char pad_0[0x48];
    void(__stdcall *func_48)(struct KLIBAUDIO_Object *self);
};

struct KLIBAUDIO_Object {
    void *vtable;
};

void FUN_00498120(void);
void FUN_00498630(const char *param_1);
int FUN_004988c0(void);
void FUN_00498920(void);
int FUN_00498b00(void);
void FUN_00498b40(void);
int FUN_00498cf0(void);
