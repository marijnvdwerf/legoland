#include <windows.h>
#include "legoland.h"
#include "crt.h"
#include "globals.h"

#include "image_sprite.h"
#include "string.h"
#include "gfx.h"
#include "llidb.h"
#include "resource.h"
#include "draw.h"
#include "debug_alloc.h"
#include "timer.h"

struct SpriteListNode;
struct Layer;
struct LayerHost;
struct LayerResult;

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

struct ILFTable {
    unsigned char pad_0[4];
    int count;
    struct Sprite **sprites;
    int *xoffs;
    int *yoffs;
};

// GLOBAL: LEGOLAND 0x0079a7c0
struct Sprite *sprite_list;
// GLOBAL: LEGOLAND 0x0079a7c4
struct Image **detail_images;
// GLOBAL: LEGOLAND 0x0079a7c8
unsigned int detail_images_capacity;
// GLOBAL: LEGOLAND 0x0079a7cc
unsigned int detail_images_count;

// FUNCTION: LEGOLAND 0x00496f20
LEGO_EXPORT void **GetVRAMAddress(struct Sprite *sprite) {
    return &sprite->surface;
}

// FUNCTION: LEGOLAND 0x00496f30
unsigned int *FUN_00496f30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00496fc0
int FUN_00496fc0(struct Image *image) {
    struct Image **slot;

    slot = (struct Image **)FUN_00496f30();
    if (slot != NULL) {
        *slot = image;
        detail_images_count++;
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00496ff0
struct Image **FUN_00496ff0(struct Image *image) {
    int count;
    struct Image **array;
    int i;
    struct Image **entry;
    struct Image *search_val;

    count = detail_images_capacity;
    array = detail_images;
    i = 0;
    entry = array;
    search_val = image;

    if (count <= 0) {
        return NULL;
    }

    while (i < count) {
        if (*entry == search_val) {
            return entry;
        }
        i++;
        entry++;
    }

    return NULL;
}

// FUNCTION: LEGOLAND 0x00497020
int FUN_00497020(struct Image *image) {
    struct Image **entry;

    entry = FUN_00496ff0(image);
    if (entry != NULL) {
        *entry = NULL;
        detail_images_count--;
        if (detail_images_count == 0) {
            free(detail_images);
            detail_images = NULL;
        }
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00497070
LEGO_EXPORT void ReloadAllDetailDependentImages(void) {
    int used;
    int capacity;
    struct Image **array;

    used = detail_images_count;
    capacity = detail_images_capacity;
    array = detail_images;
    while (capacity != 0 && used != 0) {
        if (*array != NULL) {
            ReloadImageBitmapAndBuildSprites(*array);
            used--;
        }
        capacity--;
        array++;
    }
}

// FUNCTION: LEGOLAND 0x004970b0
LEGO_EXPORT void ReloadAllDetailDependentImagesAndBuildSprites(void) {
    struct Sprite *node;
    struct Image **array;
    unsigned int capacity;
    unsigned int used;
    struct Image *image;

    node = sprite_list;
    while (node != NULL) {
        if ((node->flags & 0x400) == 0) {
            struct LayerHost *host = node->surface;
            if (host != NULL) {
                host->vtable->func_8(host);
            }
        }
        node = node->next;
    }

    capacity = detail_images_capacity;
    used = detail_images_count;
    array = detail_images;
    while (capacity != 0 && used != 0) {
        image = *array;
        if (image != NULL) {
            ReloadImageBitmapAndBuildSprites(image);
            used--;
        }
        capacity--;
        array++;
    }
}

// FUNCTION: LEGOLAND 0x00497110
LEGO_EXPORT unsigned int MarkAllSpritesForResizing(void) {
    struct Sprite *node;
    unsigned int count;

    node = sprite_list;
    count = 0;
    while (node != NULL) {
        if (node->image->type == 1) {
            node->flags &= ~0x400;
            count++;
        } else {
            node->flags |= 0x400;
        }
        node = node->next;
    }
    return count;
}

// FUNCTION: LEGOLAND 0x00497150
LEGO_EXPORT void MarkSpriteResized(struct Sprite *sprite) {
    sprite->flags |= 0x400;
}

// FUNCTION: LEGOLAND 0x00497160
LEGO_EXPORT void RemakeAllDetailDependentSprites(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00497280
LEGO_EXPORT struct Image *CreateSourceImage(const char *str, unsigned char type) {
    struct Image *image;

    image = (struct Image *)malloc(sizeof(struct Image) + strlen(str) + 1);
    if (image == NULL) {
        return NULL;
    }
    image->type = type;
    image->refcount = 1;
    image->data = 0;
    image->aux = NULL;
    image->name = (char *)(image + 1);
    strcpy(image->name, str);
    if (type == 1) {
        FUN_00496fc0(image);
    }
    return image;
}

// FUNCTION: LEGOLAND 0x00497300
LEGO_EXPORT struct Image *LoadSourceImage(const char *name, unsigned char type) {
    struct Image *image;

    image = CreateSourceImage(name, type);
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
    if (image->aux != NULL) {
        free(image->aux);
    }
    if (image->data != 0) {
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
            LLSStop((unsigned int)image->data);
        }
        if (image->data != 0) {
            free(image->data);
        }
        if (image->aux != NULL) {
            free(image->aux);
        }
        if (image->type == 1) {
            FUN_00497020(image);
        }
        free(image);
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00497580
struct Sprite *FUN_00497580(void) {
    struct Sprite *sprite;

    sprite = (struct Sprite *)malloc(sizeof(struct Sprite));
    if (sprite != NULL) {
        sprite->next = sprite_list;
        sprite_list = sprite;
    }
    return sprite;
}

// FUNCTION: LEGOLAND 0x004975b0
void FUN_004975b0(struct Sprite *sprite) {
    struct Sprite *current;
    struct Sprite *prev;

    current = sprite_list;
    prev = NULL;
    if (current != sprite) {
        while (current != NULL) {
            prev = current;
            current = current->next;
            if (current == sprite) {
                break;
            }
        }
        if (current == NULL) {
            // STRING: LEGOLAND 0x004bfeb0
            DBPrintf("Couldn't unlink sprite\n");
            free(sprite);
            return;
        }
    }
    if (prev == NULL) {
        sprite_list = sprite->next;
    } else {
        prev->next = sprite->next;
    }
    free(sprite);
}

// FUNCTION: LEGOLAND 0x00497610
LEGO_EXPORT void FreeBitmapResources(struct Image *image) {
    if (image->data != 0) {
        LLSStop((unsigned int)image->data);
        free(image->data);
        image->data = 0;
    }
}

// FUNCTION: LEGOLAND 0x00497640
LEGO_EXPORT struct Sprite *CreateSprite(struct Image *image) {
    struct Sprite *sprite;

    sprite = FUN_00497580();
    if (sprite == NULL) {
        return NULL;
    }
    sprite->surface = NULL;
    sprite->image = image;
    sprite->refcount = 1;
    sprite->src_x = 0;
    sprite->src_y = 0;
    sprite->flags = 0;
    sprite->field_c = DAT_008119a4 - 1;
    if (image != NULL) {
        image->refcount += 1;
        if (image->data == 0) {
            if (__BMPLoader(image) == 0) {
                KillSprite(sprite);
                return NULL;
            }
        }
        sprite->width = image->width;
        sprite->height = image->height;
        FUN_00499500(sprite);
    }
    return sprite;
}

// FUNCTION: LEGOLAND 0x004976c0
LEGO_EXPORT struct Sprite *CreateFunctionBasedSprite(int (*source)(struct Sprite *), unsigned short a, unsigned short b) {
    struct Sprite *sprite;

    sprite = FUN_00497580();
    if (sprite == NULL) {
        return NULL;
    }
    sprite->surface = NULL;
    sprite->render_fn = source;
    sprite->refcount = 1;
    sprite->src_x = 0;
    sprite->src_y = 0;
    sprite->flags = 0x30;
    sprite->field_c = DAT_008119a4 - 1;
    sprite->width = a;
    sprite->height = b;
    return sprite;
}

// FUNCTION: LEGOLAND 0x00497710
LEGO_EXPORT struct Sprite *CreateSysmemSprite(struct Image *image) {
    struct Sprite *sprite;

    sprite = FUN_00497580();
    if (sprite == NULL) {
        return NULL;
    }
    sprite->surface = NULL;
    sprite->image = image;
    image->refcount += 1;
    sprite->refcount = 1;
    sprite->src_x = 0;
    sprite->src_y = 0;
    sprite->flags = 0x10;
    sprite->field_c = DAT_008119a4 - 1;
    if (image->data == 0) {
        if (__BMPLoader(image) == 0) {
            KillSprite(sprite);
            return NULL;
        }
    }
    sprite->width = image->width;
    sprite->height = image->height;
    return sprite;
}

// FUNCTION: LEGOLAND 0x00497790
LEGO_EXPORT struct Sprite *CreatePartialSprite(struct Image *image, unsigned short a, unsigned short b, unsigned short c, unsigned short d) {
    struct Sprite *sprite;

    sprite = FUN_00497580();
    if (sprite == NULL) {
        return NULL;
    }
    sprite->surface = NULL;
    sprite->image = image;
    image->refcount += 1;
    sprite->src_x = a;
    sprite->refcount = 1;
    sprite->src_y = b;
    sprite->flags = 0;
    sprite->field_c = DAT_008119a4 - 1;
    if (image->data == 0) {
        if (__BMPLoader(image) == 0) {
            KillSprite(sprite);
            return NULL;
        }
    }
    sprite->width = c;
    sprite->height = d;
    return sprite;
}

// FUNCTION: LEGOLAND 0x00497810
LEGO_EXPORT int RecreatePartialSprite(struct Sprite *sprite, struct Image *image, unsigned short a, unsigned short b, unsigned short c, unsigned short d) {
    struct LayerHost *host;
    int had_host;

    ReferenceImage(image);
    KillImage(sprite->image);
    host = sprite->surface;
    if (host != NULL) {
        host->vtable->func_8(host);
        had_host = 1;
    } else {
        had_host = 0;
    }
    sprite->src_x = a;
    sprite->surface = NULL;
    sprite->image = image;
    sprite->src_y = b;
    sprite->flags = 0;
    sprite->width = c;
    sprite->height = d;
    if (image->data == 0) {
        if (__BMPLoader(image) == 0) {
            return 0;
        }
    }
    if (had_host != 0) {
        FUN_00499500(sprite);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004978b0
int FUN_004978b0(struct Sprite *sprite, const char *name, unsigned int flags) {
    struct ILFTable *table;
    struct ResFile *file;
    struct Image *image;
    struct Sprite *element;
    int count;
    int length;
    int i;
    char tag[2];
    char path[512];
    char element_name[512];

    i = 0;
    count = 0;
    if (sprite == NULL) {
        return 0;
    }
    // STRING: LEGOLAND 0x004bfec8
    sprintf(path, ".\\CompSprite\\%s", name);
    file = RES_OpenFile(path);
    if (file == NULL) {
        return 0;
    }
    table = (struct ILFTable *)malloc(0x24);
    if (table != NULL) {
        RES_ReadFile(file, &count, 2);
        RES_ReadFile(file, tag, 2);
        table->xoffs = (int *)malloc(count * sizeof(int));
        table->yoffs = (int *)malloc(count * sizeof(int));
        table->sprites = (struct Sprite **)malloc(count * sizeof(struct Sprite *));
        table->count = count;
        if (table->xoffs != NULL && table->yoffs != NULL && table->sprites != NULL) {
            RES_ReadFile(file, &length, 4);
            RES_ReadFile(file, element_name, length);
            if (count > 0) {
                do {
                    RES_ReadFile(file, &table->xoffs[i], 4);
                    RES_ReadFile(file, &table->yoffs[i], 4);
                    i++;
                } while (i < count);
            }
            i = 0;
            if (count > 0) {
                do {
                    RES_ReadFile(file, &length, 4);
                    RES_ReadFile(file, path, length);
                    path[length] = 0;
                    table->sprites[i] = LoadSprite(path, flags);
                    element = table->sprites[i];
                    if (element != NULL) {
                        image = element->image;
                        if (image->field_14 == 2 || image->field_14 == 3) {
                            LLSPlay((struct LLS *)image->data, (unsigned int)image);
                        }
                    }
                    i++;
                } while (i < count);
            }
            RES_CloseFile(file);
            sprite->image = (struct Image *)table;
            sprite->flags |= 0x8000;
            return 1;
        }
    }
    LLIDB_FreeILFTable((struct ILFTable *)table);
    RES_CloseFile(file);
    return 0;
}

// FUNCTION: LEGOLAND 0x00497ab0
LEGO_EXPORT struct Sprite *LoadSprite(const char *name, int flags) {
    char ext[256];
    struct Image *image;
    struct Sprite *sprite;
    struct Sprite *result;

    result = NULL;
    _splitpath(name, NULL, NULL, NULL, ext);
    // STRING: LEGOLAND 0x004bfed8
    if (_stricmp(ext, ".csp") == 0) {
        sprite = CreateSprite(NULL);
        if (sprite != NULL) {
            if (FUN_004978b0(sprite, name, flags & 0xff) == 0) {
                KillSprite(sprite);
                return NULL;
            }
        }
        return sprite;
    }
    image = CreateSourceImage(name, (unsigned char)flags);
    if (image != NULL) {
        result = CreateSprite(image);
        KillImage(image);
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00497b70
LEGO_EXPORT unsigned int MakeSprite(struct Sprite *sprite) {
    struct Image *image;

    if ((sprite->flags & 0x20) != 0) {
        PushSetTarget(sprite);
        sprite->render_fn(sprite);
        PopTarget();
        return 1;
    }
    image = sprite->image;
    if (image->data != 0 || __BMPLoader(image) != 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00497bb0
LEGO_EXPORT short ReferenceSprite(struct Sprite *sprite) {
    if (sprite == NULL) {
        return -1;
    }
    sprite->refcount++;
    return sprite->refcount;
}

// FUNCTION: LEGOLAND 0x00497bd0
LEGO_EXPORT int KillSprite(struct Sprite *sprite) {
    struct LayerHost *host;

    if (sprite != NULL) {
        sprite->refcount--;
        if (sprite->refcount == 0) {
            host = sprite->surface;
            if (host != NULL) {
                host->vtable->func_8(host);
            }
            if ((sprite->flags & 0x20) == 0) {
                if ((sprite->flags & 0x8000) != 0) {
                    LLIDB_FreeILFTable((struct ILFTable *)sprite->image);
                } else if (sprite->image != NULL) {
                    KillImage(sprite->image);
                }
            }
            FUN_004975b0(sprite);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00497c30
LEGO_EXPORT void GetSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00497d90
LEGO_EXPORT void SetSubSpriteSource(struct Sprite *sprite, unsigned short a, unsigned short b, unsigned short c, unsigned short d) {
    sprite->src_x = a;
    sprite->src_y = b;
    sprite->width = c;
    sprite->height = d;
}

// FUNCTION: LEGOLAND 0x00497dc0
LEGO_EXPORT void ReleaseSprite(struct Sprite *sprite) {
    struct LayerHost *host;
    unsigned int arg;

    host = sprite->host;
    arg = sprite->field_c;
    ((void(__stdcall *)(struct LayerHost *, unsigned int))(*(void **)((char *)host->vtable + 0x80)))(host, arg);
}

// FUNCTION: LEGOLAND 0x00497de0
LEGO_EXPORT void HideLayer(struct LayerOwner *owner, unsigned int index) {
    struct Sprite *layer;

    if ((owner->flags & 0x8000) != 0) {
        if (index < (unsigned int)owner->arrays->count) {
            layer = (struct Sprite *)owner->arrays->array_8[index];
            if (layer != NULL) {
                layer->flags |= 0x4000;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00497e10
LEGO_EXPORT void ShowLayer(struct LayerOwner *owner, unsigned int index) {
    struct Sprite *layer;

    if ((owner->flags & 0x8000) != 0) {
        if (index < (unsigned int)owner->arrays->count) {
            layer = (struct Sprite *)owner->arrays->array_8[index];
            if (layer != NULL) {
                layer->flags &= ~0x4000;
            }
        }
    }
}

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
LEGO_EXPORT void TellAllLayersToAnimate(struct LayerOwner *owner) {
    int i;

    if ((owner->flags & 0x8000) != 0) {
        i = 0;
        if (owner->arrays->count > 0) {
            do {
                SetLayerAnimatingState(owner, i, 1);
                i++;
            } while (i < owner->arrays->count);
        }
    }
}

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


