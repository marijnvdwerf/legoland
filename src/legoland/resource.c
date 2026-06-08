#include <windows.h>
#include "legoland.h"
#include "crt.h"
#include "cdcheck.h"
#include "debug.h"
#include "saveload.h"
#include "resource.h"
#include "globals.h"

struct ResVolume;
struct ResVolEntry;

struct ResVolume {
    struct ResVolume *next;
    struct ResVolEntry *dir;
    char name[0x14];
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
    struct ResVolume *volume;
    int size;
    int base;
    char *name;
};

struct ResDirEntry {
    unsigned char pad_0[4];
    struct ResDirEntry *next;
    unsigned char pad_8[8];
    struct ResVolume *volume;
    int size;
    int base;
    char *name;
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
void FUN_004895a0(void *dir_data, struct ResVolume *volume, void *dir_base, void *master) { STUB(); }

// FUNCTION: LEGOLAND 0x00489740
LEGO_EXPORT unsigned int RES_GetResourcePath(void) {
    return 0;
}

// FUNCTION: LEGOLAND 0x00489750
LEGO_EXPORT struct ResVolume *RES_OpenVolume(const char *path) {
    char fname[0x100];
    char res_path[0x104];
    struct ResVolume *volume;
    struct ResVolume *cur;
    unsigned int file_size;
    unsigned int dir_offset;
    unsigned int bytes_read;
    void *dir_data;
    int name_len;
    int i;

    volume = (struct ResVolume *)malloc(0x28);
    cur = DAT_00798628;

    _splitpath(path, 0, 0, fname, 0);
    // STRING: LEGOLAND 0x004bde74
    FUN_0047f870("Attempting to open Resource %s", fname);
    FUN_0047f850();

    name_len = strlen(fname);
    for (i = 0; i < 0x14; i++) {
        if (i < name_len) {
            volume->name[i] = (char)toupper(fname[i]);
        } else {
            volume->name[i] = '\0';
        }
    }

    while (cur != 0) {
        if (_stricmp(volume->name, cur->name) == 0) {
            free(volume);
            // STRING: LEGOLAND 0x004bddc8
            FUN_0047f870("Volume Already open");
            return cur;
        }
        cur = cur->next;
    }

    // STRING: LEGOLAND 0x004bde60
    sprintf(res_path, ".\\volumes\\%s.res", fname);
    // STRING: LEGOLAND 0x004bde48
    FUN_0047f870("Trying to open from %s", res_path);
    FUN_0047f850();

    volume->handle = CreateFileA(res_path, 0x80000000, 1, 0, 3, 0x8000000, 0);
    if (volume->handle == INVALID_HANDLE_VALUE) {
        // STRING: LEGOLAND 0x004bde3c
        sprintf(res_path, "%s%s.res", DAT_00813b04, fname);
        FUN_0047f870("Trying to open from %s", res_path);
        FUN_0047f850();
        volume->handle = CreateFileA(res_path, 0x80000000, 1, 0, 3, 0x8000000, 0);
    }

    if (volume->handle == INVALID_HANDLE_VALUE) {
        free(volume);
        return 0;
    }

    // STRING: LEGOLAND 0x004bde28
    FUN_0047f870("Openned resource %s", res_path);

    file_size = GetFileSize(volume->handle, 0);
    SetFilePointer(volume->handle, 0, 0, 0);
    ReadFile(volume->handle, &dir_offset, 4, &bytes_read, 0);
    // STRING: LEGOLAND 0x004bde04
    FUN_0047f870("FileSize = %x, Directory is at %x", file_size, dir_offset);

    dir_data = malloc(file_size - dir_offset);
    if (dir_data == 0) {
        // STRING: LEGOLAND 0x004bdddc
        FUN_0047f870("Failed to allocate space for directory");
        CloseHandle(volume->handle);
        free(volume);
        return 0;
    }

    SetFilePointer(volume->handle, dir_offset, 0, 0);
    ReadFile(volume->handle, dir_data, file_size - dir_offset, &bytes_read, 0);
    if (bytes_read != file_size - dir_offset) {
        // STRING: LEGOLAND 0x004bdd94
        FUN_0047f870("Failed to load directory fully (%x of %x loaded)", bytes_read, file_size - dir_offset);
        free(dir_data);
        CloseHandle(volume->handle);
        free(volume);
        return 0;
    }

    // STRING: LEGOLAND 0x004bdd80
    FUN_0047f870("Directory read OK");
    volume->dir = 0;
    FUN_004895a0(dir_data, volume, dir_data, DAT_004d8bb0);
    free(dir_data);

    volume->next = DAT_00798628;
    DAT_00798628 = volume;
    volume->refcount = 1;
    volume->current = 0;
    return volume;
}

// FUNCTION: LEGOLAND 0x00489a00
LEGO_EXPORT struct ResFile *RES_OpenFileFromVolume(const char *path, const char *vol_name) {
    char prefix[0x104];
    char path_copy[0x104];
    struct ResVolEntry *entry;
    struct ResDirNode *dir;
    struct ResFile *file;
    char *last_bs;
    char *file_name;
    char *scan;
    char *src;
    int prefix_len;

    prefix[0] = '\0';
    memset(prefix + 1, 0, sizeof(prefix) - 1);
    memcpy(path_copy, path, strlen(path) + 1);

    last_bs = 0;
    scan = path_copy;
    while (*scan != '\0') {
        if (*scan == '\\') {
            last_bs = scan;
        }
        scan++;
    }

    if (last_bs == 0) {
        file_name = path_copy;
    } else {
        src = path_copy;
        prefix_len = (int)(last_bs - path_copy) + 1;
        if (path_copy[0] == '.') {
            while (src[1] == '\\') {
                src += 2;
                prefix_len -= 2;
                if (src[0] != '.') {
                    break;
                }
            }
        }
        memcpy(prefix, src, prefix_len);
        prefix[prefix_len] = '\0';
        file_name = last_bs + 1;
    }

    dir = FUN_00489550(vol_name, prefix, &entry);
    if (dir == 0) {
        return 0;
    }

    while (entry != 0) {
        if (entry->dir == dir) {
            if (_stricmp(entry->name, file_name) == 0) {
                file = (struct ResFile *)malloc(0x10);
                file->size = entry->size;
                file->base = entry->base;
                file->volume = entry->volume;
                entry->volume->refcount++;
                file->position = 0;
                return file;
            }
        }
        entry = entry->next;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00489b60
LEGO_EXPORT struct ResFile *RES_OpenFile(const char *path) {
    char base_name[0x104];
    char orig_str[0x104];
    struct MasterDirNode *dir;
    struct ResDirEntry *entry;
    struct ResFile *file;
    char *scan;
    char *last_slash;
    char *prefix;
    char *file_name;
    int prefix_len;

    base_name[0] = '\0';
    memset(base_name + 1, 0, sizeof(base_name) - 1);
    orig_str[0] = '\0';

    if (FUN_004515e0(0) == 0) {
        exit(1);
    }

    memcpy(orig_str, path, strlen(path) + 1);

    last_slash = 0;
    scan = orig_str;
    while (*scan != '\0') {
        if (*scan == ':') {
            *scan = '\0';
            return RES_OpenFileFromVolume(scan + 1, orig_str);
        }
        if (*scan == '\\') {
            last_slash = scan;
        }
        scan++;
    }

    if (last_slash == 0) {
        file_name = orig_str;
    } else {
        prefix = orig_str;
        prefix_len = (int)(last_slash - orig_str) + 1;
        if (orig_str[0] == '.') {
            while (prefix[1] == '\\') {
                prefix += 2;
                prefix_len -= 2;
                if (prefix[0] != '.') {
                    break;
                }
            }
        }
        memcpy(base_name, prefix, prefix_len);
        base_name[prefix_len] = '\0';
        file_name = last_slash + 1;
    }

    dir = GetMasterDirPtr(base_name);
    if (dir == 0) {
        return 0;
    }

    for (entry = (struct ResDirEntry *)dir->pad_4; entry != 0; entry = entry->next) {
        if (_stricmp(entry->name, file_name) == 0) {
            file = (struct ResFile *)malloc(0x10);
            file->size = entry->size;
            file->base = entry->base;
            file->volume = entry->volume;
            entry->volume->refcount++;
            file->position = 0;
            return file;
        }
    }
    return 0;
}

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
