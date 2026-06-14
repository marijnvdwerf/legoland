#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "bricks.h"
#include "controller.h"
#include "gamemain.h"
#include "gamemap.h"
#include "map_object.h"
#include "math.h"
#include "objclass.h"
#include "pathfind.h"
#include "print_sprite.h"
#include "sound_music.h"
#include "tilemap.h"
#include "timer.h"

struct ObjInfo {
    unsigned char pad_c[12];
    int field_c;
    int field_10;
    unsigned char pad_14[16];
    signed char field_24;
    signed char field_25;
};

struct ObjEntry {
    unsigned int field_0;
    unsigned char pad_4[8];
    unsigned short flags;
};

struct Obj0c {
    unsigned char pad_0[12];
    void *field_c;
};

struct ObjFlags {
    unsigned char pad_0[0x1c];
    unsigned int flags;
};

struct ObjState {
    unsigned char pad_0[0x20];
    unsigned short state;
};

struct CursorObj {
    unsigned char pad_0[12];
    unsigned int field_c;
};

struct TileSprite {
    unsigned char pad_0[0x16];
    short size;
};

struct MapTile {
    unsigned char pad_0[8];
    unsigned short tile;
    unsigned char pad_a[2];
    unsigned short flags;
    unsigned short user_flags;
    unsigned char rf_flags[2];
    unsigned short obj_region;
};

struct ObjRect {
    int v[5];
};

struct ObjBox {
    unsigned char pad_0[12];
    int field_c;
    int field_10;
    unsigned char pad_14[0x20 - 0x14];
    short state;
    unsigned char pad_22[0x24 - 0x22];
    signed char field_24;
    signed char field_25;
    unsigned char pad_26[0x3c - 0x26];
    struct ObjRect rect;
};

struct ObjNode {
    unsigned char pad_0[12];
    struct ObjBox *field_c;
};

struct TileSpriteSrc {
    unsigned char pad_0[0x18];
    unsigned char (*get_rf_flags)(int x, int y);
};

struct ObjDef {
    unsigned char pad_0[8];
    unsigned char field_8;
};

struct ObjData {
    unsigned char pad_0[0x1c];
    unsigned int flags;
    unsigned char pad_20[0x58 - 0x20];
    struct ObjDef *field_58;
};

struct OverlayParam {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
};

struct Overlay {
    unsigned int field_0;
    unsigned int field_4;
    unsigned char pad_8[0xc];
    unsigned int field_14;
    unsigned int field_18;
    struct Overlay *next;
};

/* globals.h owns MapTileGrid as the canonical struct MapElement** map grid;
   map_object views the same memory through its own struct MapTile layout. */
#define MapTileGrid ((struct MapTile **)GameMap)

struct FootprintNode {
    /* 0x00 */ int x0;
    /* 0x04 */ int y0;
    /* 0x08 */ int x1;
    /* 0x0c */ int y1;
    /* 0x10 */ struct FootprintNode *next;
};

struct MapObject {
    /* 0x00 */ unsigned char pad_0[0x1c];
    /* 0x1c */ unsigned int flags;
    /* 0x20 */ short type;
    /* 0x22 */ unsigned char pad_22[0x2c - 0x22];
    /* 0x2c */ unsigned char field_2c;
    /* 0x2d */ unsigned char pad_2d[0x3c - 0x2d];
    /* 0x3c */ struct FootprintNode footprint;
    /* 0x50 */ unsigned char pad_50[0x74 - 0x50];
    /* 0x74 */ unsigned short *field_74;
    /* 0x78 */ unsigned char pad_78[0x90 - 0x78];
    /* 0x90 */ void (*method_90)(unsigned int param_1, void *footprint, int param_3);
    /* 0x94 */ unsigned char pad_94[0xc4 - 0x94];
    /* 0xc4 */ unsigned int field_c4;
};

struct EditObject {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ struct MapObject *obj;
};

// FUNCTION: LEGOLAND 0x0045dd80
LEGO_EXPORT void AddObjectToMap(struct EditObject *param_1, unsigned int param_2, int param_3) {
    struct MapObject *obj;
    unsigned int flags;
    struct MapElement *tile;
    int y;
    int count;
    struct FootprintNode rect;
    struct FootprintNode *next;

    obj = param_1->obj;
    flags = obj->flags;
    rect = obj->footprint;
    if (flags & 0x20000) {
        BGFullUpdate = 1;
    }
    IncrementObjectCount(obj);
    y = rect.y0;
    while (1) {
        for (; y <= rect.y1; y++) {
            if (rect.x0 <= rect.x1) {
                int ty = ((param_2 >> 8) & 0xff) + y;
                int tx = (param_2 & 0xff) + rect.x0;
                count = (rect.x1 - rect.x0) + 1;
                do {
                    if (tx >= 0 && tx < lpConfig->width && ty >= 0 && ty < lpConfig->height) {
                        tile = (struct MapElement *)((int)GameMap[ty] + tx * 0x14);
                        if (tile != 0) {
                            tile->field_10 = 0;
                            tile->field_0 = (unsigned int)param_1;
                            tile->field_4 = (unsigned short)param_2;
                            tile->flags = (unsigned short)(((tile->flags & 0x10) | param_3) | 0x80);
                            tile->field_11 = obj->field_2c;
                            if (obj->flags & 2) {
                                tile->field_10 = 2;
                            }
                            if (obj->flags & 1) {
                                tile->field_10 = 1;
                            }
                            if (obj->flags & 0x800000) {
                                *((unsigned char *)&tile->flags + 1) |= 0x80;
                            }
                        }
                    }
                    tx++;
                    count--;
                } while (count != 0);
            }
        }
        next = rect.next;
        if (next == 0) {
            break;
        }
        rect = *next;
        y = rect.y0;
    }
    if (DAT_00667cd8 == 0 && DAT_00667ca0 == 0) {
        CalculateMapRenderOrder();
    }
}

// FUNCTION: LEGOLAND 0x0045dee0
LEGO_EXPORT void SetObjRectFlags(struct EditObject *editObj, struct Point *pos, unsigned short flags) {
    struct MapObject *obj;
    unsigned char coords[2];
    struct Cursor *node;
    struct LegoConfig *cfg;
    unsigned int node_flags;
    struct FootprintNode rect;
    struct FootprintNode *next;
    struct MapElement *tile;
    int x;
    int y;
    unsigned int saved[0x60d];
    int center[2];

    obj = editObj->obj;
    coords[1] = *((unsigned char *)pos + 4);
    memcpy(saved, &EditCursor, 0x60d * sizeof(unsigned int));
    coords[0] = *(unsigned char *)pos;
    GetTileCentre(pos, center);
    obj->method_90(obj->field_c4, saved, 0x8f8);
    node = &EditCursor;
    do {
        cfg = lpConfig;
        node_flags = node->field_1828;
        rect.x0 = node->field_1414[0];
        rect.y0 = node->field_1414[1];
        rect.x1 = node->field_1414[2];
        rect.y1 = node->field_1414[3];
        rect.next = (struct FootprintNode *)node->field_1414[4];
        if ((node_flags & 0x3000) == 0) {
            y = rect.y0;
            while (1) {
                for (; y <= rect.y1; y++) {
                    for (x = rect.x0; x <= rect.x1; x++) {
                        int tx = node->field_1404 + x;
                        int ty = node->field_1408 + y;
                        if (tx >= 0 && tx < cfg->width && ty >= 0 && ty < cfg->height) {
                            tile = (struct MapElement *)((int)GameMap[ty] + tx * 0x14);
                            if (tile != 0) {
                                tile->field_0 = (unsigned int)editObj;
                                tile->field_4 = *(unsigned short *)coords;
                                tile->flags = (unsigned short)((tile->flags & 0x10) | flags);
                                if (obj->flags & 0x800000) {
                                    *((unsigned char *)&tile->flags + 1) |= 0x80;
                                }
                                tile->field_10 = 2;
                                cfg = lpConfig;
                            }
                        }
                    }
                }
                next = rect.next;
                if (next == 0) {
                    break;
                }
                rect = *next;
                y = rect.y0;
            }
        }
        node = (struct Cursor *)node->field_1830;
    } while (node != 0);
    memcpy(&EditCursor, saved, 0x60d * sizeof(unsigned int));
}

// FUNCTION: LEGOLAND 0x0045e080
void FUN_0045e080(struct EditObject *editObj, struct Point *pos, unsigned short flags) {
    struct MapObject *obj;
    unsigned char coords[2];
    struct Cursor *node;
    unsigned int node_flags;
    struct FootprintNode rect;
    struct FootprintNode *next;
    struct MapElement *tile;
    int x;
    int y;
    struct Point coord;
    unsigned int saved[0x60d];
    int center[2];

    obj = editObj->obj;
    coords[1] = *((unsigned char *)pos + 4);
    memcpy(saved, &EditCursor, 0x60d * sizeof(unsigned int));
    coords[0] = *(unsigned char *)pos;
    GetTileCentre(pos, center);
    obj->method_90(obj->field_c4, saved, 0x8f8);
    node = &EditCursor;
    do {
        node_flags = node->field_1828;
        rect.x0 = node->field_1414[0];
        rect.y0 = node->field_1414[1];
        rect.x1 = node->field_1414[2];
        rect.y1 = node->field_1414[3];
        rect.next = (struct FootprintNode *)node->field_1414[4];
        if ((node_flags & 0x3000) == 0) {
            y = rect.y0;
            while (1) {
                for (; y <= rect.y1; y++) {
                    for (x = rect.x0; x <= rect.x1; x++) {
                        coord.x = node->field_1404 + x;
                        coord.y = node->field_1408 + y;
                        if (coord.x >= 0 && coord.x < lpConfig->width && coord.y >= 0 && coord.y < lpConfig->height &&
                            (int)GameMap[coord.y] + coord.x * 0x14 != 0) {
                            FUN_004779d0(&coord);
                        }
                    }
                }
                next = rect.next;
                if (next == 0) {
                    break;
                }
                rect = *next;
                y = rect.y0;
            }
        }
        node = (struct Cursor *)node->field_1830;
    } while (node != 0);
    node = &EditCursor;
    do {
        node_flags = node->field_1828;
        rect.x0 = node->field_1414[0];
        rect.y0 = node->field_1414[1];
        rect.x1 = node->field_1414[2];
        rect.y1 = node->field_1414[3];
        rect.next = (struct FootprintNode *)node->field_1414[4];
        if ((node_flags & 0x3000) == 0) {
            y = rect.y0;
            while (1) {
                for (; y <= rect.y1; y++) {
                    for (x = rect.x0; x <= rect.x1; x++) {
                        int tx = node->field_1404 + x;
                        int ty = node->field_1408 + y;
                        if (tx >= 0 && tx < lpConfig->width && ty >= 0 && ty < lpConfig->height) {
                            tile = (struct MapElement *)((int)GameMap[ty] + tx * 0x14);
                            if (tile != 0) {
                                tile->field_0 = (unsigned int)editObj;
                                tile->field_4 = *(unsigned short *)coords;
                                tile->flags = (unsigned short)((tile->flags & 0x10) | flags);
                                if (obj->flags & 0x800000) {
                                    *((unsigned char *)&tile->flags + 1) |= 0x80;
                                }
                                tile->field_10 = 2;
                            }
                        }
                    }
                }
                next = rect.next;
                if (next == 0) {
                    break;
                }
                rect = *next;
                y = rect.y0;
            }
        }
        node = (struct Cursor *)node->field_1830;
    } while (node != 0);
    memcpy(&EditCursor, saved, 0x60d * sizeof(unsigned int));
}

// FUNCTION: LEGOLAND 0x0045e300
void FUN_0045e300(struct EditObject *editObj, struct Point *pos) {
    struct MapObject *obj;
    struct Cursor *node;
    struct LegoConfig *cfg;
    struct FootprintNode *rect;
    struct MapElement *tile;
    struct Obj0c *gtile;
    int coord[2];
    int center[2];
    int x;
    int y;
    unsigned int saved[0x60d];

    obj = editObj->obj;
    memcpy(saved, &EditCursor, 0x60d * sizeof(unsigned int));
    GetTileCentre(pos, center);
    obj->method_90(obj->field_c4, saved, 0x8f8);
    node = &EditCursor;
    cfg = lpConfig;
    do {
        if ((node->field_1828 & 0x3000) == 0) {
            for (rect = (struct FootprintNode *)&node->field_1414[0]; rect != 0; rect = rect->next) {
                y = rect->y0;
                if (y <= rect->y1) {
                    do {
                        x = rect->x0;
                        if (x <= rect->x1) {
                            do {
                                coord[0] = node->field_1404 + x;
                                coord[1] = node->field_1408 + y;
                                if (coord[0] >= 0 && coord[0] < cfg->width && coord[1] >= 0 &&
                                    coord[1] < cfg->height &&
                                    (tile = (struct MapElement *)((int)GameMap[coord[1]] + coord[0] * 0x14)) != 0 &&
                                    (tile->flags & 8) != 0 &&
                                    (gtile = (struct Obj0c *)tile->field_0)->field_c == DAT_007fd624) {
                                    RemovePathSquare((struct InstancePos *)coord);
                                    tile->flags &= 0xffe7;
                                    tile->field_10 &= 0xfe;
                                    tile = (struct MapElement *)((int)GameMap[coord[1]] + coord[0] * 0x14);
                                    tile->field_8 = tile->field_a;
                                    cfg = lpConfig;
                                }
                                x++;
                            } while (x <= rect->x1);
                        }
                        y++;
                    } while (y <= rect->y1);
                }
            }
        }
        node = (struct Cursor *)node->field_1830;
    } while (node != 0);
    memcpy(&EditCursor, saved, 0x60d * sizeof(unsigned int));
}

// FUNCTION: LEGOLAND 0x0045e4a0
void FUN_0045e4a0(struct EditObject *editObj, struct Point *pos) {
    struct MapObject *obj;
    unsigned char coords[2];
    struct Cursor *node;
    unsigned int node_flags;
    struct FootprintNode rect;
    struct FootprintNode *next;
    struct MapElement *tile;
    int x;
    int y;
    unsigned int saved[0x60d];
    int center[2];

    obj = editObj->obj;
    coords[1] = *((unsigned char *)pos + 4);
    memcpy(saved, &EditCursor, 0x60d * sizeof(unsigned int));
    coords[0] = *(unsigned char *)pos;
    GetTileCentre(pos, center);
    obj->method_90(obj->field_c4, saved, 0x8f8);
    node = &EditCursor;
    do {
        node_flags = node->field_1828;
        rect.x0 = node->field_1414[0];
        rect.y0 = node->field_1414[1];
        rect.x1 = node->field_1414[2];
        rect.y1 = node->field_1414[3];
        rect.next = (struct FootprintNode *)node->field_1414[4];
        if ((node_flags & 0x3000) == 0) {
            y = rect.y0;
            while (1) {
                for (; y <= rect.y1; y++) {
                    for (x = rect.x0; x <= rect.x1; x++) {
                        int tx = node->field_1404 + x;
                        int ty = node->field_1408 + y;
                        if (tx >= 0 && tx < lpConfig->width && ty >= 0 && ty < lpConfig->height) {
                            tile = (struct MapElement *)((int)GameMap[ty] + tx * 0x14);
                            if (tile != 0) {
                                tile->flags = 0;
                                tile->field_0 = (unsigned int)editObj;
                                tile->field_4 = *(unsigned short *)coords;
                                tile->field_10 = 0;
                            }
                        }
                    }
                }
                next = rect.next;
                if (next == 0) {
                    break;
                }
                rect = *next;
                y = rect.y0;
            }
        }
        node = (struct Cursor *)node->field_1830;
    } while (node != 0);
    memcpy(&EditCursor, saved, 0x60d * sizeof(unsigned int));
}

// FUNCTION: LEGOLAND 0x0045e620
int FUN_0045e620(struct ObjBox *obj) {
    struct ObjRect rect;

    if (obj != 0) {
        rect = obj->rect;
        if (obj->state != 3 && obj->state != 2 && obj->state != 0 &&
            (rect.v[0] > obj->field_c || obj->field_c > rect.v[2] ||
                rect.v[1] > obj->field_10 || obj->field_10 > rect.v[3])) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045e690
int FUN_0045e690(struct ObjInfo *obj) {
    if (((int)obj->field_24 != obj->field_c) || ((int)obj->field_25 != obj->field_10)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045e6b0
int FUN_0045e6b0(struct ObjBox *obj) {
    int x;
    struct ObjRect rect;

    x = obj->field_c;
    rect = obj->rect;
    if (rect.v[2] < x) {
        return 4;
    }
    if (x < rect.v[0]) {
        return 8;
    }
    return (rect.v[3] >= obj->field_10) + 1;
}

// FUNCTION: LEGOLAND 0x0045e710
int FUN_0045e710(struct ObjBox *obj) {
    struct ObjRect rect;

    rect = obj->rect;
    if (rect.v[2] < obj->field_24) {
        return 0x80;
    }
    if (obj->field_24 < rect.v[0]) {
        return 0x40;
    }
    return (((rect.v[3] >= obj->field_25) - 1) & 0x10) + 0x10;
}

// FUNCTION: LEGOLAND 0x0045e770
void FUN_0045e770(struct ObjNode *node, int *offset) {
    struct ObjBox *obj;
    struct MapTile *tile;
    int x;
    int y;

    obj = node->field_c;
    if (FUN_0045e620(obj) != 0) {
        x = obj->field_c + offset[0];
        y = obj->field_10 + offset[1];
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            tile = (struct MapTile *)((int)MapTileGrid[y] + x * 0x14);
            if (tile != 0) {
                tile->obj_region |= (unsigned short)FUN_0045e6b0(obj);
            }
        }
        if (FUN_0045e690((struct ObjInfo *)obj) != 0) {
            x = obj->field_24 + offset[0];
            y = obj->field_25 + offset[1];
            if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                tile = (struct MapTile *)((int)MapTileGrid[y] + x * 0x14);
                if (tile != 0) {
                    tile->obj_region |= (unsigned short)FUN_0045e710(obj);
                }
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0045e850
void FUN_0045e850(struct ObjNode *node, int *offset) {
    struct ObjBox *obj;
    struct MapTile *tile;
    int x;
    int y;

    obj = node->field_c;
    if (FUN_0045e620(obj) != 0) {
        x = obj->field_c + offset[0];
        y = obj->field_10 + offset[1];
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            tile = (struct MapTile *)((int)MapTileGrid[y] + x * 0x14);
            if (tile != 0) {
                tile->obj_region &= ~(unsigned short)FUN_0045e6b0(obj);
            }
        }
        if (FUN_0045e690((struct ObjInfo *)obj) != 0) {
            x = obj->field_24 + offset[0];
            y = obj->field_25 + offset[1];
            if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                tile = (struct MapTile *)((int)MapTileGrid[y] + x * 0x14);
                if (tile != 0) {
                    tile->obj_region &= ~(unsigned short)FUN_0045e710(obj);
                }
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0045e930
int FUN_0045e930(struct ObjEntry *obj) {
    struct Obj0c *p;

    if (obj == 0) {
        return 0;
    }
    if (obj->flags & 0x8a8) {
        p = (struct Obj0c *)obj->field_0;
        if (p->field_c != DAT_007fd624) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0045e960
int FUN_0045e960(struct ObjNode *node, int *offset) {
    struct ObjBox *obj;
    struct MapTile *tile;
    int x;
    int y;

    obj = node->field_c;
    if (FUN_0045e620(obj) != 0) {
        x = offset[0] + obj->field_c;
        y = offset[1] + obj->field_10;
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            tile = (struct MapTile *)((int)MapTileGrid[y] + x * 0x14);
            if (tile != 0 && FUN_0045e930((struct ObjEntry *)tile) == 0) {
                return 0;
            }
        }
        if (FUN_0045e690((struct ObjInfo *)obj) != 0) {
            x = offset[0] + obj->field_c;
            y = offset[1] + obj->field_10;
            if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                tile = (struct MapTile *)((int)MapTileGrid[y] + x * 0x14);
                if (tile != 0 && FUN_0045e930((struct ObjEntry *)tile) == 0) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0045ea40
void FUN_0045ea40(struct ObjBox *obj, int *out) {
    if ((obj->field_c < obj->rect.v[0] || obj->field_c > obj->rect.v[2] ||
            obj->field_10 < obj->rect.v[1] || obj->field_10 > obj->rect.v[3]) &&
        (obj->state == 1 || obj->state == 4 || obj->state == 5)) {
        out[0] = obj->field_c;
        out[1] = obj->field_10;
        return;
    }
    out[0] = obj->rect.v[2] + 1;
    out[1] = (obj->rect.v[3] + obj->rect.v[1]) / 2;
}

// FUNCTION: LEGOLAND 0x0045eab0
unsigned int FUN_0045eab0(struct ObjFlags *obj) {
    if (obj != 0) {
        return (~obj->flags >> 21) & 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045ead0
int FUN_0045ead0(struct ObjState *obj) {
    if (obj == 0 || obj->state == 0 || obj->state == 2) {
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0045eaf0
int FUN_0045eaf0(struct ObjData *obj) {
    if (FUN_0045ead0((struct ObjState *)obj) != 0) {
        return 1;
    }
    if (obj != 0 && obj->field_58 != 0 && (obj->field_58->field_8 & 0x10) != 0 &&
        (obj->flags & 0x600000) == 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045eb30
LEGO_EXPORT int BuildObject(struct EditObject *editObj, int *coords) {
    struct MapObject *obj;
    unsigned char packed[2];
    int cost;
    unsigned int *effect;
    int out[2];

    obj = editObj->obj;
    packed[0] = (unsigned char)coords[0];
    packed[1] = (unsigned char)coords[1];
    cost = GetObjCost((struct CostInfo *)obj);
    if (GetBrickCount() < cost) {
        return 0;
    }
    if (editObj == (struct EditObject *)DAT_0080ff64) {
        DAT_0079a8d0 = 1;
    }
    if (DAT_00667cd8 == 0) {
        PlayAppropriateBuildEffect((struct ObjClass *)obj, coords);
    } else {
        DAT_00667cdc = 1;
    }
    if ((obj->flags & 0x80000) == 0) {
        UseBricks(GetObjCost((struct CostInfo *)obj));
        if (FUN_0045eab0((struct ObjFlags *)obj) != 0 || FUN_0045eaf0((struct ObjData *)obj) != 0) {
            FUN_0045e080(editObj, (struct Point *)coords, 0);
        }
        FUN_0045ea40((struct ObjBox *)obj, out);
        out[0] = out[0] + coords[0];
        out[1] = out[1] + coords[1];
        PutObjOnMap((struct ObjClass *)obj, (unsigned int)editObj, (struct Point *)coords);
        if ((obj->flags & 0x400000) != 0) {
            FUN_00482a90();
            FUN_00482b20(1);
            effect = FUN_00482b00();
            FUN_00477bd0(out[0], out[1], effect[0], effect[1]);
        }
    } else {
        if (AddObjectToBuildList((struct ObjClass *)obj, (short)(unsigned int)editObj) == 0) {
            return 0;
        }
        UseBricks(GetObjCost((struct CostInfo *)obj));
        if (FUN_0045eab0((struct ObjFlags *)obj) != 0 || FUN_0045eaf0((struct ObjData *)obj) != 0) {
            FUN_0045e080(editObj, (struct Point *)coords, 0x20);
        } else {
            SetObjRectFlags(editObj, (struct Point *)coords, 0x20);
        }
        FUN_0045ea40((struct ObjBox *)obj, out);
        out[0] = out[0] + coords[0];
        out[1] = out[1] + coords[1];
        if ((obj->flags & 0x400000) != 0) {
            FUN_00482a90();
            FUN_00482b20(1);
            effect = FUN_00482b00();
            FUN_00477bd0(out[0], out[1], effect[0], effect[1]);
        }
        if (DAT_00667cd8 == 0) {
            CalculateMapRenderOrder();
        }
    }
    FUN_0045e770((struct ObjNode *)editObj, coords);
    return 1;
}

struct BuildBuf {
    /* 0x00 */ unsigned int x;
    /* 0x04 */ unsigned int y;
    /* 0x08 */ int two;
    /* 0x0c */ int field_c;
    /* 0x10 */ unsigned int x2;
    /* 0x14 */ unsigned int y2;
};

// FUNCTION: LEGOLAND 0x0045ed30
LEGO_EXPORT void ObjectIsBuilt(struct ObjClass *obj, unsigned int coords) {
    struct BuildBuf buf;
    int out[2];
    unsigned int saved_1414[5];
    int saved_1410;
    unsigned int saved_140c;
    struct Cursor *node;
    struct LegoConfig *cfg;
    struct FootprintNode *rect;
    struct MapElement *tile;
    int x;
    int y;

    buf.x = coords & 0xff;
    buf.y = (coords >> 8) & 0xff;
    buf.two = 2;
    buf.x2 = buf.x;
    buf.y2 = buf.y;
    UnSourceAndFadeAllSamplesFromSource(&buf.two, -200);
    GetTileCentre((struct Point *)&buf.x, out);
    EditCursor.field_1830 = 0;
    if ((GamePad & 0x1000) != 0) {
        buf.two = EditCursor.field_1404;
        saved_140c = EditCursor.field_140c;
        saved_1414[0] = EditCursor.field_1414[0];
        saved_1414[1] = EditCursor.field_1414[1];
        saved_1414[2] = EditCursor.field_1414[2];
        saved_1414[3] = EditCursor.field_1414[3];
        saved_1414[4] = EditCursor.field_1414[4];
        saved_1410 = EditCursor.field_1410;
        buf.field_c = EditCursor.field_1408;
    }
    ((struct MapObject *)obj)->method_90(obj->field_c4, out, 0x8f8);
    node = &EditCursor;
    cfg = lpConfig;
    do {
        rect = (struct FootprintNode *)&node->field_1414[0];
        do {
            if ((node->field_1828 & 0x3000) != 0) {
                break;
            }
            for (y = rect->y0; y <= rect->y1; y++) {
                for (x = rect->x0; x <= rect->x1; x++) {
                    int tx = node->field_1404 + x;
                    int ty = node->field_1408 + y;
                    if (tx >= 0 && tx < cfg->width && ty >= 0 && ty < cfg->height &&
                        (tile = (struct MapElement *)((int)GameMap[ty] + tx * 0x14)) != 0) {
                        tile->flags &= 0xffdf;
                        tile->field_10 = 0;
                        cfg = lpConfig;
                    }
                }
            }
            rect = rect->next;
        } while (rect != 0);
        node = (struct Cursor *)node->field_1830;
        if (node == 0) {
            PutObjOnMap(obj, obj->field_c4, (struct Point *)&buf.x);
            EditCursor.field_1830 = 0;
            if ((GamePad & 0x1000) != 0) {
                EditCursor.field_1414[0] = saved_1414[0];
                EditCursor.field_1414[1] = saved_1414[1];
                EditCursor.field_1414[2] = saved_1414[2];
                EditCursor.field_1414[3] = saved_1414[3];
                EditCursor.field_1414[4] = saved_1414[4];
                EditCursor.field_1404 = buf.two;
                EditCursor.field_1408 = buf.field_c;
                EditCursor.field_140c = saved_140c;
                EditCursor.field_1410 = saved_1410;
                return;
            }
            if (DAT_008119b8 != 0) {
                ((struct MapObject *)DAT_008119b8)->method_90(((struct ObjClass *)DAT_008119b8)->field_c4, &DAT_00813a44, 0x8f8);
            }
            return;
        }
    } while (1);
}

// FUNCTION: LEGOLAND 0x0045ef50
LEGO_EXPORT void ObjectIsBuilding(struct ObjClass *obj, unsigned int coords) {
    struct BuildBuf buf;

    buf.x = coords & 0xff;
    buf.y = (coords >> 8) & 0xff;
    if ((((struct MapObject *)obj)->flags & 0x200000) == 0) {
        buf.two = 2;
        buf.x2 = buf.x;
        buf.y2 = buf.y;
        if (CountSamplesFromSource((struct SampleParams *)&buf.two) == 0) {
            PlayAppropriateBuildEffect(obj, (int *)&buf.x);
        }
    }
}

// FUNCTION: LEGOLAND 0x0045efc0
LEGO_EXPORT void ApplyConsTileMap(struct EditObject *editObj, unsigned int coords) {
}

// FUNCTION: LEGOLAND 0x0045efd0
LEGO_EXPORT void ApplyDestrTileMap(struct EditObject *editObj, unsigned int coords) {
}

// FUNCTION: LEGOLAND 0x0045efe0
LEGO_EXPORT unsigned int AddBasicObject(struct EditObject *editObj, int *coords) {
    struct MapObject *obj;
    struct MapElement *tile;
    unsigned char packed[2];
    int instance;

    obj = editObj->obj;
    if (obj->flags & 0x20000) {
        BGFullUpdate = 1;
    }
    if (obj->flags & 0x40000) {
        if (coords[0] >= 0 && coords[0] < lpConfig->width && coords[1] >= 0 && coords[1] < lpConfig->height) {
            tile = (struct MapElement *)((int)GameMap[coords[1]] + coords[0] * 0x14);
        } else {
            tile = 0;
        }
        tile->field_0 = (unsigned int)editObj;
        tile->field_8 = *obj->field_74;
        *((unsigned char *)&tile->field_4) = (unsigned char)coords[0];
        *((unsigned char *)&tile->field_4 + 1) = (unsigned char)coords[1];
        if (obj->flags & 2) {
            tile->field_10 = 2;
        }
        if (obj->flags & 1) {
            tile->field_10 = 1;
        }
        *(unsigned char *)&tile->flags |= 8;
        if (obj->flags & 0x800000) {
            tile->flags |= 0x8000;
        }
        IncrementObjectCount(obj);
    } else {
        packed[0] = (unsigned char)coords[0];
        packed[1] = (unsigned char)coords[1];
        AddObjectToMap(editObj, *(unsigned short *)packed, 0);
        ApplyConsTileMap(editObj, *(unsigned short *)packed);
        if (obj->type != 2) {
            unsigned short id;
            *((unsigned char *)&id) = (unsigned char)coords[0];
            *((unsigned char *)&id + 1) = (unsigned char)coords[1];
            instance = CreateObjectInstance((unsigned int)obj, &id);
            if (instance != 0) {
                AddInstanceToList(instance);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0045f100
LEGO_EXPORT void RemoveObjectFromMap(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045f220
LEGO_EXPORT void StandardRemoveObject(unsigned int a, unsigned int b, unsigned int c) { STUB(); }

// FUNCTION: LEGOLAND 0x0045f440
LEGO_EXPORT void SetEditCursorFootPrint(void *src) {
    memcpy(EditCursor.field_1414, src, 20);
}

// FUNCTION: LEGOLAND 0x0045f460
void FUN_0045f460(struct Cursor *cursor) {
    cursor->field_140c = 1;
    cursor->field_1410 = 0;
}

// FUNCTION: LEGOLAND 0x0045f480
void FUN_0045f480(struct Cursor *cursor, int param) {
    int negated;

    negated = -param;
    if (cursor->field_140c >= negated) {
        cursor->field_140c = negated;
        cursor->field_1410 = param;
    }
}

// FUNCTION: LEGOLAND 0x0045f4b0
unsigned char FUN_0045f4b0(struct Cursor *cursor) {
    int target;

    target = cursor->field_140c;
    return target > 0;
}

// FUNCTION: LEGOLAND 0x0045f4d0
void FUN_0045f4d0(struct Cursor *cursor) { STUB(); }

// FUNCTION: LEGOLAND 0x0045f540
void FUN_0045f540(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045f5f0
LEGO_EXPORT void BuildCursorPtr(struct Cursor *cursor, unsigned int param_2, unsigned int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x0045f810
LEGO_EXPORT void ValidateCursor(struct Cursor *cursor, unsigned int param) { STUB(); }

// FUNCTION: LEGOLAND 0x0045fa80
LEGO_EXPORT void CalcBasicObjectCursor(struct CursorObj *obj, unsigned int a2, unsigned int a3) {
    unsigned int v1;

    v1 = obj->field_c;
    DefaultCursor(&EditCursor);
    ScreenToMapRef(a2, &EditCursor.field_1404, a3);
    v1 += 0x3c;
    memcpy(EditCursor.field_1414, (void *)v1, 20);
    ValidateCursor(&EditCursor, obj->field_c);
}

// FUNCTION: LEGOLAND 0x0045fad0
void FUN_0045fad0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045fca0
void FUN_0045fca0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045ff00
LEGO_EXPORT void RenderCursor(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00460540
LEGO_EXPORT void GetTileDimensions(int *width, int *height) {
    struct TileSprite *sprite;
    int size;

    sprite = TileSpriteArray[DAT_00667ca4];
    size = sprite->size;
    *height = size;
    *width = size * 2;
}

// FUNCTION: LEGOLAND 0x00460560
void FUN_00460560(int index) { STUB(); }

// FUNCTION: LEGOLAND 0x004608c0
void FUN_004608c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00460e00
void FUN_00460e00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00460e90
void FUN_00460e90(int *coords, unsigned int x, unsigned int y) {
    unsigned char dir = FUN_0045ceb0(coords);
    unsigned int mode = FUN_0045d080(dir, coords);

    FUN_00485f00(TileSpriteArray[3 + (unsigned int)dir + *(int *)PathSprite], x, y);
    switch (mode) {
    case 1:
        FUN_00485f00(TileSpriteArray[19 + *(int *)PathSprite], x, y);
        break;
    case 2:
        FUN_00485f00(TileSpriteArray[20 + *(int *)PathSprite], x, y);
        break;
    case 3:
        FUN_00485f00(TileSpriteArray[21 + *(int *)PathSprite], x, y);
        break;
    }
}

// FUNCTION: LEGOLAND 0x00460f50
void FUN_00460f50(int *coords, unsigned int x, unsigned int y, unsigned int param_4) {
    unsigned char dir = FUN_0045ceb0(coords);
    unsigned int mode = FUN_0045d080(dir, coords);

    FUN_00485f00(TileSpriteArray[3 + (unsigned int)dir + *(int *)PathSprite], x, y);
    switch (mode) {
    case 1:
        PrintSprite(TileSpriteArray[19 + *(int *)PathSprite], x, y, param_4, 0);
        break;
    case 2:
        PrintSprite(TileSpriteArray[20 + *(int *)PathSprite], x, y, param_4, 0);
        break;
    case 3:
        PrintSprite(TileSpriteArray[21 + *(int *)PathSprite], x, y, param_4, 0);
        break;
    }
}

// FUNCTION: LEGOLAND 0x00461020
void FUN_00461020(void) {
    int i;

    i = 0;
    PTR_DAT_004b95f0 = DAT_00801420;
    if (DAT_00667d44 > 0) {
        do {
            PrintSprite(PTR_DAT_004b95f0->sprite, PTR_DAT_004b95f0->x, PTR_DAT_004b95f0->y, PTR_DAT_004b95f0->flags, 0);
            i++;
            PTR_DAT_004b95f0++;
        } while (i < DAT_00667d44);
    }
    DAT_00667d44 = 0;
    PTR_DAT_004b95f0 = DAT_00801420;
}

// FUNCTION: LEGOLAND 0x00461080
void FUN_00461080(int *coord, unsigned int param2, unsigned int param3, unsigned int param4) {
    struct MapTile *tile;

    if (coord[0] >= 0 && coord[0] < lpConfig->width &&
        coord[1] >= 0 && coord[1] < lpConfig->height) {
        tile = MapTileGrid[coord[1]];
        tile = tile + coord[0];
        if (tile != 0 && FUN_0045ce10(tile) != 0 && tile->tile != 0) {
            FUN_00460f50(coord, param2, param3, param4);
        }
    }
}

// FUNCTION: LEGOLAND 0x004610f0
void FUN_004610f0(int *a, int *b) { STUB(); }

// FUNCTION: LEGOLAND 0x00461220
void FUN_00461220(void) {
    int local[6];

    local[0] = ScrollX >> 8;
    local[1] = ScrollY >> 8;
    local[2] = lpConfig->field_20;
    local[3] = lpConfig->field_22;
    local[4] = lpConfig->field_10 + local[2];
    local[5] = lpConfig->field_12 + local[3];
    FUN_004610f0(&local[0], &local[2]);
}

// FUNCTION: LEGOLAND 0x00461290
void FUN_00461290(unsigned int a, unsigned int b, unsigned int c, unsigned int d) { STUB(); }

// FUNCTION: LEGOLAND 0x004614a0
LEGO_EXPORT void ProcessScrolling(unsigned int a, unsigned int b) {
    ScrollX = ScrollX + a;
    ScrollY = ScrollY + b;
    FUN_00461290(lpConfig->field_10 << 8, lpConfig->field_12 << 8, 0, 0);
}

// FUNCTION: LEGOLAND 0x004614f0
LEGO_EXPORT void MouseScrollMap(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004615f0
LEGO_EXPORT int Get_XScroll(void) {
    return ScrollX >> 8;
}

// FUNCTION: LEGOLAND 0x00461600
LEGO_EXPORT int Get_YScroll(void) {
    return ScrollY >> 8;
}

// FUNCTION: LEGOLAND 0x00461610
LEGO_EXPORT unsigned char Get_RFFlags(int x, int y) {
    struct MapTile *tile;

    tile = MapTileGrid[y >> 8];
    return tile[x >> 8].rf_flags[0];
}

// FUNCTION: LEGOLAND 0x00461630
LEGO_EXPORT unsigned char GetCurrentRFFlags(int x, int y) {
    struct MapTile *tile;
    unsigned char rf;
    unsigned char (*callback)(int x, int y);
    int tx;
    int ty;

    if (x < 0 || x >= (int)((unsigned int)lpConfig->width * 0x100) ||
        y < 0 || y >= (int)((unsigned int)lpConfig->height * 0x100)) {
        return 2;
    }
    rf = Get_RFFlags(x, y);
    if ((rf & 2) == 0) {
        return rf;
    }
    if ((rf & 1) == 0) {
        return rf;
    }
    tx = x >> 8;
    ty = y >> 8;
    if (tx < 0 || tx >= (int)lpConfig->width || ty < 0 || ty >= (int)lpConfig->height) {
        tile = 0;
    } else {
        tile = (struct MapTile *)((int)MapTileGrid[ty] + tx * 0x14);
    }
    callback = ((struct TileSpriteSrc *)TileSpriteInfo[tile->tile].src)->get_rf_flags;
    if (callback != 0) {
        return callback(x, y);
    }
    return 2;
}

// FUNCTION: LEGOLAND 0x004616e0
LEGO_EXPORT void Set_RFFlags(int x, int y, unsigned char value) {
    struct MapTile *tile;

    tile = MapTileGrid[y >> 8];
    tile[x >> 8].rf_flags[0] = value;
}

// FUNCTION: LEGOLAND 0x00461710
LEGO_EXPORT short Get_UserFlags(int x, int y) {
    struct MapTile *tile;

    tile = MapTileGrid[y >> 8];
    return tile[x >> 8].user_flags;
}

// FUNCTION: LEGOLAND 0x00461730
LEGO_EXPORT void Set_UserFlags(int x, int y, unsigned short value) {
    struct MapTile *tile;

    tile = MapTileGrid[y >> 8];
    tile[x >> 8].user_flags = value;
}

// FUNCTION: LEGOLAND 0x00461760
LEGO_EXPORT short Get_MapFlags(int x, int y) {
    struct MapTile *tile;

    tile = MapTileGrid[y >> 8];
    return tile[x >> 8].flags;
}

// FUNCTION: LEGOLAND 0x00461780
LEGO_EXPORT void SetMapTile(int x, int y, unsigned short value) {
    if (x < 0) {
        return;
    }
    if (x > (int)lpConfig->width) {
        return;
    }
    if (y < 0) {
        return;
    }
    if (y > (int)lpConfig->height) {
        return;
    }
    MapTileGrid[y][x].tile = value;
}

// FUNCTION: LEGOLAND 0x004617d0
LEGO_EXPORT unsigned short GetMapFlags(int x, int y) {
    struct MapTile *tile;

    if (x < 0 || x > 0x100 || y < 0 || y > 0x100) {
        return 0x40;
    }
    tile = MapTileGrid[y];
    return tile[x].flags;
}

// FUNCTION: LEGOLAND 0x00461810
LEGO_EXPORT void SetMapFlags(int x, int y, short value) {
    if (x < 0 || x > 256) {
        return;
    }
    if (y < 0 || y > 256) {
        return;
    }
    MapTileGrid[y][x].flags = value;
}

// FUNCTION: LEGOLAND 0x00461850
LEGO_EXPORT void GetObjectClassAndInstance(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004618d0
void FUN_004618d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00461a50
LEGO_EXPORT unsigned int LoadBaseMap(unsigned int param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x004629e0
void FUN_004629e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00462b30
LEGO_EXPORT void AddOvSav(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00462c00
void FUN_00462c00(struct OverlayParam *param) {
    struct Overlay *current;
    struct Overlay *node;

    current = OverlayList;
    while (current != 0 && current->next != 0) {
        current = current->next;
    }
    if (OverlayILF != 0) {
        node = (struct Overlay *)malloc(0x24);
        node->next = 0;
        if (current != 0) {
            current->next = node;
        } else {
            OverlayList = node;
        }
        node->field_14 = param->field_0;
        node->field_18 = param->field_4;
        *(struct OverlayParam *)node = *param;
    }
}

// FUNCTION: LEGOLAND 0x00462c60
void FUN_00462c60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00462ce0
LEGO_EXPORT void ClearOverlays(void) {
    struct Overlay *current;
    struct Overlay *next;

    current = OverlayList;
    while (current != 0) {
        next = current->next;
        free(current);
        current = next;
    }
    OverlayList = 0;
}

// FUNCTION: LEGOLAND 0x00462d10
LEGO_EXPORT void PlayAppropriateBuildEffect(struct ObjClass *obj, int *coords) { STUB(); }

// FUNCTION: LEGOLAND 0x00462dd0
LEGO_EXPORT void ResetMapAI(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00462e50
void FUN_00462e50(unsigned int index, unsigned int value) {
    unsigned int ecx;
    unsigned int edx;

    ecx = index * 5;
    edx = index + ecx * 2;
    DAT_00832824[edx] = value;
}

// FUNCTION: LEGOLAND 0x00462e70
void FUN_00462e70(unsigned int index, unsigned int value) {
    unsigned int ecx;
    unsigned int edx;

    ecx = index * 5;
    edx = index + ecx * 2;
    DAT_00832828[edx] = value;
}

// FUNCTION: LEGOLAND 0x00462e90
void FUN_00462e90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00462ef0
LEGO_EXPORT void DoMapAI(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004632b0
void FUN_004632b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004633f0
LEGO_EXPORT void RateBlokeOnLeaving(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00463460
void FUN_00463460(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00463520
int FUN_00463520(void) {
    int curr;
    int diff;
    int quotient;

    if (DAT_00832980 == 0) {
        return 0;
    }
    curr = FUN_00499460();
    diff = curr - DAT_00667d58;
    quotient = 0x168 / (int)DAT_00832980;
    if (diff > quotient) {
        DAT_00667d58 = FUN_00499460();
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00463560
void FUN_00463560(void) {
    DAT_00667d54 = GetGameTimer();
    DAT_00667d58 = FUN_00499460();
}

// FUNCTION: LEGOLAND 0x00463580
LEGO_EXPORT void ProcessDamage(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00463680
void FUN_00463680(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004636c0
int FUN_004636c0() { STUB(); }
