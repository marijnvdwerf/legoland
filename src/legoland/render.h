#pragma once

#include "legoland.h"

struct Sprite;
struct RenderViewport;
struct Image;

struct TextureNode {
    /* 0x00 */ int format_w;
    /* 0x04 */ int format_h;
    /* 0x08 */ unsigned char *data_8;
    /* 0x0c */ void **data_c; /* colour ramps (GetOrCreateColorRamp) */
    /* 0x10 */ int width_m1;
    /* 0x14 */ int height_m1;
    /* 0x18 */ unsigned char pad_18[0x2c - 0x18];
};

LEGO_EXPORT void RenderThickBox(int x, int y, int w, int h, int thickness, unsigned int color);
LEGO_EXPORT void RenderBox(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5);
LEGO_EXPORT unsigned int RenderBlock(int x, int y, int w, int h, unsigned int color);
LEGO_EXPORT unsigned int RenderSprite(struct Sprite *sprite, int x, int y);
LEGO_EXPORT unsigned int RenderSpriteX(struct Sprite *sprite, int x, int y, unsigned int param_4);
LEGO_EXPORT unsigned int RenderTiledSprite(struct Sprite *sprite, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7);
LEGO_EXPORT unsigned int RenderScaledSprite(struct Sprite *param_1, int param_2, int param_3, int param_4, int param_5);
void InitPackedColorLookupTable(void);
void FreeCursorCache(void);
void *GetOrCreateColorRamp(int param_1, void *param_2);
void StartMousePixelHitTest(unsigned char *base, struct RenderViewport *vp);
int InitReciprocalTables(void);
void LoadTextureNode(struct Image *image, unsigned int index);
void FreeTextureNodes(void);
void SetCurrentColorRamp(void *param_1);
void DrawGouraudRampTriangle(void);
void DrawGouraudTexturedTriangle(void);
void DrawFlatRampTriangle(void);
void DrawFlatTexturedTriangle(void);
void SetCurrentTextureByIndex(unsigned int index);
