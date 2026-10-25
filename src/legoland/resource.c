#include "resource.h"
#include <windows.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdcheck.h"
#include "debug.h"
#include "globals.h"
#include "legoland.h"
#include "saveload.h"

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

struct MasterDirNode {
    struct MasterDirNode *next;
    struct ResVolEntry *entries; /* the directory's first entry (through next_dir) */
    char *name;
};

struct MasterVolNode {
    struct MasterVolNode *next;
    struct ResVolEntry *entries; /* the volume's first entry */
    char name[1];
};

struct ResVolEntry;

struct ResVolEntry {
    struct ResVolEntry *next_all;
    struct ResVolEntry *next_dir;
    struct ResVolEntry *next;
    struct MasterDirNode *dir;
    struct ResVolume *volume;
    int size;
    int base;
    char *name;
};

struct ResDirRecord {
    int child;
    int sibling;
    int is_dir;
    int size;
    int base;
    char name[1];
};

// FUNCTION: LEGOLAND 0x00489440
struct MasterDirNode *FindOrAddMasterDir(char *name) {
    struct MasterDirNode *node;
    struct MasterDirNode *new_node;
    char *copy;

    for (node = MasterDirList; node != 0; node = node->next) {
        if (_stricmp(name, node->name) == 0) {
            return node;
        }
    }

    new_node = (struct MasterDirNode *)malloc(sizeof(struct MasterDirNode));
    copy = (char *)malloc(strlen(name) + 1);
    new_node->name = copy;
    strcpy(copy, name);

    new_node->next = MasterDirList;
    new_node->entries = 0;
    MasterDirList = new_node;
    return new_node;
}

// FUNCTION: LEGOLAND 0x004894d0
LEGO_EXPORT struct MasterDirNode *GetMasterDirPtr(const char *name) {
    struct MasterDirNode *node;

    for (node = MasterDirList; node != 0; node = node->next) {
        if (_stricmp(name, node->name) == 0) {
            return node;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00489510
LEGO_EXPORT struct MasterVolNode *GetMasterVolPtr(const char *name) {
    struct MasterVolNode *node;

    for (node = MasterVolList; node != 0; node = node->next) {
        if (_stricmp(name, node->name) == 0) {
            return node;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00489550
struct MasterDirNode *FindVolumeDirEntry(const char *vol_name, const char *file_name, struct ResVolEntry **out_entry) {
    struct MasterVolNode *master;
    struct ResVolEntry *entry;

    master = GetMasterVolPtr(vol_name);
    if (master != 0) {
        for (entry = (struct ResVolEntry *)master->entries; entry != 0; entry = entry->next) {
            if (_stricmp(file_name, entry->dir->name) == 0) {
                *out_entry = entry;
                return entry->dir;
            }
        }
    }
    return 0;
}

/* Recursively resolves sibling and child offsets in a resource-volume directory and registers its files in the
 * master-directory and volume lists. */
// FUNCTION: LEGOLAND 0x004895a0
void RegisterResourceDirectoryTree(struct ResDirRecord *node, struct ResVolume *volume, struct ResDirRecord *base, char *path) {
    char buffer[0x104];
    struct MasterDirNode *master;
    struct ResVolEntry *entry;

    strcpy(buffer, path);
    master = FindOrAddMasterDir(buffer);

    if (node->is_dir == 0) {
        entry = (struct ResVolEntry *)malloc(sizeof(struct ResVolEntry));
        entry->next_all = (struct ResVolEntry *)AllResVolEntries;
        AllResVolEntries = entry;
        entry->next_dir = (struct ResVolEntry *)master->entries;
        master->entries = entry;
        entry->next = volume->dir;
        volume->dir = entry;
        entry->size = node->size;
        entry->base = node->base;
        entry->name = (char *)malloc(strlen(node->name) + 1);
        strcpy(entry->name, node->name);
        entry->dir = (struct MasterDirNode *)master;
        entry->volume = volume;
    }

    if (node->sibling != -1) {
        node->sibling += (int)base;
        RegisterResourceDirectoryTree((struct ResDirRecord *)node->sibling, volume, base, buffer);
    }

    if (node->child != -1) {
        strcat(buffer, node->name);
        // STRING: LEGOLAND 0x004bdd7c
        strcat(buffer, "\\");
        node->child += (int)base;
        RegisterResourceDirectoryTree((struct ResDirRecord *)node->child, volume, base, buffer);
    }
}

// FUNCTION: LEGOLAND 0x00489740
LEGO_EXPORT unsigned int RES_GetResourcePath(void) {
    return 0;
}

// RES_OpenVolume: opens the resource volume <path's file name>.res, from .\volumes\ first and then from the CD.
// The volume's name is the file name upper-cased (20 chars, zero padded); if a volume with that name is already
// open it is returned instead. A volume file starts with the offset of its directory, which runs to the end of the
// file; the directory is read and turned into entries (RegisterResourceDirectoryTree), and the volume goes to the front of
// MasterVolList with one reference. Returns NULL (and frees everything) if the file can't be opened or read.
// FUNCTION: LEGOLAND 0x00489750
LEGO_EXPORT struct ResVolume *RES_OpenVolume(const char *path) {
    char fname[0x100];
    char res_path[0x104];
    struct ResVolume *volume;
    struct ResVolume *cur;
    unsigned int file_size;
    unsigned int dir_offset;
    DWORD bytes_read;
    void *dir_data;
    int i;
    int name_len;

    HANDLE handle;
    HANDLE close_handle;
    volume = (struct ResVolume *)malloc(sizeof(struct ResVolume));
    cur = MasterVolList;

    _splitpath(path, 0, 0, fname, 0);
    // STRING: LEGOLAND 0x004bde74
    DebugTrace("Attempting to open Resource %s", fname);
    Empty_0047f850();

    name_len = strlen(fname);
    for (i = 0; i < 0x14; i++) {
        if (i < name_len) {
            volume->name[i] = (char)toupper(fname[i]);
        } else {
            volume->name[i] = '\0';
        }
    }

    while (cur != 0) {
        if (_stricmp(cur->name, volume->name) == 0) {
            free(volume);
            // STRING: LEGOLAND 0x004bddc8
            DebugTrace("Volume Already open");
            return cur;
        }
        cur = cur->next;
    }

    // STRING: LEGOLAND 0x004bde60
    sprintf(res_path, ".\\volumes\\%s.res", fname);
    // STRING: LEGOLAND 0x004bde48
    DebugTrace("Trying to open from %s", res_path);
    Empty_0047f850();

    volume->handle = CreateFileA(res_path, 0x80000000, 1, 0, 3, 0x8000000, 0);
    if (INVALID_HANDLE_VALUE == volume->handle) {
        // STRING: LEGOLAND 0x004bde3c
        sprintf(res_path, "%s%s.res", CdDrivePath, fname);
        DebugTrace("Trying to open from %s", res_path);
        Empty_0047f850();
        volume->handle = CreateFileA(res_path, 0x80000000, 1, 0, 3, 0x8000000, 0);
    }

    if (INVALID_HANDLE_VALUE == volume->handle) {
        free(volume);
        return 0;
    }

    // STRING: LEGOLAND 0x004bde28
    DebugTrace("Openned resource %s", res_path);

    file_size = GetFileSize(volume->handle, 0);
    SetFilePointer(volume->handle, 0, 0, 0);
    handle = volume->handle;
    ReadFile(handle, &dir_offset, 4, &bytes_read, 0);
    // STRING: LEGOLAND 0x004bde04
    DebugTrace("FileSize = %x, Directory is at %x", file_size, dir_offset);

    dir_data = malloc(file_size - dir_offset);
    if (dir_data == 0) {
        close_handle = volume->handle;
        // STRING: LEGOLAND 0x004bdddc
        DebugTrace("Failed to allocate space for directory");
        CloseHandle(close_handle);
        free(volume);
        return 0;
    }

    SetFilePointer(volume->handle, dir_offset, 0, 0);
    ReadFile(volume->handle, dir_data, file_size - dir_offset, &bytes_read, 0);
    if (bytes_read != file_size - dir_offset) {
        // STRING: LEGOLAND 0x004bdd94
        DebugTrace("Failed to load directory fully (%x of %x loaded)", bytes_read, file_size - dir_offset);
        free(dir_data);
        CloseHandle(volume->handle);
        free(volume);
        return 0;
    }

    // STRING: LEGOLAND 0x004bdd80
    DebugTrace("Directory read OK");
    volume->dir = 0;
    RegisterResourceDirectoryTree(dir_data, volume, dir_data, EmptyString);
    free(dir_data);

    volume->next = MasterVolList;
    MasterVolList = volume;
    volume->refcount = 1;
    volume->current = 0;
    return volume;
}

// FUNCTION: LEGOLAND 0x00489a00
LEGO_EXPORT struct ResFile *RES_OpenFileFromVolume(const char *path, const char *vol_name) {
    char prefix[0x104];
    char path_copy[0x104];
    struct ResVolEntry *entry;
    struct MasterDirNode *dir;
    struct ResFile *file;
    char *last_bs;
    char *scan;
    char *src;
    int prefix_len;

    last_bs = 0;
    prefix[0] = '\0';
    memset(prefix + 1, 0, sizeof(prefix) - 1);
    src = path_copy;
    scan = path_copy;
    strcpy(src, path);

    for (; scan != 0; scan++) {
        if (*scan == '\0') {
            break;
        }
        if (*scan == '\\') {
            last_bs = scan;
        }
    }

    if (last_bs != 0) {
        prefix_len = (int)(last_bs - path_copy) + 1;
        while (*src == '.' && src[1] == '\\') {
            src += 2;
            prefix_len -= 2;
        }
        memcpy(prefix, src, prefix_len);
        prefix[prefix_len] = '\0';
        last_bs++;
    } else {
        last_bs = path_copy;
    }

    dir = FindVolumeDirEntry(vol_name, prefix, &entry);
    if (dir == 0) {
        return 0;
    }

    while (entry != 0) {
        if (entry->dir == dir) {
            if (_stricmp(entry->name, last_bs) == 0) {
                file = (struct ResFile *)malloc(sizeof(struct ResFile));
                file->size = entry->size;
                file->base = entry->base;
                file->volume = entry->volume;
                file->volume->refcount++;
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
    struct ResVolEntry *entry;
    struct ResFile *file;
    char *scan;
    char *last_slash;
    char *prefix;
    char *file_name;
    int prefix_len;

    base_name[0] = '\0';
    memset(base_name + 1, 0, sizeof(base_name) - 1);
    last_slash = 0;
    prefix = orig_str;
    scan = orig_str;

    if (WaitForLegolandCd(0) == 0) {
        exit(1);
    }

    strcpy(orig_str, path);

    for (; scan != 0; scan++) {
        if (*scan == '\0') {
            break;
        }
        if (*scan == ':') {
            *scan = '\0';
            return RES_OpenFileFromVolume(scan + 1, orig_str);
        }
        if (*scan == '\\') {
            last_slash = scan;
        }
    }

    if (last_slash != 0) {
        prefix_len = (int)(last_slash - orig_str) + 1;
        while (*prefix == '.' && prefix[1] == '\\') {
            prefix += 2;
            prefix_len -= 2;
        }
        memcpy(base_name, prefix, prefix_len);
        base_name[prefix_len] = '\0';
        file_name = last_slash + 1;
    } else {
        file_name = orig_str;
    }

    dir = GetMasterDirPtr(base_name);
    if (dir == 0) {
        return 0;
    }

    for (entry = (struct ResVolEntry *)dir->entries; entry != 0; entry = entry->next_dir) {
        if (_stricmp(entry->name, file_name) == 0) {
            file = (struct ResFile *)malloc(sizeof(struct ResFile));
            file->size = entry->size;
            file->base = entry->base;
            file->volume = entry->volume;
            file->volume->refcount++;
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
    DWORD bytes_read;
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
LEGO_EXPORT int RES_SetFilePointer(struct ResFile *file, int offset) {
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
LEGO_EXPORT struct ResFile *RES_OpenFileFromVolumePtr(void *param_1, struct MasterVolNode *entry) {
    if (entry == 0) {
        return 0;
    }
    return RES_OpenFileFromVolume(param_1, entry->name);
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
