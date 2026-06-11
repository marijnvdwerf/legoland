#include <windows.h>
#include <ddraw.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "debug_alloc.h"
#include "draw.h"
#include "gfx.h"
#include "llidb.h"
#include "print_sprite.h"
#include "render.h"
#include "text.h"

#pragma intrinsic(strlen, strcmp, strcpy)

struct BubbleGfx {
    /* 0x00 */ unsigned char pad_0[8];
    /* 0x08 */ struct Sprite **sprites;
};

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
LEGO_EXPORT void BubbleHelp(int *rect, char *text, int font) {
    RECT box;
    struct TextCell *cell;
    HDC hdc;
    HGDIOBJ old_font;
    int sprite_arg[3];
    struct Sprite **sprites;
    unsigned int color;
    short corner_h;
    int text_h;
    int cx;
    int box_w;
    int corner_w;
    int row_y;
    int right4;
    int left4;
    int width;
    int top4;
    int bottom4;
    int left_corner_w;
    int mid_top;
    int mid_h;
    int hit_left;
    struct Sprite *top_sprite;
    int fits_above;

    sprite_arg[1] = 0;
    sprite_arg[0] = 5;
    sprite_arg[2] = 0;
    box.left = 0;
    box.top = 0;
    box.bottom = 0;
    box.right = 200;
    cell = FUN_00455d40(text, font, 0x10, 0xd6dede, 0);
    if (cell == NULL) {
        hdc = CreateCompatibleDC(NULL);
        SetBkMode(hdc, 1);
        old_font = SelectFont(hdc, font);
        text_h = DrawTextA(hdc, text, strlen(text), &box, 0x410);
        box.top = rect[1];
        box.bottom = box.top + text_h;
        SelectObject(hdc, old_font);
        DeleteDC(hdc);
        cell = FUN_00455bb0(text, box.right - box.left, text_h, font, 0x10, 0xd6dede, 0);
    } else {
        box.left = 0;
        box.top = 0;
        box.right = cell->width;
        box.bottom = cell->height;
        text_h = cell->height;
    }
    sprites = ((struct BubbleGfx *)DAT_00813a0c)->sprites;
    cx = (short)sprites[0]->width;
    box_w = (rect[2] - cx) + rect[0] >> 1;
    if (box_w < cx || (cx = (unsigned int)lpConfig->field_0 - cx, cx < box_w)) {
        box_w = cx;
    }
    cx = box_w;
    corner_h = (short)sprites[2]->height;
    box_w = box.right - box.left;
    box.left = cx - (box_w >> 1);
    box.right = ((box_w + 1) >> 1) + cx;
    corner_w = (short)sprites[2]->width;
    if (box.left < corner_w) {
        box.left = corner_w;
        box.right = box_w + corner_w;
    } else {
        corner_w = (unsigned int)lpConfig->field_0 - corner_w;
        if (corner_w <= box.right) {
            box.left = corner_w - box_w;
            box.right = corner_w;
        }
    }
    fits_above = (box.bottom - box.top) + 8 <= rect[1];
    if (fits_above) {
        box.bottom = rect[1] + -6;
        row_y = box.bottom - text_h;
    } else {
        row_y = rect[3] + 6;
        box.bottom = text_h + row_y;
    }
    right4 = box.right + 4;
    left4 = box.left + -4;
    width = right4 - left4;
    top4 = row_y + -4;
    bottom4 = box.bottom + 4;
    box.top = row_y;
    box.left = left4;
    RenderBlock(left4, top4, width, 1, 0);
    color = GetNearestColour(0xde, 0xde, 0xd6);
    RenderBlock(left4, row_y + -3, width, (bottom4 - top4) + -1, color);
    RenderBlock(left4, bottom4, width, 1, 0);
    if (fits_above) {
        top_sprite = sprites[1];
    } else {
        row_y = top4 - corner_h;
        top_sprite = sprites[0];
    }
    PrintSprite(top_sprite, cx, row_y, 0, sprite_arg);
    corner_h = (short)sprites[2]->height;
    left_corner_w = (short)sprites[2]->width;
    left4 = left4 - left_corner_w;
    PrintSprite(sprites[2], left4, top4, 0, sprite_arg);
    mid_top = bottom4 - corner_h;
    width = corner_h + top4;
    PrintSprite(sprites[4], left4, mid_top + 1, 0, sprite_arg);
    mid_h = (mid_top - width) + 1;
    RenderBlock(left4, width, 1, mid_h, 0);
    color = GetNearestColour(0xde, 0xde, 0xd6);
    RenderBlock(left4 + 1, width, left_corner_w + -1, mid_h, color);
    PrintSprite(sprites[3], right4, top4, 0, sprite_arg);
    PrintSprite(sprites[5], right4, mid_top + 1, 0, sprite_arg);
    RenderBlock(left_corner_w + right4 + -1, width, 1, mid_h, 0);
    color = GetNearestColour(0xde, 0xde, 0xd6);
    RenderBlock(right4, width, left_corner_w + -1, mid_h, color);
    FUN_00455ec0(cell, box.left, box.top);
    hit_left = left4;
    if (box.left <= (int)DAT_00813a44 && (int)DAT_00813a44 <= right4 && top4 <= (int)DAT_00813a48 &&
        (int)DAT_00813a48 <= bottom4) {
        DAT_004bdd00 = 5;
    }
    if (hit_left <= (int)DAT_00813a44 && (int)DAT_00813a44 <= left_corner_w + right4 &&
        width <= (int)DAT_00813a48 && (int)DAT_00813a48 <= mid_top) {
        DAT_004bdd00 = 5;
    }
}

// FUNCTION: LEGOLAND 0x004557c0
LEGO_EXPORT void HTBubbleHelp(int *rect, char *text, int font) {
    RECT box;
    struct TextCell *cell;
    HDC hdc;
    HGDIOBJ old_font;
    unsigned int block_color;
    int text_h;
    int cx;
    int x4;
    int y4;
    int w;
    int h;
    int x8;

    box.left = 0;
    box.top = 0;
    box.right = 0;
    box.bottom = 0;
    block_color = GetNearestColour(0xda, 0xc6, 0x96);
    if (text != NULL) {
        box.right = 200;
        cell = FUN_00455d40(text, font, 0x10, 0x96c6da, 0);
        if (cell == NULL) {
            hdc = CreateCompatibleDC(NULL);
            SetBkMode(hdc, 1);
            old_font = SelectFont(hdc, font);
            text_h = DrawTextA(hdc, text, strlen(text), &box, 0x410);
            box.top = rect[1];
            box.bottom = text_h + box.top;
            SelectObject(hdc, old_font);
            DeleteDC(hdc);
            cell = FUN_00455bb0(text, box.right - box.left, text_h, font, 0x10, 0x96c6da, 0);
        } else {
            box.left = 0;
            box.top = 0;
            box.right = cell->width;
            box.bottom = cell->height;
            text_h = cell->height;
        }
        cx = (rect[2] + rect[0]) >> 1;
        if (cx < 0) {
            cx = 0;
        } else if (cx > (int)(unsigned int)lpConfig->field_0) {
            cx = (unsigned int)lpConfig->field_0;
        }
        box.right = box.right - box.left;
        box.left = cx - (box.right >> 1);
        if (box.left < 0) {
            box.left = 0;
        } else if ((((box.right + 1) >> 1) + cx) >= (int)(unsigned int)lpConfig->field_0) {
            box.left = (unsigned int)lpConfig->field_0 - box.right;
        }
        box.right = box.right + box.left;
        if (rect[1] < (box.bottom - box.top) + 8) {
            box.top = rect[3] + 6;
            box.bottom = text_h + box.top;
        } else {
            box.bottom = rect[1] + -6;
            box.top = box.bottom - text_h;
        }
        y4 = box.bottom + 4;
        x4 = box.top + -4;
        h = y4 - x4;
        w = box.right + 4;
        x8 = box.left + -4;
        RenderBlock(box.left + -3, box.top + -3, w - x8, h + -1, block_color);
        RenderBlock(x8, x4, w - x8, 1, 0);
        RenderBlock(x8, y4, w - x8, 1, 0);
        RenderBlock(x8, x4, 1, h, 0);
        RenderBlock(w, x4, 1, h, 0);
        FUN_00455ec0(cell, box.left, box.top);
    }
}

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
int FUN_00455a50(struct Sprite *sprite) {
    RECT rc;
    struct TextCell *cell;
    HDC hdc;
    COLORREF color;
    HBRUSH brush;
    HGDIOBJ old_font;
    DDCOLORKEY ck;
    LPDIRECTDRAWSURFACE surface;

    rc.top = 0;
    rc.left = 0;
    cell = FUN_00455a10(sprite, 0);
    if (cell != NULL) {
        rc.right = cell->width;
        rc.bottom = cell->height;
        PushRenderingStatusAndUnlockVideoSurface();
        ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &hdc);
        color = GetNearestColor(hdc, cell->bg_color & 0xffffff);
        SetBkMode(hdc, 1);
        SetBkColor(hdc, color);
        brush = CreateSolidBrush(color);
        FillRect(hdc, &rc, brush);
        DeleteObject(brush);
        SetTextColor(hdc, cell->text_color & 0xffffff);
        old_font = SelectFont(hdc, cell->font);
        DrawTextA(hdc, cell->name, strlen(cell->name), &rc, cell->format);
        SelectObject(hdc, old_font);
        ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
        PopRenderingStatus();
        ck.dwColorSpaceLowValue = GetNearestColour(color & 0xff, color >> 8 & 0xff, color >> 0x10 & 0xff);
        ck.dwColorSpaceHighValue = ck.dwColorSpaceLowValue;
        surface = (LPDIRECTDRAWSURFACE)cell->sprite->surface;
        surface->lpVtbl->SetColorKey(surface, 8, &ck);
    }
}

// FUNCTION: LEGOLAND 0x00455bb0
struct TextCell *FUN_00455bb0(char *name, int width, int height, int font, unsigned int format, unsigned int bg_color, unsigned int text_color) {
    struct TextCell *cell;

    // STRING: LEGOLAND 0x004b9080
    DBPrintf("Creating Cell (%d) %s\n", DAT_006675b8, name);
    if (DAT_006675b8 >= 0x32) {
        FUN_00455f70(1);
    }
    cell = &DAT_006675c0[DAT_006675b8];
    DAT_006675b8 = DAT_006675b8 + 1;
    cell->format = format;
    cell->width = width;
    cell->height = height;
    cell->name = malloc(strlen(name) + 1);
    strcpy(cell->name, name);
    cell->text_color = text_color;
    cell->bg_color = bg_color;
    cell->font = font;
    cell->sprite = CreateFunctionBasedSprite(FUN_00455a50, (unsigned short)width, (unsigned short)height);
    cell->sprite->flags = cell->sprite->flags | 0x40;
    return cell;
}

// FUNCTION: LEGOLAND 0x00455c80
struct TextCell *FUN_00455c80(char *name, int width, int height, int font, unsigned int format, unsigned int bg_color, unsigned int text_color) {
    int i = 0;
    struct TextCell *cell;

    if (0 < DAT_006675b8) {
        cell = DAT_006675c0;
        do {
            if (cell->width == width && cell->height == height && cell->format == format &&
                cell->bg_color == bg_color && cell->text_color == text_color && cell->font == font &&
                strcmp(cell->name, name) == 0) {
                return &DAT_006675c0[i];
            }
            i++;
            cell++;
        } while (i < DAT_006675b8);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00455d40
struct TextCell *FUN_00455d40(char *name, int font, unsigned int format, unsigned int bg_color, unsigned int text_color) {
    int i = 0;
    struct TextCell *cell;

    if (0 < DAT_006675b8) {
        cell = DAT_006675c0;
        do {
            if (cell->format == format && cell->bg_color == bg_color && cell->text_color == text_color &&
                cell->font == font && strcmp(cell->name, name) == 0) {
                return &DAT_006675c0[i];
            }
            i++;
            cell++;
        } while (i < DAT_006675b8);
    }
    return NULL;
}

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
void FUN_00455e50(char *name, unsigned int x, unsigned int y, int width, int height, int font, unsigned int format, unsigned int bg_color, unsigned int text_color) {
    struct TextCell *cell;

    cell = FUN_00455c80(name, width, height, font, format, bg_color, text_color);
    if (cell == NULL) {
        cell = FUN_00455bb0(name, width, height, font, format, bg_color, text_color);
    }
    PrintSprite(cell->sprite, x, y, 0, 0);
}

// FUNCTION: LEGOLAND 0x00455ec0
void FUN_00455ec0(struct TextCell *cell, unsigned int x, unsigned int y) {
    PrintSprite(cell->sprite, x, y, 0, 0);
}

// FUNCTION: LEGOLAND 0x00455ee0
void FUN_00455ee0(int index) {
    // STRING: LEGOLAND 0x004b9098
    DBPrintf("Deleting Cell (%d) %s\n", index, DAT_006675c0[index].name);
    DAT_006675b8 = DAT_006675b8 - 1;
    free(DAT_006675c0[index].name);
    if (DAT_006675c0[index].sprite != NULL) {
        KillSprite(DAT_006675c0[index].sprite);
        DAT_006675c0[index].sprite = NULL;
    }
    if (index < DAT_006675b8) {
        struct TextCell *dst = &DAT_006675c0[index];
        do {
            index++;
            *dst = dst[1];
            dst++;
        } while (index < DAT_006675b8);
    }
}

// FUNCTION: LEGOLAND 0x00455f70
void FUN_00455f70(int evict_all) {
    int i = 0;
    struct TextCell *cell;

    if (0 < DAT_006675b8) {
        cell = DAT_006675c0;
        do {
            if (evict_all == 0 && DAT_008119a4 - cell->sprite->field_c <= 10) {
                i++;
                cell++;
            } else {
                FUN_00455ee0(i);
            }
        } while (i < DAT_006675b8);
    }
}

// FUNCTION: LEGOLAND 0x00455fc0
void FUN_00455fc0(int *rect, const char *text, int font, int mood) {
    RECT box;
    HDC hdc;
    HDC ddhdc;
    HGDIOBJ old_font;
    struct TextCell *cell;
    unsigned int block_color;
    int mood_pad;
    int text_h;
    int cx;
    int half_mood;
    int x4;
    int y4;
    int w;
    int h;

    box.right = 0;
    box.bottom = 0;
    box.top = 0;
    box.left = 0;
    block_color = GetNearestColour(0xda, 0xc6, 0x96);
    mood_pad = 0;
    if (mood != 0) {
        mood_pad = 0x28;
    }
    if (text != NULL) {
        box.right = 200;
        cell = FUN_00455d40(text, font, 0x10, 0x96c6da, 0);
        if (cell == NULL) {
            hdc = CreateCompatibleDC(NULL);
            SetBkMode(hdc, 1);
            old_font = SelectFont(hdc, font);
            text_h = DrawTextA(hdc, text, strlen(text), &box, 0x410);
            box.top = rect[1];
            box.bottom = text_h + box.top;
            SelectObject(hdc, old_font);
            DeleteDC(hdc);
            FUN_00455bb0((char *)text, box.right - box.left, text_h, font, 0x10, 0x96c6da, 0);
        } else {
            box.left = 0;
            box.top = 0;
            box.right = cell->width;
            box.bottom = cell->height;
            text_h = cell->height;
        }
        cx = (rect[2] + rect[0]) >> 1;
        if (cx < 0) {
            cx = 0;
        } else if ((int)(unsigned int)lpConfig->field_0 < cx) {
            cx = (unsigned int)lpConfig->field_0;
        }
        box.right = box.right - box.left;
        box.left = cx - (box.right >> 1);
        if (box.left < 0) {
            box.left = 0;
        } else if ((int)(unsigned int)lpConfig->field_0 <= (((box.right + 1) >> 1) + cx + mood_pad)) {
            box.left = ((unsigned int)lpConfig->field_0 - box.right) - mood_pad;
        }
        half_mood = mood_pad / 2;
        box.right = half_mood + box.right + box.left;
        if (rect[1] < (box.bottom - box.top) + 8) {
            box.top = rect[3] + 6;
            box.bottom = text_h + box.top;
        } else {
            box.bottom = rect[1] + -6;
            box.top = box.bottom - text_h;
        }
        y4 = box.bottom + 4;
        x4 = box.top + -4;
        h = y4 - x4;
        w = box.right + 4;
        text_h = box.left + -4;
        RenderBlock(box.left + -3, box.top + -3, w - text_h, h + -1, block_color);
        RenderBlock(text_h, x4, w - text_h, 1, 0);
        RenderBlock(text_h, y4, w - text_h, 1, 0);
        RenderBlock(text_h, x4, 1, h, 0);
        RenderBlock(w, x4, 1, h, 0);
        if (mood != 0) {
            PrintSprite((&DAT_008139e0)[mood], w - half_mood, (x4 + y4) / 2 + -0x14, 0, 0);
        }
        PushRenderingStatusAndUnlockVideoSurface();
        ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &ddhdc);
        SetBkMode(ddhdc, 1);
        old_font = SelectFont(ddhdc, font);
        DrawTextA(ddhdc, text, strlen(text), &box, 0x10);
        SelectObject(ddhdc, old_font);
        ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, ddhdc);
        PopRenderingStatus();
    }
}
