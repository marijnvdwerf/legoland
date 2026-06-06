#include "legoland.h"

#include "gamemap.h"
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

struct CallbackTable {
    unsigned char pad_0[0x8c];
    void *func_8c;
    void *func_90;
    void *func_94;
    void *func_98;
    void *func_9c;
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

struct ObjInstance {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned char pad_c[0xe - 0xc];
    unsigned short uid;
    unsigned int field_10;
};

extern unsigned int LLIDB_LoadData(struct ObjectClass *cls);
extern void SetEditCursorFootPrint(unsigned int param);
extern void CalcBasicObjectCursor(void);
extern void AddBasicObject(void);
extern void StandardRemoveObject(void);

extern void *_malloc(unsigned int size);
extern void FUN_0049e4d0(void *block);
extern int __strcmpi(const char *s1, const char *s2);

extern struct ObjectClass *ObjectClassList;
extern struct RideNode *DAT_00669248;
extern unsigned int DAT_0066924c;
extern struct BestNode *DAT_0066b44c;
extern unsigned int NEWFLC_CheckDuplicate;
extern unsigned int NEWFLC_BuffSize;
extern unsigned short NEWFLC_Repeat;
extern unsigned int EditMode;
extern void *DAT_008119b8;
extern unsigned int GamePad;
extern struct Cursor EditCursor;

// FUNCTION: LEGOLAND 0x00480990
struct ObjectClass *AddNewObjectClass(void) {
    struct ObjectClass *cls;

    cls = (struct ObjectClass *)_malloc(0xd0);
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
    if (__strcmpi(names->name, "Water Works Shower") == 0) {
        info->field_c = 1;
        info->field_10 = 0;
        info->field_24 = 1;
        info->field_25 = 0;
        return;
    }
    // STRING: LEGOLAND 0x004b9828
    if (__strcmpi(names->name, "Water Works Water Block") == 0) {
        info->field_c = 1;
        info->field_10 = 0;
        info->field_24 = 1;
        info->field_25 = 0;
        info->field_2e = 10;
        return;
    }
    // STRING: LEGOLAND 0x004b9840
    if (__strcmpi(names->name, "Water Works Elephant Fountain") == 0) {
        info->field_c = 6;
        info->field_10 = 2;
        info->field_24 = 6;
        info->field_25 = 2;
    }
}

// FUNCTION: LEGOLAND 0x00480b40
unsigned int LoadObjectClass(struct ObjectClass *cls) {
    unsigned int result;

    result = LLIDB_LoadData(cls);
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
    SetEditCursorFootPrint((unsigned int)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00480bb0
unsigned int BasicObjectDCalcCursor(unsigned int param_1, unsigned int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x00480cd0
void SetStandardCallbacks(struct CallbackTable *table) {
    table->func_8c = FUN_00480b70;
    table->func_90 = CalcBasicObjectCursor;
    table->func_94 = BasicObjectDCalcCursor;
    table->func_98 = AddBasicObject;
    table->func_9c = StandardRemoveObject;
}

// FUNCTION: LEGOLAND 0x00480d10
void ClearObjectCounters(void) {
    struct ObjectClass *cls;

    for (cls = ObjectClassList; cls != 0; cls = cls->next) {
        cls->field_8 = 0;
    }
}

// FUNCTION: LEGOLAND 0x00480d30
unsigned int ObjCount(struct ObjCountWrap *wrap) {
    if (wrap == 0) {
        return 0;
    }
    return wrap->inner->count;
}

// FUNCTION: LEGOLAND 0x00480d40
void IncrementObjectCount(struct ObjectCount *count) {
    count->count++;
    count->holder->count++;
}

// FUNCTION: LEGOLAND 0x00480d60
void DecrementObjectCount(struct ObjectCount *count) {
    count->count--;
    count->holder->count--;
}

// FUNCTION: LEGOLAND 0x00480d80
void CreateObjectClasses(void) {
    NEWFLC_Repeat = 0;
    NEWFLC_BuffSize = 1;
    NEWFLC_CheckDuplicate = 1;
}

// FUNCTION: LEGOLAND 0x00480da0
int GetObjCost(struct CostInfo *info) {
    return info->cost;
}

// FUNCTION: LEGOLAND 0x00480db0
unsigned int GetObjSalvageValue(unsigned int param_1, unsigned int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x00480de0
unsigned int GetObjRepairCost(unsigned int param_1, unsigned int param_2) {
    unsigned int salvage;
    unsigned int cost;

    salvage = GetObjSalvageValue(param_1, param_2);
    cost = GetObjCost((struct CostInfo *)param_1);
    return cost - salvage;
}

// FUNCTION: LEGOLAND 0x00480e10
void AllocBlokeCounters(unsigned int size) {
    struct ObjectClass *cls;

    for (cls = ObjectClassList; cls != 0; cls = cls->next) {
        if (cls->flags != 0 && cls->flags != 2) {
            cls->counters = _malloc(size);
        } else {
            cls->counters = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00480e60
void FreeBlokeCounters(void) {
    struct ObjectClass *cls;

    for (cls = ObjectClassList; cls != 0; cls = cls->next) {
        if (cls->counters != 0) {
            FUN_0049e4d0(cls->counters);
            cls->counters = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00480e90
void ClearBlokeCounters(unsigned int index, struct Bloke *bloke) {
    struct ObjectClass *cls;

    for (cls = ObjectClassList; cls != 0; cls = cls->next) {
        if (cls->counters != 0) {
            ((unsigned char *)cls->counters)[index] = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00480ec0
void IncrementBlokeCounter(struct ObjectClass *cls, unsigned int index) {
    unsigned char *counters;

    counters = (unsigned char *)cls->counters;
    if (counters != 0) {
        counters[index]++;
    }
}

// FUNCTION: LEGOLAND 0x00480ee0
int GetBlokeCounter(struct ObjectClass *cls, int index) {
    unsigned char *counters;

    counters = (unsigned char *)cls->counters;
    if (counters != 0) {
        return counters[index];
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00480f00
void LoadObjectLibrary(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004810f0
void UnLoadObjectLibrary(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00481170
void FUN_00481170(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00481200
void BuildObjInfoList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00481410
void CalculateViewRideCode(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00481480
unsigned int CalculateRideCode(unsigned int param_1, void *param_2, unsigned int param_3) {
    int diff;
    int value;

    if (*(short *)((char *)param_2 + 0x38) > (int)param_1) {
        diff = (*(short *)((char *)param_2 + 0x38) - (int)param_1) * 3;
    } else {
        diff = ((int)param_1 - *(short *)((char *)param_2 + 0x38)) * 3;
    }
    diff -= 10;
    if (diff < 0) {
        diff = 0;
    }
    value = *(short *)((char *)param_2 + 0x34) - diff;
    value += 200;
    return value >> param_3;
}

// FUNCTION: LEGOLAND 0x004814c0
unsigned int Calc_Item_Attractiveness(unsigned int param_1, unsigned int param_2, unsigned int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x004815e0
void CalculateRideCodes(unsigned int param_1) {
    struct RideNode *node;

    for (node = DAT_00669248; node != 0; node = node->next) {
        node->field_14 = Calc_Item_Attractiveness(node->field_4, param_1, 0);
    }
}

// FUNCTION: LEGOLAND 0x00481610
void ShuffleObjKeys(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00481690
void ResetBestPtr(void) {
    DAT_0066924c = 0;
}

// FUNCTION: LEGOLAND 0x004816a0
struct ObjInstance *CreateObjectInstance(unsigned int param_1, unsigned short *param_2) {
    struct ObjInstance *obj;

    obj = (struct ObjInstance *)_malloc(0x14);
    if (obj != 0) {
        memset(obj, 0, 0x14);
        obj->uid = *param_2;
        obj->field_8 = param_1;
    }
    return obj;
}

// FUNCTION: LEGOLAND 0x004816e0
void SetEditObject(struct EditObject *obj) {
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
unsigned int FUN_00481720(void) {
    return (unsigned int)DAT_0066b44c;
}

// FUNCTION: LEGOLAND 0x00481730
struct BestNode *FUN_00481730(void) {
    struct BestNode *node;

    node = (struct BestNode *)_malloc(0x24);
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
        FUN_0049e4d0(node);
    } else {
        for (current = DAT_0066b44c; current->next != 0; current = current->next) {
            if (current->next == node) {
                current->next = node->next;
                FUN_0049e4d0(node);
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
