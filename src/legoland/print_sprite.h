#pragma once

#include <windows.h>

#include "legoland.h"

struct Person;
struct Sprite;
struct HitInfo;
struct SpriteExArg;
struct IconNode;

struct PrintCtx {
    /* 0x00 */ unsigned int flags;
    /* 0x04 */ struct IconNode *node;
    /* 0x08 */ unsigned int field_8;
};

LEGO_EXPORT void ResetHitInfo(void);
LEGO_EXPORT unsigned int PrintSprite(struct Sprite *sprite, unsigned int x, unsigned int y, unsigned int param_4, int *param_5);
LEGO_EXPORT unsigned int PrintSpriteEx(struct SpriteExArg *arg, int x, int y);
LEGO_EXPORT unsigned int PrintTiledSprite(struct Sprite *sprite, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7);
LEGO_EXPORT unsigned int PrintScaledSprite(struct Sprite *sprite, int param_2, int param_3, int param_4, int param_5);
LEGO_EXPORT void ClearPrintList(void);
LEGO_EXPORT void DrawAndClearPrintList(void);
LEGO_EXPORT void SortSpriteWithCallback(struct Sprite *sprite, unsigned int x, unsigned int y, int key, unsigned int param_5, unsigned int param_6, unsigned int param_7, struct HitInfo *hit);
LEGO_EXPORT void SortSprite(struct Sprite *sprite, unsigned int x, unsigned int y, int key, unsigned int param_5, struct HitInfo *hit);
LEGO_EXPORT void SortPerson(struct Person *person, unsigned int param_2, void *param_3);
LEGO_EXPORT void SortClippedSprite(struct Sprite *sprite, unsigned int x, unsigned int y, int key, RECT *clip, unsigned int param_6, struct HitInfo *hit);
void FUN_00485f00(struct Sprite *param_1, unsigned int param_2, unsigned int param_3);
void FUN_00485f20(void *ptr);
void FUN_00485f30(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);
void FUN_00485fa0(void);
void FUN_00485fc0(unsigned int param_1);
void FUN_00485fe0(struct Sprite *sprite, int x, int y);
