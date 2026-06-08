#include "legoland.h"
#include "crt.h"

#include "profile_io.h"
#include "clipping.h"
#include "globals.h"

#pragma intrinsic(memset, memcpy, strcpy)


struct ProfileNode {
    struct ProfileNode *next;
    char name[0x110];
    int has_header;
    unsigned char slot;
};

struct ProfileNode *FUN_004919a0(unsigned char slot);

struct RideState {
    unsigned char pad_0[0x18];
    unsigned char var_18;
};

struct ProfileObj {
    struct ProfileObj *next;
    unsigned int var_4;
    unsigned int var_8;
    unsigned int var_c;
    unsigned int var_10;
    unsigned int var_14;
    unsigned int var_18;
    unsigned short var_1c;
    unsigned char pad_1e[2];
    unsigned int var_20;
    unsigned int var_24;
    unsigned int var_28;
    unsigned int var_2c;
    unsigned int var_30;
    unsigned int var_34;
};

// FUNCTION: LEGOLAND 0x004912e0
LEGO_EXPORT void ResetTempProfile(void) {
    DAT_007cad60.field_28 = 0x4b;
    DAT_007cad60.field_2c = 0x4b;
    DAT_007cad60.field_30 = 0x4b;
    DAT_007cad60.field_34 = 0;
    DAT_007cad60.field_38 = 0;
    DAT_007cad60.field_3c = 0;
    DAT_007cad60.field_40 = 0;
    DAT_007cad60.field_0 = 0;
    DAT_007cad60.field_20 = 5;
    DAT_007cad60.field_1e = 0;
    DAT_007cad60.field_42 = 0;
    memset(DAT_007cad60.field_43, 0, 200);
    *(int *)&DAT_007cad60.field_10b = 0;
    DAT_007cad60.field_10b = 1;
    FUN_0048a780(DAT_007cad60.field_43);
}

// FUNCTION: LEGOLAND 0x00491360
LEGO_EXPORT int Goto_ProfileDir(void) {
    struct _finddata_t find_data;
    long handle;
    int found;
    int next;

    found = 0;
    handle = _findfirst("profiles", &find_data);
    next = (int)handle;
    while (next != -1) {
        if ((find_data.attrib & 0x10) != 0) {
            found = 1;
        }
        next = _findnext(handle, &find_data);
    }
    _findclose(handle);
    if (!found) {
        return _chdir("profiles") == 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004913e0
LEGO_EXPORT int ReturnFrom_ProfileDir(void) { return 1; }

// FUNCTION: LEGOLAND 0x004913f0
LEGO_EXPORT char ScanForProfiles(void) {
    char path[120];
    char count;
    int index;
    int remaining;
    void *stream;

    count = 0;
    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    index = 1;
    remaining = 7;
    do {
        // STRING: LEGOLAND 0x004bf718
        sprintf(path, "profiles\\Profile%d.txt", index);
        stream = fopen(path, "r");
        if (stream == 0) {
            // STRING: LEGOLAND 0x004bf6fc
            printf("\ncannot open output file");
        } else {
            count++;
            fclose(stream);
        }
        index++;
        remaining--;
    } while (remaining != 0);
    if (ReturnFrom_ProfileDir() == 0) {
        return -1;
    }
    return count;
}

// FUNCTION: LEGOLAND 0x00491470
LEGO_EXPORT char LoadProfilesFormDisk(void) {
    char path[120];
    char data[272];
    char slot;
    char result;
    int index;
    void *stream;

    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    slot = 8;
    index = 8;
    do {
        sprintf(path, "profiles\\Profile%d.txt", index);
        stream = fopen(path, "r");
        if (stream == 0) {
            printf("\ncannot open output file");
            AddNodeToProfileList(0, data, slot);
        } else {
            fread(data, 0x110, 1, stream);
            AddNodeToProfileList(1, data, slot);
            fclose(stream);
        }
        slot--;
        index--;
    } while (slot != 0);
    result = (ReturnFrom_ProfileDir() != 0) - 1;
    return result;
}

// FUNCTION: LEGOLAND 0x00491540
unsigned int FUN_00491540(void) { return DAT_007cad60.field_0 != 0; }

// FUNCTION: LEGOLAND 0x00491550
LEGO_EXPORT void UpDateCurrentSaveSlotInfo(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00491680
LEGO_EXPORT void UpDateCurrentProfile(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004917c0
int FUN_004917c0(int slot) {
    char data[272];
    char path[120];
    struct ProfileNode *node;
    void *stream;

    node = FUN_004919a0((unsigned char)slot);
    if (node == NULL) {
        return 0;
    }
    memset(data, 0, 0x110);
    strcpy(data, node->name);
    *(int *)&data[0x20] = *(int *)&node->name[0x20];
    *(int *)&data[0x28] = *(int *)&node->name[0x28];
    *(int *)&data[0x2c] = *(int *)&node->name[0x2c];
    *(int *)&data[0x30] = *(int *)&node->name[0x30];
    memcpy(&data[0x34], &node->name[0x34], 15);
    memcpy(&data[0x43], &node->name[0x43], 200);
    *(int *)&data[0x10b] = *(int *)&node->name[0x10b];
    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    sprintf(path, "profiles\\Profile%d.txt", slot & 0xff);
    stream = fopen(path, "w+");
    if (stream == 0) {
        printf("\ncannot open output file");
        return 0;
    }
    fwrite(data, 0x110, 1, stream);
    fclose(stream);
    return ReturnFrom_ProfileDir() != 0;
}

// FUNCTION: LEGOLAND 0x00491910
LEGO_EXPORT char SaveProfileToDisk(void) {
    char path[120];
    void *stream;

    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    sprintf(path, "profiles\\Profile%d.txt", DAT_0080ffa0.field_43);
    stream = fopen(path, "w+");
    if (stream == 0) {
        printf("\ncannot open output file");
        return ReturnFrom_ProfileDir() != 0 ? 1 : -1;
    }
    fwrite(&DAT_007cad60, 0x110, 1, stream);
    fclose(stream);
    return ReturnFrom_ProfileDir() != 0 ? 1 : -1;
}

// FUNCTION: LEGOLAND 0x004919a0
struct ProfileNode *FUN_004919a0(unsigned char slot) {
    struct ProfileNode *current = (struct ProfileNode *)DAT_00798890;

    if (current == NULL) {
        return NULL;
    }

    while (1) {
        if (current->slot == slot) {
            return current;
        }
        current = current->next;
        if (current == NULL) {
            return NULL;
        }
    }
}

// FUNCTION: LEGOLAND 0x004919c0
LEGO_EXPORT void AddNodeToProfileList(int load, char *data, char slot) {
    struct ProfileNode *node = (struct ProfileNode *)malloc(0x11c);
    memset(node, 0, 0x11c);
    if (load != 0) {
        strcpy(node->name, data);
        *(int *)&node->name[0x20] = *(int *)&data[0x20];
        *(int *)&node->name[0x28] = *(int *)&data[0x28];
        *(int *)&node->name[0x2c] = *(int *)&data[0x2c];
        *(int *)&node->name[0x30] = *(int *)&data[0x30];
        node->slot = slot;
        node->has_header = 1;
        memcpy(&node->name[0x34], &data[0x34], 15);
        memcpy(&node->name[0x43], &data[0x43], 200);
        *(int *)&node->name[0x10b] = *(int *)&data[0x10b];
        node->next = (struct ProfileNode *)DAT_00798890;
        DAT_00798890 = node;
        return;
    }
    node->slot = slot;
    node->next = (struct ProfileNode *)DAT_00798890;
    DAT_00798890 = node;
}

// FUNCTION: LEGOLAND 0x00491ab0
LEGO_EXPORT void RemoveProfile(unsigned char index) { STUB(); }

// FUNCTION: LEGOLAND 0x00491b50
LEGO_EXPORT void DeleteProfileList(void) {
    struct ProfileNode *current = (struct ProfileNode *)DAT_00798890;

    while (current != NULL) {
        struct ProfileNode *next = current->next;
        free(current);
        current = next;
    }

    DAT_00798890 = NULL;
}

// FUNCTION: LEGOLAND 0x00491bd0
LEGO_EXPORT void EnterNewProfile(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00491d60
LEGO_EXPORT void NewPrintCent(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00491e40
void FUN_00491e40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00491f90
unsigned char FUN_00491f90(struct RideState *state, unsigned char flags) {
    if ((flags & 0x2) != 0) {
        if (state->var_18 == 2) {
            if (DAT_007cad60.field_20 >= 2) {
                DAT_007cad60.field_20--;
                return 1;
            }
        } else {
            if (DAT_007cad60.field_20 <= 99) {
                DAT_007cad60.field_20++;
                return 1;
            }
        }
        return 1;
    } else {
        if ((flags & 0x1) != 0) {
            return 2;
        }
        if ((flags & 0x4) != 0) {
            return 2;
        }
        return 1;
    }
}

// FUNCTION: LEGOLAND 0x004920a0
void FUN_004920a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004920e0
struct ProfileObj *FUN_004920e0(void) {
    struct ProfileObj *v = (struct ProfileObj *)malloc(0x38);
    v->next = 0;
    v->var_4 = 0;
    v->var_c = 0;
    v->var_28 = 0;
    v->var_2c = 0;
    v->var_30 = 0;
    v->var_34 = 0;
    v->var_8 = 0;
    v->var_1c = 0;
    v->var_24 = 0;
    v->var_20 = 0;
    return v;
}

// FUNCTION: LEGOLAND 0x00492110
void FUN_00492110(void) {
    struct ProfileObj *obj = FUN_004920e0();
    obj->next = (struct ProfileObj *)DAT_007988cc;
    DAT_007988cc = obj;
}

// FUNCTION: LEGOLAND 0x00492130
void FUN_00492130(void) { STUB(); }
