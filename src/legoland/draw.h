#pragma once

#include "legoland.h"

struct VideoArg;
struct Sprite;

LEGO_EXPORT void InitHostSystemGPU(void);
LEGO_EXPORT void KillHostSystemGPU(void);
LEGO_EXPORT int InitScreen(void);
LEGO_EXPORT unsigned int SetPointer(unsigned int param_1);
LEGO_EXPORT void PushRenderingStatusAndLockVideoSurface(void);
LEGO_EXPORT void PopRenderingStatus(void);
LEGO_EXPORT int GetVideoSurface(struct VideoArg *arg);
LEGO_EXPORT void SetOverridePalette(unsigned int param_1);
LEGO_EXPORT void SetOverrideFrame(unsigned int param_1);
LEGO_EXPORT void ClearOverrideFrame(void);
LEGO_EXPORT void ClearOverridePalette(void);
void FUN_00466360(int a, int b);
void FUN_004663c0(void);
LEGO_EXPORT void RenderingComplete(void);
LEGO_EXPORT int RecreateSprite(struct Sprite *sprite);
void FUN_004687f0(const char *param_1);
LEGO_EXPORT int CheckHostSystemGPU(void);
void FUN_004661d0(const char *msg);
