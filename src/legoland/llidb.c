#include <windows.h>
#include <commctrl.h>
#include "legoland.h"

#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <stddef.h>
#include "debug.h"
#include "debug_alloc.h"
#include "globals.h"
#include "llidb.h"
#include "objclass.h"
#include "render3d.h"
#include "resource.h"
#include "setcustomcallbacks.h"
#include "tilemap.h"
#include "wndenv.h"

#include "image_sprite.h"
#include "obj_instance.h"

#define LLIDB_PAGE(id) ((id) >> 8)
#define LLIDB_SLOT(id) ((id) & 0xff)
#define LLIDB_ELEM(id) (&LLIDB_Pages[LLIDB_PAGE(id)][LLIDB_SLOT(id)])

struct LLSImage {
    unsigned char pad_0[0xc];
    unsigned int format;
    short height;
    unsigned char pad_12[0x2];
    unsigned char flags;
};

// FUNCTION: LEGOLAND 0x0047aff0
LEGO_EXPORT int LLIDB_LoadICM(void) {
    int fd;
    int fd2;
    unsigned int page;
    unsigned int i;
    unsigned int remaining;
    unsigned int n;
    int len;
    int zero;
    struct Element *e;

    fd = _open("LEGOLAND.ICM", _O_RDONLY | _O_BINARY);
    if (fd == -1) {
        fd2 = _open("LEGOLAND.ICM", _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
        if (fd2 == fd) {
            return LLIDB_ERR_ICMWRITE;
        }
        zero = 0;
        _write(fd2, &zero, 4);
        _close(fd2);
        return 0;
    }
    _read(fd, &LLIDB_ElementCount, 4);
    LLIDB_Capacity = (LLIDB_ElementCount + 0xff) & 0xffffff00;
    LLIDB_Pages = malloc((LLIDB_Capacity >> 8) * sizeof(*LLIDB_Pages));
    for (page = 0; page < (LLIDB_Capacity >> 8); page++) {
        LLIDB_Pages[page] = malloc(0x100 * sizeof(struct Element));
    }
    remaining = LLIDB_ElementCount;
    for (page = 0; page < (LLIDB_Capacity >> 8); page++, remaining -= 0x100) {
        if (remaining >= 0x100) {
            n = 0x100;
        } else {
            n = remaining;
        }
        _read(fd, LLIDB_Pages[page], n * sizeof(struct Element));
    }
    remaining = LLIDB_ElementCount;
    for (page = 0; page < (LLIDB_Capacity >> 8); page++, remaining -= 0x100) {
        if (remaining >= 0x100) {
            n = 0x100;
        } else if (remaining <= 0) {
            continue;
        } else {
            n = remaining;
        }
        for (i = 0; i < n; i++) {
            LLIDB_Pages[page][i].flags &= ~0xa;
            _read(fd, &len, 4);
            if (len == 0) {
                LLIDB_Pages[page][i].name = NULL;
            } else {
                LLIDB_Pages[page][i].name = malloc(len + 1);
                LLIDB_Pages[page][i].name[len] = '\0';
                _read(fd, LLIDB_Pages[page][i].name, len);
            }
            _read(fd, &len, 4);
            if (len == 0) {
                LLIDB_Pages[page][i].path = NULL;
            } else {
                LLIDB_Pages[page][i].path = malloc(len + 1);
                LLIDB_Pages[page][i].path[len] = '\0';
                _read(fd, LLIDB_Pages[page][i].path, len);
            }
            LLIDB_Pages[page][i].flags &= ~1;
            LLIDB_Pages[page][i].field_10 = 0;
        }
    }
    _close(fd);
    // STRING: LEGOLAND 0x004bc114
    e = ElemID("LANGUAGE");
    if (e != NULL) {
        strcpy(LanguageName, e->path);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0047b2d0
LEGO_EXPORT unsigned int LLIDB_GetCount(void) {
    return LLIDB_ElementCount;
}

// FUNCTION: LEGOLAND 0x0047b2e0
LEGO_EXPORT int LLIDB_GetElement(unsigned int index, struct Element **output) {
    if (index < LLIDB_ElementCount) {
        if (output != NULL) {
            *output = LLIDB_ELEM(index);
        }
        return 0;
    }
    if (output != NULL) {
        *output = NULL;
    }
    return LLIDB_ERR_NOTFOUND;
}

// FUNCTION: LEGOLAND 0x0047b330
LEGO_EXPORT int LLIDB_FindElement(const char *name, struct Element **out, unsigned int *index_out) {
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
        for (i = 0; i < LLIDB_ElementCount; i++) {
            if (_stricmp(name, LLIDB_ELEM(i)->name) == 0) {
                if (out != NULL) {
                    *out = LLIDB_ELEM(i);
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
LEGO_EXPORT struct Element *ElemID(const char *name) {
    struct Element *element;

    LLIDB_FindElement(name, &element, NULL);
    return element;
}

// FUNCTION: LEGOLAND 0x0047b410
LEGO_EXPORT int LLIDB_FindElementFromDataPtr(void *data, struct Element **out, unsigned int *index_out) {
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
        for (i = 0; i < LLIDB_ElementCount; i++) {
            if (data == LLIDB_ELEM(i)->data) {
                if (out != NULL) {
                    *out = LLIDB_ELEM(i);
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

    if (LLIDB_ElementCount > 0) {
        do {
            LLIDB_ELEM(i)->flags &= ~LLIDB_FLAG_LEVEL;
            i++;
        } while (i < LLIDB_ElementCount);
    }
}

/* Removes the indexed element by shifting subsequent paged entries toward it and decrementing the element count.
 * Returns the not-found error for an invalid index. */
// FUNCTION: LEGOLAND 0x0047b500
unsigned int LLIDB_RemoveElementAtIndex(unsigned int param_1) {
    unsigned int slot;
    unsigned int page;
    unsigned int count;

    if (param_1 < LLIDB_ElementCount) {
        count = LLIDB_ElementCount;
        page = param_1 >> 8;
        slot = param_1 & 0xff;
        if (page <= count >> 8) {
            do {
                if (slot == 0xffffffff) {
                    LLIDB_Pages[page - 1][0xff] = LLIDB_Pages[page][0];
                    slot = 0;
                } else if ((int)slot >= 0xfe) {
                    slot = 0xffffffff;
                    page++;
                    continue;
                }
                do {
                    struct Element *arr = LLIDB_Pages[page];
                    arr[slot] = arr[slot + 1];
                    slot++;
                } while ((int)slot < 0xfe);
                count = LLIDB_ElementCount;
                slot = 0xffffffff;
                page++;
            } while (page <= count >> 8);
        }
        LLIDB_ElementCount = count - 1;
        return 0;
    }
    return (unsigned int)LLIDB_ERR_NOTFOUND;
}

// FUNCTION: LEGOLAND 0x0047b5a0
unsigned int LLIDB_GrowIfFull(void) {
    unsigned int capacity;

    if (((LLIDB_Capacity ^ LLIDB_ElementCount) & 0xffffff00) == 0) {
        capacity = (LLIDB_ElementCount + 0x100) & 0xffffff00;
        LLIDB_Capacity = capacity;
        LLIDB_Pages = (struct Element **)realloc(LLIDB_Pages, (capacity >> 8) * sizeof(struct Element *));
        LLIDB_Pages[(LLIDB_Capacity >> 8) - 1] = (struct Element *)malloc(0x100 * sizeof(struct Element));
    }
    return LLIDB_ElementCount;
}

// FUNCTION: LEGOLAND 0x0047b610
LEGO_EXPORT unsigned int LLIDB_RegisterNewElement(const char *param_1, const char *param_2, unsigned int param_3) {
    unsigned int index;
    char *page_off;
    unsigned int slot_off;
    char *copy;
    struct Element *found;

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

    index = LLIDB_GrowIfFull();
    page_off = (char *)(UINT_PTR)((index >> 8) * sizeof(struct Element *));
    slot_off = (index & 0xff) * sizeof(struct Element);

    copy = (char *)malloc(strlen(param_1) + 1);
    *(char **)(*(char **)(page_off + (UINT_PTR)LLIDB_Pages) + slot_off) = copy;
    strcpy(*(char **)(*(char **)(page_off + (UINT_PTR)LLIDB_Pages) + slot_off), param_1);

    if (param_2 == NULL) {
        copy = (char *)malloc(1);
        *(char **)(*(char **)(page_off + (UINT_PTR)LLIDB_Pages) + slot_off + offsetof(struct Element, path)) = copy;
        *(*(char **)(*(char **)(page_off + (UINT_PTR)LLIDB_Pages) + slot_off + offsetof(struct Element, path))) = '\0';
    } else {
        copy = (char *)malloc(strlen(param_2) + 1);
        *(char **)(*(char **)(page_off + (UINT_PTR)LLIDB_Pages) + slot_off + offsetof(struct Element, path)) = copy;
        strcpy(*(char **)(*(char **)(page_off + (UINT_PTR)LLIDB_Pages) + slot_off + offsetof(struct Element, path)), param_2);
    }

    *(unsigned int *)(*(char **)(page_off + (UINT_PTR)LLIDB_Pages) + slot_off + offsetof(struct Element, flags)) = param_3 & LLIDB_TYPE_MASK;
    *(unsigned int *)(*(char **)(page_off + (UINT_PTR)LLIDB_Pages) + slot_off + offsetof(struct Element, field_10)) = 0;

    LLIDB_ElementCount++;
    return 0;
}

// FUNCTION: LEGOLAND 0x0047b7b0
void ShowLLIDBError(int param_1) {
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
        ShowLLIDBError(result);
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0047b890
INT_PTR CALLBACK SelectElementDlgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    HWND list;
    int count;
    unsigned int i;
    struct Element *elem;
    LVCOLUMN col;
    LVITEM item;

    count = 0;
    switch (msg) {
    case WM_INITDIALOG:
        list = GetDlgItem(hwnd, 0x441);
        col.mask = LVCF_WIDTH | LVCF_TEXT;
        // STRING: LEGOLAND 0x004bc2e8
        col.pszText = "ID";
        col.cx = 180;
        SendMessageA(list, LVM_INSERTCOLUMN, 0, (LPARAM)&col);
        col.mask = LVCF_WIDTH | LVCF_TEXT;
        // STRING: LEGOLAND 0x004bc2e0
        col.pszText = "File";
        col.cx = 180;
        SendMessageA(list, LVM_INSERTCOLUMN, 1, (LPARAM)&col);
        col.mask = LVCF_WIDTH | LVCF_TEXT;
        // STRING: LEGOLAND 0x004bc2d8
        col.pszText = "Type";
        col.cx = 180;
        SendMessageA(list, LVM_INSERTCOLUMN, 2, (LPARAM)&col);
        list = GetDlgItem(hwnd, 0x441);
        for (i = 0; i < LLIDB_ElementCount; i++) {
            LLIDB_GetElement(i, &elem);
            if (elem != NULL && (SelectElementMask & elem->flags) != 0) {
                item.mask = LVIF_TEXT | LVIF_PARAM;
                item.iItem = count;
                item.iSubItem = 0;
                item.pszText = elem->name;
                item.lParam = i;
                SendMessageA(list, LVM_INSERTITEM, 0, (LPARAM)&item);
                item.mask = LVIF_TEXT;
                item.iItem = count;
                item.iSubItem = 1;
                item.pszText = elem->path;
                SendMessageA(list, LVM_SETITEM, 0, (LPARAM)&item);
                item.mask = LVIF_TEXT;
                item.iItem = count;
                item.iSubItem = 2;
                switch (elem->flags & 0xfff0) {
                case 0x10:
                    // STRING: LEGOLAND 0x004bc2c4
                    item.pszText = "Object Description";
                    break;
                case 0x20:
                    // STRING: LEGOLAND 0x004bc2b4
                    item.pszText = "Tile Mapping";
                    break;
                case 0x40:
                    // STRING: LEGOLAND 0x004bc2a8
                    item.pszText = "Tile Set";
                    break;
                case 0x80:
                    // STRING: LEGOLAND 0x004bc298
                    item.pszText = "Level Structure";
                    break;
                case 0x100:
                    // STRING: LEGOLAND 0x004bc28c
                    item.pszText = "Terrain Map";
                    break;
                case 0x200:
                    // STRING: LEGOLAND 0x004bc26c
                    item.pszText = "Game Defined Stub";
                    break;
                case 0x400:
                    // STRING: LEGOLAND 0x004bc280
                    item.pszText = "Image List";
                    break;
                case 0x800:
                    // STRING: LEGOLAND 0x004bc254
                    item.pszText = "Configuration String";
                    break;
                case 0x1010:
                    // STRING: LEGOLAND 0x004bc238
                    item.pszText = "Object Control Description";
                    break;
                }
                SendMessageA(list, LVM_SETITEM, 0, (LPARAM)&item);
                count++;
            }
        }
        return 1;
    case WM_COMMAND:
        switch (LOWORD(wparam)) {
        case IDOK:
            list = GetDlgItem(hwnd, 0x441);
            SelectedElementIndex = SendMessageA(list, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
            if (SelectedElementIndex == -1) {
                EndDialog(hwnd, 0);
                return 0;
            }
            item.mask = LVIF_PARAM;
            item.iItem = SelectedElementIndex;
            item.iSubItem = 0;
            SendMessageA(list, LVM_GETITEM, 0, (LPARAM)&item);
            SelectedElementIndex = item.lParam;
            EndDialog(hwnd, 1);
            break;
        case IDCANCEL:
            EndDialog(hwnd, 0);
            return 0;
        case 0x29a:
            EndDialog(hwnd, -1);
            return 0;
        }
        break;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0047bc20
LEGO_EXPORT int LLIDB_SelectElement(unsigned int mask, struct Element **output) {
    INT_PTR result;

    SelectElementMask = mask;
    result = DialogBoxParamA((HINSTANCE)WNDENV_GethInstance(), MAKEINTRESOURCEA(0x75), GetDesktopWindow(), SelectElementDlgProc, 0);
    if (result != -1) {
        if (result != 1) {
            return LLIDB_ERR_CANCELED;
        }
        if (output != NULL) {
            LLIDB_GetElement(SelectedElementIndex, output);
        }
        return 0;
    }
    return -1;
}

// FUNCTION: LEGOLAND 0x0047bc80
LEGO_EXPORT int LLIDB_SaveICM(void) {
    int fd;
    unsigned int page;
    unsigned int i;
    unsigned int remaining;
    unsigned int n;
    int len;

    // STRING: LEGOLAND 0x004bc120
    fd = _open("LEGOLAND.ICM", _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd == -1) {
        return LLIDB_ERR_ICMWRITE;
    }
    _write(fd, &LLIDB_ElementCount, 4);
    remaining = LLIDB_ElementCount;
    for (page = 0; page < (LLIDB_Capacity >> 8); page++) {
        if (remaining >= 0x100) {
            n = 0x100;
        } else {
            n = remaining;
        }
        _write(fd, LLIDB_Pages[page], n * sizeof(struct Element));
        remaining -= 0x100;
    }
    remaining = LLIDB_ElementCount;
    for (page = 0; page < (LLIDB_Capacity >> 8); page++, remaining -= 0x100) {
        if (remaining >= 0x100) {
            n = 0x100;
        } else {
            n = remaining;
        }
        for (i = 0; i < n; i++) {
            len = mystrlen(LLIDB_Pages[page][i].name);
            _write(fd, &len, 4);
            _write(fd, LLIDB_Pages[page][i].name, len);
            len = mystrlen(LLIDB_Pages[page][i].path);
            _write(fd, &len, 4);
            _write(fd, LLIDB_Pages[page][i].path, len);
        }
    }
    _close(fd);
    return 0;
}

// FUNCTION: LEGOLAND 0x0047be00
LEGO_EXPORT int LLIDB_CloseICM(void) {
    unsigned int chunk;
    unsigned int remaining = LLIDB_ElementCount;
    unsigned int i;
    unsigned int n;

    for (chunk = 0; chunk < (LLIDB_Capacity >> 8); chunk++, remaining -= 0x100) {
        n = remaining >= 0x100 ? 0x100 : remaining;
        for (i = 0; i < n; i++) {
            if (LLIDB_Pages[chunk][i].name != NULL) {
                free(LLIDB_Pages[chunk][i].name);
            }
            if (LLIDB_Pages[chunk][i].path != NULL) {
                free(LLIDB_Pages[chunk][i].path);
            }
        }
    }
    for (i = 0; i < (LLIDB_Capacity >> 8); i++) {
        free(LLIDB_Pages[i]);
    }
    free(LLIDB_Pages);
    return 0;
}

// FUNCTION: LEGOLAND 0x0047bef0
LEGO_EXPORT void LLIDB_FreeILFTable(struct SpriteSet *table) {
    if (table == NULL) {
        return;
    }

    if (table->offset_x != NULL) {
        free(table->offset_x);
    }
    if (table->offset_y != NULL) {
        free(table->offset_y);
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
LEGO_EXPORT void *LLIDB_LoadODFData(struct Element *head) {
    char name[0x100];
    char text[0x100];
    char filename[0x100];
    char dllname[0x100];
    unsigned int size;
    struct Element *element;
    int lang_count;
    struct ResFile *file;
    struct Ride *obj;
    struct Image *sprite_image;
    struct Sprite *sprite;
    int i;
    int j;

    DebugAllocatedBytes = 0;
    // STRING: LEGOLAND 0x004bc39c
    sprintf(filename, "Objdesc\\%s", head->path);
    file = RES_OpenFile(filename);
    if (file == NULL) {
        return NULL;
    }

    obj = (struct Ride *)malloc(sizeof(struct Ride));
    memset(obj, 0, sizeof(struct Ride));
    if (obj == NULL) {
        return NULL;
    }

    obj->element = head;
    head->data = obj;
    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, &obj->instances, size - 4);
    obj->footprint.next = 0;
    obj->next = (struct Ride *)ObjectClassList;
    ObjectClassList = obj;
    lang_count = (int)(INT_PTR)obj->instances; /* the file keeps the language count here */
    obj->instances = 0;
    obj->field_8 = 0;

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
        obj->parent_element = element;
    } else {
        obj->parent_element = 0;
    }

    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, text, size);
    text[size] = '\0';

    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, name, size);
    name[size] = '\0';
    if (name[0] != '\0') {
        LLIDB_FindElement(name, &element, NULL);
        obj->theme_element = element;
    } else {
        obj->theme_element = 0;
    }

    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, name, size);
    name[size] = '\0';
    if (name[0] != '\0') {
        LLIDB_FindElement(name, &element, NULL);
        obj->menu_element = element;
    } else {
        obj->menu_element = 0;
    }

    NEWFLC_PauseType = 1;
    NEWFLC_AutoPlay = 1;
    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, name, size);
    name[size] = '\0';
    if ((obj->flags & LLIDB_FLAG_NODATA) == 0) {
        if (name[0] != '\0') {
            sprite = LoadSprite(name, 1);
            obj->layer = (struct Sprite *)sprite;
            if (sprite != 0 && (sprite_image = sprite->image) != NULL) {
                if ((sprite->flags & 0x8000) == 0 &&
                    (sprite_image->field_14 == 2 || sprite_image->field_14 == 3)) {
                    LLSPlay((struct LLS *)sprite_image->data, sprite_image);
                }
                obj->flags |= 4;
            }
        } else {
            // STRING: LEGOLAND 0x004bc37c
            DebugTrace("Class %s has no sprite name.", *(char **)obj->element);
            obj->layer = NULL;
        }
    } else {
        obj->layer = NULL;
    }
    NEWFLC_PauseType = 2;
    NEWFLC_AutoPlay = 0;
    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, name, size);
    name[size] = '\0';
    if (name[0] != '\0') {
        obj->icon = (struct Sprite *)LoadSprite(name, 4);
    } else {
        // STRING: LEGOLAND 0x004bc360
        DebugTrace("Class %s has no icon name.", *(char **)obj->element);
        obj->icon = NULL;
    }
    if (obj->icon == NULL) {
        // STRING: LEGOLAND 0x004bc34c
        obj->icon = (struct Sprite *)LoadSprite("InstituteIcon.lls", 4);
    }

    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, name, size);
    name[size] = '\0';
    if (name[0] != '\0') {
        sprite = LoadSprite(name, 1);
        obj->anim = (struct Sprite *)sprite;
        if (sprite != 0 && (sprite->flags & 0x8000) != 0) {
            for (j = 0; j < (int)(INT_PTR)((struct Sprite *)obj->anim)->image->aux; j++) {
                struct LLS *lls;
                sprite = (struct Sprite *)GetSpriteForLayer((struct Sprite *)obj->anim, j);
                if (sprite != 0 && (lls = GetLLSForSprite((struct Sprite *)sprite)) != 0) {
                    LLSStop(lls);
                }
            }
        }
    } else {
        // STRING: LEGOLAND 0x004bc328
        DebugTrace("Class %s has no build anim name.", *(char **)obj->element);
        obj->anim = NULL;
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
            if (i == 0 || _stricmp(LanguageName, name) == 0) {
                if (obj->name != NULL) {
                    free(obj->name);
                }
                RES_ReadFile(file, &size, 4);
                obj->name = malloc(size + 1);
                RES_ReadFile(file, obj->name, size);
                ((char *)obj->name)[size] = '\0';
                if (obj->field_7c != NULL) {
                    free(obj->field_7c);
                }
                RES_ReadFile(file, &size, 4);
                obj->field_7c = malloc(size + 1);
                RES_ReadFile(file, obj->field_7c, size);
                ((char *)obj->field_7c)[size] = '\0';
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

    obj->sprites = 0;
    RES_CloseFile(file);
    SetStandardCallbacks((struct Ride *)obj);
    if ((obj->flags & LLIDB_FLAG_USEDLL) == 0 || text[0] == '\0') {
        SetCustomCallbacks(obj->element);
    } else if (LoadObjectLibrary(obj, text) == 0) {
        obj->flags &= ~LLIDB_FLAG_USEDLL;
        SetCustomCallbacks(obj->element);
        if (obj->cb_a4 != NULL) {
            obj->cb_a4(obj->element);
        }
        // STRING: LEGOLAND 0x004bc2ec
        DebugTrace("Class %s has OC_USEDLL attribute and DLL failed to load.", *(char **)obj->element);
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

    if (obj->range < 1) {
        obj->range = 1;
    }
    ApplyWaterWorksClassOverrides((struct Element *)head, (struct Ride *)obj);
    return obj;
}

// FUNCTION: LEGOLAND 0x0047c6a0
void LLIDB_UnLoadODF(struct Element *head) {
    struct Ride *obj = (struct Ride *)head->data;
    struct Ride *node;

    if (obj == NULL) {
        return;
    }

    if (obj->cb_ac != NULL) {
        obj->cb_ac(obj->element);
    }
    if (obj->flags & LLIDB_FLAG_USEDLL) {
        UnLoadObjectLibrary(obj);
    }

    node = (struct Ride *)ObjectClassList;
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

    if (obj->layer != NULL) {
        if (obj->layer->anim->type == 2 || obj->layer->anim->type == 3) {
            LLSStop(obj->layer->anim->lls);
        }
        KillSprite((struct Sprite *)obj->layer);
    }
    if (obj->icon != NULL) {
        if (obj->icon->anim->type == 2 || obj->icon->anim->type == 3) {
            LLSStop(obj->icon->anim->lls);
        }
        KillSprite((struct Sprite *)obj->icon);
    }
    if (obj->anim != NULL) {
        if (obj->anim->anim->type == 2 || obj->anim->anim->type == 3) {
            LLSStop(obj->anim->anim->lls);
        }
        KillSprite((struct Sprite *)obj->anim);
    }

    if (obj->handle != 0) {
        LLIDB_UnLoadData(obj->handle);
    }
    if (obj->name != NULL) {
        free(obj->name);
    }
    if (obj->field_7c != NULL) {
        free(obj->field_7c);
    }
    if (obj->data_80 != NULL) {
        free(obj->data_80);
    }
    free(obj);

    head->data = NULL;
}

// FUNCTION: LEGOLAND 0x0047c7f0
struct Sprite *LoadObjDescFile(struct Element *elem, char **name2, int *key, char ***after) {
    char filename[0x100];
    char buf2[0x100];
    char buf1[0x100];
    char name[0x100];
    struct Sprite *sprite;
    int size;
    struct ResFile *file;
    struct Ride *obj;
    struct Element *e;
    char *text;

    // STRING: LEGOLAND 0x004bc39c
    sprintf(filename, "Objdesc\\%s", elem->path);
    file = RES_OpenFile(filename);
    if (file == NULL) {
        return NULL;
    }

    obj = (struct Ride *)malloc(sizeof(struct Ride));
    memset(obj, 0, sizeof(struct Ride));
    if (obj == NULL) {
        RES_CloseFile(file);
        return NULL;
    }

    obj->element = elem;
    elem->data = NULL;
    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, &obj->instances, size - 4);
    RES_ReadFile(file, &size, 4);
    if (size != 0) {
        obj->desc = malloc(size);
        RES_ReadFile(file, obj->desc, size);
        free(obj->desc);
    }
    obj->desc = NULL;
    RES_ReadFile(file, &size, 4);
    if (size != 0) {
        obj->script = malloc(size);
        RES_ReadFile(file, obj->script, size);
        free(obj->script);
    } else {
        obj->script = NULL;
    }

    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, buf1, size);
    buf1[size] = '\0';
    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, name, size);
    name[size] = '\0';
    if (name[0] != '\0') {
        e = ElemID(name);
        *after = (char **)e;
        if ((e->flags & 0x10) == 0) {
            *after = NULL;
        }
    } else {
        *after = NULL;
    }

    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, buf2, size);
    buf2[size] = '\0';
    RES_ReadFile(file, &size, 4);
    RES_ReadFile(file, name, size);
    name[size] = '\0';
    if (name[0] != '\0') {
        LLIDB_FindElement(name, &LoadedObjDescElement, 0);
        RES_ReadFile(file, &size, 4);
        RES_ReadFile(file, name, size);
        RES_ReadFile(file, &size, 4);
        RES_ReadFile(file, name, size);
        name[size] = '\0';
        RES_ReadFile(file, &size, 4);
        RES_ReadFile(file, ObjDescSpriteName, size);
        ObjDescSpriteName[size] = '\0';
        sprite = NULL;
        if (ObjDescSpriteName[0] != '\0') {
            sprite = LoadSprite(ObjDescSpriteName, 4);
        }
        if (sprite == NULL) {
            // STRING: LEGOLAND 0x004bc34c
            sprite = LoadSprite("InstituteIcon.lls", 4);
        }
        RES_ReadFile(file, &size, 4);
        RES_ReadFile(file, name, size);
        name[size] = '\0';
        RES_ReadFile(file, &size, 4);
        RES_ReadFile(file, name, size);
        name[size] = '\0';
        RES_ReadFile(file, &size, 4);
        RES_ReadFile(file, name, size);
        name[size] = '\0';
        RES_ReadFile(file, &size, 4);
        text = (char *)malloc(size + 1);
        RES_ReadFile(file, text, size);
        text[size] = '\0';
        RES_CloseFile(file);
        *name2 = text;
        *key = obj->cost;
        free(obj);
        return sprite;
    }
    RES_CloseFile(file);
    free(obj);
    return NULL;
}

// "TileData\<file>".
// GLOBAL: LEGOLAND 0x004bc3a8
static const char TileDataPathFormat[] = "TileData\\%s";

// Loads a tile-set (.tsf) element: TileData\<path> holds the sprite count, a name, the sprite ids
// and per-sprite data, then each sprite's file name. Allocates tile space for the sprites, loads
// them (starting animated LLS sprites), and if the file names a parent element, links the sprite
// list to that element's ride. Marks `head` loaded and returns the sprite list.
// FUNCTION: LEGOLAND 0x0047cba0
LEGO_EXPORT void *LLIDB_LoadTSFData(struct Element *head) {
    char filename[0x200];
    char name[0x200];
    unsigned int count;
    int len;
    int ret;
    struct Element *element;
    unsigned int tile_base;
    struct ResFile *file;
    struct FXSpriteList *si;
    struct Image *anim;
    int i;

    char *data_c;
    sprintf(filename, TileDataPathFormat, head->path);
    file = RES_OpenFile(filename);
    if (file == NULL) {
        return NULL;
    }

    si = (struct FXSpriteList *)malloc(sizeof(struct FXSpriteList));
    RES_ReadFile(file, &count, 4);
    si->sprite_ids = (int *)malloc(count * 4);
    si->data_10 = (int *)malloc(count * 4);
    si->count = count;
    si->get_rf_flags = NULL;
    si->on_enter = NULL;
    si->on_leave = NULL;
    RES_ReadFile(file, &len, 4);
    RES_ReadFile(file, name, len);

    i = 0;
    if ((int)count > 0) {
        do {
            data_c = (char *)si->sprite_ids;
            RES_ReadFile(file, data_c + i * 4, 4);
            RES_ReadFile(file, (char *)si->data_10 + i * 4, 4);
            i++;
        } while (i < (int)count);
    }

    si->sprites = (struct Sprite **)AllocTileSpace(si, count, &tile_base);
    si->base = tile_base & 0xffff;

    i = 0;
    if (0 < (int)count) {
        do {
            RES_ReadFile(file, &len, 4);
            RES_ReadFile(file, filename, len);
            filename[len] = '\0';
            si->sprites[i] = LoadSprite(filename, 1);
            anim = si->sprites[i]->image;
            if ((anim->field_14 == 2 || (int)anim->field_14 == 3) && ((struct LLS *)anim->data)->frame_count > 1) {
                LLSPlay((struct LLS *)anim->data, anim);
            }
            i += 1;
        } while (i < (int)count);
    }

    si->data_14 = 0;
    ret = RES_ReadFile(file, &len, 4);
    if (ret == 4) {
        if (len != 0) {
            name[len] = '\0';
            RES_ReadFile(file, name, len);
            if (LLIDB_FindElement(name, &element, NULL) == 0) {
                head->data = si;
                si->data_14 = element;
                LLIDB_LoadData(element);
                ((struct Ride *)element->data)->sprites = si;
            }
        }
    }

    RES_CloseFile(file);
    head->data = si;
    head->flags = head->flags | LLIDB_FLAG_LOADED;
    return si;
}

// FUNCTION: LEGOLAND 0x0047cdd0
void LLIDB_UnLoadTSF(struct Element *param_1) {
    struct FXSpriteList *si = (struct FXSpriteList *)param_1->data;
    int i;

    free(si->sprite_ids);
    free(si->data_10);

    i = 0;
    if (si->count > 0) {
        do {
            KillSprite(si->sprites[i]);
            i++;
        } while (i < si->count);
    }

    FreeTileSpace(si->base, si->count);
    if (si->data_14 != 0) {
        LLIDB_UnLoadData(si->data_14);
    }

    free(si);
}

// FUNCTION: LEGOLAND 0x0047ce40
LEGO_EXPORT void *LLIDB_LoadTSMData(struct Element *head) {
    char filename[0x200];
    char name[0x200];
    int count;
    int len;
    struct Element *element;
    struct ResFile *file;
    struct TSMEntry *entries;
    struct TSMEntry *entry;
    int i;

    sprintf(filename, TileDataPathFormat, head->path);
    file = RES_OpenFile(filename);
    if (file != NULL) {
        RES_ReadFile(file, &count, 4);
        entries = (struct TSMEntry *)malloc(count * sizeof(struct TSMEntry) + sizeof(struct TSMEntry));
        if (entries != NULL) {
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
                    entry->element = element;
                    entry->data = LLIDB_LoadData((void *)element);
                    i++;
                    entry++;
                } while (i < count);
            }

            entries[i].element = (struct Element *)-1;
            entries[i].data = (void *)-1;
            RES_CloseFile(file);
            head->flags |= LLIDB_FLAG_LOADED;
            head->data = entries;
            return entries;
        }
        RES_CloseFile(file);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0047cf80
void LLIDB_UnLoadTSM(struct Element *head) {
    struct TSMEntry *base = (struct TSMEntry *)head->data;

    if (base->data != (void *)-1) {
        struct TSMEntry *q = base;
        struct TSMEntry *p = base;

        do {
            LLIDB_UnLoadData(q->element);
            p++;
            q = p;
        } while (q->data != (void *)-1);
    }
    free(base);
}

// FUNCTION: LEGOLAND 0x0047cfc0
LEGO_EXPORT void *LLIDB_LoadILFData(struct Element *head) {
    char filename[0x200];
    char name[0x200];
    int count;
    short unused;
    int len;
    struct ResFile *file;
    struct SpriteSet *table;
    int i;

    count = 0;
    // STRING: LEGOLAND 0x004bc3b4
    sprintf(filename, "ImageData\\%s", head->path);
    file = RES_OpenFile(filename);
    if (file == NULL) {
        return NULL;
    }

    table = (struct SpriteSet *)malloc(sizeof(struct SpriteSet));
    RES_ReadFile(file, &count, 2);
    RES_ReadFile(file, &unused, 2);
    if (table != NULL) {
        table->offset_x = (int *)malloc(count * 4);
        table->offset_y = (int *)malloc(count * 4);
        table->sprites = (struct Sprite **)malloc(count * sizeof(struct Sprite *));
        table->count = count;
        if (table->offset_x != NULL && table->offset_y != NULL && table->sprites != NULL) {
            RES_ReadFile(file, &len, 4);
            RES_ReadFile(file, name, len);
            i = 0;
            if (count > 0) {
                do {
                    RES_ReadFile(file, (char *)table->offset_x + i * 4, 4);
                    RES_ReadFile(file, (char *)table->offset_y + i * 4, 4);
                    table->offset_x[i] = table->offset_x[i] << 1;
                    table->offset_y[i] = table->offset_y[i] << 1;
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
LEGO_EXPORT void *LLIDB_LoadCSPData(struct Element *head) {
    char filename[0x200];
    char name[0x200];
    int count;
    short unused;
    int len;
    struct ResFile *file;
    struct SpriteSet *table;
    int i;

    count = 0;
    // STRING: LEGOLAND 0x004bc3c4
    sprintf(filename, "CompSprite\\%s", head->path);
    file = RES_OpenFile(filename);
    if (file == NULL) {
        return NULL;
    }

    table = (struct SpriteSet *)malloc(sizeof(struct SpriteSet));
    if (table != NULL) {
        RES_ReadFile(file, &count, 2);
        RES_ReadFile(file, &unused, 2);
        table->offset_x = (int *)malloc(count * 4);
        table->offset_y = (int *)malloc(count * 4);
        table->sprites = (struct Sprite **)malloc(count * sizeof(struct Sprite *));
        table->count = count;
        if (table->offset_x != NULL && table->offset_y != NULL && table->sprites != NULL) {
            RES_ReadFile(file, &len, 4);
            RES_ReadFile(file, name, len);
            i = 0;
            if (count > 0) {
                do {
                    RES_ReadFile(file, (char *)table->offset_x + i * 4, 4);
                    RES_ReadFile(file, (char *)table->offset_y + i * 4, 4);
                    table->offset_x[i] = table->offset_x[i] << 1;
                    table->offset_y[i] = table->offset_y[i] << 1;
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
            for (i = 0; i < count; i++) {
                if (table->sprites[i] == 0) {
                    break;
                }
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
LEGO_EXPORT void *LLIDB_LoadData(struct Element *handle) {
    struct Element *head = (struct Element *)handle;
    unsigned int flags = head->flags;
    void *data;

    if (flags & LLIDB_FLAG_LOADED) {
        head->field_10++;
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
    head->field_10++;
    return data;
}

// FUNCTION: LEGOLAND 0x0047d450
LEGO_EXPORT void LLIDB_UnLoadData(struct Element *handle) {
    struct Element *head = (struct Element *)handle;
    unsigned int flags;

    if (head->field_10 == 0 || --head->field_10 != 0) {
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
        LLIDB_UnLoadODF(head);
        break;
    case LLIDB_TYPE_TSM:
        LLIDB_UnLoadTSM(head);
        break;
    case LLIDB_TYPE_TSF:
        LLIDB_UnLoadTSF((struct Element *)head);
        break;
    case LLIDB_TYPE_ILF:
        LLIDB_FreeILFTable((struct SpriteSet *)head->data);
        break;
    }
}

// FUNCTION: LEGOLAND 0x0047d4c0
LEGO_EXPORT int LLSStop(struct LLS *lls) {
    struct LLSNode *cur;
    struct LLSNode *prev;

    cur = LLSPlayList;
    prev = NULL;
    while (cur != NULL) {
        if (cur->lls == lls) {
            if (prev != NULL) {
                prev->next = cur->next;
            } else {
                LLSPlayList = LLSPlayList->next;
            }
            free(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0047d520
LEGO_EXPORT void LLSPlay(struct LLS *param_1, void *param_2) {
    struct LLSNode *node = LLSPlayList;

    if (param_1 == NULL || param_1->frame_count <= 1) {
        return;
    }
    if (param_1->frame_count > 1000) {
        __asm int 3
    }
    while (node != NULL) {
        if (node->lls == param_1) {
            return;
        }
        node = node->next;
    }
    node = malloc(sizeof(struct LLSNode));
    node->lls = param_1;
    node->param = param_2;
    node->next = LLSPlayList;
    LLSPlayList = node;
}

// FUNCTION: LEGOLAND 0x0047d580
LEGO_EXPORT void LLSPlayOnce(struct LLS *param_1, void *param_2) {
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
void LLSAdvanceFrame(struct LLS *param_1) {
    param_1->frame++;
    if (param_1->frame >= param_1->frame_count) {
        param_1->frame = 0;
    }
}

// FUNCTION: LEGOLAND 0x0047d630
LEGO_EXPORT void LLSAuto(void) {
    struct LLSNode *node;
    struct LLSNode *next;
    struct LLS *lls;

    node = LLSPlayList;
    while (node != NULL) {
        lls = node->lls;
        next = node->next;
        if (lls != NULL) {
            if (lls->loop_delay < 2) {
                lls->frame++;
                if (lls->frame >= lls->frame_count) {
                    lls->frame = 0;
                    if (lls->flags & 4) {
                        lls->flags &= ~4;
                        LLSStop((struct LLS *)lls);
                    }
                }
                lls->loop_delay = lls->delay;
            } else {
                lls->loop_delay -= 2;
            }
        } else {
            // STRING: LEGOLAND 0x004bc3d4
            DBPrintf("Trying to animate Bad Sprite");
        }
        node = next;
    }
}

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
    return _read(SaveFileHandle, buffer, count) == count;
}

// FUNCTION: LEGOLAND 0x0047d760
LEGO_EXPORT unsigned int SaveGameWrite(void *buffer, unsigned int count) {
    return (unsigned int)_write(SaveFileHandle, buffer, count) == count;
}
