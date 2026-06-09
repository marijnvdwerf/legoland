#pragma once

#include "legoland.h"

// Per-TU header for clipping.c — canonical declarations for sprite clipping.

struct ClipQueryResult {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

LEGO_EXPORT void SetClipping(int *rect);
LEGO_EXPORT void StoreClipping(void);
LEGO_EXPORT void RestoreClipping(void);
void FUN_0048a750(void);
void FUN_0048a780(void *arg);
void FUN_0048a800(void);
unsigned int FUN_0048a840(unsigned int arg, struct ClipQueryResult **out);
