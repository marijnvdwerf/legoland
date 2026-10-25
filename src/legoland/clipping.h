#pragma once

#include <windows.h>
#include "legoland.h"

// Per-TU header for clipping.c — canonical declarations for sprite clipping.

struct ClipQueryResult;
extern struct ClipQueryResult FreePlayObjectTable[0x86];
struct ClipQueryResult {
    unsigned char id;
    unsigned char pad_1[3];
    char *name;
    unsigned int field_8;
    unsigned int field_c;
};

struct Element;

void UnlockFreePlayObject(struct Element *object);
LEGO_EXPORT void SetClipping(RECT *rect);
LEGO_EXPORT void GetClipping(RECT *dest);
LEGO_EXPORT void StoreClipping(void);
LEGO_EXPORT void RestoreClipping(void);
void UnlockAllFreePlayObjects(void);
void Empty_0048a780(void *arg);
void RestoreFreePlaySelection(void);
void ClearFreePlaySelection(void);
unsigned int GetFreePlayObjectCost(const char *arg, struct ClipQueryResult **out);
