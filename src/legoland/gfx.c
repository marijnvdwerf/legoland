#include "legoland.h"
#include <ddraw.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "debug_alloc.h"
#include "gfx.h"
#include "globals.h"
#include "image_sprite.h"
#include "llidb.h"
#include "resource.h"

struct LLSFileHeader {
    unsigned short field_0;
    unsigned short field_2;
    unsigned short width;
    unsigned short pad_6;
    unsigned short height;
    unsigned short pad_a;
    unsigned int format;
    unsigned short pad_10;
    unsigned short field_12;
};

// FUNCTION: LEGOLAND 0x0044de90
LEGO_EXPORT char *GetGFXFName(const char *name, unsigned int type, char *out) {
    char *result = out;
    if (result == NULL) {
        result = DAT_006660b0;
    }
    switch (type & 0xff) {
    case 0:
        // STRING: LEGOLAND 0x004b7a90
        sprintf(result, "%s%s", DAT_004b81c0, name);
        break;
    case 1:
        sprintf(result, "%s%s", DAT_004b81c8, name);
        break;
    case 2:
        sprintf(result, "%s%s", DAT_004b81c4, name);
        break;
    case 3:
        sprintf(result, "%s%s", DAT_004b81c8, name);
        break;
    case 5:
        sprintf(result, "%s%s", DAT_004b81cc, name);
        break;
    case 6:
        sprintf(result, "%s%s", DAT_004b81d0, name);
        break;
    case 4:
        sprintf(result, "%s%s", DAT_004b81d4, name);
        break;
    case 7:
        // STRING: LEGOLAND 0x004b8278
        sprintf(result, ".\\graphics\\duke\\%s", name);
        break;
    case 8:
        // STRING: LEGOLAND 0x004b826c
        sprintf(result, "%s%s.MDL", DAT_004b81d8, name);
        break;
    case 9:
        sprintf(result, "%s%s", DAT_004b81d8, name);
        break;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0044e010
LEGO_EXPORT int __BMPLoader(struct Image *image) {
    char header[0xe];
    char info[0x2c];
    char ext[0xfc];
    unsigned int palette[0x100];
    char *path;
    struct ResFile *file;
    struct LLSFileHeader *lls;
    unsigned int size;
    unsigned int offbits;
    unsigned int pixel_offset;
    unsigned int type;
    unsigned int row_size;
    unsigned char *pixels;
    unsigned short *out;
    unsigned char *lut;
    int i;
    int j;

    _splitpath(image->name, NULL, NULL, NULL, ext);
    path = GetGFXFName(image->name, image->type, NULL);
    if (_stricmp(ext, ".lls") == 0 || _stricmp(ext, ".llz") == 0) {
        file = RES_OpenFile(path);
        if (file == NULL) {
            if (image->type == 1) {
                path = GetGFXFName(image->name, 0, NULL);
                file = RES_OpenFile(path);
            }
        }
        if (file == NULL) {
            // STRING: LEGOLAND 0x004b82d0
            FUN_0047f870("Failed to load (%s)", path);
            // STRING: LEGOLAND 0x004b82b8
            DBPrintf("Failed to load (%s)\n", path);
            return 0;
        }
        size = RES_GetFileSize(file);
        lls = (struct LLSFileHeader *)malloc(size);
        RES_ReadFile(file, lls, size);
        lls->field_0 = 0;
        lls->field_2 = 4;
        lls->field_12 = 4;
        RES_CloseFile(file);
        image->aux = NULL;
        image->width = lls->width;
        image->height = lls->height;
        if (lls->format == 8) {
            image->field_14 = 2;
        } else {
            image->field_14 = 3;
        }
        image->data = lls;
        if (DAT_00668088 == 2) {
            LLS555To565((struct LLSImage *)lls);
        }
        return 1;
    }

    file = RES_OpenFile(path);
    if (file == NULL) {
        // STRING: LEGOLAND 0x004b82a0
        FUN_0047f870("Failed to load (%s). ", path);
        DBPrintf("Failed to load (%s)\n", path);
        return 0;
    }
    // STRING: LEGOLAND 0x004b828c
    FUN_0047f870("Loading BMP (%s)", path);
    RES_ReadFile(file, header, 0xe);
    offbits = *(unsigned int *)(header + 0xa);
    pixel_offset = RES_GetFilePointer(file) + 0x28;
    RES_ReadFile(file, info, 0x2c);
    if (*(unsigned int *)(info + 0x10) != 0) {
        RES_CloseFile(file);
        return 0;
    }
    if ((*(unsigned short *)(info + 0xe) & 0xffff) == 8) {
        type = 0;
        row_size = (*(unsigned int *)(info + 4) + 3) & 0xfffffffc;
        row_size *= *(unsigned int *)(info + 8);
    } else if ((*(unsigned short *)(info + 0xe) & 0xffff) == 0x18) {
        type = 1;
        row_size = (*(unsigned int *)(info + 4) * 3 + 3) & 0xfffffffc;
        row_size *= *(unsigned int *)(info + 8);
    } else {
        RES_CloseFile(file);
        return 0;
    }
    RES_SetFilePointer(file, offbits, 0);
    pixels = (unsigned char *)malloc(row_size);
    if (pixels == NULL) {
        free(image);
        RES_CloseFile(file);
        return 0;
    }
    image->aux = NULL;
    image->width = *(unsigned short *)(info + 4);
    image->height = *(unsigned short *)(info + 8);
    image->field_14 = type;
    if (*(unsigned short *)(info + 0xe) == 8) {
        unsigned char *low;
        unsigned char *high;
        unsigned char *entry;

        RES_SetFilePointer(file, pixel_offset, 0);
        memset(palette, 0, 0x100 * sizeof(unsigned int));
        RES_ReadFile(file, palette, *(unsigned int *)(info + 0x20) * 4);
        RES_SetFilePointer(file, offbits, 0);
        RES_ReadFile(file, pixels, row_size);
        image->data = pixels;
        high = pixels + (image->height - 1) * ((image->width + 3) & 0xfffffffc);
        low = pixels;
        while (high > low) {
            unsigned char *a = low;
            unsigned char *b = high;
            low += (image->width + 3) & 0xfffffffc;
            high -= (image->width + 3) & 0xfffffffc;
            for (j = 0; j < image->width; j++) {
                unsigned char t = *a;
                *a = *b;
                *b = t;
                a++;
                b++;
            }
        }
        lut = (unsigned char *)malloc(0x208);
        image->aux = lut;
        entry = (unsigned char *)palette + 1;
        if (DAT_00668088 == 2) {
            for (i = 4; i < 0x204; i += 2) {
                *(unsigned short *)((unsigned char *)image->aux + i - 2) = (unsigned short)(((((entry[1] & 0xf8) << 5) | (entry[0] & 0xfc)) << 3) | (entry[-1] >> 3));
                entry += 4;
            }
        } else {
            for (i = 4; i < 0x204; i += 2) {
                *(unsigned short *)((unsigned char *)image->aux + i - 2) = (unsigned short)(((((entry[1] & 0xf8) << 5) | (entry[0] & 0xf8)) << 2) | (entry[-1] >> 3));
                entry += 4;
            }
        }
        free(pixels);
        RES_CloseFile(file);
        return 1;
    }

    RES_ReadFile(file, pixels, row_size);
    out = (unsigned short *)malloc(*(unsigned int *)(info + 8) * *(unsigned int *)(info + 4) * 2);
    image->data = out;
    if (out == NULL) {
        free(pixels);
        free(image);
        RES_CloseFile(file);
        return 0;
    }
    out = (unsigned short *)image->data + (image->height - 1) * image->width;
    if (DAT_00668088 == 2) {
        for (i = 0; i < image->height; i++) {
            unsigned char *src = pixels;
            unsigned short *dst = out;
            out -= image->width;
            pixels = (unsigned char *)(((unsigned int)(pixels + 3)) & 0xfffffffc);
            for (j = 0; j < image->width; j++) {
                *dst++ = (unsigned short)(((((src[2] & 0xf8) << 5) | (src[1] & 0xfc)) << 3) | (src[0] >> 3));
                src += 3;
            }
        }
    } else {
        for (i = 0; i < image->height; i++) {
            unsigned char *src = pixels;
            unsigned short *dst = out;
            out -= image->width;
            pixels = (unsigned char *)(((unsigned int)(pixels + 3)) & 0xfffffffc);
            for (j = 0; j < image->width; j++) {
                *dst++ = (unsigned short)(((((src[2] & 0xf8) << 5) | (src[1] & 0xf8)) << 2) | (src[0] >> 3));
                src += 3;
            }
        }
    }
    free(pixels);
    RES_CloseFile(file);
    return 1;
}

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
