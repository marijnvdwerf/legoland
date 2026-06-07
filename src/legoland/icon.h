#pragma once

#include "legoland.h"

struct Sprite;
struct SpriteIcon;
struct IconNode;

LEGO_EXPORT void RemoveIconGroup(unsigned int group);
void FUN_0046d590(unsigned int val);
void FUN_0046d680(struct IconNode *node, struct Sprite *sprite);
LEGO_EXPORT struct SpriteIcon *LoadSpriteIcon(const char *filename, unsigned int param_2, unsigned int param_3, unsigned int param_4, unsigned int param_5);
void FUN_0046dac0(void);
void FUN_0046db40(void);
int FUN_0046df60(void);
LEGO_EXPORT void RenderHelpIcons(void);
void FUN_0046fb40(unsigned int group);
