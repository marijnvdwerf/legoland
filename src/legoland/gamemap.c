#include "legoland.h"
#include "crt.h"
#include "globals.h"

#include "gamemap.h"
#include "timer.h"
#include "sound_music.h"
#include "llidb.h"
#include "map_object.h"


struct RenderObjectVtable {
    unsigned char pad_0[0xc];
    void *get_power;
};

struct RenderObject {
    struct RenderObjectVtable *vtable;
    unsigned char pad_4[2];
    union {
        unsigned short word;
        unsigned char bytes[2];
    } coords;
    unsigned char pad_8[4];
    union {
        unsigned short word;
        unsigned char bytes[2];
    } flags;
    unsigned char pad_e[2];
};

// FUNCTION: LEGOLAND 0x00459850
LEGO_EXPORT void InitGameMap(void) {
    // STRING: LEGOLAND 0x004b5c0c
    DAT_0080ff64 = ElemID("CASTLE OBJ");
    Load_FXList(DAT_004b9228, 0x17);
}

// FUNCTION: LEGOLAND 0x00459870
LEGO_EXPORT void KillGameMap(void) {
    Kill_FXList(DAT_004b9228, 0x17);
}

// FUNCTION: LEGOLAND 0x00459880
void FUN_00459880(void) {
    DAT_00667ce0 = 0;
    DAT_00667ce4 = 0;
    DAT_00667ce8 = 0;
    DAT_00667cec = 0;
    DAT_00667cf0 = 0;
    DAT_00667cf4 = 0;
    DAT_00667cf8 = 0;
    DAT_00667cfc = 0;
}

// FUNCTION: LEGOLAND 0x004598d0
void FUN_004598d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00459960
void FUN_00459960(void) {
    DAT_00667d10 = GetGameTimer();
}

// FUNCTION: LEGOLAND 0x00459970
void FUN_00459970(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00459ad0
LEGO_EXPORT void PutObjOnMap(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00459c90
LEGO_EXPORT void RemObjFromMap(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00459fa0
LEGO_EXPORT int FindObjectsPower(void *object) { STUB(); }

// FUNCTION: LEGOLAND 0x0045a000
void FUN_0045a000(int power, struct RenderObject *object) {
    object->flags.word &= 0xfeff;
    DAT_00832bd8 = DAT_00832bd8 - power;
    DAT_00832bdc = DAT_00832bdc - 1;
}

// FUNCTION: LEGOLAND 0x0045a030
void FUN_0045a030(int power, struct RenderObject *object) { STUB(); }

// FUNCTION: LEGOLAND 0x0045a060
void FUN_0045a060(void) {
    struct RenderObject *object = GetFirstRenderObject();
    int power;
    if (object == NULL) {
        return;
    }
    while (object != NULL) {
        if (object->flags.bytes[1] & 1) {
            power = -FindObjectsPower(object->vtable->get_power);
            if (DAT_00832bd4 - (int)DAT_00832bd8 + power <= DAT_00832bd0) {
                FUN_0045a000(power, object);
                if (DAT_00832bd4 - (int)DAT_00832bd8 == DAT_00832bd0) {
                    return;
                }
            }
        }
        object = GetNextRenderObject(object);
    }
}

// FUNCTION: LEGOLAND 0x0045a0d0
void FUN_0045a0d0(void) {
    struct RenderObject *object = GetFirstRenderObject();
    int power;
    while (object != NULL) {
        if ((object->flags.bytes[1] & 1) == 0) {
            power = -FindObjectsPower(object->vtable->get_power);
            if (power > 0) {
                FUN_0045a030(power, object);
                if (DAT_00832bd4 - (int)DAT_00832bd8 <= DAT_00832bd0) {
                    break;
                }
            }
        }
        object = GetNextRenderObject(object);
    }
}

// FUNCTION: LEGOLAND 0x0045a130
LEGO_EXPORT void AddObjectsPowerStats(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045a230
LEGO_EXPORT void RemoveObjectsPowerStats(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045a390
LEGO_EXPORT void DefaultCursor(struct Cursor *cursor) {
    unsigned int saved_1404 = cursor->field_1404;
    unsigned int saved_1408 = cursor->field_1408;
    memset(cursor, 0, 0x1834);
    cursor->field_1404 = saved_1404;
    cursor->field_1828 = 0xc00;
    cursor->field_1408 = saved_1408;
    FUN_0045f460(cursor);
}

// FUNCTION: LEGOLAND 0x0045a3e0
void FUN_0045a3e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045a430
void FUN_0045a430(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045a4a0
LEGO_EXPORT void CalculateMapRenderOrder(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045a660
void FUN_0045a660(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045a850
LEGO_EXPORT struct RenderObject *GetFirstRenderObject(void) {
    int x;
    int y;
    struct RenderObject *element;

    x = (unsigned char)DAT_007febb8;
    y = (unsigned char)(DAT_007febb8 >> 8);
    if (x < 0 || x >= (int)lpConfig->width || y < 0 || y >= (int)lpConfig->height) {
        return 0;
    }
    element = (struct RenderObject *)((char *)GameMap[y] + x * 0x14);
    if (element == 0) {
        return 0;
    }
    if (DAT_007febb8 == 0 && (element->flags.bytes[0] & 0xa8) == 0) {
        return 0;
    }
    return element;
}

// FUNCTION: LEGOLAND 0x0045a8b0
LEGO_EXPORT struct RenderObject *GetNextRenderObject(struct RenderObject *object) {
    int x;
    int y;

    if (object == 0 || object->coords.word == 0) {
        return 0;
    }
    x = object->coords.bytes[0];
    y = object->coords.bytes[1];
    if (x >= 0 && x < (int)lpConfig->width && y >= 0 && y < (int)lpConfig->height) {
        return (struct RenderObject *)((char *)GameMap[y] + x * 0x14);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045a910
LEGO_EXPORT struct RenderObject *GetFirstObjectMatching(struct RenderObjectVtable *vtable) {
    struct RenderObject *object = GetFirstRenderObject();
    while (object != NULL) {
        if (object->vtable == vtable) {
            return object;
        }
        object = GetNextRenderObject(object);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0045a940
LEGO_EXPORT struct RenderObject *GetNextObjectMatching(struct RenderObject *object, struct RenderObjectVtable *vtable) {
    object = GetNextRenderObject(object);
    while (object != NULL) {
        if (object->vtable == vtable) {
            return object;
        }
        object = GetNextRenderObject(object);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0045a970
LEGO_EXPORT void PlayfieldToMap(void) { STUB(); }
