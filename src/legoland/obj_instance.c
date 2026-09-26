#include "obj_instance.h"
#include <stdlib.h>
#include "bloke.h"
#include "bloke_ai.h"
#include "globals.h"
#include "legoland.h"
#include "llidb.h"
#include "man3d.h"
#include "objclass.h"
#include "resource.h"
#include "sound_music.h"

struct InstanceNode {
    struct InstanceNode *prev;
    struct InstanceNode *next;
    struct InstanceList *list;
};

struct InstanceList {
    unsigned int pad_0;
    struct InstanceNode *next;
};

// FUNCTION: LEGOLAND 0x00489e60
char *FUN_00489e60(struct ResFile *file, char *dest, int maxlen) {
    int error;
    int count;
    char c;
    char *out;

    error = 0;
    count = 0;
    out = dest;
    for (;;) {
        if (RES_ReadFile(file, &c, 1) == 0) {
            error = 1;
            break;
        }
        if (c == '\r' || c == '\n') {
            break;
        }
        *out++ = c;
        count++;
        if (count >= maxlen) {
            break;
        }
    }
    if (c == '\r') {
        RES_ReadFile(file, &c, 1);
    }
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
int FUN_00489f00(const struct Point *pos) {
    int index;
    struct ObjTableEntry *entry;

    index = 0;
    for (entry = DAT_007cb3e0; (int)entry < (int)&DAT_007cb5e0; entry++) {
        if (entry->key == 0xffff) {
            DAT_007cb3e0[index].key = (pos->x << 8) + pos->y;
            DAT_007cb3e2[index].key = 0;
            return 1;
        }
        index++;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00489f50
int FUN_00489f50(const struct Point *pos) {
    int index;
    unsigned short target;
    struct ObjTableEntry *entry;

    index = 0;
    target = (pos->x << 8) + pos->y;
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
int FUN_00489f90(const struct Point *pos) {
    int index;
    unsigned short target;
    struct ObjTableEntry *entry;

    index = 0;
    target = (pos->x << 8) + pos->y;
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
unsigned short FUN_00489fd0(const struct Point *pos) {
    int index;
    unsigned short target;
    struct ObjTableEntry *entry;

    index = 0;
    target = (pos->x << 8) + pos->y;
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
    struct Ride *node;

    for (node = ObjectClassList; node != 0; node = node->next) {
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
LEGO_EXPORT struct ObjInstance *GetInstanceOfClass(struct Ride *ride, const TileId *tile) {
    struct ObjInstance *instance;

    instance = ride->instances;
    if (instance == 0) {
        return 0;
    }
    while (instance->uid != tile->id) {
        instance = instance->next;
        if (instance == 0) {
            return 0;
        }
    }
    return instance;
}

// FUNCTION: LEGOLAND 0x0048a0f0
LEGO_EXPORT void HandleRideAI(int arg_1, int arg_2, int arg_3) {}

// FUNCTION: LEGOLAND 0x0048a100
LEGO_EXPORT void RemoveBlokeFromRide(struct Ride *ride, struct RideNode *node) {
    struct Bloke *bloke;
    struct MapElement *element;
    int counter;
    int code;

    bloke = node->rider;
    RemoveBlokeFromList(ride, node);
    if (FUN_0044f3d0(ride, &node->tile) == 0) {
        int x = node->tile.pos.x;
        int y = node->tile.pos.y;
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            element = GameMap[y] + x;
        } else {
            element = 0;
        }
        element->flags &= 0xfffb;
    }
    free(node);
    bloke->flags &= 0xffdf;
    if (ride->type == 5) {
        if (IsFavouriteFood(bloke, ride->element)) {
            counter = GetBlokeCounter(bloke->target->data, GetBlokeNum(bloke));
            code = CalculateRideCode(bloke->field_7e, bloke->target->data, counter);
            FUN_00482df0(bloke, 9, code);
            bloke->field_7c = 0;
        } else {
            counter = GetBlokeCounter(bloke->target->data, GetBlokeNum(bloke));
            code = CalculateRideCode(bloke->field_7e, bloke->target->data, counter);
            FUN_00482df0(bloke, 10, code);
            bloke->field_7c = 0;
        }
    } else {
        if (IsFavouriteAttraction(bloke, ride->element)) {
            counter = GetBlokeCounter(bloke->target->data, GetBlokeNum(bloke));
            code = CalculateRideCode(bloke->field_7e, bloke->target->data, counter);
            FUN_00482df0(bloke, 0xb, code);
        } else {
            counter = GetBlokeCounter(bloke->target->data, GetBlokeNum(bloke));
            code = CalculateRideCode(bloke->field_7e, bloke->target->data, counter);
            FUN_00482df0(bloke, 0xc, code);
        }
    }
    counter = GetBlokeCounter(bloke->target->data, GetBlokeNum(bloke));
    bloke->field_78 = bloke->field_78 + (short)(counter * 0x32);
    bloke->last_ride = bloke->target;
    IncrementBlokeCounter(bloke->target->data, GetBlokeNum(bloke));
    NewLongTermAction(bloke, 0x17);
}

// FUNCTION: LEGOLAND 0x0048a2d0
LEGO_EXPORT void UpdateBlokesOnRide(void) {}

// FUNCTION: LEGOLAND 0x0048a2e0
LEGO_EXPORT void RemoveAllBlokesFromRide(struct Ride *ride, TileId tile) {
    int tx;
    int ty;
    struct RideNode *node;
    struct RideNode *next;
    struct Bloke *bloke;
    struct SampleSource source;

    tx = ride->x + tile.pos.x;
    ty = ride->y + tile.pos.y;
    source.type = 1;
    next = ride->riders;
    while (node = next, node != 0) {
        next = node->next;
        if (node->tile.id == tile.id) {
            bloke = node->rider;
            bloke->person->sprite = NULL;
            bloke->pos.x = tx * 0x100;
            bloke->pos.y = ty * 0x100;
            bloke->field_70 = 0;
            RemoveBlokeFromRide(ride, node);
            BlokeWalkAnim(bloke);
            bloke->flags &= 0xff7f;
            source.field_4 = bloke;
            KillAllSamplesFromSource(&source);
        }
    }
}

// FUNCTION: LEGOLAND 0x0048a390
LEGO_EXPORT int GetAllBlokesOffRide(struct Ride *ride, unsigned short uid) {
    struct RideNode *node;
    struct RideNode *next;

    for (node = ride->riders; node != 0; node = next) {
        next = node->next;
        if (node->tile.id == uid) {
            struct Bloke *rider = node->rider;
            if ((rider->flags & 0x40) == 0) {
                rider->flags |= 0x8;
                rider->param_action++;
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048a3e0
LEGO_EXPORT TileId GetObjectUID(struct Point *pos, struct Ride *ride) {
    struct MapElement *element;
    struct Point tile;
    int n;
    struct Point origin;
    TileId none;

    tile = *pos;
    tile.x >>= 8;
    tile.y >>= 8;
    n = tile.y - 1;
    if (tile.x >= 0 && tile.x < lpConfig->width && n >= 0 && n < lpConfig->height) {
        element = &GameMap[n][tile.x];
    } else {
        element = NULL;
    }
    if (element != NULL && (element->flags & 0x80) != 0 && element->field_0 != NULL && element->field_0->data == ride) {
        origin.x = element->field_4 + ride->x;
        origin.y = element->field_5 + ride->y;
        if (origin.x == tile.x && origin.y == tile.y) {
            return element->anchor;
        }
    }
    n = tile.y + 1;
    if (tile.x >= 0 && tile.x < lpConfig->width && n >= 0 && n < lpConfig->height) {
        element = &GameMap[n][tile.x];
    } else {
        element = NULL;
    }
    if (element != NULL && (element->flags & 0x80) != 0 && element->field_0 != NULL && element->field_0->data == ride) {
        origin.x = element->field_4 + ride->x;
        origin.y = element->field_5 + ride->y;
        if (origin.x == tile.x && origin.y == tile.y) {
            return element->anchor;
        }
    }
    n = tile.x - 1;
    if (n >= 0 && n < lpConfig->width && tile.y >= 0 && tile.y < lpConfig->height) {
        element = &GameMap[tile.y][n];
    } else {
        element = NULL;
    }
    if (element != NULL && (element->flags & 0x80) != 0 && element->field_0 != NULL && element->field_0->data == ride) {
        origin.x = element->field_4 + ride->x;
        origin.y = element->field_5 + ride->y;
        if (origin.x == tile.x && origin.y == tile.y) {
            return element->anchor;
        }
    }
    n = tile.x + 1;
    if (n >= 0 && n < lpConfig->width && tile.y >= 0 && tile.y < lpConfig->height) {
        element = &GameMap[tile.y][n];
    } else {
        element = NULL;
    }
    if (element != NULL && (element->flags & 0x80) != 0 && element->field_0 != NULL && element->field_0->data == ride) {
        origin.x = element->field_4 + ride->x;
        origin.y = element->field_5 + ride->y;
        if (origin.x == tile.x && origin.y == tile.y) {
            return element->anchor;
        }
    }
    none.id = 0;
    return none;
}
