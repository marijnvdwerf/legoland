#pragma once

#include "legoland.h"

struct Sprite;

struct IconNode {
    /* 0x00 */ struct IconNode *next;
    /* 0x04 */ struct Sprite *sprite;
    /* 0x08 */ unsigned char pad_8[0xc - 0x8];
    /* 0x0c */ short field_c;
    /* 0x0e */ short field_e;
    /* 0x10 */ short field_10;
    /* 0x12 */ short field_12;
    /* 0x14 */ unsigned short field_14;
    /* 0x16 */ unsigned char pad_16[0x28 - 0x16];
    /* 0x28 */ void *field_28;
    /* 0x2c */ void *field_2c;
    /* 0x30 */ void *field_30;
    /* 0x34 */ unsigned int field_34;
    /* 0x38 */ unsigned int field_38;
    /* 0x3c */ unsigned int field_3c;
};

struct SpriteIcon {
    /* 0x00 */ struct SpriteIcon *next;
    /* 0x04 */ struct Sprite *sprite;
    /* 0x08 */ unsigned char pad_8[0xc - 0x8];
    /* 0x0c */ short x;
    /* 0x0e */ short y;
    /* 0x10 */ short field_10;
    /* 0x12 */ short field_12;
    /* 0x14 */ unsigned short id;
    /* 0x16 */ unsigned char pad_16[0x28 - 0x16];
    /* 0x28 */ void *field_28;
    /* 0x2c */ unsigned char (*event_handler)(unsigned int param_1, unsigned int param_2);
    /* 0x30 */ void *field_30;
    /* 0x34 */ unsigned int field_34;
    /* 0x38 */ unsigned int field_38;
    /* 0x3c */ unsigned int field_3c;
};

LEGO_EXPORT void RenderIcons(void);
LEGO_EXPORT void RenderIcons2(short param_1, short param_2, short param_3);
LEGO_EXPORT unsigned char CheckFocussedIcon(void);
LEGO_EXPORT void UpdateFocussedIconPtr(void);
LEGO_EXPORT void RemoveIconGroup(unsigned int group);
void FUN_0046d590(unsigned int val);
void FUN_0046d680(struct IconNode *node, struct Sprite *sprite);
LEGO_EXPORT struct SpriteIcon *LoadSpriteIcon(const char *filename, unsigned int param_2, int param_3, int param_4, int param_5);
void FUN_0046dac0(void);
void FUN_0046db40(void);
int FUN_0046df60(int param);
LEGO_EXPORT void RenderHelpIcons(void);
void FUN_0046fb40(unsigned int group);
