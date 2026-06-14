#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "controller.h"
#include "gamemap.h"
#include "llidb.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "objclass.h"
#include "pathfind.h"
#include "sound_music.h"
#include "timer.h"
#include "worker.h"

struct RenderObjectVtable {
    unsigned char pad_0[0xc];
    void *get_power;
};

struct MapCellObjKind {
    unsigned char pad_0[0x20];
    short field_20;
};

struct MapCellObj {
    unsigned char pad_0[0xc];
    struct MapCellObjKind *field_c;
};

struct MapCell {
    /* 0x00 */ struct MapCellObj *obj;
    /* 0x04 */ unsigned char byte_4;
    /* 0x05 */ unsigned char byte_5;
    /* 0x06 */ unsigned char pad_6[2];
    /* 0x08 */ unsigned short word_8;
    /* 0x0a */ unsigned short word_a;
    /* 0x0c */ union {
        unsigned short word;
        unsigned char bytes[2];
    } flags;
    /* 0x0e */ unsigned char pad_e[2];
    /* 0x10 */ unsigned char byte_10;
    /* 0x11 */ unsigned char pad_11[3];
};

struct RectListNode {
    /* 0x00 */ struct RectListNode *next;
    /* 0x04 */ unsigned char pad_4[4];
    /* 0x08 */ int field_8;
    /* 0x0c */ int field_c;
    /* 0x10 */ int field_10;
    /* 0x14 */ int field_14;
};

struct RemBlock {
    /* 0x00 */ int f0;
    /* 0x04 */ int f4;
    /* 0x08 */ int f8;
    /* 0x0c */ int fc;
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
void FUN_004598d0(struct Point *coord, int *param_2, int *param_3) {
    struct MapCell *cell;
    short kind;

    if (coord->x >= 0 && coord->x < (int)lpConfig->width && coord->y >= 0 && coord->y < (int)lpConfig->height &&
        (cell = (struct MapCell *)((char *)GameMap[coord->y] + coord->x * 0x14)) != NULL) {
        if ((cell->flags.word & 0x10) != 0) {
            *param_2 = *param_2 + -1;
            return;
        }
        if ((cell->flags.word & 0x80) != 0) {
            if (cell->obj->field_c == DAT_007fd624) {
                *param_2 = *param_2 + -1;
                return;
            }
            kind = cell->obj->field_c->field_20;
            if (kind == 2 || kind == 3) {
                *param_3 = *param_3 + 1;
            }
        }
        return;
    }
    *param_2 = *param_2 + -1;
}

// FUNCTION: LEGOLAND 0x00459960
void FUN_00459960(void) {
    DAT_00667d10 = GetGameTimer();
}

// FUNCTION: LEGOLAND 0x00459970
void FUN_00459970(void) {
    int now;
    struct RectListNode *node;
    int total;
    int matched;
    struct Point coord;

    now = GetGameTimer();
    if (10000 < now - (int)DAT_00667d10) {
        DAT_00667d0c = 0;
        matched = 0;
        total = 0;
        DAT_00667d10 = now;
        for (node = (struct RectListNode *)FUN_00481720(); node != NULL; node = node->next) {
            total = total + 4 + (((node->field_14 - node->field_8) - node->field_c) + node->field_10) * 2;
            coord.x = node->field_8;
            if (coord.x <= node->field_10) {
                do {
                    coord.y = node->field_c + -1;
                    FUN_004598d0(&coord, &total, &matched);
                    coord.y = node->field_14 + 1;
                    FUN_004598d0(&coord, &total, &matched);
                    coord.x = coord.x + 1;
                } while (coord.x <= node->field_10);
            }
            coord.y = node->field_c;
            if (coord.y <= node->field_14) {
                do {
                    coord.x = node->field_8 + -1;
                    FUN_004598d0(&coord, &total, &matched);
                    coord.x = node->field_10 + 1;
                    FUN_004598d0(&coord, &total, &matched);
                    coord.y = coord.y + 1;
                } while (coord.y <= node->field_14);
            }
        }
        DAT_00667d00 = total;
        DAT_00667d04 = matched;
        if (total != 0) {
            DAT_00667d08 = (matched * 100) / total;
            return;
        }
        DAT_00667d08 = 0;
        DAT_00667d00 = 0;
    }
}

// FUNCTION: LEGOLAND 0x00459ad0
LEGO_EXPORT void PutObjOnMap(struct ObjClass *obj, unsigned int classid, struct Point *pos) {
    int area;
    struct ObjClass *entrance;
    struct MapCell *cell;

    if (classid == DAT_0080ff64) {
        DAT_0079a8d0 = 1;
    }
    obj->method_98(classid, pos);
    if (obj == DAT_007fd624) {
        DAT_00667cf4 = DAT_00667cf4 + 1;
        DAT_00667ce0 = DAT_00667ce0 + 1;
        DAT_00667d0c = 1;
    } else {
        area = GetRectArea((struct RectNode *)&obj->field_3c);
        switch (obj->type) {
        case 1:
            FUN_00489f00(pos);
            DAT_00667ce4 = DAT_00667ce4 + area;
            break;
        case 2:
            DAT_00667d0c = 1;
            DAT_00667cf8 = DAT_00667cf8 + area;
            break;
        case 3:
            DAT_00667cf0 = DAT_00667cf0 + area;
            break;
        case 4:
            FUN_00489f00(pos);
            DAT_00667ce8 = DAT_00667ce8 + area;
            break;
        case 5:
            FUN_00489f00(pos);
            DAT_00667cec = DAT_00667cec + area;
        }
        DAT_00667ce0 = DAT_00667ce0 + area;
        AddObjectsPowerStats(classid, pos);
    }
    if (classid == ElemID("ENTRANCE 1")) {
        entrance = (struct ObjClass *)((struct ClassNode *)ElemID("ENTRANCE 1"))->iface;
        if (pos->x < 0 || pos->x >= (int)lpConfig->width || pos->y < 0 || pos->y >= (int)lpConfig->height) {
            cell = NULL;
        } else {
            cell = (struct MapCell *)((char *)GameMap[pos->y] + pos->x * 0x14);
        }
        DAT_004b8320 = (cell->byte_4 + entrance->field_3c) * 0x100 + -0x100;
        DAT_004b8324 = ((unsigned int)(entrance->field_48 - entrance->field_40) >> 1) * 0x100 +
                       (cell->byte_5 + entrance->field_40) * 0x100;
    }
    DAT_00668610 = DAT_00668610 | 1;
}

// FUNCTION: LEGOLAND 0x00459c90
LEGO_EXPORT void RemObjFromMap(struct ObjClass *obj, unsigned int classid, unsigned int coords, void *cursor) {
    int area;
    struct MapCell *cell;
    struct Cursor *query;
    unsigned int x;
    unsigned int y;
    struct RemBlock blk;

    FUN_0049b270(obj, coords);
    if (classid == DAT_0080ff64) {
        DAT_0079a8d0 = 0;
    }
    if (DAT_00667cd8 == 0) {
        blk.f8 = coords & 0xff;
        blk.fc = coords >> 8 & 0xff;
        blk.f0 = 2;
        PlayInstanceOfSample(DAT_004b9248, 0, 1, &blk);
    } else {
        DAT_00667cdc = 1;
    }
    RemoveObjectsPowerStats(classid, coords);
    obj->method_9c(classid, coords, cursor);
    if (obj == DAT_007fd624) {
        DAT_00667cf4 = DAT_00667cf4 + -1;
        DAT_00667ce0 = DAT_00667ce0 + -1;
        DAT_00667d0c = 1;
    } else {
        area = GetRectArea((struct RectNode *)&obj->field_3c);
        switch (obj->type) {
        case 1:
            FUN_00489f50((struct ObjClassKey *)&blk);
            DAT_00667ce4 = DAT_00667ce4 - area;
            break;
        case 2:
            DAT_00667d0c = 1;
            DAT_00667cf8 = DAT_00667cf8 - area;
            break;
        case 3:
            DAT_00667cf0 = DAT_00667cf0 - area;
            break;
        case 4:
            FUN_00489f50((struct ObjClassKey *)&blk);
            DAT_00667ce8 = DAT_00667ce8 - area;
            break;
        case 5:
            FUN_00489f50((struct ObjClassKey *)&blk);
            DAT_00667cec = DAT_00667cec - area;
        }
        DAT_00667ce0 = DAT_00667ce0 - area;
    }
    x = coords & 0xff;
    y = coords >> 8 & 0xff;
    if (x < lpConfig->width && y < lpConfig->height) {
        cell = (struct MapCell *)((char *)GameMap[y] + x * 0x14);
    } else {
        cell = NULL;
    }
    if (cell->flags.bytes[1] & 0x40) {
        RemoveRepairOrderAT((struct Worker *)obj, x, y);
        cell->flags.word = cell->flags.word & 0xbfff;
    }
    query = &QueryCursor;
    do {
        if ((query->field_1828 & 0x1000) != 0) {
            if (query != NULL) {
                for (blk.f4 = query->field_1414[1] + query->field_1408;
                     blk.f4 <= (int)(query->field_1408 + query->field_1414[3]); blk.f4 = blk.f4 + 1) {
                    for (blk.f0 = query->field_1404 + query->field_1414[0];
                         blk.f0 <= (int)(query->field_1404 + query->field_1414[2]); blk.f0 = blk.f0 + 1) {
                        cell = (struct MapCell *)((char *)GameMap[blk.f4] + blk.f0 * 0x14);
                        cell->flags.word = cell->flags.word & 0xffe7;
                        cell->byte_10 = 0;
                        cell->word_8 = cell->word_a;
                        FUN_0045d260((struct Point *)&blk);
                        RemovePathSquare((struct InstancePos *)&blk);
                    }
                }
            }
            DAT_00668610 = DAT_00668610 | 4;
            return;
        }
        query = (struct Cursor *)query->field_1830;
    } while (query != NULL);
    DAT_00668610 = DAT_00668610 | 4;
}

// FUNCTION: LEGOLAND 0x00459fa0
LEGO_EXPORT int FindObjectsPower(void *object) { STUB(); }

// FUNCTION: LEGOLAND 0x0045a000
void FUN_0045a000(int power, struct RenderObject *object) {
    object->flags.word &= 0xfeff;
    DAT_00832bd8 = DAT_00832bd8 - power;
    DAT_00832bdc = DAT_00832bdc - 1;
}

// FUNCTION: LEGOLAND 0x0045a030
void FUN_0045a030(int power, struct RenderObject *object) {
    object->flags.bytes[1] |= 1;
    DAT_00832bd8 = DAT_00832bd8 + power;
    DAT_00832bdc = DAT_00832bdc + 1;
}

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
LEGO_EXPORT void AddObjectsPowerStats(unsigned int classid, struct Point *pos) {
    int power;
    struct MapCell *cell;
    int amount;

    power = FindObjectsPower(((struct ClassNode *)classid)->iface);
    if (power != 0) {
        if (0 < power) {
            DAT_00832bd0 = DAT_00832bd0 + power;
            if (DAT_00832bd8 != 0) {
                FUN_0045a060();
            }
            return;
        }
        if (pos->x < 0 || pos->x >= (int)lpConfig->width || pos->y < 0 || pos->y >= (int)lpConfig->height) {
            cell = NULL;
        } else {
            cell = (struct MapCell *)((char *)GameMap[pos->y] + pos->x * 0x14);
        }
        amount = (power ^ power >> 0x1f) - (power >> 0x1f);
        DAT_00832bd4 = DAT_00832bd4 + amount;
        if (DAT_00832bd0 < DAT_00832bd4 - (int)DAT_00832bd8) {
            DAT_00832bd8 = DAT_00832bd8 + amount;
            DAT_00832bdc = DAT_00832bdc + 1;
            cell->flags.bytes[1] |= 1;
        } else {
            cell->flags.word = cell->flags.word & 0xfeff;
        }
        if (DAT_00832bd0 <= DAT_00832bd4) {
            DAT_00832bcc = 0;
            return;
        }
        DAT_00832bcc = 100 - (DAT_00832bd4 * 100) / DAT_00832bd0;
    }
}

// FUNCTION: LEGOLAND 0x0045a230
LEGO_EXPORT void RemoveObjectsPowerStats(unsigned int classid, unsigned int coords) {
    int power;
    struct MapCell *cell;
    int amount;

    power = FindObjectsPower(((struct ClassNode *)classid)->iface);
    if (power != 0) {
        if (0 < power) {
            if ((coords & 0xff) < lpConfig->width && (coords >> 8 & 0xff) < lpConfig->height) {
                cell = (struct MapCell *)((char *)GameMap[coords >> 8 & 0xff] + (coords & 0xff) * 0x14);
            } else {
                cell = NULL;
            }
            if ((cell->flags.bytes[1] & 2) == 0) {
                DAT_00832bd0 = DAT_00832bd0 - power;
                if (DAT_00832bd0 < DAT_00832bd4 - (int)DAT_00832bd8) {
                    FUN_0045a0d0();
                }
            }
        } else {
            amount = (power ^ power >> 0x1f) - (power >> 0x1f);
            DAT_00832bd4 = DAT_00832bd4 - amount;
            if (DAT_00832bd8 != 0) {
                if ((coords & 0xff) < lpConfig->width && (coords >> 8 & 0xff) < lpConfig->height) {
                    cell = (struct MapCell *)((char *)GameMap[coords >> 8 & 0xff] + (coords & 0xff) * 0x14);
                } else {
                    cell = NULL;
                }
                if ((cell->flags.bytes[1] & 1) != 0) {
                    DAT_00832bd8 = DAT_00832bd8 - amount;
                    DAT_00832bdc = DAT_00832bdc + -1;
                }
                FUN_0045a060();
            }
        }
        if (DAT_00832bd0 <= DAT_00832bd4) {
            DAT_00832bcc = 0;
            return;
        }
        DAT_00832bcc = 100 - (DAT_00832bd4 * 100) / DAT_00832bd0;
    }
}

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
void FUN_0045a3e0(int *param) { STUB(); }

// FUNCTION: LEGOLAND 0x0045a430
void FUN_0045a430(short param_1, int *param_2) { STUB(); }

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
