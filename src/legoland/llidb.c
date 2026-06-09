#include "legoland.h"
#include <windows.h>

#include "crt.h"

#include "llidb.h"
#include "tilemap.h"
#include "globals.h"
#include "wndenv.h"
#include "objclass.h"
#include "resource.h"
#include "render3d.h"
#include "debug.h"
#include "setcustomcallbacks.h"

#include "image_sprite.h"

struct ILFTable {
    unsigned char pad_0[4];
    int count;
    unsigned int *sprites;
    int *data_c;
    int *data_10;
    void *data_14;
};

struct SpriteManager {
    unsigned int var_0;
    int count;
    unsigned int *sprites;
    int *data_c;
    int *data_10;
    unsigned int data_14;
    void *data_18;
    void *data_1c;
    void *data_20;
};

struct LLSImage {
    unsigned char pad_0[0xc];
    unsigned int format;
    short height;
    unsigned char pad_12[0x2];
    unsigned char flags;
};

struct LLIDBHead {
    unsigned char pad_0[4];
    const char *name;
    unsigned int flags;
    void *data;
    int refcount;
};

struct ODFAnim {
    unsigned int handle;
    unsigned char pad_4[0x14 - 0x4];
    unsigned int type;
};

struct ODFSprite {
    unsigned char pad_0[8];
    struct ODFAnim *anim;
};

struct ODFObject {
    struct ODFObject *next;
    int data_4;
    unsigned int data_8;
    unsigned char pad_c[0x1c - 0xc];
    unsigned int flags;
    unsigned char pad_20[0x2a - 0x20];
    short order;
    unsigned char pad_2c[0x4c - 0x2c];
    unsigned int data_4c;
    void *desc;
    void *script;
    unsigned int elem_58;
    unsigned int elem_5c;
    unsigned int elem_60;
    struct ODFSprite *sprite_0;
    struct ODFSprite *sprite_1;
    struct ODFSprite *sprite_2;
    unsigned int handle;
    unsigned int data_74;
    void *data_78;
    void *data_7c;
    void *data_80;
    unsigned char pad_84[0xa4 - 0x84];
    void (*callback)(void *);
    unsigned char pad_a8[0xac - 0xa8];
    void (*cleanup)(void *);
    unsigned char pad_b0[0xc4 - 0xb0];
    void *cleanup_arg;
};

// FUNCTION: LEGOLAND 0x0047aff0
LEGO_EXPORT void LLIDB_LoadICM(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047b2d0
LEGO_EXPORT unsigned int LLIDB_GetCount(void) {
    return DAT_006691a4;
}

// FUNCTION: LEGOLAND 0x0047b2e0
LEGO_EXPORT int LLIDB_GetElement(unsigned int index, unsigned int *output) {
    if (index < DAT_006691a4) {
        if (output != NULL) {
            *output = (unsigned int)LLIDB_ELEM(index);
        }
        return 0;
    }
    if (output != NULL) {
        *output = 0;
    }
    return LLIDB_ERR_NOTFOUND;
}

// FUNCTION: LEGOLAND 0x0047b330
LEGO_EXPORT int LLIDB_FindElement(const char *name, unsigned int *out, unsigned int *index_out) {
    unsigned int i;

    if (name == NULL) {
        if (out != NULL) {
            *out = 0;
        }
        if (index_out != NULL) {
            *index_out = 0;
            return LLIDB_ERR_NOTFOUND;
        }
    } else {
        for (i = 0; i < DAT_006691a4; i++) {
            if (_stricmp(name, LLIDB_ELEM(i)->name) == 0) {
                if (out != NULL) {
                    *out = (unsigned int)LLIDB_ELEM(i);
                }
                if (index_out != NULL) {
                    *index_out = i;
                }
                return 0;
            }
        }
        if (out != NULL) {
            *out = 0;
        }
        if (index_out != NULL) {
            *index_out = 0;
        }
    }
    return LLIDB_ERR_NOTFOUND;
}

// FUNCTION: LEGOLAND 0x0047b3f0
LEGO_EXPORT unsigned int ElemID(const char *name) {
    LLIDB_FindElement(name, (unsigned int *)&name, NULL);
    return (unsigned int)name;
}

// FUNCTION: LEGOLAND 0x0047b410
LEGO_EXPORT int LLIDB_FindElementFromDataPtr(void *data, unsigned int *out, unsigned int *index_out) {
    unsigned int i;

    if (data == NULL) {
        if (out != NULL) {
            *out = 0;
        }
        if (index_out != NULL) {
            *index_out = 0;
            return LLIDB_ERR_NOTFOUND;
        }
    } else {
        for (i = 0; i < DAT_006691a4; i++) {
            if (data == LLIDB_ELEM(i)->data) {
                if (out != NULL) {
                    *out = (unsigned int)LLIDB_ELEM(i);
                }
                if (index_out != NULL) {
                    *index_out = i;
                }
                return 0;
            }
        }
        if (out != NULL) {
            *out = 0;
        }
        if (index_out != NULL) {
            *index_out = 0;
        }
    }
    return LLIDB_ERR_NOTFOUND;
}

// FUNCTION: LEGOLAND 0x0047b4c0
LEGO_EXPORT void LLIDB_ClearOnLevel(void) {
    unsigned int i = 0;

    if (DAT_006691a4 > 0) {
        do {
            LLIDB_ELEM(i)->flags &= ~LLIDB_FLAG_LEVEL;
            i++;
        } while (i < DAT_006691a4);
    }
}

// FUNCTION: LEGOLAND 0x0047b500
unsigned int FUN_0047b500(unsigned int param_1) {
    unsigned int slot;
    unsigned int page;
    unsigned int count;
    int offset;
    int page_base;

    count = DAT_006691a4;
    if (param_1 < count) {
        page = param_1 >> 8;
        slot = param_1 & 0xff;
        if (page <= count >> 8) {
            do {
                if (slot == 0xffffffff) {
                    DAT_006691a8[page - 1][0xff] = DAT_006691a8[page][0];
                    slot = 0;
                } else if ((int)slot >= 0xfe) {
                    slot = 0xffffffff;
                    page++;
                    continue;
                }
                offset = slot * 0x14;
                do {
                    page_base = (int)DAT_006691a8[page];
                    *(struct Element *)(page_base + offset) = *(struct Element *)(page_base + offset + 0x14);
                    offset += 0x14;
                    count = DAT_006691a4;
                } while (offset < 0x13d8);
                slot = 0xffffffff;
                page++;
            } while (page <= count >> 8);
        }
        DAT_006691a4 = count - 1;
        return 0;
    }
    return (unsigned int)LLIDB_ERR_NOTFOUND;
}

// FUNCTION: LEGOLAND 0x0047b5a0
unsigned int FUN_0047b5a0(void) {
    unsigned int capacity;

    if (((DAT_006691a0 ^ DAT_006691a4) & 0xffffff00) == 0) {
        capacity = (DAT_006691a4 + 0x100) & 0xffffff00;
        DAT_006691a0 = capacity;
        DAT_006691a8 = (struct Element **)realloc(DAT_006691a8, (capacity >> 8) * 4);
        DAT_006691a8[(DAT_006691a0 >> 8) - 1] = (struct Element *)malloc(0x1400);
    }
    return DAT_006691a4;
}

// FUNCTION: LEGOLAND 0x0047b610
LEGO_EXPORT unsigned int LLIDB_RegisterNewElement(const char *param_1, const char *param_2, unsigned int param_3) {
    unsigned int index;
    char *page_off;
    unsigned int slot_off;
    char *copy;
    unsigned int found;

    if (param_1 == NULL || param_1[0] == '\0') {
        return (unsigned int)LLIDB_ERR_NOKEY;
    }
    if ((param_2 == NULL || param_2[0] == '\0') && param_3 != LLIDB_TYPE_NOFILE) {
        return (unsigned int)LLIDB_ERR_NOFILE;
    }

    if (LLIDB_FindElement(param_1, &found, NULL) == 0) {
        if (param_3 != LLIDB_TYPE_NOFILE && _stricmp(param_2, ((struct Element *)found)->path) != 0) {
            return (unsigned int)LLIDB_ERR_MISMATCH;
        }
        return 0;
    }

    index = FUN_0047b5a0();
    page_off = (char *)((index >> 8) * 4);
    slot_off = (index & 0xff) * 20;

    copy = (char *)malloc(strlen(param_1) + 1);
    *(char **)(*(char **)(page_off + (unsigned int)DAT_006691a8) + slot_off) = copy;
    strcpy(*(char **)(*(char **)(page_off + (unsigned int)DAT_006691a8) + slot_off), param_1);

    if (param_2 == NULL) {
        copy = (char *)malloc(1);
        *(char **)(*(char **)(page_off + (unsigned int)DAT_006691a8) + slot_off + 4) = copy;
        *(*(char **)(*(char **)(page_off + (unsigned int)DAT_006691a8) + slot_off + 4)) = '\0';
    } else {
        copy = (char *)malloc(strlen(param_2) + 1);
        *(char **)(*(char **)(page_off + (unsigned int)DAT_006691a8) + slot_off + 4) = copy;
        strcpy(*(char **)(*(char **)(page_off + (unsigned int)DAT_006691a8) + slot_off + 4), param_2);
    }

    *(unsigned int *)(*(char **)(page_off + (unsigned int)DAT_006691a8) + slot_off + 8) = param_3 & LLIDB_TYPE_MASK;
    *(unsigned int *)(*(char **)(page_off + (unsigned int)DAT_006691a8) + slot_off + 0x10) = 0;

    DAT_006691a4++;
    return 0;
}

// FUNCTION: LEGOLAND 0x0047b7b0
void FUN_0047b7b0(int param_1) {
    // STRING: LEGOLAND 0x004bc208
    const char *title = "LEGOLAND Installation & Configuration Manager";
    switch (param_1) {
    case LLIDB_ERR_MISMATCH:
        // STRING: LEGOLAND 0x004bc1dc
        MessageBoxA(NULL, "An element of the same name already exists.", title, 0x30);
        break;
    case LLIDB_ERR_ICMWRITE:
        // STRING: LEGOLAND 0x004bc1ac
        MessageBoxA(NULL, "LEGOLAND.ICM could not be created or updated.", title, 0x30);
        break;
    case LLIDB_ERR_NOTFOUND:
        // STRING: LEGOLAND 0x004bc184
        MessageBoxA(NULL, "The given element could not be found.", title, 0x30);
        break;
    case LLIDB_ERR_NOKEY:
        // STRING: LEGOLAND 0x004bc160
        MessageBoxA(NULL, "No identification key was given.", title, 0x30);
        break;
    case LLIDB_ERR_NOFILE:
        // STRING: LEGOLAND 0x004bc148
        MessageBoxA(NULL, "No file name was given.", title, 0x30);
        break;
    case LLIDB_ERR_CANCELED:
        // STRING: LEGOLAND 0x004bc130
        MessageBoxA(NULL, "Operation was canceled.", title, 0x30);
        break;
    }
}

// FUNCTION: LEGOLAND 0x0047b860
LEGO_EXPORT unsigned int LLIDB_RegisterNewElementB(const char *param_1, const char *param_2, unsigned int param_3) {
    unsigned int result = LLIDB_RegisterNewElement(param_1, param_2, param_3);
    if (result != 0) {
        FUN_0047b7b0(result);
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0047b890
INT_PTR CALLBACK FUN_0047b890(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) { STUB(); }

// FUNCTION: LEGOLAND 0x0047bc20
LEGO_EXPORT int LLIDB_SelectElement(unsigned int mask, unsigned int *output) {
    INT_PTR result;

    DAT_007fdb88 = mask;
    result = DialogBoxParamA((HINSTANCE)WNDENV_GethInstance(), MAKEINTRESOURCEA(0x75), GetDesktopWindow(), FUN_0047b890, 0);
    if (result != -1) {
        if (result != 1) {
            return LLIDB_ERR_CANCELED;
        }
        if (output != NULL) {
            LLIDB_GetElement(DAT_007fdca0, output);
        }
        return 0;
    }
    return -1;
}

// FUNCTION: LEGOLAND 0x0047bc80
LEGO_EXPORT void LLIDB_SaveICM(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047be00
LEGO_EXPORT void LLIDB_CloseICM(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047bef0
LEGO_EXPORT void LLIDB_FreeILFTable(struct ILFTable *table) {
    if (table == NULL) {
        return;
    }

    if (table->data_c != NULL) {
        free(table->data_c);
    }
    if (table->data_10 != NULL) {
        free(table->data_10);
    }

    if (table->sprites != NULL) {
        int i = 0;
        if (table->count > 0) {
            do {
                if (table->sprites[i] != 0) {
                    KillSprite(table->sprites[i]);
                    table->sprites[i] = 0;
                }
                i++;
            } while (i < table->count);
        }
        free(table->sprites);
    }

    free(table);
}

// FUNCTION: LEGOLAND 0x0047bf70
LEGO_EXPORT void *LLIDB_LoadODFData(struct LLIDBHead *head) {
    char name[0x100];
    char text[0x100];
    char filename[0x100];
    char dllname[0x100];
    unsigned int size;
    unsigned int element;
    int lang_count;
    struct ResFile *file;
    struct ODFObject *obj;
    unsigned int *sprite_anim;
    int sprite;
    int i;
    int j;

    DAT_00813a10 = 0;
    // STRING: LEGOLAND 0x004bc39c
    sprintf(filename, "Objdesc\\%s", head->name);
    file = RES_OpenFile(filename);
    if (file == NULL) {
        return NULL;
    }

    obj = (struct ODFObject *)malloc(0xd0);
    memset(obj, 0, 0xd0);
    if (obj == NULL) {
        return NULL;
    }

    obj->cleanup_arg = head;
    head->data = obj;
    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, &obj->data_4, size - 4);
    obj->data_4c = 0;
    obj->next = (struct ODFObject *)ObjectClassList;
    ObjectClassList = obj;
    lang_count = obj->data_4;
    obj->data_4 = 0;
    obj->data_8 = 0;

    RES_ReadFile(file, &size, 4);
    if (size != 0) {
        obj->desc = malloc(size);
        RES_ReadFile(file, obj->desc, size);
        free(obj->desc);
    } else {
        obj->desc = NULL;
    }

    RES_ReadFile(file, &size, 4);
    if (size != 0) {
        obj->script = malloc(size);
        RES_ReadFile(file, obj->script, size);
        free(obj->script);
    } else {
        obj->script = NULL;
    }

    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, dllname, size);
    dllname[size] = '\0';

    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, name, size);
    name[size] = '\0';
    if (name[0] != '\0') {
        LLIDB_FindElement(name, &element, NULL);
        obj->elem_58 = element;
    } else {
        obj->elem_58 = 0;
    }

    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, text, size);
    text[size] = '\0';

    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, name, size);
    name[size] = '\0';
    if (name[0] != '\0') {
        LLIDB_FindElement(name, &element, NULL);
        obj->elem_5c = element;
    } else {
        obj->elem_5c = 0;
    }

    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, name, size);
    name[size] = '\0';
    if (name[0] != '\0') {
        LLIDB_FindElement(name, &element, NULL);
        obj->elem_60 = element;
    } else {
        obj->elem_60 = 0;
    }

    NEWFLC_PauseType = 1;
    NEWFLC_AutoPlay = 1;
    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, name, size);
    name[size] = '\0';
    if ((obj->flags & LLIDB_FLAG_NODATA) == 0) {
        if (name[0] != '\0') {
            sprite = LoadSprite(name, 1);
            obj->sprite_0 = (struct ODFSprite *)sprite;
            if (sprite != 0 && (sprite_anim = *(unsigned int **)(sprite + 8)) != NULL) {
                if ((*(unsigned int *)(sprite + 0x10) & 0x8000) == 0 &&
                    (sprite_anim[5] == 2 || sprite_anim[5] == 3)) {
                    LLSPlay((struct LLS *)*sprite_anim, (unsigned int)sprite_anim);
                }
                obj->flags |= 4;
            }
            goto icon;
        }
        // STRING: LEGOLAND 0x004bc37c
        FUN_0047f870("Class %s has no sprite name.", *(char **)obj->cleanup_arg);
    }
    obj->sprite_0 = NULL;

icon:
    NEWFLC_PauseType = 2;
    NEWFLC_AutoPlay = 0;
    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, name, size);
    name[size] = '\0';
    if (name[0] != '\0') {
        obj->sprite_1 = (struct ODFSprite *)LoadSprite(name, 4);
    } else {
        // STRING: LEGOLAND 0x004bc360
        FUN_0047f870("Class %s has no icon name.", *(char **)obj->cleanup_arg);
        obj->sprite_1 = NULL;
    }
    if (obj->sprite_1 == NULL) {
        // STRING: LEGOLAND 0x004bc34c
        obj->sprite_1 = (struct ODFSprite *)LoadSprite("InstituteIcon.lls", 4);
    }

    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, name, size);
    name[size] = '\0';
    if (name[0] != '\0') {
        sprite = LoadSprite(name, 1);
        obj->sprite_2 = (struct ODFSprite *)sprite;
        if (sprite != 0 && (*(unsigned int *)(sprite + 0x10) & 0x8000) != 0 &&
            *(int *)(*(int *)(sprite + 8) + 4) > 0) {
            j = 0;
            do {
                sprite = GetSpriteForLayer((struct LayerContainer *)obj->sprite_2, j);
                if (sprite != 0 && (sprite = GetLLSForSprite((struct SpriteLLS *)sprite)) != 0) {
                    LLSStop(sprite);
                }
                j++;
            } while (j < *(int *)(*(int *)((char *)obj->sprite_2 + 8) + 4));
        }
    } else {
        // STRING: LEGOLAND 0x004bc328
        FUN_0047f870("Class %s has no build anim name.", *(char **)obj->cleanup_arg);
        obj->sprite_2 = NULL;
    }

    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, name, size);
    name[size] = '\0';
    obj->handle = 0;
    if (name[0] != '\0' && (obj->flags & LLIDB_FLAG_NODATA) == 0) {
        LLIDB_FindElement(name, &element, NULL);
        obj->handle = element;
        LLIDB_LoadData((void *)element);
    }

    i = 0;
    if (lang_count > 0) {
        do {
            RES_ReadFile(file, &size, 4);
            RES_ReadFile(file, name, size);
            name[size] = '\0';
            // STRING: LEGOLAND 0x004bc0ec
            if (i == 0 || _stricmp("english", name) == 0) {
                if (obj->data_78 != NULL) {
                    free(obj->data_78);
                }
                RES_ReadFile(file, &size, 4);
                obj->data_78 = malloc(size + 1);
                RES_ReadFile(file, obj->data_78, size);
                ((char *)obj->data_78)[size] = '\0';
                if (obj->data_7c != NULL) {
                    free(obj->data_7c);
                }
                RES_ReadFile(file, &size, 4);
                obj->data_7c = malloc(size + 1);
                RES_ReadFile(file, obj->data_7c, size);
                ((char *)obj->data_7c)[size] = '\0';
                if (obj->data_80 != NULL) {
                    free(obj->data_80);
                }
                RES_ReadFile(file, &size, 4);
                obj->data_80 = malloc(size + 1);
                RES_ReadFile(file, obj->data_80, size);
                ((char *)obj->data_80)[size] = '\0';
            } else {
                RES_ReadFile(file, &size, 4);
                RES_ReadFile(file, name, size);
                RES_ReadFile(file, &size, 4);
                RES_ReadFile(file, name, size);
                RES_ReadFile(file, &size, 4);
                RES_ReadFile(file, name, size);
            }
            i++;
        } while (i < lang_count);
    }

    obj->data_74 = 0;
    RES_CloseFile(file);
    SetStandardCallbacks((struct CallbackTable *)obj);
    if ((obj->flags & LLIDB_FLAG_USEDLL) == 0 || text[0] == '\0') {
        SetCustomCallbacks(obj->cleanup_arg);
    } else if (LoadObjectLibrary(obj, text) == 0) {
        obj->flags &= ~LLIDB_FLAG_USEDLL;
        SetCustomCallbacks(obj->cleanup_arg);
        if (obj->callback != NULL) {
            obj->callback(obj->cleanup_arg);
        }
        // STRING: LEGOLAND 0x004bc2ec
        FUN_0047f870("Class %s has OC_USEDLL attribute and DLL failed to load.", *(char **)obj->cleanup_arg);
    }

    if ((obj->flags & LLIDB_FLAG_NODATA) != 0) {
        if (obj->desc != NULL) {
            free(obj->desc);
            obj->desc = NULL;
        }
        if (obj->script != NULL) {
            free(obj->script);
            obj->script = NULL;
        }
    }

    if (obj->order < 1) {
        obj->order = 1;
    }
    FUN_00480aa0((struct ObjClassNames *)head, (struct ObjectInfo *)obj);
    return obj;
}

// FUNCTION: LEGOLAND 0x0047c6a0
void FUN_0047c6a0(struct LLIDBHead *head) {
    struct ODFObject *obj = (struct ODFObject *)head->data;
    struct ODFObject *node;

    if (obj == NULL) {
        return;
    }

    if (obj->cleanup != NULL) {
        obj->cleanup(obj->cleanup_arg);
    }
    if (obj->flags & LLIDB_FLAG_USEDLL) {
        UnLoadObjectLibrary(obj);
    }

    node = (struct ODFObject *)ObjectClassList;
    if (node == obj) {
        ObjectClassList = obj->next;
    } else {
        while (node != NULL) {
            if (node->next == obj) {
                break;
            }
            node = node->next;
        }
        if (node != NULL) {
            node->next = obj->next;
        }
    }

    if (obj->sprite_0 != NULL) {
        if (obj->sprite_0->anim->type == 2 || obj->sprite_0->anim->type == 3) {
            LLSStop(obj->sprite_0->anim->handle);
        }
        KillSprite((unsigned int)obj->sprite_0);
    }
    if (obj->sprite_1 != NULL) {
        if (obj->sprite_1->anim->type == 2 || obj->sprite_1->anim->type == 3) {
            LLSStop(obj->sprite_1->anim->handle);
        }
        KillSprite((unsigned int)obj->sprite_1);
    }
    if (obj->sprite_2 != NULL) {
        if (obj->sprite_2->anim->type == 2 || obj->sprite_2->anim->type == 3) {
            LLSStop(obj->sprite_2->anim->handle);
        }
        KillSprite((unsigned int)obj->sprite_2);
    }

    if (obj->handle != 0) {
        LLIDB_UnLoadData(obj->handle);
    }
    if (obj->data_78 != NULL) {
        free(obj->data_78);
    }
    if (obj->data_7c != NULL) {
        free(obj->data_7c);
    }
    if (obj->data_80 != NULL) {
        free(obj->data_80);
    }
    free(obj);

    head->data = NULL;
}

// FUNCTION: LEGOLAND 0x0047c7f0
void FUN_0047c7f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047cba0
LEGO_EXPORT void *LLIDB_LoadTSFData(struct LLIDBHead *head) {
    char filename[0x200];
    char name[0x200];
    unsigned int count;
    int len;
    int ret;
    unsigned int element;
    unsigned int tile_base;
    struct ResFile *file;
    struct SpriteManager *si;
    int *anim;
    int i;

    sprintf(filename, "TileData\\%s", head->name);
    file = RES_OpenFile(filename);
    if (file == NULL) {
        return NULL;
    }

    si = (struct SpriteManager *)malloc(0x24);
    RES_ReadFile(file, &count, 4);
    si->data_c = (int *)malloc(count * 4);
    si->data_10 = (int *)malloc(count * 4);
    si->count = count;
    si->data_18 = NULL;
    si->data_1c = NULL;
    si->data_20 = NULL;
    RES_ReadFile(file, &len, 4);
    RES_ReadFile(file, name, len);

    i = 0;
    if ((int)count > 0) {
        do {
            RES_ReadFile(file, (char *)si->data_c + i * 4, 4);
            RES_ReadFile(file, (char *)si->data_10 + i * 4, 4);
            i++;
        } while (i < (int)count);
    }

    si->sprites = AllocTileSpace(si, count, &tile_base);
    si->var_0 = tile_base & 0xffff;

    i = 0;
    if ((int)count > 0) {
        do {
            RES_ReadFile(file, &len, 4);
            RES_ReadFile(file, filename, len);
            filename[len] = '\0';
            si->sprites[i] = LoadSprite(filename, 1);
            anim = *(int **)(si->sprites[i] + 8);
            if ((anim[5] == 2 || anim[5] == 3) && *(short *)(*anim + 0x10) > 1) {
                LLSPlay((struct LLS *)*anim, (unsigned int)anim);
            }
            i++;
        } while (i < (int)count);
    }

    si->data_14 = 0;
    ret = RES_ReadFile(file, &len, 4);
    if (ret == 4 && len != 0) {
        RES_ReadFile(file, name, len);
        name[len] = '\0';
        if (LLIDB_FindElement(name, &element, NULL) == 0) {
            head->data = si;
            si->data_14 = element;
            LLIDB_LoadData((void *)element);
            *(struct SpriteManager **)(*(int *)(element + 0xc) + 0x74) = si;
        }
    }

    RES_CloseFile(file);
    head->data = si;
    head->flags |= LLIDB_FLAG_LOADED;
    return si;
}

// FUNCTION: LEGOLAND 0x0047cdd0
void FUN_0047cdd0(struct SpriteManager *param_1) {
    struct SpriteManager *si = (struct SpriteManager *)param_1->data_c;
    int i;

    free(si->data_c);
    free(si->data_10);

    i = 0;
    if (si->count > 0) {
        do {
            KillSprite(si->sprites[i]);
            i++;
        } while (i < si->count);
    }

    FreeTileSpace(si->var_0, si->count);
    if (si->data_14 != 0) {
        LLIDB_UnLoadData(si->data_14);
    }

    free(si);
}

// FUNCTION: LEGOLAND 0x0047ce40
LEGO_EXPORT void *LLIDB_LoadTSMData(struct LLIDBHead *head) {
    char filename[0x200];
    char name[0x200];
    int count;
    int len;
    unsigned int element;
    struct ResFile *file;
    unsigned int *entries;
    unsigned int *entry;
    int i;

    // STRING: LEGOLAND 0x004bc3a8
    sprintf(filename, "TileData\\%s", head->name);
    file = RES_OpenFile(filename);
    if (file == NULL) {
        goto fail;
    }

    RES_ReadFile(file, &count, 4);
    entries = (unsigned int *)malloc(count * 8 + 8);
    if (entries == NULL) {
        goto closefail;
    }

    RES_ReadFile(file, &len, 4);
    RES_ReadFile(file, name, len);

    i = 0;
    if (count > 0) {
        entry = entries;
        do {
            RES_ReadFile(file, &len, 4);
            RES_ReadFile(file, name, len);
            name[len] = '\0';
            LLIDB_FindElement(name, &element, NULL);
            entry[0] = element;
            entry[1] = (unsigned int)LLIDB_LoadData((void *)element);
            i++;
            entry += 2;
        } while (i < count);
    }

    entries[i * 2] = 0xffffffff;
    entries[i * 2 + 1] = 0xffffffff;
    RES_CloseFile(file);
    head->flags |= LLIDB_FLAG_LOADED;
    head->data = entries;
    return entries;
closefail:
    RES_CloseFile(file);
fail:
    return NULL;
}

// FUNCTION: LEGOLAND 0x0047cf80
void FUN_0047cf80(struct LLIDBHead *head) {
    unsigned int *base = (unsigned int *)head->data;
    unsigned int *entry = base;
    int sentinel = base[1];

    while (sentinel != -1) {
        LLIDB_UnLoadData(*entry);
        entry += 2;
        sentinel = entry[1];
    }
    free(base);
}

// FUNCTION: LEGOLAND 0x0047cfc0
LEGO_EXPORT void *LLIDB_LoadILFData(struct LLIDBHead *head) {
    char filename[0x200];
    char name[0x200];
    int count;
    short unused;
    int len;
    struct ResFile *file;
    struct ILFTable *table;
    int i;

    count = 0;
    // STRING: LEGOLAND 0x004bc3b4
    sprintf(filename, "ImageData\\%s", head->name);
    file = RES_OpenFile(filename);
    if (file == NULL) {
        return NULL;
    }

    table = (struct ILFTable *)malloc(0x24);
    RES_ReadFile(file, &count, 2);
    RES_ReadFile(file, &unused, 2);
    if (table != NULL) {
        table->data_c = (int *)malloc(count * 4);
        table->data_10 = (int *)malloc(count * 4);
        table->sprites = (unsigned int *)malloc(count * 4);
        table->count = count;
        if (table->data_c != NULL && table->data_10 != NULL && table->sprites != NULL) {
            RES_ReadFile(file, &len, 4);
            RES_ReadFile(file, name, len);
            i = 0;
            if (count > 0) {
                do {
                    RES_ReadFile(file, (char *)table->data_c + i * 4, 4);
                    RES_ReadFile(file, (char *)table->data_10 + i * 4, 4);
                    table->data_c[i] = table->data_c[i] << 1;
                    table->data_10[i] = table->data_10[i] << 1;
                    i++;
                } while (i < count);
            }
            i = 0;
            if (count > 0) {
                do {
                    RES_ReadFile(file, &len, 4);
                    RES_ReadFile(file, filename, len);
                    filename[len] = '\0';
                    table->sprites[i] = LoadSprite(filename, 1);
                    i++;
                } while (i < count);
            }
            if (i == count) {
                table->data_14 = NULL;
                RES_CloseFile(file);
                head->data = table;
                head->flags |= LLIDB_FLAG_LOADED;
                return table;
            }
        }
    }
    LLIDB_FreeILFTable(table);
    RES_CloseFile(file);
    return NULL;
}

// FUNCTION: LEGOLAND 0x0047d1a0
LEGO_EXPORT void *LLIDB_LoadCSPData(struct LLIDBHead *head) {
    char filename[0x200];
    char name[0x200];
    int count;
    short unused;
    int len;
    struct ResFile *file;
    struct ILFTable *table;
    int i;

    count = 0;
    // STRING: LEGOLAND 0x004bc3c4
    sprintf(filename, "CompSprite\\%s", head->name);
    file = RES_OpenFile(filename);
    if (file == NULL) {
        return NULL;
    }

    table = (struct ILFTable *)malloc(0x24);
    if (table != NULL) {
        RES_ReadFile(file, &count, 2);
        RES_ReadFile(file, &unused, 2);
        table->data_c = (int *)malloc(count * 4);
        table->data_10 = (int *)malloc(count * 4);
        table->sprites = (unsigned int *)malloc(count * 4);
        table->count = count;
        if (table->data_c != NULL && table->data_10 != NULL && table->sprites != NULL) {
            RES_ReadFile(file, &len, 4);
            RES_ReadFile(file, name, len);
            i = 0;
            if (count > 0) {
                do {
                    RES_ReadFile(file, (char *)table->data_c + i * 4, 4);
                    RES_ReadFile(file, (char *)table->data_10 + i * 4, 4);
                    table->data_c[i] = table->data_c[i] << 1;
                    table->data_10[i] = table->data_10[i] << 1;
                    i++;
                } while (i < count);
            }
            i = 0;
            if (count > 0) {
                do {
                    RES_ReadFile(file, &len, 4);
                    RES_ReadFile(file, filename, len);
                    filename[len] = '\0';
                    table->sprites[i] = LoadSprite(filename, 1);
                    i++;
                } while (i < count);
            }
            i = 0;
            if (count > 0) {
                unsigned int *sprite = table->sprites;
                do {
                    if (*sprite == 0) {
                        break;
                    }
                    i++;
                    sprite++;
                } while (i < count);
            }
            if (i == count) {
                table->data_14 = NULL;
                head->data = table;
                head->flags |= LLIDB_FLAG_LOADED;
                RES_CloseFile(file);
                return head->data;
            }
        }
    }
    LLIDB_FreeILFTable(table);
    RES_CloseFile(file);
    return NULL;
}

// FUNCTION: LEGOLAND 0x0047d3a0
LEGO_EXPORT void *LLIDB_LoadData(void *handle) {
    struct LLIDBHead *head = (struct LLIDBHead *)handle;
    unsigned int flags = head->flags;
    void *data;

    if (flags & LLIDB_FLAG_LOADED) {
        head->refcount++;
        return head->data;
    }

    head->data = NULL;
    flags |= LLIDB_FLAG_LOADED;
    head->flags = flags;
    switch (flags & LLIDB_TYPE_MASK) {
    case LLIDB_TYPE_ODF:
    case LLIDB_TYPE_ODF_DLL:
        head->data = LLIDB_LoadODFData(head);
        break;
    case LLIDB_TYPE_TSM:
        head->data = LLIDB_LoadTSMData(head);
        break;
    case LLIDB_TYPE_TSF:
        head->data = LLIDB_LoadTSFData(head);
        break;
    case LLIDB_TYPE_ILF:
        head->data = LLIDB_LoadILFData(head);
        break;
    case LLIDB_TYPE_CSP:
        head->data = LLIDB_LoadCSPData(head);
        break;
    case LLIDB_TYPE_NOFILE:
    case LLIDB_TYPE_TXT:
        return NULL;
    }

    data = head->data;
    if (data == NULL) {
        head->flags &= ~LLIDB_FLAG_LOADED;
        return data;
    }
    head->refcount++;
    return data;
}

// FUNCTION: LEGOLAND 0x0047d450
LEGO_EXPORT void LLIDB_UnLoadData(unsigned int handle) {
    struct LLIDBHead *head = (struct LLIDBHead *)handle;
    unsigned int flags;

    if (head->refcount == 0 || --head->refcount != 0) {
        return;
    }
    flags = head->flags;
    if ((flags & LLIDB_FLAG_LOADED) == 0) {
        return;
    }
    flags &= 0xfffcfff0;
    head->flags = flags;
    switch (flags & LLIDB_TYPE_MASK) {
    case LLIDB_TYPE_ODF:
    case LLIDB_TYPE_ODF_DLL:
        FUN_0047c6a0(head);
        break;
    case LLIDB_TYPE_TSM:
        FUN_0047cf80(head);
        break;
    case LLIDB_TYPE_TSF:
        FUN_0047cdd0((struct SpriteManager *)head);
        break;
    case LLIDB_TYPE_ILF:
        LLIDB_FreeILFTable((struct ILFTable *)head->data);
        break;
    }
}

// FUNCTION: LEGOLAND 0x0047d4c0
LEGO_EXPORT void LLSStop(unsigned int handle) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d520
LEGO_EXPORT void LLSPlay(struct LLS *param_1, unsigned int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d580
LEGO_EXPORT void LLSPlayOnce(struct LLS *param_1, unsigned int param_2) {
    LLSPlay(param_1, param_2);
    param_1->flags |= 0x4;
}

// FUNCTION: LEGOLAND 0x0047d5a0
LEGO_EXPORT void LLSSetFrame(struct LLS *param_1, int index) {
    if (param_1 == NULL) {
        return;
    }
    if (index < 0) {
        index = 0;
    }
    if (index >= param_1->frame_count) {
        index = param_1->frame_count - 1;
    }
    param_1->frame = (short)index;
}

// FUNCTION: LEGOLAND 0x0047d5d0
LEGO_EXPORT void LLSNextFrame(struct LLS *param_1) {
    if (param_1 != NULL) {
        param_1->frame++;
        if (param_1->frame == param_1->frame_count) {
            param_1->frame = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x0047d5f0
LEGO_EXPORT void LLSSetDelay(struct LLS *param_1, unsigned int param_2) {
    param_1->delay = (unsigned short)param_2;
    param_1->loop_delay = (short)param_2;
}

// FUNCTION: LEGOLAND 0x0047d610
void FUN_0047d610(struct LLS *param_1) {
    param_1->frame++;
    if (param_1->frame >= param_1->frame_count) {
        param_1->frame = 0;
    }
}

// FUNCTION: LEGOLAND 0x0047d630
LEGO_EXPORT void LLSAuto(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d6a0
LEGO_EXPORT void LLS555To565(struct LLSImage *param_1) {
    unsigned char *esi = (unsigned char *)param_1 + 0x18;

    if (param_1->format == 8) {
        unsigned short *data = (unsigned short *)(esi + *(int *)(esi + 4) + 8);
        int counter = 0x100;
        do {
            unsigned short pixel = *data;
            *data = ((pixel & 0xffe0) << 1) | (pixel & 0x1f);
            data++;
            counter--;
        } while (counter != 0);
    } else if (param_1->format == 0x10) {
        int rows = param_1->height;
        if (param_1->flags & 0x1) {
            rows++;
        }
        if (rows > 0) {
            int row_count = rows;
            do {
                int width = *(int *)(esi + 4);
                unsigned short *data = (unsigned short *)(esi + 0x10);
                if (width > 0) {
                    do {
                        unsigned short pixel = *data;
                        *data = ((pixel & 0xffe0) << 1) | (pixel & 0x1f);
                        data++;
                        width--;
                    } while (width != 0);
                }
                esi += *(unsigned int *)esi;
                row_count--;
            } while (row_count != 0);
        }
    }
}

// FUNCTION: LEGOLAND 0x0047d730
LEGO_EXPORT unsigned int SaveGameRead(void *buffer, unsigned int count) {
    return _read(DAT_006691b0, buffer, count) == count;
}

// FUNCTION: LEGOLAND 0x0047d760
LEGO_EXPORT unsigned int SaveGameWrite(void *buffer, unsigned int count) {
    return (unsigned int)_write(DAT_006691b0, buffer, count) == count;
}
