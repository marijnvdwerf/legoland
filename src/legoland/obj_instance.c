#include "legoland.h"
#include <stdlib.h>
#include "obj_instance.h"
#include "globals.h"

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
void FUN_00489e60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00489ee0
void FUN_00489ee0(void) {
    struct ObjTableEntry *entry;

    for (entry = DAT_007cb3e0; (int)entry < (int)&DAT_007cb5e0; entry++) {
        entry->key = 0xffff;
    }
}

// FUNCTION: LEGOLAND 0x00489f00
void FUN_00489f00(void) { STUB(); }

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
LEGO_EXPORT void AddInstanceToList(void) { STUB(); }

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

// FUNCTION: LEGOLAND 0x0048a3e0
LEGO_EXPORT void GetObjectUID(void) { STUB(); }
