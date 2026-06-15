#include "obj_instance.h"
#include <stdlib.h>
#include "globals.h"
#include "legoland.h"
#include "resource.h"

struct ObjClassKey {
    unsigned short hi;
    unsigned char pad_2[2];
    unsigned short lo;
};

struct InstanceNode {
    struct InstanceNode *prev;
    struct InstanceNode *next;
    struct InstanceList *list;
};

struct InstanceList {
    unsigned int pad_0;
    struct InstanceNode *next;
};

struct ObjClassNode {
    struct ObjClassNode *next;
    void *instances;
};

struct ObjInstance {
    struct ObjInstance *next;
    unsigned char pad_4[10];
    unsigned short uid;
};

struct RideRider {
    unsigned char pad_0[0x60];
    unsigned char count;
    unsigned short flags;
};

struct RideNode {
    struct RideNode *next;
    unsigned int pad_4;
    struct RideRider *rider;
    unsigned short uid;
};

struct Ride {
    unsigned char pad_0[0xcc];
    struct RideNode *riders;
};

// FUNCTION: LEGOLAND 0x00489e60
char *FUN_00489e60(struct ResFile *file, char *dest, int maxlen) {
    int error;
    int count;
    int buf;
    char *out;

    error = 0;
    count = 0;
    out = dest;
    do {
        if (RES_ReadFile(file, &buf, 1) == 0) {
            error = 1;
            goto skip_cr;
        }
        if ((char)buf == '\r') {
            goto extra_read;
        }
        if ((char)buf == '\n') {
            goto done;
        }
        *out++ = (char)buf;
        count++;
    } while (count < maxlen);
skip_cr:
    if ((char)buf == '\r') {
extra_read:
        RES_ReadFile(file, &buf, 1);
    }
done:
    *out = '\0';
    if (error && count == 0) {
        return 0;
    }
    return dest;
}

// FUNCTION: LEGOLAND 0x00489ee0
void FUN_00489ee0(void) {
    struct ObjTableEntry *entry;

    for (entry = DAT_007cb3e0; (int)entry < (int)&DAT_007cb5e0; entry++) {
        entry->key = 0xffff;
    }
}

// FUNCTION: LEGOLAND 0x00489f00
int FUN_00489f00(const struct ObjClassKey *key) {
    int index;
    struct ObjTableEntry *entry;

    index = 0;
    for (entry = DAT_007cb3e0; (int)entry < (int)&DAT_007cb5e0; entry++) {
        if (entry->key == 0xffff) {
            DAT_007cb3e0[index].key = (key->hi << 8) + key->lo;
            DAT_007cb3e2[index].key = 0;
            return 1;
        }
        index++;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00489f50
int FUN_00489f50(const struct ObjClassKey *key) {
    int index;
    unsigned short target;
    struct ObjTableEntry *entry;

    index = 0;
    target = (key->hi << 8) + key->lo;
    for (entry = DAT_007cb3e0; (int)entry < (int)&DAT_007cb5e0; entry++) {
        if (entry->key == target) {
            DAT_007cb3e0[index].key = 0xffff;
            return 1;
        }
        index++;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00489f90
int FUN_00489f90(const struct ObjClassKey *key) {
    int index;
    unsigned short target;
    struct ObjTableEntry *entry;

    index = 0;
    target = (key->hi << 8) + key->lo;
    for (entry = DAT_007cb3e0; (int)entry < (int)&DAT_007cb5e0; entry++) {
        if (entry->key == target) {
            DAT_007cb3e2[index].key++;
            return 1;
        }
        index++;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00489fd0
unsigned short FUN_00489fd0(const struct ObjClassKey *key) {
    int index;
    unsigned short target;
    struct ObjTableEntry *entry;

    index = 0;
    target = (key->hi << 8) + key->lo;
    for (entry = DAT_007cb3e0; (int)entry < (int)&DAT_007cb5e0; entry++) {
        if (entry->key == target) {
            return DAT_007cb3e2[index].key;
        }
        index++;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0048a010
LEGO_EXPORT void AddInstanceToList(struct InstanceNode *node) {
    struct InstanceList *list;

    list = node->list;
    node->next = 0;
    node->prev = 0;
    if (list->next == 0) {
        list->next = node;
        return;
    }
    node->prev = list->next;
    list->next->next = node;
    node->list->next = node;
}

// FUNCTION: LEGOLAND 0x0048a040
void FUN_0048a040(void) {
    struct ObjClassNode *node;

    for (node = (struct ObjClassNode *)ObjectClassList; node != 0; node = node->next) {
        void *obj = node->instances;
        while (obj != 0) {
            void *next = *(void **)obj;
            free(obj);
            obj = next;
        }
        node->instances = 0;
    }
}

// FUNCTION: LEGOLAND 0x0048a080
LEGO_EXPORT void RemoveInstanceFromList(struct InstanceNode *node) {
    if (node->next == 0) {
        node->list->next = node->prev;
        if (node->list->next != 0) {
            node->list->next->next = 0;
        }
    } else {
        node->next->prev = node->prev;
    }

    if (node->prev != 0) {
        node->prev->next = node->next;
    }

    node->prev = 0;
    node->next = 0;
}

// FUNCTION: LEGOLAND 0x0048a0c0
LEGO_EXPORT struct ObjInstance *GetInstanceOfClass(struct ObjClassNode *cls, const unsigned short *uid) {
    struct ObjInstance *instance;

    instance = (struct ObjInstance *)cls->instances;
    if (instance == 0) {
        return 0;
    }
    while (instance->uid != *uid) {
        instance = instance->next;
        if (instance == 0) {
            return 0;
        }
    }
    return instance;
}

// FUNCTION: LEGOLAND 0x0048a0f0
LEGO_EXPORT void HandleRideAI(void) {}

// FUNCTION: LEGOLAND 0x0048a100
LEGO_EXPORT void RemoveBlokeFromRide(void *ride, void *node) { STUB(); }

// FUNCTION: LEGOLAND 0x0048a2d0
LEGO_EXPORT void UpdateBlokesOnRide(void) {}

// FUNCTION: LEGOLAND 0x0048a2e0
LEGO_EXPORT void RemoveAllBlokesFromRide(unsigned int arg1, void *arg2) { STUB(); }

// FUNCTION: LEGOLAND 0x0048a390
LEGO_EXPORT int GetAllBlokesOffRide(struct Ride *ride, unsigned short uid) {
    struct RideNode *node;
    struct RideNode *next;

    for (node = ride->riders; node != 0; node = next) {
        next = node->next;
        if (node->uid == uid) {
            struct RideRider *rider = node->rider;
            if ((rider->flags & 0x40) == 0) {
                rider->flags |= 0x8;
                rider->count++;
            }
        }
    }
    return 1;
}

struct MapObj {
    unsigned char pad_0[0xc];
    struct ClassOffset *field_c;
};

struct ClassOffset {
    unsigned char pad_0[0xc];
    int field_c;
    int field_10;
};

// FUNCTION: LEGOLAND 0x0048a3e0
LEGO_EXPORT int GetObjectUID(int *param_1, struct ClassOffset *param_2) {
    int x;
    int y;
    int row;
    struct MapElement *element;

    x = *param_1 >> 8;
    y = param_1[1] >> 8;
    row = y - 1;
    if (x >= 0 && x < lpConfig->width && row >= 0 && row < lpConfig->height && (element = GameMap[row] + x) != 0) {
        if ((element->flags & 0x80) != 0 && element->field_0 != 0 && ((struct MapObj *)element->field_0)->field_c == param_2 &&
            element->field_4 + param_2->field_c == x && element->field_5 + param_2->field_10 == y) {
            return (x & 0xffff0000) | *(unsigned short *)&element->field_4;
        }
        row = y + 1;
        if (x < 0 || lpConfig->width <= x || row < 0 || lpConfig->height <= row) {
            element = 0;
        } else {
            element = GameMap[row] + x;
        }
        if ((element->flags & 0x80) != 0 && element->field_0 != 0 && ((struct MapObj *)element->field_0)->field_c == param_2 &&
            element->field_4 + param_2->field_c == x && element->field_5 + param_2->field_10 == y) {
            return (x & 0xffff0000) | *(unsigned short *)&element->field_4;
        }
    }
    row = x - 1;
    if (row >= 0 && row < lpConfig->width && y >= 0 && y < lpConfig->height && (element = GameMap[y] + row) != 0 &&
        (element->flags & 0x80) != 0 && element->field_0 != 0 && ((struct MapObj *)element->field_0)->field_c == param_2 &&
        element->field_4 + param_2->field_c == x && element->field_5 + param_2->field_10 == y) {
        return (x & 0xffff0000) | *(unsigned short *)&element->field_4;
    }
    row = x + 1;
    if (row >= 0 && row < lpConfig->width && y >= 0 && y < lpConfig->height && (element = GameMap[y] + row) != 0 &&
        (element->flags & 0x80) != 0 && element->field_0 != 0 && ((struct MapObj *)element->field_0)->field_c == param_2 &&
        element->field_4 + param_2->field_c == x && element->field_5 + param_2->field_10 == y) {
        return (x & 0xffff0000) | *(unsigned short *)&element->field_4;
    }
    return x & 0xffff0000;
}
