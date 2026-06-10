#pragma once

#include "legoland.h"

struct Sprite;

LEGO_EXPORT void RenderThickBox(int x, int y, int w, int h, int thickness, unsigned int color);
LEGO_EXPORT unsigned int RenderBlock(int x, int y, int w, int h, unsigned int color);
LEGO_EXPORT unsigned int RenderSprite(struct Sprite *sprite, int x, int y);
LEGO_EXPORT unsigned int RenderSpriteX(struct Sprite *sprite, int x, int y, unsigned int param_4);
void FUN_004860f0(void);
void FUN_00486250(void);
void FUN_00486540(void);
void FUN_004886a0(void);
