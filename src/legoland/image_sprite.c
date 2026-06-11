#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "debug_alloc.h"
#include "draw.h"
#include "gfx.h"
#include "image_sprite.h"
#include "llidb.h"
#include "resource.h"
#include "string.h"
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
struct Image **FUN_00496f30(void) {
    struct Image **new_array;
    int i;

    if (detail_images_count == detail_images_capacity) {
        if (detail_images == NULL) {
            new_array = (struct Image **)calloc(0x80, 4);
        } else {
            new_array = (struct Image **)realloc(detail_images, detail_images_capacity * 4 + 0x200);
            if (new_array == NULL) {
                return NULL;
            }
            memset(new_array + detail_images_capacity, 0, 0x80 * sizeof(struct Image *));
        }
        if (new_array != NULL) {
            detail_images = new_array;
            detail_images_capacity += 0x80;
            return new_array + detail_images_capacity - 0x80;
        }
    } else {
        new_array = detail_images;
        i = 0;
        if (0 < (int)detail_images_capacity) {
            while (i < (int)detail_images_capacity) {
                if (*new_array == NULL) {
                    return new_array;
                }
                i++;
                new_array++;
            }
        }
    }
    return NULL;
}

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
LEGO_EXPORT void RemakeAllDetailDependentSprites(void) {
    unsigned short uVar1;
    int iVar3;
    struct Sprite *sprite;
    struct Sprite *node;

    sprite = sprite_list;
    node = sprite_list;
    if (sprite_list != NULL) {
        do {
            if ((node->flags & 0x400) == 0) {
                struct LayerHost *host = node->surface;
                if (host != NULL) {
                    host->vtable->func_8(host);
                    sprite = sprite_list;
                }
            }
            node = node->next;
        } while (node != NULL);
        for (; sprite != NULL; sprite = sprite->next) {
            if ((sprite->flags & 0x400) == 0) {
                *(short *)&sprite->src_x >>= 1;
                uVar1 = sprite->src_x;
                *(short *)&sprite->width >>= 1;
                *(short *)&sprite->height >>= 1;
                *(short *)&sprite->src_y >>= 1;
                if ((short)uVar1 < 0) {
                    sprite->src_x = 0;
                    sprite->width = sprite->width + uVar1;
                }
                iVar3 = (int)sprite->image->width;
                if ((int)(short)sprite->src_x + (int)(short)sprite->width > iVar3) {
                    if ((int)(short)sprite->src_x > iVar3 - 1) {
                        sprite->src_x = sprite->image->width - 1;
                    }
                    sprite->width = sprite->image->width - sprite->src_x;
                }
                if ((short)sprite->src_y < 0) {
                    uVar1 = sprite->src_y;
                    sprite->src_y = 0;
                    sprite->height = sprite->height + uVar1;
                }
                iVar3 = (int)sprite->image->height;
                if ((int)(short)sprite->src_y + (int)(short)sprite->height > iVar3) {
                    if ((int)(short)sprite->src_y > iVar3 - 1) {
                        sprite->src_y = sprite->image->height - 1;
                    }
                    sprite->height = sprite->image->height - sprite->src_y;
                }
                if (sprite->surface != 0) {
                    sprite->surface = 0;
                    FUN_00499500(sprite);
                }
            }
            MarkSpriteResized(sprite);
        }
    }
}

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
LEGO_EXPORT void ReloadImageBitmapAndBuildSprites(struct Image *image) {
    unsigned short uVar1;
    struct Sprite *sprite;
    int bVar4;
    int iVar5;
    unsigned int local_buf[6];

    bVar4 = 0;
    if ((image->field_14 == 2 || image->field_14 == 3) &&
        LLSStop((unsigned int)image->data) != 0) {
        bVar4 = 1;
        memcpy(local_buf, image->data, 24);
    }
    if (image->aux != NULL) {
        free(image->aux);
    }
    FreeBitmapResources(image);
    iVar5 = __BMPLoader(image);
    if (iVar5 == 0) {
        return;
    }
    if (bVar4) {
        *(unsigned short *)image->data = (unsigned short)local_buf[0];
        ((unsigned short *)image->data)[9] = *(unsigned short *)((char *)local_buf + 18);
        ((unsigned int *)image->data)[5] = ((unsigned int *)image->data)[5] | (local_buf[5] & 4);
        LLSPlay((struct LLS *)image->data, (unsigned int)image);
    }
    sprite = sprite_list;
    while (sprite->image != image) {
        sprite = sprite->next;
    }
    if ((sprite->flags & 0x400) == 0 && sprite->image == image) {
        *(short *)&sprite->src_x >>= 1;
        uVar1 = sprite->src_x;
        *(short *)&sprite->width >>= 1;
        *(short *)&sprite->height >>= 1;
        *(short *)&sprite->src_y >>= 1;
        if ((short)uVar1 < 0) {
            sprite->src_x = 0;
            sprite->width = sprite->width + uVar1;
        }
        iVar5 = (int)sprite->image->width;
        if ((int)(short)sprite->src_x + (int)(short)sprite->width > iVar5) {
            if ((int)(short)sprite->src_x > iVar5 - 1) {
                sprite->src_x = sprite->image->width - 1;
            }
            sprite->width = sprite->image->width - sprite->src_x;
        }
        if ((short)sprite->src_y < 0) {
            uVar1 = sprite->src_y;
            sprite->src_y = 0;
            sprite->height = sprite->height + uVar1;
        }
        iVar5 = (int)sprite->image->height;
        if ((int)(short)sprite->src_y + (int)(short)sprite->height > iVar5) {
            if ((int)(short)sprite->src_y > iVar5 - 1) {
                sprite->src_y = sprite->image->height - 1;
            }
            sprite->height = sprite->image->height - sprite->src_y;
        }
        FUN_00499500(sprite);
        MarkSpriteResized(sprite);
    }
}

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
LEGO_EXPORT int GetSprite(unsigned int *param_1, struct Sprite *param_2) {
    int iVar1;
    unsigned int locals[4];
    unsigned char surfDesc[0x6c];

    locals[0] = 0;
    locals[1] = 0;
    locals[2] = 0;
    locals[3] = 0;
    *(unsigned int *)surfDesc = 0x6c;
    if (param_2 != NULL) {
        locals[2] = (unsigned int)(short)param_2->width;
        param_1[1] = locals[2];
        locals[3] = (unsigned int)(short)param_2->height;
        param_1[2] = locals[3];
        param_1[4] = (unsigned int)param_2->surface;
        iVar1 = (*(int(__stdcall **)(void *, unsigned int *, unsigned char *, int, int))(*(unsigned int *)param_2->surface + 0x64))(
            param_2->surface, locals, surfDesc, 1, 0);
        if (iVar1 == (int)0x887601c2) {
            (*(void(__stdcall **)(void *))(*(unsigned int *)param_2->surface + 0x6c))(param_2->surface);
            iVar1 = (*(int(__stdcall **)(void *, unsigned int *, unsigned char *, int, int))(*(unsigned int *)param_2->surface + 0x64))(
                param_2->surface, locals, surfDesc, 1, 0);
        }
        if (iVar1 != 0) {
            return 0;
        }
    } else {
        locals[2] = (unsigned int)*(unsigned short *)lpConfig;
        param_1[1] = locals[2];
        locals[3] = (unsigned int)*((unsigned short *)lpConfig + 1);
        param_1[2] = locals[3];
        param_1[4] = (unsigned int)DDRAWENV[194];
        iVar1 = (*(int(__stdcall **)(void *, unsigned int *, unsigned char *, int, int))(*(unsigned int *)DDRAWENV[194] + 0x64))(
            (void *)DDRAWENV[194], locals, surfDesc, 1, 0);
        if (iVar1 == (int)0x887601c2) {
            (*(void(__stdcall **)(void *))(*(unsigned int *)DDRAWENV[194] + 0x6c))((void *)DDRAWENV[194]);
            iVar1 = (*(int(__stdcall **)(void *, unsigned int *, unsigned char *, int, int))(*(unsigned int *)DDRAWENV[194] + 0x64))(
                (void *)DDRAWENV[194], locals, surfDesc, 1, 0);
        }
        if (iVar1 != 0) {
            return 0;
        }
    }
    *param_1 = *(unsigned int *)(surfDesc + 0x10);
    param_1[3] = *(unsigned int *)(surfDesc + 0x24);
    if (DAT_00668088 == 0) {
        param_1[5] = 1;
    } else {
        param_1[5] = 2;
    }
    return 1;
}

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
