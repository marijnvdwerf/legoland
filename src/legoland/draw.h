#pragma once

struct VideoArg;
struct Sprite;

unsigned int SetPointer(unsigned int param_1);
void PushRenderingStatusAndLockVideoSurface(void);
void PopRenderingStatus(void);
int GetVideoSurface(struct VideoArg *arg);
void SetOverridePalette(unsigned int param_1);
void SetOverrideFrame(unsigned int param_1);
void ClearOverrideFrame(void);
void ClearOverridePalette(void);
void FUN_00466360(int a, int b);
void FUN_004663c0(void);
void RenderingComplete(void);
int RecreateSprite(struct Sprite *sprite);
void FUN_004687f0(const char *param_1);
int CheckHostSystemGPU(void);
void FUN_004661d0(const char *msg);
