#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "bloke_ai.h"
#include "gamemap.h"
#include "llidb.h"
#include "map_object.h"
#include "math.h"
#include "objclass.h"

struct ObjectClass {
    struct ObjectClass *next;
    unsigned int field_4;
    unsigned int field_8;
    unsigned char pad_c[0x1c - 0xc];
    unsigned int field_1c;
    short flags;
    unsigned char pad_22[0x36 - 0x22];
    short field_36;
    short field_38;
    unsigned char pad_3a[0xc8 - 0x3a];
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
    unsigned char pad_28[0x2c - 0x28];
    unsigned char divisor;
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

struct FootprintNode {
    /* 0x00 */ int x_min;
    /* 0x04 */ unsigned char pad_4[4];
    /* 0x08 */ int x_max;
    /* 0x0c */ unsigned char pad_c[4];
    /* 0x10 */ struct FootprintNode *next;
};

struct ClassObjNode {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ int field_c;
    /* 0x10 */ struct ClassObjNode *next;
};

struct ObjClassInfo {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ int field_c;
    /* 0x10 */ int field_10;
    /* 0x14 */ unsigned char pad_14[0x20 - 0x14];
    /* 0x20 */ short type;
    /* 0x22 */ unsigned char pad_22[0x24 - 0x22];
    /* 0x24 */ char field_24;
    /* 0x25 */ char field_25;
    /* 0x26 */ unsigned char pad_26[0x3c - 0x26];
    /* 0x3c */ int footprint_base;
    /* 0x40 */ unsigned char pad_40[0x48 - 0x40];
    /* 0x48 */ int field_48;
    /* 0x4c */ struct ClassObjNode *objlist;
};

struct InfoNode {
    /* 0x00 */ struct InfoNode *next;
    /* 0x04 */ int classid;
    /* 0x08 */ short coords;
    /* 0x0a */ unsigned char pad_a[0xc - 0xa];
    /* 0x0c */ int x;
    /* 0x10 */ int y;
    /* 0x14 */ int sort_key;
    /* 0x18 */ unsigned char origin_x;
    /* 0x19 */ unsigned char origin_y;
    /* 0x1a */ unsigned char pad_1a[0x1c - 0x1a];
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

struct ObjClassAlias {
    const char *name;
    const char *aliases;
};

struct ElemView {
    /* 0x00 */ char *name;
    /* 0x04 */ unsigned char pad_4[4];
    /* 0x08 */ unsigned int flags;
};

static const struct ObjClassAlias ObjClassAliasTable[] = {
    // STRING: LEGOLAND 0x004b5c0c
    {"CASTLE OBJ",
     // STRING: LEGOLAND 0x004bce60
     "CASTLE_DUMMY;ROLLER COASTER TRACK;SQUARE_TRACK;"},
    // STRING: LEGOLAND 0x004b4bb4
    {"LOG FLUME ENTRANCE",
     // STRING: LEGOLAND 0x004bce4c
     "LOG FLUME TRACK;"},
    // STRING: LEGOLAND 0x004b80c0
    {"JUNGLE CRUISE",
     // STRING: LEGOLAND 0x004bce34
     "JUNGLE CRUISE WATER;"},
    // STRING: LEGOLAND 0x004b80b0
    {"DRIVING SCHOOL",
     // STRING: LEGOLAND 0x004bce0c
     "DRIVING SCHOOL ROADS;ZEBRA CROSSING;"},
    // STRING: LEGOLAND 0x004b536c
    {"BOATING SCHOOL",
     // STRING: LEGOLAND 0x004bcdf4
     "BOATING SCHOOL WATER;"},
};

// FUNCTION: LEGOLAND 0x004809d0
void FUN_004809d0(struct ElemView *cls) {
    const char *const *slot;
    char *token;
    char *semicolon;
    struct ElemView *elem;
    char buffer[256];

    slot = &ObjClassAliasTable[0].aliases;
    do {
        if (_stricmp(cls->name, slot[-1]) == 0) {
            strcpy(buffer, slot[0]);
            token = buffer;
            semicolon = strchr(buffer, ';');
            while (semicolon != 0) {
                *semicolon = '\0';
                elem = (struct ElemView *)ElemID(token);
                if (elem != 0 && (elem->flags & 4) == 0 && LLIDB_LoadData(elem) != 0) {
                    elem->flags |= 4;
                }
                token = semicolon + 1;
                *semicolon = ';';
                semicolon = strchr(token, ';');
            }
        }
        slot += 2;
    } while (slot < &ObjClassAliasTable[5].aliases);
}

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
LEGO_EXPORT unsigned int LoadObjectClass(struct ElemView *cls) {
    unsigned int result;

    result = (unsigned int)LLIDB_LoadData(cls);
    if (result != 0) {
        cls->flags |= 0x4;
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
LEGO_EXPORT void BasicObjectDCalcCursor(unsigned int param_1, struct Point *param_2) {
    struct MapElement **map;
    struct LegoConfig *config;
    int x;
    int y;
    unsigned int buf[5];

    map = GameMap;
    config = lpConfig;
    x = param_2->x;
    if (x < 0 || x >= (int)config->width || (y = param_2->y) < 0 || y >= (int)config->height) {
        *(unsigned short *)&buf[3] = 0x40;
    } else {
        memcpy(buf, &GameMap[y][x], 20);
    }
    memcpy(QueryCursor.field_1414, (char *)QueryClass + 0x3c, 20);
    QueryCursor.field_1828 = 8;
    if ((buf[3] & 0x8a0) != 0) {
        QueryCursor.field_1828 = 9;
        QueryCursor.field_1404 = ((unsigned char *)&QueryObj)[0];
        QueryCursor.field_1408 = ((unsigned char *)&QueryObj)[1];
    } else {
        QueryCursor.field_1404 = param_2->x;
        QueryCursor.field_1408 = param_2->y;
    }
    if ((int)QueryCursor.field_1404 < 0 || (int)QueryCursor.field_1404 >= (int)config->width ||
        (int)QueryCursor.field_1408 < 0 || (int)QueryCursor.field_1408 >= (int)config->height) {
        *(unsigned short *)&buf[3] = 0x40;
    } else {
        memcpy(buf, &map[QueryCursor.field_1408][QueryCursor.field_1404], 20);
    }
    if ((buf[3] & 0x40) != 0) {
        FUN_0045f480(&QueryCursor, 1);
        return;
    }
    FUN_0045f460(&QueryCursor);
}

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
LEGO_EXPORT unsigned int GetObjSalvageValue(unsigned int param_1, unsigned int param_2) {
    struct CostInfo *info;
    int cost;

    info = (struct CostInfo *)param_1;
    if (info->divisor == 0) {
        return GetObjCost(info);
    }
    cost = GetObjCost(info);
    return (cost * (int)param_2) / (int)info->divisor;
}

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
LEGO_EXPORT void ClearBlokeCounters(unsigned int index) {
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
LEGO_EXPORT int LoadObjectLibrary(void *object, const char *name) {
    struct CallbackTable *obj;
    struct LibraryNode *node;
    struct LibraryNode *staging;
    int results[14];
    char fname[256];
    char path[260];

    obj = (struct CallbackTable *)object;
    _splitpath(name, 0, 0, fname, 0);
    NewObjectPtr = &DAT_007fd610;
    // STRING: LEGOLAND 0x004bcea4
    sprintf(path, ".\\dlls\\%s.dll", fname);
    ((struct LibraryNode *)NewObjectPtr)->module = LoadLibraryExA(path, 0, 0);
    if (((struct LibraryNode *)NewObjectPtr)->module == 0) {
        return 0;
    }
    node = DAT_00669244;
    while (node != 0) {
        if (node->module == ((struct LibraryNode *)NewObjectPtr)->module) {
            NewObjectPtr = node;
            node->refcount++;
            obj->library = node;
            goto loaded;
        }
        node = node->next;
    }
    ((struct LibraryNode *)NewObjectPtr)->next = DAT_00669244;
    ((struct LibraryNode *)NewObjectPtr)->refcount = 1;
    staging = (struct LibraryNode *)malloc(sizeof(struct LibraryNode));
    node = (struct LibraryNode *)NewObjectPtr;
    DAT_00669244 = staging;
    staging->next = node->next;
    staging->module = node->module;
    staging->refcount = node->refcount;
    staging->init = node->init;
    obj->library = DAT_00669244;
    node = DAT_00669244;
loaded:
    memset(results, 0, sizeof(results));
    node->init(obj->context, results);
    if (results[0] != 0) {
        obj->cb_8c = (RideCallback)results[0];
    }
    if (results[1] != 0) {
        obj->cb_90 = (RideCallback)results[1];
    }
    if (results[2] != 0) {
        obj->cb_94 = (RideCallback)results[2];
    }
    if (results[3] != 0) {
        obj->cb_98 = (RideCallback)results[3];
    }
    if (results[4] != 0) {
        obj->cb_9c = (RideCallback)results[4];
    }
    if (results[5] != 0) {
        obj->cb_a0 = (RidePtrCallback)results[5];
    }
    if (results[6] != 0) {
        obj->cb_a8 = (RideCallback)results[6];
    }
    if (results[8] != 0) {
        obj->cb_ac = (RideCallback)results[8];
    }
    if (results[13] != 0) {
        obj->cb_c0 = (RideIntCallback)results[13];
    }
    obj->cb_b4 = (RideCallback)results[10];
    obj->cb_b8 = (RideIntCallback)results[11];
    obj->cb_bc = (RideIntCallback)results[12];
    if (results[9] != 0) {
        obj->cb_b0 = (RideCallback)results[9];
    }
    if (results[7] != 0) {
        ((void (*)(void *))results[7])(obj->context);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004810f0
LEGO_EXPORT void UnLoadObjectLibrary(void *object) {
    struct CallbackTable *obj;
    struct LibraryNode *node;
    struct LibraryNode *prev;

    obj = (struct CallbackTable *)object;
    if (obj->library->refcount == 0) {
        return;
    }
    FreeLibrary(obj->library->module);
    obj->library->refcount--;
    node = obj->library;
    if (node->refcount == 0) {
        prev = DAT_00669244;
        if (DAT_00669244 == node) {
            DAT_00669244 = DAT_00669244->next;
            free(prev);
            return;
        }
        if (DAT_00669244 != 0) {
            while (prev != 0) {
                if (prev->next == node) {
                    break;
                }
                prev = prev->next;
            }
            if (prev != 0) {
                prev->next = prev->next->next;
                free(obj->library);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00481170
void FUN_00481170(void) {
    struct LegoConfig *config;
    struct MapElement *cell;
    void *next;
    int x;
    int y;
    int offset;
    unsigned int width;

    while (DAT_00669248 != 0) {
        next = *(void **)DAT_00669248;
        free(DAT_00669248);
        DAT_00669248 = next;
    }
    config = lpConfig;
    y = 0;
    DAT_00669248 = 0;
    if (config->height != 0) {
        do {
            x = 0;
            width = config->width;
            if (width != 0) {
                offset = 0;
                do {
                    if (offset < 0 || x >= (int)width || y < 0 || y >= (int)lpConfig->height) {
                        cell = 0;
                    } else {
                        cell = (struct MapElement *)((char *)GameMap[y] + offset);
                    }
                    cell->flags &= 0xfbff;
                    x++;
                    width = lpConfig->width;
                    offset += 0x14;
                } while (x < (int)width);
            }
            config = lpConfig;
            y++;
        } while (y < (int)config->height);
    }
}

// FUNCTION: LEGOLAND 0x00481200
LEGO_EXPORT void BuildObjInfoList(void) {
    struct LegoConfig *config;
    struct MapElement *cell;
    struct MapElement *origin;
    struct ObjClassInfo *cls;
    struct InfoNode *node;
    struct FootprintNode *fp;
    struct ClassObjNode *obj;
    unsigned char ox;
    unsigned char oy;
    short coords;
    unsigned int x;
    unsigned int y;
    unsigned int ux;
    unsigned int uy;
    unsigned int width;
    int offset;
    int best;
    int rng;

    FUN_00481170();
    y = 0;
    config = lpConfig;
    if (config->height == 0) {
        return;
    }
    do {
        x = 0;
        width = config->width;
        if (width != 0) {
            offset = 0;
            do {
                if ((int)x < 0 || (int)x >= (int)width || (int)y < 0 || (int)y >= (int)config->height) {
                    cell = 0;
                } else {
                    cell = (struct MapElement *)((char *)GameMap[y] + offset);
                }
                if ((cell->flags & 0x80) != 0) {
                    ox = cell->field_4;
                    oy = cell->field_5;
                    coords = *(short *)&cell->field_4;
                    uy = oy;
                    ux = ox;
                    if (ux < width && uy < config->height) {
                        origin = &GameMap[uy][ux];
                    } else {
                        origin = 0;
                    }
                    cls = (struct ObjClassInfo *)*(int *)(origin->field_0 + 0xc);
                    if (cls->type != 0 && cls->type != 2 && (*((unsigned char *)origin + 0xd) & 4) == 0) {
                        node = DAT_00669248;
                        while (node != 0) {
                            if (node->classid == (int)cls) {
                                rng = rand();
                                rng = rng & 0x800000ff;
                                if (rng < 0) {
                                    rng = ((rng - 1) | 0xffffff00) + 1;
                                }
                                if (rng < 0x50) {
                                    node->x = cls->field_c + ux;
                                    node->y = cls->field_10 + uy;
                                    *(unsigned char *)&node->origin_x = cls->field_24 + ox;
                                    node->origin_y = cls->field_25 + oy;
                                }
                                goto marked;
                            }
                            node = node->next;
                        }
                        node = (struct InfoNode *)malloc(sizeof(struct InfoNode));
                        node->next = DAT_00669248;
                        DAT_00669248 = node;
                        node->classid = (int)cls;
                        node->coords = coords;
                        node->x = cls->field_c + ux;
                        node->y = cls->field_10 + uy;
                        node->origin_x = cls->field_24 + ox;
                        node->origin_y = cls->field_25 + oy;
                    marked:
                        *((unsigned char *)origin + 0xd) |= 4;
                    }
                    fp = (struct FootprintNode *)&cls->footprint_base;
                    while ((int)x < (int)(fp->x_min + ux) || (int)(fp->x_max + ux) < (int)x) {
                        fp = fp->next;
                    }
                    best = cls->field_48;
                    x = fp->x_max + ux;
                    for (obj = cls->objlist; obj != 0; obj = obj->next) {
                        if (best < obj->field_c) {
                            best = obj->field_c;
                        }
                    }
                    config = lpConfig;
                    if ((int)(y - uy) == best) {
                        *(unsigned short *)((char *)origin + 0xc) &= 0xfbff;
                        config = lpConfig;
                    }
                }
                x++;
                width = config->width;
            } while ((int)x < (int)width);
        }
        y++;
    } while ((int)y < (int)config->height);
}

// FUNCTION: LEGOLAND 0x00481410
LEGO_EXPORT int CalculateViewRideCode(int param_1, struct ObjectClass *param_2, int param_3) {
    int value;

    value = param_2->field_38;
    if (value > param_1) {
        value = (value - param_1) * 3;
    } else {
        value = (param_1 - value) * 3;
    }
    value -= 10;
    if (value < 0) {
        value = 0;
    } else if (value > 100) {
        value = 100;
    }
    if (param_3 != 0) {
        return ((4 - (1 << param_3)) * 0x19 + param_2->field_36) - value;
    }
    return (param_2->field_36 - value) + 100;
}

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
LEGO_EXPORT unsigned int Calc_Item_Attractiveness(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    struct ObjectClass *item;
    struct Bloke *bloke;
    unsigned int fatigue;
    unsigned char category;
    int rating;
    int blokenum;
    int counter;

    item = (struct ObjectClass *)param_1;
    bloke = (struct Bloke *)param_2;
    fatigue = bloke->field_7e;
    category = FUN_0044eb10(bloke);
    rating = item->field_38;
    blokenum = GetBlokeNum(bloke);
    counter = GetBlokeCounter(item, blokenum);
    if ((int)fatigue < rating) {
        rating -= fatigue;
    } else {
        rating = (fatigue - rating) * 2;
    }
    rating -= 10;
    if (rating < 0) {
        rating = 0;
    }
    if (counter == 0) {
        rating = (item->field_36 - rating) + 100;
    } else {
        rating = ((4 - (1 << (counter & 0x1f))) * 0x19 + item->field_36) - rating;
    }
    switch (category) {
    case 0:
        if (item->flags == 5) {
            return -100;
        }
        break;
    case 1:
        if (item->flags == 5 && param_3 == 0) {
            return -100;
        }
        break;
    case 2:
        if (item->flags == 5) {
            if (rating < 0x14) {
                rating = 0x14;
            }
            if (param_3 != 0) {
                return rating * 2;
            }
        }
        break;
    case 3:
    case 4:
        if (item->flags == 5) {
            if (rating < 0x14) {
                rating = 0x32;
            }
            return rating * ((-(int)(param_3 != 0) & 2) + 4);
        }
        rating = 0;
    }
    return rating;
}

// FUNCTION: LEGOLAND 0x004815e0
LEGO_EXPORT void CalculateRideCodes(unsigned int param_1) {
    struct RideNode *node;

    for (node = DAT_00669248; node != 0; node = node->next) {
        node->field_14 = Calc_Item_Attractiveness(node->field_4, param_1, 0);
    }
}

// FUNCTION: LEGOLAND 0x00481610
LEGO_EXPORT int ShuffleObjKeys(int *param_1, void **param_2) {
    struct InfoNode **slot;
    struct InfoNode *node;
    struct InfoNode *next;

    slot = (struct InfoNode **)&DAT_00669248;
    node = DAT_00669248;
    if (DAT_00669248 != (void *)DAT_0066924c) {
        while (next = node, node != 0) {
            node = next->next;
            if ((unsigned int)node == DAT_0066924c) {
                DAT_0066924c = (unsigned int)next;
                param_1[0] = next->x << 8;
                param_1[1] = next->y << 8;
                *param_2 = (void *)next->classid;
                return 1;
            }
            if ((int)node->sort_key < (int)next->sort_key) {
                *slot = node;
                next->next = node->next;
                node->next = next;
                slot = (struct InfoNode **)node;
                node = next;
            } else {
                slot = (struct InfoNode **)next;
            }
        }
    }
    return 0;
}

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
struct BestNode *FUN_004817d0(int *param_1) {
    struct BestNode *node;

    if (DAT_0066b44c == 0) {
        return 0;
    }
    node = DAT_0066b44c;
    do {
        if (param_1[0] >> 8 >= (int)node->x_min && param_1[0] >> 8 <= (int)node->x_max &&
            param_1[1] >> 8 >= (int)node->y_min && param_1[1] >> 8 <= (int)node->y_max) {
            return node;
        }
        node = node->next;
    } while (node != 0);
    return 0;
}

// FUNCTION: LEGOLAND 0x00481810
void FUN_00481810(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004819a0
void FUN_004819a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00481b10
void FUN_00481b10(struct BestNode *node) { STUB(); }
