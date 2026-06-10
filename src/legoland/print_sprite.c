#include "legoland.h"
#include "crt.h"

#include "print_sprite.h"
#include "render.h"
#include "globals.h"
#include "image_sprite.h"
#include "timer.h"

struct PrintListNode {
    unsigned int field_0;
    struct PrintListNode *next;
};

struct PersonBlock {
    unsigned char pad_0[8];
    unsigned int field_8;
    unsigned int field_c;
    unsigned char pad_10[12];
    unsigned int field_1c;
};


struct SpriteGroup {
    /* 0x00 */ unsigned char pad_0[4];
    /* 0x04 */ int count;
    /* 0x08 */ struct Sprite **subs;
    /* 0x0c */ int *xoffs;
    /* 0x10 */ int *yoffs;
};

// FUNCTION: LEGOLAND 0x004853a0
LEGO_EXPORT unsigned int PrintSprite(struct Sprite *sprite, unsigned int x, unsigned int y, unsigned int param_4, int *param_5)
{
    int i;
    int xoff;
    int yoff;
    unsigned int result;

    result = 1;
    DAT_007feb14 = 0;
    if ((sprite->flags & 0x8000) != 0) {
        i = 0;
        if (((struct SpriteGroup *)sprite->image)->count > 0) {
            do {
                if ((((struct SpriteGroup *)sprite->image)->subs[i]->flags & 0x4000) == 0) {
                    xoff = ((struct SpriteGroup *)sprite->image)->xoffs[i];
                    yoff = ((struct SpriteGroup *)sprite->image)->yoffs[i];
                    if (xoff < 0) {
                        xoff = -(-xoff >> 1);
                    } else {
                        xoff = xoff >> 1;
                    }
                    if (yoff < 0) {
                        yoff = -(-yoff >> 1);
                    } else {
                        yoff = yoff >> 1;
                    }
                    if (param_4 == 0) {
                        if (*GetVRAMAddress(((struct SpriteGroup *)sprite->image)->subs[i]) == 0) {
                            if (FUN_00499500(((struct SpriteGroup *)sprite->image)->subs[i]) == 0) {
                                goto cont;
                            }
                        }
                        RenderSprite(((struct SpriteGroup *)sprite->image)->subs[i], xoff + x, yoff + y);
                    } else {
                        if (*GetVRAMAddress(((struct SpriteGroup *)sprite->image)->subs[i]) != 0 || FUN_00499500(((struct SpriteGroup *)sprite->image)->subs[i]) != 0) {
                            RenderSpriteX(((struct SpriteGroup *)sprite->image)->subs[i], xoff + x, yoff + y, param_4);
                        }
                    }
                }
            cont:
                i = i + 1;
            } while (i < ((struct SpriteGroup *)sprite->image)->count);
        }
        goto writeback;
    }
    if (param_4 == 0) {
        if (*GetVRAMAddress(sprite) != 0) {
            result = RenderSprite(sprite, x, y);
            goto writeback;
        }
        if (FUN_00499500(sprite) != 0) {
            result = RenderSprite(sprite, x, y);
            goto writeback;
        }
    } else {
        if (*GetVRAMAddress(sprite) != 0) {
            result = RenderSpriteX(sprite, x, y, param_4);
            goto writeback;
        }
        if (FUN_00499500(sprite) != 0) {
            result = RenderSpriteX(sprite, x, y, param_4);
            goto writeback;
        }
    }
    result = 0;
writeback:
    if (param_5 != NULL && DAT_007feb14 != 0 && *param_5 != 0x100) {
        DAT_004bdd00 = *param_5;
        DAT_004bdd04 = param_5[1];
        DAT_004bdd08 = param_5[2];
    }
    return result;
}

// FUNCTION: LEGOLAND 0x004855d0
void FUN_004855d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004856a0
LEGO_EXPORT void PrintSpriteEx(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004858e0
LEGO_EXPORT void PrintTiledSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00485940
LEGO_EXPORT void PrintScaledSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004859b0
LEGO_EXPORT void ClearPrintList(void)
{
    struct PrintListNode *node = (struct PrintListNode *)DAT_0066b5a4;

    while (node != NULL) {
        node = node->next;
    }

    DAT_0066b5a8 = 0;
    DAT_0066b5a4 = NULL;
}

// FUNCTION: LEGOLAND 0x004859d0
LEGO_EXPORT void DrawAndClearPrintList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00485bd0
void FUN_00485bd0(void *ctx) { STUB(); }

// FUNCTION: LEGOLAND 0x00485cd0
LEGO_EXPORT void SortSpriteWithCallback(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00485d70
LEGO_EXPORT void SortSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00485e00
LEGO_EXPORT void SortPerson(struct Person *person, unsigned int param_2, void *param_3)
{
    unsigned int original = DAT_0066b5a8;
    struct PersonBlock *block;

    DAT_0066b5a8 += 0x20;
    block = (struct PersonBlock *)&DAT_007cb600[original];
    block->field_8 = param_2;
    block->field_c = 0x2000;
    block->field_1c = (unsigned int)person;
    FUN_00485bd0(block);
}

// FUNCTION: LEGOLAND 0x00485e40
LEGO_EXPORT void SortClippedSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00485ef0
LEGO_EXPORT void ResetHitInfo(void)
{
    DAT_004bdd00 = 0x100;
}

// FUNCTION: LEGOLAND 0x00485f00
void FUN_00485f00(struct Sprite *param_1, unsigned int param_2, unsigned int param_3)
{
    PrintSprite(param_1, param_2, param_3, 0, 0);
}

// FUNCTION: LEGOLAND 0x00485f20
void FUN_00485f20(void *ptr)
{
    DAT_0066b630 = ptr;
}

// FUNCTION: LEGOLAND 0x00485f30
void FUN_00485f30(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4)
{
    DAT_00797e68 = param_1;
    DAT_00701e58 = param_2;
    DAT_00701e60 = param_3;
    DAT_0066be48 = param_4;
}

// FUNCTION: LEGOLAND 0x00485f60
void FUN_00485f60(void)
{
    DAT_0066be40 = 0x80;
    DAT_0066be44 = 0x78;
    DAT_00701e5c = malloc(0xf000);
    DAT_0066be4c = DAT_0066be40 * 4;
}

// FUNCTION: LEGOLAND 0x00485fa0
void FUN_00485fa0(void)
{
    if (DAT_00701e5c != NULL) {
        free(DAT_00701e5c);
    }
}

// FUNCTION: LEGOLAND 0x00485fc0
void FUN_00485fc0(unsigned int param_1)
{
    *(unsigned int *)&DAT_007cb5e0 = param_1;
    FUN_004860f0();
    FUN_00485f60();
    FUN_00486540();
}

// FUNCTION: LEGOLAND 0x00485fe0
void FUN_00485fe0(void) { STUB(); }
