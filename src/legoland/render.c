#include "legoland.h"

#include "print_sprite.h"

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

extern struct CursorCacheNode *DAT_00797e6c;
extern unsigned int DAT_0066b61c;
extern void *DAT_00798190[256];
extern void *DAT_00798590;
extern unsigned int DAT_007feb14;
extern unsigned int DAT_007fe9a8;
extern unsigned int DAT_00701e58;
extern unsigned int DAT_0066be4c;
extern unsigned int DAT_00701e5c;

extern void *_malloc(unsigned int size);
extern void FUN_0049e4d0(void *ptr);
extern void FUN_004437d0(unsigned int arg, void *ptr);

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
    struct CursorCacheNode *node = (struct CursorCacheNode *)_malloc(0xc);
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
            FUN_0049e4d0(bmp->pixels);
        }
        FUN_0049e4d0(bmp);
    }
}

// FUNCTION: LEGOLAND 0x00486250
void FUN_00486250(void) {
    struct CursorCacheNode *node = DAT_00797e6c;
    while (node != 0) {
        struct CursorCacheNode *next = node->next;
        FUN_00486220((void *)node->value);
        FUN_0049e4d0(node);
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x00486280
void FUN_00486280(void) { STUB(); }

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
    void *ptr = _malloc(0x2c);
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
            FUN_0049e4d0((*slot)->data_8);
            FUN_0049e4d0((*slot)->data_c);
            FUN_0049e4d0(*slot);
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
void FUN_00488730(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004887a0
void FUN_004887a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00488820
unsigned int FUN_00488820(unsigned int x, unsigned int y) {
    unsigned int *row = (unsigned int *)(DAT_00701e5c + DAT_0066be4c * y);
    return row[x] >> 0x18;
}

// FUNCTION: LEGOLAND 0x00488840
void GenerateNewImageFromZBuffer(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00488a10
void RenderSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00488b90
void RenderSpriteX(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00488c50
void RenderTiledSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00488c80
void FUN_00488c80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00489080
void RenderScaledSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004890c0
void RenderBlock(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00489190
void RenderTransSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00489390
void RenderThickBox(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6) { STUB(); }

// FUNCTION: LEGOLAND 0x00489410
void RenderBox(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5) {
    RenderThickBox(a1, a2, a3, a4, 1, a5);
}
