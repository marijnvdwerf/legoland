#pragma once

#include "legoland.h"

/* Shared layouts: struct Sprite and struct Image are defined once here and
 * #included by every TU that touches them (image_sprite.c, gfx.c, mapscreen.c,
 * ride_bloke.c, timer.c, ...). Field names carry their byte offset. */
struct Image {
    /* 0x00 */ void *data;
    /* 0x04 */ void *field_4;
    /* 0x08 */ short width;
    /* 0x0a */ short height;
    /* 0x0c */ unsigned short refcount;
    /* 0x0e */ unsigned char type;
    /* 0x0f */ unsigned char pad_f[0x10 - 0xf];
    /* 0x10 */ char *name;
    /* 0x14 */ unsigned int field_14;
};

struct Sprite {
    /* 0x00 */ struct Sprite *next;
    /* 0x04 */ unsigned int field_4;
    /* 0x08 */ struct Image *image;
    /* 0x0c */ unsigned int field_c;
    /* 0x10 */ unsigned int field_10;
    /* 0x14 */ unsigned short field_14;
    /* 0x16 */ unsigned short field_16;
    /* 0x18 */ unsigned short field_18;
    /* 0x1a */ unsigned short field_1a;
    /* 0x1c */ unsigned short field_1c;
};

LEGO_EXPORT struct Image *CreateSourceImage(const char *str, unsigned char type);
LEGO_EXPORT int KillImage(struct Image *image);
LEGO_EXPORT void ReloadImageBitmapAndBuildSprites(struct Image *image);
LEGO_EXPORT void FreeBitmapResources(struct Image *image);

LEGO_EXPORT unsigned int GetVRAMAddress(unsigned int address);
LEGO_EXPORT struct Sprite *CreateSprite(struct Image *image);
LEGO_EXPORT struct Sprite *CreateFunctionBasedSprite(unsigned int source, unsigned short a, unsigned short b);
LEGO_EXPORT unsigned int LoadSprite(const char *name, int flags);
LEGO_EXPORT unsigned int MakeSprite(unsigned int sprite);
LEGO_EXPORT short ReferenceSprite(struct Sprite *sprite);
LEGO_EXPORT int KillSprite(unsigned int sprite);

LEGO_EXPORT void HideLayer(void *layer, unsigned int flag);

void FUN_00498120(void);
void FUN_00498630(const char *param_1);
int FUN_004988c0(void);
void FUN_00498920(void);
int FUN_00498b00(void);
void FUN_00498b40(void);
int FUN_00498cf0(void);
void FUN_00498d00(void);
