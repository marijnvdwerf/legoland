#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "gamemap.h"
#include "llidb.h"
#include "map_object.h"
#include "objclass.h"

struct ObjectClass {
    struct ObjectClass *next;
    unsigned int field_4;
    unsigned int field_8;
    unsigned char pad_c[0x1c - 0xc];
    unsigned int field_1c;
    unsigned short flags;
    unsigned char pad_22[0xc8 - 0x22];
    void *counters;
    unsigned char pad_cc[0xd0 - 0xcc];
};

struct ObjClassNames {
    char *name;
    unsigned char pad_4[0xc - 0x4];
    void *field_c;
};

struct ObjectInfo {
    unsigned char pad_0[0xc];
    unsigned int field_c;
    unsigned int field_10;
    unsigned char pad_14[0x24 - 0x14];
    unsigned char field_24;
    unsigned char field_25;
    unsigned char pad_26[0x2e - 0x26];
    unsigned short field_2e;
};

struct CountHolder {
    unsigned char pad_0[0x10];
    unsigned int count;
};

struct ObjectCount {
    unsigned char pad_0[8];
    unsigned int count;
    unsigned char pad_c[0x5c - 0xc];
    struct CountHolder *holder;
};

struct ObjCountWrap {
    unsigned char pad_0[0xc];
    struct ObjectCount *inner;
};

struct CostInfo {
    unsigned char pad_0[0x26];
    short cost;
};

struct EditObject {
    unsigned char pad_0[0x1c];
    unsigned int flags;
};

struct RideNode {
    struct RideNode *next;
    unsigned int field_4;
    unsigned int field_8;
    unsigned char pad_c[0x14 - 0xc];
    unsigned int field_14;
    unsigned char pad_18[0x20 - 0x18];
};

struct BestNode {
    struct BestNode *next;
    unsigned int pad_4;
    unsigned int x_min;
    unsigned int y_min;
    unsigned int x_max;
    unsigned int y_max;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned int field_20;
};

struct InstancePos {
    unsigned int x;
    unsigned int y;
};

struct RideStats {
    /* 0x00 */ unsigned char pad_0[0x34];
    /* 0x34 */ short base_code;
    /* 0x36 */ unsigned char pad_36[2];
    /* 0x38 */ short rating;
};

struct ObjInstance {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned char pad_c[0xe - 0xc];
    unsigned short uid;
    unsigned int field_10;
};

// FUNCTION: LEGOLAND 0x00480990
LEGO_EXPORT struct ObjectClass *AddNewObjectClass(void) {
    struct ObjectClass *cls;

    cls = (struct ObjectClass *)malloc(0xd0);
    memset(cls, 0, 0xd0);
    if (cls == 0) {
        return cls;
    }
    cls->next = ObjectClassList;
    ObjectClassList = cls;
    cls->field_4 = 0;
    cls->field_8 = 0;
    cls->field_1c = 0;
    return cls;
}

// FUNCTION: LEGOLAND 0x004809d0
void FUN_004809d0(struct ObjectClass *cls) { STUB(); }

// FUNCTION: LEGOLAND 0x00480aa0
void FUN_00480aa0(struct ObjClassNames *names, struct ObjectInfo *info) {
    // STRING: LEGOLAND 0x004bce90
    if (_stricmp(names->name, "Water Works Shower") == 0) {
        info->field_c = 1;
        info->field_10 = 0;
        info->field_24 = 1;
        info->field_25 = 0;
        return;
    }
    // STRING: LEGOLAND 0x004b9828
    if (_stricmp(names->name, "Water Works Water Block") == 0) {
        info->field_c = 1;
        info->field_10 = 0;
        info->field_24 = 1;
        info->field_25 = 0;
        info->field_2e = 10;
        return;
    }
    // STRING: LEGOLAND 0x004b9840
    if (_stricmp(names->name, "Water Works Elephant Fountain") == 0) {
        info->field_c = 6;
        info->field_10 = 2;
        info->field_24 = 6;
        info->field_25 = 2;
    }
}

// FUNCTION: LEGOLAND 0x00480b40
LEGO_EXPORT unsigned int LoadObjectClass(struct ObjectClass *cls) {
    unsigned int result;

    result = (unsigned int)LLIDB_LoadData(cls); /* TODO: fold — LLIDB_LoadData handle as uint */
    if (result != 0) {
        cls->field_8 |= 0x4;
    }
    FUN_004809d0(cls);
    return result;
}

// FUNCTION: LEGOLAND 0x00480b70
void FUN_00480b70(struct ObjClassNames *param) {
    void *value;

    value = param->field_c;
    EditMode = 1;
    DAT_008119b8 = value;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((void *)((unsigned int)DAT_008119b8 + 0x3c));
}

// FUNCTION: LEGOLAND 0x00480bb0
LEGO_EXPORT unsigned int BasicObjectDCalcCursor(unsigned int param_1, unsigned int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x00480cd0
LEGO_EXPORT void SetStandardCallbacks(struct CallbackTable *table) {
    table->cb_8c = FUN_00480b70;
    table->cb_90 = CalcBasicObjectCursor;
    table->cb_94 = BasicObjectDCalcCursor;
    table->cb_98 = AddBasicObject;
    table->cb_9c = StandardRemoveObject;
}

// FUNCTION: LEGOLAND 0x00480d10
LEGO_EXPORT void ClearObjectCounters(void) {
    struct ObjectClass *cls;

    for (cls = ObjectClassList; cls != 0; cls = cls->next) {
        cls->field_8 = 0;
    }
}

// FUNCTION: LEGOLAND 0x00480d30
LEGO_EXPORT unsigned int ObjCount(struct ObjCountWrap *wrap) {
    if (wrap == 0) {
        return 0;
    }
    return wrap->inner->count;
}

// FUNCTION: LEGOLAND 0x00480d40
LEGO_EXPORT void IncrementObjectCount(struct ObjectCount *count) {
    count->count++;
    count->holder->count++;
}

// FUNCTION: LEGOLAND 0x00480d60
LEGO_EXPORT void DecrementObjectCount(struct ObjectCount *count) {
    count->count--;
    count->holder->count--;
}

// FUNCTION: LEGOLAND 0x00480d80
LEGO_EXPORT void CreateObjectClasses(void) {
    NEWFLC_Repeat = 0;
    NEWFLC_BuffSize = 1;
    NEWFLC_CheckDuplicate = 1;
}

// FUNCTION: LEGOLAND 0x00480da0
LEGO_EXPORT int GetObjCost(struct CostInfo *info) {
    return info->cost;
}

// FUNCTION: LEGOLAND 0x00480db0
LEGO_EXPORT unsigned int GetObjSalvageValue(unsigned int param_1, unsigned int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x00480de0
LEGO_EXPORT unsigned int GetObjRepairCost(unsigned int param_1, unsigned int param_2) {
    unsigned int salvage;
    unsigned int cost;

    salvage = GetObjSalvageValue(param_1, param_2);
    cost = GetObjCost((struct CostInfo *)param_1);
    return cost - salvage;
}

// FUNCTION: LEGOLAND 0x00480e10
LEGO_EXPORT void AllocBlokeCounters(unsigned int size) {
    struct ObjectClass *cls;

    for (cls = ObjectClassList; cls != 0; cls = cls->next) {
        if (cls->flags != 0 && cls->flags != 2) {
            cls->counters = malloc(size);
        } else {
            cls->counters = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00480e60
LEGO_EXPORT void FreeBlokeCounters(void) {
    struct ObjectClass *cls;

    for (cls = ObjectClassList; cls != 0; cls = cls->next) {
        if (cls->counters != 0) {
            free(cls->counters);
            cls->counters = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00480e90
LEGO_EXPORT void ClearBlokeCounters(unsigned int index, struct Bloke *bloke) {
    struct ObjectClass *cls;

    for (cls = ObjectClassList; cls != 0; cls = cls->next) {
        if (cls->counters != 0) {
            ((unsigned char *)cls->counters)[index] = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00480ec0
LEGO_EXPORT void IncrementBlokeCounter(struct ObjectClass *cls, unsigned int index) {
    unsigned char *counters;

    counters = (unsigned char *)cls->counters;
    if (counters != 0) {
        counters[index]++;
    }
}

// FUNCTION: LEGOLAND 0x00480ee0
LEGO_EXPORT int GetBlokeCounter(struct ObjectClass *cls, int index) {
    unsigned char *counters;

    counters = (unsigned char *)cls->counters;
    if (counters != 0) {
        return counters[index];
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00480f00
LEGO_EXPORT int LoadObjectLibrary(void *obj, const char *name) { STUB(); }

// FUNCTION: LEGOLAND 0x004810f0
LEGO_EXPORT void UnLoadObjectLibrary(void *library) { STUB(); }

// FUNCTION: LEGOLAND 0x00481170
void FUN_00481170(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00481200
LEGO_EXPORT void BuildObjInfoList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00481410
LEGO_EXPORT void CalculateViewRideCode(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00481480
LEGO_EXPORT unsigned int CalculateRideCode(unsigned int param_1, struct RideStats *param_2, unsigned int param_3) {
    int diff;
    int value;

    if (param_2->rating > (int)param_1) {
        diff = (param_2->rating - (int)param_1) * 3;
    } else {
        diff = ((int)param_1 - param_2->rating) * 3;
    }
    diff -= 10;
    if (diff < 0) {
        diff = 0;
    }
    value = param_2->base_code - diff;
    value += 200;
    return value >> param_3;
}

// FUNCTION: LEGOLAND 0x004814c0
LEGO_EXPORT unsigned int Calc_Item_Attractiveness(unsigned int param_1, unsigned int param_2, unsigned int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x004815e0
LEGO_EXPORT void CalculateRideCodes(unsigned int param_1) {
    struct RideNode *node;

    for (node = DAT_00669248; node != 0; node = node->next) {
        node->field_14 = Calc_Item_Attractiveness(node->field_4, param_1, 0);
    }
}

// FUNCTION: LEGOLAND 0x00481610
LEGO_EXPORT void ShuffleObjKeys(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00481690
LEGO_EXPORT void ResetBestPtr(void) {
    DAT_0066924c = 0;
}

// FUNCTION: LEGOLAND 0x004816a0
LEGO_EXPORT struct ObjInstance *CreateObjectInstance(unsigned int param_1, unsigned short *param_2) {
    struct ObjInstance *obj;

    obj = (struct ObjInstance *)malloc(sizeof(struct ObjInstance));
    if (obj != 0) {
        memset(obj, 0, sizeof(struct ObjInstance));
        obj->uid = *param_2;
        obj->field_8 = param_1;
    }
    return obj;
}

// FUNCTION: LEGOLAND 0x004816e0
LEGO_EXPORT void SetEditObject(struct EditObject *obj) {
    EditMode = 1;
    DAT_008119b8 = obj;
    if (obj == 0) {
        return;
    }
    if ((obj->flags & 0x2000000) != 0) {
        GamePad |= 0x400;
    } else {
        GamePad &= ~0x400;
    }
}

// FUNCTION: LEGOLAND 0x00481720
void *FUN_00481720(void) {
    return DAT_0066b44c;
}

// FUNCTION: LEGOLAND 0x00481730
struct BestNode *FUN_00481730(void) {
    struct BestNode *node;

    node = (struct BestNode *)malloc(0x24);
    node->field_20 = 0;
    node->field_1c = 0;
    node->next = DAT_0066b44c;
    DAT_0066b44c = node;
    return node;
}

// FUNCTION: LEGOLAND 0x00481750
void FUN_00481750(struct BestNode *node) {
    struct BestNode *current;

    if (DAT_0066b44c == node) {
        DAT_0066b44c = node->next;
        free(node);
    } else {
        for (current = DAT_0066b44c; current->next != 0; current = current->next) {
            if (current->next == node) {
                current->next = node->next;
                free(node);
                return;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00481790
struct BestNode *FUN_00481790(struct InstancePos *pos) {
    struct BestNode *node;

    node = DAT_0066b44c;
    if (node == 0) {
        return 0;
    }
    while (node != 0) {
        if ((int)pos->x >= (int)node->x_min && (int)pos->x <= (int)node->x_max &&
            (int)pos->y >= (int)node->y_min && (int)pos->y <= (int)node->y_max) {
            return node;
        }
        node = node->next;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004817d0
void FUN_004817d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00481810
void FUN_00481810(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004819a0
void FUN_004819a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00481b10
void FUN_00481b10(struct BestNode *node) { STUB(); }
