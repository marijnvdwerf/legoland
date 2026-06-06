#include "legoland.h"

#include "draw.h"

extern unsigned int DDRAWENV[246];
extern unsigned int DAT_0066814c;
extern unsigned int DAT_00668148;
extern unsigned int DAT_007fe9c0[];
extern unsigned int DAT_006681e8;
extern unsigned int DAT_004b9ca8;
extern unsigned int DAT_00668208;
extern unsigned int DAT_00668204;
extern void *renderEngine;
extern int renderEngineTargetIdx;
extern void *renderEngineTargets[1];

extern char DAT_0066861c[128];
extern unsigned char DAT_0066869b;

#include "image_sprite.h"
extern char *_strncpy(char *dest, const char *src, unsigned int count);
extern __declspec(noreturn) void FUN_004a02b8(int status);

// FUNCTION: LEGOLAND 0x004636f0
int InstallDirectDraw(void) { return 0; }

// FUNCTION: LEGOLAND 0x00463700
void InitHostSystemGPU(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004637c0
void CheckHostSystemGPU(void) {
    int i;
    for (i = 0; i < 246; ++i) {
        DDRAWENV[i] = 0;
    }
    InitHostSystemGPU();
}

// FUNCTION: LEGOLAND 0x004637e0
void KillHostSystemGPU(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00463850
unsigned int SetPointer(unsigned int param_1) {
    unsigned int old = DAT_0066814c;
    DAT_0066814c = param_1;
    DAT_00668148 = DAT_007fe9c0[param_1];
    return old;
}

// FUNCTION: LEGOLAND 0x00463870
void InitScreen(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00463ef0
void FUN_00463ef0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00463fc0
void PushRenderingStatusAndLockVideoSurface(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00464080
void PushRenderingStatusAndUnlockVideoSurface(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004640f0
void FUN_004640f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004641f0
void PopRenderingStatus(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00464310
int GetVideoSurface(struct VideoArg *arg) { STUB(); }

// FUNCTION: LEGOLAND 0x00464360
void PrintBackground(void) { return; }

// FUNCTION: LEGOLAND 0x00464370
void CommitCliprectToHardware(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00464400
void SetOverridePalette(unsigned int param_1) { DAT_006681e8 = param_1; }

// FUNCTION: LEGOLAND 0x00464410
unsigned int GetOverridePalette(void) { return DAT_006681e8; }

// FUNCTION: LEGOLAND 0x00464420
void SetOverrideFrame(unsigned int param_1) { DAT_004b9ca8 = param_1; }

// FUNCTION: LEGOLAND 0x00464430
unsigned int GetOverrideFrame(void) { return DAT_004b9ca8; }

// FUNCTION: LEGOLAND 0x00464440
void ClearOverrideFrame(void) { DAT_004b9ca8 = 0xffffffff; }

// FUNCTION: LEGOLAND 0x00464450
void ClearOverridePalette(void) { DAT_006681e8 = 0; }

// FUNCTION: LEGOLAND 0x00464460
void ClearSpriteOverrides(void) {
    DAT_004b9ca8 = 0xffffffff;
    DAT_006681e8 = 0;
}

// FUNCTION: LEGOLAND 0x00464480
void FUN_00464480(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00464a90
void ZBufferHelper(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00464ee0
void FUN_00464ee0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004651d0
void SoftPrint_Clear(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00465240
void FUN_00465240(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00465850
void FUN_00465850(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004659a0
void FUN_004659a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00465a40
void SoftPrint_XBltFast(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00465ee0
void FUN_00465ee0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00466080
void FUN_00466080(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004661d0
void FUN_004661d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00466360
void FUN_00466360(int a, int b) { STUB(); }

// FUNCTION: LEGOLAND 0x004663c0
void FUN_004663c0(void) {
    if (DAT_00668208 != 0) {
        KillSprite(DAT_00668208);
        DAT_00668208 = 0;
    }
    DAT_00668204 = 0;
}

// FUNCTION: LEGOLAND 0x004663f0
void FUN_004663f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00466500
void RenderingComplete(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00466560
void PushSetTarget(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00466600
void PopTarget(void) {
    PopRenderingStatus();
    renderEngineTargetIdx--;
    renderEngine = renderEngineTargets[renderEngineTargetIdx];
    PopRenderingStatus();
    if (renderEngineTargetIdx >= 0) {
        return;
    }
    FUN_004a02b8(2);
}

// FUNCTION: LEGOLAND 0x00466640
int RecreateSprite(struct Sprite *sprite) { STUB(); }

// FUNCTION: LEGOLAND 0x00466770
void FUN_00466770(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00466d80
void FUN_00466d80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00467180
void FUN_00467180(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004673f0
void FUN_004673f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00467640
void FUN_00467640(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004677b0
void FUN_004677b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00467b00
void FUN_00467b00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00467d10
void FUN_00467d10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00467f00
void FUN_00467f00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00468040
void FUN_00468040(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00468410
void FUN_00468410(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004687f0
void FUN_004687f0(const char *param_1) {
    _strncpy(DAT_0066861c, param_1, 0x80);
    DAT_0066869b = 0;
}
