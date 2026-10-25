#include <ddraw.h>
#include "legoland.h"

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
LEGO_EXPORT char *GetGFXFName(const char *name, unsigned char type, char *out) {
    char *result = out;
    if (result == NULL) {
        result = GfxFileNameBuffer;
    }
    switch (type) {
    case 0:
        // STRING: LEGOLAND 0x004b7a90
        sprintf(result, "%s%s", GraphicsPath, name);
        break;
    case 1:
        sprintf(result, "%s%s", GraphicsSmallPath, name);
        break;
    case 2:
        sprintf(result, "%s%s", GraphicsPathPrefix, name);
        break;
    case 3:
        sprintf(result, "%s%s", GraphicsSmallPath, name);
        break;
    case 5:
        sprintf(result, "%s%s", MasksPath, name);
        break;
    case 6:
        sprintf(result, "%s%s", MasksSmallPath, name);
        break;
    case 4:
        sprintf(result, "%s%s", IconsPath, name);
        break;
    case 7:
        // STRING: LEGOLAND 0x004b8278
        sprintf(result, ".\\graphics\\duke\\%s", name);
        break;
    case 8:
        // STRING: LEGOLAND 0x004b826c
        sprintf(result, "%s%s.MDL", ModelsPath, name);
        break;
    case 9:
        sprintf(result, "%s%s", ModelsPath, name);
        break;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0044e010
LEGO_EXPORT int __BMPLoader(struct Image *image) {
    BITMAPFILEHEADER header;
    BITMAPINFO info;
    char ext[0x100];
    RGBQUAD palette[0x100];
    char *path;
    struct ResFile *file;
    struct LLSFileHeader *lls;
    unsigned int size;
    int i;
    int j;
    int is_bmp;

    _splitpath(image->name, NULL, NULL, NULL, ext);
    path = GetGFXFName(image->name, image->type, NULL);
    /* is_bmp stays nonzero unless the extension is .lls or .llz. Written as one
       `a == 0 || b == 0` condition, MSVC6 moves the LLS branch to the end. */
    // STRING: LEGOLAND 0x004b82ec
    is_bmp = _strcmpi(".lls", ext);
    if (is_bmp != 0) {
        // STRING: LEGOLAND 0x004b82e4
        is_bmp = _strcmpi(".llz", ext);
    }
    if (is_bmp == 0) {
        file = RES_OpenFile(path);
        if (file == NULL && image->type == 1) {
            /* retry with the type-0 file name */
            path = GetGFXFName(image->name, 0, NULL);
            file = RES_OpenFile(path);
        }
        if (file == NULL) {
            // STRING: LEGOLAND 0x004b82d0
            DebugTrace("Failed to load (%s)", path);
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
        if (DisplayPixelFormat == 2) {
            LLS555To565((struct LLSImage *)lls);
        }
    } else {
        unsigned int offbits;
        unsigned int palette_offset;
        unsigned int type;
        unsigned int row_size;
        unsigned char *pixels;

        file = RES_OpenFile(path);
        if (file == NULL) {
            // STRING: LEGOLAND 0x004b82a0
            DebugTrace("Failed to load (%s). ", path);
            DBPrintf("Failed to load (%s)\n", path);
            return 0;
        }
        // STRING: LEGOLAND 0x004b828c
        DebugTrace("Loading BMP (%s)", path);
        RES_ReadFile(file, &header, sizeof(header));
        offbits = header.bfOffBits;
        /* the palette follows the 40-byte info header */
        palette_offset = RES_GetFilePointer(file) + sizeof(BITMAPINFOHEADER);
        RES_ReadFile(file, &info, sizeof(info));
        if (info.bmiHeader.biCompression != BI_RGB) {
            RES_CloseFile(file);
            return 0;
        }
        switch (info.bmiHeader.biBitCount) {
        case 8:
            row_size = ((info.bmiHeader.biWidth + 3) & ~3) * info.bmiHeader.biHeight;
            type = 0;
            break;
        case 24:
            row_size = ((info.bmiHeader.biWidth * 3 + 3) & ~3) * info.bmiHeader.biHeight;
            type = 1;
            break;
        default:
            RES_CloseFile(file);
            return 0;
        }
        RES_SetFilePointer(file, header.bfOffBits);
        pixels = (unsigned char *)malloc(row_size);
        if (pixels == NULL) {
            free(image);
            RES_CloseFile(file);
            return 0;
        }
        image->aux = NULL;
        image->width = (short)info.bmiHeader.biWidth;
        image->height = (short)info.bmiHeader.biHeight;
        image->field_14 = type;
        if (info.bmiHeader.biBitCount == 8) {
            unsigned char *low;
            unsigned char *high;

            RES_SetFilePointer(file, palette_offset);
            memset(palette, 0, sizeof(palette));
            RES_ReadFile(file, palette, info.bmiHeader.biClrUsed * 4);
            RES_SetFilePointer(file, offbits);
            RES_ReadFile(file, pixels, row_size);
            image->data = pixels;
            /* flip the rows: BMPs are stored bottom-up */
            high = pixels + (image->height - 1) * ((image->width + 3) & ~3);
            low = pixels;
            while (high > low) {
                unsigned char *b = high;
                unsigned char *a = low;
                low += (image->width + 3) & ~3;
                high -= (image->width + 3) & ~3;
                for (j = 0; j < image->width; j++, b++, a++) {
                    unsigned char t = *a;
                    *a = *b;
                    *b = t;
                }
            }
            /* 16-bit palette lookup table; entries 0..255 are written at aux + 4 */
            image->aux = malloc(0x208);
            if (DisplayPixelFormat == 2) {
                for (i = 0; i < 0x100; i++) {
                    ((unsigned short *)image->aux)[i + 2] =
                        (unsigned short)((((palette[i].rgbRed & 0xf8) << 5 | (palette[i].rgbGreen & 0xfc)) << 3) |
                            (palette[i].rgbBlue >> 3));
                }
            } else {
                for (i = 0; i < 0x100; i++) {
                    ((unsigned short *)image->aux)[i + 2] =
                        (unsigned short)((((palette[i].rgbRed & 0xf8) << 5 | (palette[i].rgbGreen & 0xf8)) << 2) |
                            (palette[i].rgbBlue >> 3));
                }
            }
            /* pixels now belong to image->data */
            RES_CloseFile(file);
            return 1;
        }

        RES_ReadFile(file, pixels, row_size);
        image->data = malloc(info.bmiHeader.biHeight * info.bmiHeader.biWidth * 2);
        if (image->data == NULL) {
            free(pixels);
            free(image);
            RES_CloseFile(file);
            return 0;
        }
        {
            unsigned char *src;
            unsigned short *dst;

            /* convert 24-bit BGR rows (bottom-up, 4-byte aligned) to 16-bit, top-down */
            if (DisplayPixelFormat == 2) {
                unsigned short *out;

                out = (unsigned short *)image->data + (image->height - 1) * image->width;
                src = pixels;
                for (i = 0; i < image->height; i++) {
                    src = (unsigned char *)(((UINT_PTR)src + 3) & ~(UINT_PTR)3);
                    dst = out;
                    out -= image->width;
                    for (j = 0; j < image->width; j++, dst++, src += 3) {
                        *dst = (unsigned short)((((src[2] & 0xf8) << 5 | (src[1] & 0xfc)) << 3) | (src[0] >> 3));
                    }
                }
            } else {
                unsigned short *out;

                out = (unsigned short *)image->data + (image->height - 1) * image->width;
                src = pixels;
                for (i = 0; i < image->height; i++) {
                    src = (unsigned char *)(((UINT_PTR)src + 3) & ~(UINT_PTR)3);
                    dst = out;
                    out -= image->width;
                    for (j = 0; j < image->width; j++, dst++, src += 3) {
                        *dst = (unsigned short)((((src[2] & 0xf8) << 5 | (src[1] & 0xf8)) << 2) | (src[0] >> 3));
                    }
                }
            }
        }
        free(pixels);
        RES_CloseFile(file);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0044e580
LEGO_EXPORT void LoadColourTable(void) {
    struct ResFile *file;
    PALETTEENTRY entries[256];
    int i;
    LPDIRECTDRAW2 ddraw;
    LPDIRECTDRAWSURFACE surface;

    // STRING: LEGOLAND 0x004b82f4
    file = RES_OpenFile(".\\graphics\\colours.tga");
    RES_ReadFile(file, PaletteReadBuffer, 0x12);
    RES_ReadFile(file, PaletteReadBuffer, 0x300);

    for (i = 0; i < 256; i++) {
        entries[i].peRed = PaletteReadBuffer[i * 3 + 2];
        entries[i].peGreen = PaletteReadBuffer[i * 3 + 1];
        entries[i].peBlue = PaletteReadBuffer[i * 3];
        Palette16Bit[i] = (unsigned short)((((entries[i].peRed & 0xf8) << 5 | (entries[i].peGreen & 0xf8)) << 2) |
            (entries[i].peBlue >> 3));
        entries[i].peFlags = 4;
    }

    RES_ReadFile(file, ColourLookupTable, 0x8000);

    ddraw = DDRAWENV.ddraw2;
    ddraw->lpVtbl->CreatePalette(ddraw, 0x44, entries, &DDPalette, NULL);
    surface = (LPDIRECTDRAWSURFACE)PrimarySurface;
    surface->lpVtbl->SetPalette(surface, DDPalette);
    RES_CloseFile(file);
}

// FUNCTION: LEGOLAND 0x0044e670
LEGO_EXPORT void ResendPalette(void) {
    if (DisplayPixelFormat == 0) {
        LPDIRECTDRAWSURFACE surface = (LPDIRECTDRAWSURFACE)PrimarySurface;
        surface->lpVtbl->SetPalette(surface, DDPalette);
    }
}

// FUNCTION: LEGOLAND 0x0044e690
LEGO_EXPORT unsigned int GetTransparentColour(void) {
    switch (DisplayPixelFormat) {
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
    switch (DisplayPixelFormat) {
    case 0:
        color = (r & 0xf8) << 5;
        color |= (g & 0xf8);
        color <<= 2;
        color |= (b >> 3) & 0x1f;
        return ColourLookupTable[color];
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
