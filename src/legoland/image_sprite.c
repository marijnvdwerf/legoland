#include <windows.h>
#include "legoland.h"
#include "crt.h"
#include "globals.h"

#include "image_sprite.h"
#include "gfx.h"
#include "llidb.h"
#include "draw.h"

struct SpriteListNode;
struct Layer;
struct LayerHost;
struct LayerResult;

struct Sprite {
    struct Sprite *next;
    unsigned int field_4;
    struct Image *image;
    unsigned int field_c;
    unsigned int field_10;
    unsigned short field_14;
    unsigned short field_16;
    unsigned short field_18;
    unsigned short field_1a;
    unsigned short field_1c;
};

struct Image {
    unsigned int field_0;
    void *field_4;
    unsigned short width;
    unsigned short height;
    unsigned short refcount;
    unsigned char field_e;
    unsigned char pad_f[0x10 - 0xf];
    char *name;
    unsigned int field_14;
};

struct LayerVtbl {
    void *func_0;
    void *func_4;
    void(__stdcall *func_8)(struct LayerHost *self);
};

struct LayerHost {
    struct LayerVtbl *vtable;
};

struct LayerArrays {
    unsigned char pad_0[4];
    int count;
    void **array_8;
    void **array_c;
    void **array_10;
};

struct LayerOwner {
    unsigned char pad_0[8];
    struct LayerArrays *arrays;
    unsigned char pad_c[0x10 - 0xc];
    unsigned int flags;
};

struct LayerResult {
    void *field_0;
    void *field_4;
    void *field_8;
};

struct KLIBAUDIO_Vtbl {
    unsigned char pad_0[0x30];
    void(__stdcall *func_30)(void *self, int a1, int a2, int a3);
    unsigned char pad_34[0x3c - 0x34];
    void(__stdcall *func_3c)(void *self, unsigned int a1);
};

struct KLIBAUDIO_Stop {
    unsigned char pad_0[0x48];
    void(__stdcall *func_48)(struct KLIBAUDIO_Object *self);
};

struct KLIBAUDIO_Object {
    void *vtable;
};


// FUNCTION: LEGOLAND 0x00496f20
LEGO_EXPORT unsigned int GetVRAMAddress(unsigned int address) {
    return address + 4;
}

// FUNCTION: LEGOLAND 0x00496f30
unsigned int *FUN_00496f30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496fc0
int FUN_00496fc0(unsigned int value) {
    unsigned int *slot;

    slot = FUN_00496f30();
    if (slot != NULL) {
        *slot = value;
        DAT_0079a7cc++;
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00496ff0
unsigned int *FUN_00496ff0(unsigned int value) {
    int count;
    unsigned int *array;
    int i;
    unsigned int *entry;
    int search_val;

    count = DAT_0079a7c8;
    array = (unsigned int *)DAT_0079a7c4;
    i = 0;
    entry = array;
    search_val = value;

    if (count <= 0) {
        return NULL;
    }

    while (i < count) {
        if (*entry == (unsigned int)search_val) {
            return entry;
        }
        i++;
        entry++;
    }

    return NULL;
}

// FUNCTION: LEGOLAND 0x00497020
int FUN_00497020(unsigned int value) {
    unsigned int *entry;

    entry = FUN_00496ff0(value);
    if (entry != NULL) {
        *entry = 0;
        DAT_0079a7cc--;
        if (DAT_0079a7cc == 0) {
            free(DAT_0079a7c4);
            DAT_0079a7c4 = NULL;
        }
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00497070
LEGO_EXPORT void ReloadAllDetailDependentImages(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004970b0
LEGO_EXPORT void ReloadAllDetailDependentImagesAndBuildSprites(void) {
    struct Sprite *node;
    struct Image **array;
    unsigned int count1;
    unsigned int count2;
    struct Image *image;

    node = DAT_0079a7c0;
    while (node != NULL) {
        if ((node->field_10 & 0x400) == 0) {
            image = (struct Image *)node->field_4;
            if (image != NULL) {
                ((struct LayerHost *)image)->vtable->func_8((struct LayerHost *)image);
            }
        }
        node = node->next;
    }

    count1 = DAT_0079a7c8;
    count2 = DAT_0079a7cc;
    array = DAT_0079a7c4;
    while (count1 != 0 && count2 != 0) {
        image = *array;
        if (image != NULL) {
            ReloadImageBitmapAndBuildSprites(image);
            count2--;
        }
        count1--;
        array++;
    }
}

// FUNCTION: LEGOLAND 0x00497110
LEGO_EXPORT unsigned int MarkAllSpritesForResizing(void) {
    struct Sprite *node;
    unsigned int count;

    node = DAT_0079a7c0;
    count = 0;
    while (node != NULL) {
        if (node->image->field_e == 1) {
            node->field_10 &= 0xfffffbff;
            count++;
        } else {
            node->field_10 |= 0x400;
        }
        node = node->next;
    }
    return count;
}

// FUNCTION: LEGOLAND 0x00497150
LEGO_EXPORT void MarkSpriteResized(struct Sprite *sprite) {
    sprite->field_10 |= 0x400;
}

// FUNCTION: LEGOLAND 0x00497160
LEGO_EXPORT void RemakeAllDetailDependentSprites(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00497280
LEGO_EXPORT struct Image *CreateSourceImage(const char *str, unsigned char type) {
    struct Image *image;

    image = (struct Image *)malloc(strlen(str) + 0x19);
    if (image == NULL) {
        return NULL;
    }
    image->field_e = type;
    image->refcount = 1;
    image->field_0 = 0;
    image->field_4 = NULL;
    image->name = (char *)image + 0x18;
    strcpy(image->name, str);
    if (type == 1) {
        FUN_00496fc0((unsigned int)image);
    }
    return image;
}

// FUNCTION: LEGOLAND 0x00497300
LEGO_EXPORT struct Image *LoadSourceImage(unsigned int a, unsigned int b) {
    struct Image *image;

    image = (struct Image *)CreateSourceImage(a, b);
    if (image == NULL) {
        return NULL;
    }
    if (__BMPLoader(image) == 0) {
        KillImage(image);
        return NULL;
    }
    return image;
}

// FUNCTION: LEGOLAND 0x00497340
LEGO_EXPORT int ReloadImageBitmap(struct Image *image) {
    if (image->field_4 != NULL) {
        free(image->field_4);
    }
    if (image->field_0 != 0) {
        FreeBitmapResources(image);
        return __BMPLoader(image) != 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00497380
LEGO_EXPORT void ReloadImageBitmapAndBuildSprites(struct Image *image) { STUB(); }

// FUNCTION: LEGOLAND 0x00497500
LEGO_EXPORT unsigned short ReferenceImage(struct Image *image) {
    image->refcount++;
    return image->refcount;
}

// FUNCTION: LEGOLAND 0x00497510
LEGO_EXPORT int KillImage(struct Image *image) {
    image->refcount--;
    if (image->refcount == 0) {
        if (image->field_14 == 2 || image->field_14 == 3) {
            LLSStop(image->field_0);
        }
        if (image->field_0 != 0) {
            free((void *)image->field_0);
        }
        if (image->field_4 != NULL) {
            free(image->field_4);
        }
        if (image->field_e == 1) {
            FUN_00497020((unsigned int)image);
        }
        free(image);
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00497580
struct Sprite *FUN_00497580(void) {
    struct Sprite *sprite;

    sprite = (struct Sprite *)malloc(0x20);
    if (sprite != NULL) {
        sprite->next = DAT_0079a7c0;
        DAT_0079a7c0 = sprite;
    }
    return sprite;
}

// FUNCTION: LEGOLAND 0x004975b0
void FUN_004975b0(struct Sprite *sprite) { STUB(); }

// FUNCTION: LEGOLAND 0x00497610
LEGO_EXPORT void FreeBitmapResources(struct Image *image) {
    if (image->field_0 != 0) {
        LLSStop(image->field_0);
        free((void *)image->field_0);
        image->field_0 = 0;
    }
}

// FUNCTION: LEGOLAND 0x00497640
LEGO_EXPORT void CreateSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004976c0
LEGO_EXPORT struct Sprite *CreateFunctionBasedSprite(unsigned int source, unsigned short a, unsigned short b) {
    struct Sprite *sprite;

    sprite = FUN_00497580();
    if (sprite == NULL) {
        return NULL;
    }
    sprite->field_4 = 0;
    sprite->image = (struct Image *)source;
    sprite->field_1c = 1;
    sprite->field_18 = 0;
    sprite->field_1a = 0;
    sprite->field_10 = 0x30;
    sprite->field_c = DAT_008119a4 - 1;
    sprite->field_14 = a;
    sprite->field_16 = b;
    return sprite;
}

// FUNCTION: LEGOLAND 0x00497710
LEGO_EXPORT struct Sprite *CreateSysmemSprite(struct Image *image) {
    struct Sprite *sprite;

    sprite = FUN_00497580();
    if (sprite == NULL) {
        return NULL;
    }
    sprite->field_4 = 0;
    sprite->image = image;
    *(unsigned short *)((char *)image + 0xc) += 1;
    sprite->field_1c = 1;
    sprite->field_18 = 0;
    sprite->field_1a = 0;
    sprite->field_10 = 0x10;
    sprite->field_c = DAT_008119a4 - 1;
    if (image->field_0 == 0) {
        if (__BMPLoader(image) == 0) {
            KillSprite((unsigned int)sprite);
            return NULL;
        }
    }
    sprite->field_14 = *(unsigned short *)((char *)image + 8);
    sprite->field_16 = *(unsigned short *)((char *)image + 0xa);
    return sprite;
}

// FUNCTION: LEGOLAND 0x00497790
LEGO_EXPORT struct Sprite *CreatePartialSprite(struct Image *image, unsigned short a, unsigned short b, unsigned short c, unsigned short d) {
    struct Sprite *sprite;

    sprite = FUN_00497580();
    if (sprite == NULL) {
        return NULL;
    }
    sprite->field_4 = 0;
    sprite->image = image;
    *(unsigned short *)((char *)image + 0xc) += 1;
    sprite->field_18 = a;
    sprite->field_1c = 1;
    sprite->field_1a = b;
    sprite->field_10 = 0;
    sprite->field_c = DAT_008119a4 - 1;
    if (image->field_0 == 0) {
        if (__BMPLoader(image) == 0) {
            KillSprite((unsigned int)sprite);
            return NULL;
        }
    }
    sprite->field_14 = c;
    sprite->field_16 = d;
    return sprite;
}

// FUNCTION: LEGOLAND 0x00497810
LEGO_EXPORT void RecreatePartialSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004978b0
void FUN_004978b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00497ab0
LEGO_EXPORT unsigned int LoadSprite(const char *name, int flags) { STUB(); }

// FUNCTION: LEGOLAND 0x00497b70
LEGO_EXPORT unsigned int MakeSprite(unsigned int sprite) {
    struct Sprite *s;
    struct Image *image;

    s = (struct Sprite *)sprite;
    if ((s->field_10 & 0x20) != 0) {
        PushSetTarget(s);
        ((int(*)(struct Sprite *))s->image)(s);
        PopTarget();
        return 1;
    }
    image = s->image;
    if (image->field_0 != 0 || __BMPLoader(image) != 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00497bb0
LEGO_EXPORT short ReferenceSprite(struct Sprite *sprite) {
    if (sprite == NULL) {
        return -1;
    }
    sprite->field_1c++;
    return sprite->field_1c;
}

// FUNCTION: LEGOLAND 0x00497bd0
LEGO_EXPORT int KillSprite(unsigned int sprite) {
    struct Sprite *s;
    struct LayerHost *host;

    s = (struct Sprite *)sprite;
    if (s != NULL) {
        s->field_1c--;
        if (s->field_1c == 0) {
            host = (struct LayerHost *)s->field_4;
            if (host != NULL) {
                host->vtable->func_8(host);
            }
            if ((s->field_10 & 0x20) == 0) {
                if ((s->field_10 & 0x8000) != 0) {
                    LLIDB_FreeILFTable((struct ILFTable *)s->image);
                } else if (s->image != NULL) {
                    KillImage(s->image);
                }
            }
            FUN_004975b0(s);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00497c30
LEGO_EXPORT void GetSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00497d90
LEGO_EXPORT void SetSubSpriteSource(struct Sprite *sprite, unsigned short a, unsigned short b, unsigned short c, unsigned short d) {
    sprite->field_18 = a;
    sprite->field_1a = b;
    sprite->field_14 = c;
    sprite->field_16 = d;
}

// FUNCTION: LEGOLAND 0x00497dc0
LEGO_EXPORT void ReleaseSprite(struct Sprite *sprite) {
    struct LayerHost *host;
    unsigned int arg;

    host = (struct LayerHost *)sprite->field_10;
    arg = sprite->field_c;
    ((void(__stdcall *)(struct LayerHost *, unsigned int))(*(void **)((char *)host->vtable + 0x80)))(host, arg);
}

// FUNCTION: LEGOLAND 0x00497de0
LEGO_EXPORT void HideLayer(void *layer, unsigned int flag) { STUB(); }

// FUNCTION: LEGOLAND 0x00497e10
LEGO_EXPORT void ShowLayer(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00497e80
LEGO_EXPORT void GetLayer(struct LayerOwner *owner, struct LayerResult *result, int index) {
    struct LayerArrays *arrays;
    void *layer;

    if ((owner->flags & 0x8000) == 0) {
        return;
    }
    arrays = owner->arrays;
    if ((unsigned int)index >= (unsigned int)arrays->count) {
        return;
    }
    layer = arrays->array_8[index];
    if (layer == NULL) {
        return;
    }
    result->field_0 = layer;
    result->field_4 = owner->arrays->array_c[index];
    result->field_8 = owner->arrays->array_10[index];
}

// FUNCTION: LEGOLAND 0x00497ed0
LEGO_EXPORT void SetLayerAnimatingState(struct LayerOwner *owner, int index, int state) {}

// FUNCTION: LEGOLAND 0x00497ee0
LEGO_EXPORT void TellAllLayersToAnimate(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00497f20
LEGO_EXPORT void TellAllLayersToStopAnimating(struct LayerOwner *owner) {
    int i;

    if ((owner->flags & 0x8000) != 0) {
        i = 0;
        if (owner->arrays->count > 0) {
            do {
                SetLayerAnimatingState(owner, i, 0);
                i++;
            } while (i < owner->arrays->count);
        }
    }
}

// FUNCTION: LEGOLAND 0x00497f60
unsigned int FUN_00497f60(void) {
    int head;
    int tail;

    head = DAT_0079a7dc;
    tail = DAT_0079a7d8;
    if (head >= tail) {
        if (tail == 0) {
            return 0xffff - head;
        }
        return 0x10000 - head;
    }
    return tail - head - 1;
}

// FUNCTION: LEGOLAND 0x00497f90
int FUN_00497f90(void) {
    int head;
    int tail;

    head = DAT_0079a7dc;
    tail = DAT_0079a7d8;
    if (head < tail) {
        head = 0x10000;
    }
    return head - tail;
}

// FUNCTION: LEGOLAND 0x00497fb0
int FUN_00497fb0(void) {
    int used;
    int limit;

    limit = DAT_0079a7e4[0];
    used = (DAT_0079a7dc - DAT_0079a7d8) & 0xffff;
    if (limit == 0) {
        memcpy(DAT_0079a7e4, &DAT_0079a7e4[1], 0x13 * sizeof(unsigned int));
        DAT_0079a7e4[19] = 0;
        if (DAT_0079a7e0 != 0) {
            DAT_0079a7e0--;
        }
    }
    if (used <= limit) {
        return used;
    }
    return limit;
}

// FUNCTION: LEGOLAND 0x00498000
void FUN_00498000(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00498100
void FUN_00498100(void) {
    FUN_00498120();
    DAT_0079a83c |= 8;
}

// FUNCTION: LEGOLAND 0x00498120
void FUN_00498120(void) {
    _lseek(DAT_007caca8, DAT_007cacb4, 0);
    DAT_007cacac = DAT_0079ac04;
}

// FUNCTION: LEGOLAND 0x00498150
void FUN_00498150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004981e0
unsigned int FUN_004981e0(void) {
    int head;
    int tail;

    head = DAT_0079a838;
    tail = DAT_0079a834;
    if (head >= tail) {
        if (tail == 0) {
            return 0x1ffff - head;
        }
        return 0x20000 - head;
    }
    return tail - head - 1;
}

// FUNCTION: LEGOLAND 0x00498210
int FUN_00498210(void) {
    int head;
    int tail;

    head = DAT_0079a838;
    tail = DAT_0079a834;
    if (head < tail) {
        head = 0x20000;
    }
    return head - tail;
}

// FUNCTION: LEGOLAND 0x00498230
unsigned int FUN_00498230(void) {
    return (DAT_0079a838 - DAT_0079a834) & 0x1ffff;
}

// FUNCTION: LEGOLAND 0x00498250
void FUN_00498250(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004983a0
void FUN_004983a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00498420
void FUN_00498420(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00498630
void FUN_00498630(const char *param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x00498870
void FUN_00498870(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004988c0
int FUN_004988c0(void) {
    if (DAT_0079a84c == 0 || DAT_0079a84c == 1) {
        return 0;
    }
    ((struct KLIBAUDIO_Stop *)((struct KLIBAUDIO_Object *)DAT_0079a848)->vtable)->func_48(DAT_0079a848);
    FUN_00498870();
    FUN_00498120();
    DAT_0079a84c = 1;
    return 1;
}

// FUNCTION: LEGOLAND 0x00498900
void FUN_00498900(unsigned int param_1) {
    DAT_0079a7d0 = param_1;
    if (DAT_0079a848 != NULL) {
        ((struct KLIBAUDIO_Vtbl *)((struct KLIBAUDIO_Object *)DAT_0079a848)->vtable)->func_3c(DAT_0079a848, param_1);
    }
}

// FUNCTION: LEGOLAND 0x00498920
void FUN_00498920(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004989b0
void FUN_004989b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00498b00
int FUN_00498b00(void) {
    if (DAT_0079a84c == 0 || DAT_0079a84c == 3) {
        return 0;
    }
    FUN_004989b0();
    ((struct KLIBAUDIO_Vtbl *)((struct KLIBAUDIO_Object *)DAT_0079a848)->vtable)->func_30(DAT_0079a848, 0, 0, 1);
    DAT_0079a84c = 3;
    return 1;
}

// FUNCTION: LEGOLAND 0x00498b40
void FUN_00498b40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00498cf0
int FUN_00498cf0(void) {
    return DAT_0079a84c == 3;
}

// FUNCTION: LEGOLAND 0x00498d00
void FUN_00498d00(void) { STUB(); }
