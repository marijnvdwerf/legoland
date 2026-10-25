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
#include "math.h"
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

    if (BubbleHelpGFXLoaded != 0) {
        return;
    }

    // STRING: LEGOLAND 0x004b9064
    if (LLIDB_FindElement("SPEECH BUBBLE", (struct Element **)&element, 0) == 0) {
        SpeechBubbleData = LLIDB_LoadData(element);
    }

    MoodIconSprites = 0;
    // STRING: LEGOLAND 0x004b9054
    MiHungrySprite = LoadSprite("mi_hungry.lls", 0);
    // STRING: LEGOLAND 0x004b9044
    MiHappySprite = LoadSprite("mi_happy.lls", 0);
    // STRING: LEGOLAND 0x004b9038
    MiSadSprite = LoadSprite("mi_sad.lls", 0);
    // STRING: LEGOLAND 0x004b902c
    MiHomeSprite = LoadSprite("mi_home.lls", 0);
    // STRING: LEGOLAND 0x004b9020
    MiEatSprite = LoadSprite("mi_eat.lls", 0);
    // STRING: LEGOLAND 0x004b9014
    GreatSprite = LoadSprite("great.lls", 0);
    // STRING: LEGOLAND 0x004b7a78
    PoorSprite = LoadSprite("poor.lls", 0);
    // STRING: LEGOLAND 0x004b9004
    FavouriteSprite = LoadSprite("favourite.lls", 0);
    // STRING: LEGOLAND 0x004b8ff8
    OpinionSprite = LoadSprite("opinion.lls", 0);
    // STRING: LEGOLAND 0x004b8fe8
    MiBoredSprite = LoadSprite("mi_bored.lls", 0);

    BubbleHelpGFXLoaded = 1;
}

// FUNCTION: LEGOLAND 0x00454a10
void UnloadBubbleHelpGFX(void) {
    struct Element *element;

    if (BubbleHelpGFXLoaded != 0) {
        BubbleHelpGFXLoaded = 0;
        if (LLIDB_FindElement("SPEECH BUBBLE", &element, 0) == 0) {
            LLIDB_UnLoadData(element);
        }
        if (MiHungrySprite != 0) {
            KillSprite(MiHungrySprite);
            MiHungrySprite = 0;
        }
        if (MiHappySprite != 0) {
            KillSprite(MiHappySprite);
            MiHappySprite = 0;
        }
        if (MiSadSprite != 0) {
            KillSprite(MiSadSprite);
            MiSadSprite = 0;
        }
        if (MiHomeSprite != 0) {
            KillSprite(MiHomeSprite);
            MiHomeSprite = 0;
        }
        if (MiEatSprite != 0) {
            KillSprite(MiEatSprite);
            MiEatSprite = 0;
        }
        if (GreatSprite != 0) {
            KillSprite(GreatSprite);
            GreatSprite = 0;
        }
        if (PoorSprite != 0) {
            KillSprite(PoorSprite);
            PoorSprite = 0;
        }
        if (FavouriteSprite != 0) {
            KillSprite(FavouriteSprite);
            FavouriteSprite = 0;
        }
        if (OpinionSprite != 0) {
            KillSprite(OpinionSprite);
            OpinionSprite = 0;
        }
        if (MiBoredSprite != 0) {
            KillSprite(MiBoredSprite);
            MiBoredSprite = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00454b40
LEGO_EXPORT HGDIOBJ SelectFont(HDC hdc, int font_id) {
    switch (font_id) {
    case 1:
        return SelectObject(hdc, LegoFont20Bold);
    case 2:
        return SelectObject(hdc, LegoFont18SemiBold);
    case 3:
        return SelectObject(hdc, LegoFont28Normal);
    default:
        return SelectObject(hdc, LegoFont24Bold);
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
void DrawTextOnRenderSurface(char *text, int font, RECT rc, COLORREF color) {
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
int MeasureTextHeight(const char *text, int font, int width) {
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

/* Measures word-wrapped text at the supplied width, translates the rectangle to the requested position, and draws
 * it clipped to the sprite clip rectangle. */
// FUNCTION: LEGOLAND 0x00455220
void DrawWrappedTextAt(int x, int y, const char *text, int font, int width) {
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
    region = CreateRectRgnIndirect(&SPRITE_ClipRect);
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
    int sprite_arg[3] = {5};
    struct TextCell *cell;
    HDC hdc;
    HGDIOBJ old_font;
    int text_h;
    int tail_w;
    int cx;
    int lim;
    int box_w;
    short corner_w;
    short corner_h;
    int below;
    RECT frame;
    short side_w;
    short side_h;
    int side_left;
    int mid_top;
    int mid_bot;
    int right_edge;

    box.left = 0;
    box.top = 0;
    box.bottom = 0;
    box.right = 200;
    cell = FindTextCellByStyle(text, font, 0x10, 0xd6dede, 0);
    if (!cell) {
        hdc = CreateCompatibleDC(NULL);
        SetBkMode(hdc, 1);
        old_font = SelectFont(hdc, font);
        text_h = DrawTextA(hdc, text, strlen(text), &box, 0x410);
        box.top = rect[1];
        box.bottom = text_h + box.top;
        SelectObject(hdc, old_font);
        DeleteDC(hdc);
        cell = CreateTextCell(text, box.right - box.left, text_h, font, 0x10, 0xd6dede, 0);
    } else {
        box.left = 0;
        box.top = 0;
        box.right = cell->width;
        box.bottom = cell->height;
        text_h = cell->height;
    }

    /* centre the tail between the anchor's left and right edges, kept on screen */
    tail_w = ((struct BubbleGfx *)SpeechBubbleData)->sprites[0]->width;
    cx = (rect[2] - tail_w + rect[0]) >> 1;
    if (cx < tail_w) {
        cx = tail_w;
    } else {
        lim = lpConfig->screen_width - tail_w;
        if (cx > lim) {
            cx = lim;
        }
    }

    /* centre the text box on the tail, kept on screen */
    corner_w = ((struct BubbleGfx *)SpeechBubbleData)->sprites[2]->width;
    corner_h = ((struct BubbleGfx *)SpeechBubbleData)->sprites[2]->height;
    box_w = box.right - box.left;
    box.left = cx - (box_w >> 1);
    box.right = cx + ((box_w + 1) >> 1);
    if (box.left < corner_w) {
        box.left = corner_w;
        box.right = box_w + box.left;
    } else {
        if (box.right >= lpConfig->screen_width - corner_w) {
            box.right = lpConfig->screen_width - corner_w;
            box.left = box.right - box_w;
        }
    }

    /* above the anchor if it fits, otherwise below it */
    if (rect[1] < box.bottom - box.top + 8) {
        below = 1;
        box.top = rect[3] + 6;
        box.bottom = text_h + box.top;
    } else {
        below = 0;
        box.bottom = rect[1] - 6;
        box.top = box.bottom - text_h;
    }

    /* the frame: top line, fill, bottom line */
    frame.right = box.right + 4;
    frame.left = box.left - 4;
    frame.top = box.top - 4;
    frame.bottom = box.bottom + 4;
    RenderBlock(frame.left, frame.top, frame.right - frame.left, 1, 0);
    RenderBlock(frame.left, frame.top + 1, frame.right - frame.left, frame.bottom - frame.top - 1,
        GetNearestColour(0xde, 0xde, 0xd6));
    RenderBlock(frame.left, frame.bottom, frame.right - frame.left, 1, 0);

    /* the tail points at the anchor */
    if (below) {
        PrintSprite(((struct BubbleGfx *)SpeechBubbleData)->sprites[0], cx, frame.top - corner_h, 0, sprite_arg);
    } else {
        PrintSprite(((struct BubbleGfx *)SpeechBubbleData)->sprites[1], cx, frame.bottom, 0, sprite_arg);
    }

    /* left side: corners, edge line and fill */
    side_w = ((struct BubbleGfx *)SpeechBubbleData)->sprites[2]->width;
    side_h = ((struct BubbleGfx *)SpeechBubbleData)->sprites[2]->height;
    side_left = frame.left - side_w;
    PrintSprite(((struct BubbleGfx *)SpeechBubbleData)->sprites[2], side_left, frame.top, 0, sprite_arg);
    mid_top = side_h + frame.top;
    mid_bot = frame.bottom - side_h;
    PrintSprite(((struct BubbleGfx *)SpeechBubbleData)->sprites[4], side_left, mid_bot + 1, 0, sprite_arg);
    RenderBlock(side_left, mid_top, 1, mid_bot - mid_top + 1, 0);
    RenderBlock(side_left + 1, mid_top, side_w - 1, mid_bot - mid_top + 1, GetNearestColour(0xde, 0xde, 0xd6));

    /* right side */
    PrintSprite(((struct BubbleGfx *)SpeechBubbleData)->sprites[3], frame.right, frame.top, 0, sprite_arg);
    PrintSprite(((struct BubbleGfx *)SpeechBubbleData)->sprites[5], frame.right, mid_bot + 1, 0, sprite_arg);
    right_edge = side_w + frame.right;
    RenderBlock(right_edge - 1, mid_top, 1, mid_bot - mid_top + 1, 0);
    RenderBlock(frame.right, mid_top, side_w - 1, mid_bot - mid_top + 1, GetNearestColour(0xde, 0xde, 0xd6));

    PrintTextCell(cell, box.left, box.top);

    if (MousePos.x >= frame.left && MousePos.x <= frame.right && MousePos.y >= frame.top &&
        MousePos.y <= frame.bottom) {
        Hover.type = 5;
    }
    if (MousePos.x >= side_left && MousePos.x <= right_edge && MousePos.y >= mid_top && MousePos.y <= mid_bot) {
        Hover.type = 5;
    }
}

/* Finds or builds a cached tooltip text cell, clamps its rectangle to the screen,
 * and draws the framed bubble above or below the supplied anchor rectangle. */
// FUNCTION: LEGOLAND 0x004557c0
LEGO_EXPORT void HTBubbleHelp(RECT *rect, char *text, int font) {
    RECT box;
    RECT frame;
    struct TextCell *cell;
    HDC hdc;
    HGDIOBJ old_font;
    unsigned int block_color;
    int text_h;
    int cx;

    box.left = 0;
    box.top = 0;
    box.right = 0;
    box.bottom = 0;
    block_color = GetNearestColour(0xda, 0xc6, 0x96);
    if (text != NULL) {
        box.right = 200;
        cell = FindTextCellByStyle(text, font, 0x10, 0x96c6da, 0);
        if (cell == NULL) {
            hdc = CreateCompatibleDC(NULL);
            SetBkMode(hdc, 1);
            old_font = SelectFont(hdc, font);
            text_h = DrawTextA(hdc, text, strlen(text), &box, 0x410);
            box.top = rect->top;
            box.bottom = text_h + box.top;
            SelectObject(hdc, old_font);
            DeleteDC(hdc);
            cell = CreateTextCell(text, box.right - box.left, text_h, font, 0x10, 0x96c6da, 0);
        } else {
            box.left = 0;
            box.top = 0;
            box.right = cell->width;
            box.bottom = cell->height;
            text_h = cell->height;
        }
        cx = (rect->right + rect->left) >> 1;
        if (cx < 0) {
            cx = 0;
        } else if (cx > (int)(unsigned int)lpConfig->screen_width) {
            cx = (unsigned int)lpConfig->screen_width;
        }
        {
            int width = box.right - box.left;
            box.left = cx - (width >> 1);
            box.right = ((width + 1) >> 1) + cx;
            if (box.left < 0) {
                box.left = 0;
            } else if (box.right >= (int)(unsigned int)lpConfig->screen_width) {
                box.left = (unsigned int)lpConfig->screen_width - width;
            }
            box.right = width + box.left;
        }
        if (rect->top < (box.bottom - box.top) + 8) {
            box.top = rect->bottom + 6;
            box.bottom = text_h + box.top;
        } else {
            box.bottom = rect->top + -6;
            box.top = box.bottom - text_h;
        }
        frame.left = box.left - 4;
        frame.top = box.top - 4;
        frame.right = box.right + 4;
        frame.bottom = box.bottom + 4;
        RenderBlock(frame.left + 1, frame.top + 1, frame.right - frame.left, frame.bottom - frame.top - 1, block_color);
        RenderBlock(frame.left, frame.top, frame.right - frame.left, 1, 0);
        RenderBlock(frame.left, frame.bottom, frame.right - frame.left, 1, 0);
        RenderBlock(frame.left, frame.top, 1, frame.bottom - frame.top, 0);
        RenderBlock(frame.right, frame.top, 1, frame.bottom - frame.top, 0);
        PrintTextCell(cell, box.left, box.top);
    }
}

// FUNCTION: LEGOLAND 0x00455a10
struct TextCell *FindTextCellBySprite(struct Sprite *sprite, int *out_index) {
    int i;
    struct TextCell *cell = TextCells;

    for (i = 0; i < TextCellCount; i++, cell++) {
        if (cell->sprite == sprite) {
            if (out_index != NULL) {
                *out_index = i;
            }
            return &TextCells[i];
        }
    }
    return NULL;
}

/* Looks up the sprite's text cell, fills its background, draws the formatted text, and sets the sprite color key to
 * the nearest background color. */
// FUNCTION: LEGOLAND 0x00455a50
int RenderTextCellSprite(struct Sprite *sprite) {
    RECT rc = {0};
    struct TextCell *cell;
    HDC hdc;
    COLORREF color;
    HBRUSH brush;
    HGDIOBJ old_font;
    DDCOLORKEY ck;
    LPDIRECTDRAWSURFACE surface;

    cell = FindTextCellBySprite(sprite, 0);
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
        ck.dwColorSpaceLowValue = ck.dwColorSpaceHighValue = GetNearestColour(color & 0xff, color >> 8 & 0xff, color >> 0x10 & 0xff);
        surface = (LPDIRECTDRAWSURFACE)cell->sprite->surface;
        surface->lpVtbl->SetColorKey(surface, 8, &ck);
    }
}

// FUNCTION: LEGOLAND 0x00455bb0
struct TextCell *CreateTextCell(char *name, int width, int height, int font, unsigned int format, unsigned int bg_color, unsigned int text_color) {
    struct TextCell *cell;

    // STRING: LEGOLAND 0x004b9080
    DBPrintf("Creating Cell (%d) %s\n", TextCellCount, name);
    if (TextCellCount >= 0x32) {
        FlushTextCells(1);
    }
    cell = &TextCells[TextCellCount];
    TextCellCount++;
    cell->width = width;
    cell->height = height;
    cell->format = format;
    cell->name = malloc(strlen(name) + 1);
    strcpy(cell->name, name);
    cell->bg_color = bg_color;
    cell->text_color = text_color;
    cell->font = font;
    cell->sprite = CreateFunctionBasedSprite(RenderTextCellSprite, (unsigned short)width, (unsigned short)height);
    cell->sprite->flags = cell->sprite->flags | 0x40;
    return cell;
}

// FUNCTION: LEGOLAND 0x00455c80
struct TextCell *FindTextCell(char *name, int width, int height, int font, unsigned int format, unsigned int bg_color, unsigned int text_color) {
    int i;

    for (i = 0; i < TextCellCount; i++) {
        if (TextCells[i].width == width && TextCells[i].height == height && TextCells[i].format == format &&
            TextCells[i].bg_color == bg_color && TextCells[i].text_color == text_color && TextCells[i].font == font &&
            strcmp(TextCells[i].name, name) == 0) {
            return &TextCells[i];
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00455d40
struct TextCell *FindTextCellByStyle(const char *name, int font, unsigned int format, unsigned int bg_color, unsigned int text_color) {
    int i;
    struct TextCell *cell = TextCells;

    for (i = 0; i < TextCellCount; i++, cell++) {
        if (cell->format == format && cell->bg_color == bg_color && cell->text_color == text_color && cell->font == font &&
            strcmp(cell->name, name) == 0) {
            return &TextCells[i];
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00455de0
struct TextCell *FindTextCellByName(char *name) {
    int i;
    struct TextCell *cell = TextCells;

    for (i = 0; i < TextCellCount; i++, cell++) {
        if (strcmp(cell->name, name) == 0) {
            return &TextCells[i];
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00455e50
void PrintTextBox(char *name, unsigned int x, unsigned int y, int width, int height, int font, unsigned int format, unsigned int bg_color, unsigned int text_color) {
    struct TextCell *cell;

    cell = FindTextCell(name, width, height, font, format, bg_color, text_color);
    if (cell == NULL) {
        cell = CreateTextCell(name, width, height, font, format, bg_color, text_color);
    }
    PrintSprite(cell->sprite, x, y, 0, 0);
}

// FUNCTION: LEGOLAND 0x00455ec0
void PrintTextCell(struct TextCell *cell, unsigned int x, unsigned int y) {
    PrintSprite(cell->sprite, x, y, 0, 0);
}

// FUNCTION: LEGOLAND 0x00455ee0
void DeleteTextCell(int index) {
    int i;
    struct TextCell *dst;

    // STRING: LEGOLAND 0x004b9098
    DBPrintf("Deleting Cell (%d) %s\n", index, TextCells[index].name);
    TextCellCount = TextCellCount - 1;
    free(TextCells[index].name);
    if (TextCells[index].sprite != NULL) {
        KillSprite(TextCells[index].sprite);
        TextCells[index].sprite = NULL;
    }
    for (i = index; i < TextCellCount; i++) {
        dst = &TextCells[i];
        *dst = dst[1];
    }
}

// FUNCTION: LEGOLAND 0x00455f70
void FlushTextCells(int evict_all) {
    int i;
    struct TextCell *cell = TextCells;

    for (i = 0; i < TextCellCount;) {
        if (evict_all == 0 && FrameCounter - cell->sprite->field_c <= 10) {
            i++;
            cell++;
        } else {
            DeleteTextCell(i);
        }
    }
}

/* Draw hover-help text with an optional visitor mood/status icon. Index 0 omits
   the icon; screens.c uses this for the hovered visitor's name. */
// FUNCTION: LEGOLAND 0x00455fc0
void HTBubbleHelpWithMood(RECT *rect, const char *text, int font, int mood_icon) {
    RECT box;
    RECT frame;
    HDC measure_dc;
    HDC surface_dc;
    HGDIOBJ old_font;
    struct TextCell *cell;
    unsigned int background_color;
    int center_x;
    int icon_width;
    int text_height;
    int half_icon_width;

    /* Keep the RECT initialization in field order, as in HTBubbleHelp. */
    box.left = 0;
    box.top = 0;
    box.right = 0;
    box.bottom = 0;
    background_color = GetNearestColour(0xda, 0xc6, 0x96);
    icon_width = 0;
    if (mood_icon) {
        icon_width = 0x28;
    }
    if (text != NULL) {
        box.right = 200;
        /* Reuse the measured text cell; otherwise measure it with GDI and cache it. */
        cell = FindTextCellByStyle(text, font, 0x10, 0x96c6da, 0);
        if (cell == NULL) {
            measure_dc = CreateCompatibleDC(NULL);
            SetBkMode(measure_dc, 1);
            old_font = SelectFont(measure_dc, font);
            text_height = DrawTextA(measure_dc, text, strlen(text), &box, 0x410);
            box.top = rect->top;
            box.bottom = text_height + box.top;
            SelectObject(measure_dc, old_font);
            DeleteDC(measure_dc);
            CreateTextCell((char *)text, box.right - box.left, text_height, font, 0x10, 0x96c6da, 0);
        } else {
            box.left = 0;
            box.top = 0;
            box.right = cell->width;
            box.bottom = cell->height;
            text_height = cell->height;
        }
        center_x = (rect->right + rect->left) >> 1;
        if (center_x < 0) {
            center_x = 0;
        } else if (center_x > (int)(unsigned int)lpConfig->screen_width) {
            center_x = (unsigned int)lpConfig->screen_width;
        }
        {
            int width = box.right - box.left;
            /* Store the text-only bounds first. The icon allowance belongs in the
               screen-edge test, not in this initial right edge (MSVC6 match). */
            box.left = center_x - (width >> 1);
            box.right = ((width + 1) >> 1) + center_x;
            if (box.left < 0) {
                box.left = 0;
            } else if (box.right + icon_width >= (int)(unsigned int)lpConfig->screen_width) {
                box.left = ((unsigned int)lpConfig->screen_width - width) - icon_width;
            }
            /* The original reserves 40 pixels at the edge, then extends this box by 20. */
            half_icon_width = icon_width / 2;
            box.right = half_icon_width + width + box.left;
        }
        /* Prefer above the hovered rectangle, falling below it when space is tight. */
        if (rect->top < (box.bottom - box.top) + 8) {
            box.top = rect->bottom + 6;
            box.bottom = text_height + box.top;
        } else {
            box.bottom = rect->top + -6;
            box.top = box.bottom - text_height;
        }
        /* Beige fill and a one-pixel black border around the padded text rectangle. */
        frame.left = box.left - 4;
        frame.top = box.top - 4;
        frame.right = box.right + 4;
        frame.bottom = 4 + box.bottom;
        RenderBlock(frame.left + 1, frame.top + 1, frame.right - frame.left, frame.bottom - frame.top - 1, background_color);
        RenderBlock(frame.left, frame.top, frame.right - frame.left, 1, 0);
        RenderBlock(frame.left, frame.bottom, frame.right - frame.left, 1, 0);
        RenderBlock(frame.left, frame.top, 1, frame.bottom - frame.top, 0);
        RenderBlock(frame.right, frame.top, 1, frame.bottom - frame.top, 0);
        if (mood_icon != 0) {
            PrintSprite((&MoodIconSprites)[mood_icon], frame.right - half_icon_width, (frame.top + frame.bottom) / 2 - 0x14, 0, 0);
        }
        /* DrawText needs a GDI DC: unlock the render surface, then restore its status. */
        PushRenderingStatusAndUnlockVideoSurface();
        ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &surface_dc);
        SetBkMode(surface_dc, 1);
        old_font = SelectFont(surface_dc, font);
        DrawTextA(surface_dc, text, strlen(text), &box, 0x10);
        SelectObject(surface_dc, old_font);
        ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, surface_dc);
        PopRenderingStatus();
    }
}
