#include "legoland.h"
#include "crt.h"
#include "globals.h"

#include "draw.h"
#include "text.h"
#include "wndenv.h"

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x004636f0
LEGO_EXPORT int InstallDirectDraw(void) { return 0; }

// FUNCTION: LEGOLAND 0x00463700
LEGO_EXPORT void InitHostSystemGPU(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004637c0
LEGO_EXPORT int CheckHostSystemGPU(void) {
    int i;
    for (i = 0; i < 246; ++i) {
        DDRAWENV[i] = 0;
    }
    InitHostSystemGPU();
}

// FUNCTION: LEGOLAND 0x004637e0
LEGO_EXPORT void KillHostSystemGPU(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00463850
LEGO_EXPORT unsigned int SetPointer(unsigned int param_1) {
    unsigned int old = DAT_0066814c;
    DAT_0066814c = param_1;
    DAT_00668148 = DAT_007fe9c0[param_1];
    return old;
}

// FUNCTION: LEGOLAND 0x00463870
LEGO_EXPORT int InitScreen(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00463ef0
void FUN_00463ef0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00463fc0
LEGO_EXPORT void PushRenderingStatusAndLockVideoSurface(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00464080
LEGO_EXPORT void PushRenderingStatusAndUnlockVideoSurface(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004640f0
void FUN_004640f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004641f0
LEGO_EXPORT void PopRenderingStatus(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00464310
LEGO_EXPORT int GetVideoSurface(struct VideoArg *arg) { STUB(); }

// FUNCTION: LEGOLAND 0x00464360
LEGO_EXPORT void PrintBackground(void) { return; }

// FUNCTION: LEGOLAND 0x00464370
LEGO_EXPORT void CommitCliprectToHardware(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00464400
LEGO_EXPORT void SetOverridePalette(unsigned int param_1) { DAT_006681e8 = param_1; }

// FUNCTION: LEGOLAND 0x00464410
LEGO_EXPORT unsigned int GetOverridePalette(void) { return DAT_006681e8; }

// FUNCTION: LEGOLAND 0x00464420
LEGO_EXPORT void SetOverrideFrame(unsigned int param_1) { DAT_004b9ca8 = param_1; }

// FUNCTION: LEGOLAND 0x00464430
LEGO_EXPORT unsigned int GetOverrideFrame(void) { return DAT_004b9ca8; }

// FUNCTION: LEGOLAND 0x00464440
LEGO_EXPORT void ClearOverrideFrame(void) { DAT_004b9ca8 = 0xffffffff; }

// FUNCTION: LEGOLAND 0x00464450
LEGO_EXPORT void ClearOverridePalette(void) { DAT_006681e8 = 0; }

// FUNCTION: LEGOLAND 0x00464460
LEGO_EXPORT void ClearSpriteOverrides(void) {
    DAT_004b9ca8 = 0xffffffff;
    DAT_006681e8 = 0;
}

// FUNCTION: LEGOLAND 0x00464480
void FUN_00464480(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00464a90
LEGO_EXPORT void ZBufferHelper(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00464ee0
void FUN_00464ee0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004651d0
LEGO_EXPORT void SoftPrint_Clear(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00465240
void FUN_00465240(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00465850
void FUN_00465850(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004659a0
void FUN_004659a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00465a40
LEGO_EXPORT void SoftPrint_XBltFast(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00465ee0
void FUN_00465ee0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00466080
void FUN_00466080(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004661d0
int FUN_004661d0(void) { STUB(); }

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
LEGO_EXPORT int RenderingComplete(void) {
    int result;

    ProcessSystemEvents();
    FUN_00455f70(0);
#if defined(_MSC_VER) && (_MSC_VER <= 1200) && defined(_M_IX86)
    __asm {
        pushad
        rdtsc
        mov ebx, DAT_00813a18
        sub eax, ebx
        mov DAT_00813a18, eax
        add DAT_00813a2c, eax
        popad
    }
#else
    {
        int tsc = (int)GetTickCount();
        tsc -= DAT_00813a18;
        DAT_00813a18 = tsc;
        DAT_00813a2c += tsc;
    }
#endif
    DAT_00667d68 = GetTickCount();
    result = DAT_004b9ca4();
    DAT_00813a2c = 0;
#if defined(_MSC_VER) && (_MSC_VER <= 1200) && defined(_M_IX86)
    __asm {
        pushad
        rdtsc
        mov DAT_00813a18, eax
        popad
    }
#else
    DAT_00813a18 = (int)GetTickCount();
#endif
    return result;
}

// FUNCTION: LEGOLAND 0x00466560
LEGO_EXPORT void PushSetTarget(struct Sprite *sprite) { STUB(); }

// FUNCTION: LEGOLAND 0x00466600
LEGO_EXPORT void PopTarget(void) {
    PopRenderingStatus();
    renderEngineTargetIdx--;
    renderEngine = renderEngineTargets[renderEngineTargetIdx];
    PopRenderingStatus();
    if (renderEngineTargetIdx >= 0) {
        return;
    }
    exit(2);
}

// FUNCTION: LEGOLAND 0x00466640
LEGO_EXPORT int RecreateSprite(struct Sprite *sprite) { STUB(); }

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
    strncpy((char *)&DAT_0066861c, param_1, 0x80);
    DAT_0066869b = 0;
}
