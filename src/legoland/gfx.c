#include "legoland.h"
#include <ddraw.h>

#include "gfx.h"
#include "globals.h"

// FUNCTION: LEGOLAND 0x0044de90
void GetGFXFName(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044e010
struct Image *__BMPLoader(struct Image *image) { STUB(); }

// FUNCTION: LEGOLAND 0x0044e580
void LoadColourTable(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044e670
void ResendPalette(void) {
    if (DAT_00668088 == 0) {
        LPDIRECTDRAWSURFACE surface = (LPDIRECTDRAWSURFACE)DAT_00668070;
        surface->lpVtbl->SetPalette(surface, (LPDIRECTDRAWPALETTE)DAT_00668084);
    }
}

// FUNCTION: LEGOLAND 0x0044e690
unsigned int GetTransparentColour(void) {
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
unsigned int GetNearestColour(int r, int g, int b) {
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
