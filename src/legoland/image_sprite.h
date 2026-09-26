#pragma once

#include <ddraw.h>
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
struct LLS;

/* Sub-sprites of a layered sprite (Sprite.group) and their draw offsets. */
struct SpriteGroup {
    /* 0x00 */ unsigned char pad_0[4];
    /* 0x04 */ int count;
    /* 0x08 */ struct Sprite **subs;
    /* 0x0c */ int *xoffs;
    /* 0x10 */ int *yoffs;
};
typedef struct SpriteGroup SpriteGroup;

struct Sprite {
    /* 0x00 */ struct Sprite *next;
    /* 0x04 */ LPDIRECTDRAWSURFACE surface;
    /* 0x08 */ union {
        struct Image *image;
        int (*render_fn)(struct Sprite *);
        struct SpriteGroup *group; /* layered sprites (flags & 0x8000) */
        struct LLS **lls; /* animated (.lls) sprites */
    };
    /* 0x0c */ unsigned int field_c;
    /* 0x10 */ union {
        unsigned int flags;
        struct LayerHost *host;
    };
    /* 0x14 */ short width;
    /* 0x16 */ unsigned short height;
    /* 0x18 */ unsigned short src_x;
    /* 0x1a */ unsigned short src_y;
    /* 0x1c */ unsigned short refcount;
};
typedef struct Sprite Sprite;

LEGO_EXPORT struct Image *CreateSourceImage(const char *str, unsigned char type);
LEGO_EXPORT int KillImage(struct Image *image);
LEGO_EXPORT int ReloadImageBitmapAndBuildSprites(struct Image *image);
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

LEGO_EXPORT void HideLayer(struct Sprite *sprite, unsigned int index);
LEGO_EXPORT void ShowLayer(struct Sprite *sprite, unsigned int index);

/* GetLayer fills the first three fields; callers clear field_10. */
struct LayerResult {
    /* 0x00 */ struct Sprite *sprite;
    /* 0x04 */ int x;
    /* 0x08 */ int y;
    /* 0x0c */ unsigned int field_c;
    /* 0x10 */ unsigned int field_10;
    /* 0x14 */ unsigned int field_14;
};
typedef struct LayerResult LayerResult;
LEGO_EXPORT void GetLayer(struct Sprite *sprite, struct LayerResult *result, int index);
