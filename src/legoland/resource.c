#include <windows.h>
#include "legoland.h"
#include "crt.h"
#include "resource.h"
#include "globals.h"

struct ResVolume;

struct ResVolume {
    unsigned char pad_0[0x1c];
    HANDLE handle;
    struct ResFile *current;
    unsigned int refcount;
};

struct ResFile {
    int size;
    int base;
    struct ResVolume *volume;
    int position;
};

struct ResVolumeEntry {
    unsigned char pad_0[8];
    unsigned char name[1];
};

struct MasterDirNode {
    struct MasterDirNode *next;
    unsigned int pad_4;
    char *name;
};

struct MasterVolNode {
    struct MasterVolNode *next;
    unsigned int pad_4;
    char name[1];
};

struct ResVolEntry;

struct ResDirNode {
    unsigned char pad_0[8];
    char *name;
};

struct ResVolEntry {
    unsigned char pad_0[8];
    struct ResVolEntry *next;
    struct ResDirNode *dir;
};


// FUNCTION: LEGOLAND 0x00489440
struct MasterDirNode *FUN_00489440(char *name) {
    struct MasterDirNode *node;
    struct MasterDirNode *new_node;
    char *copy;

    for (node = DAT_00798624; node != 0; node = node->next) {
        if (_stricmp(name, node->name) == 0) {
            return node;
        }
    }

    new_node = (struct MasterDirNode *)malloc(0xc);
    copy = (char *)malloc(strlen(name) + 1);
    new_node->name = copy;
    strcpy(copy, name);

    new_node->next = DAT_00798624;
    new_node->pad_4 = 0;
    DAT_00798624 = new_node;
    return new_node;
}

// FUNCTION: LEGOLAND 0x004894d0
LEGO_EXPORT struct MasterDirNode *GetMasterDirPtr(const char *name) {
    struct MasterDirNode *node;

    for (node = DAT_00798624; node != 0; node = node->next) {
        if (_stricmp(name, node->name) == 0) {
            return node;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00489510
LEGO_EXPORT struct MasterVolNode *GetMasterVolPtr(const char *name) {
    struct MasterVolNode *node;

    for (node = DAT_00798628; node != 0; node = node->next) {
        if (_stricmp(name, node->name) == 0) {
            return node;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00489550
struct ResDirNode *FUN_00489550(const char *vol_name, const char *file_name, struct ResVolEntry **out_entry) {
    struct MasterVolNode *master;
    struct ResVolEntry *entry;

    master = GetMasterVolPtr(vol_name);
    if (master != 0) {
        for (entry = (struct ResVolEntry *)master->pad_4; entry != 0; entry = entry->next) {
            if (_stricmp(file_name, entry->dir->name) == 0) {
                *out_entry = entry;
                return entry->dir;
            }
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004895a0
void FUN_004895a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00489740
LEGO_EXPORT unsigned int RES_GetResourcePath(void) {
    return 0;
}

// FUNCTION: LEGOLAND 0x00489750
LEGO_EXPORT struct ResVolume *RES_OpenVolume(const char *path) { STUB(); }

// FUNCTION: LEGOLAND 0x00489a00
LEGO_EXPORT struct ResFile *RES_OpenFileFromVolume(void *param_1, void *name) { STUB(); }

// FUNCTION: LEGOLAND 0x00489b60
LEGO_EXPORT struct ResFile *RES_OpenFile(const char *path) { STUB(); }

// FUNCTION: LEGOLAND 0x00489ce0
LEGO_EXPORT unsigned int RES_GetFileSize(struct ResFile *file) {
    if (file == 0) {
        return 0xffffffff;
    }
    return file->size;
}

// FUNCTION: LEGOLAND 0x00489cf0
LEGO_EXPORT int RES_ReadFile(struct ResFile *file, void *buffer, int count) {
    int remaining;
    int bytes_read;
    struct ResVolume *volume;

    if (file == 0) {
        return -1;
    }

    volume = file->volume;
    if (file != volume->current) {
        volume->current = file;
        SetFilePointer(volume->handle, file->position + file->base, 0, 0);
    }

    remaining = file->size - file->position;
    if (count > remaining) {
        count = remaining;
    }
    if (count == 0) {
        return 0;
    }

    ReadFile(volume->handle, buffer, count, &bytes_read, 0);
    file->position += bytes_read;
    return bytes_read;
}

// FUNCTION: LEGOLAND 0x00489d70
LEGO_EXPORT int RES_SetFilePointer(struct ResFile *file, int offset, int param_3) {
    struct ResVolume *volume;

    if (file != 0) {
        if (offset >= 0) {
            if (offset < file->size) {
                file->position = offset;
                volume = file->volume;
                volume->current = file;
                SetFilePointer(volume->handle, file->base + file->position, 0, 0);
                return offset;
            }
        }
    }
    return -1;
}

// FUNCTION: LEGOLAND 0x00489db0
LEGO_EXPORT int RES_GetFilePointer(struct ResFile *file) {
    if (file == 0) {
        return -1;
    }
    return file->position;
}

// FUNCTION: LEGOLAND 0x00489dc0
LEGO_EXPORT unsigned int RES_CloseVolume(struct ResVolume *volume) {
    if (volume == 0) {
        return 0xffffffff;
    }
    volume->refcount--;
    return volume->refcount;
}

// FUNCTION: LEGOLAND 0x00489de0
LEGO_EXPORT int RES_CloseFile(struct ResFile *file) {
    struct ResVolume *volume;

    if (file == 0) {
        return -1;
    }

    volume = file->volume;
    if (volume->current == file) {
        volume->current = 0;
    }

    RES_CloseVolume(volume);
    free(file);
    return 0;
}

// FUNCTION: LEGOLAND 0x00489e10
LEGO_EXPORT struct ResFile *RES_OpenFileFromVolumePtr(void *param_1, struct ResVolumeEntry *entry) {
    if (entry == 0) {
        return 0;
    }
    return RES_OpenFileFromVolume(param_1, &entry->name);
}

// FUNCTION: LEGOLAND 0x00489e30
LEGO_EXPORT int RES_FileExists(const char *path) {
    struct ResFile *file;

    file = RES_OpenFile(path);
    if (file == 0) {
        return 0;
    }
    RES_CloseFile(file);
    return 1;
}
