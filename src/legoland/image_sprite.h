#pragma once

#include "legoland.h"

/* Shared layouts: struct Sprite and struct Image are defined once here and
 * #included by every TU that touches them (image_sprite.c, gfx.c, mapscreen.c,
 * ride_bloke.c, timer.c, ...). */
struct Image {
    /* 0x00 */ void *data;
    /* 0x04 */ void *aux;
    /* 0x08 */ short width;
    /* 0x0a */ short height;
    /* 0x0c */ unsigned short refcount;
    /* 0x0e */ unsigned char type;
    /* 0x0f */ unsigned char pad_f[0x10 - 0xf];
    /* 0x10 */ char *name;
    /* 0x14 */ unsigned int field_14;
};

struct LayerHost;

struct Sprite {
    /* 0x00 */ struct Sprite *next;
    /* 0x04 */ void *surface;
    /* 0x08 */ union {
        struct Image *image;
        int (*render_fn)(struct Sprite *);
    };
    /* 0x0c */ unsigned int field_c;
    /* 0x10 */ union {
        unsigned int flags;
        struct LayerHost *host;
    };
    /* 0x14 */ unsigned short width;
    /* 0x16 */ unsigned short height;
    /* 0x18 */ unsigned short src_x;
    /* 0x1a */ unsigned short src_y;
    /* 0x1c */ unsigned short refcount;
};

LEGO_EXPORT struct Image *CreateSourceImage(const char *str, unsigned char type);
LEGO_EXPORT int KillImage(struct Image *image);
LEGO_EXPORT void ReloadImageBitmapAndBuildSprites(struct Image *image);
LEGO_EXPORT void FreeBitmapResources(struct Image *image);

LEGO_EXPORT void **GetVRAMAddress(struct Sprite *sprite);
LEGO_EXPORT struct Sprite *CreateSprite(struct Image *image);
LEGO_EXPORT struct Sprite *CreateFunctionBasedSprite(int (*source)(struct Sprite *), unsigned short a, unsigned short b);
LEGO_EXPORT struct Sprite *LoadSprite(const char *name, int flags);
LEGO_EXPORT unsigned int MakeSprite(struct Sprite *sprite);
LEGO_EXPORT short ReferenceSprite(struct Sprite *sprite);
LEGO_EXPORT int KillSprite(struct Sprite *sprite);
LEGO_EXPORT int GetSprite(unsigned int *param_1, struct Sprite *param_2);
LEGO_EXPORT void ReleaseSprite(struct Sprite *sprite);

struct LayerOwner;
LEGO_EXPORT void HideLayer(struct LayerOwner *owner, unsigned int index);
LEGO_EXPORT void ShowLayer(struct LayerOwner *owner, unsigned int index);

