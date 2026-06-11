#include "legoland.h"
#include <stdio.h>
#include <stdlib.h>

#include "print_sprite.h"
#include "render.h"
#include "globals.h"
#include "image_sprite.h"
#include "timer.h"
#include "draw.h"
#include "clipping.h"
#include "debug_alloc.h"
#include "man3d.h"

struct HitInfo {
    /* 0x00 */ int field_0;
    /* 0x04 */ int field_4;
    /* 0x08 */ int field_8;
};

struct SortNode {
    /* 0x00 */ struct SortNode *left;
    /* 0x04 */ struct SortNode *right;
    /* 0x08 */ int key;
    /* 0x0c */ unsigned int flags;
    /* 0x10 */ struct HitInfo hit;
    /* 0x1c */ struct Sprite *sprite;
    /* 0x20 */ int x;
    /* 0x24 */ int y;
    /* 0x28 */ int field_28;
    /* 0x2c */ int field_2c;
    /* 0x30 */ int field_30;
    /* 0x34 */ int field_34;
    /* 0x38 */ int field_38;
    /* 0x3c */ int field_3c;
    /* 0x40 */ int field_40;
};

struct SpriteGroup {
    /* 0x00 */ unsigned char pad_0[4];
    /* 0x04 */ int count;
    /* 0x08 */ struct Sprite **subs;
    /* 0x0c */ int *xoffs;
    /* 0x10 */ int *yoffs;
};

struct SpriteExArg {
    /* 0x00 */ struct Sprite *sprite;
    /* 0x04 */ int xoff;
    /* 0x08 */ int yoff;
    /* 0x0c */ int field_c;
    /* 0x10 */ unsigned int mode;
    /* 0x14 */ unsigned int mask;
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
void FUN_004855d0(struct Sprite *sprite, int *out)
{
    int xoff;
    int yoff;
    int i;
    int right;
    int bottom;
    struct SpriteGroup *group;
    struct Sprite *sub;

    if ((sprite->flags & 0x8000) == 0) {
        out[0] = 0;
        out[1] = 0;
        if (sprite != NULL) {
            out[2] = (short)sprite->width + -1;
            out[3] = (short)sprite->height + -1;
            return;
        }
        out[2] = 0;
        out[3] = 0;
        return;
    }
    i = 0;
    out[0] = 0x7fffffff;
    out[1] = 0x7fffffff;
    out[2] = -0x80000000;
    out[3] = -0x80000000;
    group = (struct SpriteGroup *)sprite->image;
    if (0 < group->count) {
        do {
            xoff = group->xoffs[i];
            yoff = group->yoffs[i];
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
            sub = group->subs[i];
            right = (short)sub->width + xoff;
            bottom = (short)sub->height + yoff;
            if (xoff < out[0]) {
                out[0] = xoff;
            }
            if (yoff < out[1]) {
                out[1] = yoff;
            }
            if (right > out[2]) {
                out[2] = right;
            }
            if (bottom > out[3]) {
                out[3] = bottom;
            }
            group = (struct SpriteGroup *)sprite->image;
            i = i + 1;
        } while (i < group->count);
    }
}

// FUNCTION: LEGOLAND 0x004856a0
LEGO_EXPORT unsigned int PrintSpriteEx(struct SpriteExArg *arg, int x, int y)
{
    struct Sprite *sprite;
    struct SpriteGroup *group;
    unsigned int mask;
    unsigned int result;
    int i;
    int xoff;
    int yoff;

    result = 1;
    sprite = arg->sprite;
    x = x + arg->xoff / 2;
    y = y + arg->yoff / 2;
    if ((sprite->flags & 0x8000) == 0) {
        if (arg->mode != 0) {
            if (*GetVRAMAddress(sprite) == 0) {
                if (FUN_00499500(sprite) == 0) {
                    return 0;
                }
                return RenderSpriteX(sprite, x, y, arg->mode);
            }
            return RenderSpriteX(sprite, x, y, arg->mode);
        }
        if (*GetVRAMAddress(sprite) == 0 && FUN_00499500(sprite) == 0) {
            return 0;
        }
        return RenderSprite(sprite, x, y);
    }
    mask = arg->mask;
    group = (struct SpriteGroup *)sprite->image;
    i = 0;
    if (group->count <= 0) {
        return result;
    }
    do {
        if ((mask & 1) != 0) {
            xoff = group->xoffs[i];
            yoff = group->yoffs[i];
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
            if (arg->mode != 0) {
                if (*GetVRAMAddress(group->subs[i]) == 0) {
                    if (FUN_00499500(((struct SpriteGroup *)sprite->image)->subs[i]) != 0) {
                        RenderSpriteX(((struct SpriteGroup *)sprite->image)->subs[i], xoff + x, yoff + y, arg->mode);
                    }
                } else {
                    RenderSpriteX(((struct SpriteGroup *)sprite->image)->subs[i], xoff + x, yoff + y, arg->mode);
                }
            } else {
                if (*GetVRAMAddress(group->subs[i]) == 0) {
                    if (FUN_00499500(((struct SpriteGroup *)sprite->image)->subs[i]) == 0) {
                        goto cont;
                    }
                    RenderSprite(((struct SpriteGroup *)sprite->image)->subs[i], xoff + x, yoff + y);
                } else {
                    RenderSprite(((struct SpriteGroup *)sprite->image)->subs[i], xoff + x, yoff + y);
                }
            }
        }
    cont:
        group = (struct SpriteGroup *)sprite->image;
        i = i + 1;
        mask = (int)mask >> 1;
    } while (i < group->count);
    return 1;
}

// FUNCTION: LEGOLAND 0x004858e0
LEGO_EXPORT unsigned int PrintTiledSprite(struct Sprite *sprite, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7)
{
    if ((sprite->flags & 0x8000) == 0) {
        if (*GetVRAMAddress(sprite) != 0 || FUN_00499500(sprite) != 0) {
            return RenderTiledSprite(sprite, param_2, param_3, param_4, param_5, param_6, param_7);
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00485940
LEGO_EXPORT unsigned int PrintScaledSprite(struct Sprite *sprite, int param_2, int param_3, int param_4, int param_5)
{
    if (*GetVRAMAddress(sprite) == 0) {
        if (FUN_00499500(sprite) != 0) {
            return RenderScaledSprite(sprite, param_2, param_3, param_4, param_5);
        }
        return 0;
    }
    return RenderScaledSprite(sprite, param_2, param_3, param_4, param_5);
}

// FUNCTION: LEGOLAND 0x004859b0
LEGO_EXPORT void ClearPrintList(void)
{
    struct SortNode *node = DAT_0066b5a4;

    while (node != NULL) {
        node = node->right;
    }

    DAT_0066b5a8 = 0;
    DAT_0066b5a4 = NULL;
}

// FUNCTION: LEGOLAND 0x004859d0
LEGO_EXPORT void DrawAndClearPrintList(void)
{
    struct SortNode *node;
    struct SortNode *node_save;
    unsigned int saved_pal;
    unsigned int saved_frame;
    void *obj;
    RECT saved_clip;

    node = DAT_0066b5a4;
    saved_pal = GetOverridePalette();
    saved_frame = GetOverrideFrame();
    DAT_0066b5ac = 0;
    for (; node != NULL; node = node->right) {
        if ((node->flags & 1) != 0) {
            if (node->field_28 < 0 || node->field_30 > (int)(unsigned int)lpConfig->field_0 || node->field_2c < 0 || node->field_34 > (int)(unsigned int)lpConfig->field_2) {
                // STRING: LEGOLAND 0x004bdd0c
                printf("error");
            }
            GetClipping((struct ClipRect *)&saved_clip);
            SetClipping(&node->field_28);
            if (node->sprite != NULL) {
                SetOverridePalette(node->field_3c);
                SetOverrideFrame(node->field_40);
                PrintSprite(node->sprite, node->x, node->y, node->field_38, (int *)&node->hit);
                ClearSpriteOverrides();
                if (node->hit.field_0 == 0x103 && node->hit.field_4 != 0 && (((struct Sprite *)node->sprite)->flags & 0x2000) != 0) {
                    obj = (void *)node->hit.field_4;
                    (*(void (**)(void *, int, int, int *, int *, int))((char *)*(void **)((char *)obj + 0xc) + 0xb0))(obj, node->x, node->y, &node->hit.field_8, &node->field_28, node->field_38);
                }
            }
            if ((node->flags & 4) != 0) {
                (*(void (**)(int))&node_save->field_34)(node_save->field_38);
            }
            SetClipping((int *)&saved_clip);
        } else if ((node->flags & 0x2000) != 0) {
            Render3DPerson((struct Person *)node->sprite);
        } else {
            node_save = node;
            if (node->sprite != NULL) {
                SetOverridePalette(node->field_2c);
                SetOverrideFrame(node->field_30);
                PrintSprite(node->sprite, node->x, node->y, node->field_28, (int *)&node->hit);
                ClearSpriteOverrides();
                if (node->hit.field_0 == 0x103 && node->hit.field_4 != 0 && (((struct Sprite *)node->sprite)->flags & 0x2000) != 0) {
                    obj = (void *)node->hit.field_4;
                    (*(void (**)(void *, int, int, int *, int, int))((char *)*(void **)((char *)obj + 0xc) + 0xb0))(obj, node->x, node->y, &node->hit.field_8, 0, node->field_28);
                }
                if ((node->flags & 4) != 0) {
                    (*(void (**)(int))&node->field_34)(node->field_38);
                }
            }
        }
    }
    DAT_0066b5a8 = 0;
    DAT_0066b5a4 = NULL;
    SetOverridePalette(saved_pal);
    SetOverrideFrame(saved_frame);
}

// FUNCTION: LEGOLAND 0x00485bd0
void FUN_00485bd0(struct SortNode *node)
{
    int key;

    if (DAT_007fd600 == NULL) {
        // STRING: LEGOLAND 0x004bdd40
        DBPrintf("Oh drat, Bad stuff in the sprite sorter\n");
    }
    if (node == NULL) {
        // STRING: LEGOLAND 0x004bdd14
        DBPrintf("Oh no, Not enough RAM for sprite sort list\n");
    }
    if (DAT_0066b5a4 == NULL) {
        node->left = NULL;
        node->right = NULL;
        DAT_0066b5a4 = node;
    } else {
        key = node->key;
        if (key < DAT_007fd600->key) {
            while (DAT_007fd600->left != NULL) {
                DAT_007fd600 = DAT_007fd600->left;
                if (DAT_007fd600->key <= key) break;
            }
        } else if (DAT_007fd600->key < key) {
            while (DAT_007fd600->right != NULL) {
                DAT_007fd600 = DAT_007fd600->right;
                if (key <= DAT_007fd600->key) break;
            }
        }
        if (key < DAT_007fd600->key) {
            node->left = DAT_007fd600->left;
            if (DAT_007fd600->left == NULL) {
                DAT_0066b5a4 = node;
            } else {
                DAT_007fd600->left->right = node;
            }
            node->right = DAT_007fd600;
            DAT_007fd600->left = node;
        } else {
            node->right = DAT_007fd600->right;
            if (DAT_007fd600->right != NULL) {
                DAT_007fd600->right->left = node;
            }
            node->left = DAT_007fd600;
            DAT_007fd600->right = node;
        }
    }
    DAT_007fd600 = node;
    if (node == NULL) {
        DBPrintf("Oh drat, Bad stuff in the sprite sorter\n");
    }
}

// FUNCTION: LEGOLAND 0x00485cd0
LEGO_EXPORT void SortSpriteWithCallback(struct Sprite *sprite, unsigned int x, unsigned int y, int key, unsigned int param_5, unsigned int param_6, unsigned int param_7, struct HitInfo *hit)
{
    unsigned int original = DAT_0066b5a8;
    struct SortNode *node;

    DAT_0066b5a8 += 0x3c;
    node = (struct SortNode *)&DAT_007cb600[original];
    node->key = key;
    node->sprite = sprite;
    node->x = x;
    node->y = y;
    node->flags = 4;
    node->field_28 = param_5;
    node->field_34 = param_6;
    node->field_38 = param_7;
    node->field_2c = GetOverridePalette();
    node->field_30 = GetOverrideFrame();
    if (hit != NULL) {
        node->hit = *hit;
        FUN_00485bd0(node);
        return;
    }
    node->hit.field_0 = 0x100;
    FUN_00485bd0(node);
}

// FUNCTION: LEGOLAND 0x00485d70
LEGO_EXPORT void SortSprite(struct Sprite *sprite, unsigned int x, unsigned int y, int key, unsigned int param_5, struct HitInfo *hit)
{
    unsigned int original = DAT_0066b5a8;
    struct SortNode *node;

    DAT_0066b5a8 += 0x3c;
    node = (struct SortNode *)&DAT_007cb600[original];
    node->key = key;
    node->sprite = sprite;
    node->flags = 0;
    node->x = x;
    node->y = y;
    node->field_28 = param_5;
    node->field_2c = GetOverridePalette();
    node->field_30 = GetOverrideFrame();
    if (hit != NULL) {
        node->hit = *hit;
        FUN_00485bd0(node);
        return;
    }
    node->hit.field_0 = 0x100;
    FUN_00485bd0(node);
}

// FUNCTION: LEGOLAND 0x00485e00
LEGO_EXPORT void SortPerson(struct Person *person, unsigned int param_2, void *param_3)
{
    unsigned int original = DAT_0066b5a8;
    struct SortNode *block;

    DAT_0066b5a8 += 0x20;
    block = (struct SortNode *)&DAT_007cb600[original];
    block->key = param_2;
    block->flags = 0x2000;
    block->sprite = (struct Sprite *)person;
    FUN_00485bd0(block);
}

// FUNCTION: LEGOLAND 0x00485e40
LEGO_EXPORT void SortClippedSprite(struct Sprite *sprite, unsigned int x, unsigned int y, int key, RECT *clip, unsigned int param_6, struct HitInfo *hit)
{
    unsigned int original = DAT_0066b5a8;
    struct SortNode *node;

    DAT_0066b5a8 += 0x4c;
    node = (struct SortNode *)&DAT_007cb600[original];
    node->key = key;
    node->sprite = sprite;
    node->x = x;
    node->flags = 1;
    node->y = y;
    *(RECT *)&node->field_28 = *clip;
    node->field_38 = param_6;
    node->field_3c = GetOverridePalette();
    node->field_40 = GetOverrideFrame();
    if (hit != NULL) {
        node->hit = *hit;
        FUN_00485bd0(node);
        return;
    }
    node->hit.field_0 = 0x100;
    FUN_00485bd0(node);
}

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
void FUN_00485fe0(struct Sprite *sprite, int x, int y)
{
    RECT rect1;
    RECT rect2;
    int offsets[2];
    unsigned int *p;
    int count;

    p = (unsigned int *)DAT_00701e5c;
    for (count = DAT_0066be40 * DAT_0066be44; count != 0; count = count + -1) {
        *p = 0;
        p = p + 1;
    }
    if (sprite == NULL) {
        return;
    }
    rect1.bottom = (short)sprite->height;
    rect1.left = 0;
    rect1.top = 0;
    rect2.right = DAT_0066be40 + x;
    rect1.right = (short)sprite->width;
    rect2.bottom = DAT_0066be44 + y;
    rect2.left = x;
    rect2.top = y;
    if (IntersectRect(&rect1, &rect1, &rect2) == 0) {
        return;
    }
    if (x < 0) {
        offsets[0] = -x;
        rect1.left = 0;
    } else {
        offsets[0] = 0;
    }
    if (y < 0) {
        offsets[1] = -y;
        rect1.top = 0;
    } else {
        offsets[1] = 0;
    }
    ZBufferHelper((unsigned int *)sprite->image->data, (int *)&rect1, offsets, DAT_00701e5c);
}
