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
char *ReadResFileLine(struct ResFile *file, char *dest, int maxlen) {
    int error = 0;
    int count = 0;
    char c;
    char *out = dest;
    while (1) {
        if (RES_ReadFile(file, &c, 1) == 0) {
            error = 1;
            break;
        }
        if (c == '\r') {
            break;
        }
        if (c == '\n') {
            goto done;
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
done:
    *out = '\0';
    if (error && count == 0) {
        return 0;
    }
    return dest;
}

// FUNCTION: LEGOLAND 0x00489ee0
void ClearObjInstanceTable(void) {
    struct ObjTableEntry *entry;

    for (entry = ObjInstanceTable; (INT_PTR)entry < (INT_PTR)&ColorGreenBits; entry++) {
        entry->key = 0xffff;
    }
}

// FUNCTION: LEGOLAND 0x00489f00
int AddObjInstance(const struct Point *pos) {
    int index;
    struct ObjTableEntry *entry;

    index = 0;
    for (entry = ObjInstanceTable; (INT_PTR)entry < (INT_PTR)&ColorGreenBits; entry++) {
        if (entry->key == 0xffff) {
            ObjInstanceTable[index].key = (pos->x << 8) + pos->y;
            ObjInstanceTableCounts[index].key = 0;
            return 1;
        }
        index++;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00489f50
int RemoveObjInstance(const struct Point *pos) {
    int index;
    unsigned short target;
    struct ObjTableEntry *entry;

    index = 0;
    target = (pos->x << 8) + pos->y;
    for (entry = ObjInstanceTable; (INT_PTR)entry < (INT_PTR)&ColorGreenBits; entry++) {
        if (entry->key == target) {
            ObjInstanceTable[index].key = 0xffff;
            return 1;
        }
        index++;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00489f90
int IncObjInstanceCount(const struct Point *pos) {
    int index;
    unsigned short target;
    struct ObjTableEntry *entry;

    index = 0;
    target = (pos->x << 8) + pos->y;
    for (entry = ObjInstanceTable; (INT_PTR)entry < (INT_PTR)&ColorGreenBits; entry++) {
        if (entry->key == target) {
            ObjInstanceTableCounts[index].key++;
            return 1;
        }
        index++;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00489fd0
unsigned short GetObjInstanceCount(const struct Point *pos) {
    int index;
    unsigned short target;
    struct ObjTableEntry *entry;

    index = 0;
    target = (pos->x << 8) + pos->y;
    for (entry = ObjInstanceTable; (INT_PTR)entry < (INT_PTR)&ColorGreenBits; entry++) {
        if (entry->key == target) {
            return ObjInstanceTableCounts[index].key;
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
void FreeAllObjectClassInstances(void) {
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
LEGO_EXPORT void HandleRideAI(void) {}

// FUNCTION: LEGOLAND 0x0048a100
LEGO_EXPORT void RemoveBlokeFromRide(struct Ride *ride, struct RideNode *node) {
    struct Bloke *bloke;
    struct MapElement *element;
    int counter;
    int code;

    bloke = node->rider;
    RemoveBlokeFromList(ride, node);
    if (CountRidersAtTile(ride, &node->tile) == 0) {
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
            ApplyMoodDelta(bloke, 9, code);
            bloke->field_7c = 0;
        } else {
            counter = GetBlokeCounter(bloke->target->data, GetBlokeNum(bloke));
            code = CalculateRideCode(bloke->field_7e, bloke->target->data, counter);
            ApplyMoodDelta(bloke, 10, code);
            bloke->field_7c = 0;
        }
    } else {
        if (IsFavouriteAttraction(bloke, ride->element)) {
            counter = GetBlokeCounter(bloke->target->data, GetBlokeNum(bloke));
            code = CalculateRideCode(bloke->field_7e, bloke->target->data, counter);
            ApplyMoodDelta(bloke, 0xb, code);
        } else {
            counter = GetBlokeCounter(bloke->target->data, GetBlokeNum(bloke));
            code = CalculateRideCode(bloke->field_7e, bloke->target->data, counter);
            ApplyMoodDelta(bloke, 0xc, code);
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
            bloke->height = 0;
            RemoveBlokeFromRide(ride, node);
            BlokeWalkAnim(bloke);
            bloke->flags &= 0xff7f;
            source.bloke = bloke;
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

// Finds the TileId of `ride` at map position `pos` (in 1/256 tiles): checks the tiles above, below,
// left and right of it for an object tile (flag 0x80) of this ride whose anchor (ride origin plus
// the tile's field_4/field_5 offset) is the tile itself, and returns that tile's anchor id; 0 if
// none. As in the original, the tile below is only checked when the tile above exists, and it is
// never NULL-checked.
// FUNCTION: LEGOLAND 0x0048a3e0
LEGO_EXPORT TileId GetObjectUID(struct Point *pos, struct Ride *ride) {
    struct MapElement *element;
    struct Point tile;
    struct Point probe;
    struct Point origin;
    int n;
    TileId none;

    tile = *pos;
    tile.x >>= 8;
    tile.y >>= 8;
    n = tile.y - 1;
    element = (tile.x >= 0 && tile.x < lpConfig->width && n >= 0 && n < lpConfig->height) ? &GameMap[n][tile.x] : NULL;
    /* the original only looks below when the tile above exists, and never NULL-checks the tile below */
    if (element != NULL) {
        if ((element->flags & 0x80) != 0 && element->field_0 != NULL && element->field_0->data == ride) {
            origin.x = element->field_4 + ride->x;
            origin.y = element->field_5 + ride->y;
            if (origin.x == tile.x && origin.y == tile.y) {
                return element->anchor;
            }
        }
        probe = tile;
        probe.y++;
        element = (probe.x >= 0 && probe.x < lpConfig->width && probe.y >= 0 && probe.y < lpConfig->height) ? &GameMap[probe.y][probe.x] : NULL;
        if ((element->flags & 0x80) != 0 && element->field_0 != NULL && element->field_0->data == ride) {
            origin.x = element->field_4 + ride->x;
            origin.y = element->field_5 + ride->y;
            if (origin.x == tile.x && origin.y == tile.y) {
                return element->anchor;
            }
        }
    }
    probe = tile;
    probe.x--;
    element = (probe.x >= 0 && probe.x < lpConfig->width && probe.y >= 0 && probe.y < lpConfig->height) ? &GameMap[probe.y][probe.x] : NULL;
    if (element != NULL && (element->flags & 0x80) != 0 && element->field_0 != NULL && element->field_0->data == ride) {
        origin.x = element->field_4 + ride->x;
        origin.y = element->field_5 + ride->y;
        if (origin.x == tile.x && origin.y == tile.y) {
            return element->anchor;
        }
    }
    probe.x = tile.x + 1;
    element = (probe.x >= 0 && probe.x < lpConfig->width && probe.y >= 0 && probe.y < lpConfig->height) ? &GameMap[probe.y][probe.x] : NULL;
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
