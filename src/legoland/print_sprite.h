#pragma once

#include <windows.h>

#include "legoland.h"

struct Person;
struct Sprite;
struct Element;
struct HitInfo {
    int type;
    union {
        int field_4;
        struct Element *element;
        void *ptr; /* what is under the cursor (an advisor, ...) */
    };
    union {
        int field_8;
        unsigned short coords;
    };
};
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
LEGO_EXPORT void SortSpriteWithCallback(struct Sprite *sprite, unsigned int x, unsigned int y, int key, unsigned int param_5, void (*param_6)(void *arg), void *param_7, struct HitInfo *hit);
LEGO_EXPORT void SortSprite(struct Sprite *sprite, unsigned int x, unsigned int y, int key, unsigned int param_5, struct HitInfo *hit);
LEGO_EXPORT void SortPerson(struct Person *person, unsigned int param_2, void *param_3);
LEGO_EXPORT void SortClippedSprite(struct Sprite *sprite, unsigned int x, unsigned int y, int key, RECT *clip, unsigned int param_6, struct HitInfo *hit);
void PrintSpriteSimple(struct Sprite *param_1, unsigned int param_2, unsigned int param_3);
void SetCurrentTexture(void *ptr);
void SetRender3DTarget(unsigned char *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);
void FreeZBuffer(void);
void InitRender3D(unsigned int param_1);
void LoadZBufferFromSprite(struct Sprite *sprite, int x, int y);
