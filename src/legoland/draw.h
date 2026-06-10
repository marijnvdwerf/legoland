#pragma once

#include "legoland.h"

struct VideoArg {
    /* 0x00 */ int field_0;
    /* 0x04 */ int field_4;
    /* 0x08 */ int field_8;
    /* 0x0c */ void *field_c;
    /* 0x10 */ int field_10;
    /* 0x14 */ int field_14;
};
struct Sprite;

LEGO_EXPORT int InitHostSystemGPU(void);
LEGO_EXPORT void KillHostSystemGPU(void);
LEGO_EXPORT int InitScreen(void);
LEGO_EXPORT unsigned int SetPointer(unsigned int param_1);
LEGO_EXPORT void PushRenderingStatusAndLockVideoSurface(void);
LEGO_EXPORT void PushRenderingStatusAndUnlockVideoSurface(void);
LEGO_EXPORT void PopRenderingStatus(void);
LEGO_EXPORT int GetVideoSurface(struct VideoArg *arg);
LEGO_EXPORT void SetOverridePalette(unsigned int param_1);
LEGO_EXPORT void SetOverrideFrame(unsigned int param_1);
LEGO_EXPORT unsigned int GetOverridePalette(void);
LEGO_EXPORT unsigned int GetOverrideFrame(void);
LEGO_EXPORT void ClearSpriteOverrides(void);
LEGO_EXPORT void ClearOverrideFrame(void);
LEGO_EXPORT void ClearOverridePalette(void);
void FUN_00466360(int a, int b);
void FUN_004663c0(void);
LEGO_EXPORT int RenderingComplete(void);
LEGO_EXPORT void PushSetTarget(struct Sprite *sprite);
LEGO_EXPORT void PopTarget(void);
LEGO_EXPORT int RecreateSprite(struct Sprite *sprite);
void FUN_004687f0(const char *param_1);
LEGO_EXPORT int CheckHostSystemGPU(void);
int FUN_004661d0(void);
