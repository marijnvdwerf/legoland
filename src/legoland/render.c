#include <windows.h>
#include <ddraw.h>

#include "legoland.h"
#include "crt.h"

#include "print_sprite.h"
#include "render.h"
#include "challenge.h"
#include "globals.h"
#include "image_sprite.h"
#include "draw.h"
#include "gfx.h"

struct ZBlitDesc {
    /* 0x00 */ int left;
    /* 0x04 */ int top;
    /* 0x08 */ int right;
    /* 0x0c */ int bottom;
    /* 0x10 */ int w;
    /* 0x14 */ int h;
};

struct CursorCacheNode {
    struct CursorCacheNode *next;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char pad_7;
    unsigned int value;
};

struct CursorKey {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct CursorBitmap {
    unsigned char pad_0[4];
    void *pixels;
};

struct TextureNode {
    unsigned char pad_0[8];
    void *data_8;
    void *data_c;
};

struct RenderViewport {
    unsigned int x;
    unsigned int y;
};

struct TextureFrame {
    /* 0x00 */ void *field_0;
    /* 0x04 */ unsigned short *data;
};

struct TextureDesc {
    /* 0x00 */ unsigned int width;
    /* 0x04 */ unsigned int height;
    /* 0x08 */ unsigned char shift;
    /* 0x09 */ unsigned char pad_9[3];
    /* 0x0c */ unsigned char *index_map;
    /* 0x10 */ struct TextureFrame **frame_table;
};


// FUNCTION: LEGOLAND 0x004860f0
void FUN_004860f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00486190
unsigned int FUN_00486190(struct CursorKey *key) {
    struct CursorCacheNode *node;

    if (DAT_00797e6c != 0) {
        node = DAT_00797e6c;
        while (node != 0) {
            if (node->b == key->b && node->g == key->g && node->r == key->r) {
                return node->value;
            }
            node = node->next;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004861d0
struct CursorCacheNode *FUN_004861d0(unsigned int value, const unsigned char *key) {
    struct CursorCacheNode *node = (struct CursorCacheNode *)malloc(0xc);
    if (node != 0) {
        memset(node, 0, 0xc);
        node->b = key[2];
        node->g = key[1];
        node->r = key[0];
        node->value = value;
        if (DAT_00797e6c) {
            node->next = DAT_00797e6c;
        }
        DAT_00797e6c = node;
    }
    return node;
}

// FUNCTION: LEGOLAND 0x00486220
void FUN_00486220(void *param) {
    struct CursorBitmap *bmp = (struct CursorBitmap *)param;
    if (bmp != 0) {
        if (bmp->pixels != 0) {
            free(bmp->pixels);
        }
        free(bmp);
    }
}

// FUNCTION: LEGOLAND 0x00486250
void FUN_00486250(void) {
    struct CursorCacheNode *node = DAT_00797e6c;
    while (node != 0) {
        struct CursorCacheNode *next = node->next;
        FUN_00486220((void *)node->value);
        free(node);
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x00486280
unsigned int FUN_00486280(int param_1, void *param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x004864e0
void FUN_004864e0(unsigned int param_1) {
    DAT_0066b61c = param_1;
}

// FUNCTION: LEGOLAND 0x00486540
void FUN_00486540(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00486590
void FUN_00486590(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00486c70
void FUN_00486c70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004877b0
void FUN_004877b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00487d40
void FUN_00487d40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00488670
void FUN_00488670(unsigned int arg, unsigned int index) {
    void *ptr = malloc(0x2c);
    if (ptr != 0) {
        FUN_004437d0(arg, ptr);
        DAT_00798190[index] = ptr;
    }
}

// FUNCTION: LEGOLAND 0x004886a0
void FUN_004886a0(void) {
    struct TextureNode **slot = (struct TextureNode **)&DAT_00798190;
    do {
        if (*slot != 0) {
            free((*slot)->data_8);
            free((*slot)->data_c);
            free(*slot);
            *slot = 0;
        }
        slot++;
    } while ((int)slot < (int)&DAT_00798590);
}

// FUNCTION: LEGOLAND 0x004886e0
void FUN_004886e0(unsigned int index) {
    FUN_00485f20(DAT_00798190[index]);
}

// FUNCTION: LEGOLAND 0x00488700
void FUN_00488700(unsigned int base, struct RenderViewport *vp) {
    DAT_007feb14 = 0;
    DAT_007fe9a8 = base + (vp->y * DAT_00701e58) + (vp->x * 2);
}

// FUNCTION: LEGOLAND 0x00488730
unsigned short FUN_00488730(unsigned int p1, unsigned int p2, unsigned int p3) {
    struct TextureDesc *desc = (struct TextureDesc *)DAT_0066b630;
    unsigned int u = (unsigned int)((unsigned __int64)(desc->width - 1) * (p1 & 0xffff) >> 0x10);
    unsigned int v = (unsigned int)((unsigned __int64)(desc->height - 1) * (p2 & 0xffff) >> 0x10);
    unsigned int idx = (v << desc->shift) + u;
    struct TextureFrame *frame = desc->frame_table[desc->index_map[idx]];
    return frame->data[(p3 - ((p3 >> 0x10 & 1) != 0)) >> 10];
}

// FUNCTION: LEGOLAND 0x004887a0
void FUN_004887a0(void) {
    DAT_00798590 = (void *)1;
    DAT_0066be50 = CreateSourceImage(DAT_004d8bb0, 0);
    DAT_0066be50->data = DAT_00701e68;
    DAT_0066be50->width = 0x280;
    DAT_0066be50->height = 0x1e0;
    DAT_0066be50->refcount = 1;
    DAT_0066be50->aux = NULL;
    DAT_0066be50->field_14 = 1;
    DAT_00701e64 = CreateSprite(DAT_0066be50);
    DAT_00701e64->flags = DAT_00701e64->flags | 0x208;
}

// FUNCTION: LEGOLAND 0x00488820
unsigned int FUN_00488820(unsigned int x, unsigned int y) {
    unsigned int *row = (unsigned int *)(DAT_00701e5c + DAT_0066be4c * y);
    return row[x] >> 0x18;
}

// FUNCTION: LEGOLAND 0x00488840
LEGO_EXPORT struct Sprite *GenerateNewImageFromZBuffer(struct Sprite *sprite, struct Sprite *param_2, int param_3, int param_4, int param_5) {
    int w = (short)sprite->width;
    int h = (short)sprite->height;
    struct ZBlitDesc local;
    unsigned short transp;
    int row;
    int yy;

    if (DAT_00798590 == 0) {
        FUN_004887a0();
    }
    DAT_0066b620 = DAT_00668108;
    DAT_0066b628 = DAT_00668110;
    DAT_0066b5b0 = DAT_0066809c;
    DAT_0066b62c = DAT_00668114;
    DAT_0066809c.pitch = w * 2;
    DAT_0066b624 = DAT_0066810c;
    DAT_0066809c.pixels = DAT_0066be54;
    DAT_00668108 = 0;
    DAT_0066810c = 0;
    local.left = 0;
    local.top = 0;
    local.right = 0;
    local.bottom = 0;
    DAT_0066809c.height = h;
    DAT_0066809c.width = w;
    DAT_00668110 = w;
    DAT_00668114 = h;
    local.w = w;
    local.h = h;
    DAT_007fea44 = GetTransparentColour();
    SoftPrint_Clear();
    FUN_00464ee0(&local);
    FUN_00485fe0(param_2, param_4, param_5);
    transp = (unsigned short)GetTransparentColour();
    yy = 0;
    if (0 < h) {
        row = 0;
        do {
            int xx = 0;
            int col = row;
            if (0 < w) {
                do {
                    unsigned int z = FUN_00488820(xx, yy);
                    unsigned short px = transp;
                    if ((int)(z & 0xff) <= param_3) {
                        px = *(unsigned short *)((char *)DAT_0066be54 + col);
                    }
                    *(unsigned short *)((char *)DAT_00701e68 + col) = px;
                    xx = xx + 1;
                    col = col + 2;
                } while (xx < w);
            }
            yy = yy + 1;
            row = row + w * 2;
        } while (yy < h);
    }
    DAT_0066be50->width = (short)w;
    DAT_0066be50->height = (short)h;
    DAT_00701e64->width = (short)w;
    DAT_00701e64->height = (short)h;
    DAT_00668110 = DAT_0066b628;
    DAT_0066810c = DAT_0066b624;
    DAT_0066809c = DAT_0066b5b0;
    DAT_00668108 = DAT_0066b620;
    DAT_00668114 = DAT_0066b62c;
    return DAT_00701e64;
}

// FUNCTION: LEGOLAND 0x00488a10
LEGO_EXPORT unsigned int RenderSprite(struct Sprite *sprite, int x, int y) {
    RECT dst;
    RECT src;
    DDBLTFX fx;

    src.right = (short)sprite->width;
    dst.left = x;
    dst.right = src.right + x;
    dst.top = y;
    src.bottom = (short)sprite->height;
    dst.bottom = src.bottom + y;
    src.left = 0;
    src.top = 0;
    if ((sprite->flags & 0x60) != 0) {
        if (IntersectRect(&dst, &dst, (RECT *)&SPRITE_ClipRect) != 0) {
            int i;
            unsigned int *p;
            src.top = dst.top;
            sprite->field_c = DAT_008119a4;
            src.left = dst.left;
            src.right = dst.right;
            src.bottom = dst.bottom;
            OffsetRect(&src, -x, -y);
            p = (unsigned int *)&fx;
            for (i = 0x19; i != 0; i--) {
                *p = 0;
                p++;
            }
            fx.dwSize = 100;
            PushRenderingStatusAndUnlockVideoSurface();
            if ((sprite->flags & 0x40) == 0) {
                ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Blt((LPDIRECTDRAWSURFACE)renderEngine, &dst, (LPDIRECTDRAWSURFACE)sprite->surface, &src, 0x1000000, &fx);
            } else {
                ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Blt((LPDIRECTDRAWSURFACE)renderEngine, &dst, (LPDIRECTDRAWSURFACE)sprite->surface, &src, 0x1008000, &fx);
            }
            PopRenderingStatus();
            return 1;
        }
    } else {
        if (IntersectRect(&dst, &dst, (RECT *)&SPRITE_ClipRect) != 0) {
            src.top = dst.top;
            sprite->field_c = DAT_008119a4;
            src.left = dst.left;
            src.right = dst.right;
            src.bottom = dst.bottom;
            OffsetRect(&src, -x, -y);
            FUN_00464ee0(&dst);
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00488b90
LEGO_EXPORT unsigned int RenderSpriteX(struct Sprite *sprite, int x, int y, unsigned int param_4) {
    RECT dst;
    RECT src;

    src.right = (short)sprite->width;
    dst.right = src.right + x;
    src.bottom = (short)sprite->height;
    dst.bottom = src.bottom + y;
    dst.left = x;
    dst.top = y;
    src.left = 0;
    src.top = 0;
    if (IntersectRect(&dst, &dst, (RECT *)&SPRITE_ClipRect) != 0) {
        src.top = dst.top;
        sprite->field_c = DAT_008119a4;
        src.left = dst.left;
        src.right = dst.right;
        src.bottom = dst.bottom;
        OffsetRect(&src, -x, -y);
        SoftPrint_XBltFast(sprite, &src, &dst, param_4);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00488c50
LEGO_EXPORT unsigned int RenderTiledSprite(struct Sprite *sprite, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7) { STUB(); }

// FUNCTION: LEGOLAND 0x00488c80
unsigned int FUN_00488c80(struct Sprite *sprite, int param_2, int param_3, int param_4, int param_5, int *param_6) { STUB(); }

// FUNCTION: LEGOLAND 0x00489080
LEGO_EXPORT unsigned int RenderScaledSprite(struct Sprite *param_1, int param_2, int param_3, int param_4, int param_5) {
    int local[2];

    local[0] = 0;
    local[1] = 0;
    FUN_00488c80(param_1, param_2, param_3, param_4, param_5, local);
    return 1;
}

// FUNCTION: LEGOLAND 0x004890c0
LEGO_EXPORT unsigned int RenderBlock(int x, int y, int w, int h, unsigned int color) {
    RECT dst;
    DDBLTFX fx;

    dst.left = x;
    dst.top = y;
    dst.right = x + w;
    dst.bottom = y + h;
    fx.dwSize = 100;
    fx.dwFillColor = color;
    if (IntersectRect(&dst, &dst, (RECT *)&SPRITE_ClipRect) == 0) {
        return 1;
    }
    PushRenderingStatusAndUnlockVideoSurface();
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->SetClipper((LPDIRECTDRAWSURFACE)renderEngine, (LPDIRECTDRAWCLIPPER)DAT_00668080);
    if (((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Blt((LPDIRECTDRAWSURFACE)renderEngine, &dst, NULL, NULL, 0x1000400, &fx) == 0) {
        ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->SetClipper((LPDIRECTDRAWSURFACE)renderEngine, NULL);
        PopRenderingStatus();
        return 1;
    }
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->SetClipper((LPDIRECTDRAWSURFACE)renderEngine, NULL);
    PopRenderingStatus();
    return 0;
}

// FUNCTION: LEGOLAND 0x00489190
LEGO_EXPORT void RenderTransSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00489390
LEGO_EXPORT void RenderThickBox(int x, int y, int w, int h, int thickness, unsigned int color) {
    int inner;
    RenderBlock(x, y, w, thickness, color);
    inner = h + thickness * -2;
    RenderBlock(x, y + thickness, thickness, inner, color);
    RenderBlock((x - thickness) + w, y + thickness, thickness, inner, color);
    RenderBlock(x, (y - thickness) + h, w, thickness, color);
}

// FUNCTION: LEGOLAND 0x00489410
LEGO_EXPORT void RenderBox(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5) {
    RenderThickBox(a1, a2, a3, a4, 1, a5);
}
