#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "controller.h"
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "bloke_ai.h"
#include "gamemap.h"
#include "llidb.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "objclass.h"

struct ClassRideNode {
    struct ClassRideNode *next;
    void *ride;
    unsigned int field_8;
    unsigned char pad_c[0x14 - 0xc];
    int score;
    unsigned char pad_18[0x20 - 0x18];
};

struct RideStats {
    /* 0x00 */ unsigned char pad_0[0x34];
    /* 0x34 */ short base_code;
    /* 0x36 */ unsigned char pad_36[2];
    /* 0x38 */ short rating;
};

struct ClassObjNode {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ int field_c;
    /* 0x10 */ struct ClassObjNode *next;
};

struct ObjClassInfo {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ int x;
    /* 0x10 */ int y;
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
    /* 0x04 */ void *classid; /* the object's class (Ride) */
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
LEGO_EXPORT struct Ride *AddNewObjectClass(void) {
    struct Ride *cls;

    cls = malloc(sizeof(struct Ride));
    memset(cls, 0, sizeof(struct Ride));
    if (cls == 0) {
        return cls;
    }
    cls->next = ObjectClassList;
    ObjectClassList = cls;
    cls->instances = NULL;
    cls->field_8 = 0;
    cls->flags = 0;
    return cls;
}

struct ObjClassAlias {
    const char *name;
    const char *aliases;
};

/* Not const: the original keeps this table in .data, directly followed by its alias strings, so the loop's
 * end pointer (&ObjClassAliasTable[5].aliases) lands inside the first of them. */
// GLOBAL: LEGOLAND 0x004bcdcc
static struct ObjClassAlias ObjClassAliasTable[] = {
    {"CASTLE OBJ",
        // STRING: LEGOLAND 0x004bce60
        "CASTLE_DUMMY;ROLLER COASTER TRACK;SQUARE_TRACK;"},
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

/* Load the semicolon-terminated dependencies of a matching object class. Each name is
   temporarily terminated for ElemID; successful loads get flag 4 so they are not loaded
   again. Like the original, this only processes names followed by a semicolon. */
// FUNCTION: LEGOLAND 0x004809d0
void LoadObjectClassAliasElements(struct Element *cls) {
    const char **slot;
    char *token;
    char *semicolon;
    struct Element *elem;
    char buffer[256];

    slot = &ObjClassAliasTable[0].aliases;
    do {
        if (_strcmpi(cls->name, slot[-1]) == 0) {
            /* Initialize before the copy and restore each delimiter before advancing:
               MSVC6 then keeps the token and resolved element in separate registers. */
            token = buffer;
            strcpy(buffer, slot[0]);
            semicolon = strchr(buffer, ';');
            while (semicolon != 0) {
                *semicolon = '\0';
                elem = (struct Element *)ElemID(token);
                if (elem != 0 && (elem->flags & 4) == 0 && LLIDB_LoadData(elem) != 0) {
                    elem->flags |= 4;
                }
                *semicolon = ';';
                token = semicolon + 1;
                semicolon = strchr(token, ';');
            }
        }
        slot += 2;
    } while (slot < &ObjClassAliasTable[5].aliases);
}

/* Hard-coded class data fixes applied while loading object data: entrance offsets (and 10 seats for the water
 * block) for the Water Works Shower, Water Block and Elephant Fountain. */
// FUNCTION: LEGOLAND 0x00480aa0
void ApplyWaterWorksClassOverrides(struct Element *names, struct Ride *info) {
    // STRING: LEGOLAND 0x004bce90
    if (_stricmp(names->name, "Water Works Shower") == 0) {
        info->x = 1;
        info->y = 0;
        info->field_24 = 1;
        info->field_25 = 0;
        return;
    }
    // STRING: LEGOLAND 0x004b9828
    if (_stricmp(names->name, "Water Works Water Block") == 0) {
        info->x = 1;
        info->y = 0;
        info->field_24 = 1;
        info->field_25 = 0;
        info->seats = 10;
        return;
    }
    // STRING: LEGOLAND 0x004b9840
    if (_stricmp(names->name, "Water Works Elephant Fountain") == 0) {
        info->x = 6;
        info->y = 2;
        info->field_24 = 6;
        info->field_25 = 2;
    }
}

// FUNCTION: LEGOLAND 0x00480b40
LEGO_EXPORT void *LoadObjectClass(struct Element *cls) {
    void *result;

    result = LLIDB_LoadData(cls);
    if (result != 0) {
        cls->flags |= 0x4;
    }
    LoadObjectClassAliasElements(cls);
    return result;
}

// FUNCTION: LEGOLAND 0x00480b70
void SetObjectEditCursor(struct Element *param) {
    void *value;

    value = param->ride;
    EditMode.unk0 = 1;
    EditMode.unk8 = value;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
}

// FUNCTION: LEGOLAND 0x00480bb0
LEGO_EXPORT void BasicObjectDCalcCursor(struct Element *param_1, struct Point *param_2) {
    struct MapElement **map;
    struct LegoConfig *config;
    int x;
    int y;
    struct MapElement buf; /* a copy of the tile */

    map = GameMap;
    config = lpConfig;
    x = param_2->x;
    if (x < 0 || x >= (int)config->width || (y = param_2->y) < 0 || y >= (int)config->height) {
        buf.flags = 0x40;
    } else {
        memcpy(&buf, &GameMap[y][x], sizeof(buf));
    }
    memcpy(QueryCursor.field_1414, &QueryClass->footprint, sizeof(QueryClass->footprint));
    QueryCursor.field_1828 = 8;
    if ((*(unsigned int *)&buf.flags & 0x8a0) != 0) {
        QueryCursor.field_1828 = 9;
        QueryCursor.tile_x = QueryObj.pos.x;
        QueryCursor.tile_y = QueryObj.pos.y;
    } else {
        QueryCursor.tile_x = param_2->x;
        QueryCursor.tile_y = param_2->y;
    }
    if ((int)QueryCursor.tile_x < 0 || (int)QueryCursor.tile_x >= (int)config->width ||
        (int)QueryCursor.tile_y < 0 || (int)QueryCursor.tile_y >= (int)config->height) {
        buf.flags = 0x40;
    } else {
        memcpy(&buf, &map[QueryCursor.tile_y][QueryCursor.tile_x], sizeof(buf));
    }
    if ((*(unsigned int *)&buf.flags & 0x40) != 0) {
        SetCursorBlocked(&QueryCursor, 1);
        return;
    }
    ResetCursorBuildable(&QueryCursor);
}

// FUNCTION: LEGOLAND 0x00480cd0
LEGO_EXPORT void SetStandardCallbacks(struct Ride *table) {
    table->cb_8c = SetObjectEditCursor;
    table->cb_90 = CalcBasicObjectCursor;
    table->cb_94 = BasicObjectDCalcCursor;
    table->cb_98 = (void (*)())AddBasicObject;
    table->cb_9c = StandardRemoveObject;
}

// FUNCTION: LEGOLAND 0x00480d10
LEGO_EXPORT void ClearObjectCounters(void) {
    struct Ride *cls;

    for (cls = ObjectClassList; cls != 0; cls = cls->next) {
        cls->field_8 = 0;
    }
}

// FUNCTION: LEGOLAND 0x00480d30
LEGO_EXPORT unsigned int ObjCount(struct Element *wrap) {
    if (wrap == 0) {
        return 0;
    }
    return wrap->ride->field_8;
}

// FUNCTION: LEGOLAND 0x00480d40
LEGO_EXPORT void IncrementObjectCount(struct Ride *count) {
    count->field_8++;
    count->theme_element->field_10++;
}

// FUNCTION: LEGOLAND 0x00480d60
LEGO_EXPORT void DecrementObjectCount(struct Ride *count) {
    count->field_8--;
    count->theme_element->field_10--;
}

// FUNCTION: LEGOLAND 0x00480d80
LEGO_EXPORT void CreateObjectClasses(void) {
    NEWFLC_Repeat = 0;
    NEWFLC_BuffSize = 1;
    NEWFLC_CheckDuplicate = 1;
}

// FUNCTION: LEGOLAND 0x00480da0
LEGO_EXPORT int GetObjCost(Ride *ride) {
    return ride->cost;
}

// FUNCTION: LEGOLAND 0x00480db0
LEGO_EXPORT unsigned int GetObjSalvageValue(Ride *ride, unsigned int level) {
    int cost;

    if (ride->durability == 0) {
        return GetObjCost(ride);
    }
    cost = GetObjCost(ride);
    return (cost * (int)level) / (int)ride->durability;
}

// FUNCTION: LEGOLAND 0x00480de0
LEGO_EXPORT int GetObjRepairCost(Ride *ride, unsigned int level) {
    unsigned int salvage;
    unsigned int cost;

    salvage = GetObjSalvageValue(ride, level);
    cost = GetObjCost(ride);
    return cost - salvage;
}

// FUNCTION: LEGOLAND 0x00480e10
LEGO_EXPORT void AllocBlokeCounters(unsigned int size) {
    struct Ride *cls;

    for (cls = ObjectClassList; cls != 0; cls = cls->next) {
        if (cls->type != 0 && cls->type != 2) {
            cls->counters = malloc(size);
        } else {
            cls->counters = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00480e60
LEGO_EXPORT void FreeBlokeCounters(void) {
    struct Ride *cls;

    for (cls = ObjectClassList; cls != 0; cls = cls->next) {
        if (cls->counters != 0) {
            free(cls->counters);
            cls->counters = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00480e90
LEGO_EXPORT void ClearBlokeCounters(unsigned int index) {
    struct Ride *cls;

    for (cls = ObjectClassList; cls != 0; cls = cls->next) {
        if (cls->counters != 0) {
            cls->counters[index] = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x00480ec0
LEGO_EXPORT void IncrementBlokeCounter(struct Ride *cls, unsigned int index) {
    unsigned char *counters;

    counters = cls->counters;
    if (counters != 0) {
        counters[index]++;
    }
}

// FUNCTION: LEGOLAND 0x00480ee0
LEGO_EXPORT int GetBlokeCounter(struct Ride *cls, int index) {
    unsigned char *counters;

    counters = cls->counters;
    if (counters != 0) {
        return counters[index];
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00480f00
LEGO_EXPORT int LoadObjectLibrary(void *object, const char *name) {
    struct Ride *obj;
    struct LibraryNode *node;
    struct LibraryNode *staging;
    void *results[14];
    char fname[256];
    char path[260];

    obj = (struct Ride *)object;
    _splitpath(name, 0, 0, fname, 0);
    NewObjectPtr = &NewObjectLibraryNode;
    // STRING: LEGOLAND 0x004bcea4
    sprintf(path, ".\\dlls\\%s.dll", fname);
    ((struct LibraryNode *)NewObjectPtr)->module = LoadLibraryExA(path, 0, 0);
    if (((struct LibraryNode *)NewObjectPtr)->module == 0) {
        return 0;
    }
    node = ObjectLibraryHead;
    while (node != 0) {
        if (node->module == ((struct LibraryNode *)NewObjectPtr)->module) {
            NewObjectPtr = node;
            node->refcount++;
            obj->library = node;
            goto loaded;
        }
        node = node->next;
    }
    ((struct LibraryNode *)NewObjectPtr)->next = ObjectLibraryHead;
    ((struct LibraryNode *)NewObjectPtr)->refcount = 1;
    staging = (struct LibraryNode *)malloc(sizeof(struct LibraryNode));
    node = (struct LibraryNode *)NewObjectPtr;
    ObjectLibraryHead = staging;
    staging->next = node->next;
    staging->module = node->module;
    staging->refcount = node->refcount;
    staging->init = node->init;
    obj->library = ObjectLibraryHead;
    node = ObjectLibraryHead;
loaded:
    memset(results, 0, sizeof(results));
    node->init(obj->element, results);
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
        obj->cb_sprite = (RidePtrCallback)results[5];
    }
    if (results[6] != 0) {
        obj->cb_pre_render = (RideCallback)results[6];
    }
    if (results[8] != 0) {
        obj->cb_ac = (RideCallback)results[8];
    }
    if (results[13] != 0) {
        obj->cb_c0 = (RideIntCallback)results[13];
    }
    obj->cb_b4 = (RideCallback)results[10];
    obj->load_hook = (RideIntCallback)results[11];
    obj->save_hook = (RideIntCallback)results[12];
    if (results[9] != 0) {
        obj->cb_b0 = (RideCallback)results[9];
    }
    if (results[7] != 0) {
        ((void (*)(void *))results[7])(obj->element);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004810f0
LEGO_EXPORT void UnLoadObjectLibrary(void *object) {
    struct Ride *obj;
    struct LibraryNode *node;
    struct LibraryNode *prev;

    obj = (struct Ride *)object;
    if (obj->library->refcount == 0) {
        return;
    }
    FreeLibrary(obj->library->module);
    obj->library->refcount--;
    node = obj->library;
    if (node->refcount == 0) {
        prev = ObjectLibraryHead;
        if (ObjectLibraryHead == node) {
            ObjectLibraryHead = ObjectLibraryHead->next;
            free(prev);
            return;
        }
        if (ObjectLibraryHead != 0) {
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

/* Frees every ClassRideList node and clears tile flag 0x400 across the whole map. */
// FUNCTION: LEGOLAND 0x00481170
void ResetClassRideList(void) {
    struct MapElement *cell;
    void *next;
    int x;
    int y;

    while (ClassRideList != 0) {
        next = *(void **)ClassRideList;
        free(ClassRideList);
        ClassRideList = next;
    }
    for (y = 0; y < lpConfig->height; y++) {
        for (x = 0; x < lpConfig->width; x++) {
            if (x < 0 || x >= lpConfig->width || y < 0 || y >= lpConfig->height) {
                cell = 0;
            } else {
                cell = &GameMap[y][x];
            }
            cell->flags &= 0xfbff;
        }
    }
}

// FUNCTION: LEGOLAND 0x00481200
LEGO_EXPORT void BuildObjInfoList(void) {
    struct MapElement *cell;
    struct MapElement *origin;
    struct ObjClassInfo *cls;
    struct InfoNode *node;
    struct Footprint *fp;
    struct ClassObjNode *obj;
    TileId tile;
    struct Point pos;
    int best;
    struct Point at;

    ResetClassRideList();
    for (pos.y = 0; pos.y < lpConfig->height; pos.y++) {
        for (pos.x = 0; pos.x < lpConfig->width; pos.x++) {
            if (pos.x >= 0 && pos.x < lpConfig->width && pos.y >= 0 && pos.y < lpConfig->height) {
                cell = &GameMap[pos.y][pos.x];
            } else {
                cell = NULL;
            }
            if ((cell->flags & 0x80) == 0) {
                continue;
            }
            tile.pos.x = cell->field_4;
            at.x = tile.pos.x;
            tile.pos.y = cell->field_5;
            at.y = tile.pos.y;
            if (at.x >= 0 && at.x < lpConfig->width && at.y >= 0 && at.y < lpConfig->height) {
                origin = &GameMap[at.y][at.x];
            } else {
                origin = NULL;
            }
            cls = origin->field_0->data;
            if (cls->type != 0 && cls->type != 2 && (origin->flags & 0x400) == 0) {
                for (node = ClassRideList; node != NULL; node = node->next) {
                    if (node->classid == cls) {
                        break;
                    }
                }
                if (node != NULL) {
                    if (rand() % 256 < 0x50) {
                        node->x = cls->x + at.x;
                        node->y = cls->y + at.y;
                        node->origin_x = cls->field_24 + at.x;
                        node->origin_y = cls->field_25 + at.y;
                    }
                } else {
                    node = (struct InfoNode *)malloc(sizeof(struct InfoNode));
                    node->next = ClassRideList;
                    ClassRideList = node;
                    node->classid = cls;
                    node->coords = tile.id;
                    node->x = cls->x + at.x;
                    node->y = cls->y + at.y;
                    node->origin_x = cls->field_24 + at.x;
                    node->origin_y = cls->field_25 + at.y;
                }
                origin->flags |= 0x400;
            }
            fp = (struct Footprint *)&cls->footprint_base;
            while (pos.x < fp->x0 + at.x || pos.x > fp->x1 + at.x) {
                fp = fp->next;
            }
            pos.x = fp->x1 + at.x;
            best = cls->field_48;
            for (obj = cls->objlist; obj != NULL; obj = obj->next) {
                if (obj->field_c > best) {
                    best = obj->field_c;
                }
            }
            if (pos.y - at.y == best) {
                origin->flags &= 0xfbff;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00481410
LEGO_EXPORT int CalculateViewRideCode(int param_1, struct Ride *param_2, int param_3) {
    int value;

    value = param_2->intensity;
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
        return ((4 - (1 << param_3)) * 0x19 + param_2->value) - value;
    }
    return (param_2->value - value) + 100;
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
LEGO_EXPORT int Calc_Item_Attractiveness(struct Ride *item, struct Bloke *bloke, int param_3) {
    volatile int category;
    int rating;
    int fatigue;
    int blokenum;
    int counter;

    fatigue = bloke->field_7e;
    category = GetBlokeHungerLevel(bloke);
    rating = item->intensity;
    blokenum = GetBlokeNum(bloke);
    counter = GetBlokeCounter(item, blokenum);
    if (rating > fatigue) {
        fatigue = rating - fatigue;
    } else {
        fatigue = (fatigue - rating) * 2;
    }
    fatigue -= 10;
    if (fatigue < 0) {
        fatigue = 0;
    }
    if (counter != 0) {
        rating = ((4 - (1 << counter)) * 0x19 + item->value) - fatigue;
    } else {
        rating = (item->value - fatigue) + 100;
    }
    counter = rating;
    switch (category) {
    case 0:
        if (item->type == 5) {
            return -100;
        }
        break;
    case 1:
        if (item->type == 5 && !param_3) {
            return -100;
        }
        break;
    case 2:
        if (item->type == 5) {
            if (counter < 0x14) {
                counter = 0x14;
            }
            if (param_3 != 0) {
                return 2 * counter;
            }
        }
        break;
    case 3:
    case 4:
        if (item->type == 5) {
            if (0x14 > counter) {
                counter = 0x32;
            }
            return counter * (param_3 != 0 ? 6 : 4);
        }
        counter = 0;
    }
    return counter;
}

// FUNCTION: LEGOLAND 0x004815e0
LEGO_EXPORT void CalculateRideCodes(struct Bloke *bloke) {
    struct ClassRideNode *node;

    for (node = ClassRideList; node != 0; node = node->next) {
        node->score = Calc_Item_Attractiveness(node->ride, bloke, 0);
    }
}

// FUNCTION: LEGOLAND 0x00481610
LEGO_EXPORT int ShuffleObjKeys(struct Point *goal, struct Ride **ride) {
    struct InfoNode **slot;
    struct InfoNode *node;
    struct InfoNode *next;

    slot = (struct InfoNode **)&ClassRideList;
    node = ClassRideList;
    if (ClassRideList != ClassRideBestPtr) {
        while (node != 0) {
            next = node->next;
            if (next == ClassRideBestPtr) {
                ClassRideBestPtr = node;
                goal->x = node->x << 8;
                goal->y = node->y << 8;
                *ride = node->classid;
                return 1;
            }
            if ((int)node->sort_key > (int)next->sort_key) {
                *slot = next;
                node->next = next->next;
                next->next = node;
                slot = (struct InfoNode **)next;
            } else {
                slot = (struct InfoNode **)node;
                node = next;
            }
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00481690
LEGO_EXPORT void ResetBestPtr(void) {
    ClassRideBestPtr = 0;
}

// FUNCTION: LEGOLAND 0x004816a0
LEGO_EXPORT struct ObjInstance *CreateObjectInstance(struct Ride *param_1, unsigned short *param_2) {
    struct ObjInstance *obj;

    obj = malloc(sizeof(struct ObjInstance));
    if (obj != 0) {
        memset(obj, 0, sizeof(struct ObjInstance));
        obj->uid = *param_2;
        obj->ride = param_1;
    }
    return obj;
}

// FUNCTION: LEGOLAND 0x004816e0
LEGO_EXPORT void SetEditObject(struct Ride *obj) {
    EditMode.unk0 = 1;
    EditMode.unk8 = obj;
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
void *GetBestNodeList(void) {
    return BestNodeList;
}

// FUNCTION: LEGOLAND 0x00481730
struct BestNode *AddBestNode(void) {
    struct BestNode *node;

    node = (struct BestNode *)malloc(sizeof(struct BestNode));
    node->field_20 = 0;
    node->field_1c = 0;
    node->next = BestNodeList;
    BestNodeList = node;
    return node;
}

// FUNCTION: LEGOLAND 0x00481750
void RemoveBestNode(struct BestNode *node) {
    struct BestNode *current;

    if (BestNodeList == node) {
        BestNodeList = node->next;
        free(node);
    } else {
        for (current = BestNodeList; current->next != 0; current = current->next) {
            if (current->next == node) {
                current->next = node->next;
                free(node);
                return;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00481790
struct BestNode *FindBestNodeAtPoint(struct Point *pos) {
    struct BestNode *node;

    node = BestNodeList;
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
struct BestNode *FindBestNodeAtPixel(int *param_1) {
    struct BestNode *node;

    if (BestNodeList == 0) {
        return 0;
    }
    node = BestNodeList;
    do {
        if (param_1[0] >> 8 >= (int)node->x_min && param_1[0] >> 8 <= (int)node->x_max &&
            param_1[1] >> 8 >= (int)node->y_min && param_1[1] >> 8 <= (int)node->y_max) {
            return node;
        }
        node = node->next;
    } while (node != 0);
    return 0;
}

/* Collects the best-path nodes bordering rectangle param_1 {x0, y0, x1, y1} into AdjacentNodes, one NULL-terminated
 * run per side (above, below, left, right); AdjacentNodeCount counts the nodes. */
// FUNCTION: LEGOLAND 0x00481810
void CollectAdjacentNodesBySide(int *param_1) {
    struct BestNode *found;
    void **out;
    int count;
    struct Point pos;

    AdjacentNodeCount = 0;
    count = 0;
    pos.y = param_1[1] - 1;
    pos.x = param_1[0];
    if ((int)pos.x <= param_1[2]) {
        out = AdjacentNodes;
        do {
            found = FindBestNodeAtPoint(&pos);
            *out = found;
            if (found != 0) {
                pos.x = found->x_max;
                count++;
                out++;
                AdjacentNodeCount++;
            }
            pos.x++;
        } while ((int)pos.x <= param_1[2]);
    }
    AdjacentNodes[count] = 0;
    pos.x = param_1[0];
    count++;
    pos.y = param_1[3] + 1;
    if ((int)pos.x <= param_1[2]) {
        out = &AdjacentNodes[count];
        do {
            found = FindBestNodeAtPoint(&pos);
            *out = found;
            if (found != 0) {
                pos.x = found->x_max;
                count++;
                out++;
                AdjacentNodeCount++;
            }
            pos.x++;
        } while ((int)pos.x <= param_1[2]);
    }
    AdjacentNodes[count] = 0;
    pos.y = param_1[1];
    count++;
    pos.x = param_1[0] - 1;
    if ((int)pos.y <= param_1[3]) {
        out = &AdjacentNodes[count];
        do {
            found = FindBestNodeAtPoint(&pos);
            *out = found;
            if (found != 0) {
                pos.y = found->y_max;
                count++;
                out++;
                AdjacentNodeCount++;
            }
            pos.y++;
        } while ((int)pos.y <= param_1[3]);
    }
    AdjacentNodes[count] = 0;
    count++;
    pos.x = param_1[2] + 1;
    pos.y = param_1[1];
    if ((int)pos.y <= param_1[3]) {
        out = &AdjacentNodes[count];
        do {
            found = FindBestNodeAtPoint(&pos);
            *out = found;
            if (found != 0) {
                pos.y = found->y_max;
                count++;
                out++;
                AdjacentNodeCount++;
            }
            pos.y++;
        } while ((int)pos.y <= param_1[3]);
    }
    AdjacentNodes[count] = 0;
}

// FUNCTION: LEGOLAND 0x004819a0
void CollectAdjacentNodes(int *param_1) {
    struct BestNode *found;
    void **out;
    int count;
    struct Point pos;

    count = 0;
    AdjacentNodeCount = 0;
    pos.y = param_1[1] - 1;
    pos.x = param_1[0];
    if ((int)pos.x <= param_1[2]) {
        out = AdjacentNodes;
        do {
            found = FindBestNodeAtPoint(&pos);
            *out = found;
            if (found != 0) {
                pos.x = found->x_max;
                count++;
                out++;
                AdjacentNodeCount++;
            }
            pos.x++;
        } while ((int)pos.x <= param_1[2]);
    }
    pos.x = param_1[0];
    pos.y = param_1[3] + 1;
    if ((int)pos.x <= param_1[2]) {
        out = &AdjacentNodes[count];
        do {
            found = FindBestNodeAtPoint(&pos);
            *out = found;
            if (found != 0) {
                pos.x = found->x_max;
                count++;
                out++;
                AdjacentNodeCount++;
            }
            pos.x++;
        } while ((int)pos.x <= param_1[2]);
    }
    pos.y = param_1[1];
    pos.x = param_1[0] - 1;
    if ((int)pos.y <= param_1[3]) {
        out = &AdjacentNodes[count];
        do {
            found = FindBestNodeAtPoint(&pos);
            *out = found;
            if (found != 0) {
                pos.y = found->y_max;
                count++;
                out++;
                AdjacentNodeCount++;
            }
            pos.y++;
        } while ((int)pos.y <= param_1[3]);
    }
    pos.x = param_1[2] + 1;
    pos.y = param_1[1];
    if ((int)pos.y <= param_1[3]) {
        out = &AdjacentNodes[count];
        do {
            found = FindBestNodeAtPoint(&pos);
            *out = found;
            if (found != 0) {
                pos.y = found->y_max;
                out++;
                AdjacentNodeCount++;
            }
            pos.y++;
        } while ((int)pos.y <= param_1[3]);
    }
}

// FUNCTION: LEGOLAND 0x00481b10
void MergeBestNode(struct BestNode *node) {
    struct BestNode *cur;
    int i;

    for (;;) {
        CollectAdjacentNodesBySide((int *)&node->x_min);
        i = 0;
        for (cur = AdjacentNodes[i]; cur != 0; cur = AdjacentNodes[++i]) {
            if (cur->x_min == node->x_min && ((struct BestNode *)AdjacentNodes[i])->x_max == node->x_max) {
                ((struct BestNode *)AdjacentNodes[i])->y_max = node->y_max;
                goto merged;
            }
        }
        i++;
        for (cur = AdjacentNodes[i]; cur != 0; cur = AdjacentNodes[++i]) {
            if (cur->x_min == node->x_min && ((struct BestNode *)AdjacentNodes[i])->x_max == node->x_max) {
                ((struct BestNode *)AdjacentNodes[i])->y_min = node->y_min;
                goto merged;
            }
        }
        i++;
        for (cur = AdjacentNodes[i]; cur != 0; cur = AdjacentNodes[++i]) {
            if (cur->y_min == node->y_min && ((struct BestNode *)AdjacentNodes[i])->y_max == node->y_max) {
                ((struct BestNode *)AdjacentNodes[i])->x_max = node->x_max;
                goto merged;
            }
        }
        i++;
        cur = AdjacentNodes[i];
        if (cur == 0) {
            return;
        }
        while (cur->y_min != node->y_min || ((struct BestNode *)AdjacentNodes[i])->y_max != node->y_max) {
            cur = AdjacentNodes[++i];
            if (cur == 0) {
                return;
            }
        }
        ((struct BestNode *)AdjacentNodes[i])->x_min = node->x_min;
    merged:
        RemoveBestNode(node);
        node = AdjacentNodes[i];
    }
}
