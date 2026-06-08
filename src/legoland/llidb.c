#include "legoland.h"
#include <windows.h>

#include "crt.h"

#include "llidb.h"
#include "tilemap.h"
#include "globals.h"
#include "wndenv.h"
#include "objclass.h"

#include "image_sprite.h"

struct ILFTable {
    unsigned char pad_0[4];
    int count;
    unsigned int *sprites;
    void *data_c;
    void *data_10;
};

struct SpriteManager {
    unsigned short var_0;
    unsigned char pad_2[0x4 - 0x2];
    int count;
    unsigned int *sprites;
    void *data_c;
    void *data_10;
    void *data_14;
};

struct LLSImage {
    unsigned char pad_0[0xc];
    unsigned int format;
    short height;
    unsigned char pad_12[0x2];
    unsigned char flags;
};

struct Element {
    unsigned int field[5];
};

struct LLIDBHead {
    unsigned char pad_0[8];
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
    unsigned char pad_4[0x1c - 0x4];
    unsigned int flags;
    unsigned char pad_20[0x64 - 0x20];
    struct ODFSprite *sprite_0;
    struct ODFSprite *sprite_1;
    struct ODFSprite *sprite_2;
    unsigned int handle;
    unsigned char pad_74[0x78 - 0x74];
    void *data_78;
    void *data_7c;
    void *data_80;
    unsigned char pad_84[0xac - 0x84];
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
            *output = (unsigned int)DAT_006691a8[index >> 8] + (index & 0xff) * 20;
        }
        return 0;
    }
    if (output != NULL) {
        *output = 0;
    }
    return -3;
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
            return -3;
        }
    } else {
        for (i = 0; i < DAT_006691a4; i++) {
            if (_stricmp(name, *(char **)((unsigned int)DAT_006691a8[i >> 8] + (i & 0xff) * 20)) == 0) {
                if (out != NULL) {
                    *out = (unsigned int)DAT_006691a8[i >> 8] + (i & 0xff) * 20;
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
    return -3;
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
            return -3;
        }
    } else {
        for (i = 0; i < DAT_006691a4; i++) {
            if (data == *(void **)((unsigned int)DAT_006691a8[i >> 8] + (i & 0xff) * 20 + 0xc)) {
                if (out != NULL) {
                    *out = (unsigned int)DAT_006691a8[i >> 8] + (i & 0xff) * 20;
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
    return -3;
}

// FUNCTION: LEGOLAND 0x0047b4c0
LEGO_EXPORT void LLIDB_ClearOnLevel(void) {
    unsigned int i = 0;

    if (DAT_006691a4 > 0) {
        do {
            DAT_006691a8[i >> 8][(i & 0xff) * 5 + 2] &= 0xfffffffb;
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
                    *(struct Element *)((int)DAT_006691a8[page - 1] + 0x13ec) = *(struct Element *)DAT_006691a8[page];
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
    return 0xfffffffd;
}

// FUNCTION: LEGOLAND 0x0047b5a0
unsigned int FUN_0047b5a0(void) {
    unsigned int capacity;

    if (((DAT_006691a0 ^ DAT_006691a4) & 0xffffff00) == 0) {
        capacity = (DAT_006691a4 + 0x100) & 0xffffff00;
        DAT_006691a0 = capacity;
        DAT_006691a8 = (unsigned int **)realloc(DAT_006691a8, (capacity >> 8) * 4);
        DAT_006691a8[(DAT_006691a0 >> 8) - 1] = (unsigned int *)malloc(0x1400);
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
        return (unsigned int)-4;
    }
    if ((param_2 == NULL || param_2[0] == '\0') && param_3 != 0x200) {
        return (unsigned int)-5;
    }

    if (LLIDB_FindElement(param_1, &found, NULL) == 0) {
        if (param_3 != 0x200 && _stricmp(param_2, *(char **)(found + 4)) != 0) {
            return (unsigned int)-1;
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

    *(unsigned int *)(*(char **)(page_off + (unsigned int)DAT_006691a8) + slot_off + 8) = param_3 & 0xfff0;
    *(unsigned int *)(*(char **)(page_off + (unsigned int)DAT_006691a8) + slot_off + 0x10) = 0;

    DAT_006691a4++;
    return 0;
}

// FUNCTION: LEGOLAND 0x0047b7b0
void FUN_0047b7b0(int param_1) {
    // STRING: LEGOLAND 0x004bc208
    const char *title = "LEGOLAND Installation & Configuration Manager";
    switch (param_1) {
    case -1:
        // STRING: LEGOLAND 0x004bc1dc
        MessageBoxA(NULL, "An element of the same name already exists.", title, 0x30);
        break;
    case -2:
        // STRING: LEGOLAND 0x004bc1ac
        MessageBoxA(NULL, "LEGOLAND.ICM could not be created or updated.", title, 0x30);
        break;
    case -3:
        // STRING: LEGOLAND 0x004bc184
        MessageBoxA(NULL, "The given element could not be found.", title, 0x30);
        break;
    case -4:
        // STRING: LEGOLAND 0x004bc160
        MessageBoxA(NULL, "No identification key was given.", title, 0x30);
        break;
    case -5:
        // STRING: LEGOLAND 0x004bc148
        MessageBoxA(NULL, "No file name was given.", title, 0x30);
        break;
    case -6:
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
            return -6;
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
LEGO_EXPORT void *LLIDB_LoadODFData(struct LLIDBHead *head) { STUB(); }

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
    if (obj->flags & 0x10000) {
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
LEGO_EXPORT void *LLIDB_LoadTSFData(struct LLIDBHead *head) { STUB(); }

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
    if (si->data_14 != NULL) {
        LLIDB_UnLoadData((unsigned int)si->data_14);
    }

    free(si);
}

// FUNCTION: LEGOLAND 0x0047ce40
LEGO_EXPORT void *LLIDB_LoadTSMData(struct LLIDBHead *head) { STUB(); }

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
LEGO_EXPORT void *LLIDB_LoadILFData(struct LLIDBHead *head) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d1a0
LEGO_EXPORT void *LLIDB_LoadCSPData(struct LLIDBHead *head) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d3a0
LEGO_EXPORT void *LLIDB_LoadData(void *handle) {
    struct LLIDBHead *head = (struct LLIDBHead *)handle;
    unsigned int flags = head->flags;
    void *data;

    if (flags & 1) {
        head->refcount++;
        return head->data;
    }

    head->data = NULL;
    flags |= 1;
    head->flags = flags;
    switch (flags & 0xfff0) {
    case 0x10:
    case 0x1010:
        head->data = LLIDB_LoadODFData(head);
        break;
    case 0x20:
        head->data = LLIDB_LoadTSMData(head);
        break;
    case 0x40:
        head->data = LLIDB_LoadTSFData(head);
        break;
    case 0x400:
        head->data = LLIDB_LoadILFData(head);
        break;
    case 0x2000:
        head->data = LLIDB_LoadCSPData(head);
        break;
    case 0x200:
    case 0x800:
        return NULL;
    }

    data = head->data;
    if (data == NULL) {
        head->flags &= 0xfffffffe;
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
    if ((flags & 1) == 0) {
        return;
    }
    flags &= 0xfffcfff0;
    head->flags = flags;
    switch (flags & 0xfff0) {
    case 0x10:
    case 0x1010:
        FUN_0047c6a0(head);
        break;
    case 0x20:
        FUN_0047cf80(head);
        break;
    case 0x40:
        FUN_0047cdd0((struct SpriteManager *)head);
        break;
    case 0x400:
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
