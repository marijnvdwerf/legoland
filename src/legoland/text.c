#include "legoland.h"
#include "crt.h"
#include <windows.h>
#include <ddraw.h>
#include "globals.h"

#include "draw.h"
#include "debug_alloc.h"
#include "print_sprite.h"
#include "text.h"
#include "llidb.h"

#pragma intrinsic(strlen, strcmp)

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x00454910
LEGO_EXPORT void LoadBubbleHelpGFX(void) {
    void *element;

    if (DAT_006675b4 != 0) {
        return;
    }

    // STRING: LEGOLAND 0x004b9064
    if (LLIDB_FindElement("SPEECH BUBBLE", (unsigned int *)&element, 0) == 0) {
        DAT_00813a0c = LLIDB_LoadData(element);
    }

    DAT_008139e0 = 0;
    // STRING: LEGOLAND 0x004b9054
    DAT_008139e4 = LoadSprite("mi_hungry.lls", 0);
    // STRING: LEGOLAND 0x004b9044
    DAT_008139e8 = LoadSprite("mi_happy.lls", 0);
    // STRING: LEGOLAND 0x004b9038
    DAT_008139ec = LoadSprite("mi_sad.lls", 0);
    // STRING: LEGOLAND 0x004b902c
    DAT_008139f0 = LoadSprite("mi_home.lls", 0);
    // STRING: LEGOLAND 0x004b9020
    DAT_008139f4 = LoadSprite("mi_eat.lls", 0);
    // STRING: LEGOLAND 0x004b9014
    DAT_008139f8 = LoadSprite("great.lls", 0);
    // STRING: LEGOLAND 0x004b7a78
    DAT_008139fc = LoadSprite("poor.lls", 0);
    // STRING: LEGOLAND 0x004b9004
    DAT_00813a00 = LoadSprite("favourite.lls", 0);
    // STRING: LEGOLAND 0x004b8ff8
    DAT_00813a04 = LoadSprite("opinion.lls", 0);
    // STRING: LEGOLAND 0x004b8fe8
    DAT_00813a08 = LoadSprite("mi_bored.lls", 0);

    DAT_006675b4 = 1;
}

// FUNCTION: LEGOLAND 0x00454a10
void FUN_00454a10(void) {
    unsigned int element;

    if (DAT_006675b4 != 0) {
        DAT_006675b4 = 0;
        if (LLIDB_FindElement("SPEECH BUBBLE", &element, 0) == 0) {
            LLIDB_UnLoadData(element);
        }
        if (DAT_008139e4 != 0) {
            KillSprite(DAT_008139e4);
            DAT_008139e4 = 0;
        }
        if (DAT_008139e8 != 0) {
            KillSprite(DAT_008139e8);
            DAT_008139e8 = 0;
        }
        if (DAT_008139ec != 0) {
            KillSprite(DAT_008139ec);
            DAT_008139ec = 0;
        }
        if (DAT_008139f0 != 0) {
            KillSprite(DAT_008139f0);
            DAT_008139f0 = 0;
        }
        if (DAT_008139f4 != 0) {
            KillSprite(DAT_008139f4);
            DAT_008139f4 = 0;
        }
        if (DAT_008139f8 != 0) {
            KillSprite(DAT_008139f8);
            DAT_008139f8 = 0;
        }
        if (DAT_008139fc != 0) {
            KillSprite(DAT_008139fc);
            DAT_008139fc = 0;
        }
        if (DAT_00813a00 != 0) {
            KillSprite(DAT_00813a00);
            DAT_00813a00 = 0;
        }
        if (DAT_00813a04 != 0) {
            KillSprite(DAT_00813a04);
            DAT_00813a04 = 0;
        }
        if (DAT_00813a08 != 0) {
            KillSprite(DAT_00813a08);
            DAT_00813a08 = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00454b40
LEGO_EXPORT HGDIOBJ SelectFont(HDC hdc, int font_id) {
    switch (font_id) {
    case 1:
        return SelectObject(hdc, PTR_0066808c);
    case 2:
        return SelectObject(hdc, PTR_00668094);
    case 3:
        return SelectObject(hdc, PTR_00668098);
    default:
        return SelectObject(hdc, PTR_00668090);
    }
}

// FUNCTION: LEGOLAND 0x00454ba0
LEGO_EXPORT void Print(int x, int y, const char *text, int font) {
    HRGN region;
    HDC hdc;
    HGDIOBJ old_region;
    HGDIOBJ old_font;

    region = CreateRectRgn(SPRITE_ClipRect.left, SPRITE_ClipRect.top, SPRITE_ClipRect.right, SPRITE_ClipRect.bottom);
    PushRenderingStatusAndUnlockVideoSurface();
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &hdc);
    SetBkMode(hdc, 2);
    old_region = SelectObject(hdc, region);
    old_font = SelectFont(hdc, font);
    TextOutA(hdc, x, y, text, strlen(text));
    SelectObject(hdc, old_font);
    SelectObject(hdc, old_region);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
    PopRenderingStatus();
    DeleteObject(region);
}

// FUNCTION: LEGOLAND 0x00454c70
LEGO_EXPORT void PrintLimitedText(int x, int y, int width, const char *text, int font, COLORREF color, UINT format) {
    RECT rc;
    HRGN region;
    HDC hdc;
    HGDIOBJ old_region;
    HGDIOBJ old_font;

    rc.left = x;
    rc.right = x + width;
    rc.top = y;
    rc.bottom = y + 0x190;
    region = CreateRectRgn(SPRITE_ClipRect.left, SPRITE_ClipRect.top, SPRITE_ClipRect.right, SPRITE_ClipRect.bottom);
    PushRenderingStatusAndUnlockVideoSurface();
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &hdc);
    SetBkMode(hdc, 1);
    SetTextColor(hdc, color);
    old_region = SelectObject(hdc, region);
    old_font = SelectFont(hdc, font);
    DrawTextA(hdc, text, strlen(text), &rc, format);
    SelectObject(hdc, old_font);
    SelectObject(hdc, old_region);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
    PopRenderingStatus();
    DeleteObject(region);
}

// FUNCTION: LEGOLAND 0x00454d80
void FUN_00454d80(char *text, int font, RECT rc, COLORREF color) {
    HRGN region;
    HDC hdc;
    HGDIOBJ old_region;
    HGDIOBJ old_font;

    region = CreateRectRgn(SPRITE_ClipRect.left, SPRITE_ClipRect.top, SPRITE_ClipRect.right, SPRITE_ClipRect.bottom);
    PushRenderingStatusAndUnlockVideoSurface();
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &hdc);
    SetBkMode(hdc, 1);
    SetTextColor(hdc, color);
    old_region = SelectObject(hdc, region);
    old_font = SelectFont(hdc, font);
    DrawTextA(hdc, text, strlen(text), &rc, 0x20);
    SelectObject(hdc, old_font);
    SelectObject(hdc, old_region);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
    PopRenderingStatus();
    DeleteObject(region);
}

// FUNCTION: LEGOLAND 0x00454e60
LEGO_EXPORT void PrintCent(int cx, int y, int width, const char *text, int font) {
    RECT rc;
    HRGN region;
    HDC hdc;
    HGDIOBJ old_region;
    HGDIOBJ old_font;
    int half = width / 2;

    rc.left = cx - half;
    rc.right = cx + half;
    rc.top = y;
    rc.bottom = y + 0x190;
    region = CreateRectRgn(SPRITE_ClipRect.left, SPRITE_ClipRect.top, SPRITE_ClipRect.right, SPRITE_ClipRect.bottom);
    PushRenderingStatusAndUnlockVideoSurface();
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &hdc);
    SetBkMode(hdc, 1);
    old_region = SelectObject(hdc, region);
    old_font = SelectFont(hdc, font);
    DrawTextA(hdc, text, strlen(text), &rc, 0x11);
    SelectObject(hdc, old_font);
    SelectObject(hdc, old_region);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
    PopRenderingStatus();
    DeleteObject(region);
}

// FUNCTION: LEGOLAND 0x00454f60
LEGO_EXPORT void PrintCentOpaque(int cx, int y, const char *text, int font) {
    RECT rc;
    HRGN region;
    HDC hdc;
    HGDIOBJ old_region;
    HGDIOBJ old_font;

    rc.left = cx - 0x280;
    rc.right = cx + 0x280;
    rc.top = y;
    rc.bottom = y + 0x190;
    region = CreateRectRgn(SPRITE_ClipRect.left, SPRITE_ClipRect.top, SPRITE_ClipRect.right, SPRITE_ClipRect.bottom);
    PushRenderingStatusAndUnlockVideoSurface();
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &hdc);
    SetBkMode(hdc, 2);
    old_region = SelectObject(hdc, region);
    old_font = SelectFont(hdc, font);
    DrawTextA(hdc, text, strlen(text), &rc, 1);
    SelectObject(hdc, old_font);
    SelectObject(hdc, old_region);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
    PopRenderingStatus();
    DeleteObject(region);
}

// FUNCTION: LEGOLAND 0x00455060
LEGO_EXPORT void PrintCentColref(COLORREF color, int cx, int y, int width, const char *text, int font) {
    RECT rc;
    HRGN region;
    HDC hdc;
    HGDIOBJ old_region;
    HGDIOBJ old_font;
    COLORREF old_color;
    int half = width / 2;

    rc.left = cx - half;
    rc.right = cx + half;
    rc.top = y;
    rc.bottom = y + 0x190;
    region = CreateRectRgn(SPRITE_ClipRect.left, SPRITE_ClipRect.top, SPRITE_ClipRect.right, SPRITE_ClipRect.bottom);
    PushRenderingStatusAndUnlockVideoSurface();
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &hdc);
    SetBkMode(hdc, 1);
    old_color = SetTextColor(hdc, color);
    // STRING: LEGOLAND 0x004b9074
    DBPrintf("DC= %08x\n", hdc);
    old_region = SelectObject(hdc, region);
    old_font = SelectFont(hdc, font);
    DrawTextA(hdc, text, strlen(text), &rc, 0x11);
    SelectObject(hdc, old_font);
    SelectObject(hdc, old_region);
    SetTextColor(hdc, old_color);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
    PopRenderingStatus();
    DeleteObject(region);
}

// FUNCTION: LEGOLAND 0x004551a0
int FUN_004551a0(const char *text, int font, int width) {
    RECT rc;
    HDC hdc;

    rc.left = 0;
    rc.top = 0;
    rc.right = 0;
    rc.bottom = 0;
    hdc = CreateCompatibleDC(NULL);
    rc.right = width - 1;
    SetBkMode(hdc, 1);
    SelectFont(hdc, font);
    DrawTextA(hdc, text, strlen(text), &rc, 0x450);
    DeleteDC(hdc);
    return (rc.bottom - rc.top) + 1;
}

// FUNCTION: LEGOLAND 0x00455220
void FUN_00455220(int x, int y, const char *text, int font, int width) {
    RECT rc;
    HDC hdc;
    HRGN region;
    HDC ddhdc;
    HGDIOBJ old_region;
    HGDIOBJ old_font;

    rc.left = 0;
    rc.top = 0;
    rc.right = 0;
    rc.bottom = 0;
    hdc = CreateCompatibleDC(NULL);
    region = CreateRectRgnIndirect((RECT *)&SPRITE_ClipRect);
    rc.right = width - 1;
    SetBkMode(hdc, 1);
    SelectFont(hdc, font);
    DrawTextA(hdc, text, strlen(text), &rc, 0x450);
    DeleteDC(hdc);
    rc.left = rc.left + x;
    rc.top = rc.top + y;
    rc.right = rc.right + x;
    rc.bottom = rc.bottom + y;
    PushRenderingStatusAndUnlockVideoSurface();
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &ddhdc);
    SetBkMode(ddhdc, 1);
    old_region = SelectObject(ddhdc, region);
    old_font = SelectFont(ddhdc, font);
    DrawTextA(ddhdc, text, strlen(text), &rc, 0x50);
    SelectObject(ddhdc, old_font);
    SelectObject(ddhdc, old_region);
    DeleteObject(region);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, ddhdc);
    PopRenderingStatus();
}

// FUNCTION: LEGOLAND 0x00455370
LEGO_EXPORT void BubbleHelp(unsigned int *table, unsigned int a2, unsigned int a3) { STUB(); }

// FUNCTION: LEGOLAND 0x004557c0
LEGO_EXPORT void HTBubbleHelp(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455a10
struct TextCell *FUN_00455a10(struct Sprite *sprite, int *out_index) {
    int i = 0;
    struct TextCell *cell;

    if (0 < DAT_006675b8) {
        cell = DAT_006675c0;
        do {
            if (cell->sprite == sprite) {
                if (out_index != NULL) {
                    *out_index = i;
                }
                return &DAT_006675c0[i];
            }
            i++;
            cell++;
        } while (i < DAT_006675b8);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00455a50
void FUN_00455a50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455bb0
void FUN_00455bb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455c80
void FUN_00455c80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455d40
void FUN_00455d40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455de0
struct TextCell *FUN_00455de0(char *name) {
    int i = 0;
    struct TextCell *cell;

    if (0 < DAT_006675b8) {
        cell = DAT_006675c0;
        do {
            if (strcmp(cell->name, name) == 0) {
                return &DAT_006675c0[i];
            }
            i++;
            cell++;
        } while (i < DAT_006675b8);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00455e50
void FUN_00455e50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455ec0
void FUN_00455ec0(struct TextCell *cell, unsigned int x, unsigned int y) {
    PrintSprite(cell->sprite, x, y, 0, 0);
}

// FUNCTION: LEGOLAND 0x00455ee0
void FUN_00455ee0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455f70
void FUN_00455f70(int param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x00455fc0
void FUN_00455fc0(void) { STUB(); }
