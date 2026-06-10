#include "legoland.h"
#include "crt.h"
#include "globals.h"
#include "man3d.h"
#include "render3d.h"
#include "resource.h"
#include "llidb.h"
#include "math.h"
#include "tilemap.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include <math.h>

#include "image_sprite.h"

struct AdjustStruct {
    int field0;
    int field4;
};

LEGO_EXPORT void AdjustOffsetForViewMode(struct AdjustStruct *param_1);

struct Viewport {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_c;
};

// FUNCTION: LEGOLAND 0x00441800
LEGO_EXPORT void Render_SetViewport(struct Viewport *viewport) {
    DAT_0081c8d0 = viewport->field_0;
    DAT_0081c8d8 = viewport->field_8;
    DAT_0081c8d4 = viewport->field_4;
    DAT_0081c8dc = viewport->field_c;
}

struct RenderListNode {
    struct RenderListNode *next;
    unsigned char pad_4[8];
    short id;
};

// FUNCTION: LEGOLAND 0x00441830
struct RenderListNode *FUN_00441830(void *param_1, short *param_2) {
    struct RenderListNode *node = (struct RenderListNode *)DAT_0081c8cc;
    while (node != NULL) {
        if (*param_2 == node->id) {
            DAT_0081c8cc = node;
            return node;
        }
        node = node->next;
    }
    DAT_0081c8cc = NULL;
    return NULL;
}

struct ViewportEntry {
    unsigned char pad_0[0x2e];
    short field_2e;
    unsigned char pad_30[0xcc - 0x30];
    void *field_cc;
};

// FUNCTION: LEGOLAND 0x00441870
struct RenderListNode *FUN_00441870(struct ViewportEntry *param_1, short *param_2) {
    DAT_0081c8cc = param_1->field_cc;
    return FUN_00441830(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x00441890
struct RenderListNode *FUN_00441890(void *param_1, short *param_2) {
    struct RenderListNode *node = (struct RenderListNode *)DAT_0081c8cc;
    if (node != NULL) {
        DAT_0081c8cc = node->next;
        return FUN_00441830(param_1, param_2);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x004418c0
struct RenderListNode *FUN_004418c0(int param_1, struct ViewportEntry *param_2, short *param_3) {
    struct RenderListNode *node = FUN_00441870(param_2, param_3);
    int i = 0;
    if (node != NULL && param_2->field_2e > 0) {
        do {
            if (i == param_1) {
                return node;
            }
            node = FUN_00441890(param_2, param_3);
            i = i + 1;
        } while (i < param_2->field_2e);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00441910
void FUN_00441910(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00441980
void FUN_00441980(int *param_1, int param_2, int param_3, int param_4, int param_5, int param_6) { STUB(); }

struct BlokeRideInner {
    unsigned char pad_0[0x62];
    unsigned char flags;
};

struct BlokeRideNode {
    unsigned char pad_0[8];
    struct BlokeRideInner *inner;
    unsigned char pad_c[4];
    unsigned int field_10;
};

// FUNCTION: LEGOLAND 0x00441a60
LEGO_EXPORT void Put3DBlokesOnRide(struct ViewportEntry *param_1, unsigned char *param_2, int param_3, int *param_4) {
    union { __int64 i; struct { int lo; int hi; } p; } coords;
    int i;

    coords.i = GetScreenCoordsForObject(param_2, param_1);
    i = 0;
    if (param_4[1] > 0) {
        do {
            struct BlokeRideNode *node = (struct BlokeRideNode *)FUN_004418c0(i, param_1, (short *)param_2);
            if (node != NULL && (node->inner->flags & 0x80) != 0) {
                FUN_00441980(param_4, i, param_3, node->field_10, coords.p.lo, coords.p.hi);
            }
            i = i + 1;
        } while (i < param_4[1]);
    }
}

// FUNCTION: LEGOLAND 0x00441ad0
LEGO_EXPORT void Put3DBlokesOnRide2(struct RideObject *ride, struct RideObject *obj) {
    struct BlokeRideNode *node = (struct BlokeRideNode *)FUN_00441870((struct ViewportEntry *)ride, (short *)obj);
    if (node != NULL) {
        while ((node->inner->flags & 0x80) != 0) {
            node = (struct BlokeRideNode *)FUN_00441890(ride, (short *)obj);
            if (node == NULL) {
                return;
            }
        }
        while (node != NULL) {
            if (node->field_10 != 0) {
                FUN_004401b0(node->field_10, (struct Person *)node->inner);
            }
            node = (struct BlokeRideNode *)FUN_00441890(ride, (short *)obj);
            if (node == NULL) {
                return;
            }
            while ((node->inner->flags & 0x80) != 0) {
                node = (struct BlokeRideNode *)FUN_00441890(ride, (short *)obj);
                if (node == NULL) {
                    return;
                }
            }
        }
    }
}

struct RenderNode {
    struct RenderNode *next;
    unsigned char pad_4[4];
    void *data_ptr;
    unsigned short id;
};

struct RenderBloke {
    unsigned short id;
};

struct RenderData {
    unsigned char pad_0[0x62];
    unsigned char flags;
};

struct RenderContext {
    unsigned char pad_0[0xcc];
    struct RenderNode *bloke_list;
};

// FUNCTION: LEGOLAND 0x00441b60
LEGO_EXPORT void RenderBlokesNotInSeats(unsigned int a1, unsigned int a2) {
    struct RenderContext *context = (struct RenderContext *)a1;
    struct RenderBloke *bloke = (struct RenderBloke *)a2;
    struct RenderNode *node = context->bloke_list;

    while (node != NULL) {
        if (bloke->id == node->id) {
            struct RenderData *data = (struct RenderData *)node->data_ptr;
            if ((data->flags & 0x80) == 0) {
                IP_RenderBlokeIn3DNow((struct Bloke *)data);
            }
        }
        node = node->next;
    }
}

struct RinData {
    unsigned char pad_0[0x10];
    int sprite_count;
    int data_count;
    struct Sprite **sprites;
    void **datas;
};

#pragma intrinsic(memset)

// FUNCTION: LEGOLAND 0x00441ba0
LEGO_EXPORT struct RinData *LoadRin(const char *path, const char *dir) {
    struct RinData *rin;
    struct ResFile *file;
    char name[264];
    char sprite_name[264];
    char c;
    int i;
    int j;

    rin = (struct RinData *)malloc(0x20);
    file = RES_OpenFile(path);
    if (file != NULL) {
        memset(rin, 0, 0x20);
        RES_ReadFile(file, &rin->sprite_count, 4);
        rin->sprites = (struct Sprite **)malloc(rin->sprite_count << 2);
        i = 0;
        if (rin->sprite_count > 0) {
            do {
                char *p = name;
                do {
                    RES_ReadFile(file, &c, 1);
                    *p = c;
                    p++;
                } while (c != '\0');
                // STRING: LEGOLAND 0x004b7cf4
                sprintf(sprite_name, "%s\\%s.lls", dir, name);
                rin->sprites[i] = LoadSprite(sprite_name, 1);
                i++;
            } while (i < rin->sprite_count);
        }
        RES_ReadFile(file, &rin->data_count, 4);
        rin->datas = (void **)malloc(rin->data_count << 2);
        if (rin->datas != NULL && (j = 0, rin->data_count > 0)) {
            do {
                rin->datas[j] = malloc(rin->sprite_count << 2);
                RES_ReadFile(file, rin->datas[j], rin->sprite_count << 2);
                j++;
            } while (j < rin->data_count);
        }
        RES_CloseFile(file);
    }
    return rin;
}

#pragma function(memset)

// FUNCTION: LEGOLAND 0x00441cf0
LEGO_EXPORT void UnLoadRin(struct RinData *rin) {
    int i;
    int count;

    count = rin->sprite_count;
    for (i = 0; i < count; i++) {
        KillSprite(rin->sprites[i]);
        count = rin->sprite_count;
    }
    free(rin->sprites);

    count = rin->data_count;
    for (i = 0; i < count; i++) {
        free(rin->datas[i]);
        count = rin->data_count;
    }
    free(rin->datas);

    free(rin);
}

struct RinRender {
    int x;
    int y;
    int *remap_table;
    int *data_table;
    int loop_count;
    int modulo;
    int **index_array;
    int **frame_array;
};

// FUNCTION: LEGOLAND 0x00441d60
LEGO_EXPORT void RenderUsingRin(struct RinRender *param_1, int param_2, struct ViewportEntry *param_3, unsigned char *param_4) {
    union { __int64 i; struct { int lo; int hi; } p; } coords;
    int idx;
    int *entry;
    int i;
    struct AdjustStruct offset;

    coords.i = GetScreenCoordsForObject(param_4, param_3);
    idx = param_2;
    if (param_1->modulo <= param_2) {
        idx = param_2 % param_1->modulo;
    }
    i = param_1->loop_count - 1;
    if (i >= 0) {
        entry = (int *)((char *)param_1->frame_array[idx] + i * 4);
        i = i + 1;
        do {
            int sprite_id = *entry;
            int lookup = sprite_id;
            struct BlokeRideNode *node;
            int *frame;
            if (param_1->data_table != NULL) {
                lookup = param_1->data_table[param_1->remap_table[sprite_id]];
            }
            node = (struct BlokeRideNode *)FUN_004418c0(lookup, param_3, (short *)param_4);
            if (node != NULL && (node->inner->flags & 0x80) != 0) {
                IP_RenderBlokeIn3DNow((struct Bloke *)node->inner);
            }
            frame = (int *)param_1->index_array[sprite_id];
            if (frame != NULL) {
                LLSSetFrame((struct LLS *)GetLLSForSprite((struct SpriteLLS *)frame), param_2);
            }
            offset.field0 = param_1->x;
            offset.field4 = param_1->y;
            AdjustOffsetForViewMode(&offset);
            frame = (int *)param_1->index_array[sprite_id];
            if (frame != NULL) {
                PrintSprite((struct Sprite *)frame, coords.p.lo + offset.field0, coords.p.hi + offset.field4, 0, 0);
            }
            entry = entry - 1;
            i = i - 1;
        } while (i != 0);
    }
}

struct SpriteLLS {
    unsigned char pad_0[8];
    unsigned int *field_8;
};

// FUNCTION: LEGOLAND 0x00441e80
LEGO_EXPORT unsigned int GetLLSForSprite(struct SpriteLLS *sprite) {
    if (sprite != NULL) {
        return *sprite->field_8;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00441ea0
LEGO_EXPORT unsigned int GetLLSForLayer(unsigned int a, unsigned int b) {
    unsigned int c = *(unsigned int *)(a + 8);
    unsigned int d = *(unsigned int *)(c + 8);
    unsigned int e = *(unsigned int *)(d + b * 4);
    if (e == 0)
        return 0;
    e = *(unsigned int *)(e + 8);
    return *(unsigned int *)e;
}

struct LayerContainer {
    unsigned char pad_0[8];
    struct SpriteListContainer *field_8;
};

struct SpriteListContainer {
    unsigned char pad_0[8];
    unsigned int *field_8;
};

// FUNCTION: LEGOLAND 0x00441ec0
LEGO_EXPORT unsigned int GetSpriteForLayer(struct LayerContainer *arg1, unsigned int arg2) {
    struct SpriteListContainer *container = arg1->field_8;
    unsigned int *sprite_array = container->field_8;
    return sprite_array[arg2];
}

struct LayerOffsets {
    unsigned char pad_0[0xc];
    int *x_offsets;
    int *y_offsets;
};

struct LayerOffsetHolder {
    unsigned char pad_0[8];
    struct LayerOffsets *field_8;
};

// FUNCTION: LEGOLAND 0x00441ee0
LEGO_EXPORT __int64 GetRenderOffsetForLayer(struct LayerOffsetHolder *param_1, int param_2) {
    struct LayerOffsets *o = param_1->field_8;
    union { __int64 i; struct { int lo; int hi; } p; } r;
    r.p.lo = o->x_offsets[param_2];
    r.p.hi = o->y_offsets[param_2];
    return r.i;
}

// FUNCTION: LEGOLAND 0x00441f00
LEGO_EXPORT void StopLayerPlaying(unsigned int layerID, unsigned int someValue) {
    unsigned int lls_handle = GetLLSForLayer(layerID, someValue);
    LLSStop(lls_handle);
}

#pragma intrinsic(memset)

// FUNCTION: LEGOLAND 0x00441f20
LEGO_EXPORT unsigned short *LoadPalette(unsigned int path) {
    unsigned short *palette;
    unsigned short *out;
    struct ResFile *file;
    int i;
    unsigned char g;
    unsigned char b;
    unsigned char header[8];

    palette = (unsigned short *)malloc(0x200);
    if (palette != NULL) {
        memset(palette, 0, 0x200);
        file = RES_OpenFile((const char *)path);
        if (file != NULL) {
            RES_ReadFile(file, header, 8);
            out = palette;
            for (i = 0x100; i != 0; i--) {
                RES_ReadFile(file, &path, 1);
                RES_ReadFile(file, &g, 1);
                RES_ReadFile(file, &b, 1);
                if (DAT_00668088 != 2) {
                    *out = (unsigned short)((((((unsigned char)path & 0xf8) << 5) | (g & 0xf8)) << 2) | (b >> 3));
                } else {
                    *out = (unsigned short)((((((unsigned char)path & 0xf8) << 5) | (g & 0xfc)) << 3) | (b >> 3));
                }
                out++;
            }
            RES_CloseFile(file);
        }
    }
    return palette;
}

#pragma function(memset)

// FUNCTION: LEGOLAND 0x00442040
void FUN_00442040(void) { STUB(); }

extern unsigned int FUN_00486280(int param_1, void *param_2);

struct ColourRef {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
};

struct TexEntry {
    unsigned int flags;
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    unsigned char pad_7[1];
    unsigned int field_8;
    unsigned char pad_c[0x24 - 0xc];
};

// FUNCTION: LEGOLAND 0x004424e0
void FUN_004424e0(struct ColourRef *param_1, struct ColourRef *param_2, unsigned int param_3, struct ColourRef *param_4, int param_5, int param_6) {
    int i = 0;
    int count;
    struct TexEntry *entry;

    if (param_6 > 0) {
        entry = (struct TexEntry *)param_5;
        count = param_6;
        do {
            if ((entry->flags & 0x2000) != 0 && entry->b2 == param_1->b2 &&
                entry->b1 == param_1->b1 && entry->b0 == param_1->b0) {
                if (i >= count >> 1) {
                    entry->b2 = param_2->b2;
                    entry->b1 = param_2->b1;
                    entry->b0 = param_2->b0;
                    param_5 = *(int *)param_2;
                } else {
                    entry->b2 = param_4->b2;
                    entry->b1 = param_4->b1;
                    entry->b0 = param_4->b0;
                    param_5 = *(int *)param_4;
                }
                entry->field_8 = FUN_00486280(0x40, &param_5);
                count = param_6;
            }
            entry = (struct TexEntry *)((char *)entry + 0x24);
            i = i + 1;
        } while (i < count);
    }
}

// FUNCTION: LEGOLAND 0x00442580
void FUN_00442580(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004427e0
char *FUN_004427e0(struct ResFile *param_1, char *param_2, int param_3) {
    struct ResFile *file = param_1;
    char *c = (char *)&param_1;
    int count = 0;
    int read;
    char *dst = param_2;

    do {
        read = RES_ReadFile(file, &param_1, 1);
        if (read != 0) {
            if (*c == '\r') goto skip_lf;
            if (*c == '\n') goto terminate;
            *dst = *c;
            dst++;
            count++;
        }
        if (*c == '\r') goto skip_lf;
    } while (*c != '\n' && count < param_3 && read != 0);
    if (*c == '\r') {
skip_lf:
        RES_ReadFile(file, &param_1, 1);
    }
terminate:
    *dst = '\0';
    if (read == 0 && count == 0) {
        return NULL;
    }
    return param_2;
}

// FUNCTION: LEGOLAND 0x00442860
int FUN_00442860(char *param_1, char *param_2) {
    int index = 0;

    while (_stricmp(param_1, param_2) != 0) {
        param_1 = param_1 + strlen(param_1) + 1;
        index = index + 1;
        if (strlen(param_1) == 0) {
            return -1;
        }
    }
    return index;
}

// FUNCTION: LEGOLAND 0x004428c0
unsigned char *FUN_004428c0(unsigned char *str, int count) {
    unsigned char *result = str;
    if (count > 0) {
        do {
            result = result + strlen((char *)result) + 1;
            count = count - 1;
        } while (count != 0);
    }
    return result;
}

// FUNCTION: LEGOLAND 0x004428f0
unsigned char *FUN_004428f0(char *param_1, int param_2, int param_3) {
    char *pcVar4;
    char *pcVar5;

    if (param_1 == NULL) {
        pcVar4 = (char *)param_3;
    } else {
        char *after = param_1 + strlen(param_1) + 1;
        param_1 = after + 4;
        pcVar4 = param_1;
        if (*(int *)after != 0) {
            pcVar5 = param_1;
            do {
                pcVar5 = pcVar5 + strlen(pcVar5) + 1;
            } while (strlen(pcVar5) != 0);
            param_1 = pcVar5 + 1 + strlen(pcVar5 + 1) + 1 + 4;
        }
    }
    if (param_2 == 1) {
        return FUN_004428c0((unsigned char *)param_1, param_3);
    }
    return FUN_004428c0((unsigned char *)pcVar4, param_3);
}

// FUNCTION: LEGOLAND 0x00442980
void FUN_00442980(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00442c70
unsigned int FUN_00442c70(void) {
    if (DAT_00655a38) {
        free(DAT_00655a38);
    }
    if (DAT_0062fea8) {
        free(DAT_0062fea8);
    }
    if (DAT_0062fef8) {
        free(DAT_0062fef8);
    }
    if (DAT_0064cd8c) {
        free(DAT_0064cd8c);
    }
    return 0;
}

struct ScreenObj {
    unsigned char pad_0[0x14];
    int field_14;
    int field_18;
};

// FUNCTION: LEGOLAND 0x00442cc0
LEGO_EXPORT __int64 GetScreenCoordsForObject(unsigned char *param_1, void *param_2) {
    int bounds[2];
    struct Point ref;
    int iVar1;
    int iVar2;
    union { __int64 i; struct { int lo; int hi; } p; } r;

    ref.x = param_1[0];
    ref.y = param_1[1];
    GetTileBounds(&ref, bounds);
    iVar2 = ((struct ScreenObj *)param_2)->field_14;
    iVar1 = ((struct ScreenObj *)param_2)->field_18;
    if (iVar2 < 0) {
        iVar2 = -(-iVar2 >> 1);
    } else {
        iVar2 = iVar2 >> 1;
    }
    if (iVar1 < 0) {
        r.p.hi = bounds[1] - (-iVar1 >> 1);
        r.p.lo = iVar2 + bounds[0];
        return r.i;
    }
    r.p.hi = bounds[1] + (iVar1 >> 1);
    r.p.lo = iVar2 + bounds[0];
    return r.i;
}

// FUNCTION: LEGOLAND 0x00442d30
LEGO_EXPORT void AdjustOffsetForViewMode(struct AdjustStruct *param_1) {
    int temp0;
    int temp4;

    temp0 = param_1->field0;
    if (temp0 < 0) {
        temp0 = -((-temp0) >> 1);
    } else {
        temp0 = temp0 >> 1;
    }
    param_1->field0 = temp0;

    temp4 = param_1->field4;
    if (temp4 < 0) {
        temp4 = -((-temp4) >> 1);
    } else {
        temp4 = temp4 >> 1;
    }
    param_1->field4 = temp4;
}

// FUNCTION: LEGOLAND 0x00442d60
LEGO_EXPORT void AdjustBlokePosition(struct BlokePos *pBloke) {
    pBloke->field_0 -= 0x4b;
    pBloke->field_4 -= 0x4d;
}

// FUNCTION: LEGOLAND 0x00442d80
LEGO_EXPORT void UnAdjustBlokePosition(struct BlokePos *pBloke) {
    pBloke->field_0 += 0x4b;
    pBloke->field_4 += 0x4d;
}

struct Vec3 {
    float x;
    float y;
    float z;
};

// FUNCTION: LEGOLAND 0x00442da0
void FUN_00442da0(struct Vec3 *a, struct Vec3 *b, struct Vec3 *out) {
    out->x = b->z * a->y - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}

// FUNCTION: LEGOLAND 0x00442de0
float FUN_00442de0(struct Vec3 *param_1, struct Vec3 *param_2) {
    return param_1->z * param_2->z + param_1->y * param_2->y + param_1->x * param_2->x;
}

// FUNCTION: LEGOLAND 0x00442e00
LEGO_EXPORT int TMNegParity(float *param_1) {
    struct Vec3 a;
    struct Vec3 b;
    struct Vec3 cross;

    a.x = param_1[0];
    a.y = param_1[1];
    a.z = param_1[2];
    b.x = param_1[3];
    b.y = param_1[4];
    b.z = param_1[5];
    FUN_00442da0(&a, &b, &cross);
    a.x = param_1[6];
    a.y = param_1[7];
    a.z = param_1[8];
    if (FUN_00442de0(&cross, &a) < DAT_004ab4c8) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00442e90
LEGO_EXPORT void RenderItems_New(void) {
    DAT_0062feec[0] = (unsigned int)&DAT_00630108;
    DAT_00655a4c = 0;
}

struct RenderItemNode {
    int key;
    unsigned char pad_4[4];
    struct RenderItemNode *next;
    struct RenderItemNode *prev;
};

// FUNCTION: LEGOLAND 0x00442eb0
LEGO_EXPORT void RenderItem_Link(struct RenderItemNode **head, struct RenderItemNode *node, int key) {
    struct RenderItemNode *cur;

    node->next = NULL;
    node->prev = NULL;
    cur = *head;
    if (cur == NULL) {
        *head = node;
        return;
    }
    while (key > cur->key) {
        if (cur->next == NULL) {
            cur->next = node;
            node->prev = cur;
            return;
        }
        cur = cur->next;
    }
    node->next = cur;
    node->prev = cur->prev;
    if (cur->prev != NULL) {
        cur->prev->next = node;
    }
    cur->prev = node;
    if (cur == *head) {
        *head = node;
    }
}

struct RenderItem {
    unsigned int field_0;
    unsigned int field_4;
};

struct BlokeRenderSrc {
    unsigned char pad_0[8];
    unsigned int field_8;
};

// FUNCTION: LEGOLAND 0x00442f20
LEGO_EXPORT void AddBlokeToRenderList(struct RenderItemNode **head, struct BlokeRenderSrc *src, int key) {
    struct RenderItem *item = (struct RenderItem *)FUN_00442f50();
    item->field_4 = src->field_8;
    item->field_0 = key;
    RenderItem_Link(head, (struct RenderItemNode *)item, key);
}

// FUNCTION: LEGOLAND 0x00442f50
unsigned int FUN_00442f50(void) {
    unsigned int result = DAT_0062feec[0];
    DAT_0062feec[0] = DAT_0062feec[0] + 16;
    DAT_00655a4c = DAT_00655a4c + 1;
    return result;
}

struct BlokeListNode {
    unsigned int field_0;
    unsigned int field_4;
    struct BlokeListNode *next;
};

struct BlokeListHead {
    struct BlokeListNode *next;
};

// FUNCTION: LEGOLAND 0x00442f70
LEGO_EXPORT void RenderBlokeList(struct BlokeListHead *list) {
    struct BlokeListNode *node = list->next;
    while (node) {
        if (node->field_4) {
            IP_RenderBlokeIn3DNow((struct Bloke *)node->field_4);
        }
        node = node->next;
    }
}

struct MapCellElement {
    struct RenderObjectVtable3 *vtable;
};

struct RenderObjectVtable3 {
    unsigned char pad_0[0xc];
    struct ObjClassNode *class_node;
};

struct RideInstance {
    unsigned char pad_0[0xc];
    unsigned short flags;
};

// FUNCTION: LEGOLAND 0x00442fa0
LEGO_EXPORT void Ride_SetFlagToNotLetAnyoneOn(unsigned char *param_1) {
    struct MapCellElement *cell;
    struct RideInstance *instance;
    int x = param_1[0];
    int y = param_1[1];

    if (x >= 0 && x < (int)lpConfig->width && y >= 0 && y < (int)lpConfig->height) {
        cell = (struct MapCellElement *)((char *)GameMap[y] + x * 0x14);
    } else {
        cell = 0;
    }
    instance = (struct RideInstance *)GetInstanceOfClass(cell->vtable->class_node, (const unsigned short *)param_1);
    if (instance != 0) {
        *(unsigned char *)&instance->flags |= 2;
    }
}

// FUNCTION: LEGOLAND 0x00443000
LEGO_EXPORT void Ride_ClearFlagToNotLetAnyoneOn(unsigned char *param_1) {
    struct MapCellElement *cell;
    struct RideInstance *instance;
    int x = param_1[0];
    int y = param_1[1];

    if (x >= 0 && x < (int)lpConfig->width && y >= 0 && y < (int)lpConfig->height) {
        cell = (struct MapCellElement *)((char *)GameMap[y] + x * 0x14);
    } else {
        cell = 0;
    }
    instance = (struct RideInstance *)GetInstanceOfClass(cell->vtable->class_node, (const unsigned short *)param_1);
    if (instance != 0) {
        instance->flags &= 0xfffd;
    }
}

// FUNCTION: LEGOLAND 0x00443060
LEGO_EXPORT void RenderItems2_New(void) {
    DAT_0062fef0 = (unsigned int)&DAT_00638218;
    DAT_00655a50 = 0;
}

// FUNCTION: LEGOLAND 0x00443080
LEGO_EXPORT void RenderItem2_Link(struct RenderItemNode **head, struct RenderItemNode *node, int key) {
    struct RenderItemNode *cur;

    node->next = NULL;
    node->prev = NULL;
    cur = *head;
    if (cur == NULL) {
        *head = node;
        return;
    }
    while (key > cur->key) {
        if (cur->next == NULL) {
            cur->next = node;
            node->prev = cur;
            return;
        }
        cur = cur->next;
    }
    node->next = cur;
    node->prev = cur->prev;
    if (cur->prev != NULL) {
        cur->prev->next = node;
    }
    cur->prev = node;
    if (cur == *head) {
        *head = node;
    }
}

struct RenderItem2 {
    unsigned int field_0;
    unsigned int field_4;
};

// FUNCTION: LEGOLAND 0x004430f0
LEGO_EXPORT void RenderItem2_AddItem(struct RenderItemNode **head, unsigned int param_2, int key) {
    struct RenderItem2 *item = (struct RenderItem2 *)FUN_00443120();
    item->field_4 = param_2;
    item->field_0 = key;
    RenderItem2_Link(head, (struct RenderItemNode *)item, key);
}

// FUNCTION: LEGOLAND 0x00443120
unsigned int FUN_00443120(void) {
    unsigned int result = DAT_0062fef0;
    DAT_0062fef0 = DAT_0062fef0 + 16;
    DAT_00655a50 = DAT_00655a50 + 1;
    return result;
}

struct BlokeSex0 {
    unsigned char pad_0[4];
    struct BlokeSex1 *field_4;
};

struct BlokeSex1 {
    unsigned char pad_0[0x80];
    unsigned int field_80;
    unsigned int field_84;
    unsigned char pad_88[0x8c - 0x88];
    unsigned int field_8c;
    unsigned int field_90;
};

// FUNCTION: LEGOLAND 0x00443140
LEGO_EXPORT unsigned int GetSexOfBloke(struct BlokeSex0 *param_1) {
    struct BlokeSex1 *inner = param_1->field_4;
    return inner->field_84;
}

// FUNCTION: LEGOLAND 0x00443150
LEGO_EXPORT char *GetFaceTextureNameOfBloke(struct BlokeSex0 *param_1) {
    struct BlokeSex1 *inner = param_1->field_4;
    void *ptr;
    char *name;
    switch (inner->field_84) {
    case 0:
        ptr = DAT_00630100;
        break;
    case 1:
        ptr = DAT_0062feac;
        break;
    default:
        ptr = param_1;
    }
    name = (char *)FUN_004428f0((char *)ptr, 0, inner->field_80);
    // STRING: LEGOLAND 0x004b7d24
    _stricmp(name, "chest girly1");
    // STRING: LEGOLAND 0x004b7d14
    return "chest girly2";
}

// FUNCTION: LEGOLAND 0x004431a0
LEGO_EXPORT char *GetChestTextureNameOfBloke(struct BlokeSex0 *param_1) {
    struct BlokeSex1 *inner = param_1->field_4;
    void *ptr;
    char *name;
    switch (inner->field_84) {
    case 0:
        ptr = DAT_00630100;
        break;
    case 1:
        ptr = DAT_0062feac;
        break;
    default:
        ptr = param_1;
    }
    name = (char *)FUN_004428f0((char *)ptr, 1, inner->field_80);
    _stricmp(name, "chest girly1");
    return "chest girly2";
}

// FUNCTION: LEGOLAND 0x004431f0
LEGO_EXPORT unsigned int GetLegColourOfBloke(struct BlokeSex0 *param_1) {
    unsigned int idx = param_1->field_4->field_8c;
    return (DAT_004b7ac0[idx * 3] << 16) | (DAT_004b7ac0[idx * 3 + 1] << 8) | DAT_004b7ac0[idx * 3 + 2];
}

// FUNCTION: LEGOLAND 0x00443220
LEGO_EXPORT unsigned int GetArmColourOfBloke(struct BlokeSex0 *param_1) {
    unsigned int idx = param_1->field_4->field_90;
    return (DAT_004b7ac0[idx * 3] << 16) | (DAT_004b7ac0[idx * 3 + 1] << 8) | DAT_004b7ac0[idx * 3 + 2];
}

// FUNCTION: LEGOLAND 0x00443250
float FUN_00443250(float param_1) {
    return (float)sin(param_1);
}

// FUNCTION: LEGOLAND 0x00443260
float FUN_00443260(float param_1) {
    return (float)cos(param_1);
}

// FUNCTION: LEGOLAND 0x00443270
LEGO_EXPORT void MatrixMultiply(float *A, float *B, float *C) {
    C[0] = A[0] * B[0] + A[1] * B[3] + A[2] * B[6];
    C[1] = A[1] * B[4] + A[2] * B[7] + A[0] * B[1];
    C[2] = A[1] * B[5] + A[2] * B[8] + A[0] * B[2];

    C[3] = A[4] * B[3] + A[3] * B[0] + A[5] * B[6];
    C[4] = A[5] * B[7] + A[4] * B[4] + A[3] * B[1];
    C[5] = A[5] * B[8] + A[3] * B[2] + A[4] * B[5];

    C[6] = A[6] * B[0] + A[8] * B[6] + A[7] * B[3];
    C[7] = A[8] * B[7] + A[7] * B[4] + A[6] * B[1];
    C[8] = A[8] * B[8] + A[7] * B[5] + A[6] * B[2];
}

// FUNCTION: LEGOLAND 0x00443360
LEGO_EXPORT void BuildYRotationMatrix(float angle, float *out) {
    float s = FUN_00443250(angle);
    float c = FUN_00443260(angle);
    out[0] = c;
    out[6] = -s;
    out[1] = 0.0f;
    out[2] = s;
    out[8] = c;
    out[3] = 0.0f;
    out[4] = 1.0f;
    out[5] = 0.0f;
    out[7] = 0.0f;
}

// FUNCTION: LEGOLAND 0x004433b0
LEGO_EXPORT void TransformVectorsL(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00443450
LEGO_EXPORT void NormaliseVector(struct Vec3 *v) {
    float m = (float)sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x = v->x / m;
    v->y = v->y / m;
    v->z = v->z / m;
}

struct Matrix3x3 {
    unsigned int m[9];
};

// FUNCTION: LEGOLAND 0x00443490
LEGO_EXPORT void CopyMatrix(struct Matrix3x3 *src, struct Matrix3x3 *dest) {
    dest->m[0] = src->m[0];
    dest->m[1] = src->m[1];
    dest->m[2] = src->m[2];
    dest->m[3] = src->m[3];
    dest->m[4] = src->m[4];
    dest->m[5] = src->m[5];
    dest->m[6] = src->m[6];
    dest->m[7] = src->m[7];
    dest->m[8] = src->m[8];
}
