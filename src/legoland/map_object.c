#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "bloke_ai.h"
#include "bricks.h"
#include "clipping.h"
#include "controller.h"
#include "draw.h"
#include "gamemain.h"
#include "gamemap.h"
#include "gfx.h"
#include "llidb.h"
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
    unsigned char pad_8[0x10 - 0x8];
    int field_10;
    unsigned int field_14;
    unsigned int field_18;
    struct Overlay *next;
    int field_20;
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
LEGO_EXPORT void RemoveObjectFromMap(unsigned int coords) {
    int cx;
    int cy;
    struct MapElement *tile;
    struct MapObject *obj;
    struct FootprintNode rect;
    struct FootprintNode *next;
    int y;
    int off;
    int count;
    unsigned short *flagsp;
    unsigned char *flag10;

    cx = coords & 0xff;
    cy = (coords >> 8) & 0xff;
    if (cx >= 0 && cx < lpConfig->width && cy >= 0 && cy < lpConfig->height) {
        tile = (struct MapElement *)((int)GameMap[cy] + cx * 0x14);
    } else {
        tile = 0;
    }
    obj = ((struct EditObject *)tile->field_0)->obj;
    if (obj->flags & 0x20000) {
        BGFullUpdate = 1;
    }
    DecrementObjectCount(obj);
    next = &obj->footprint;
    do {
        rect = *next;
        for (y = rect.y0; y <= rect.y1; y++) {
            if (rect.x0 <= rect.x1) {
                off = (cx + rect.x0) * 0x14;
                count = (rect.x1 - rect.x0) + 1;
                do {
                    flagsp = (unsigned short *)((int)GameMap[cy + y] + 0xc + off);
                    *flagsp &= 0xff5f;
                    flag10 = (unsigned char *)((int)GameMap[cy + y] + 0x10 + off);
                    *flag10 &= 0xfc;
                    off += 0x14;
                    count--;
                } while (count != 0);
            }
        }
        next = rect.next;
    } while (rect.next != 0);
    if (DAT_00667cd8 == 0) {
        CalculateMapRenderOrder();
        DAT_00667cdc = 1;
    }
}

// FUNCTION: LEGOLAND 0x0045f220
LEGO_EXPORT void StandardRemoveObject(struct EditObject *editObj, unsigned int coords, struct Cursor *cursor) {
    int cx;
    int cy;
    struct MapElement *tile;
    struct MapObject *obj;
    struct FootprintNode rect;
    struct FootprintNode *next;
    struct MapElement *tile2;
    int x;
    int y;
    int instance;

    cx = coords & 0xff;
    cy = (coords >> 8) & 0xff;
    if (cx >= 0 && cx < lpConfig->width && cy >= 0 && cy < lpConfig->height) {
        tile = (struct MapElement *)((int)GameMap[cy] + cx * 0x14);
    } else {
        tile = 0;
    }
    obj = editObj->obj;
    if (obj->flags & 0x20000) {
        BGFullUpdate = 1;
    }
    AddBricks(GetObjSalvageValue((unsigned int)obj, tile->field_11));
    if ((tile->flags & 0x80) == 0) {
        rect = *(struct FootprintNode *)&cursor->field_1414[0];
        DecrementObjectCount(obj);
        while (1) {
            for (y = rect.y0; y <= rect.y1; y++) {
                for (x = rect.x0; x <= rect.x1; x++) {
                    RestoreBaseMap(cursor->field_1404 + x, cursor->field_1408 + y);
                    SetMapFlags(cursor->field_1404 + x, cursor->field_1408 + y, 0);
                    Set_RFFlags((cursor->field_1404 + x) * 0x100, (cursor->field_1408 + y) * 0x100, 0);
                    *(unsigned int *)((int)GameMap[cursor->field_1408 + y] + (cursor->field_1404 + x) * 0x14) = 0;
                }
            }
            next = rect.next;
            if (next == 0) {
                break;
            }
            rect = *next;
        }
    } else {
        ApplyDestrTileMap(editObj, coords);
        FUN_0045e850((struct ObjNode *)editObj, (int *)&cx);
        RemoveObjectFromMap(coords);
    }
    if (obj->type != 2) {
        ((unsigned char *)&cursor)[0] = (unsigned char)cx;
        ((unsigned char *)&cursor)[1] = (unsigned char)cy;
        if (cx >= 0 && cx < lpConfig->width && cy >= 0 && cy < lpConfig->height) {
            tile2 = (struct MapElement *)((int)GameMap[cy] + cx * 0x14);
        } else {
            tile2 = 0;
        }
        instance = GetInstanceOfClass((unsigned int)((struct EditObject *)tile2->field_0)->obj, (unsigned short *)&cursor);
        if (instance != 0) {
            RemoveInstanceFromList(instance);
            free((void *)instance);
        }
    }
}

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
void FUN_0045f4d0(struct Cursor *cursor) {
    struct Cursor *node;
    struct Cursor *best;

    best = cursor;
    for (node = cursor; node != 0; node = (struct Cursor *)node->field_1830) {
        node->field_1828 |= 8;
        if (node->field_140c < best->field_140c) {
            best = node;
        }
    }
    if ((int)FUN_0045f4b0(best) == 0) {
        for (; cursor != 0; cursor = (struct Cursor *)cursor->field_1830) {
            FUN_0045f480(cursor, best->field_1410);
        }
    }
}

// FUNCTION: LEGOLAND 0x0045f540
struct Cursor *FUN_0045f540(struct Cursor *cursor) {
    struct Cursor *node;
    struct Cursor *last;

    node = cursor;
    if (cursor == 0) {
        return 0;
    }
    do {
        last = node;
        if ((last->field_1828 & 0x1000) != 0) {
            return last;
        }
        node = (struct Cursor *)last->field_1830;
    } while (node != 0);
    FUN_0045f460(&PathCursor);
    PathCursor.field_1404 = cursor->field_1404;
    PathCursor.field_1408 = cursor->field_1408;
    PathCursor.field_1414[0] = cursor->field_1414[0] - 1;
    PathCursor.field_1414[1] = cursor->field_1414[1] - 1;
    PathCursor.field_1414[2] = cursor->field_1414[2] + 1;
    PathCursor.field_1414[3] = cursor->field_1414[3] + 1;
    PathCursor.field_1828 = 0x1008;
    PathCursor.field_1830 = 0;
    last->field_1830 = (unsigned int)&PathCursor;
    return &PathCursor;
}

struct CursorPts {
    /* 0x0000 */ unsigned short count;
    /* 0x0002 */ unsigned short xpts[0x400];
    /* 0x0802 */ unsigned short ypts[0x400];
    /* 0x1002 */ unsigned char fpts[0x400];
};

// FUNCTION: LEGOLAND 0x0045f5f0
LEGO_EXPORT void BuildCursorPtr(struct Cursor *cursor, unsigned int param_2, int param_3) {
    struct CursorPts *pts;
    struct TileSprite *sprite;
    short size;
    unsigned char fbits;
    int built;
    struct FootprintNode rect;
    int x;
    int y;
    short half2;
    short half;

    while (1) {
        rect = *(struct FootprintNode *)&cursor->field_1414[0];
        if (param_3 != 0) {
            FUN_0045f540(cursor);
        }
        sprite = (struct TileSprite *)TileSpriteArray[DAT_00667ca4];
        size = sprite->size;
        pts = (struct CursorPts *)cursor;
        pts->count = 0;
        built = FUN_0045f4b0(cursor);
        fbits = (unsigned char)(((built != 0) + 1) * 4 - 4);
        *(unsigned char *)((char *)cursor + 0x1428) = fbits;
        for (y = rect.y0; y <= rect.y1; y++) {
            if (rect.x0 <= rect.x1) {
                half2 = (short)((short)(size * 2) / 2);
                half = (short)(size / 2);
                x = rect.x0;
                do {
                    unsigned short ux = (unsigned short)(((short)x - (short)y) * half2);
                    unsigned short uy = (unsigned short)(((short)y + (short)x) * half);
                    if (x == rect.x0 && (cursor->field_1828 & 0x100) == 0) {
                        pts->xpts[pts->count] = (unsigned short)(half2 + ux);
                        pts->ypts[pts->count] = uy;
                        pts->fpts[pts->count] = fbits | 1;
                        pts->count++;
                    }
                    if (x == rect.x1 && (cursor->field_1828 & 0x200) == 0) {
                        pts->xpts[pts->count] = (unsigned short)(size * 2 + ux);
                        pts->ypts[pts->count] = (unsigned short)(half + uy);
                        pts->fpts[pts->count] = fbits | 1;
                        pts->count++;
                    }
                    if (y == rect.y0 && (cursor->field_1828 & 0x40) == 0) {
                        pts->xpts[pts->count] = (unsigned short)(half2 + ux);
                        pts->ypts[pts->count] = uy;
                        pts->fpts[pts->count] = fbits | 2;
                        pts->count++;
                    }
                    if (y == rect.y1 && (cursor->field_1828 & 0x80) == 0) {
                        pts->xpts[pts->count] = ux;
                        pts->ypts[pts->count] = (unsigned short)(half + uy);
                        pts->fpts[pts->count] = fbits | 2;
                        pts->count++;
                    }
                    x++;
                } while (x <= rect.x1);
            }
        }
        cursor = (struct Cursor *)cursor->field_1830;
        if (cursor == 0) {
            break;
        }
        param_3 = 0;
    }
}

// FUNCTION: LEGOLAND 0x0045f810
LEGO_EXPORT void ValidateCursor(struct Cursor *cursor, unsigned int param) {
    struct Cursor *node;
    struct FootprintNode *rect;
    struct MapElement *tile;
    struct Obj0c *gtile;
    int people;
    int x;
    int y;
    RECT inter;
    RECT box;
    RECT bounds;

    if (FUN_0045ead0((struct ObjState *)param) != 0 && (cursor->field_1828 & 0x4000) == 0) {
        FUN_0045f540(cursor);
    }
    FUN_0045f460(cursor);
    node = cursor;
    while (1) {
        if (node == 0) {
            FUN_0045f4d0(cursor);
            return;
        }
        if ((node->field_1828 & 0x2000) == 0) {
            for (rect = (struct FootprintNode *)&node->field_1414[0]; rect != 0; rect = rect->next) {
                bounds.top = 0;
                bounds.left = 0;
                bounds.bottom = lpConfig->height;
                bounds.right = lpConfig->height;
                box.top = rect->y0 + node->field_1408;
                box.bottom = node->field_1408 + rect->y1;
                box.left = rect->x0 + node->field_1404;
                box.right = rect->x1 + node->field_1404;
                if (IntersectRect(&inter, &box, &bounds) != 0) {
                    people = CheckForPeople(&inter);
                    if (people == -1) {
                        FUN_0045f480(node, 4);
                    } else if (people == 1) {
                        FUN_0045f480(node, 3);
                    }
                }
                for (y = rect->y0; y <= rect->y1; y++) {
                    for (x = rect->x0; x <= rect->x1; x++) {
                        int tx = node->field_1404 + x;
                        int ty = node->field_1408 + y;
                        if (tx < 0 || lpConfig->width <= tx || ty < 0 || lpConfig->height <= ty ||
                            (tile = (struct MapElement *)((int)GameMap[ty] + tx * 0x14)) == 0) {
                            FUN_0045f480(node, 7);
                        } else {
                            unsigned int objval;
                            int run_eab0 = 1;
                            if (tile->flags & 0x40) {
                                FUN_0045f480(node, 1);
                            }
                            gtile = (struct Obj0c *)tile->field_0;
                            if ((tile->flags & 0xa8) == 0 || (objval = (unsigned int)gtile->field_c) == (unsigned int)DAT_007fd624) {
                                objval = 0;
                            } else if (objval == 0 || (((struct MapObject *)objval)->flags & 0x200000) != 0) {
                                /* fall through to eab0 with objval kept */
                            } else {
                                FUN_0045f480(node, 10);
                                run_eab0 = 0;
                            }
                            if (run_eab0 && FUN_0045eab0((struct ObjFlags *)param) == 0) {
                                if (objval != 0) {
                                    FUN_0045f480(node, 6);
                                }
                                if (*((unsigned char *)&tile->flags + 1) & 8) {
                                    FUN_0045f480(node, 5);
                                }
                            }
                            if (FUN_0045eaf0((struct ObjData *)param) == 0) {
                                if ((tile->flags & 0x10) || (tile->field_10 & 1)) {
                                    FUN_0045f480(node, 8);
                                }
                            } else if ((tile->flags & 0x10) && (tile->field_10 & 1) == 0) {
                                FUN_0045f480(node, 9);
                            }
                        }
                    }
                }
            }
        }
        node = (struct Cursor *)node->field_1830;
    }
}

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
void FUN_0045fad0(int *surface, int dir, int px, int py, unsigned char *color, int count) {
    POINT pt;
    int flip;
    unsigned short c1;
    unsigned short c2;
    int step;
    BOOL in;
    unsigned short *dst;
    unsigned short *dst2;
    unsigned int frame;
    int y;
    unsigned short pattern[16];

    y = py;
    flip = 0;
    c1 = (unsigned short)GetNearestColour(color[0], color[1], color[2]);
    c2 = (unsigned short)GetNearestColour(color[4], color[5], color[6]);
    frame = FrameNumber;
    pattern[12] = c1;
    pattern[13] = c1;
    pattern[14] = c1;
    pattern[15] = c1;
    if (surface[5] == 2) {
        dst = (unsigned short *)(surface[0] * py + surface[3] + px * 2);
        dst2 = (unsigned short *)((int)dst - surface[0]);
        pattern[0] = c2;
        pattern[1] = c2;
        pattern[2] = c2;
        pattern[3] = c2;
        pattern[4] = c2;
        pattern[5] = c2;
        pattern[6] = c2;
        pattern[7] = c2;
        pattern[8] = c2;
        pattern[9] = c2;
        pattern[10] = c2;
        pattern[11] = c2;
        if (dir == 0) {
            for (; count != 0; count--) {
                pt.y = py;
                pt.x = px;
                in = PtInRect((RECT *)&SPRITE_ClipRect, pt);
                if (in != 0) {
                    unsigned short c = pattern[frame & 0xf];
                    *dst = c;
                    if (px > 0) {
                        dst[-1] = c;
                    }
                }
                py--;
                dst = (unsigned short *)((int)dst - surface[0]);
                frame++;
            }
        } else {
            if (dir == 1) {
                step = -1;
            } else if (dir == 2) {
                step = 1;
            } else {
                return;
            }
            count++;
            if (count != 0) {
                do {
                    unsigned short *cur;
                    pt.y = y;
                    pt.x = px;
                    in = PtInRect((RECT *)&SPRITE_ClipRect, pt);
                    if (in != 0) {
                        unsigned short c = pattern[frame & 0xf];
                        *dst = c;
                        if (y > 0) {
                            *dst2 = c;
                        }
                    }
                    px += step;
                    cur = dst + step;
                    dst = cur;
                    dst2 += step;
                    if (flip) {
                        y++;
                        dst = (unsigned short *)((int)cur + surface[0]);
                        dst2 = cur;
                    }
                    flip ^= 1;
                    frame++;
                    count--;
                } while (count != 0);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0045fca0
void FUN_0045fca0(int *surface, int dir, int px, int py, unsigned char *color, int count) {
    POINT pt;
    int flip;
    unsigned short c1;
    unsigned short c2;
    int step;
    BOOL in;
    short *dst;
    short *dst2;
    unsigned int frame;
    short val;
    unsigned short pattern[32];
    int i;

    flip = 0;
    c1 = (unsigned short)GetNearestColour(color[0], color[1], color[2]);
    c2 = (unsigned short)GetNearestColour(color[4], color[5], color[6]);
    frame = FrameNumber;
    for (i = 0; i < 12; i++) {
        pattern[i] = 0;
    }
    for (i = 16; i < 28; i++) {
        pattern[i] = 0;
    }
    if (surface[5] == 2) {
        dst = (short *)(surface[0] * py + surface[3] + px * 2);
        dst2 = (short *)((int)dst - surface[0]);
        pattern[29] = c2;
        pattern[30] = c2;
        pattern[31] = c2;
        pattern[12] = c1;
        pattern[13] = c1;
        pattern[14] = c1;
        pattern[15] = c1;
        if (dir == 0) {
            for (; count != 0; count--) {
                pt.y = py;
                pt.x = px;
                in = PtInRect((RECT *)&SPRITE_ClipRect, pt);
                if (in != 0) {
                    val = (short)pattern[frame & 0x1f];
                    if (val != 0) {
                        *dst = val;
                    }
                    if (px > 0 && val != 0) {
                        dst[-1] = val;
                    }
                }
                py--;
                dst = (short *)((int)dst - surface[0]);
                frame++;
            }
        } else {
            if (dir == 1) {
                step = -1;
            } else if (dir == 2) {
                step = 1;
            } else {
                return;
            }
            count++;
            if (count != 0) {
                do {
                    short *cur;
                    pt.y = py;
                    pt.x = px;
                    in = PtInRect((RECT *)&SPRITE_ClipRect, pt);
                    if (in != 0) {
                        val = (short)pattern[frame & 0x1f];
                        if (val != 0) {
                            *dst = val;
                        }
                        if (py > 0 && val != 0) {
                            *dst2 = val;
                        }
                    }
                    px += step;
                    cur = dst + step;
                    dst2 += step;
                    dst = cur;
                    if (flip) {
                        py++;
                        dst = (short *)((int)cur + surface[0]);
                        dst2 = cur;
                    }
                    flip ^= 1;
                    frame++;
                    count--;
                } while (count != 0);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0045ff00
LEGO_EXPORT void RenderCursor(struct Cursor *cursor) {
    struct CursorPts *pts;
    struct FootprintNode *nextp;
    struct FootprintNode rect;
    struct MapElement *tile;
    struct Sprite *sprite;
    struct ObjBox *editing;
    short size;
    int code;
    unsigned int color;
    unsigned char *pat;
    unsigned char fbits;
    int bx;
    int by;
    int x;
    int y;
    int i;
    int clip[4];
    int tilept[2];
    int screen[2];
    struct ClipRect saved_clip;
    struct VideoArg surf;

    clip[0] = lpConfig->field_20;
    clip[1] = lpConfig->field_22;
    clip[2] = lpConfig->field_10 + lpConfig->field_20;
    clip[3] = lpConfig->field_12 + lpConfig->field_22;
    GetClipping(&saved_clip);
    SetClipping(clip);
    nextp = (struct FootprintNode *)&cursor->field_1414[0];
    do {
        rect = *nextp;
        if ((cursor->field_1828 & 0x10) == 0) {
            for (y = rect.y0; y <= rect.y1; y++) {
                for (x = rect.x0; x <= rect.x1; x++) {
                    tilept[0] = cursor->field_1404 + x;
                    tilept[1] = cursor->field_1408 + y;
                    GetTileBounds((struct Point *)tilept, screen);
                    if ((cursor->field_1828 & 6) == 0) {
                        if (tilept[0] < 0 || lpConfig->width <= tilept[0] || tilept[1] < 0 ||
                            lpConfig->height <= tilept[1] ||
                            (tile = (struct MapElement *)((int)GameMap[tilept[1]] + tilept[0] * 0x14)) == 0 ||
                            (tile->flags & 0x8f8) != 0) {
                            color = 0xff0000;
                            sprite = (struct Sprite *)((struct Sprite **)TileSpriteArray)[(DAT_00805f48 & 0xff) + *(int *)DAT_00801a6c];
                        } else {
                            color = 0;
                            if ((*(unsigned char *)((char *)cursor + 0x1428) & 0xc) == 0) {
                                sprite = (struct Sprite *)((struct Sprite **)TileSpriteArray)[(DAT_00801b20 & 0xff) + *(int *)DAT_00801a6c];
                            } else {
                                sprite = (struct Sprite *)((struct Sprite **)TileSpriteArray)[(DAT_008003f8 & 0xff) + *(int *)DAT_00801a6c];
                            }
                        }
                    } else {
                        color = 0;
                        sprite = (struct Sprite *)((struct Sprite **)TileSpriteArray)[(DAT_0080ff60 & 0xff) + *(int *)DAT_00801a6c];
                    }
                    PrintSprite(sprite, screen[0], screen[1], color, 0);
                }
            }
        }
        nextp = rect.next;
    } while (nextp != 0);
    if (GetVideoSurface(&surf) == 0) {
        return;
    }
    pts = (struct CursorPts *)cursor;
    size = ((struct TileSprite *)TileSpriteArray[DAT_00667ca4])->size;
    for (i = 0; i < (int)pts->count; i++) {
        GetTileBounds((struct Point *)&cursor->field_1404, screen);
        bx = (short)pts->xpts[i] + screen[0];
        by = (short)pts->ypts[i] + screen[1];
        fbits = pts->fpts[i];
        if (cursor->field_1828 & 4) {
            pat = DAT_004b95cc;
        } else if (cursor->field_1828 & 2) {
            pat = DAT_004b95c4;
        } else if ((fbits & 0xc) == 0) {
            pat = DAT_004b95dc;
        } else {
            pat = DAT_004b95d4;
        }
        if ((cursor->field_1828 & 0x20) == 0) {
            if ((fbits & 3) == 0) {
                FUN_0045fad0((int *)&surf, 0, bx, by, pat, lpConfig->field_18);
            } else if ((fbits & 3) == 1) {
                FUN_0045fad0((int *)&surf, 1, bx, by, pat, size);
            } else if ((fbits & 3) == 2) {
                FUN_0045fad0((int *)&surf, 2, bx, by, pat, size);
            }
        } else if ((fbits & 3) == 0) {
            FUN_0045fca0((int *)&surf, 0, bx, by, pat, lpConfig->field_18);
        } else if ((fbits & 3) == 1) {
            FUN_0045fca0((int *)&surf, 1, bx, by, pat, size);
        } else if ((fbits & 3) == 2) {
            FUN_0045fca0((int *)&surf, 2, bx, by, pat, size);
        }
    }
    SetClipping((int *)&saved_clip);
    if (cursor->field_1830 != 0) {
        RenderCursor((struct Cursor *)cursor->field_1830);
    }
    if ((cursor->field_1828 & 0x400) == 0) {
        return;
    }
    editing = (struct ObjBox *)DAT_008119b8;
    if (FUN_0045e620(editing) == 0) {
        return;
    }
    if (EditMode != 1) {
        return;
    }
    tilept[0] = ((struct ObjBox *)DAT_008119b8)->field_c + cursor->field_1404;
    tilept[1] = ((struct ObjBox *)DAT_008119b8)->field_10 + cursor->field_1408;
    GetTileBounds((struct Point *)tilept, screen);
    code = FUN_0045e6b0((struct ObjBox *)DAT_008119b8);
    switch (code) {
    case 1:
        sprite = DAT_00667c8c;
        break;
    case 2:
        sprite = DAT_00667c94;
        break;
    case 4:
        sprite = DAT_00667c90;
        break;
    case 8:
        sprite = DAT_00667c88;
        break;
    default:
        goto after_first;
    }
    PrintSprite(sprite, screen[0], screen[1], 0, 0);
after_first:
    if ((cursor->field_1828 & 0x800) != 0 && FUN_0045e690((struct ObjInfo *)DAT_008119b8) != 0) {
        tilept[0] = ((signed char *)DAT_008119b8)[0x24] + cursor->field_1404;
        tilept[1] = ((signed char *)DAT_008119b8)[0x25] + cursor->field_1408;
        GetTileBounds((struct Point *)tilept, screen);
        code = FUN_0045e710((struct ObjBox *)DAT_008119b8);
        switch (code) {
        case 0x10:
            PrintSprite(DAT_00667c8c, screen[0], screen[1], 0, 0);
            break;
        case 0x20:
            PrintSprite(DAT_00667c94, screen[0], screen[1], 0, 0);
            return;
        case 0x40:
            PrintSprite(DAT_00667c90, screen[0], screen[1], 0, 0);
            return;
        case 0x80:
            PrintSprite(DAT_00667c88, screen[0], screen[1], 0, 0);
            return;
        }
    }
}

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
void FUN_00460560(int index) {
    struct Overlay *ov;
    int coord[2];
    int sc[2];
    int row;
    int inner;
    int outer;

    ov = (struct Overlay *)OverlayList;
    if (OverlayList == 0) {
        DAT_00832be0[index] = 1;
        return;
    }
    do {
        if (ov->field_10 >> 8 == index + 1) {
            sc[0] = (lpConfig->field_20 - (ScrollX >> 8)) + ov->field_14;
            sc[1] = (lpConfig->field_22 - (ScrollY >> 8)) + ov->field_18;
            ScreenToMapRef(sc, coord, 0);
            if ((char)ov->field_10 == 0) {
                coord[1] = coord[1] + 0xf;
                outer = 10;
                do {
                    inner = 2;
                    row = coord[1];
                    do {
                        struct MapElement *t;
                        coord[1] = row;
                        t = (struct MapElement *)((int)GameMap[coord[0]] + coord[1] * 0x14);
                        t->field_0 = ((struct MapObject *)DAT_007fd624)->field_c4;
                        t->field_10 |= 1;
                        t->field_10 &= 0xfd;
                        *(unsigned char *)&t->flags |= 0x48;
                        t->flags &= 0x7fff;
                        t->field_8 = *(unsigned short *)PathSprite;
                        *((unsigned char *)&t->field_4) = (unsigned char)coord[1];
                        *((unsigned char *)&t->field_4 + 1) = (unsigned char)coord[0];
                        AddPathTile((struct Point *)coord, *(unsigned short *)PathSprite);
                        inner--;
                        row = coord[1] + 1;
                    } while (inner != 0);
                    coord[1]--;
                    coord[0]++;
                    outer--;
                } while (outer != 0);
            } else {
                coord[1] = coord[1] + 10;
                coord[0] = coord[0] + 6;
                outer = 2;
                do {
                    inner = 10;
                    row = coord[1];
                    do {
                        struct MapElement *t;
                        coord[1] = row;
                        t = (struct MapElement *)((int)GameMap[coord[0]] + coord[1] * 0x14);
                        t->field_0 = ((struct MapObject *)DAT_007fd624)->field_c4;
                        t->field_10 |= 1;
                        t->field_10 &= 0xfd;
                        *(unsigned char *)&t->flags |= 0x48;
                        t->flags &= 0x7fff;
                        t->field_8 = *(unsigned short *)PathSprite;
                        *((unsigned char *)&t->field_4) = (unsigned char)coord[1];
                        *((unsigned char *)&t->field_4 + 1) = (unsigned char)coord[0];
                        AddPathTile((struct Point *)coord, *(unsigned short *)PathSprite);
                        inner--;
                        row = coord[1] + 1;
                    } while (inner != 0);
                    coord[1] = coord[1] - 9;
                    coord[0]++;
                    outer--;
                } while (outer != 0);
            }
        }
        ov = ov->next;
    } while (ov != 0);
    DAT_00832be0[index] = 1;
}

// FUNCTION: LEGOLAND 0x004608c0
void FUN_004608c0(int *param_1, int *param_2) {
    struct Overlay *ov;
    int dx;
    int dy;
    int size;
    int dbl;
    int sx;
    int half_x;
    int half_y;
    int rem_x;
    int row;
    int phase;
    int col;
    int sxpix;
    int sypix;
    struct MapElement *tile;
    unsigned short sprite_id;
    int draw_x;
    int draw_y;
    int sprite;
    int frame;
    int k;

    ov = (struct Overlay *)OverlayList;
    dx = param_1[0] - param_2[0];
    dy = param_1[1] - param_2[1];
    SetClipping(param_2);
    half_y = (int)((struct TileSprite *)TileSpriteArray[DAT_00667ca4])->size;
    dbl = (int)(short)(((struct TileSprite *)TileSpriteArray[DAT_00667ca4])->size * 2);
    sx = param_1[0] / dbl;
    half_x = (dbl + 1) >> 1;
    rem_x = (half_y + 1) >> 1;
    param_1 = (int *)(param_1[0] % dbl);
    row = (param_1[1] - rem_x) / half_y;
    phase = (param_1[1] - rem_x) % half_y;
    sxpix = row + -3 + sx;
    sypix = row - sx;
    {
        char which = (char)((half_x <= (int)param_1) + 1);
        if (rem_x < phase) {
            which = (char)((half_x <= (int)param_1) + 3);
        }
        switch (which) {
        case 1:
            if ((int)param_1 < half_x + phase * -2) {
                param_1 = (int *)((int)param_1 + half_x);
                sxpix = sxpix - 1;
                phase = phase + rem_x;
            }
            break;
        case 2:
            if (half_x + phase * 2 <= (int)param_1) {
                sypix = sypix + -1;
                param_1 = (int *)((int)param_1 - half_x);
                phase = phase + rem_x;
            }
            break;
        case 3:
            if (half_x + (phase - half_y) * 2 <= (int)param_1) {
                break;
            }
            sypix = sypix + 1;
            size = half_x;
            param_1 = (int *)((int)param_1 + size);
            phase = phase - rem_x;
            break;
        case 4:
            if ((int)param_1 < half_x + (half_y - phase) * 2) {
                break;
            }
            sxpix = sxpix + 1;
            size = -half_x;
            param_1 = (int *)((int)param_1 + size);
            phase = phase - rem_x;
            break;
        }
    }
    {
        int ybot = param_2[3] + half_y * 2;
        int xright = param_2[2];
        PushRenderingStatusAndLockVideoSurface();
        draw_y = (param_2[1] + half_y * -2) - phase;
        if (draw_y < ybot) {
            rem_x = rem_x + draw_y;
            do {
                int saved_row = sypix;
                int saved_col = sxpix;
                for (col = (param_2[0] + dbl * -2) - (int)param_1; col < xright + dbl * 2; col = col + dbl) {
                    if ((int)sxpix < 0 || lpConfig->width <= (int)sxpix || sypix < 0 || lpConfig->height <= sypix) {
                        tile = 0;
                    } else {
                        tile = (struct MapElement *)((int)GameMap[sypix] + sxpix * 0x14);
                        if (tile != 0 && (sprite_id = tile->field_8) != 0) {
                            if ((tile->flags & 3) == 0 || (tile->flags & 8) == 0 || tile->field_0 == 0) {
                                FUN_00485f00((struct Sprite *)TileSpriteArray[sprite_id], col, draw_y);
                                if (FUN_0045ce10((struct MapTile *)tile) != 0) {
                                    FUN_00460e90((int *)&sxpix, col, draw_y, 0);
                                }
                            } else if (*(int *)(*(int *)(tile->field_0 + 0xc) + 0xa0) == 0) {
                                FUN_00485f00((struct Sprite *)TileSpriteArray[sprite_id], col, draw_y);
                            }
                        }
                    }
                    sxpix = sxpix + 1;
                    if (tile == 0) {
                        if ((int)sxpix >= 0 && (int)sxpix < lpConfig->width && sypix >= 0 && sypix < lpConfig->height &&
                            (tile = (struct MapElement *)((int)GameMap[sypix] + sxpix * 0x14)) != 0) {
                            goto draw_lower;
                        }
                    } else {
                        if ((unsigned int)sxpix == lpConfig->width) {
                            break;
                        }
                        tile = tile + 1;
                        if (tile == 0) {
                            goto check_lower;
                        }
                    draw_lower:
                        sprite_id = tile->field_8;
                        if (sprite_id != 0) {
                            if ((tile->flags & 3) == 0 || (tile->flags & 8) == 0 || tile->field_0 == 0) {
                                FUN_00485f00((struct Sprite *)TileSpriteArray[sprite_id], col + half_x, rem_x);
                                if (FUN_0045ce10((struct MapTile *)tile) != 0) {
                                    FUN_00460e90((int *)&sxpix, col + half_x, rem_x, 0);
                                }
                            } else if (*(int *)(*(int *)(tile->field_0 + 0xc) + 0xa0) == 0) {
                                FUN_00485f00((struct Sprite *)TileSpriteArray[sprite_id], col, draw_y);
                            }
                        }
                    }
                check_lower:
                    sypix = sypix + -1;
                }
                sxpix = saved_col + 1;
                sypix = saved_row + 1;
                draw_y = draw_y + half_y;
                rem_x = rem_x + half_y;
            } while (draw_y < ybot);
        }
    }
    while (ov != 0) {
        if (ov->field_20 != 0) {
            FUN_00485f00((struct Sprite *)ov->field_20, ov->field_14 - dx, ov->field_18 - dy);
            frame = ov->field_10;
            if ((char)((unsigned int)frame >> 8) != 0 && (&DAT_00832bdc)[(int)(unsigned int)frame >> 8] != 0) {
                if ((frame & 0xff) == 0) {
                    draw_x = ov->field_14 - dx;
                    draw_y = ov->field_18 - dy;
                    FUN_00485f00((struct Sprite *)*(int *)(*(int *)((int)DAT_00667cb0 + 8) + ((frame + 2) & 0xff) * 4),
                                 draw_x + DAT_004b9218, draw_y + DAT_004b921c);
                    k = (ov->field_10 + 4) & 0xff;
                    if (k < *(int *)((int)DAT_00667cb0 + 4)) {
                        sprite = *(int *)(*(int *)((int)DAT_00667cb0 + 8) + k * 4);
                        SortSprite((struct Sprite *)sprite, draw_x + DAT_00805f40, draw_y + DAT_00805f44,
                                   *(short *)(sprite + 0x16) + draw_y + DAT_00805f44, 0, 0);
                    }
                } else if ((frame & 0xff) == 1) {
                    draw_x = ov->field_14 - dx;
                    draw_y = ov->field_18 - dy;
                    FUN_00485f00((struct Sprite *)*(int *)(*(int *)((int)DAT_00667cb0 + 8) + ((frame + 2) & 0xff) * 4),
                                 draw_x + DAT_004b9210, draw_y + DAT_004b9214);
                    k = (ov->field_10 + 4) & 0xff;
                    if (k < *(int *)((int)DAT_00667cb0 + 4)) {
                        sprite = *(int *)(*(int *)((int)DAT_00667cb0 + 8) + k * 4);
                        SortSprite((struct Sprite *)sprite, draw_x + DAT_00801a60, draw_y + DAT_00801a64,
                                   *(short *)(sprite + 0x16) + draw_y + DAT_00801a64, 0, 0);
                    }
                }
            }
        }
        ov = ov->next;
    }
    PopRenderingStatus();
}

// FUNCTION: LEGOLAND 0x00460e00
void FUN_00460e00(void) {
    int sx;
    int sy;
    int rect[2];
    int clip[4];

    sx = ScrollX >> 8;
    sy = ScrollY >> 8;
    clip[0] = lpConfig->field_20;
    clip[1] = lpConfig->field_22;
    clip[2] = lpConfig->field_10 + clip[0];
    clip[3] = lpConfig->field_12 + clip[1];
    rect[0] = sx;
    rect[1] = sy;
    FUN_004608c0(rect, clip);
    DAT_004b95ec = sy;
    DAT_004b95e8 = sx;
    DAT_00667cd0 = 0;
    DAT_00667cd4 = 0;
    BGFullUpdate = 0;
}

// FUNCTION: LEGOLAND 0x00460e90
void FUN_00460e90(int *coords, unsigned int x, unsigned int y, unsigned int param_4) {
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
void FUN_004610f0(int *param_1, int *param_2) {
    short size;
    int dbl;
    int x0;
    struct Point coord;
    struct Point row_start;
    int out[2];
    int ix;
    int iy;
    int half_x;
    short half_y;

    size = ((struct TileSprite *)TileSpriteArray[DAT_00667ca4])->size;
    dbl = (short)(size * 2);
    x0 = param_1[0];
    param_1[0] = x0 - dbl;
    coord = PlayfieldToMap(x0 - dbl, param_1[1]);
    GetTileBounds(&coord, out);
    if (out[1] < param_2[3]) {
        half_x = (short)((dbl + 1) >> 1);
        iy = out[1];
        ix = out[0];
        do {
            int saved_ix = ix;
            struct Point saved = coord;
            if (ix < half_x + param_2[2]) {
                half_y = (short)((size + 1) >> 1);
                row_start = coord;
                do {
                    FUN_00461080((int *)&coord, ix, iy, 0);
                    coord.y = coord.y + 1;
                    FUN_00461080((int *)&coord, ix - half_x, half_y + iy, 0);
                    coord.x = coord.x + 1;
                    coord.y = coord.y - 2;
                    ix = ix + dbl;
                    saved_ix = out[0];
                    saved = row_start;
                } while (ix < half_x + param_2[2]);
            }
            coord = saved;
            coord.x = coord.x + 1;
            coord.y = coord.y + 1;
            iy = iy + size;
            ix = saved_ix;
        } while (iy < param_2[3]);
    }
}

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
void FUN_00461290(int param_1, int param_2, int param_3, int param_4) {
    int v10;
    int v4;
    int vc;
    int v8;
    int v14;
    int v18;
    int v20;
    int iv4;
    int dim[2];
    int tile;
    int wfull;
    int hhalf;
    int whalf;
    int hneg;
    int ix;
    int iy;
    int adj;

    v10 = DAT_004b95f4 >> 1;
    v4 = DAT_004b95f8 >> 1;
    vc = DAT_004b95fc >> 1;
    v8 = DAT_004b9600 >> 1;
    v14 = DAT_004b960c >> 1;
    v18 = DAT_004b9610 >> 1;
    v20 = DAT_004b9608 >> 1;
    iv4 = DAT_004b9604 >> 1;
    GetTileDimensions(dim, &tile);
    dim[0] = dim[0] * 0x80;
    wfull = lpConfig->width * dim[0];
    hhalf = lpConfig->height * (dim[0] >> 1);
    whalf = lpConfig->width * (dim[0] >> 1);
    hneg = -(lpConfig->height * dim[0]);
    iv4 = iv4 + (wfull - param_1);
    ix = param_3 + ScrollX;
    if (iv4 < param_3 + ScrollX) {
        ix = iv4;
    }
    if (ix < hneg - v20) {
        ix = hneg - v20;
    }
    v18 = ((hhalf + whalf) - param_2) + v18;
    iy = param_4 + ScrollY;
    if (v18 < param_4 + ScrollY) {
        iy = v18;
    }
    if (iy < -v14) {
        iy = -v14;
    }
    if (iy < whalf && 0 < param_1 + ix && (adj = ((ix + iy * -2) - v10) + param_1, 0 < adj)) {
        ix = ix - (adj >> 1);
        iy = iy + (adj >> 2);
    }
    if (iy < hhalf && ix < 0 && (vc = (iy * -2 - ix) - vc, 0 < vc)) {
        ix = ix + (vc >> 1);
        iy = iy + (vc >> 2);
    }
    if (whalf < param_2 + iy && hneg + wfull < param_1 + ix &&
        (param_1 = ((((iy - whalf) + param_2) * 2 - wfull) - v8) + ix + param_1, 0 < param_1)) {
        ix = ix - (param_1 >> 1);
        iy = iy - (param_1 >> 2);
    }
    if (hhalf < param_2 + iy && ix < hneg + wfull &&
        (hneg = ((((iy - hhalf) + param_2) * 2 - ix) - v4) + hneg, 0 < hneg)) {
        ix = ix + (hneg >> 1);
        iy = iy - (hneg >> 2);
    }
    ScrollX = ix - param_3;
    ScrollY = iy - param_4;
}

// FUNCTION: LEGOLAND 0x004614a0
LEGO_EXPORT void ProcessScrolling(unsigned int a, unsigned int b) {
    ScrollX = ScrollX + a;
    ScrollY = ScrollY + b;
    FUN_00461290(lpConfig->field_10 << 8, lpConfig->field_12 << 8, 0, 0);
}

// FUNCTION: LEGOLAND 0x004614f0
LEGO_EXPORT void MouseScrollMap(void) {
    if ((int)DAT_00813a44 < (int)(unsigned int)lpConfig->field_4) {
        if (-(int)(unsigned int)lpConfig->field_c < ScrollSpeedX) {
            ScrollSpeedX = ScrollSpeedX - lpConfig->field_8;
        }
    } else if ((int)((unsigned int)lpConfig->field_0 - (unsigned int)lpConfig->field_4) < (int)DAT_00813a44) {
        if (ScrollSpeedX < (int)(unsigned int)lpConfig->field_c) {
            ScrollSpeedX = ScrollSpeedX + lpConfig->field_8;
        }
    } else {
        ScrollSpeedX = 0;
    }
    if ((int)DAT_00813a48 < (int)(unsigned int)lpConfig->field_6) {
        if (-(int)(unsigned int)lpConfig->field_e < ScrollSpeedY) {
            ScrollSpeedY = ScrollSpeedY - lpConfig->field_a;
        }
    } else if ((int)((unsigned int)lpConfig->field_2 - (unsigned int)lpConfig->field_6) < (int)DAT_00813a48) {
        if (ScrollSpeedY < (int)(unsigned int)lpConfig->field_e) {
            ScrollSpeedY = ScrollSpeedY + lpConfig->field_a;
        }
    } else {
        ScrollSpeedY = 0;
    }
    ProcessScrolling((int)(LastFrameMS * ScrollSpeedX) / 0x100, (int)(LastFrameMS * ScrollSpeedY) / 0x100);
    DAT_00667d4c = LastFrameMS;
}

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
LEGO_EXPORT unsigned int GetObjectClassAndInstance(int *coords, unsigned short *out) {
    struct MapElement local;
    struct MapElement *tile;

    if (coords[0] >= 0 && coords[0] < lpConfig->width && coords[1] >= 0 && coords[1] < lpConfig->height) {
        tile = (struct MapElement *)((int)GameMap[coords[1]] + coords[0] * 0x14);
        local = *tile;
        if (out != 0) {
            *out = local.field_4;
        }
        if ((local.field_10 & 0x88) != 0 && local.field_0 != 0) {
            return *(unsigned int *)(local.field_0 + 0xc);
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004618d0
void FUN_004618d0(const char *param_1) {
    // STRING: LEGOLAND 0x004b9c14
    if (strcmp(param_1, "CASTLE BRIDGES") == 0) {
        DAT_004b9210 = 4;
        DAT_004b9214 = 0x6d;
        DAT_004b9218 = 0x3b;
        DAT_004b921c = 0x6d;
        DAT_00801a60 = 0;
        DAT_00801a64 = 0;
        DAT_00805f40 = 0;
        DAT_00805f44 = 0;
        return;
    }
    // STRING: LEGOLAND 0x004b9c00
    if (strcmp(param_1, "EXPLORER BRIDGES") == 0) {
        DAT_004b9214 = 0x6a;
        DAT_004b921c = 0x6a;
        DAT_004b9210 = 1;
        DAT_004b9218 = 0x40;
        DAT_00801a60 = 0xe;
        DAT_00801a64 = 0x89;
        DAT_00805f40 = 0x86;
        DAT_00805f44 = 0x89;
        return;
    }
    // STRING: LEGOLAND 0x004b9bf0
    if (strcmp(param_1, "WESTERN BRIDGES") == 0) {
        DAT_004b9210 = 2;
        DAT_004b9214 = 0x5b;
        DAT_004b921c = 0x5b;
        DAT_004b9218 = 0x3e;
        DAT_00801a60 = 0xe;
        DAT_00801a64 = 0x79;
        DAT_00805f40 = 0x85;
        DAT_00805f44 = 0x79;
    }
}

// FUNCTION: LEGOLAND 0x00461a50
LEGO_EXPORT unsigned int LoadBaseMap(unsigned int param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x004629e0
unsigned int FUN_004629e0(void) {
    int row;
    int col;
    int off;
    int count;
    int i;
    struct Element *elem;
    struct Element *elem2;

    if (DAT_00667d50 != 0) {
        FUN_00459880();
        row = 0;
        if (lpConfig->height != 0) {
            do {
                col = 0;
                if (lpConfig->width != 0) {
                    off = 0;
                    do {
                        col = col + 1;
                        off = off + 0x14;
                        *(unsigned short *)(*(int *)((int)GameMap + row * 4) + -8 + off) = 0;
                    } while (col < (int)(unsigned int)lpConfig->width);
                }
                row = row + 1;
            } while (row < (int)(unsigned int)lpConfig->height);
        }
        LLIDB_UnLoadData((unsigned int)DAT_0080140c);
        LLIDB_UnLoadData((unsigned int)DAT_00801410);
        if (DAT_00801404 != 0) {
            LLIDB_UnLoadData((unsigned int)DAT_00801404);
            DAT_00801404 = 0;
        }
        count = LLIDB_GetCount();
        i = 0;
        if (0 < count) {
            do {
                LLIDB_GetElement(i, &elem);
                if ((*(unsigned int *)((char *)elem + 8) & 0x10) != 0 && (*(unsigned int *)((char *)elem + 8) & 1) != 0) {
                    LLIDB_UnLoadData((unsigned int)elem);
                }
                i = i + 1;
            } while (i < count);
        }
        free(DAT_00801a68);
        i = 0;
        if (0 < DAT_00801a74) {
            do {
                LLIDB_FindElementFromDataPtr(*(void **)((char *)DAT_00801a70 + i * 4), (unsigned int *)&elem2, 0);
                *(unsigned int *)((char *)elem2 + 8) = *(unsigned int *)((char *)elem2 + 8) & 0xfffcfff1;
                LLIDB_UnLoadData((unsigned int)elem2);
                i = i + 1;
            } while (i < DAT_00801a74);
        }
        free(DAT_00801a70);
        ClearOverlays();
        FUN_004828f0();
        DAT_00667d50 = 0;
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00462b30
LEGO_EXPORT void AddOvSav(struct OverlayParam *param) {
    struct Overlay *cur;
    struct Overlay *node;
    int tbl;
    int sample;
    int *lls;

    cur = (struct Overlay *)OverlayList;
    while (cur != 0 && cur->next != 0) {
        cur = cur->next;
    }
    if (OverlayILF != 0 && ((char)(param->field_10 >> 8) == 0 || DAT_00667cb0 != 0)) {
        node = (struct Overlay *)malloc(sizeof(struct Overlay));
        node->next = 0;
        if (cur != 0) {
            cur->next = node;
        } else {
            OverlayList = node;
        }
        node->field_14 = param->field_0;
        node->field_18 = param->field_4;
        *(struct OverlayParam *)node = *param;
        tbl = OverlayILF;
        if ((char)(param->field_10 >> 8) != 0) {
            tbl = (int)DAT_00667cb0;
        }
        sample = *(int *)(*(int *)(tbl + 8) + (param->field_10 & 0xff) * 4);
        node->field_20 = sample;
        lls = *(int **)(sample + 8);
        if ((lls[5] == 2 || lls[5] == 3) && 1 < *(short *)(*lls + 0x10)) {
            LLSPlay((struct LLS *)*lls, (unsigned int)lls);
        }
    }
}

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
void FUN_00462c60(void) {
    struct Overlay *node;
    int sample;
    int *lls;

    node = (struct Overlay *)OverlayList;
    while (node != 0) {
        if ((char)(node->field_10 >> 8) == 0) {
            sample = *(int *)(*(int *)(OverlayILF + 8) + (node->field_10 & 0xff) * 4);
            node->field_20 = sample;
            if (sample != 0) {
                lls = *(int **)(sample + 8);
                if ((lls[5] == 2 || lls[5] == 3) && 1 < *(short *)(*lls + 0x10)) {
                    LLSPlay((struct LLS *)*lls, (unsigned int)lls);
                }
            }
        } else if (DAT_00667cb0 != 0) {
            sample = *(int *)(*(int *)((int)DAT_00667cb0 + 8) + (node->field_10 & 0xff) * 4);
            node->field_20 = sample;
            if (sample != 0) {
                lls = *(int **)(sample + 8);
                if ((lls[5] == 2 || lls[5] == 3) && 1 < *(short *)(*lls + 0x10)) {
                    LLSPlay((struct LLS *)*lls, (unsigned int)lls);
                }
            }
        } else {
            node->field_20 = 0;
        }
        node = node->next;
    }
}

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
LEGO_EXPORT void PlayAppropriateBuildEffect(struct ObjClass *obj, int *coords) {
    unsigned int flags;
    int *param;
    int buf[4];

    param = buf;
    if (coords == 0) {
        param = 0;
    } else {
        buf[0] = 2;
        buf[2] = coords[0];
        buf[3] = coords[1];
    }
    flags = ((struct MapObject *)obj)->flags;
    if ((flags & 0x40000) != 0) {
        PlayInstanceOfSample(DAT_004b9230[3], 0, 1, param);
        return;
    }
    if ((flags & 0x80000) == 0) {
        PlayInstanceOfSample(DAT_004b9230[0], 0, 1, param);
        return;
    }
    if ((flags & 0x200000) != 0) {
        PlayInstanceOfSample(DAT_004b9230[0], 0, 1, param);
        return;
    }
    PlayInstanceOfSample(DAT_004b9230[(rand() % 5 + 3) * 3], 0, 1, param);
}

// FUNCTION: LEGOLAND 0x00462dd0
LEGO_EXPORT void ResetMapAI(void) {
    unsigned int *p;
    int i;

    p = &MapStats;
    for (i = 0xfc; i != 0; i--) {
        *p = 0;
        p++;
    }
    DAT_0083291c = 0;
    MapStats = 0x1fff;
    p = (unsigned int *)DAT_00832bb0;
    for (i = 6; i != 0; i--) {
        *p = 0x2020202;
        p++;
    }
    *(unsigned char *)p = 2;
    DAT_00832928 = 0xffffe0c0;
    DAT_0083292c = 0xfffffc18;
    DAT_00832930 = 100;
    DAT_00832934 = 1000;
    DAT_00832938 = 5000;
    DAT_00832bc9 = 0;
    DAT_00832984 = 0;
}

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
void FUN_00462e90(void) {
    DAT_00832824[0] = 0x32;
    DAT_00832854 = 0x32;
    DAT_00832828[0] = 0x14;
    DAT_0083287c = 0;
    DAT_00832880 = 0;
    DAT_008328a8 = 0;
    DAT_008328ac = 0;
    DAT_00832850 = 0x21;
    DAT_008328d4 = 0x21;
    DAT_008328d8 = 0x28;
    DAT_00832900 = 0x21;
    DAT_00832904 = 0x28;
}

// FUNCTION: LEGOLAND 0x00462ef0
LEGO_EXPORT void DoMapAI(void) {
    int i;
    int *p;
    struct MapElement *tile;
    struct MapObject *obj;
    short type;
    unsigned char xb;
    unsigned char yb;
    int r;
    int v1;
    int v2;
    int v3;
    int v4;
    unsigned int x;
    unsigned int *cls;

    if (((unsigned char)DAT_008119a4 & 0x3f) != 0) {
        i = 0;
        x = DAT_00832804;
        do {
            if (DAT_0083280c == 0) {
                p = &DAT_0083282c;
                do {
                    p[1] = 0;
                    p[0] = 0;
                    p[2] = 0;
                    p[3] = 0;
                    p = p + 0xb;
                } while ((int)p < 0x832934);
                DAT_0083280c = DAT_0083280c + 1;
                DAT_00832804 = 0;
                DAT_00832808 = 0;
                x = 0;
            } else if (DAT_0083280c == 1) {
                if ((int)x < 0 || lpConfig->width <= (int)x || DAT_00832808 < 0 || lpConfig->height <= DAT_00832808) {
                    tile = 0;
                } else {
                    tile = (struct MapElement *)((int)GameMap[DAT_00832808] + x * 0x14);
                }
                if ((tile->field_10 & 1) == 0) {
                    if ((tile->flags & 0x88) != 0 && tile->field_0 != 0) {
                        obj = ((struct EditObject *)tile->field_0)->obj;
                        type = obj->type;
                        if (type != 0) {
                            (&DAT_0083282c)[type * 0xb] = (&DAT_0083282c)[type * 0xb] + 1;
                            xb = *((unsigned char *)&tile->field_4);
                            x = DAT_00832804;
                            if (xb == DAT_00832804 && (yb = *((unsigned char *)&tile->field_4 + 1), yb == DAT_00832808) &&
                                (r = FUN_0044f360((int)obj, &xb), x = DAT_00832804, r != 0)) {
                                (&DAT_00832830)[obj->type * 0xb] = (&DAT_00832830)[obj->type * 0xb] + 1;
                                type = obj->type;
                                r = GetObjSalvageValue((unsigned int)obj, tile->field_11);
                                (&DAT_00832834)[type * 0xb] = (&DAT_00832834)[type * 0xb] + r;
                                (&DAT_00832838)[obj->type * 0xb] = (&DAT_00832838)[obj->type * 0xb] + (int)*(short *)((char *)obj + 0x2e);
                                x = DAT_00832804;
                            }
                        }
                    }
                } else {
                    DAT_00832830 = DAT_00832830 + 1;
                    DAT_0083282c = DAT_0083282c + 1;
                    DAT_00832838 = DAT_00832838 + 1;
                }
                DAT_00832804 = x + 1;
                x = DAT_00832804;
                if (lpConfig->width <= (int)DAT_00832804) {
                    DAT_00832808 = DAT_00832808 + 1;
                    DAT_00832804 = 0;
                    x = 0;
                    if (lpConfig->height <= DAT_00832808) {
                        DAT_0083280c = DAT_0083280c + 1;
                    }
                }
            } else if (DAT_0083280c == 2) {
                DAT_0083280c = 0;
                DAT_00832918 = 0;
                p = &DAT_00832820;
                i = 6;
                do {
                    p[0] = p[5];
                    p[-1] = p[3];
                    p[-4] = p[4];
                    p[-2] = p[6];
                    DAT_00832918 = DAT_00832918 + p[-1];
                    p = p + 0xb;
                } while ((int)p < 0x832928);
                DAT_00832818 = DAT_00832818 / 100;
                v1 = (int)(DAT_00832824[0] * DAT_0083281c) / 100;
                if ((int)(DAT_00832828[0] * 100) < v1) {
                    v1 = (int)(DAT_00832828[0] * 100);
                }
                v2 = DAT_00832850 * DAT_00832844;
                if (DAT_00832854 * 100 <= v2) {
                    v2 = DAT_00832854 * 100;
                }
                v3 = DAT_008328d4 * DAT_008328c8;
                if (DAT_008328d8 * 100 <= v3) {
                    v3 = DAT_008328d8 * 100;
                }
                v4 = DAT_00832900 * DAT_008328f4;
                if (DAT_00832904 * 100 <= v4) {
                    v4 = DAT_00832904 * 100;
                }
                DAT_0083291c = (v4 + v1 + v2 + v3) / 100;
                if ((int)DAT_0083291c < (int)DAT_00832924) {
                    DAT_0083291c = DAT_00832924;
                }
                if ((int)DAT_00832920 < (int)DAT_0083291c) {
                    DAT_0083291c = DAT_00832920;
                }
                x = DAT_00832804;
                if ((int)(unsigned int)lpConfig->field_1a < (int)DAT_0083291c) {
                    DAT_0083291c = lpConfig->field_1a;
                }
            }
            i = i + 1;
        } while (i < 0x100);
        return;
    }
    p = &DAT_00832814;
    do {
        if (p != &DAT_00832814) {
            *p = 0;
        }
        p = p + 0xb;
    } while ((int)p < 0x83291c);
    for (cls = (unsigned int *)ObjectClassList; cls != 0; cls = (unsigned int *)*cls) {
        if (*(short *)(cls + 8) != 0 && cls[2] != 0) {
            (&DAT_00832814)[*(short *)(cls + 8) * 0xb] = (&DAT_00832814)[*(short *)(cls + 8) * 0xb] + 1;
        }
    }
}

// FUNCTION: LEGOLAND 0x004632b0
void FUN_004632b0(void) {
    int total;
    int y;
    char **names;
    int *p;
    int prod;
    int cap;
    int capped;
    char buf[504];

    total = 0;
    y = 0x14;
    names = (char **)&DAT_004bb6bc;
    p = (int *)&DAT_00832828[0];
    do {
        prod = p[-4] * p[-1];
        cap = *p * 100;
        capped = prod;
        if (cap <= prod) {
            capped = cap;
        }
        // STRING: LEGOLAND 0x004b9c6c
        sprintf(buf, "[%s x %d]: Cap %d  x %d%% = %.2f (Capped %d) = %.2f", *names, p[-6], p[-4], p[-1],
                (double)((float)prod * DAT_004ab518), *p, (double)((float)capped * DAT_004ab518));
        Print(SPRITE_ClipRect.right, y + SPRITE_ClipRect.top, buf, 2);
        y = y + 0x14;
        total = total + capped;
        p = p + 0xb;
        names = names + 1;
    } while (y < 0x8c);
    // STRING: LEGOLAND 0x004b9c40
    sprintf(buf, "Tot Capacity = %.2f (limit %d - %d) = %d", (double)((float)total * DAT_004ab518),
            DAT_00832924, DAT_00832920, DAT_0083291c);
    Print(SPRITE_ClipRect.right, SPRITE_ClipRect.top + 0x96, buf, 2);
}

// FUNCTION: LEGOLAND 0x004633f0
LEGO_EXPORT void RateBlokeOnLeaving(int param_1) {
    int rating;

    if (param_1 < DAT_00832928) {
        rating = 0;
    } else if (param_1 < DAT_00832930) {
        rating = 1;
    } else if (param_1 < DAT_00832934) {
        rating = 2;
    } else {
        rating = (DAT_00832938 <= param_1) + 3;
    }
    *((char *)DAT_00832bb0 + DAT_00832bc9) = (char)rating;
    DAT_00832bc9 = DAT_00832bc9 + 1;
    if (DAT_00832bc9 == 0x19) {
        DAT_00832bc9 = 0;
    }
}

// FUNCTION: LEGOLAND 0x00463460
void FUN_00463460(struct MapElement *tile, int *coords) {
    unsigned short flags;
    struct MapObject *obj;
    unsigned char threshold;
    int power;

    if (tile->field_11 != 0) {
        flags = tile->flags;
        obj = ((struct EditObject *)tile->field_0)->obj;
        threshold = obj->field_2c;
        tile->flags = flags & 0xfdff;
        threshold = threshold >> 2;
        if (tile->field_11 < threshold) {
            if ((flags & 4) != 0) {
                *((unsigned char *)&tile->flags + 1) |= 2;
                tile->field_11 = threshold;
                return;
            }
            if (lpConfig->field_3c != 0 && (flags & 0x4000) == 0) {
                if (AddRepairOrderForObject((int)obj, coords[0], coords[1]) != 0) {
                    *((unsigned char *)&tile->flags + 1) |= 0x40;
                }
            }
            if ((flags & 0x200) == 0) {
                power = FindObjectsPower(obj);
                if (0 < power && (DAT_00832bd0 = DAT_00832bd0 - power, DAT_00832bd0 < DAT_00832bd4 - (int)DAT_00832bd8)) {
                    FUN_0045a0d0();
                }
            }
            *((unsigned char *)&tile->flags + 1) |= 2;
        }
    }
}

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
LEGO_EXPORT void ProcessDamage(void) {
    short coords;
    unsigned char xlow;
    unsigned char ylow;
    int amount;
    struct MapElement *tile;
    unsigned char dmg;
    unsigned int x;
    unsigned int y;

    coords = (short)DAT_007febb8;
    xlow = (unsigned char)DAT_007febb8;
    ylow = (unsigned char)((unsigned short)DAT_007febb8 >> 8);
    amount = FUN_00463520();
    if (amount != 0) {
        if (coords == 0) {
            if (lpConfig->width == 0 || lpConfig->height == 0) {
                tile = 0;
            } else {
                tile = (struct MapElement *)GameMap[0];
            }
            if ((tile->flags & 0xa0) == 0) {
                return;
            }
        }
        do {
            y = ylow;
            x = (unsigned int)(((unsigned int)ylow << 8) | xlow) & 0xff;
            if (x < lpConfig->width && y < lpConfig->height) {
                tile = (struct MapElement *)((int)GameMap[y] + x * 0x14);
            } else {
                tile = 0;
            }
            if ((tile->flags & 0x80) != 0) {
                dmg = tile->field_11;
                if (1 < dmg) {
                    if (amount < (int)(unsigned int)dmg) {
                        tile->field_11 = dmg - (char)amount;
                    } else {
                        tile->field_11 = 1;
                    }
                }
                FUN_00463460(tile, (int *)&x);
            }
            coords = (short)tile->field_6;
            xlow = (unsigned char)coords;
            ylow = (unsigned char)((unsigned short)coords >> 8);
        } while (coords != 0);
    }
}

// FUNCTION: LEGOLAND 0x00463680
void FUN_00463680(void) {
    int row;
    int off;
    int *tile;

    row = 0;
    do {
        off = 0;
        do {
            tile = (int *)(*(int *)((int)GameMap + row) + off);
            off = off + 0x14;
            tile[0] = 0;
            tile[1] = 0;
            tile[2] = 0;
            tile[3] = 0;
            tile[4] = 0;
        } while (off < 0x1400);
        row = row + 4;
    } while (row < 0x400);
    ClearOverlays();
}

// FUNCTION: LEGOLAND 0x004636c0
int FUN_004636c0(void) {
    int total;
    unsigned int rows;

    total = 0;
    rows = lpConfig->height;
    if (rows != 0) {
        unsigned int width = lpConfig->width;
        do {
            if (width != 0) {
                total = total + width;
            }
            rows = rows - 1;
        } while (rows != 0);
    }
    return total;
}
