#include "legoland.h"
#include <windows.h>
#include <ddraw.h>

#include <stdlib.h>
#include <string.h>
#include "debug_alloc.h"
#include "globals.h"

#include "draw.h"
#include "gfx.h"
#include "llidb.h"
#include "print_sprite.h"
#include "text.h"
#include "timer.h"
#include "wndenv.h"

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x004636f0
LEGO_EXPORT int InstallDirectDraw(void) { return 0; }

// FUNCTION: LEGOLAND 0x00463700
LEGO_EXPORT int InitHostSystemGPU(void) {
    LPDIRECTDRAW ddraw;
    LPDIRECTDRAW2 ddraw2;

    if (DDRAWENV[1] != 0) {
        return 1;
    }
    if (DirectDrawCreate(NULL, (LPDIRECTDRAW *)&DDRAWENV[0], NULL) == 0) {
        ddraw = (LPDIRECTDRAW)DDRAWENV[0];
        if (ddraw->lpVtbl->QueryInterface(ddraw, &DAT_004acf80, (LPVOID *)&DDRAWENV[1]) != 0) {
            ((LPDIRECTDRAW)DDRAWENV[0])->lpVtbl->Release((LPDIRECTDRAW)DDRAWENV[0]);
            // STRING: LEGOLAND 0x004b9cd0
            DBPrintf("Can't Create DDCOM");
            return 0;
        }
        DDRAWENV[2] = 0x17c;
        DDRAWENV[97] = 0x17c;
        ddraw2 = (LPDIRECTDRAW2)DDRAWENV[1];
        if (ddraw2->lpVtbl->GetCaps(ddraw2, (LPDDCAPS)&DDRAWENV[2], (LPDDCAPS)&DDRAWENV[97]) != 0) {
            ((LPDIRECTDRAW2)DDRAWENV[1])->lpVtbl->Release((LPDIRECTDRAW2)DDRAWENV[1]);
            ((LPDIRECTDRAW)DDRAWENV[0])->lpVtbl->Release((LPDIRECTDRAW)DDRAWENV[0]);
            // STRING: LEGOLAND 0x004b9cb8
            DBPrintf("Can't get DDCOM caps");
            return 0;
        }
        // STRING: LEGOLAND 0x004b9cac
        AddFontResourceA("Lego.ttf");
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004637c0
LEGO_EXPORT int CheckHostSystemGPU(void) {
    int i;
    for (i = 0; i < 246; ++i) {
        DDRAWENV[i] = 0;
    }
    InitHostSystemGPU();
}

// FUNCTION: LEGOLAND 0x004637e0
LEGO_EXPORT void KillHostSystemGPU(void) {
    // STRING: LEGOLAND 0x004b9ce4
    RemoveFontResourceA("lego.ttf");
    if (DDRAWENV[1] != 0) {
        ((LPDIRECTDRAW2)DDRAWENV[1])->lpVtbl->Release((LPDIRECTDRAW2)DDRAWENV[1]);
        DDRAWENV[1] = 0;
    }
    DeleteObject((HGDIOBJ)PTR_00668090);
    DeleteObject((HGDIOBJ)PTR_00668098);
    DeleteObject((HGDIOBJ)PTR_0066808c);
    DeleteObject((HGDIOBJ)PTR_00668094);
    if (DDRAWENV[0] != 0) {
        ((LPDIRECTDRAW)DDRAWENV[0])->lpVtbl->Release((LPDIRECTDRAW)DDRAWENV[0]);
        DDRAWENV[0] = 0;
    }
}

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
int FUN_00463ef0(void) {
    DDSURFACEDESC desc;
    LPDIRECTDRAW2 ddraw2;

    if (DAT_00667d6c == 0) {
        ddraw2 = (LPDIRECTDRAW2)DDRAWENV[1];
        if (ddraw2->lpVtbl->SetDisplayMode(ddraw2, lpConfig->field_0, lpConfig->field_2, 0x10, 0, 0) != 0) {
            ddraw2 = (LPDIRECTDRAW2)DDRAWENV[1];
            if (ddraw2->lpVtbl->SetDisplayMode(ddraw2, lpConfig->field_0, lpConfig->field_2, 8, 0, 0) != 0) {
                return 0;
            }
        }
    }
    desc.dwSize = 0x6c;
    ddraw2 = (LPDIRECTDRAW2)DDRAWENV[1];
    ddraw2->lpVtbl->GetDisplayMode(ddraw2, &desc);
    if (desc.ddpfPixelFormat.dwRGBBitCount != 8) {
        if (desc.ddpfPixelFormat.dwRGBBitCount != 0x10) {
            return 0;
        }
        DAT_00668088 = (desc.ddpfPixelFormat.dwGBitMask == 0x7e0) + 1;
        return 1;
    }
    DAT_00668088 = 0;
    return 1;
}

// FUNCTION: LEGOLAND 0x00463fc0
LEGO_EXPORT void PushRenderingStatusAndLockVideoSurface(void) {
    RECT local;
    LPDIRECTDRAWSURFACE surface;
    int value;

    value = DAT_00668144;
    DAT_00668164[DAT_006681e4] = DAT_00668144;
    DAT_006681e4 = DAT_006681e4 + 1;
    if (value == 0) {
        local.left = value;
        local.top = value;
        local.right = lpConfig->field_0 - 1;
        local.bottom = lpConfig->field_2 - 1;
        DAT_0066809c.field_0 = 0x6c;
        IntersectRect((LPRECT)&DAT_00668108, &local, (RECT *)&SPRITE_ClipRect);
        surface = (LPDIRECTDRAWSURFACE)renderEngine;
        if (surface->lpVtbl->Lock(surface, NULL, (LPDDSURFACEDESC)&DAT_0066809c, 0x21, NULL) == 0x887601c2) {
            ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Restore((LPDIRECTDRAWSURFACE)renderEngine);
            ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Lock((LPDIRECTDRAWSURFACE)renderEngine, NULL, (LPDDSURFACEDESC)&DAT_0066809c, 0x21, NULL);
        }
        DAT_007fea44 = GetTransparentColour();
    }
    DAT_00668144 = 1;
}

// FUNCTION: LEGOLAND 0x00464080
LEGO_EXPORT void PushRenderingStatusAndUnlockVideoSurface(void) {
    LPDIRECTDRAWSURFACE surface;

    DAT_00668164[DAT_006681e4] = DAT_00668144;
    DAT_006681e4 = DAT_006681e4 + 1;
    if (DAT_00668144 != 0) {
        surface = (LPDIRECTDRAWSURFACE)renderEngine;
        if (surface->lpVtbl->Unlock(surface, DAT_0066809c.pixels) == 0x887601c2) {
            ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Restore((LPDIRECTDRAWSURFACE)renderEngine);
            ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Unlock((LPDIRECTDRAWSURFACE)renderEngine, DAT_0066809c.pixels);
        }
    }
    DAT_00668144 = 0;
}

// FUNCTION: LEGOLAND 0x004640f0
void FUN_004640f0(void) {
    RECT local;
    LPDIRECTDRAWSURFACE surface;
    int wasLocked;

    wasLocked = DAT_00668144;
    local.left = 0;
    local.top = 0;
    local.right = lpConfig->field_0 - 1;
    local.bottom = lpConfig->field_2 - 1;
    DAT_00668164[DAT_006681e4] = DAT_00668144;
    DAT_006681e4 = DAT_006681e4 + 1;
    if (wasLocked != 0) {
        surface = (LPDIRECTDRAWSURFACE)renderEngine;
        if (surface->lpVtbl->Unlock(surface, DAT_0066809c.pixels) == 0x887601c2) {
            ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Restore((LPDIRECTDRAWSURFACE)renderEngine);
            ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Unlock((LPDIRECTDRAWSURFACE)renderEngine, DAT_0066809c.pixels);
        }
    }
    DAT_0066809c.field_0 = 0x6c;
    IntersectRect((LPRECT)&DAT_00668108, &local, (RECT *)&SPRITE_ClipRect);
    surface = (LPDIRECTDRAWSURFACE)renderEngine;
    if (surface->lpVtbl->Lock(surface, NULL, (LPDDSURFACEDESC)&DAT_0066809c, 0x21, NULL) == 0x887601c2) {
        ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Restore((LPDIRECTDRAWSURFACE)renderEngine);
        ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Lock((LPDIRECTDRAWSURFACE)renderEngine, NULL, (LPDDSURFACEDESC)&DAT_0066809c, 0x21, NULL);
    }
    DAT_007fea44 = GetTransparentColour();
    DAT_00668144 = 1;
}

// FUNCTION: LEGOLAND 0x004641f0
LEGO_EXPORT void PopRenderingStatus(void) {
    RECT local;
    LPDIRECTDRAWSURFACE surface;

    DAT_006681e4 = DAT_006681e4 - 1;
    if (DAT_00668164[DAT_006681e4] != 0) {
        if (DAT_00668144 == 0) {
            local.left = 0;
            local.top = 0;
            local.right = lpConfig->field_0 - 1;
            local.bottom = lpConfig->field_2 - 1;
            DAT_0066809c.field_0 = 0x6c;
            IntersectRect((LPRECT)&DAT_00668108, &local, (RECT *)&SPRITE_ClipRect);
            surface = (LPDIRECTDRAWSURFACE)renderEngine;
            if (surface->lpVtbl->Lock(surface, NULL, (LPDDSURFACEDESC)&DAT_0066809c, 0x21, NULL) == 0x887601c2) {
                ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Restore((LPDIRECTDRAWSURFACE)renderEngine);
                ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Lock((LPDIRECTDRAWSURFACE)renderEngine, NULL, (LPDDSURFACEDESC)&DAT_0066809c, 0x21, NULL);
            }
            DAT_007fea44 = GetTransparentColour();
            DAT_00668144 = 1;
        }
        return;
    }
    if (DAT_00668144 != 0) {
        surface = (LPDIRECTDRAWSURFACE)renderEngine;
        if (surface->lpVtbl->Unlock(surface, DAT_0066809c.pixels) == 0x887601c2) {
            ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Restore((LPDIRECTDRAWSURFACE)renderEngine);
            ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Unlock((LPDIRECTDRAWSURFACE)renderEngine, DAT_0066809c.pixels);
        }
        DAT_00668144 = 0;
    }
}

// FUNCTION: LEGOLAND 0x00464310
LEGO_EXPORT int GetVideoSurface(struct VideoArg *arg) {
    if (DAT_00668144 == 0) {
        return 0;
    }
    arg->field_0 = DAT_0066809c.pitch;
    arg->field_4 = lpConfig->field_0;
    arg->field_8 = lpConfig->field_2;
    arg->field_c = DAT_0066809c.pixels;
    arg->field_14 = 2;
    return 1;
}

// FUNCTION: LEGOLAND 0x00464360
LEGO_EXPORT void PrintBackground(void) { return; }

// FUNCTION: LEGOLAND 0x00464370
LEGO_EXPORT void CommitCliprectToHardware(void) {
    struct ClipRgnData {
        /* 0x00 */ RGNDATAHEADER rdh;
        /* 0x20 */ RECT rect;
    } *rgn;
    LPDIRECTDRAWCLIPPER clipper;

    rgn = (struct ClipRgnData *)malloc(0x33);
    rgn->rdh.iType = 1;
    rgn->rdh.nCount = 1;
    rgn->rdh.dwSize = 0x20;
    rgn->rdh.nRgnSize = 0x10;
    rgn->rdh.rcBound = *(RECT *)&SPRITE_ClipRect;
    rgn->rect = *(RECT *)&SPRITE_ClipRect;
    clipper = (LPDIRECTDRAWCLIPPER)DAT_00668080;
    clipper->lpVtbl->SetClipList(clipper, (LPRGNDATA)rgn, 0);
    free(rgn);
}

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
LEGO_EXPORT void ZBufferHelper(unsigned int *param_1, int *param_2, int *param_3, void *param_4) { STUB(); }

// FUNCTION: LEGOLAND 0x00464ee0
void FUN_00464ee0(void *rect) { STUB(); }

// FUNCTION: LEGOLAND 0x004651d0
LEGO_EXPORT void SoftPrint_Clear(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00465240
void FUN_00465240(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00465850
void FUN_00465850(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004659a0
void FUN_004659a0(int param_1, int param_2, int param_3) {
    int height;
    int width;
    int src;
    unsigned short *dst;
    int i;
    int j;

    height = *(int *)(param_1 + 8);
    width = *(int *)(param_1 + 4);
    dst = (unsigned short *)((char *)DAT_0066809c.pixels + DAT_0066809c.pitch * param_3 + param_2 * 2);
    src = param_1 + 0x28 + (height - 1) * width * 2;
    for (i = height; i != 0; i--) {
        if (width > 0) {
            int offset = src - (int)dst;
            unsigned short *p = dst;
            j = width;
            do {
                unsigned short v = *(unsigned short *)(offset + (int)p);
                if (DAT_00668088 == 2) {
                    v = (v & 0x1f) | (v & 0xffe0) << 1;
                }
                *p = v;
                p++;
                j--;
            } while (j != 0);
        }
        dst = (unsigned short *)((char *)dst + DAT_0066809c.pitch);
        src += width * -2;
    }
}

// FUNCTION: LEGOLAND 0x00465a40
LEGO_EXPORT void SoftPrint_XBltFast(struct Sprite *sprite, RECT *a, RECT *b, unsigned int param_4) { STUB(); }

// FUNCTION: LEGOLAND 0x00465ee0
void FUN_00465ee0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00466080
int FUN_00466080(void) {
    DWORD tick;
    int result;
    int frames;
    LPDIRECTDRAWSURFACE primary;
    LPDIRECTDRAWSURFACE back;

    LLSAuto();
    if (lpConfig->field_1e != 0 && DAT_00668148 != 0) {
        PushRenderingStatusAndLockVideoSurface();
        PrintSprite(DAT_00668148, DAT_00813a44, DAT_00813a48, 0, 0);
        PopRenderingStatus();
    }
    tick = GetTickCount();
    while (tick - DAT_00668200 < 0x1c) {
        tick = GetTickCount();
    }
    DAT_00668200 = GetTickCount();
    primary = (LPDIRECTDRAWSURFACE)DAT_00668070;
    result = primary->lpVtbl->Flip(primary, NULL, 1);
    while (result != 0) {
        if (result == 0x887601c2) {
            ((LPDIRECTDRAWSURFACE)DAT_00668070)->lpVtbl->Restore((LPDIRECTDRAWSURFACE)DAT_00668070);
            ((LPDIRECTDRAWSURFACE)DAT_00668078)->lpVtbl->Restore((LPDIRECTDRAWSURFACE)DAT_00668078);
            return 0;
        }
        if (result != 0x887601ae && result != 0x8876021c) {
            return 0;
        }
        primary = (LPDIRECTDRAWSURFACE)DAT_00668070;
        result = primary->lpVtbl->Flip(primary, NULL, 1);
    }
    back = (LPDIRECTDRAWSURFACE)DAT_00668078;
    result = back->lpVtbl->GetFlipStatus(back, 2);
    while (result != 0) {
        back = (LPDIRECTDRAWSURFACE)DAT_00668078;
        result = back->lpVtbl->GetFlipStatus(back, 2);
    }
    tick = GetTickCount();
    FrameNumber = FrameNumber + 1;
    DAT_006681f8 = DAT_006681f8 + 1;
    if (tick - DAT_006681f0 >= 0x3e8) {
        FramesPerSecond = DAT_006681f8;
        DAT_006681f8 = 0;
        DAT_006681f0 = tick;
    }
    LastFrameMS = tick - DAT_006681f4;
    DAT_006681f4 = tick;
    return 1;
}

// FUNCTION: LEGOLAND 0x004661d0
int FUN_004661d0(void) {
    RECT dst;
    RECT client;
    DWORD tick;
    LPDIRECTDRAWSURFACE surface;
    int result;
    int frames;

    dst.left = 0;
    dst.top = 0;
    dst.right = 0x280;
    dst.bottom = 0x1e0;
    LLSAuto();
    if (lpConfig->field_1e != 0 && DAT_00668148 != 0) {
        PushRenderingStatusAndLockVideoSurface();
        PrintSprite(DAT_00668148, DAT_00813a44, DAT_00813a48, 0, 0);
        PopRenderingStatus();
    }
    tick = GetTickCount();
    while (tick - DAT_00668200 < 0x1c) {
        tick = GetTickCount();
    }
    DAT_00668200 = GetTickCount();
    GetClientRect((HWND)WNDENV_Gethwnd(), &client);
    ClientToScreen((HWND)WNDENV_Gethwnd(), (LPPOINT)&client);
    OffsetRect(&dst, client.left, client.top);
    surface = (LPDIRECTDRAWSURFACE)DAT_00668070;
    result = surface->lpVtbl->Blt(surface, &dst, (LPDIRECTDRAWSURFACE)DAT_00668078, NULL, 0x1000000, NULL);
    if (result == 0x887601c2) {
        ((LPDIRECTDRAWSURFACE)DAT_00668070)->lpVtbl->Restore((LPDIRECTDRAWSURFACE)DAT_00668070);
        result = ((LPDIRECTDRAWSURFACE)DAT_00668070)->lpVtbl->Blt((LPDIRECTDRAWSURFACE)DAT_00668070, &dst, (LPDIRECTDRAWSURFACE)DAT_00668078, NULL, 0x1000000, NULL);
    }
    if (result != 0) {
        return 0;
    }
    tick = GetTickCount();
    frames = DAT_006681f8 + 1;
    FrameNumber = FrameNumber + 1;
    DAT_006681f8 = frames;
    if (tick - DAT_006681f0 >= 0x3e8) {
        FramesPerSecond = frames;
        DAT_006681f8 = 0;
        DAT_006681f0 = tick;
    }
    LastFrameMS = tick - DAT_006681f4;
    DAT_006681f4 = tick;
    return 1;
}

// FUNCTION: LEGOLAND 0x00466360
void FUN_00466360(int a, int b) {
    short w;
    short h;

    if (DAT_00668208 == NULL) {
        // STRING: LEGOLAND 0x004b9d30
        DAT_00668208 = LoadSprite("Watch.lls", 4);
        if (DAT_00668208 == NULL) {
            return;
        }
    }
    DAT_00668204 = 1;
    w = DAT_00668208->width;
    h = DAT_00668208->height;
    DAT_007fea30.left = a;
    DAT_007fea30.right = w + a;
    DAT_007fea30.top = b;
    DAT_007fea30.bottom = h + b;
}

// FUNCTION: LEGOLAND 0x004663c0
void FUN_004663c0(void) {
    if (DAT_00668208 != 0) {
        KillSprite(DAT_00668208);
        DAT_00668208 = 0;
    }
    DAT_00668204 = 0;
}

// FUNCTION: LEGOLAND 0x004663f0
void FUN_004663f0(void) {
    RECT cursor;
    LPDIRECTDRAWSURFACE surface;
    struct Image *image;

    if (DAT_00668204 != 0) {
        if ((int)(GetTicks() - DAT_00667d60) > 0xc8) {
            cursor.left = DAT_007fea30.left;
            cursor.top = DAT_007fea30.top;
            cursor.right = DAT_007fea30.right;
            cursor.bottom = DAT_007fea30.bottom;
            DAT_00667d60 = GetTicks();
            image = DAT_00668208->image;
            FUN_0047d610((struct LLS *)image->data);
            PushRenderingStatusAndLockVideoSurface();
            PrintSprite(DAT_00668208, DAT_007fea30.left, DAT_007fea30.top, 0, 0);
            PopRenderingStatus();
            ClientToScreen((HWND)WNDENV_Gethwnd(), (LPPOINT)&cursor);
            ClientToScreen((HWND)WNDENV_Gethwnd(), (LPPOINT)&cursor.right);
            surface = (LPDIRECTDRAWSURFACE)DAT_00668070;
            if (surface->lpVtbl->Blt(surface, &cursor, (LPDIRECTDRAWSURFACE)DAT_00668078, &DAT_007fea30, 0x1000000, NULL) == 0x887601c2) {
                ((LPDIRECTDRAWSURFACE)DAT_00668070)->lpVtbl->Restore((LPDIRECTDRAWSURFACE)DAT_00668070);
                ((LPDIRECTDRAWSURFACE)DAT_00668070)->lpVtbl->Blt((LPDIRECTDRAWSURFACE)DAT_00668070, &cursor, (LPDIRECTDRAWSURFACE)DAT_00668078, NULL, 0x1000000, NULL);
            }
        }
    }
}

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
LEGO_EXPORT void PushSetTarget(struct Sprite *sprite) {
    LPDIRECTDRAWSURFACE surface;
    int locked;

    locked = DAT_00668144;
    DAT_00668164[DAT_006681e4] = DAT_00668144;
    DAT_006681e4 = DAT_006681e4 + 1;
    if (locked != 0) {
        surface = (LPDIRECTDRAWSURFACE)renderEngine;
        if (surface->lpVtbl->Unlock(surface, DAT_0066809c.pixels) == 0x887601c2) {
            ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Restore((LPDIRECTDRAWSURFACE)renderEngine);
            ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Unlock((LPDIRECTDRAWSURFACE)renderEngine, DAT_0066809c.pixels);
        }
    }
    DAT_00668144 = 0;
    renderEngineTargets[renderEngineTargetIdx] = renderEngine;
    renderEngineTargetIdx = renderEngineTargetIdx + 1;
    renderEngine = sprite->surface;
    FUN_004640f0();
}

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
LEGO_EXPORT int RecreateSprite(struct Sprite *sprite) {
    DDSURFACEDESC desc;
    DDCOLORKEY colorkey;
    LPDIRECTDRAW2 ddraw2;
    LPDIRECTDRAWSURFACE surface;

    desc.dwSize = 0x6c;
    desc.dwWidth = (short)sprite->width;
    desc.dwFlags = 7;
    desc.ddsCaps.dwCaps = 0x40;
    desc.dwHeight = (short)sprite->height;
    if ((sprite->flags & 0x10) == 0) {
        ddraw2 = (LPDIRECTDRAW2)DDRAWENV[1];
        if (ddraw2->lpVtbl->CreateSurface(ddraw2, &desc, (LPDIRECTDRAWSURFACE *)&sprite->surface, NULL) == 0) {
            goto created;
        }
    }
    desc.dwWidth = (short)sprite->width;
    desc.dwHeight = (short)sprite->height;
    desc.dwSize = 0x6c;
    desc.dwFlags = 7;
    desc.ddsCaps.dwCaps = 0x840;
    ddraw2 = (LPDIRECTDRAW2)DDRAWENV[1];
    if (ddraw2->lpVtbl->CreateSurface(ddraw2, &desc, (LPDIRECTDRAWSURFACE *)&sprite->surface, NULL) != 0) {
        ddraw2 = (LPDIRECTDRAW2)DDRAWENV[1];
        if (ddraw2->lpVtbl->Compact(ddraw2) == 0) {
            ((LPDIRECTDRAW2)DDRAWENV[1])->lpVtbl->CreateSurface((LPDIRECTDRAW2)DDRAWENV[1], &desc, (LPDIRECTDRAWSURFACE *)&sprite->surface, NULL);
        }
        return 0;
    }
created:
    if ((sprite->flags & 0x80) != 0) {
        colorkey.dwColorSpaceLowValue = GetNearestColour(0xff, 0, 0xff);
        surface = (LPDIRECTDRAWSURFACE)sprite->surface;
        colorkey.dwColorSpaceHighValue = colorkey.dwColorSpaceLowValue;
        surface->lpVtbl->SetColorKey(surface, 8, &colorkey);
        return 1;
    }
    colorkey.dwColorSpaceLowValue = GetTransparentColour();
    surface = (LPDIRECTDRAWSURFACE)sprite->surface;
    colorkey.dwColorSpaceHighValue = colorkey.dwColorSpaceLowValue;
    surface->lpVtbl->SetColorKey(surface, 8, &colorkey);
    return 1;
}

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
