#include "legoland.h"
#include <ddraw.h>

#include "gfx.h"
#include "globals.h"
#include "resource.h"

// FUNCTION: LEGOLAND 0x0044de90
LEGO_EXPORT void GetGFXFName(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044e010
LEGO_EXPORT struct Image *__BMPLoader(struct Image *image) { STUB(); }

// FUNCTION: LEGOLAND 0x0044e580
LEGO_EXPORT void LoadColourTable(void) {
    struct ResFile *file;
    PALETTEENTRY entries[256];
    PALETTEENTRY *entry;
    unsigned char *src;
    unsigned short *lookup;
    LPDIRECTDRAW2 ddraw;
    LPDIRECTDRAWSURFACE surface;

    // STRING: LEGOLAND 0x004b82f4
    file = RES_OpenFile(".\\graphics\\colours.tga");
    RES_ReadFile(file, DAT_00813b20, 0x12);
    RES_ReadFile(file, DAT_00813b20, 0x300);

    entry = entries;
    src = &DAT_00813b20[1];
    lookup = DAT_00813e20;
    do {
        unsigned char blue;
        entry->peRed = src[1];
        entry->peGreen = src[0];
        blue = src[-1];
        entry->peBlue = blue;
        src += 3;
        lookup++;
        entry->peFlags = 4;
        lookup[-1] = (unsigned short)((((entry->peRed & 0xf8) << 5 | (entry->peGreen & 0xf8)) << 2) | (blue >> 3));
        entry++;
    } while ((int)src < (int)&DAT_00813b20[0x301]);

    RES_ReadFile(file, DAT_00814020, 0x8000);

    ddraw = (LPDIRECTDRAW2)DDRAWENV[1];
    ddraw->lpVtbl->CreatePalette(ddraw, 0x44, entries, (LPDIRECTDRAWPALETTE *)&DAT_00668084, NULL);
    surface = (LPDIRECTDRAWSURFACE)DAT_00668070;
    surface->lpVtbl->SetPalette(surface, (LPDIRECTDRAWPALETTE)DAT_00668084);
    RES_CloseFile(file);
}

// FUNCTION: LEGOLAND 0x0044e670
LEGO_EXPORT void ResendPalette(void) {
    if (DAT_00668088 == 0) {
        LPDIRECTDRAWSURFACE surface = (LPDIRECTDRAWSURFACE)DAT_00668070;
        surface->lpVtbl->SetPalette(surface, (LPDIRECTDRAWPALETTE)DAT_00668084);
    }
}

// FUNCTION: LEGOLAND 0x0044e690
LEGO_EXPORT unsigned int GetTransparentColour(void) {
    switch (DAT_00668088) {
    case 0:
        return 0xfe;
    case 1:
        return 0x3ff;
    case 2:
        return 0x7ff;
    default:
        return 0;
    }
}

// FUNCTION: LEGOLAND 0x0044e6c0
LEGO_EXPORT unsigned int GetNearestColour(int r, int g, int b) {
    unsigned int color;
    switch (DAT_00668088) {
    case 0:
        color = (r & 0xf8) << 5;
        color |= (g & 0xf8);
        color <<= 2;
        color |= (b >> 3) & 0x1f;
        return DAT_00814020[color];
    case 1:
        color = (r & 0xf8) << 5;
        color |= (g & 0xf8);
        color <<= 2;
        color |= (b >> 3) & 0x1f;
        return color;
    case 2:
        color = (r & 0xf8) << 5;
        color |= (g & 0xfc);
        color <<= 3;
        color |= (b >> 3) & 0x1f;
        return color;
    default:
        return 0;
    }
}
