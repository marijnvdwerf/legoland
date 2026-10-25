#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "legoland.h"

#include "clipping.h"
#include "debug_alloc.h"
#include "draw.h"
#include "globals.h"
#include "image_sprite.h"
#include "man3d.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render.h"
#include "timer.h"

struct SortNode {
    /* 0x00 */ struct SortNode *left;
    /* 0x04 */ struct SortNode *right;
    /* 0x08 */ int key;
    /* 0x0c */ unsigned int flags;
    /* 0x10 */ struct HitInfo hit;
    /* 0x1c */ struct Sprite *sprite;
    /* 0x20 */ int x;
    /* 0x24 */ int y;
    union {
        /* SortSprite / SortSpriteWithCallback */
        struct {
            /* 0x28 */ int mode;
            /* 0x2c */ unsigned short *palette;
            /* 0x30 */ int frame;
            /* 0x34 */ void (*callback)(void *arg); /* run after the sprite is drawn (flags & 4) */
            /* 0x38 */ void *callback_arg;
        } plain;
        /* SortClippedSprite (flags & 1) */
        struct {
            /* 0x28 */ RECT clip;
            /* 0x38 */ int mode;
            /* 0x3c */ unsigned short *palette;
            /* 0x40 */ int frame;
        } clipped;
    };
};

/* the print list pool's bytes per node kind (0x3c, 0x4c and 0x20 on x86) */
#define SORT_NODE_PLAIN (offsetof(struct SortNode, plain) + sizeof(((struct SortNode *)0)->plain))
#define SORT_NODE_CLIPPED (sizeof(struct SortNode) + 8)
#define SORT_NODE_PERSON offsetof(struct SortNode, x)

struct SpriteExArg {
    /* 0x00 */ struct Sprite *sprite;
    /* 0x04 */ int xoff;
    /* 0x08 */ int yoff;
    /* 0x0c */ int field_c;
    /* 0x10 */ unsigned int mode;
    /* 0x14 */ unsigned int mask;
};

// FUNCTION: LEGOLAND 0x004853a0
LEGO_EXPORT unsigned int PrintSprite(struct Sprite *sprite, unsigned int x, unsigned int y, unsigned int param_4, int *param_5) {
    int i;
    int xoff;
    int yoff;
    unsigned int result;

    result = 1;
    MousePixelCovered = 0;
    if ((sprite->flags & 0x8000) == 0) {
        if (param_4 != 0) {
            if (*GetVRAMAddress(sprite) == 0) {
                if (EnsureSpriteSurface(sprite) != 0) {
                    result = RenderSpriteX(sprite, x, y, param_4);
                } else {
                    result = 0;
                }
            } else {
                result = RenderSpriteX(sprite, x, y, param_4);
            }
        } else {
            if (*GetVRAMAddress(sprite) == 0) {
                if (EnsureSpriteSurface(sprite) != 0) {
                    result = RenderSprite(sprite, x, y);
                } else {
                    result = 0;
                }
            } else {
                result = RenderSprite(sprite, x, y);
            }
        }
    } else {
        i = 0;
        if (sprite->group->count > 0) {
            do {
                if ((sprite->group->subs[i]->flags & 0x4000) == 0) {
                    xoff = sprite->group->xoffs[i];
                    yoff = sprite->group->yoffs[i];
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
                    if (param_4 != 0) {
                        if (*GetVRAMAddress(sprite->group->subs[i]) == 0) {
                            if (EnsureSpriteSurface(sprite->group->subs[i]) != 0) {
                                RenderSpriteX(sprite->group->subs[i], xoff + x, yoff + y, param_4);
                            }
                        } else {
                            RenderSpriteX(sprite->group->subs[i], xoff + x, yoff + y, param_4);
                        }
                    } else {
                        if (*GetVRAMAddress(sprite->group->subs[i]) == 0) {
                            if (EnsureSpriteSurface(sprite->group->subs[i]) != 0) {
                                RenderSprite(sprite->group->subs[i], xoff + x, yoff + y);
                            }
                        } else {
                            RenderSprite(sprite->group->subs[i], xoff + x, yoff + y);
                        }
                    }
                }
                i = i + 1;
            } while (i < sprite->group->count);
        }
    }
    if (param_5 != NULL && MousePixelCovered != 0 && *param_5 != 0x100) {
        Hover = *(struct HoverInfo *)param_5;
    }
    return result;
}

/* Bounding rectangle {x0, y0, x1, y1} of a sprite: (0, 0, w-1, h-1) for a plain sprite, the union of the parts
 * (with their offsets) for a sprite group (flags & 0x8000). */
// FUNCTION: LEGOLAND 0x004855d0
void GetSpriteExtent(struct Sprite *sprite, int *out) {
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
    group = sprite->group;
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
            group = sprite->group;
            i = i + 1;
        } while (i < group->count);
    }
}

// Draws a sprite at (x, y) shifted by half of arg's offset. A plain sprite is drawn with
// RenderSprite, or RenderSpriteX when arg->mode is set, after making sure it has a surface; a
// sprite group (flags 0x8000) draws each sub-sprite whose bit is set in arg->mask, at half its
// group offset (rounded towards zero). Returns 0 only if a plain sprite has no surface.
// FUNCTION: LEGOLAND 0x004856a0
LEGO_EXPORT unsigned int PrintSpriteEx(struct SpriteExArg *arg, int x, int y) {
    struct Sprite *sprite;
    struct SpriteGroup *group;
    int mask;
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
                if (EnsureSpriteSurface(sprite) != 0) {
                    return RenderSpriteX(sprite, x, y, arg->mode);
                }
            } else {
                return RenderSpriteX(sprite, x, y, arg->mode);
            }
        } else {
            if (*GetVRAMAddress(sprite) == 0) {
                if (EnsureSpriteSurface(sprite) != 0) {
                    return RenderSprite(sprite, x, y);
                }
            } else {
                return RenderSprite(sprite, x, y);
            }
        }
        return 0;
    }
    mask = arg->mask;
    group = sprite->group;
    if (group->count <= 0) {
        return result;
    }
    i = 0;
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
                    if (EnsureSpriteSurface(sprite->group->subs[i]) != 0) {
                        RenderSpriteX(sprite->group->subs[i], xoff + x, yoff + y, arg->mode);
                    }
                } else {
                    RenderSpriteX(sprite->group->subs[i], xoff + x, yoff + y, arg->mode);
                }
            } else {
                if (*GetVRAMAddress(group->subs[i]) == 0) {
                    if (EnsureSpriteSurface(sprite->group->subs[i]) != 0) {
                        RenderSprite(sprite->group->subs[i], xoff + x, yoff + y);
                    }
                } else {
                    RenderSprite(sprite->group->subs[i], xoff + x, yoff + y);
                }
            }
        }
        group = sprite->group;
        i++;
        mask >>= 1;
    } while (i < group->count);
    return 1;
}

// FUNCTION: LEGOLAND 0x004858e0
LEGO_EXPORT unsigned int PrintTiledSprite(struct Sprite *sprite, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7) {
    if ((sprite->flags & 0x8000) == 0) {
        if (*GetVRAMAddress(sprite) != 0 || EnsureSpriteSurface(sprite) != 0) {
            return RenderTiledSprite(sprite, param_2, param_3, param_4, param_5, param_6, param_7);
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00485940
LEGO_EXPORT unsigned int PrintScaledSprite(struct Sprite *sprite, int param_2, int param_3, int param_4, int param_5) {
    if (*GetVRAMAddress(sprite) == 0) {
        if (EnsureSpriteSurface(sprite) != 0) {
            return RenderScaledSprite(sprite, param_2, param_3, param_4, param_5);
        }
        return 0;
    }
    return RenderScaledSprite(sprite, param_2, param_3, param_4, param_5);
}

// FUNCTION: LEGOLAND 0x004859b0
LEGO_EXPORT void ClearPrintList(void) {
    struct SortNode *node = PrintListHead;

    while (node != NULL) {
        node = node->right;
    }

    PrintListBytesUsed = 0;
    PrintListHead = NULL;
}

// FUNCTION: LEGOLAND 0x004859d0
LEGO_EXPORT void DrawAndClearPrintList(void) {
    struct SortNode *node;
    struct SortNode *node_save;
    unsigned short *saved_pal;
    unsigned int saved_frame;
    void *obj;
    RECT saved_clip;

    node = PrintListHead;
    saved_pal = GetOverridePalette();
    saved_frame = GetOverrideFrame();
    UnusedPrintListCounter = 0;
    for (; node != NULL; node = node->right) {
        if ((node->flags & 1) != 0) {
            if (node->clipped.clip.left < 0 || node->clipped.clip.right > (int)(unsigned int)lpConfig->screen_width || node->clipped.clip.top < 0 || node->clipped.clip.bottom > (int)(unsigned int)lpConfig->screen_height) {
                // STRING: LEGOLAND 0x004bdd0c
                printf("error");
            }
            GetClipping(&saved_clip);
            SetClipping(&node->clipped.clip);
            if (node->sprite != NULL) {
                SetOverridePalette(node->clipped.palette);
                SetOverrideFrame(node->clipped.frame);
                PrintSprite(node->sprite, node->x, node->y, node->clipped.mode, (int *)&node->hit);
                ClearSpriteOverrides();
                if (node->hit.type == 0x103 && node->hit.field_4 != 0 && (((struct Sprite *)node->sprite)->flags & 0x2000) != 0) {
                    obj = node->hit.ptr;
                    ((struct Element *)obj)->ride->cb_b0(obj, node->x, node->y, &node->hit.field_8, &node->clipped.clip, node->clipped.mode);
                }
            }
            if ((node->flags & 4) != 0) {
                node_save->plain.callback(node_save->plain.callback_arg);
            }
            SetClipping(&saved_clip);
        } else if ((node->flags & 0x2000) != 0) {
            Render3DPerson((struct Person *)node->sprite);
        } else {
            node_save = node;
            if (node->sprite != NULL) {
                SetOverridePalette(node->plain.palette);
                SetOverrideFrame(node->plain.frame);
                PrintSprite(node->sprite, node->x, node->y, node->plain.mode, (int *)&node->hit);
                ClearSpriteOverrides();
                if (node->hit.type == 0x103 && node->hit.field_4 != 0 && (((struct Sprite *)node->sprite)->flags & 0x2000) != 0) {
                    obj = node->hit.ptr;
                    ((struct Element *)obj)->ride->cb_b0(obj, node->x, node->y, &node->hit.field_8, 0, node->plain.mode);
                }
                if ((node->flags & 4) != 0) {
                    node->plain.callback(node->plain.callback_arg);
                }
            }
        }
    }
    PrintListBytesUsed = 0;
    PrintListHead = NULL;
    SetOverridePalette(saved_pal);
    SetOverrideFrame(saved_frame);
}

// FUNCTION: LEGOLAND 0x00485bd0
void InsertSortNode(struct SortNode *node) {
    struct SortNode *l;

    if (SortCursor == NULL) {
        // STRING: LEGOLAND 0x004bdd40
        DBPrintf("Oh drat, Bad stuff in the sprite sorter\n");
    }
    if (node == NULL) {
        // STRING: LEGOLAND 0x004bdd14
        DBPrintf("Oh no, Not enough RAM for sprite sort list\n");
    }
    if (PrintListHead == NULL) {
        node->left = NULL;
        node->right = NULL;
        PrintListHead = node;
    } else {
        if (node->key < SortCursor->key) {
            while (node->key < SortCursor->key && SortCursor->left != NULL) {
                SortCursor = SortCursor->left;
            }
        } else {
            while (node->key > SortCursor->key && SortCursor->right != NULL) {
                SortCursor = SortCursor->right;
            }
        }
        if (node->key < SortCursor->key) {
            l = SortCursor->left;
            node->left = l;
            if (l == NULL) {
                PrintListHead = node;
            } else {
                l->right = node;
            }
            node->right = SortCursor;
            SortCursor->left = node;
        } else {
            l = SortCursor->right;
            node->right = l;
            if (l != NULL) {
                l->left = node;
            }
            node->left = SortCursor;
            SortCursor->right = node;
        }
    }
    SortCursor = node;
    if (node == NULL) {
        DBPrintf("Oh drat, Bad stuff in the sprite sorter\n");
    }
}

// FUNCTION: LEGOLAND 0x00485cd0
LEGO_EXPORT void SortSpriteWithCallback(struct Sprite *sprite, unsigned int x, unsigned int y, int key, unsigned int param_5, void (*param_6)(void *arg), void *param_7, struct HitInfo *hit) {
    unsigned int original = PrintListBytesUsed;
    struct SortNode *node;

    PrintListBytesUsed += SORT_NODE_PLAIN;
    node = (struct SortNode *)&PrintListPool[original];
    node->key = key;
    node->sprite = sprite;
    node->x = x;
    node->y = y;
    node->flags = 4;
    node->plain.mode = param_5;
    node->plain.callback = param_6;
    node->plain.callback_arg = param_7;
    node->plain.palette = GetOverridePalette();
    node->plain.frame = GetOverrideFrame();
    if (hit != NULL) {
        node->hit = *hit;
        InsertSortNode(node);
        return;
    }
    node->hit.type = 0x100;
    InsertSortNode(node);
}

// FUNCTION: LEGOLAND 0x00485d70
LEGO_EXPORT void SortSprite(struct Sprite *sprite, unsigned int x, unsigned int y, int key, unsigned int param_5, struct HitInfo *hit) {
    unsigned int original = PrintListBytesUsed;
    struct SortNode *node;

    PrintListBytesUsed += SORT_NODE_PLAIN;
    node = (struct SortNode *)&PrintListPool[original];
    node->key = key;
    node->sprite = sprite;
    node->flags = 0;
    node->x = x;
    node->y = y;
    node->plain.mode = param_5;
    node->plain.palette = GetOverridePalette();
    node->plain.frame = GetOverrideFrame();
    if (hit != NULL) {
        node->hit = *hit;
        InsertSortNode(node);
        return;
    }
    node->hit.type = 0x100;
    InsertSortNode(node);
}

// FUNCTION: LEGOLAND 0x00485e00
LEGO_EXPORT void SortPerson(struct Person *person, unsigned int param_2, void *param_3) {
    unsigned int original = PrintListBytesUsed;
    struct SortNode *block;

    PrintListBytesUsed += SORT_NODE_PERSON;
    block = (struct SortNode *)&PrintListPool[original];
    block->key = param_2;
    block->flags = 0x2000;
    block->sprite = (struct Sprite *)person;
    InsertSortNode(block);
}

// FUNCTION: LEGOLAND 0x00485e40
LEGO_EXPORT void SortClippedSprite(struct Sprite *sprite, unsigned int x, unsigned int y, int key, RECT *clip, unsigned int param_6, struct HitInfo *hit) {
    unsigned int original = PrintListBytesUsed;
    struct SortNode *node;

    PrintListBytesUsed += SORT_NODE_CLIPPED;
    node = (struct SortNode *)&PrintListPool[original];
    node->key = key;
    node->sprite = sprite;
    node->x = x;
    node->flags = 1;
    node->y = y;
    node->clipped.clip = *clip;
    node->clipped.mode = param_6;
    node->clipped.palette = GetOverridePalette();
    node->clipped.frame = GetOverrideFrame();
    if (hit != NULL) {
        node->hit = *hit;
        InsertSortNode(node);
        return;
    }
    node->hit.type = 0x100;
    InsertSortNode(node);
}

// FUNCTION: LEGOLAND 0x00485ef0
LEGO_EXPORT void ResetHitInfo(void) {
    Hover.type = 0x100;
}

// FUNCTION: LEGOLAND 0x00485f00
void PrintSpriteSimple(struct Sprite *param_1, unsigned int param_2, unsigned int param_3) {
    PrintSprite(param_1, param_2, param_3, 0, 0);
}

/* Sets the texture node the textured span drawers sample from. */
// FUNCTION: LEGOLAND 0x00485f20
void SetCurrentTexture(void *ptr) {
    CurrentTexture = ptr;
}

// FUNCTION: LEGOLAND 0x00485f30
void SetRender3DTarget(unsigned char *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4) {
    Render3DTargetBits = param_1;
    Render3DTargetPitch = param_2;
    Render3DTargetWidth = param_3;
    Render3DTargetHeight = param_4;
}

// FUNCTION: LEGOLAND 0x00485f60
void AllocZBuffer(void) {
    ZBufferWidth = 0x80;
    ZBufferHeight = 0x78;
    ZBuffer = malloc(0xf000);
    ZBufferStride = ZBufferWidth * 4;
}

// FUNCTION: LEGOLAND 0x00485fa0
void FreeZBuffer(void) {
    if (ZBuffer != NULL) {
        free(ZBuffer);
    }
}

/* Initialises the software 3D renderer for InitMan: stores the green channel bit count (6 when DisplayPixelFormat
 * is 2, else 5) read by InitPackedColorLookupTable, builds that colour table, allocates the Z buffer and fills the
 * reciprocal tables. */
// FUNCTION: LEGOLAND 0x00485fc0
void InitRender3D(unsigned int param_1) {
    *(unsigned int *)&ColorGreenBits = param_1;
    InitPackedColorLookupTable();
    AllocZBuffer();
    InitReciprocalTables();
}

/* Clears the Z buffer, then fills it from the sprite's 32-bit image data clipped to the Z buffer window at (x, y)
 * (ZBufferHelper). With no sprite it only clears. */
// FUNCTION: LEGOLAND 0x00485fe0
void LoadZBufferFromSprite(struct Sprite *sprite, int x, int y) {
    RECT rect1;
    RECT rect2;
    int offsets[2];

    __asm {
        push edi
        mov eax, ZBufferWidth
        imul ZBufferHeight
        mov ecx, eax
        mov eax, 0
        mov edi, ZBuffer
        rep stosd
        pop edi
    }
    if (sprite == NULL) {
        return;
    }
    rect1.left = 0;
    rect1.top = 0;
    rect1.right = sprite->width;
    rect1.bottom = (short)sprite->height;
    rect2.left = x;
    rect2.top = y;
    rect2.right = x + ZBufferWidth;
    rect2.bottom = y + ZBufferHeight;
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
    ZBufferHelper((unsigned int *)sprite->image->data, (int *)&rect1, offsets, ZBuffer);
}
