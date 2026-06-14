#pragma once

#include "legoland.h"

struct Sprite;
struct RenderViewport;
struct Image;

struct TextureNode {
    /* 0x00 */ int format_w;
    /* 0x04 */ int format_h;
    /* 0x08 */ unsigned char *data_8;
    /* 0x0c */ unsigned int *data_c;
    /* 0x10 */ int width_m1;
    /* 0x14 */ int height_m1;
    /* 0x18 */ unsigned char pad_18[0x2c - 0x18];
};

LEGO_EXPORT void RenderThickBox(int x, int y, int w, int h, int thickness, unsigned int color);
LEGO_EXPORT unsigned int RenderBlock(int x, int y, int w, int h, unsigned int color);
LEGO_EXPORT unsigned int RenderSprite(struct Sprite *sprite, int x, int y);
LEGO_EXPORT unsigned int RenderSpriteX(struct Sprite *sprite, int x, int y, unsigned int param_4);
LEGO_EXPORT unsigned int RenderTiledSprite(struct Sprite *sprite, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7);
LEGO_EXPORT unsigned int RenderScaledSprite(struct Sprite *param_1, int param_2, int param_3, int param_4, int param_5);
void FUN_004860f0(void);
void FUN_00486250(void);
unsigned int FUN_00486280(int param_1, void *param_2);
void FUN_00488700(unsigned int base, struct RenderViewport *vp);
void FUN_00486540(void);
void FUN_00488670(struct Image *image, unsigned int index);
void FUN_004886a0(void);
