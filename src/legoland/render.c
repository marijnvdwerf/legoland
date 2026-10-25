#include <windows.h>
#include <ddraw.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "challenge.h"
#include "draw.h"
#include "gfx.h"
#include "globals.h"
#include "image_sprite.h"
#include "print_sprite.h"
#include "render.h"

struct ZBlitDesc {
    int off[2];
    RECT rect;
};

struct CursorCacheNode {
    struct CursorCacheNode *next;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char pad_7;
    void *value; /* the cached colour ramp (struct TextureFrame) */
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

struct RenderViewport {
    unsigned int x;
    unsigned int y;
};

struct TextureFrame {
    /* 0x00 */ int field_0; /* entry count */
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

/* Builds the per-channel packed-pixel lookup table used to combine red, green and blue intensities for the current
 * display format. */
// FUNCTION: LEGOLAND 0x004860f0
void InitPackedColorLookupTable(void) {
    int i;
    unsigned int n;
    unsigned int sh;
    unsigned int mask;
    unsigned short v;
    float f;

    n = *(unsigned int *)&ColorGreenBits;
    sh = n + 5;
    mask = 0xffu >> (8 - n);
    for (i = 0; i < 256; i++) {
        f = i * FLOAT_004ab550;
        v = (unsigned short)(int)(f * FLOAT_004ab444);
        ColorLut[i].shifted = v << sh;
        ColorLut[i].scaled = (int)(f * mask) << 5;
        ColorLut[i].value = v;
    }
}

// FUNCTION: LEGOLAND 0x00486190
void *FindCursorCacheEntry(struct CursorKey *key) {
    struct CursorCacheNode *node;

    if (CursorCacheList != 0) {
        node = CursorCacheList;
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
struct CursorCacheNode *AddCursorCacheEntry(void *value, const unsigned char *key) {
    struct CursorCacheNode *node = (struct CursorCacheNode *)malloc(sizeof(struct CursorCacheNode));
    if (node != 0) {
        memset(node, 0, sizeof(struct CursorCacheNode));
        node->b = key[2];
        node->g = key[1];
        node->r = key[0];
        node->value = value;
        if (CursorCacheList) {
            node->next = CursorCacheList;
        }
        CursorCacheList = node;
    }
    return node;
}

// FUNCTION: LEGOLAND 0x00486220
void FreeCursorBitmap(void *param) {
    struct CursorBitmap *bmp = (struct CursorBitmap *)param;
    if (bmp != 0) {
        if (bmp->pixels != 0) {
            free(bmp->pixels);
        }
        free(bmp);
    }
}

// FUNCTION: LEGOLAND 0x00486250
void FreeCursorCache(void) {
    struct CursorCacheNode *node = CursorCacheList;
    while (node != 0) {
        struct CursorCacheNode *next = node->next;
        FreeCursorBitmap(node->value);
        free(node);
        node = next;
    }
}

/* Returns a cached color ramp for the RGB key, or allocates and caches a ramp interpolating from black through the
 * key color to white. */
// FUNCTION: LEGOLAND 0x00486280
void *GetOrCreateColorRamp(int param_1, void *param_2) {
    struct CursorKey *key = (struct CursorKey *)param_2;
    struct TextureFrame *frame;
    void *cached;
    float fb, fg, fr;
    float sb, sg, sr;
    float ab, ag, ar;
    int i;

    cached = FindCursorCacheEntry(key);
    if (cached != 0) {
        return cached;
    }
    frame = (struct TextureFrame *)malloc(sizeof(struct TextureFrame));
    if (frame != 0) {
        frame->field_0 = param_1;
        frame->data = (unsigned short *)malloc(param_1 * 2);
        fb = key->b;
        fg = key->g;
        fr = key->r;
        param_1 >>= 1;
        ab = FLOAT_004ab390;
        ag = FLOAT_004ab390;
        ar = FLOAT_004ab390;
        sb = fb / param_1;
        sg = fg / param_1;
        sr = fr / param_1;
        i = 0;
        if (param_1 > 0) {
            do {
                frame->data[i++] = ColorLut[(unsigned char)(int)ar].value | ColorLut[(unsigned char)(int)ag].scaled | ColorLut[(unsigned char)(int)ab].shifted;
                ab += sb;
                ag += sg;
                ar += sr;
            } while (i < param_1);
        }
        sb = (255.0f - fb) / param_1;
        sg = (255.0f - fg) / param_1;
        sr = (255.0f - fr) / param_1;
        ab = fb;
        ag = fg;
        ar = fr;
        if (param_1 > 0) {
            i = param_1;
            do {
                frame->data[i++] = ColorLut[(unsigned char)(int)ar].value | ColorLut[(unsigned char)(int)ag].scaled | ColorLut[(unsigned char)(int)ab].shifted;
                ab += sb;
                ag += sg;
                ar += sr;
            } while (--param_1 != 0);
        }
    }
    AddCursorCacheEntry(frame, (unsigned char *)key);
    return frame;
}

/* Sets the colour ramp (from GetOrCreateColorRamp: a count, then 16-bit colours) that the shaded span drawers index
 * by intensity; the person renderer passes it for materials with flag 0x2000. */
// FUNCTION: LEGOLAND 0x004864e0
void SetCurrentColorRamp(void *param_1) {
    CurrentColorRamp = param_1;
}

// FUNCTION: LEGOLAND 0x00486540
int InitReciprocalTables(void) {
    int i;
    float f;

    for (i = 1; i < 100; i++) {
        f = (float)i;
        ReciprocalTable[i] = 1.0f / f;
        ScaledReciprocalTable[i] = (int)(DOUBLE_004ab558 / f);
    }
    return 0;
}

/* Rasterises one person-mesh triangle with Gouraud shading through CurrentColorRamp (vertex intensities field_14
 * interpolated), Z-buffered, clipped to the viewport, with the mouse-pixel hit test. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00486590
__declspec(naked) void DrawGouraudRampTriangle(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x6c
        mov edx, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp + 0xc]
        push ebx
        push esi
        mov eax, dword ptr [edx + 4]
        mov esi, dword ptr [ecx + 4]
        cmp eax, esi
        push edi
        jle L4865b8
        mov eax, dword ptr [ebp + 8]
        xchg dword ptr [ebp + 0xc], eax
        mov dword ptr [ebp + 8], eax
        mov ecx, dword ptr [ebp + 0xc]
        mov edx, dword ptr [ebp + 8]
L4865b8:
        mov ebx, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ecx + 4]
        cmp eax, dword ptr [ebx + 4]
        jle L4865d2
        mov eax, dword ptr [ebp + 0xc]
        xchg dword ptr [ebp + 0x10], eax
        mov dword ptr [ebp + 0xc], eax
        mov ebx, dword ptr [ebp + 0x10]
        mov ecx, dword ptr [ebp + 0xc]
L4865d2:
        mov eax, dword ptr [edx + 4]
        mov esi, dword ptr [ecx + 4]
        cmp eax, esi
        jle L4865eb
        mov eax, dword ptr [ebp + 8]
        xchg dword ptr [ebp + 0xc], eax
        mov dword ptr [ebp + 8], eax
        mov ecx, dword ptr [ebp + 0xc]
        mov edx, dword ptr [ebp + 8]
L4865eb:
        mov edi, dword ptr [ebx + 4]
        mov eax, dword ptr [edx + 4]
        sar edi, 0x10
        mov dword ptr [ebp - 0x28], edi
        mov edi, dword ptr [Render3DTargetPitch]
        sar eax, 0x10
        imul edi, eax
        add edi, dword ptr [Render3DTargetBits]
        mov esi, dword ptr [ecx + 4]
        sar esi, 0x10
        mov dword ptr [ebp - 0x18], edi
        mov edi, dword ptr [edx]
        mov dword ptr [ebp + 8], edi
        mov edi, dword ptr [ecx]
        mov dword ptr [ebp - 0x64], edi
        mov edi, dword ptr [ebx]
        mov dword ptr [ebp - 0x5c], edi
        mov edi, dword ptr [edx + 8]
        mov edx, dword ptr [edx + 0x14]
        mov dword ptr [ebp - 0x24], edi
        mov edi, dword ptr [ecx + 8]
        mov ecx, dword ptr [ecx + 0x14]
        mov dword ptr [ebp - 0x68], edi
        mov edi, dword ptr [ebx + 8]
        mov dword ptr [ebp - 0x14], edx
        mov edx, dword ptr [ebx + 0x14]
        mov dword ptr [ebp - 0x54], eax
        mov dword ptr [ebp - 0x40], esi
        mov dword ptr [ebp - 0x60], edi
        mov dword ptr [ebp - 0x30], ecx
        mov dword ptr [ebp - 0x2c], edx
        shl dword ptr [ebp - 0x14], 6
        shl dword ptr [ebp - 0x30], 6
        shl dword ptr [ebp - 0x2c], 6
        mov edi, eax
        cmp eax, esi
        mov dword ptr [ebp + 0x10], edi
        je L48697f
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x40]
        sub ecx, dword ptr [ebp - 0x54]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x58], eax
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x28]
        sub ecx, dword ptr [ebp - 0x54]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x64]
        sub eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp - 0x58]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x34], eax
        mov eax, dword ptr [ebp - 0x5c]
        sub eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x50], eax
        mov eax, dword ptr [ebp - 0x30]
        sub eax, dword ptr [ebp - 0x14]
        mov ecx, dword ptr [ebp - 0x58]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x38], eax
        mov eax, dword ptr [ebp - 0x2c]
        sub eax, dword ptr [ebp - 0x14]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x54], eax
        mov eax, dword ptr [ebp - 0x68]
        sub eax, dword ptr [ebp - 0x24]
        mov ecx, dword ptr [ebp - 0x58]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x3c], eax
        mov eax, dword ptr [ebp - 0x60]
        sub eax, dword ptr [ebp - 0x24]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x58], eax
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [ebp + 0xc], eax
        mov dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp - 0x14]
        mov dword ptr [ebp - 4], eax
        mov dword ptr [ebp - 8], eax
        mov eax, dword ptr [ebp - 0x24]
        mov dword ptr [ebp - 0xc], eax
        mov dword ptr [ebp - 0x10], eax
        mov ebx, dword ptr [ViewportTop]
        mov ecx, dword ptr [ebp + 0x10]
        mov edx, dword ptr [ebp - 0x40]
        sub ebx, ecx
        jle L486768
        sub edx, ecx
        jle L486768
        cmp ebx, edx
        mov ecx, edx
        jns L48672b
        mov ecx, ebx
L48672b:
        add dword ptr [ebp + 0x10], ecx
        mov eax, dword ptr [ebp - 0x34]
        mul ecx
        add dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x50]
        mul ecx
        add dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp - 0x38]
        mul ecx
        add dword ptr [ebp - 4], eax
        mov eax, dword ptr [ebp - 0x54]
        mul ecx
        add dword ptr [ebp - 8], eax
        mov eax, dword ptr [ebp - 0x3c]
        mul ecx
        add dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp - 0x58]
        mul ecx
        add dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [Render3DTargetPitch]
        mul ecx
        add dword ptr [ebp - 0x18], eax
L486768:
        cmp dword ptr [ebp + 0x10], esi
        jge L48692f
L486771:
        mov eax, dword ptr [ebp + 0x10]
        mov ecx, dword ptr [ViewportBottom]
        cmp eax, ecx
        jg L48692f
        mov eax, dword ptr [ebp - 0x18]
        mov ecx, dword ptr [Render3DTargetPitch]
        mov esi, dword ptr [ebp + 0xc]
        mov edi, dword ptr [ebp + 8]
        mov dword ptr [ebp - 0x6c], eax
        add eax, ecx
        cmp esi, edi
        mov dword ptr [ebp - 0x18], eax
        jle L4867ef
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 0xc]
        sub ecx, dword ptr [ebp + 8]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x14], eax
        mov eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp - 8]
        sub eax, ecx
        mov dword ptr [ebp - 0x24], ecx
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x48], eax
        mov eax, dword ptr [ebp - 0xc]
        mov ecx, dword ptr [ebp - 0x10]
        sub eax, ecx
        mov dword ptr [ebp - 0x20], ecx
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x44], eax
        sar edi, 0x10
        sar esi, 0x10
        mov dword ptr [ebp - 0x4c], edi
        mov dword ptr [ebp - 0x1c], esi
        jmp L48683f
L4867ef:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 8]
        sub ecx, dword ptr [ebp + 0xc]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x14], eax
        mov eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp - 4]
        sub eax, ecx
        mov dword ptr [ebp - 0x24], ecx
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x48], eax
        mov eax, dword ptr [ebp - 0x10]
        mov ecx, dword ptr [ebp - 0xc]
        sub eax, ecx
        mov dword ptr [ebp - 0x20], ecx
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x44], eax
        sar esi, 0x10
        sar edi, 0x10
        mov dword ptr [ebp - 0x4c], esi
        mov dword ptr [ebp - 0x1c], edi
L48683f:
        push esi
        push edi
        mov ecx, dword ptr [ebp - 0x4c]
        mov edi, ecx
        mov ebx, dword ptr [ViewportLeft]
        sub ecx, ebx
        jns L486864
        mov edi, ebx
        neg ecx
        mov eax, dword ptr [ebp - 0x48]
        mul ecx
        add dword ptr [ebp - 0x24], eax
        mov eax, dword ptr [ebp - 0x44]
        mul ecx
        add dword ptr [ebp - 0x20], eax
L486864:
        mov esi, edi
        shl esi, 1
        add esi, dword ptr [ebp - 0x6c]
        mov eax, edi
        mov edi, dword ptr [ViewportRight]
        shl edi, 1
        add edi, dword ptr [ebp - 0x6c]
        cmp eax, dword ptr [ebp - 0x1c]
        jge L4868d9
        cmp esi, edi
        jg L4868d9
L486881:
        mov ebx, dword ptr [ZBuffer]
        mov ecx, dword ptr [ebp + 0x10]
        lea ebx, [ebx + eax*4]
        shl ecx, 9
        mov edx, dword ptr [ebp - 0x20]
        cmp edx, dword ptr [ebx + ecx]
        jb L4868be
        mov dword ptr [ebx + ecx], edx
        mov ebx, dword ptr [CurrentColorRamp]
        mov ebx, dword ptr [ebx + 4]
        cmp esi, dword ptr [MousePixelAddress]
        movzx edx, word ptr [ebp - 0x22]
        sete cl
        movzx edx, word ptr [ebx + edx*2]
        or byte ptr [MousePixelCovered], cl
        mov word ptr [esi], dx
L4868be:
        inc eax
        mov ebx, dword ptr [ebp - 0x48]
        add dword ptr [ebp - 0x24], ebx
        mov ebx, dword ptr [ebp - 0x44]
        add dword ptr [ebp - 0x20], ebx
        add esi, 2
        cmp eax, dword ptr [ebp - 0x1c]
        jge L4868d9
        cmp esi, edi
        jg L4868d9
        jmp L486881
L4868d9:
        pop edi
        pop esi
        mov ecx, dword ptr [ebp - 0x34]
        mov edi, dword ptr [ebp + 0xc]
        mov edx, dword ptr [ebp - 0x50]
        mov esi, dword ptr [ebp + 8]
        mov eax, dword ptr [ebp - 0x38]
        mov ebx, dword ptr [ebp - 8]
        add edi, ecx
        mov ecx, dword ptr [ebp - 4]
        add esi, edx
        mov edx, dword ptr [ebp - 0x3c]
        mov dword ptr [ebp + 8], esi
        mov esi, dword ptr [ebp - 0x10]
        add ecx, eax
        mov eax, dword ptr [ebp - 0x58]
        mov dword ptr [ebp + 0xc], edi
        mov edi, dword ptr [ebp - 0xc]
        mov dword ptr [ebp - 4], ecx
        mov ecx, dword ptr [ebp - 0x54]
        add esi, eax
        mov eax, dword ptr [ebp + 0x10]
        add ebx, ecx
        mov ecx, dword ptr [ebp - 0x40]
        add edi, edx
        inc eax
        cmp eax, ecx
        mov dword ptr [ebp - 8], ebx
        mov dword ptr [ebp - 0xc], edi
        mov dword ptr [ebp - 0x10], esi
        mov dword ptr [ebp + 0x10], eax
        jl L486771
L48692f:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x28]
        sub ecx, dword ptr [ebp - 0x40]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x1c], eax
        mov eax, dword ptr [ebp - 0x5c]
        sub eax, dword ptr [ebp - 0x64]
        mov ecx, dword ptr [ebp - 0x1c]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x34], eax
        mov eax, dword ptr [ebp - 0x2c]
        sub eax, dword ptr [ebp - 0x30]
        mov ecx, dword ptr [ebp - 0x1c]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x38], eax
        mov eax, dword ptr [ebp - 0x60]
        sub eax, dword ptr [ebp - 0x68]
        mov ecx, dword ptr [ebp - 0x1c]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x3c], eax
        mov edi, dword ptr [ebp + 0x10]
        jmp L486a33
L48697f:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x28]
        sub ecx, dword ptr [ebp - 0x54]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x28]
        sub ecx, dword ptr [ebp - 0x40]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x1c], eax
        mov eax, dword ptr [ebp - 0x5c]
        sub eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x34], eax
        mov eax, dword ptr [ebp - 0x5c]
        sub eax, dword ptr [ebp - 0x64]
        mov ecx, dword ptr [ebp - 0x1c]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x50], eax
        mov eax, dword ptr [ebp - 0x2c]
        sub eax, dword ptr [ebp - 0x14]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x38], eax
        mov eax, dword ptr [ebp - 0x2c]
        sub eax, dword ptr [ebp - 0x30]
        mov ecx, dword ptr [ebp - 0x1c]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x54], eax
        mov eax, dword ptr [ebp - 0x60]
        sub eax, dword ptr [ebp - 0x24]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x3c], eax
        mov eax, dword ptr [ebp - 0x60]
        sub eax, dword ptr [ebp - 0x68]
        mov ecx, dword ptr [ebp - 0x1c]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x58], eax
        mov ecx, dword ptr [ebp + 8]
        mov edx, dword ptr [ebp - 0x64]
        mov eax, dword ptr [ebp - 0x14]
        mov dword ptr [ebp + 0xc], ecx
        mov ecx, dword ptr [ebp - 0x30]
        mov dword ptr [ebp + 8], edx
        mov edx, dword ptr [ebp - 0x24]
        mov dword ptr [ebp - 4], eax
        mov eax, dword ptr [ebp - 0x68]
        mov dword ptr [ebp - 8], ecx
        mov dword ptr [ebp - 0xc], edx
        mov dword ptr [ebp - 0x10], eax
L486a33:
        cmp edi, dword ptr [ViewportBottom]
        jg L486c69
        mov esi, dword ptr [ebp - 0x28]
        cmp edi, esi
        jge L486c69
        mov ebx, dword ptr [ViewportTop]
        mov ecx, dword ptr [ebp + 0x10]
        mov edx, dword ptr [ebp - 0x28]
        sub ebx, ecx
        jle L486aa3
        sub edx, ecx
        jle L486aa3
        cmp ebx, edx
        mov ecx, edx
        jns L486a66
        mov ecx, ebx
L486a66:
        add dword ptr [ebp + 0x10], ecx
        mov eax, dword ptr [ebp - 0x34]
        mul ecx
        add dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x50]
        mul ecx
        add dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp - 0x38]
        mul ecx
        add dword ptr [ebp - 4], eax
        mov eax, dword ptr [ebp - 0x54]
        mul ecx
        add dword ptr [ebp - 8], eax
        mov eax, dword ptr [ebp - 0x3c]
        mul ecx
        add dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp - 0x58]
        mul ecx
        add dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [Render3DTargetPitch]
        mul ecx
        add dword ptr [ebp - 0x18], eax
L486aa3:
        cmp dword ptr [ebp + 0x10], esi
        jge L486c69
L486aac:
        mov ecx, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ViewportBottom]
        cmp ecx, eax
        jg L486c69
        mov eax, dword ptr [ebp - 0x18]
        mov ecx, dword ptr [Render3DTargetPitch]
        mov esi, dword ptr [ebp + 0xc]
        mov edi, dword ptr [ebp + 8]
        mov dword ptr [ebp - 0x6c], eax
        add eax, ecx
        cmp esi, edi
        mov dword ptr [ebp - 0x18], eax
        jle L486b29
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 0xc]
        sub ecx, dword ptr [ebp + 8]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x14], eax
        mov eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp - 8]
        sub eax, ecx
        mov dword ptr [ebp - 0x24], ecx
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x48], eax
        mov eax, dword ptr [ebp - 0xc]
        mov ecx, dword ptr [ebp - 0x10]
        sub eax, ecx
        mov dword ptr [ebp - 0x20], ecx
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x44], eax
        sar edi, 0x10
        sar esi, 0x10
        mov dword ptr [ebp - 0x4c], edi
        mov dword ptr [ebp - 0x1c], esi
        jmp L486b79
L486b29:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 8]
        sub ecx, dword ptr [ebp + 0xc]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x14], eax
        mov eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp - 4]
        sub eax, ecx
        mov dword ptr [ebp - 0x24], ecx
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x48], eax
        mov eax, dword ptr [ebp - 0x10]
        mov ecx, dword ptr [ebp - 0xc]
        sub eax, ecx
        mov dword ptr [ebp - 0x20], ecx
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x44], eax
        sar esi, 0x10
        sar edi, 0x10
        mov dword ptr [ebp - 0x4c], esi
        mov dword ptr [ebp - 0x1c], edi
L486b79:
        push esi
        push edi
        mov ecx, dword ptr [ebp - 0x4c]
        mov edi, ecx
        mov ebx, dword ptr [ViewportLeft]
        sub ecx, ebx
        jns L486b9e
        mov edi, ebx
        neg ecx
        mov eax, dword ptr [ebp - 0x48]
        mul ecx
        add dword ptr [ebp - 0x24], eax
        mov eax, dword ptr [ebp - 0x44]
        mul ecx
        add dword ptr [ebp - 0x20], eax
L486b9e:
        mov esi, edi
        shl esi, 1
        add esi, dword ptr [ebp - 0x6c]
        mov eax, edi
        mov edi, dword ptr [ViewportRight]
        shl edi, 1
        add edi, dword ptr [ebp - 0x6c]
        cmp eax, dword ptr [ebp - 0x1c]
        jge L486c13
        cmp esi, edi
        jg L486c13
L486bbb:
        mov ebx, dword ptr [ZBuffer]
        mov ecx, dword ptr [ebp + 0x10]
        lea ebx, [ebx + eax*4]
        shl ecx, 9
        mov edx, dword ptr [ebp - 0x20]
        cmp edx, dword ptr [ebx + ecx]
        jb L486bf8
        mov dword ptr [ebx + ecx], edx
        mov ebx, dword ptr [CurrentColorRamp]
        mov ebx, dword ptr [ebx + 4]
        cmp esi, dword ptr [MousePixelAddress]
        movzx edx, word ptr [ebp - 0x22]
        sete cl
        movzx edx, word ptr [ebx + edx*2]
        or byte ptr [MousePixelCovered], cl
        mov word ptr [esi], dx
L486bf8:
        inc eax
        mov ebx, dword ptr [ebp - 0x48]
        add dword ptr [ebp - 0x24], ebx
        mov ebx, dword ptr [ebp - 0x44]
        add dword ptr [ebp - 0x20], ebx
        add esi, 2
        cmp eax, dword ptr [ebp - 0x1c]
        jge L486c13
        cmp esi, edi
        jg L486c13
        jmp L486bbb
L486c13:
        pop edi
        pop esi
        mov eax, dword ptr [ebp - 0x50]
        mov ecx, dword ptr [ebp + 8]
        mov edx, dword ptr [ebp - 0x34]
        mov esi, dword ptr [ebp + 0xc]
        add ecx, eax
        mov eax, dword ptr [ebp - 4]
        mov edi, dword ptr [ebp - 0xc]
        mov dword ptr [ebp + 8], ecx
        mov ecx, dword ptr [ebp - 0x38]
        mov ebx, dword ptr [ebp - 8]
        add eax, ecx
        mov ecx, dword ptr [ebp - 0x58]
        add esi, edx
        mov edx, dword ptr [ebp - 0x54]
        mov dword ptr [ebp - 4], eax
        mov eax, dword ptr [ebp - 0x3c]
        mov dword ptr [ebp + 0xc], esi
        mov esi, dword ptr [ebp - 0x10]
        add edi, eax
        mov eax, dword ptr [ebp + 0x10]
        add ebx, edx
        add esi, ecx
        mov ecx, dword ptr [ebp - 0x28]
        inc eax
        cmp eax, ecx
        mov dword ptr [ebp - 8], ebx
        mov dword ptr [ebp - 0xc], edi
        mov dword ptr [ebp - 0x10], esi
        mov dword ptr [ebp + 0x10], eax
        jl L486aac
L486c69:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Textured twin of DrawGouraudRampTriangle: samples CurrentTexture and lights it with interpolated vertex
 * intensities. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00486c70
__declspec(naked) void DrawGouraudTexturedTriangle(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0xac
        mov edx, dword ptr [ebp + 8]
        mov eax, dword ptr [ebp + 0xc]
        push ebx
        push esi
        mov ecx, dword ptr [edx + 4]
        mov esi, dword ptr [eax + 4]
        cmp ecx, esi
        push edi
        mov dword ptr [ebp - 0x18], 0x47800000
        jle L486ca2
        mov eax, dword ptr [ebp + 8]
        xchg dword ptr [ebp + 0xc], eax
        mov dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp + 0xc]
        mov edx, dword ptr [ebp + 8]
L486ca2:
        mov ebx, dword ptr [ebp + 0x10]
        mov ecx, dword ptr [eax + 4]
        cmp ecx, dword ptr [ebx + 4]
        jle L486cbc
        mov eax, dword ptr [ebp + 0xc]
        xchg dword ptr [ebp + 0x10], eax
        mov dword ptr [ebp + 0xc], eax
        mov ebx, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ebp + 0xc]
L486cbc:
        mov ecx, dword ptr [edx + 4]
        mov esi, dword ptr [eax + 4]
        cmp ecx, esi
        jle L486cd5
        mov eax, dword ptr [ebp + 8]
        xchg dword ptr [ebp + 0xc], eax
        mov dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp + 0xc]
        mov edx, dword ptr [ebp + 8]
L486cd5:
        mov ecx, dword ptr [ebx + 4]
        mov esi, dword ptr [edx + 4]
        sar ecx, 0x10
        mov dword ptr [ebp - 0x40], ecx
        mov ecx, dword ptr [Render3DTargetPitch]
        sar esi, 0x10
        imul ecx, esi
        add ecx, dword ptr [Render3DTargetBits]
        mov edi, dword ptr [eax + 4]
        sar edi, 0x10
        mov dword ptr [ebp - 0x3c], ecx
        mov ecx, dword ptr [edx]
        mov dword ptr [ebp - 0x2c], ecx
        mov ecx, dword ptr [eax]
        mov dword ptr [ebp - 0xa0], ecx
        mov ecx, dword ptr [ebx]
        mov dword ptr [ebp - 0x9c], ecx
        mov ecx, dword ptr [edx + 8]
        mov dword ptr [ebp - 0x24], ecx
        mov ecx, dword ptr [eax + 8]
        mov dword ptr [ebp - 0xa4], ecx
        mov ecx, dword ptr [ebx + 8]
        mov dword ptr [ebp - 0x38], esi
        mov dword ptr [ebp - 0x80], edi
        mov dword ptr [ebp - 0xa8], ecx
        mov ecx, dword ptr [ebp + 8]
        fld dword ptr [ecx + 0xc]
        fmul dword ptr [ebp - 0x18]
        fistp dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp + 0xc]
        fld dword ptr [ecx + 0xc]
        fmul dword ptr [ebp - 0x18]
        fistp dword ptr [ebp - 0x54]
        mov ecx, dword ptr [ebp + 0x10]
        fld dword ptr [ecx + 0xc]
        fmul dword ptr [ebp - 0x18]
        fistp dword ptr [ebp - 0x5c]
        mov ecx, dword ptr [ebp + 8]
        fld dword ptr [ecx + 0x10]
        fmul dword ptr [ebp - 0x18]
        fistp dword ptr [ebp - 0xc]
        mov ecx, dword ptr [ebp + 0xc]
        fld dword ptr [ecx + 0x10]
        fmul dword ptr [ebp - 0x18]
        fistp dword ptr [ebp - 0x64]
        mov ecx, dword ptr [ebp + 0x10]
        fld dword ptr [ecx + 0x10]
        fmul dword ptr [ebp - 0x18]
        fistp dword ptr [ebp - 0x6c]
        mov edx, dword ptr [edx + 0x14]
        mov eax, dword ptr [eax + 0x14]
        mov ecx, dword ptr [ebx + 0x14]
        mov dword ptr [ebp - 8], edx
        mov dword ptr [ebp - 0x48], eax
        mov dword ptr [ebp - 0x70], ecx
        mov ebx, dword ptr [CurrentTexture]
        mov ecx, dword ptr [ebx]
        mov edx, dword ptr [ebp - 4]
        and edx, 0xffff
        shl edx, cl
        mov dword ptr [ebp - 4], edx
        mov ecx, dword ptr [ebx + 4]
        mov eax, dword ptr [ebp - 0xc]
        and eax, 0xffff
        shl eax, cl
        mov dword ptr [ebp - 0xc], eax
        shl dword ptr [ebp - 8], 6
        mov ebx, dword ptr [CurrentTexture]
        mov ecx, dword ptr [ebx]
        mov edx, dword ptr [ebp - 0x54]
        and edx, 0xffff
        shl edx, cl
        mov dword ptr [ebp - 0x54], edx
        mov ecx, dword ptr [ebx + 4]
        mov eax, dword ptr [ebp - 0x64]
        and eax, 0xffff
        shl eax, cl
        mov dword ptr [ebp - 0x64], eax
        shl dword ptr [ebp - 0x48], 6
        mov ebx, dword ptr [CurrentTexture]
        mov ecx, dword ptr [ebx]
        mov edx, dword ptr [ebp - 0x5c]
        and edx, 0xffff
        shl edx, cl
        mov dword ptr [ebp - 0x5c], edx
        mov ecx, dword ptr [ebx + 4]
        mov eax, dword ptr [ebp - 0x6c]
        and eax, 0xffff
        shl eax, cl
        mov dword ptr [ebp - 0x6c], eax
        shl dword ptr [ebp - 0x70], 6
        cmp esi, edi
        mov dword ptr [ebp - 0x14], esi
        je L487300
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x80]
        sub ecx, dword ptr [ebp - 0x38]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 8], eax
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x40]
        sub ecx, dword ptr [ebp - 0x38]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0xa0]
        sub eax, dword ptr [ebp - 0x2c]
        mov ecx, dword ptr [ebp + 8]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x4c], eax
        mov eax, dword ptr [ebp - 0x9c]
        sub eax, dword ptr [ebp - 0x2c]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x98], eax
        mov eax, dword ptr [ebp - 0x54]
        sub eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp + 8]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x50], eax
        mov eax, dword ptr [ebp - 0x5c]
        sub eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x94], eax
        mov eax, dword ptr [ebp - 0x64]
        sub eax, dword ptr [ebp - 0xc]
        mov ecx, dword ptr [ebp + 8]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x58], eax
        mov eax, dword ptr [ebp - 0x6c]
        sub eax, dword ptr [ebp - 0xc]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x78], eax
        mov eax, dword ptr [ebp - 0x48]
        sub eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp + 8]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x60], eax
        mov eax, dword ptr [ebp - 0x70]
        sub eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x7c], eax
        mov eax, dword ptr [ebp - 0xa4]
        sub eax, dword ptr [ebp - 0x24]
        mov ecx, dword ptr [ebp + 8]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x68], eax
        mov eax, dword ptr [ebp - 0xa8]
        sub eax, dword ptr [ebp - 0x24]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x38], eax
        mov eax, dword ptr [ebp - 0x2c]
        mov dword ptr [ebp + 8], eax
        mov dword ptr [ebp + 0x10], eax
        mov eax, dword ptr [ebp - 4]
        mov dword ptr [ebp - 0x28], eax
        mov dword ptr [ebp - 0x1c], eax
        mov eax, dword ptr [ebp - 0xc]
        mov dword ptr [ebp - 0x30], eax
        mov dword ptr [ebp - 0x34], eax
        mov eax, dword ptr [ebp - 8]
        mov dword ptr [ebp - 0x20], eax
        mov dword ptr [ebp - 8], eax
        mov eax, dword ptr [ebp - 0x24]
        mov dword ptr [ebp - 0xc], eax
        mov dword ptr [ebp - 4], eax
        mov ebx, dword ptr [ViewportTop]
        mov ecx, dword ptr [ebp - 0x14]
        mov edx, dword ptr [ebp - 0x80]
        sub ebx, ecx
        jle L486fa8
        sub edx, ecx
        jle L486fa8
        cmp ebx, edx
        mov ecx, edx
        jns L486f45
        mov ecx, ebx
L486f45:
        add dword ptr [ebp - 0x14], ecx
        mov eax, dword ptr [ebp - 0x4c]
        mul ecx
        add dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp - 0x98]
        mul ecx
        add dword ptr [ebp + 0x10], eax
        mov eax, dword ptr [ebp - 0x60]
        mul ecx
        add dword ptr [ebp - 0x20], eax
        mov eax, dword ptr [ebp - 0x7c]
        mul ecx
        add dword ptr [ebp - 8], eax
        mov eax, dword ptr [ebp - 0x50]
        mul ecx
        add dword ptr [ebp - 0x28], eax
        mov eax, dword ptr [ebp - 0x94]
        mul ecx
        add dword ptr [ebp - 0x1c], eax
        mov eax, dword ptr [ebp - 0x58]
        mul ecx
        add dword ptr [ebp - 0x30], eax
        mov eax, dword ptr [ebp - 0x78]
        mul ecx
        add dword ptr [ebp - 0x34], eax
        mov eax, dword ptr [ebp - 0x68]
        mul ecx
        add dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp - 0x38]
        mul ecx
        add dword ptr [ebp - 4], eax
        mov eax, dword ptr [Render3DTargetPitch]
        mul ecx
        add dword ptr [ebp - 0x3c], eax
L486fa8:
        cmp dword ptr [ebp - 0x14], edi
        jge L487280
L486fb1:
        mov edx, dword ptr [ebp - 0x14]
        mov eax, dword ptr [ViewportBottom]
        cmp edx, eax
        jg L487280
        mov eax, dword ptr [ebp - 0x3c]
        mov ecx, dword ptr [Render3DTargetPitch]
        mov esi, dword ptr [ebp + 8]
        mov edi, dword ptr [ebp + 0x10]
        mov dword ptr [ebp - 0xac], eax
        add eax, ecx
        cmp esi, edi
        mov dword ptr [ebp - 0x3c], eax
        jle L487072
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 8]
        sub ecx, dword ptr [ebp + 0x10]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x28]
        mov ecx, dword ptr [ebp - 0x1c]
        sub eax, ecx
        mov dword ptr [ebp - 0x2c], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x74], eax
        mov eax, dword ptr [ebp - 0x30]
        mov ecx, dword ptr [ebp - 0x34]
        sub eax, ecx
        mov dword ptr [ebp - 0x24], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x8c], eax
        mov eax, dword ptr [ebp - 0x20]
        mov ecx, dword ptr [ebp - 8]
        sub eax, ecx
        mov dword ptr [ebp - 0x18], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x90], eax
        mov eax, dword ptr [ebp - 0xc]
        mov ecx, dword ptr [ebp - 4]
        sub eax, ecx
        mov dword ptr [ebp - 0x44], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x84], eax
        sar edi, 0x10
        sar esi, 0x10
        mov dword ptr [ebp - 0x88], edi
        mov dword ptr [ebp - 0x10], esi
        jmp L4870fc
L487072:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 0x10]
        sub ecx, dword ptr [ebp + 8]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x1c]
        mov ecx, dword ptr [ebp - 0x28]
        sub eax, ecx
        mov dword ptr [ebp - 0x2c], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x74], eax
        mov eax, dword ptr [ebp - 0x34]
        mov ecx, dword ptr [ebp - 0x30]
        sub eax, ecx
        mov dword ptr [ebp - 0x24], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x8c], eax
        mov eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp - 0x20]
        sub eax, ecx
        mov dword ptr [ebp - 0x18], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x90], eax
        mov eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp - 0xc]
        sub eax, ecx
        mov dword ptr [ebp - 0x44], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x84], eax
        sar esi, 0x10
        sar edi, 0x10
        mov dword ptr [ebp - 0x88], esi
        mov dword ptr [ebp - 0x10], edi
L4870fc:
        push esi
        push edi
        mov ecx, dword ptr [ebp - 0x88]
        mov edi, ecx
        mov ebx, dword ptr [ViewportLeft]
        sub ecx, ebx
        jns L48713d
        mov edi, ebx
        neg ecx
        mov eax, dword ptr [ebp - 0x90]
        mul ecx
        add dword ptr [ebp - 0x18], eax
        mov eax, dword ptr [ebp - 0x74]
        mul ecx
        add dword ptr [ebp - 0x2c], eax
        mov eax, dword ptr [ebp - 0x8c]
        mul ecx
        add dword ptr [ebp - 0x24], eax
        mov eax, dword ptr [ebp - 0x84]
        mul ecx
        add dword ptr [ebp - 0x44], eax
L48713d:
        mov esi, edi
        shl esi, 1
        add esi, dword ptr [ebp - 0xac]
        mov eax, edi
        mov edi, dword ptr [ViewportRight]
        shl edi, 1
        add edi, dword ptr [ebp - 0xac]
        cmp eax, dword ptr [ebp - 0x10]
        jge L4871f8
        cmp esi, edi
        jg L4871f8
L487168:
        mov ebx, dword ptr [ZBuffer]
        mov ecx, dword ptr [ebp - 0x14]
        lea ebx, [ebx + eax*4]
        shl ecx, 9
        mov edx, dword ptr [ebp - 0x44]
        cmp edx, dword ptr [ebx + ecx]
        jb L4871c5
        mov dword ptr [ebx + ecx], edx
        mov ebx, dword ptr [CurrentTexture]
        mov ecx, dword ptr [ebx]
        movzx edx, word ptr [ebp - 0x22]
        and edx, dword ptr [ebx + 0x10]
        shl edx, cl
        add edx, dword ptr [ebx + 8]
        movzx ecx, word ptr [ebp - 0x2a]
        and ecx, dword ptr [ebx + 0x14]
        mov ebx, dword ptr [ebx + 0xc]
        add edx, ecx
        movzx edx, byte ptr [edx]
        mov ebx, dword ptr [ebx + edx*4]
        mov ebx, dword ptr [ebx + 4]
        cmp esi, dword ptr [MousePixelAddress]
        movzx edx, word ptr [ebp - 0x16]
        sete cl
        movzx edx, word ptr [ebx + edx*2]
        or byte ptr [MousePixelCovered], cl
        mov word ptr [esi], dx
L4871c5:
        inc eax
        mov ebx, dword ptr [ebp - 0x90]
        add dword ptr [ebp - 0x18], ebx
        mov ebx, dword ptr [ebp - 0x74]
        add dword ptr [ebp - 0x2c], ebx
        mov ebx, dword ptr [ebp - 0x8c]
        add dword ptr [ebp - 0x24], ebx
        mov ebx, dword ptr [ebp - 0x84]
        add dword ptr [ebp - 0x44], ebx
        add esi, 2
        cmp eax, dword ptr [ebp - 0x10]
        jge L4871f8
        cmp esi, edi
        jg L4871f8
        jmp L487168
L4871f8:
        pop edi
        pop esi
        mov ecx, dword ptr [ebp - 0x98]
        mov edx, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ebp - 0x4c]
        mov esi, dword ptr [ebp + 8]
        add edx, ecx
        mov ebx, dword ptr [ebp - 0x1c]
        mov ecx, dword ptr [ebp - 0x58]
        mov edi, dword ptr [ebp - 0x30]
        add esi, eax
        mov eax, dword ptr [ebp - 0x28]
        mov dword ptr [ebp + 0x10], edx
        mov edx, dword ptr [ebp - 0x50]
        add eax, edx
        mov edx, dword ptr [ebp - 0x78]
        mov dword ptr [ebp - 0x28], eax
        mov eax, dword ptr [ebp - 0x94]
        mov dword ptr [ebp + 8], esi
        mov esi, dword ptr [ebp - 0x34]
        add ebx, eax
        mov eax, dword ptr [ebp - 0x60]
        add edi, ecx
        mov ecx, dword ptr [ebp - 0x20]
        add esi, edx
        mov edx, dword ptr [ebp - 0x68]
        mov dword ptr [ebp - 0x34], esi
        mov esi, dword ptr [ebp - 4]
        add ecx, eax
        mov eax, dword ptr [ebp - 0x38]
        mov dword ptr [ebp - 0x1c], ebx
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 0x30], edi
        mov edi, dword ptr [ebp - 0xc]
        mov dword ptr [ebp - 0x20], ecx
        mov ecx, dword ptr [ebp - 0x7c]
        add esi, eax
        mov eax, dword ptr [ebp - 0x14]
        add ebx, ecx
        mov ecx, dword ptr [ebp - 0x80]
        add edi, edx
        inc eax
        cmp eax, ecx
        mov dword ptr [ebp - 8], ebx
        mov dword ptr [ebp - 0xc], edi
        mov dword ptr [ebp - 4], esi
        mov dword ptr [ebp - 0x14], eax
        jl L486fb1
L487280:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x40]
        sub ecx, dword ptr [ebp - 0x80]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp - 0x9c]
        sub eax, dword ptr [ebp - 0xa0]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x4c], eax
        mov eax, dword ptr [ebp - 0x5c]
        sub eax, dword ptr [ebp - 0x54]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x50], eax
        mov eax, dword ptr [ebp - 0x6c]
        sub eax, dword ptr [ebp - 0x64]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x58], eax
        mov eax, dword ptr [ebp - 0x70]
        sub eax, dword ptr [ebp - 0x48]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x60], eax
        mov eax, dword ptr [ebp - 0xa8]
        sub eax, dword ptr [ebp - 0xa4]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x68], eax
        mov esi, dword ptr [ebp - 0x14]
        jmp L487432
L487300:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x40]
        sub ecx, dword ptr [ebp - 0x38]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x40]
        sub ecx, dword ptr [ebp - 0x80]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp - 0x9c]
        sub eax, dword ptr [ebp - 0x2c]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x4c], eax
        mov eax, dword ptr [ebp - 0x9c]
        sub eax, dword ptr [ebp - 0xa0]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x98], eax
        mov eax, dword ptr [ebp - 0x5c]
        sub eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x50], eax
        mov eax, dword ptr [ebp - 0x5c]
        sub eax, dword ptr [ebp - 0x54]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x94], eax
        mov eax, dword ptr [ebp - 0x6c]
        sub eax, dword ptr [ebp - 0xc]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x58], eax
        mov eax, dword ptr [ebp - 0x6c]
        sub eax, dword ptr [ebp - 0x64]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x78], eax
        mov eax, dword ptr [ebp - 0x70]
        sub eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x60], eax
        mov eax, dword ptr [ebp - 0x70]
        sub eax, dword ptr [ebp - 0x48]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x7c], eax
        mov eax, dword ptr [ebp - 0xa8]
        sub eax, dword ptr [ebp - 0x24]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x68], eax
        mov eax, dword ptr [ebp - 0xa8]
        sub eax, dword ptr [ebp - 0xa4]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x38], eax
        mov ecx, dword ptr [ebp - 0x2c]
        mov edx, dword ptr [ebp - 0xa0]
        mov eax, dword ptr [ebp - 4]
        mov dword ptr [ebp + 8], ecx
        mov ecx, dword ptr [ebp - 0x54]
        mov dword ptr [ebp + 0x10], edx
        mov edx, dword ptr [ebp - 0xc]
        mov dword ptr [ebp - 0x28], eax
        mov eax, dword ptr [ebp - 0x64]
        mov dword ptr [ebp - 0x1c], ecx
        mov ecx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 0x30], edx
        mov edx, dword ptr [ebp - 0x48]
        mov dword ptr [ebp - 0x34], eax
        mov eax, dword ptr [ebp - 0x24]
        mov dword ptr [ebp - 0x20], ecx
        mov ecx, dword ptr [ebp - 0xa4]
        mov dword ptr [ebp - 8], edx
        mov dword ptr [ebp - 0xc], eax
        mov dword ptr [ebp - 4], ecx
L487432:
        cmp esi, dword ptr [ViewportBottom]
        jg L4877a0
        mov edi, dword ptr [ebp - 0x40]
        cmp esi, edi
        jge L4877a0
        mov ebx, dword ptr [ViewportTop]
        mov ecx, dword ptr [ebp - 0x14]
        mov edx, dword ptr [ebp - 0x40]
        sub ebx, ecx
        jle L4874c8
        sub edx, ecx
        jle L4874c8
        cmp ebx, edx
        mov ecx, edx
        jns L487465
        mov ecx, ebx
L487465:
        add dword ptr [ebp - 0x14], ecx
        mov eax, dword ptr [ebp - 0x4c]
        mul ecx
        add dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp - 0x98]
        mul ecx
        add dword ptr [ebp + 0x10], eax
        mov eax, dword ptr [ebp - 0x60]
        mul ecx
        add dword ptr [ebp - 0x20], eax
        mov eax, dword ptr [ebp - 0x7c]
        mul ecx
        add dword ptr [ebp - 8], eax
        mov eax, dword ptr [ebp - 0x50]
        mul ecx
        add dword ptr [ebp - 0x28], eax
        mov eax, dword ptr [ebp - 0x94]
        mul ecx
        add dword ptr [ebp - 0x1c], eax
        mov eax, dword ptr [ebp - 0x58]
        mul ecx
        add dword ptr [ebp - 0x30], eax
        mov eax, dword ptr [ebp - 0x78]
        mul ecx
        add dword ptr [ebp - 0x34], eax
        mov eax, dword ptr [ebp - 0x68]
        mul ecx
        add dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp - 0x38]
        mul ecx
        add dword ptr [ebp - 4], eax
        mov eax, dword ptr [Render3DTargetPitch]
        mul ecx
        add dword ptr [ebp - 0x3c], eax
L4874c8:
        cmp dword ptr [ebp - 0x14], edi
        jge L4877a0
L4874d1:
        mov edx, dword ptr [ebp - 0x14]
        mov eax, dword ptr [ViewportBottom]
        cmp edx, eax
        jg L4877a0
        mov eax, dword ptr [ebp - 0x3c]
        mov ecx, dword ptr [Render3DTargetPitch]
        mov esi, dword ptr [ebp + 8]
        mov edi, dword ptr [ebp + 0x10]
        mov dword ptr [ebp - 0xac], eax
        add eax, ecx
        cmp esi, edi
        mov dword ptr [ebp - 0x3c], eax
        jle L487592
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 8]
        sub ecx, dword ptr [ebp + 0x10]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x28]
        mov ecx, dword ptr [ebp - 0x1c]
        sub eax, ecx
        mov dword ptr [ebp - 0x2c], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x74], eax
        mov eax, dword ptr [ebp - 0x30]
        mov ecx, dword ptr [ebp - 0x34]
        sub eax, ecx
        mov dword ptr [ebp - 0x24], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x8c], eax
        mov eax, dword ptr [ebp - 0x20]
        mov ecx, dword ptr [ebp - 8]
        sub eax, ecx
        mov dword ptr [ebp - 0x18], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x90], eax
        mov eax, dword ptr [ebp - 0xc]
        mov ecx, dword ptr [ebp - 4]
        sub eax, ecx
        mov dword ptr [ebp - 0x44], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x84], eax
        sar edi, 0x10
        sar esi, 0x10
        mov dword ptr [ebp - 0x88], edi
        mov dword ptr [ebp - 0x10], esi
        jmp L48761c
L487592:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 0x10]
        sub ecx, dword ptr [ebp + 8]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x1c]
        mov ecx, dword ptr [ebp - 0x28]
        sub eax, ecx
        mov dword ptr [ebp - 0x2c], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x74], eax
        mov eax, dword ptr [ebp - 0x34]
        mov ecx, dword ptr [ebp - 0x30]
        sub eax, ecx
        mov dword ptr [ebp - 0x24], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x8c], eax
        mov eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp - 0x20]
        sub eax, ecx
        mov dword ptr [ebp - 0x18], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x90], eax
        mov eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp - 0xc]
        sub eax, ecx
        mov dword ptr [ebp - 0x44], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x84], eax
        sar esi, 0x10
        sar edi, 0x10
        mov dword ptr [ebp - 0x88], esi
        mov dword ptr [ebp - 0x10], edi
L48761c:
        push esi
        push edi
        mov ecx, dword ptr [ebp - 0x88]
        mov edi, ecx
        mov ebx, dword ptr [ViewportLeft]
        sub ecx, ebx
        jns L48765d
        mov edi, ebx
        neg ecx
        mov eax, dword ptr [ebp - 0x90]
        mul ecx
        add dword ptr [ebp - 0x18], eax
        mov eax, dword ptr [ebp - 0x74]
        mul ecx
        add dword ptr [ebp - 0x2c], eax
        mov eax, dword ptr [ebp - 0x8c]
        mul ecx
        add dword ptr [ebp - 0x24], eax
        mov eax, dword ptr [ebp - 0x84]
        mul ecx
        add dword ptr [ebp - 0x44], eax
L48765d:
        mov esi, edi
        shl esi, 1
        add esi, dword ptr [ebp - 0xac]
        mov eax, edi
        mov edi, dword ptr [ViewportRight]
        shl edi, 1
        add edi, dword ptr [ebp - 0xac]
        cmp eax, dword ptr [ebp - 0x10]
        jge L487718
        cmp esi, edi
        jg L487718
L487688:
        mov ebx, dword ptr [ZBuffer]
        mov ecx, dword ptr [ebp - 0x14]
        lea ebx, [ebx + eax*4]
        shl ecx, 9
        mov edx, dword ptr [ebp - 0x44]
        cmp edx, dword ptr [ebx + ecx]
        jb L4876e5
        mov dword ptr [ebx + ecx], edx
        mov ebx, dword ptr [CurrentTexture]
        mov ecx, dword ptr [ebx]
        movzx edx, word ptr [ebp - 0x22]
        and edx, dword ptr [ebx + 0x10]
        shl edx, cl
        add edx, dword ptr [ebx + 8]
        movzx ecx, word ptr [ebp - 0x2a]
        and ecx, dword ptr [ebx + 0x14]
        mov ebx, dword ptr [ebx + 0xc]
        add edx, ecx
        movzx edx, byte ptr [edx]
        mov ebx, dword ptr [ebx + edx*4]
        mov ebx, dword ptr [ebx + 4]
        cmp esi, dword ptr [MousePixelAddress]
        movzx edx, word ptr [ebp - 0x16]
        sete cl
        movzx edx, word ptr [ebx + edx*2]
        or byte ptr [MousePixelCovered], cl
        mov word ptr [esi], dx
L4876e5:
        inc eax
        mov ebx, dword ptr [ebp - 0x90]
        add dword ptr [ebp - 0x18], ebx
        mov ebx, dword ptr [ebp - 0x74]
        add dword ptr [ebp - 0x2c], ebx
        mov ebx, dword ptr [ebp - 0x8c]
        add dword ptr [ebp - 0x24], ebx
        mov ebx, dword ptr [ebp - 0x84]
        add dword ptr [ebp - 0x44], ebx
        add esi, 2
        cmp eax, dword ptr [ebp - 0x10]
        jge L487718
        cmp esi, edi
        jg L487718
        jmp L487688
L487718:
        pop edi
        pop esi
        mov ecx, dword ptr [ebp - 0x98]
        mov edx, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ebp - 0x4c]
        mov esi, dword ptr [ebp + 8]
        add edx, ecx
        mov ebx, dword ptr [ebp - 0x1c]
        mov ecx, dword ptr [ebp - 0x58]
        mov edi, dword ptr [ebp - 0x30]
        add esi, eax
        mov eax, dword ptr [ebp - 0x28]
        mov dword ptr [ebp + 0x10], edx
        mov edx, dword ptr [ebp - 0x50]
        add eax, edx
        mov edx, dword ptr [ebp - 0x78]
        mov dword ptr [ebp - 0x28], eax
        mov eax, dword ptr [ebp - 0x94]
        mov dword ptr [ebp + 8], esi
        mov esi, dword ptr [ebp - 0x34]
        add ebx, eax
        mov eax, dword ptr [ebp - 0x60]
        add edi, ecx
        mov ecx, dword ptr [ebp - 0x20]
        add esi, edx
        mov edx, dword ptr [ebp - 0x68]
        mov dword ptr [ebp - 0x34], esi
        mov esi, dword ptr [ebp - 4]
        add ecx, eax
        mov eax, dword ptr [ebp - 0x38]
        mov dword ptr [ebp - 0x1c], ebx
        mov ebx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 0x30], edi
        mov edi, dword ptr [ebp - 0xc]
        mov dword ptr [ebp - 0x20], ecx
        mov ecx, dword ptr [ebp - 0x7c]
        add esi, eax
        mov eax, dword ptr [ebp - 0x14]
        add ebx, ecx
        mov ecx, dword ptr [ebp - 0x40]
        add edi, edx
        inc eax
        cmp eax, ecx
        mov dword ptr [ebp - 8], ebx
        mov dword ptr [ebp - 0xc], edi
        mov dword ptr [ebp - 4], esi
        mov dword ptr [ebp - 0x14], eax
        jl L4874d1
L4877a0:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Flat-shaded twin of DrawGouraudRampTriangle: one ramp colour from the first vertex's intensity. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x004877b0
__declspec(naked) void DrawFlatRampTriangle(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x50
        push ebx
        push esi
        push edi
        mov ebx, dword ptr [CurrentColorRamp]
        mov ebx, dword ptr [ebx + 4]
        mov edx, dword ptr [ebp + 8]
        shl dword ptr [edx + 0x14], 6
        movzx edx, word ptr [edx + 0x16]
        movzx edx, word ptr [ebx + edx*2]
        mov dword ptr [ebp - 0x50], edx
        mov edx, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp + 0xc]
        mov eax, dword ptr [edx + 4]
        mov esi, dword ptr [ecx + 4]
        cmp eax, esi
        jle L4877f3
        mov eax, dword ptr [ebp + 8]
        xchg dword ptr [ebp + 0xc], eax
        mov dword ptr [ebp + 8], eax
        mov ecx, dword ptr [ebp + 0xc]
        mov edx, dword ptr [ebp + 8]
L4877f3:
        mov ebx, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ecx + 4]
        cmp eax, dword ptr [ebx + 4]
        jle L48780d
        mov eax, dword ptr [ebp + 0xc]
        xchg dword ptr [ebp + 0x10], eax
        mov dword ptr [ebp + 0xc], eax
        mov ebx, dword ptr [ebp + 0x10]
        mov ecx, dword ptr [ebp + 0xc]
L48780d:
        mov eax, dword ptr [edx + 4]
        mov esi, dword ptr [ecx + 4]
        cmp eax, esi
        jle L487826
        mov eax, dword ptr [ebp + 8]
        xchg dword ptr [ebp + 0xc], eax
        mov dword ptr [ebp + 8], eax
        mov ecx, dword ptr [ebp + 0xc]
        mov edx, dword ptr [ebp + 8]
L487826:
        mov edi, dword ptr [ebx + 4]
        mov eax, dword ptr [edx + 4]
        sar edi, 0x10
        mov dword ptr [ebp - 0x18], edi
        mov edi, dword ptr [Render3DTargetPitch]
        sar eax, 0x10
        imul edi, eax
        add edi, dword ptr [Render3DTargetBits]
        mov esi, dword ptr [ecx + 4]
        sar esi, 0x10
        mov dword ptr [ebp - 0xc], edi
        mov edi, dword ptr [edx]
        mov dword ptr [ebp + 8], edi
        mov edi, dword ptr [ecx]
        mov ecx, dword ptr [ecx + 8]
        mov dword ptr [ebp - 0x44], edi
        mov edi, dword ptr [ebx]
        cmp eax, esi
        mov dword ptr [ebp - 0x3c], edi
        mov edi, dword ptr [edx + 8]
        mov edx, dword ptr [ebx + 8]
        mov dword ptr [ebp - 0x38], eax
        mov dword ptr [ebp - 0x24], esi
        mov dword ptr [ebp - 0x30], edi
        mov dword ptr [ebp - 0x48], ecx
        mov dword ptr [ebp - 0x40], edx
        mov dword ptr [ebp + 0x10], eax
        je L487ae5
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x24]
        sub ecx, dword ptr [ebp - 0x38]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x4c], eax
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x18]
        sub ecx, dword ptr [ebp - 0x38]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x44]
        sub eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp - 0x4c]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x1c], eax
        mov eax, dword ptr [ebp - 0x3c]
        sub eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x34], eax
        mov eax, dword ptr [ebp - 0x48]
        sub eax, dword ptr [ebp - 0x30]
        mov ecx, dword ptr [ebp - 0x4c]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x20], eax
        mov eax, dword ptr [ebp - 0x40]
        sub eax, dword ptr [ebp - 0x30]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x38], eax
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [ebp - 4], edi
        mov dword ptr [ebp + 0xc], eax
        mov dword ptr [ebp + 8], eax
        mov dword ptr [ebp - 8], edi
        mov ebx, dword ptr [ViewportTop]
        mov ecx, dword ptr [ebp + 0x10]
        mov edx, dword ptr [ebp - 0x24]
        sub ebx, ecx
        jle L487943
        sub edx, ecx
        jle L487943
        cmp ebx, edx
        mov ecx, edx
        jns L487916
        mov ecx, ebx
L487916:
        add dword ptr [ebp + 0x10], ecx
        mov eax, dword ptr [ebp - 0x1c]
        mul ecx
        add dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x34]
        mul ecx
        add dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp - 0x20]
        mul ecx
        add dword ptr [ebp - 4], eax
        mov eax, dword ptr [ebp - 0x38]
        mul ecx
        add dword ptr [ebp - 8], eax
        mov eax, dword ptr [Render3DTargetPitch]
        mul ecx
        add dword ptr [ebp - 0xc], eax
L487943:
        cmp dword ptr [ebp + 0x10], esi
        jge L487aaa
L48794c:
        mov eax, dword ptr [ebp + 0x10]
        mov ecx, dword ptr [ViewportBottom]
        cmp eax, ecx
        jg L487aaa
        mov eax, dword ptr [ebp - 0xc]
        mov ecx, dword ptr [Render3DTargetPitch]
        mov esi, dword ptr [ebp + 0xc]
        mov edi, dword ptr [ebp + 8]
        mov dword ptr [ebp - 0x4c], eax
        add eax, ecx
        cmp esi, edi
        mov dword ptr [ebp - 0xc], eax
        jle L4879b3
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 0xc]
        sub ecx, dword ptr [ebp + 8]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp - 8]
        sub eax, ecx
        mov dword ptr [ebp - 0x14], ecx
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x2c], eax
        sar edi, 0x10
        sar esi, 0x10
        mov dword ptr [ebp - 0x30], edi
        mov dword ptr [ebp - 0x28], esi
        jmp L4879ec
L4879b3:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 8]
        sub ecx, dword ptr [ebp + 0xc]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp - 4]
        sub eax, ecx
        mov dword ptr [ebp - 0x14], ecx
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x2c], eax
        sar esi, 0x10
        sar edi, 0x10
        mov dword ptr [ebp - 0x30], esi
        mov dword ptr [ebp - 0x28], edi
L4879ec:
        push esi
        push edi
        mov ecx, dword ptr [ebp - 0x30]
        mov edi, ecx
        mov ebx, dword ptr [ViewportLeft]
        sub ecx, ebx
        jns L487a09
        mov edi, ebx
        neg ecx
        mov eax, dword ptr [ebp - 0x2c]
        mul ecx
        add dword ptr [ebp - 0x14], eax
L487a09:
        mov esi, edi
        shl esi, 1
        add esi, dword ptr [ebp - 0x4c]
        mov eax, edi
        mov edi, dword ptr [ViewportRight]
        shl edi, 1
        add edi, dword ptr [ebp - 0x4c]
        cmp eax, dword ptr [ebp - 0x28]
        jge L487a6a
        cmp esi, edi
        jg L487a6a
L487a26:
        mov ebx, dword ptr [ZBuffer]
        mov ecx, dword ptr [ebp + 0x10]
        lea ebx, [ebx + eax*4]
        shl ecx, 9
        mov edx, dword ptr [ebp - 0x14]
        cmp edx, dword ptr [ebx + ecx]
        jb L487a55
        cmp esi, dword ptr [MousePixelAddress]
        mov dword ptr [ebx + ecx], edx
        mov edx, dword ptr [ebp - 0x50]
        sete cl
        mov word ptr [esi], dx
        or byte ptr [MousePixelCovered], cl
L487a55:
        inc eax
        mov ebx, dword ptr [ebp - 0x2c]
        add dword ptr [ebp - 0x14], ebx
        add esi, 2
        cmp eax, dword ptr [ebp - 0x28]
        jge L487a6a
        cmp esi, edi
        jg L487a6a
        jmp L487a26
L487a6a:
        pop edi
        pop esi
        mov ecx, dword ptr [ebp - 0x1c]
        mov eax, dword ptr [ebp + 0xc]
        mov edi, dword ptr [ebp - 4]
        mov edx, dword ptr [ebp - 0x34]
        mov ebx, dword ptr [ebp + 8]
        mov esi, dword ptr [ebp - 8]
        add eax, ecx
        mov ecx, dword ptr [ebp - 0x38]
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x20]
        add edi, eax
        mov eax, dword ptr [ebp + 0x10]
        add ebx, edx
        add esi, ecx
        mov ecx, dword ptr [ebp - 0x24]
        inc eax
        cmp eax, ecx
        mov dword ptr [ebp + 8], ebx
        mov dword ptr [ebp - 4], edi
        mov dword ptr [ebp - 8], esi
        mov dword ptr [ebp + 0x10], eax
        jl L48794c
L487aaa:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x18]
        sub ecx, dword ptr [ebp - 0x24]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x28], eax
        mov eax, dword ptr [ebp - 0x3c]
        sub eax, dword ptr [ebp - 0x44]
        mov ecx, dword ptr [ebp - 0x28]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x1c], eax
        mov eax, dword ptr [ebp - 0x40]
        sub eax, dword ptr [ebp - 0x48]
        mov ecx, dword ptr [ebp - 0x28]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x20], eax
        jmp L487b66
L487ae5:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x18]
        sub ecx, dword ptr [ebp - 0x38]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x18]
        sub ecx, dword ptr [ebp - 0x24]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x28], eax
        mov eax, dword ptr [ebp - 0x3c]
        sub eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x1c], eax
        mov eax, dword ptr [ebp - 0x3c]
        sub eax, dword ptr [ebp - 0x44]
        mov ecx, dword ptr [ebp - 0x28]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x34], eax
        mov eax, dword ptr [ebp - 0x40]
        sub eax, dword ptr [ebp - 0x30]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x20], eax
        mov eax, dword ptr [ebp - 0x40]
        sub eax, dword ptr [ebp - 0x48]
        mov ecx, dword ptr [ebp - 0x28]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x38], eax
        mov edx, dword ptr [ebp + 8]
        mov eax, dword ptr [ebp - 0x44]
        mov ecx, dword ptr [ebp - 0x48]
        mov dword ptr [ebp + 0xc], edx
        mov dword ptr [ebp + 8], eax
        mov dword ptr [ebp - 4], edi
        mov dword ptr [ebp - 8], ecx
L487b66:
        mov eax, dword ptr [ebp + 0x10]
        mov ecx, dword ptr [ViewportBottom]
        cmp eax, ecx
        jg L487d31
        mov esi, dword ptr [ebp - 0x18]
        cmp eax, esi
        jge L487d31
        mov ebx, dword ptr [ViewportTop]
        mov ecx, dword ptr [ebp + 0x10]
        mov edx, dword ptr [ebp - 0x18]
        sub ebx, ecx
        jle L487bcb
        sub edx, ecx
        jle L487bcb
        cmp ebx, edx
        mov ecx, edx
        jns L487b9e
        mov ecx, ebx
L487b9e:
        add dword ptr [ebp + 0x10], ecx
        mov eax, dword ptr [ebp - 0x1c]
        mul ecx
        add dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x34]
        mul ecx
        add dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp - 0x20]
        mul ecx
        add dword ptr [ebp - 4], eax
        mov eax, dword ptr [ebp - 0x38]
        mul ecx
        add dword ptr [ebp - 8], eax
        mov eax, dword ptr [Render3DTargetPitch]
        mul ecx
        add dword ptr [ebp - 0xc], eax
L487bcb:
        cmp dword ptr [ebp + 0x10], esi
        jge L487d31
L487bd4:
        mov edx, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ViewportBottom]
        cmp edx, eax
        jg L487d31
        mov eax, dword ptr [ebp - 0xc]
        mov ecx, dword ptr [Render3DTargetPitch]
        mov esi, dword ptr [ebp + 0xc]
        mov edi, dword ptr [ebp + 8]
        mov dword ptr [ebp - 0x4c], eax
        add eax, ecx
        cmp esi, edi
        mov dword ptr [ebp - 0xc], eax
        jle L487c3a
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 0xc]
        sub ecx, dword ptr [ebp + 8]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp - 8]
        sub eax, ecx
        mov dword ptr [ebp - 0x14], ecx
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x2c], eax
        sar edi, 0x10
        sar esi, 0x10
        mov dword ptr [ebp - 0x30], edi
        mov dword ptr [ebp - 0x28], esi
        jmp L487c73
L487c3a:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 8]
        sub ecx, dword ptr [ebp + 0xc]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0x10], eax
        mov eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp - 4]
        sub eax, ecx
        mov dword ptr [ebp - 0x14], ecx
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x2c], eax
        sar esi, 0x10
        sar edi, 0x10
        mov dword ptr [ebp - 0x30], esi
        mov dword ptr [ebp - 0x28], edi
L487c73:
        push esi
        push edi
        mov ecx, dword ptr [ebp - 0x30]
        mov edi, ecx
        mov ebx, dword ptr [ViewportLeft]
        sub ecx, ebx
        jns L487c90
        mov edi, ebx
        neg ecx
        mov eax, dword ptr [ebp - 0x2c]
        mul ecx
        add dword ptr [ebp - 0x14], eax
L487c90:
        mov esi, edi
        shl esi, 1
        add esi, dword ptr [ebp - 0x4c]
        mov eax, edi
        mov edi, dword ptr [ViewportRight]
        shl edi, 1
        add edi, dword ptr [ebp - 0x4c]
        cmp eax, dword ptr [ebp - 0x28]
        jge L487cf1
        cmp esi, edi
        jg L487cf1
L487cad:
        mov ebx, dword ptr [ZBuffer]
        mov ecx, dword ptr [ebp + 0x10]
        lea ebx, [ebx + eax*4]
        shl ecx, 9
        mov edx, dword ptr [ebp - 0x14]
        cmp edx, dword ptr [ebx + ecx]
        jb L487cdc
        cmp esi, dword ptr [MousePixelAddress]
        mov dword ptr [ebx + ecx], edx
        mov edx, dword ptr [ebp - 0x50]
        sete cl
        mov word ptr [esi], dx
        or byte ptr [MousePixelCovered], cl
L487cdc:
        inc eax
        mov ebx, dword ptr [ebp - 0x2c]
        add dword ptr [ebp - 0x14], ebx
        add esi, 2
        cmp eax, dword ptr [ebp - 0x28]
        jge L487cf1
        cmp esi, edi
        jg L487cf1
        jmp L487cad
L487cf1:
        pop edi
        pop esi
        mov eax, dword ptr [ebp - 0x1c]
        mov ecx, dword ptr [ebp + 0xc]
        mov esi, dword ptr [ebp - 8]
        mov ebx, dword ptr [ebp + 8]
        mov edx, dword ptr [ebp - 0x20]
        mov edi, dword ptr [ebp - 4]
        add ecx, eax
        mov eax, dword ptr [ebp - 0x38]
        mov dword ptr [ebp + 0xc], ecx
        mov ecx, dword ptr [ebp - 0x34]
        add esi, eax
        mov eax, dword ptr [ebp + 0x10]
        add ebx, ecx
        mov ecx, dword ptr [ebp - 0x18]
        add edi, edx
        inc eax
        cmp eax, ecx
        mov dword ptr [ebp + 8], ebx
        mov dword ptr [ebp - 4], edi
        mov dword ptr [ebp - 8], esi
        mov dword ptr [ebp + 0x10], eax
        jl L487bd4
L487d31:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

/* Textured triangle lit by the first vertex's intensity only (flat). */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00487d40
__declspec(naked) void DrawFlatTexturedTriangle(void) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x90
        mov ecx, dword ptr [ebp + 8]
        push ebx
        push esi
        push edi
        mov eax, dword ptr [ecx + 0x14]
        mov edx, dword ptr [ecx + 4]
        shl eax, 6
        mov dword ptr [ebp - 0x90], eax
        mov eax, dword ptr [ebp + 0xc]
        mov dword ptr [ebp - 0x14], 0x47800000
        cmp edx, dword ptr [eax + 4]
        jle L487d7c
        mov eax, dword ptr [ebp + 8]
        xchg dword ptr [ebp + 0xc], eax
        mov dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp + 0xc]
        mov ecx, dword ptr [ebp + 8]
L487d7c:
        mov ebx, dword ptr [ebp + 0x10]
        mov edx, dword ptr [eax + 4]
        cmp edx, dword ptr [ebx + 4]
        jle L487d96
        mov eax, dword ptr [ebp + 0xc]
        xchg dword ptr [ebp + 0x10], eax
        mov dword ptr [ebp + 0xc], eax
        mov ebx, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ebp + 0xc]
L487d96:
        mov edx, dword ptr [ecx + 4]
        mov esi, dword ptr [eax + 4]
        cmp edx, esi
        jle L487daf
        mov eax, dword ptr [ebp + 8]
        xchg dword ptr [ebp + 0xc], eax
        mov dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp + 0xc]
        mov ecx, dword ptr [ebp + 8]
L487daf:
        mov edx, dword ptr [ebx + 4]
        mov esi, dword ptr [ecx + 4]
        sar edx, 0x10
        mov dword ptr [ebp - 0x38], edx
        mov edx, dword ptr [Render3DTargetPitch]
        sar esi, 0x10
        imul edx, esi
        add edx, dword ptr [Render3DTargetBits]
        mov edi, dword ptr [eax + 4]
        sar edi, 0x10
        mov dword ptr [ebp - 0x34], edx
        mov edx, dword ptr [ecx]
        mov dword ptr [ebp - 0x28], edx
        mov edx, dword ptr [eax]
        mov ecx, dword ptr [ecx + 8]
        mov dword ptr [ebp - 0x88], edx
        mov edx, dword ptr [ebx]
        mov dword ptr [ebp - 0x30], esi
        mov dword ptr [ebp - 0x7c], edx
        mov edx, dword ptr [eax + 8]
        mov eax, dword ptr [ebx + 8]
        mov dword ptr [ebp - 0x6c], edi
        mov dword ptr [ebp - 0x20], ecx
        mov dword ptr [ebp - 0x80], edx
        mov dword ptr [ebp - 0x84], eax
        mov ecx, dword ptr [ebp + 8]
        fld dword ptr [ecx + 0xc]
        fmul dword ptr [ebp - 0x14]
        fistp dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp + 0xc]
        fld dword ptr [ecx + 0xc]
        fmul dword ptr [ebp - 0x14]
        fistp dword ptr [ebp - 0x58]
        mov ecx, dword ptr [ebp + 0x10]
        fld dword ptr [ecx + 0xc]
        fmul dword ptr [ebp - 0x14]
        fistp dword ptr [ebp - 0x4c]
        mov ecx, dword ptr [ebp + 8]
        fld dword ptr [ecx + 0x10]
        fmul dword ptr [ebp - 0x14]
        fistp dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp + 0xc]
        fld dword ptr [ecx + 0x10]
        fmul dword ptr [ebp - 0x14]
        fistp dword ptr [ebp - 0x54]
        mov ecx, dword ptr [ebp + 0x10]
        fld dword ptr [ecx + 0x10]
        fmul dword ptr [ebp - 0x14]
        fistp dword ptr [ebp - 0x44]
        mov ebx, dword ptr [CurrentTexture]
        mov ecx, dword ptr [ebx]
        mov edx, dword ptr [ebp - 4]
        and edx, 0xffff
        shl edx, cl
        mov dword ptr [ebp - 4], edx
        mov ecx, dword ptr [ebx + 4]
        mov eax, dword ptr [ebp - 8]
        and eax, 0xffff
        shl eax, cl
        mov dword ptr [ebp - 8], eax
        mov ebx, dword ptr [CurrentTexture]
        mov ecx, dword ptr [ebx]
        mov edx, dword ptr [ebp - 0x58]
        and edx, 0xffff
        shl edx, cl
        mov dword ptr [ebp - 0x58], edx
        mov ecx, dword ptr [ebx + 4]
        mov eax, dword ptr [ebp - 0x54]
        and eax, 0xffff
        shl eax, cl
        mov dword ptr [ebp - 0x54], eax
        mov ebx, dword ptr [CurrentTexture]
        mov ecx, dword ptr [ebx]
        mov edx, dword ptr [ebp - 0x4c]
        and edx, 0xffff
        shl edx, cl
        mov dword ptr [ebp - 0x4c], edx
        mov ecx, dword ptr [ebx + 4]
        mov eax, dword ptr [ebp - 0x44]
        and eax, 0xffff
        shl eax, cl
        mov dword ptr [ebp - 0x44], eax
        cmp esi, edi
        mov dword ptr [ebp - 0x10], esi
        je L4882b5
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x6c]
        sub ecx, dword ptr [ebp - 0x30]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 8], eax
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x38]
        sub ecx, dword ptr [ebp - 0x30]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x88]
        sub eax, dword ptr [ebp - 0x28]
        mov ecx, dword ptr [ebp + 8]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x3c], eax
        mov eax, dword ptr [ebp - 0x7c]
        sub eax, dword ptr [ebp - 0x28]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x74], eax
        mov eax, dword ptr [ebp - 0x58]
        sub eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp + 8]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x40], eax
        mov eax, dword ptr [ebp - 0x4c]
        sub eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x68], eax
        mov eax, dword ptr [ebp - 0x54]
        sub eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp + 8]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x48], eax
        mov eax, dword ptr [ebp - 0x44]
        sub eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x70], eax
        mov eax, dword ptr [ebp - 0x80]
        sub eax, dword ptr [ebp - 0x20]
        mov ecx, dword ptr [ebp + 8]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x50], eax
        mov eax, dword ptr [ebp - 0x84]
        sub eax, dword ptr [ebp - 0x20]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x30], eax
        mov eax, dword ptr [ebp - 0x28]
        mov dword ptr [ebp + 8], eax
        mov dword ptr [ebp + 0x10], eax
        mov eax, dword ptr [ebp - 4]
        mov dword ptr [ebp - 0x18], eax
        mov dword ptr [ebp - 0x2c], eax
        mov eax, dword ptr [ebp - 8]
        mov dword ptr [ebp - 0x1c], eax
        mov dword ptr [ebp - 0x24], eax
        mov eax, dword ptr [ebp - 0x20]
        mov dword ptr [ebp - 8], eax
        mov dword ptr [ebp - 4], eax
        mov ebx, dword ptr [ViewportTop]
        mov ecx, dword ptr [ebp - 0x10]
        mov edx, dword ptr [ebp - 0x6c]
        sub ebx, ecx
        jle L48800f
        sub edx, ecx
        jle L48800f
        cmp ebx, edx
        mov ecx, edx
        jns L487fc2
        mov ecx, ebx
L487fc2:
        add dword ptr [ebp - 0x10], ecx
        mov eax, dword ptr [ebp - 0x3c]
        mul ecx
        add dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp - 0x74]
        mul ecx
        add dword ptr [ebp + 0x10], eax
        mov eax, dword ptr [ebp - 0x40]
        mul ecx
        add dword ptr [ebp - 0x18], eax
        mov eax, dword ptr [ebp - 0x68]
        mul ecx
        add dword ptr [ebp - 0x2c], eax
        mov eax, dword ptr [ebp - 0x48]
        mul ecx
        add dword ptr [ebp - 0x1c], eax
        mov eax, dword ptr [ebp - 0x70]
        mul ecx
        add dword ptr [ebp - 0x24], eax
        mov eax, dword ptr [ebp - 0x50]
        mul ecx
        add dword ptr [ebp - 8], eax
        mov eax, dword ptr [ebp - 0x30]
        mul ecx
        add dword ptr [ebp - 4], eax
        mov eax, dword ptr [Render3DTargetPitch]
        mul ecx
        add dword ptr [ebp - 0x34], eax
L48800f:
        cmp dword ptr [ebp - 0x10], edi
        jge L48824d
L488018:
        mov ecx, dword ptr [ebp - 0x10]
        mov eax, dword ptr [ViewportBottom]
        cmp ecx, eax
        jg L48824d
        mov eax, dword ptr [ebp - 0x34]
        mov ecx, dword ptr [Render3DTargetPitch]
        mov esi, dword ptr [ebp + 8]
        mov edi, dword ptr [ebp + 0x10]
        mov dword ptr [ebp - 0x8c], eax
        add eax, ecx
        cmp esi, edi
        mov dword ptr [ebp - 0x34], eax
        jle L4880af
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 8]
        sub ecx, dword ptr [ebp + 0x10]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x18]
        mov ecx, dword ptr [ebp - 0x2c]
        sub eax, ecx
        mov dword ptr [ebp - 0x14], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x78], eax
        mov eax, dword ptr [ebp - 0x1c]
        mov ecx, dword ptr [ebp - 0x24]
        sub eax, ecx
        mov dword ptr [ebp - 0x28], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x64], eax
        mov eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp - 4]
        sub eax, ecx
        mov dword ptr [ebp - 0x20], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x60], eax
        sar edi, 0x10
        sar esi, 0x10
        mov dword ptr [ebp - 0x5c], edi
        mov dword ptr [ebp - 0xc], esi
        jmp L488116
L4880af:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 0x10]
        sub ecx, dword ptr [ebp + 8]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x2c]
        mov ecx, dword ptr [ebp - 0x18]
        sub eax, ecx
        mov dword ptr [ebp - 0x14], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x78], eax
        mov eax, dword ptr [ebp - 0x24]
        mov ecx, dword ptr [ebp - 0x1c]
        sub eax, ecx
        mov dword ptr [ebp - 0x28], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x64], eax
        mov eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp - 8]
        sub eax, ecx
        mov dword ptr [ebp - 0x20], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x60], eax
        sar esi, 0x10
        sar edi, 0x10
        mov dword ptr [ebp - 0x5c], esi
        mov dword ptr [ebp - 0xc], edi
L488116:
        push esi
        push edi
        mov ecx, dword ptr [ebp - 0x5c]
        mov edi, ecx
        mov ebx, dword ptr [ViewportLeft]
        sub ecx, ebx
        jns L488143
        mov edi, ebx
        neg ecx
        mov eax, dword ptr [ebp - 0x78]
        mul ecx
        add dword ptr [ebp - 0x14], eax
        mov eax, dword ptr [ebp - 0x64]
        mul ecx
        add dword ptr [ebp - 0x28], eax
        mov eax, dword ptr [ebp - 0x60]
        mul ecx
        add dword ptr [ebp - 0x20], eax
L488143:
        mov esi, edi
        shl esi, 1
        add esi, dword ptr [ebp - 0x8c]
        mov eax, edi
        mov edi, dword ptr [ViewportRight]
        shl edi, 1
        add edi, dword ptr [ebp - 0x8c]
        cmp eax, dword ptr [ebp - 0xc]
        jge L4881e1
        cmp esi, edi
        jg L4881e1
L488166:
        mov ebx, dword ptr [ZBuffer]
        mov ecx, dword ptr [ebp - 0x10]
        lea ebx, [ebx + eax*4]
        shl ecx, 9
        mov edx, dword ptr [ebp - 0x20]
        cmp edx, dword ptr [ebx + ecx]
        jb L4881c0
        mov dword ptr [ebx + ecx], edx
        mov ebx, dword ptr [CurrentTexture]
        mov ecx, dword ptr [ebx]
        movzx edx, word ptr [ebp - 0x26]
        shl edx, cl
        add edx, dword ptr [ebx + 8]
        mov ebx, dword ptr [ebx + 0xc]
        movzx ecx, word ptr [ebp - 0x12]
        add edx, ecx
        movzx edx, byte ptr [edx]
        mov ebx, dword ptr [ebx + edx*4]
        mov ebx, dword ptr [ebx + 4]
        cmp esi, dword ptr [MousePixelAddress]
        movzx edx, word ptr [ebp - 0x8e]
        sete cl
        movzx edx, word ptr [ebx + edx*2]
        or byte ptr [MousePixelCovered], cl
        mov word ptr [esi], dx
L4881c0:
        inc eax
        mov ebx, dword ptr [ebp - 0x78]
        add dword ptr [ebp - 0x14], ebx
        mov ebx, dword ptr [ebp - 0x64]
        add dword ptr [ebp - 0x28], ebx
        mov ebx, dword ptr [ebp - 0x60]
        add dword ptr [ebp - 0x20], ebx
        add esi, 2
        cmp eax, dword ptr [ebp - 0xc]
        jge L4881e1
        cmp esi, edi
        jg L4881e1
        jmp L488166
L4881e1:
        pop edi
        pop esi
        mov edx, dword ptr [ebp - 0x3c]
        mov eax, dword ptr [ebp + 8]
        mov ebx, dword ptr [ebp + 0x10]
        mov ecx, dword ptr [ebp - 0x40]
        mov edi, dword ptr [ebp - 0x18]
        mov esi, dword ptr [ebp - 0x2c]
        add eax, edx
        mov edx, dword ptr [ebp - 0x68]
        mov dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp - 0x74]
        add ebx, eax
        mov eax, dword ptr [ebp - 0x48]
        add edi, ecx
        mov ecx, dword ptr [ebp - 0x1c]
        add esi, edx
        mov edx, dword ptr [ebp - 0x50]
        mov dword ptr [ebp - 0x2c], esi
        mov esi, dword ptr [ebp - 4]
        add ecx, eax
        mov eax, dword ptr [ebp - 0x30]
        mov dword ptr [ebp + 0x10], ebx
        mov ebx, dword ptr [ebp - 0x24]
        mov dword ptr [ebp - 0x18], edi
        mov edi, dword ptr [ebp - 8]
        mov dword ptr [ebp - 0x1c], ecx
        mov ecx, dword ptr [ebp - 0x70]
        add esi, eax
        mov eax, dword ptr [ebp - 0x10]
        add ebx, ecx
        mov ecx, dword ptr [ebp - 0x6c]
        add edi, edx
        inc eax
        cmp eax, ecx
        mov dword ptr [ebp - 0x24], ebx
        mov dword ptr [ebp - 8], edi
        mov dword ptr [ebp - 4], esi
        mov dword ptr [ebp - 0x10], eax
        jl L488018
L48824d:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x38]
        sub ecx, dword ptr [ebp - 0x6c]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp - 0x7c]
        sub eax, dword ptr [ebp - 0x88]
        mov ecx, dword ptr [ebp - 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x3c], eax
        mov eax, dword ptr [ebp - 0x4c]
        sub eax, dword ptr [ebp - 0x58]
        mov ecx, dword ptr [ebp - 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x40], eax
        mov eax, dword ptr [ebp - 0x44]
        sub eax, dword ptr [ebp - 0x54]
        mov ecx, dword ptr [ebp - 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x48], eax
        mov eax, dword ptr [ebp - 0x84]
        sub eax, dword ptr [ebp - 0x80]
        mov ecx, dword ptr [ebp - 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x50], eax
        mov esi, dword ptr [ebp - 0x10]
        jmp L4883a5
L4882b5:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x38]
        sub ecx, dword ptr [ebp - 0x30]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp - 0x38]
        sub ecx, dword ptr [ebp - 0x6c]
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp - 0x7c]
        sub eax, dword ptr [ebp - 0x28]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x3c], eax
        mov eax, dword ptr [ebp - 0x7c]
        sub eax, dword ptr [ebp - 0x88]
        mov ecx, dword ptr [ebp - 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x74], eax
        mov eax, dword ptr [ebp - 0x4c]
        sub eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x40], eax
        mov eax, dword ptr [ebp - 0x4c]
        sub eax, dword ptr [ebp - 0x58]
        mov ecx, dword ptr [ebp - 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x68], eax
        mov eax, dword ptr [ebp - 0x44]
        sub eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x48], eax
        mov eax, dword ptr [ebp - 0x44]
        sub eax, dword ptr [ebp - 0x54]
        mov ecx, dword ptr [ebp - 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x70], eax
        mov eax, dword ptr [ebp - 0x84]
        sub eax, dword ptr [ebp - 0x20]
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x50], eax
        mov eax, dword ptr [ebp - 0x84]
        sub eax, dword ptr [ebp - 0x80]
        mov ecx, dword ptr [ebp - 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x30], eax
        mov ecx, dword ptr [ebp - 0x28]
        mov edx, dword ptr [ebp - 0x88]
        mov eax, dword ptr [ebp - 4]
        mov dword ptr [ebp + 8], ecx
        mov ecx, dword ptr [ebp - 0x58]
        mov dword ptr [ebp + 0x10], edx
        mov edx, dword ptr [ebp - 8]
        mov dword ptr [ebp - 0x18], eax
        mov eax, dword ptr [ebp - 0x54]
        mov dword ptr [ebp - 0x2c], ecx
        mov ecx, dword ptr [ebp - 0x20]
        mov dword ptr [ebp - 0x1c], edx
        mov edx, dword ptr [ebp - 0x80]
        mov dword ptr [ebp - 0x24], eax
        mov dword ptr [ebp - 8], ecx
        mov dword ptr [ebp - 4], edx
L4883a5:
        cmp esi, dword ptr [ViewportBottom]
        jg L488664
        mov edi, dword ptr [ebp - 0x38]
        cmp esi, edi
        jge L488664
        mov ebx, dword ptr [ViewportTop]
        mov ecx, dword ptr [ebp - 0x10]
        mov edx, dword ptr [ebp - 0x38]
        sub ebx, ecx
        jle L488425
        sub edx, ecx
        jle L488425
        cmp ebx, edx
        mov ecx, edx
        jns L4883d8
        mov ecx, ebx
L4883d8:
        add dword ptr [ebp - 0x10], ecx
        mov eax, dword ptr [ebp - 0x3c]
        mul ecx
        add dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp - 0x74]
        mul ecx
        add dword ptr [ebp + 0x10], eax
        mov eax, dword ptr [ebp - 0x40]
        mul ecx
        add dword ptr [ebp - 0x18], eax
        mov eax, dword ptr [ebp - 0x68]
        mul ecx
        add dword ptr [ebp - 0x2c], eax
        mov eax, dword ptr [ebp - 0x48]
        mul ecx
        add dword ptr [ebp - 0x1c], eax
        mov eax, dword ptr [ebp - 0x70]
        mul ecx
        add dword ptr [ebp - 0x24], eax
        mov eax, dword ptr [ebp - 0x50]
        mul ecx
        add dword ptr [ebp - 8], eax
        mov eax, dword ptr [ebp - 0x30]
        mul ecx
        add dword ptr [ebp - 4], eax
        mov eax, dword ptr [Render3DTargetPitch]
        mul ecx
        add dword ptr [ebp - 0x34], eax
L488425:
        cmp dword ptr [ebp - 0x10], edi
        jge L488664
L48842e:
        mov eax, dword ptr [ebp - 0x10]
        mov ecx, dword ptr [ViewportBottom]
        cmp eax, ecx
        jg L488664
        mov eax, dword ptr [ebp - 0x34]
        mov ecx, dword ptr [Render3DTargetPitch]
        mov esi, dword ptr [ebp + 8]
        mov edi, dword ptr [ebp + 0x10]
        mov dword ptr [ebp - 0x8c], eax
        add eax, ecx
        cmp esi, edi
        mov dword ptr [ebp - 0x34], eax
        jle L4884c6
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 8]
        sub ecx, dword ptr [ebp + 0x10]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x18]
        mov ecx, dword ptr [ebp - 0x2c]
        sub eax, ecx
        mov dword ptr [ebp - 0x14], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x78], eax
        mov eax, dword ptr [ebp - 0x1c]
        mov ecx, dword ptr [ebp - 0x24]
        sub eax, ecx
        mov dword ptr [ebp - 0x28], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x64], eax
        mov eax, dword ptr [ebp - 8]
        mov ecx, dword ptr [ebp - 4]
        sub eax, ecx
        mov dword ptr [ebp - 0x20], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x60], eax
        sar edi, 0x10
        sar esi, 0x10
        mov dword ptr [ebp - 0x5c], edi
        mov dword ptr [ebp - 0xc], esi
        jmp L48852d
L4884c6:
        lea ebx, [ScaledReciprocalTable]
        mov ecx, dword ptr [ebp + 0x10]
        sub ecx, dword ptr [ebp + 8]
        shr ecx, 0x10
        inc ecx
        mov eax, dword ptr [ebx + ecx*4]
        mov dword ptr [ebp + 0xc], eax
        mov eax, dword ptr [ebp - 0x2c]
        mov ecx, dword ptr [ebp - 0x18]
        sub eax, ecx
        mov dword ptr [ebp - 0x14], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x78], eax
        mov eax, dword ptr [ebp - 0x24]
        mov ecx, dword ptr [ebp - 0x1c]
        sub eax, ecx
        mov dword ptr [ebp - 0x28], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x64], eax
        mov eax, dword ptr [ebp - 4]
        mov ecx, dword ptr [ebp - 8]
        sub eax, ecx
        mov dword ptr [ebp - 0x20], ecx
        mov ecx, dword ptr [ebp + 0xc]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x60], eax
        sar esi, 0x10
        sar edi, 0x10
        mov dword ptr [ebp - 0x5c], esi
        mov dword ptr [ebp - 0xc], edi
L48852d:
        push esi
        push edi
        mov ecx, dword ptr [ebp - 0x5c]
        mov edi, ecx
        mov ebx, dword ptr [ViewportLeft]
        sub ecx, ebx
        jns L48855a
        mov edi, ebx
        neg ecx
        mov eax, dword ptr [ebp - 0x78]
        mul ecx
        add dword ptr [ebp - 0x14], eax
        mov eax, dword ptr [ebp - 0x64]
        mul ecx
        add dword ptr [ebp - 0x28], eax
        mov eax, dword ptr [ebp - 0x60]
        mul ecx
        add dword ptr [ebp - 0x20], eax
L48855a:
        mov esi, edi
        shl esi, 1
        add esi, dword ptr [ebp - 0x8c]
        mov eax, edi
        mov edi, dword ptr [ViewportRight]
        shl edi, 1
        add edi, dword ptr [ebp - 0x8c]
        cmp eax, dword ptr [ebp - 0xc]
        jge L4885f8
        cmp esi, edi
        jg L4885f8
L48857d:
        mov ebx, dword ptr [ZBuffer]
        mov ecx, dword ptr [ebp - 0x10]
        lea ebx, [ebx + eax*4]
        shl ecx, 9
        mov edx, dword ptr [ebp - 0x20]
        cmp edx, dword ptr [ebx + ecx]
        jb L4885d7
        mov dword ptr [ebx + ecx], edx
        mov ebx, dword ptr [CurrentTexture]
        mov ecx, dword ptr [ebx]
        movzx edx, word ptr [ebp - 0x26]
        shl edx, cl
        add edx, dword ptr [ebx + 8]
        mov ebx, dword ptr [ebx + 0xc]
        movzx ecx, word ptr [ebp - 0x12]
        add edx, ecx
        movzx edx, byte ptr [edx]
        mov ebx, dword ptr [ebx + edx*4]
        mov ebx, dword ptr [ebx + 4]
        cmp esi, dword ptr [MousePixelAddress]
        movzx edx, word ptr [ebp - 0x8e]
        sete cl
        movzx edx, word ptr [ebx + edx*2]
        or byte ptr [MousePixelCovered], cl
        mov word ptr [esi], dx
L4885d7:
        inc eax
        mov ebx, dword ptr [ebp - 0x78]
        add dword ptr [ebp - 0x14], ebx
        mov ebx, dword ptr [ebp - 0x64]
        add dword ptr [ebp - 0x28], ebx
        mov ebx, dword ptr [ebp - 0x60]
        add dword ptr [ebp - 0x20], ebx
        add esi, 2
        cmp eax, dword ptr [ebp - 0xc]
        jge L4885f8
        cmp esi, edi
        jg L4885f8
        jmp L48857d
L4885f8:
        pop edi
        pop esi
        mov ecx, dword ptr [ebp - 0x3c]
        mov ebx, dword ptr [ebp + 8]
        mov edx, dword ptr [ebp - 0x74]
        mov edi, dword ptr [ebp + 0x10]
        mov eax, dword ptr [ebp - 0x40]
        mov esi, dword ptr [ebp - 0x18]
        add ebx, ecx
        mov ecx, dword ptr [ebp - 0x68]
        add edi, edx
        mov edx, dword ptr [ebp - 0x2c]
        add edx, ecx
        add esi, eax
        mov eax, dword ptr [ebp - 0x1c]
        mov dword ptr [ebp - 0x2c], edx
        mov edx, dword ptr [ebp - 0x48]
        mov ecx, dword ptr [ebp - 0x50]
        add eax, edx
        mov edx, dword ptr [ebp - 0x30]
        mov dword ptr [ebp + 8], ebx
        mov ebx, dword ptr [ebp - 0x24]
        mov dword ptr [ebp - 0x1c], eax
        mov eax, dword ptr [ebp - 0x70]
        mov dword ptr [ebp + 0x10], edi
        mov edi, dword ptr [ebp - 8]
        mov dword ptr [ebp - 0x18], esi
        mov esi, dword ptr [ebp - 4]
        add ebx, eax
        mov eax, dword ptr [ebp - 0x10]
        add edi, ecx
        mov ecx, dword ptr [ebp - 0x38]
        add esi, edx
        inc eax
        cmp eax, ecx
        mov dword ptr [ebp - 0x24], ebx
        mov dword ptr [ebp - 8], edi
        mov dword ptr [ebp - 4], esi
        mov dword ptr [ebp - 0x10], eax
        jl L48842e
L488664:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

// FUNCTION: LEGOLAND 0x00488670
void LoadTextureNode(struct Image *image, unsigned int index) {
    struct TextureNode *ptr = (struct TextureNode *)malloc(sizeof(struct TextureNode));
    if (ptr != 0) {
        BuildTextureNodeFromImage(image, ptr);
        TextureNodes[index] = ptr;
    }
}

// FUNCTION: LEGOLAND 0x004886a0
void FreeTextureNodes(void) {
    struct TextureNode **slot = (struct TextureNode **)&TextureNodes;
    do {
        if (*slot != 0) {
            free((*slot)->data_8);
            free((*slot)->data_c);
            free(*slot);
            *slot = 0;
        }
        slot++;
    } while ((INT_PTR)slot < (INT_PTR)&ZBufferImageSpriteReady);
}

/* SetCurrentTexture for texture slot index (TextureNodes). */
// FUNCTION: LEGOLAND 0x004886e0
void SetCurrentTextureByIndex(unsigned int index) {
    SetCurrentTexture(TextureNodes[index]);
}

/* Starts a mouse hit test for the next draw: clears the covered flag and points the mouse pixel address at point pt
 * (MousePos) in the surface at base, using the 3D target pitch. */
// FUNCTION: LEGOLAND 0x00488700
void StartMousePixelHitTest(unsigned char *base, struct RenderViewport *vp) {
    MousePixelCovered = 0;
    MousePixelAddress = (unsigned short *)(base + (vp->y * Render3DTargetPitch) + (vp->x * 2));
}

/* Returns the 16-bit colour of CurrentTexture at u, v (16.16 fractions) lit by intensity p3 through the texel's
 * colour ramp. Nothing calls it. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00488730
__declspec(naked) unsigned short SampleCurrentTexture(unsigned int p1, unsigned int p2, unsigned int p3) {
    __asm {
        push ebp
        mov ebp, esp
        push ecx
        push ebx
        mov ebx, dword ptr [CurrentTexture]
        mov eax, dword ptr [ebx]
        dec eax
        mov ecx, dword ptr [ebp + 8]
        and ecx, 0xffff
        mul ecx
        shrd eax, edx, 0x10
        mov ecx, eax
        mov eax, dword ptr [ebx + 4]
        dec eax
        mov edx, dword ptr [ebp + 0xc]
        and edx, 0xffff
        mul edx
        shrd eax, edx, 0x10
        mov edx, ecx
        mov ecx, dword ptr [ebx + 8]
        shl eax, cl
        add eax, edx
        mov edx, dword ptr [ebx + 0xc]
        mov ebx, dword ptr [ebx + 0x10]
        movzx edx, byte ptr [edx + eax]
        mov ebx, dword ptr [ebx + edx*4]
        mov ebx, dword ptr [ebx + 4]
        mov eax, dword ptr [ebp + 0x10]
        mov ecx, eax
        shr ecx, 0x11
        sbb eax, 0
        shr eax, 0xa
        movzx eax, word ptr [ebx + eax*2]
        mov dword ptr [ebp - 4], eax
        mov ax, word ptr [ebp - 4]
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

// FUNCTION: LEGOLAND 0x004887a0
void InitZBufferImageSprite(void) {
    ZBufferImageSpriteReady = (void *)1;
    ZBufferSourceImage = CreateSourceImage(EmptyString, 0);
    ZBufferSourceImage->data = ZBufferImagePixels;
    ZBufferSourceImage->width = 0x280;
    ZBufferSourceImage->height = 0x1e0;
    ZBufferSourceImage->refcount = 1;
    ZBufferSourceImage->aux = NULL;
    ZBufferSourceImage->field_14 = 1;
    ZBufferImageSprite = CreateSprite(ZBufferSourceImage);
    ZBufferImageSprite->flags = ZBufferImageSprite->flags | 0x208;
}

/* Returns the top 8 bits of the Z buffer value at (x, y); GenerateNewImageFromZBuffer keeps the pixels whose depth
 * is within its threshold. */
// FUNCTION: LEGOLAND 0x00488820
unsigned int GetZBufferDepth(unsigned int x, unsigned int y) {
    unsigned int *row = (unsigned int *)(ZBuffer + ZBufferStride * y);
    return row[x] >> 0x18;
}

// FUNCTION: LEGOLAND 0x00488840
LEGO_EXPORT struct Sprite *GenerateNewImageFromZBuffer(struct Sprite *sprite, struct Sprite *param_2, int param_3, int param_4, int param_5) {
    int *volatile ptmp19;
    int w = (short)sprite->width;
    int h = (short)sprite->height;
    struct ZBlitDesc local;
    int row;
    unsigned int transp;
    int yy;

    if (!ZBufferImageSpriteReady) {
        InitZBufferImageSprite();
    }
    ZBufferSavedSurfaceDesc = CurrentSurfaceDesc;
    ZBufferSavedSoftPrintClip = SoftPrintClip;
    CurrentSurfaceDesc.lpSurface = ZBufferRenderPixels;
    CurrentSurfaceDesc.dwWidth = w;
    CurrentSurfaceDesc.dwHeight = h;
    CurrentSurfaceDesc.lPitch = 2 * w;
    SoftPrintClip.left = 0;
    SoftPrintClip.right = w;
    SoftPrintClip.top = 0;
    SoftPrintClip.bottom = h;
    local.off[0] = 0;
    local.off[1] = 0;
    local.rect.left = 0;
    local.rect.right = w;
    local.rect.top = 0;
    local.rect.bottom = h;
    StoredTransparentColour = GetTransparentColour();
    SoftPrint_Clear();
    ptmp19 = local.off;
    SoftPrint_BltFast(param_2, &local.rect, ptmp19);
    LoadZBufferFromSprite(param_2, param_4, param_5);
    transp = GetTransparentColour();
    yy = 0;
    while (yy < h) {
        for (row = 0; row < w; row++) {
            unsigned short px;
            if ((int)(GetZBufferDepth(row, yy) & 0xff) <= param_3) {
                px = ((unsigned short *)ZBufferRenderPixels)[yy * w + row];
            } else {
                px = transp;
            }
            ((unsigned short *)ZBufferImagePixels)[yy * w + row] = px;
        }
        yy = 1 + yy;
    }
    ZBufferSourceImage->width = (short)w;
    ZBufferSourceImage->height = (short)h;
    ZBufferImageSprite->width = (short)w;
    ZBufferImageSprite->height = (short)h;
    CurrentSurfaceDesc = ZBufferSavedSurfaceDesc;
    SoftPrintClip = ZBufferSavedSoftPrintClip;
    return ZBufferImageSprite;
}

// FUNCTION: LEGOLAND 0x00488a10
LEGO_EXPORT unsigned int RenderSprite(struct Sprite *sprite, int x, int y) {
    RECT dst;
    RECT src;
    DDBLTFX fx;

    dst.left = x;
    dst.top = y;
    dst.right = (short)sprite->width + x;
    dst.bottom = (short)sprite->height + y;
    src.left = 0;
    src.top = 0;
    src.bottom = (short)sprite->height;
    src.right = (short)sprite->width;
    if ((sprite->flags & 0x60) != 0) {
        if (IntersectRect(&dst, &dst, &SPRITE_ClipRect) != 0) {
            int i;
            unsigned int *p;
            sprite->field_c = FrameCounter;
            src.left = dst.left;
            src.top = dst.top;
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
            if ((sprite->flags & 0x40) != 0) {
                ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Blt((LPDIRECTDRAWSURFACE)renderEngine, &dst, (LPDIRECTDRAWSURFACE)sprite->surface, &src, 0x1008000, &fx);
            } else {
                ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Blt((LPDIRECTDRAWSURFACE)renderEngine, &dst, (LPDIRECTDRAWSURFACE)sprite->surface, &src, 0x1000000, &fx);
            }
            PopRenderingStatus();
            return 1;
        }
    } else {
        if (IntersectRect(&dst, &dst, &SPRITE_ClipRect) != 0) {
            sprite->field_c = FrameCounter;
            src.left = dst.left;
            src.top = dst.top;
            src.right = dst.right;
            src.bottom = dst.bottom;
            OffsetRect(&src, -x, -y);
            SoftPrint_BltFast(sprite, &src, (int *)&dst);
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00488b90
LEGO_EXPORT unsigned int RenderSpriteX(struct Sprite *sprite, int x, int y, unsigned int param_4) {
    RECT dst;
    RECT src;

    dst.left = x;
    dst.top = y;
    dst.right = (short)sprite->width + x;
    dst.bottom = (short)sprite->height + y;
    src.left = 0;
    src.top = 0;
    src.bottom = (short)sprite->height;
    src.right = (short)sprite->width;
    if (IntersectRect(&dst, &dst, &SPRITE_ClipRect) != 0) {
        sprite->field_c = FrameCounter;
        src.left = dst.left;
        src.top = dst.top;
        src.right = dst.right;
        src.bottom = dst.bottom;
        OffsetRect(&src, -x, -y);
        SoftPrint_XBltFast(sprite, &src, &dst, param_4);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00488c50
// Never implemented: it exits before drawing. The Blt after exit(1) is a
// reconstruction; the compiler drops it as unreachable, but because it takes
// &dst the four stores into dst survive, exactly as in the original.
LEGO_EXPORT unsigned int RenderTiledSprite(struct Sprite *sprite, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7) {
    RECT dst;

    dst.left = param_2;
    dst.top = param_3;
    dst.right = param_2 + param_4;
    dst.bottom = param_3 + param_5;
    exit(1);
    return ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Blt((LPDIRECTDRAWSURFACE)renderEngine, &dst, (LPDIRECTDRAWSURFACE)sprite->surface, NULL, 0x1000000, NULL) == 0;
}

// GLOBAL: LEGOLAND 0x004bdd74
static const char ScaledSpriteBreakText[] = "break";

// Draws a sprite stretched to (param_4 + 1) x (param_5 + 1) at (param_2, param_3) moved by param_6: the sprite is
// software-printed whole into a scratch surface (made on the first call: 1280x960, colour-keyed with the transparent
// colour), then stretched onto the render target with a colour-keyed Blt through the game's clipper. The software
// screen (CurrentSurfaceDesc, SoftPrintClip) is pointed at the scratch for the print and restored after. Returns 1
// when the Blt worked (after restoring a lost scratch surface once).
// FUNCTION: LEGOLAND 0x00488c80
unsigned int RenderScaledSpriteOffset(struct Sprite *sprite, int param_2, int param_3, int param_4, int param_5, int *param_6) {
    int off[2];
    RECT src;
    RECT dst;
    RECT clip;
    DDSURFACEDESC desc1;
    RECT r2;
    DDSURFACEDESC desc2;
    HRESULT hr;

    void *bits;
    int x = param_2 + param_6[0];
    int y = param_3 + param_6[1];
    dst.left = x;
    dst.top = y;
    dst.right = x + param_4 + 1;
    dst.bottom = y + param_5 + 1;
    src.top = 0;
    src.left = 0;
    src.right = (short)sprite->width;
    src.bottom = (short)sprite->height;
    if (ScaledSpriteScratchMade == 0) {
        memset(&desc1, 0, sizeof(desc1));
        ScaledSpriteScratchMade = 1;
        desc1.dwSize = sizeof(desc1);
        if (IDirectDrawSurface_GetSurfaceDesc(renderEngine, &desc1) == 0) {
            desc1.dwFlags = 0x1007;
            desc1.ddsCaps.dwCaps = 0x840;
            desc1.dwWidth = 0x500;
            desc1.dwHeight = 0x3c0;
            IDirectDraw2_CreateSurface(DDRAWENV.ddraw2, &desc1, &ScaledSpriteScratch, NULL);
            off[0] = off[1] = GetTransparentColour();
            IDirectDrawSurface_SetColorKey(ScaledSpriteScratch, 8, (LPDDCOLORKEY)off);
        }
    }
    SavedSurfaceDesc = CurrentSurfaceDesc;
    SavedSoftPrintClip = SoftPrintClip;
    memset(&desc2, 0, sizeof(desc2));
    desc2.dwSize = sizeof(desc2);
    if (IDirectDrawSurface_Lock(ScaledSpriteScratch, NULL, &desc2, 0x21, NULL) == 0) {
        clip.left = 0;
        clip.right = desc2.dwWidth;
        clip.top = 0;
        clip.bottom = desc2.dwHeight;
        IntersectRect(&SoftPrintClip, &clip, &SPRITE_ClipRect);
        off[0] = 0;
        off[1] = 0;
        StoredTransparentColour = GetTransparentColour();
        CurrentSurfaceDesc.lpSurface = desc2.lpSurface;
        CurrentSurfaceDesc.dwWidth = (short)sprite->width;
        CurrentSurfaceDesc.dwHeight = (short)sprite->height;
        CurrentSurfaceDesc.lPitch = desc2.lPitch;
        SoftPrint_Clear();
        r2.left = 0;
        r2.right = (short)sprite->width;
        r2.top = 0;
        r2.bottom = (short)sprite->height;
        if ((short)sprite->src_x < 0 || (short)sprite->src_y < 0) {
            printf(ScaledSpriteBreakText);
        }
        SoftPrint_BltFast(sprite, &r2, off);
        bits = desc2.lpSurface;
        IDirectDrawSurface_Unlock(ScaledSpriteScratch, bits);
    }
    IDirectDrawSurface_SetClipper(renderEngine, DDrawClipper);
    hr = IDirectDrawSurface_Blt(renderEngine, &dst, ScaledSpriteScratch, &src, 0x1008000, NULL);
    if (hr == 0) {
        goto ok;
    }
    if (hr == 0x887601c2) {
        if (IDirectDrawSurface_Restore(ScaledSpriteScratch) != 0) {
            IDirectDrawSurface_SetClipper(renderEngine, NULL);
            CurrentSurfaceDesc = SavedSurfaceDesc;
            SoftPrintClip = SavedSoftPrintClip;
            return 0;
        }
        MakeSprite(sprite);
        if (IDirectDrawSurface_IsLost(PrimarySurface) != 0x887601c2 || IDirectDrawSurface_Restore(PrimarySurface) == 0) {
            if (IDirectDrawSurface_Blt(renderEngine, &dst, ScaledSpriteScratch, &src, 0x8000, NULL) == 0) {
            ok:
                IDirectDrawSurface_SetClipper(renderEngine, NULL);
                CurrentSurfaceDesc = SavedSurfaceDesc;
                SoftPrintClip = SavedSoftPrintClip;
                return 1;
            }
        }
    }
    IDirectDrawSurface_SetClipper(renderEngine, NULL);
    CurrentSurfaceDesc = SavedSurfaceDesc;
    SoftPrintClip = SavedSoftPrintClip;
    return 0;
}

// FUNCTION: LEGOLAND 0x00489080
LEGO_EXPORT unsigned int RenderScaledSprite(struct Sprite *param_1, int param_2, int param_3, int param_4, int param_5) {
    int local[2];

    local[0] = 0;
    local[1] = 0;
    RenderScaledSpriteOffset(param_1, param_2, param_3, param_4, param_5, local);
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
    if (IntersectRect(&dst, &dst, &SPRITE_ClipRect) == 0) {
        return 1;
    }
    PushRenderingStatusAndUnlockVideoSurface();
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->SetClipper((LPDIRECTDRAWSURFACE)renderEngine, (LPDIRECTDRAWCLIPPER)DDrawClipper);
    if (((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->Blt((LPDIRECTDRAWSURFACE)renderEngine, &dst, NULL, NULL, 0x1000400, &fx) == 0) {
        ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->SetClipper((LPDIRECTDRAWSURFACE)renderEngine, NULL);
        PopRenderingStatus();
        return 1;
    }
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->SetClipper((LPDIRECTDRAWSURFACE)renderEngine, NULL);
    PopRenderingStatus();
    return 0;
}

/* Lock info filled by GetSprite (NULL locks the screen) and released by ReleaseSprite. */

/* Draws `sprite` at (x, y) blended 50% over the screen, 16-bit modes only: each pair of 5:6:5 pixels is
 * averaged with the mask 0xf7def7de, transparent (zero) pairs are skipped, and every second source row is
 * written to two screen rows. As in the original, the blend reads the next screen pair, not the one it
 * writes. The original does the clipping and blending in an inline __asm block (it even reuses ebp as a
 * loop register); this is the C equivalent: same effect, but it cannot byte-match without __asm. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00489190
__declspec(naked) LEGO_EXPORT int RenderTransSprite(struct Sprite *sprite, int x, int y) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x70
        push ebx
        push esi
        mov esi, dword ptr [ebp + 8]
        push edi
        lea edx, [ebp - 0x70]
        push 0
        mov ax, word ptr [esi + 0x14]
        mov cx, word ptr [esi + 0x16]
        push edx
        mov dword ptr [ebp - 4], eax
        mov dword ptr [ebp + 8], ecx
        call GetSprite
        add esp, 8
        test eax, eax
        jne L4891c3
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
L4891c3:
        lea eax, [ebp - 0x58]
        push esi
        push eax
        call GetSprite
        add esp, 8
        test eax, eax
        jne L4891db
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
L4891db:
        mov eax, dword ptr [DisplayPixelFormat]
        sub eax, 0
        je L489362
        dec eax
        je L48935b
        dec eax
        jne L489367
        lea edx, [SPRITE_ClipRect]
        _emit 0x8d
        _emit 0xb5
        _emit 0xc0
        _emit 0xff
        _emit 0xff
        _emit 0xff
        mov eax, dword ptr [ebp + 0xc]
        mov ecx, dword ptr [edx]
        _emit 0x8d
        _emit 0xbd
        _emit 0xd0
        _emit 0xff
        _emit 0xff
        _emit 0xff
        cmp eax, ecx
        jl L48921c
        mov dword ptr [edi], eax
        mov dword ptr [esi], 0
        jmp L489222
L48921c:
        mov dword ptr [edi], ecx
        sub ecx, eax
        mov dword ptr [esi], ecx
L489222:
        movzx ebx, word ptr [ebp - 4]
        mov ecx, dword ptr [edx + 8]
        dec ebx
        add eax, ebx
        cmp eax, ecx
        jg L489238
        mov dword ptr [edi + 8], eax
        mov dword ptr [esi + 8], ebx
        jmp L489242
L489238:
        sub eax, ecx
        sub ebx, eax
        mov dword ptr [edi + 8], ecx
        mov dword ptr [esi + 8], ebx
L489242:
        mov eax, dword ptr [ebp + 0x10]
        mov ecx, dword ptr [edx + 4]
        cmp eax, ecx
        jl L489258
        mov dword ptr [edi + 4], eax
        mov dword ptr [esi + 4], 0
        jmp L489260
L489258:
        mov dword ptr [edi + 4], ecx
        sub ecx, eax
        mov dword ptr [esi + 4], ecx
L489260:
        movzx ebx, word ptr [ebp + 8]
        dec ebx
        mov ecx, dword ptr [edx + 0xc]
        add eax, ebx
        cmp eax, ecx
        jg L489276
        mov dword ptr [edi + 0xc], eax
        mov dword ptr [esi + 0xc], ebx
        jmp L489280
L489276:
        sub eax, ecx
        sub ebx, eax
        mov dword ptr [edi + 0xc], ecx
        mov dword ptr [esi + 0xc], ebx
L489280:
        xor eax, eax
        mov edx, dword ptr [edi]
        mov ebx, dword ptr [edi + 8]
        mov ecx, dword ptr [edi + 4]
        cmp edx, ebx
        jge L489356
        mov edx, dword ptr [edi + 0xc]
        cmp ecx, edx
        jge L489356
        _emit 0x8d
        _emit 0xb5
        _emit 0x90
        _emit 0xff
        _emit 0xff
        _emit 0xff
        mov eax, dword ptr [esi]
        mov dword ptr [ebp - 0x1c], eax
        mov dword ptr [ebp - 0x18], eax
        mov ebx, dword ptr [edi + 4]
        mul ebx
        mov ecx, dword ptr [edi]
        shl ecx, 1
        add eax, dword ptr [esi + 0xc]
        add eax, ecx
        mov dword ptr [ebp - 0x14], eax
        _emit 0x8d
        _emit 0xb5
        _emit 0xa8
        _emit 0xff
        _emit 0xff
        _emit 0xff
        mov eax, dword ptr [esi]
        _emit 0x8d
        _emit 0xbd
        _emit 0xc0
        _emit 0xff
        _emit 0xff
        _emit 0xff
        mov dword ptr [ebp - 0x10], eax
        mov ebx, dword ptr [edi + 4]
        mul ebx
        mov ecx, dword ptr [edi]
        shl ecx, 1
        add eax, dword ptr [esi + 0xc]
        add eax, ecx
        and eax, 0xfffffffc
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp - 0x14]
        mov edx, dword ptr [edi + 0xc]
        sub edx, dword ptr [edi + 4]
        mov ecx, dword ptr [edi + 8]
        sub edx, 1
        sub ecx, dword ptr [edi]
        sub ecx, 2
        and edx, 0xffffffff
        and ecx, 0xffffffff
        mov dword ptr [ebp - 8], ecx
        shl dword ptr [ebp - 0x10], 1
        shl dword ptr [ebp - 0x1c], 1
        mov ebx, eax
L489306:
        mov edi, dword ptr [ebp - 0xc]
        push ebp
        mov ebp, dword ptr [ebp - 0x18]
        push edx
L48930e:
        mov esi, dword ptr [edi]
        add eax, 4
        and esi, 0xf7def7de
        je L489339
        shr esi, 1
        mov edx, dword ptr [eax]
        add edi, 4
        and edx, 0xf7def7de
        shr edx, 1
        nop
        add esi, edx
        sub ecx, 2
        mov dword ptr [eax - 4], esi
        mov dword ptr [eax + ebp - 4], esi
        jns L48930e
L489339:
        add edi, 4
        sub ecx, 2
        jns L48930e
        pop edx
        pop ebp
        mov eax, dword ptr [ebp - 0x10]
        add ebx, dword ptr [ebp - 0x1c]
        add dword ptr [ebp - 0xc], eax
        mov eax, ebx
        mov ecx, dword ptr [ebp - 8]
        sub edx, 2
        jns L489306
L489356:
        mov dword ptr [ebp - 0x20], eax
        jmp L489367
L48935b:
        mov eax, 0
        jmp L489367
L489362:
        mov eax, 0
L489367:
        lea ecx, [ebp - 0x58]
        push ecx
        call ReleaseSprite
        lea edx, [ebp - 0x70]
        push edx
        call ReleaseSprite
        mov eax, dword ptr [ebp - 0x20]
        add esp, 8
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

// FUNCTION: LEGOLAND 0x00489390
LEGO_EXPORT void RenderThickBox(int x, int y, int w, int h, int thickness, unsigned int color) {
    RenderBlock(x, y, w, thickness, color);
    RenderBlock(x, y + thickness, thickness, h - thickness * 2, color);
    RenderBlock((x - thickness) + w, y + thickness, thickness, h - thickness * 2, color);
    RenderBlock(x, (y - thickness) + h, w, thickness, color);
}

// FUNCTION: LEGOLAND 0x00489410
LEGO_EXPORT void RenderBox(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5) {
    RenderThickBox(a1, a2, a3, a4, 1, a5);
}
