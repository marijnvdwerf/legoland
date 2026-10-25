#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "bloke_ai.h"
#include "bricks.h"
#include "build.h"
#include "clipping.h"
#include "controller.h"
#include "draw.h"
#include "gamemain.h"
#include "gamemap.h"
#include "gfx.h"
#include "image_sprite.h"
#include "llidb.h"
#include "map_object.h"
#include "mapscreen.h"
#include "math.h"
#include "obj_instance.h"
#include "objclass.h"
#include "pathfind.h"
#include "print_sprite.h"
#include "resource.h"
#include "sound_music.h"
#include "text.h"
#include "tilemap.h"
#include "timer.h"
#include "worker.h"

/* globals.h owns MapTileGrid as the canonical struct MapElement** map grid;
   map_object views the same memory through its own struct MapElement layout. */
#define MapTileGrid ((struct MapElement **)GameMap)

// FUNCTION: LEGOLAND 0x0045dd80
LEGO_EXPORT void AddObjectToMap(Element *param_1, TileId param_2, int param_3) {
    struct Ride *obj;
    struct MapElement *tile;
    int x;
    int y;
    struct Footprint rect;

    obj = param_1->obj;
    rect = obj->footprint;
    if (obj->flags & 0x20000) {
        BGFullUpdate = 1;
    }
    IncrementObjectCount((struct Ride *)obj);
    for (;;) {
        for (y = rect.y0; y <= rect.y1; y++) {
            for (x = rect.x0; x <= rect.x1; x++) {
                int tx = param_2.pos.x + x;
                int ty = param_2.pos.y + y;
                if (tx >= 0 && tx < lpConfig->width && ty >= 0 && ty < lpConfig->height) {
                    tile = &GameMap[ty][tx];
                } else {
                    tile = 0;
                }
                if (tile != 0) {
                    tile->field_10 = 0;
                    tile->field_0 = param_1;
                    *(unsigned short *)&tile->field_4 = param_2.id;
                    tile->flags = (unsigned short)(((tile->flags & 0x10) | param_3) | 0x80);
                    tile->durability_level = obj->durability;
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
        }
        if (rect.next == 0) {
            break;
        }
        rect = *rect.next;
    }
    if (MapBatchUpdate == 0 && LoadInProgress == 0) {
        CalculateMapRenderOrder();
    }
}

// FUNCTION: LEGOLAND 0x0045dee0
LEGO_EXPORT void SetObjRectFlags(Element *editObj, struct Point *pos, unsigned short flags) {
    struct Ride *obj;
    TileId coords;
    struct Cursor *node;
    struct LegoConfig *cfg;
    struct Footprint rect;
    struct MapElement *tile;
    int x;
    int y;
    int tx;
    int ty;
    unsigned int saved[0x60d];
    int center[2];

    obj = editObj->obj;
    memcpy(saved, &EditCursor, 0x60d * sizeof(unsigned int));
    coords.pos.x = (unsigned char)pos->x;
    coords.pos.y = (unsigned char)pos->y;
    GetTileCentre(pos, center);
    obj->cb_90(obj->element, center, 0x8f8);
    node = &EditCursor;
    if (node != 0) {
        cfg = lpConfig;
        do {
            rect = *(struct Footprint *)node->field_1414;
            if ((node->field_1828 & 0x3000) == 0) {
                for (;;) {
                    for (y = rect.y0; y <= rect.y1; y++) {
                        for (x = rect.x0; x <= rect.x1; x++) {
                            tx = node->tile_x + x;
                            ty = node->tile_y + y;
                            if (tx >= 0 && tx < cfg->width && ty >= 0 && ty < cfg->height) {
                                tile = &GameMap[ty][tx];
                                if (tile != 0) {
                                    tile->field_0 = editObj;
                                    *(unsigned short *)&tile->field_4 = coords.id;
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
                    if (rect.next == 0) {
                        break;
                    }
                    rect = *rect.next;
                }
            }
            node = node->next;
        } while (node != 0);
    }
    memcpy(&EditCursor, saved, 0x60d * sizeof(unsigned int));
}

/* Places an object on the map for BuildObject: positions the class cursor at the tile centre (cb_90), removes
 * whatever lies under every footprint of the cursor chain (parts without flag 0x3000), then stamps those tiles with
 * the element, anchor id and flags (plus 0x8000 for class flag 0x800000) and field_10 = 2. EditCursor is saved and
 * restored. */
// FUNCTION: LEGOLAND 0x0045e080
void StampObjectFootprintOnMap(Element *editObj, struct Point *pos, unsigned short flags) {
    struct Ride *obj;
    TileId coords;
    struct Cursor *node;
    struct Footprint rect;
    struct MapElement *tile;
    int x;
    int y;
    int tx;
    int ty;
    struct Point coord;
    unsigned int saved[0x60d];
    int center[2];

    obj = editObj->obj;
    memcpy(saved, &EditCursor, 0x60d * sizeof(unsigned int));
    coords.pos.x = (unsigned char)pos->x;
    coords.pos.y = (unsigned char)pos->y;
    GetTileCentre(pos, center);
    obj->cb_90(obj->element, center, 0x8f8);
    node = &EditCursor;
    if (node != 0) {
        do {
            rect = *(struct Footprint *)node->field_1414;
            if ((node->field_1828 & 0x3000) == 0) {
                for (;;) {
                    for (y = rect.y0; y <= rect.y1; y++) {
                        for (x = rect.x0; x <= rect.x1; x++) {
                            coord.x = node->tile_x + x;
                            coord.y = node->tile_y + y;
                            if (coord.x >= 0 && coord.x < lpConfig->width && coord.y >= 0 && coord.y < lpConfig->height) {
                                tile = &GameMap[coord.y][coord.x];
                            } else {
                                tile = 0;
                            }
                            if (tile != 0) {
                                RemoveObjectAtPoint(&coord);
                            }
                        }
                    }
                    if (rect.next == 0) {
                        break;
                    }
                    rect = *rect.next;
                }
            }
            node = node->next;
        } while (node != 0);
    }
    node = &EditCursor;
    if (node != 0) {
        do {
            rect = *(struct Footprint *)node->field_1414;
            if ((node->field_1828 & 0x3000) == 0) {
                for (;;) {
                    for (y = rect.y0; y <= rect.y1; y++) {
                        for (x = rect.x0; x <= rect.x1; x++) {
                            coord.x = node->tile_x + x;
                            coord.y = node->tile_y + y;
                            if (coord.x >= 0 && coord.x < lpConfig->width && coord.y >= 0 && coord.y < lpConfig->height) {
                                tile = &GameMap[coord.y][coord.x];
                            } else {
                                tile = 0;
                            }
                            if (tile != 0) {
                                tile->field_0 = editObj;
                                *(unsigned short *)&tile->field_4 = coords.id;
                                tile->flags = (unsigned short)((tile->flags & 0x10) | flags);
                                if (obj->flags & 0x800000) {
                                    *((unsigned char *)&tile->flags + 1) |= 0x80;
                                }
                                tile->field_10 = 2;
                            }
                        }
                    }
                    if (rect.next == 0) {
                        break;
                    }
                    rect = *rect.next;
                }
            }
            node = node->next;
        } while (node != 0);
    }
    memcpy(&EditCursor, saved, 0x60d * sizeof(unsigned int));
}

/* Temporarily builds the object cursor, removes path tiles covered by its footprints, and restores the edit cursor. */
// FUNCTION: LEGOLAND 0x0045e300
void RemovePathsUnderObjectFootprint(Element *editObj, struct Point *pos) {
    struct Ride *obj;
    struct Cursor *node;
    struct LegoConfig *cfg;
    struct Footprint *rect;
    struct MapElement *tile;
    struct Element *gtile;
    int coord[2];
    int center[2];
    int x;
    int y;
    unsigned int saved[0x60d];

    obj = editObj->obj;
    memcpy(saved, &EditCursor, 0x60d * sizeof(unsigned int));
    GetTileCentre(pos, center);
    obj->cb_90(obj->element, center, 0x8f8);
    node = &EditCursor;
    if (node != 0) {
        cfg = lpConfig;
        do {
            if ((node->field_1828 & 0x3000) == 0) {
                for (rect = (struct Footprint *)&node->field_1414[0]; rect != 0; rect = rect->next) {
                    y = rect->y0;
                    if (y <= rect->y1) {
                        do {
                            x = rect->x0;
                            if (x <= rect->x1) {
                                do {
                                    coord[0] = node->tile_x + x;
                                    coord[1] = node->tile_y + y;
                                    if (coord[0] >= 0 && coord[0] < cfg->width && coord[1] >= 0 &&
                                        coord[1] < cfg->height &&
                                        (tile = &GameMap[coord[1]][coord[0]]) != 0 &&
                                        (tile->flags & 8) != 0 &&
                                        (gtile = (struct Element *)tile->field_0)->data == PathControlObject) {
                                        RemovePathSquare((struct Point *)coord);
                                        tile->flags &= 0xffe7;
                                        tile->field_10 &= 0xfe;
                                        tile = &GameMap[coord[1]][coord[0]];
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
            node = node->next;
        } while (node != 0);
    }
    memcpy(&EditCursor, saved, 0x60d * sizeof(unsigned int));
}

/* Temporarily builds the object cursor and assigns its footprint tiles to the object and anchor tile, clearing
 * their flags. Restores the edit cursor afterwards. */
// FUNCTION: LEGOLAND 0x0045e4a0
void SetObjectFootprintMapTiles(Element *editObj, struct Point *pos) {
    struct Ride *obj;
    TileId coords;
    struct Cursor *node;
    struct Footprint rect;
    struct MapElement *tile;
    int x;
    int y;
    int tx;
    int ty;
    unsigned int saved[0x60d];
    int center[2];

    obj = editObj->obj;
    memcpy(saved, &EditCursor, 0x60d * sizeof(unsigned int));
    coords.pos.x = (unsigned char)pos->x;
    coords.pos.y = (unsigned char)pos->y;
    GetTileCentre(pos, center);
    obj->cb_90(obj->element, center, 0x8f8);
    node = &EditCursor;
    if (node != 0) {
        do {
            rect = *(struct Footprint *)node->field_1414;
            if ((node->field_1828 & 0x3000) == 0) {
                for (;;) {
                    for (y = rect.y0; y <= rect.y1; y++) {
                        for (x = rect.x0; x <= rect.x1; x++) {
                            tx = node->tile_x + x;
                            ty = node->tile_y + y;
                            if (tx >= 0 && tx < lpConfig->width && ty >= 0 && ty < lpConfig->height) {
                                tile = &GameMap[ty][tx];
                            } else {
                                tile = 0;
                            }
                            if (tile != 0) {
                                tile->flags = 0;
                                tile->field_0 = editObj;
                                *(unsigned short *)&tile->field_4 = coords.id;
                                tile->field_10 = 0;
                            }
                        }
                    }
                    if (rect.next == 0) {
                        break;
                    }
                    rect = *rect.next;
                }
            }
            node = node->next;
        } while (node != 0);
    }
    memcpy(&EditCursor, saved, 0x60d * sizeof(unsigned int));
}

// FUNCTION: LEGOLAND 0x0045e620
int HasExternalEntrance(struct Ride *obj) {
    struct Footprint rect;

    if (obj != 0) {
        rect = obj->footprint;
        if (obj->type != 3 && obj->type != 2 && obj->type != 0 &&
            (rect.v[0] > (int)obj->x || (int)obj->x > rect.v[2] ||
                rect.v[1] > (int)obj->y || (int)obj->y > rect.v[3])) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045e690
int HasSeparateExit(struct Ride *obj) {
    if (((int)obj->field_24 != obj->x) || ((int)obj->field_25 != obj->y)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045e6b0
int GetEntranceSide(struct Ride *obj) {
    int x;
    struct Footprint rect;

    x = (int)obj->x;
    rect = obj->footprint;
    if (rect.v[2] < x) {
        return 4;
    }
    if (x < rect.v[0]) {
        return 8;
    }
    return (rect.v[3] >= (int)obj->y) + 1;
}

// FUNCTION: LEGOLAND 0x0045e710
int GetExitSide(struct Ride *obj) {
    struct Footprint rect;

    rect = obj->footprint;
    if (rect.v[2] < obj->field_24) {
        return 0x80;
    }
    if (obj->field_24 < rect.v[0]) {
        return 0x40;
    }
    return (((rect.v[3] >= obj->field_25) - 1) & 0x10) + 0x10;
}

// FUNCTION: LEGOLAND 0x0045e770
void MarkObjectEntrances(struct Element *node, int *offset) {
    struct Ride *obj;
    struct MapElement *tile;
    int x;
    int y;

    obj = node->ride;
    if (HasExternalEntrance(obj) != 0) {
        x = (int)obj->x + offset[0];
        y = (int)obj->y + offset[1];
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            tile = &MapTileGrid[y][x];
            if (tile != 0) {
                tile->field_12 |= (unsigned short)GetEntranceSide(obj);
            }
        }
        if (HasSeparateExit((struct Ride *)obj) != 0) {
            x = obj->field_24 + offset[0];
            y = obj->field_25 + offset[1];
            if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                tile = &MapTileGrid[y][x];
                if (tile != 0) {
                    tile->field_12 |= (unsigned short)GetExitSide(obj);
                }
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0045e850
void ClearObjectEntrances(struct Element *node, int *offset) {
    struct Ride *obj;
    struct MapElement *tile;
    int x;
    int y;

    obj = node->ride;
    if (HasExternalEntrance(obj) != 0) {
        x = (int)obj->x + offset[0];
        y = (int)obj->y + offset[1];
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            tile = &MapTileGrid[y][x];
            if (tile != 0) {
                tile->field_12 &= ~(unsigned short)GetEntranceSide(obj);
            }
        }
        if (HasSeparateExit((struct Ride *)obj) != 0) {
            x = obj->field_24 + offset[0];
            y = obj->field_25 + offset[1];
            if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                tile = &MapTileGrid[y][x];
                if (tile != 0) {
                    tile->field_12 &= ~(unsigned short)GetExitSide(obj);
                }
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0045e930
int IsTileFreeOrPath(struct MapElement *obj) {
    struct Element *p;

    if (obj == 0) {
        return 0;
    }
    if (obj->flags & 0x8a8) {
        p = obj->field_0;
        if (p->ride != PathControlObject) {
            return 0;
        }
    }
    return 1;
}

/* Checks the translated entrance position, and the separate-exit check when required, rejecting in-bounds tiles
 * that are neither free nor path tiles. */
// FUNCTION: LEGOLAND 0x0045e960
int IsObjectEntranceOffsetClear(struct Element *node, int *offset) {
    struct Ride *obj;
    struct MapElement *tile;
    int x;
    int y;

    obj = node->ride;
    if (HasExternalEntrance(obj) != 0) {
        x = offset[0] + (int)obj->x;
        y = offset[1] + (int)obj->y;
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            tile = &MapTileGrid[y][x];
            if (tile != 0 && IsTileFreeOrPath(tile) == 0) {
                return 0;
            }
        }
        if (HasSeparateExit((struct Ride *)obj) != 0) {
            x = offset[0] + (int)obj->x;
            y = offset[1] + (int)obj->y;
            if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                tile = &MapTileGrid[y][x];
                if (tile != 0 && IsTileFreeOrPath(tile) == 0) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0045ea40
void GetObjectEntrancePos(struct Ride *obj, struct Point *out) {
    if ((int)obj->x < obj->footprint.v[0] || (int)obj->x > obj->footprint.v[2] || (int)obj->y < obj->footprint.v[1] ||
        (int)obj->y > obj->footprint.v[3]) {
        if (obj->type == 1 || obj->type == 4 || obj->type == 5) {
            *out = obj->pos;
            return;
        }
    }
    out->x = obj->footprint.v[2] + 1;
    out->y = (obj->footprint.v[3] + obj->footprint.v[1]) / 2;
}

/* 1 when the class exists and its flag 0x200000 is clear, else 0. */
// FUNCTION: LEGOLAND 0x0045eab0
unsigned int IsClassFlag200000Clear(struct Ride *obj) {
    if (obj != 0) {
        return (~obj->flags >> 21) & 1;
    }
    return 0;
}

/* 1 when the class exists and its type is neither 0 nor 2. */
// FUNCTION: LEGOLAND 0x0045ead0
int IsObjectTypeNot0Or2(struct Ride *obj) {
    if (obj == 0 || obj->type == 0 || obj->type == 2) {
        return 0;
    }
    return 1;
}

/* 1 when IsObjectTypeNot0Or2, or the object's parent element has flag 0x10 and the object has neither flag 0x200000
 * nor 0x400000. BuildObject then stamps its footprint; ValidateCursor lets it stand on path tiles. */
// FUNCTION: LEGOLAND 0x0045eaf0
int IsTypeNot0Or2OrParentFlag10(struct Ride *obj) {
    if (IsObjectTypeNot0Or2((struct Ride *)obj) != 0) {
        return 1;
    }
    if (obj != 0 && obj->parent_element != 0 && (obj->parent_element->flags & 0x10) != 0 &&
        (obj->flags & 0x600000) == 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0045eb30
LEGO_EXPORT int BuildObject(Element *editObj, int *coords) {
    struct Ride *obj;
    TileId packed;
    int cost;
    struct Point *effect;
    struct Point out;

    packed.pos.x = (unsigned char)coords[0];
    packed.pos.y = (unsigned char)coords[1];
    obj = editObj->obj;
    cost = GetObjCost(obj);
    if (GetBrickCount() < cost) {
        return 0;
    }
    if (editObj == CastleObjElem) {
        CastlePlacedFlag = 1;
    }
    if (MapBatchUpdate == 0) {
        PlayAppropriateBuildEffect((struct Ride *)obj, coords);
    } else {
        MapBatchChanged = 1;
    }
    if (obj->flags & 0x80000) {
        if (AddObjectToBuildList((struct Ride *)obj, packed) == 0) {
            return 0;
        }
        UseBricks(GetObjCost(obj));
        if (IsClassFlag200000Clear((struct Ride *)obj) != 0 || IsTypeNot0Or2OrParentFlag10((struct Ride *)obj) != 0) {
            StampObjectFootprintOnMap(editObj, (struct Point *)coords, 0x20);
        } else {
            SetObjRectFlags(editObj, (struct Point *)coords, 0x20);
        }
        GetObjectEntrancePos(obj, &out);
        out.x = out.x + coords[0];
        out.y = out.y + coords[1];
        if (obj->flags & 0x400000) {
            InitEntrance1Point();
            UpdatePathLinks(1);
            effect = GetEntrance1Point();
            FindMapPathAStar(out.x, out.y, effect->x, effect->y);
        }
        if (MapBatchUpdate == 0) {
            CalculateMapRenderOrder();
        }
    } else {
        UseBricks(GetObjCost(obj));
        if (IsClassFlag200000Clear((struct Ride *)obj) != 0 || IsTypeNot0Or2OrParentFlag10((struct Ride *)obj) != 0) {
            StampObjectFootprintOnMap(editObj, (struct Point *)coords, 0);
        }
        GetObjectEntrancePos(obj, &out);
        out.x = out.x + coords[0];
        out.y = out.y + coords[1];
        PutObjOnMap((struct Ride *)obj, editObj, (struct Point *)coords);
        if (obj->flags & 0x400000) {
            InitEntrance1Point();
            UpdatePathLinks(1);
            effect = GetEntrance1Point();
            FindMapPathAStar(out.x, out.y, effect->x, effect->y);
        }
    }
    MarkObjectEntrances((struct Element *)editObj, coords);
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
LEGO_EXPORT void ObjectIsBuilt(struct Ride *obj, TileId coords) {
    struct Point pos;
    struct Point saved_tile;
    int out[2];
    struct Footprint saved_rect;
    int saved_140c;
    int saved_1410;
    struct Cursor *node;
    struct LegoConfig *cfg;
    struct Footprint *next;
    struct Footprint rect;
    int x;
    int y;
    int tx;
    int ty;
    struct MapElement *tile;

    pos.x = coords.pos.x;
    pos.y = coords.pos.y;
    {
        struct SampleSource source;
        source.type = 2;
        source.x = pos.x;
        source.y = pos.y;
        UnSourceAndFadeAllSamplesFromSource(&source, -200);
    }
    GetTileCentre(&pos, out);
    EditCursor.field_1830 = 0;
    if (GamePad & 0x1000) {
        saved_rect = *(struct Footprint *)EditCursor.field_1414;
        saved_tile.x = EditCursor.tile_x;
        saved_tile.y = EditCursor.tile_y;
        saved_140c = EditCursor.field_140c;
        saved_1410 = EditCursor.field_1410;
    }
    ((struct Ride *)obj)->cb_90(obj->element, out, 0x8f8);
    node = &EditCursor;
    if (node != 0) {
        cfg = lpConfig;
        do {
            next = (struct Footprint *)node->field_1414;
            do {
                rect = *next;
                if ((node->field_1828 & 0x3000) == 0) {
                    for (y = rect.y0; y <= rect.y1; y++) {
                        for (x = rect.x0; x <= rect.x1; x++) {
                            tx = node->tile_x + x;
                            ty = node->tile_y + y;
                            if (tx >= 0 && tx < cfg->width && ty >= 0 && ty < cfg->height) {
                                tile = &GameMap[ty][tx];
                            } else {
                                tile = 0;
                            }
                            if (tile != 0) {
                                tile->flags &= 0xffdf;
                                tile->field_10 = 0;
                                cfg = lpConfig;
                            }
                        }
                    }
                } else {
                    break;
                }
                next = rect.next;
            } while (next != 0);
            node = node->next;
        } while (node != 0);
    }
    PutObjOnMap(obj, obj->element, &pos);
    EditCursor.field_1830 = 0;
    if (GamePad & 0x1000) {
        *(struct Footprint *)EditCursor.field_1414 = saved_rect;
        EditCursor.tile_x = saved_tile.x;
        EditCursor.tile_y = saved_tile.y;
        EditCursor.field_140c = saved_140c;
        EditCursor.field_1410 = saved_1410;
    } else if (EditMode.unk8 != 0) {
        ((struct Ride *)EditMode.unk8)->cb_90(((struct Ride *)EditMode.unk8)->element, &MousePos, 0x8f8);
    }
}

// FUNCTION: LEGOLAND 0x0045ef50
LEGO_EXPORT void ObjectIsBuilding(struct Ride *obj, TileId coords) {
    struct BuildBuf buf;

    buf.x = coords.pos.x;
    buf.y = coords.pos.y;
    if ((((struct Ride *)obj)->flags & 0x200000) == 0) {
        buf.two = 2;
        buf.x2 = buf.x;
        buf.y2 = buf.y;
        if (CountSamplesFromSource((struct SampleSource *)&buf.two) == 0) {
            PlayAppropriateBuildEffect(obj, (int *)&buf.x);
        }
    }
}

// FUNCTION: LEGOLAND 0x0045efc0
LEGO_EXPORT void ApplyConsTileMap(Element *editObj, TileId coords) {
}

// FUNCTION: LEGOLAND 0x0045efd0
LEGO_EXPORT void ApplyDestrTileMap(Element *editObj, TileId coords) {
}

// FUNCTION: LEGOLAND 0x0045efe0
LEGO_EXPORT unsigned int AddBasicObject(Element *editObj, int *coords) {
    struct Ride *obj;
    struct MapElement *tile;
    TileId packed;
    TileId id;
    struct ObjInstance *instance;

    obj = editObj->obj;
    if (obj->flags & 0x20000) {
        BGFullUpdate = 1;
    }
    if (obj->flags & 0x40000) {
        if (coords[0] >= 0 && coords[0] < lpConfig->width && coords[1] >= 0 && coords[1] < lpConfig->height) {
            tile = &GameMap[coords[1]][coords[0]];
        } else {
            tile = 0;
        }
        tile->field_8 = *obj->tiles;
        tile->field_0 = editObj;
        tile->field_4 = (unsigned char)coords[0];
        tile->field_5 = (unsigned char)coords[1];
        if (obj->flags & 2) {
            tile->field_10 = 2;
        }
        if (obj->flags & 1) {
            tile->field_10 = 1;
        }
        tile->flags |= 8;
        if (obj->flags & 0x800000) {
            tile->flags |= 0x8000;
        }
        IncrementObjectCount((struct Ride *)obj);
    } else {
        packed.pos.x = (unsigned char)coords[0];
        packed.pos.y = (unsigned char)coords[1];
        AddObjectToMap(editObj, packed, 0);
        ApplyConsTileMap(editObj, packed);
        if (obj->type != 2) {
            id.pos.x = (unsigned char)coords[0];
            id.pos.y = (unsigned char)coords[1];
            instance = CreateObjectInstance(obj, &id.id);
            if (instance != 0) {
                AddInstanceToList((struct InstanceNode *)instance);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0045f100
LEGO_EXPORT void RemoveObjectFromMap(TileId coords) {
    short cx;
    short cy;
    struct MapElement *tile;
    struct Ride *obj;
    struct Footprint rect;
    struct Footprint *next;
    int x;
    int y;

    cx = coords.pos.x;
    cy = coords.pos.y;
    if (cx >= 0 && cx < lpConfig->width && cy >= 0 && cy < lpConfig->height) {
        tile = &GameMap[cy][cx];
    } else {
        tile = 0;
    }
    obj = ((Element *)tile->field_0)->obj;
    if (obj->flags & 0x20000) {
        BGFullUpdate = 1;
    }
    DecrementObjectCount((struct Ride *)obj);
    next = &obj->footprint;
    do {
        rect = *next;
        for (y = rect.y0; y <= rect.y1; y++) {
            for (x = rect.x0; x <= rect.x1; x++) {
                GameMap[cy + y][cx + x].flags &= 0xff5f;
                GameMap[cy + y][cx + x].field_10 &= 0xfc;
            }
        }
        next = rect.next;
    } while (next != 0);
    if (MapBatchUpdate == 0) {
        CalculateMapRenderOrder();
        MapBatchChanged = 1;
    }
}

// FUNCTION: LEGOLAND 0x0045f220
LEGO_EXPORT void StandardRemoveObject(Element *editObj, TileId coords, struct Cursor *cursor) {
    struct Point pos;
    struct MapElement *tile;
    struct Ride *obj;
    struct Footprint rect;
    int x;
    int y;

    pos.x = coords.pos.x;
    pos.y = coords.pos.y;
    if (pos.x >= 0 && pos.x < lpConfig->width && pos.y >= 0 && pos.y < lpConfig->height) {
        tile = &GameMap[pos.y][pos.x];
    } else {
        tile = 0;
    }
    obj = editObj->obj;
    if (obj->flags & 0x20000) {
        BGFullUpdate = 1;
    }
    AddBricks(GetObjSalvageValue(obj, tile->durability_level));
    if (tile->flags & 0x80) {
        ApplyDestrTileMap(editObj, coords);
        ClearObjectEntrances((struct Element *)editObj, (int *)&pos);
        RemoveObjectFromMap(coords);
    } else {
        rect = *(struct Footprint *)cursor->field_1414;
        DecrementObjectCount((struct Ride *)obj);
        for (;;) {
            for (y = rect.y0; y <= rect.y1; y++) {
                for (x = rect.x0; x <= rect.x1; x++) {
                    RestoreBaseMap(cursor->tile_x + x, cursor->tile_y + y);
                    SetMapFlags(cursor->tile_x + x, cursor->tile_y + y, 0);
                    Set_RFFlags((cursor->tile_x + x) * 0x100, (cursor->tile_y + y) * 0x100, 0);
                    GameMap[cursor->tile_y + y][cursor->tile_x + x].field_0 = 0;
                }
            }
            if (rect.next == 0) {
                break;
            }
            rect = *rect.next;
        }
    }
    if (obj->type != 2) {
        TileId id;
        struct ObjInstance *instance;
        id.pos.x = (unsigned char)pos.x;
        id.pos.y = (unsigned char)pos.y;
        if (pos.x >= 0 && pos.x < lpConfig->width && pos.y >= 0 && pos.y < lpConfig->height) {
            tile = &GameMap[pos.y][pos.x];
        } else {
            tile = 0;
        }
        instance = GetInstanceOfClass(tile->field_0->data, &id);
        if (instance != 0) {
            RemoveInstanceFromList((struct InstanceNode *)instance);
            free(instance);
        }
    }
}

// FUNCTION: LEGOLAND 0x0045f440
LEGO_EXPORT void SetEditCursorFootPrint(void *src) {
    memcpy(EditCursor.field_1414, src, 20);
}

// FUNCTION: LEGOLAND 0x0045f460
void ResetCursorBuildable(struct Cursor *cursor) {
    cursor->field_140c = 1;
    cursor->field_1410 = 0;
}

// FUNCTION: LEGOLAND 0x0045f480
void SetCursorBlocked(struct Cursor *cursor, int param) {
    int negated;

    negated = -param;
    if (cursor->field_140c >= negated) {
        cursor->field_140c = negated;
        cursor->field_1410 = param;
    }
}

// FUNCTION: LEGOLAND 0x0045f4b0
int IsCursorBuildable(struct Cursor *cursor) {
    int target;

    target = cursor->field_140c;
    return target > 0;
}

// FUNCTION: LEGOLAND 0x0045f4d0
void PropagateCursorBlocked(struct Cursor *cursor) {
    struct Cursor *node;
    struct Cursor *best;

    best = cursor;
    for (node = cursor; node != 0; node = node->next) {
        node->field_1828 |= 8;
        if (node->field_140c < best->field_140c) {
            best = node;
        }
    }
    if (IsCursorBuildable(best) == 0) {
        for (; cursor != 0; cursor = cursor->next) {
            SetCursorBlocked(cursor, best->field_1410);
        }
    }
}

// FUNCTION: LEGOLAND 0x0045f540
struct Cursor *GetOrAddPathCursor(struct Cursor *cursor) {
    struct Cursor *node;
    struct Cursor *last;

    if (cursor == 0) {
        return 0;
    }
    for (node = cursor; node != 0; node = node->next) {
        last = node;
        if ((node->field_1828 & 0x1000) != 0) {
            return node;
        }
    }
    ResetCursorBuildable(&PathCursor);
    PathCursor.tile_x = cursor->tile_x;
    PathCursor.tile_y = cursor->tile_y;
    PathCursor.field_1414[0] = cursor->field_1414[0] - 1;
    PathCursor.field_1414[1] = cursor->field_1414[1] - 1;
    PathCursor.field_1414[2] = cursor->field_1414[2] + 1;
    PathCursor.field_1414[3] = cursor->field_1414[3] + 1;
    PathCursor.field_1828 = 0x1008;
    PathCursor.field_1830 = 0;
    last->next = &PathCursor;
    return &PathCursor;
}

// FUNCTION: LEGOLAND 0x0045f5f0
LEGO_EXPORT void BuildCursorPtr(struct Cursor *cursor, unsigned int param_2, int param_3) {
    struct Cursor *pts;
    struct Sprite *sprite;
    short size;
    int fbits;
    unsigned short flag;
    int built;
    struct Footprint rect;
    int x;
    int y;
    short half2;
    short half;

    while (1) {
        rect = *(struct Footprint *)&cursor->field_1414[0];
        if (param_3 != 0) {
            GetOrAddPathCursor(cursor);
        }
        sprite = (struct Sprite *)TileSpriteArray[LevelBaseTileId];
        size = sprite->height;
        pts = (struct Cursor *)cursor;
        pts->count = 0;
        built = IsCursorBuildable(cursor);
        flag = (built != 0) + 1;
        fbits = (flag - 1) * 4;
        cursor->pad_1428[0] = (unsigned char)fbits;
        for (y = rect.y0; y <= rect.y1; y++) {
            if (rect.x0 <= rect.x1) {
                x = rect.x0;
                half2 = (short)((short)(size * 2) / 2);
                half = (short)(size / 2);
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
                        pts->ypts[pts->count] = (unsigned short)(half + uy);
                        pts->xpts[pts->count] = ux;
                        pts->fpts[pts->count] = 2 | fbits;
                        pts->count++;
                    }
                    x++;
                } while (rect.x1 >= x);
            }
        }
        cursor = cursor->next;
        if (cursor == 0) {
            break;
        }
        param_3 = 0;
    }
}

// FUNCTION: LEGOLAND 0x0045f810
LEGO_EXPORT void ValidateCursor(struct Cursor *cursor, void *param) {
    struct Cursor *first;
    struct Footprint *rect;
    struct MapElement *tile;
    struct Ride *obj;
    int people;
    int x;
    int y;
    int tx;
    int ty;

    int y1;
    int x1;
    first = cursor;
    if (IsObjectTypeNot0Or2((struct Ride *)param) != 0 && (cursor->field_1828 & 0x4000) == 0) {
        GetOrAddPathCursor(cursor);
    }
    ResetCursorBuildable(cursor);
    for (; cursor != 0; cursor = cursor->next) {
        if (cursor->field_1828 & 0x2000) {
            continue;
        }
        for (rect = (struct Footprint *)cursor->field_1414; rect != 0; rect = rect->next) {
            RECT inter;
            RECT box;
            RECT bounds;

            bounds.top = 0;
            bounds.left = 0;
            bounds.bottom = lpConfig->height;
            bounds.right = lpConfig->height;
            x1 = rect->x1;
            box.right = x1 + cursor->tile_x;
            box.left = rect->x0 + cursor->tile_x;
            y1 = rect->y1;
            box.top = rect->y0 + cursor->tile_y;
            box.bottom = y1 + cursor->tile_y;
            if (IntersectRect(&inter, &box, &bounds) != 0) {
                people = CheckForPeople((struct MapRect *)&inter);
                switch (people) {
                case -1:
                    SetCursorBlocked(cursor, 4);
                    break;
                case 1:
                    SetCursorBlocked(cursor, 3);
                    break;
                }
            }
            for (y = rect->y0; y <= rect->y1; y++) {
                for (x = rect->x0; x <= rect->x1; x++) {
                    tx = cursor->tile_x + x;
                    ty = cursor->tile_y + y;
                    if (tx >= 0 && tx < lpConfig->width && ty >= 0 && ty < lpConfig->height) {
                        tile = &GameMap[ty][tx];
                    } else {
                        tile = 0;
                    }
                    if (tile == 0) {
                        SetCursorBlocked(cursor, 7);
                        continue;
                    }
                    if (tile->flags & 0x40) {
                        SetCursorBlocked(cursor, 1);
                    }
                    if ((tile->flags & 0xa8) == 0 ||
                        (obj = ((Element *)tile->field_0)->obj) == (struct Ride *)PathControlObject) {
                        obj = 0;
                    }
                    if (obj != 0 && (obj->flags & 0x200000) == 0) {
                        SetCursorBlocked(cursor, 10);
                    } else if (IsClassFlag200000Clear((struct Ride *)param) == 0) {
                        if (obj != 0) {
                            SetCursorBlocked(cursor, 6);
                        }
                        if (tile->flags & 0x800) {
                            SetCursorBlocked(cursor, 5);
                        }
                    }
                    if (IsTypeNot0Or2OrParentFlag10((struct Ride *)param) != 0) {
                        if ((tile->flags & 0x10) && (tile->field_10 & 1) == 0) {
                            SetCursorBlocked(cursor, 9);
                        }
                    } else if ((tile->flags & 0x10) || (tile->field_10 & 1)) {
                        SetCursorBlocked(cursor, 8);
                    }
                }
            }
        }
    }
    PropagateCursorBlocked(first);
}

// FUNCTION: LEGOLAND 0x0045fa80
LEGO_EXPORT void CalcBasicObjectCursor(struct Element *obj, int *a2, unsigned int a3) {
    struct Ride *v1;

    v1 = obj->data;
    DefaultCursor(&EditCursor);
    ScreenToMapRef(a2, &EditCursor.tile_x, a3);
    memcpy(EditCursor.field_1414, &v1->footprint, sizeof(struct Footprint));
    ValidateCursor(&EditCursor, obj->data);
}

/* Draws one animated cursor outline segment on a 16-bit surface: dir 0 a vertical run upwards, dir 1/2 an isometric
 * 2:1 diagonal to the left/right, two pixels thick, cycling a 16-step two-colour pattern with FrameNumber (marching
 * ants). Clipped to SPRITE_ClipRect. */
// FUNCTION: LEGOLAND 0x0045fad0
void DrawCursorMarchingLine(struct VideoArg *surface, int dir, int px, int py, unsigned char *color, int count) {
    POINT pt;
    int flip;
    unsigned short c1;
    unsigned short c2;
    unsigned short c;
    int step;
    unsigned short *dst;
    unsigned short *dst2;
    unsigned int frame;
    unsigned short pattern[16];

    flip = 0;
    c1 = (unsigned short)GetNearestColour(color[0], color[1], color[2]);
    c2 = (unsigned short)GetNearestColour(color[4], color[5], color[6]);
    frame = FrameNumber;
    pattern[12] = c2;
    pattern[13] = c2;
    pattern[14] = c2;
    pattern[15] = c2;
    pattern[0] = c1;
    pattern[1] = c1;
    pattern[2] = c1;
    pattern[3] = c1;
    pattern[4] = c1;
    pattern[5] = c1;
    pattern[6] = c1;
    pattern[7] = c1;
    pattern[8] = c1;
    pattern[9] = c1;
    pattern[10] = c1;
    pattern[11] = c1;
    if (surface->format != 2) {
        return;
    }
    dst = (unsigned short *)(surface->pitch * py + (char *)surface->bits + px * 2);
    dst2 = (unsigned short *)((char *)dst - surface->pitch);
    switch (dir) {
    case 0:
        while (count--) {
            pt.x = px;
            pt.y = py;
            if (PtInRect(&SPRITE_ClipRect, pt)) {
                c = pattern[frame & 0xf];
                *dst = c;
                if (px > 0) {
                    dst[-1] = c;
                }
            }
            py--;
            dst = (unsigned short *)((char *)dst - surface->pitch);
            frame++;
        }
        return;
    case 1:
        step = -1;
        break;
    case 2:
        step = 1;
        break;
    default:
        return;
    }
    count++;
    while (count--) {
        pt.x = px;
        pt.y = py;
        if (PtInRect(&SPRITE_ClipRect, pt)) {
            c = pattern[frame & 0xf];
            *dst = c;
            if (py > 0) {
                *dst2 = c;
            }
        }
        px += step;
        dst += step;
        dst2 += step;
        if (flip) {
            py++;
            dst2 = dst;
            dst = (unsigned short *)((char *)dst + surface->pitch);
        }
        flip ^= 1;
        frame++;
    }
}

/* Dashed variant of DrawCursorMarchingLine: a 32-step pattern with transparent gaps, one dash of each colour. */
// FUNCTION: LEGOLAND 0x0045fca0
void DrawCursorDashedMarchingLine(struct VideoArg *surface, int dir, int px, int py, unsigned char *color, int count) {
    POINT pt;
    int flip;
    unsigned short c1;
    unsigned short c2;
    short c;
    int step;
    unsigned short *dst;
    unsigned short *dst2;
    unsigned int frame;
    unsigned short pattern[32];

    flip = 0;
    c1 = (unsigned short)GetNearestColour(color[0], color[1], color[2]);
    c2 = (unsigned short)GetNearestColour(color[4], color[5], color[6]);
    frame = FrameNumber;
    pattern[28] = c2;
    pattern[29] = c2;
    pattern[30] = c2;
    pattern[31] = c2;
    pattern[0] = 0;
    pattern[1] = 0;
    pattern[2] = 0;
    pattern[3] = 0;
    pattern[4] = 0;
    pattern[5] = 0;
    pattern[6] = 0;
    pattern[7] = 0;
    pattern[8] = 0;
    pattern[9] = 0;
    pattern[10] = 0;
    pattern[11] = 0;
    pattern[12] = c1;
    pattern[13] = c1;
    pattern[14] = c1;
    pattern[15] = c1;
    pattern[16] = 0;
    pattern[17] = 0;
    pattern[18] = 0;
    pattern[19] = 0;
    pattern[20] = 0;
    pattern[21] = 0;
    pattern[22] = 0;
    pattern[23] = 0;
    pattern[24] = 0;
    pattern[25] = 0;
    pattern[26] = 0;
    pattern[27] = 0;
    if (surface->format != 2) {
        return;
    }
    dst = (unsigned short *)(surface->pitch * py + (char *)surface->bits + px * 2);
    dst2 = (unsigned short *)((char *)dst - surface->pitch);
    switch (dir) {
    case 0:
        while (count--) {
            pt.x = px;
            pt.y = py;
            if (PtInRect(&SPRITE_ClipRect, pt)) {
                c = pattern[frame & 0x1f];
                if (c != 0) {
                    *dst = c;
                }
                if (px > 0 && c != 0) {
                    dst[-1] = c;
                }
            }
            py--;
            dst = (unsigned short *)((char *)dst - surface->pitch);
            frame++;
        }
        return;
    case 1:
        step = -1;
        break;
    case 2:
        step = 1;
        break;
    default:
        return;
    }
    count++;
    while (count--) {
        pt.x = px;
        pt.y = py;
        if (PtInRect(&SPRITE_ClipRect, pt)) {
            c = pattern[frame & 0x1f];
            if (c != 0) {
                *dst = c;
            }
            if (py > 0 && c != 0) {
                *dst2 = c;
            }
        }
        px += step;
        dst += step;
        dst2 += step;
        if (flip) {
            py++;
            dst2 = dst;
            dst = (unsigned short *)((char *)dst + surface->pitch);
        }
        flip ^= 1;
        frame++;
    }
}

// FUNCTION: LEGOLAND 0x0045ff00
LEGO_EXPORT void RenderCursor(struct Cursor *cursor) {
    struct Cursor *pts;
    struct Footprint *nextp;
    struct Footprint rect;
    struct MapElement *tile;
    struct Ride *editing;
    short size;
    int code;
    unsigned char *pat;
    int bx;
    int by;
    int x;
    int y;
    int i;
    RECT clip;
    struct Point tilept;
    int screen[4];
    RECT saved_clip;
    struct VideoArg surf;

    clip.left = lpConfig->view_x;
    clip.top = lpConfig->view_y;
    clip.right = lpConfig->view_width + lpConfig->view_x;
    clip.bottom = lpConfig->view_height + lpConfig->view_y;
    GetClipping(&saved_clip);
    SetClipping(&clip);
    nextp = (struct Footprint *)&cursor->field_1414[0];
    do {
        rect = *nextp;
        if ((cursor->field_1828 & 0x10) == 0) {
            for (y = rect.y0; y <= rect.y1; y++) {
                for (x = rect.x0; x <= rect.x1; x++) {
                    tilept.x = cursor->tile_x + x;
                    tilept.y = cursor->tile_y + y;
                    GetTileBounds(&tilept, screen);
                    if (cursor->field_1828 & 6) {
                        PrintSprite(TileSpriteArray[(BasicTileSlot2 & 0xff) + *(int *)BasicTiles1Data], screen[0], screen[1], 0, 0);
                    } else {
                        if (tilept.x >= 0 && tilept.x < lpConfig->width && tilept.y >= 0 && tilept.y < lpConfig->height) {
                            tile = &GameMap[tilept.y][tilept.x];
                        } else {
                            tile = 0;
                        }
                        if (tile == 0 || (tile->flags & 0x8f8)) {
                            PrintSprite(TileSpriteArray[(BasicTileSlot3 & 0xff) + *(int *)BasicTiles1Data], screen[0], screen[1], 0xff0000, 0);
                        } else if (cursor->pad_1428[0] & 0xc) {
                            PrintSprite(TileSpriteArray[(BasicTileSlot0 & 0xff) + *(int *)BasicTiles1Data], screen[0], screen[1], 0, 0);
                        } else {
                            PrintSprite(TileSpriteArray[(BasicTileSlot1 & 0xff) + *(int *)BasicTiles1Data], screen[0], screen[1], 0, 0);
                        }
                    }
                }
            }
        }
        nextp = rect.next;
    } while (nextp != 0);
    if (GetVideoSurface(&surf) == 0) {
        return;
    }
    pts = (struct Cursor *)cursor;
    size = ((struct Sprite *)TileSpriteArray[LevelBaseTileId])->height;
    for (i = 0; i < (int)pts->count; i++) {
        GetTileBounds((struct Point *)&cursor->tile_x, screen);
        bx = (short)pts->xpts[i] + screen[0];
        by = (short)pts->ypts[i] + screen[1];
        if (cursor->field_1828 & 4) {
            pat = CursorBlueColours;
        } else if (cursor->field_1828 & 2) {
            pat = CursorCyanColours;
        } else if (pts->fpts[i] & 0xc) {
            pat = CursorGreenColours;
        } else {
            pat = CursorRedColours;
        }
        if (cursor->field_1828 & 0x20) {
            switch (pts->fpts[i] & 3) {
            case 0:
                DrawCursorDashedMarchingLine(&surf, 0, bx, by, pat, lpConfig->field_18);
                break;
            case 1:
                DrawCursorDashedMarchingLine(&surf, 1, bx, by, pat, size);
                break;
            case 2:
                DrawCursorDashedMarchingLine(&surf, 2, bx, by, pat, size);
                break;
            }
        } else {
            switch (pts->fpts[i] & 3) {
            case 0:
                DrawCursorMarchingLine(&surf, 0, bx, by, pat, lpConfig->field_18);
                break;
            case 1:
                DrawCursorMarchingLine(&surf, 1, bx, by, pat, size);
                break;
            case 2:
                DrawCursorMarchingLine(&surf, 2, bx, by, pat, size);
                break;
            }
        }
    }
    SetClipping(&saved_clip);
    if (cursor->field_1830 != 0) {
        RenderCursor(cursor->next);
    }
    if ((cursor->field_1828 & 0x400) == 0) {
        return;
    }
    editing = (struct Ride *)EditMode.unk8;
    if (HasExternalEntrance(editing) == 0) {
        return;
    }
    if (EditMode.unk0 != 1) {
        return;
    }
    tilept.x = (int)EditMode.unk8->x + cursor->tile_x;
    tilept.y = (int)EditMode.unk8->y + cursor->tile_y;
    GetTileBounds(&tilept, screen);
    code = GetEntranceSide((struct Ride *)EditMode.unk8);
    switch (code) {
    case 8:
        PrintSprite(Arrow02Sprite, screen[0], screen[1], 0, 0);
        break;
    case 4:
        PrintSprite(Arrow04Sprite, screen[0], screen[1], 0, 0);
        break;
    case 2:
        PrintSprite(Arrow03Sprite, screen[0], screen[1], 0, 0);
        break;
    case 1:
        PrintSprite(Arrow01Sprite, screen[0], screen[1], 0, 0);
        break;
    }
    if ((cursor->field_1828 & 0x800) != 0 && HasSeparateExit((struct Ride *)EditMode.unk8) != 0) {
        tilept.x = EditMode.unk8->field_24 + cursor->tile_x;
        tilept.y = EditMode.unk8->field_25 + cursor->tile_y;
        GetTileBounds(&tilept, screen);
        code = GetExitSide((struct Ride *)EditMode.unk8);
        switch (code) {
        case 0x80:
            PrintSprite(Arrow02Sprite, screen[0], screen[1], 0, 0);
            return;
        case 0x40:
            PrintSprite(Arrow04Sprite, screen[0], screen[1], 0, 0);
            return;
        case 0x20:
            PrintSprite(Arrow03Sprite, screen[0], screen[1], 0, 0);
            return;
        case 0x10:
            PrintSprite(Arrow01Sprite, screen[0], screen[1], 0, 0);
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x00460540
LEGO_EXPORT void GetTileDimensions(int *width, int *height) {
    struct Sprite *sprite;
    int size;

    sprite = TileSpriteArray[LevelBaseTileId];
    size = (short)sprite->height;
    *height = size;
    *width = size * 2;
}

/* Opens bridge `index` (0-3): for every overlay whose high byte of field_10 is index + 1, lays a 2x10 (type 0) or
 * 10x2 (type 1) strip of path tiles under it, then sets MapStats.bridge_open[index] so the bridge overlay is drawn.
 * Used by the bridges script event and the :DIGGER cheat (all four). */
// FUNCTION: LEGOLAND 0x00460560
void OpenBridge(int index) {
    struct Overlay *ov;
    struct Point pt;
    int sc[2];
    int inner;
    int outer;

    for (ov = OverlayList; ov != 0; ov = ov->next) {
        if (ov->field_10 >> 8 == index + 1) {
            sc[0] = (lpConfig->view_x - (ScrollX >> 8)) + ov->x;
            sc[1] = (lpConfig->view_y - (ScrollY >> 8)) + ov->y;
            ScreenToMapRef(sc, &pt.x, 0);
            if ((char)ov->field_10 == 0) {
                pt.x += 0xf;
                for (outer = 10; outer != 0; outer--) {
                    for (inner = 2; inner != 0; inner--) {
                        GameMap[pt.y][pt.x].field_0 = ((struct Ride *)PathControlObject)->element;
                        GameMap[pt.y][pt.x].field_10 |= 1;
                        GameMap[pt.y][pt.x].field_10 &= 0xfd;
                        GameMap[pt.y][pt.x].flags |= 0x48;
                        GameMap[pt.y][pt.x].flags &= 0x7fff;
                        GameMap[pt.y][pt.x].field_8 = *(unsigned short *)PathSprite;
                        GameMap[pt.y][pt.x].field_4 = (unsigned char)pt.x;
                        GameMap[pt.y][pt.x].field_5 = (unsigned char)pt.y;
                        AddPathTile(&pt, *(unsigned short *)PathSprite);
                        pt.x++;
                    }
                    pt.x -= 2;
                    pt.y++;
                }
            } else {
                pt.x += 10;
                pt.y += 6;
                for (outer = 2; outer != 0; outer--) {
                    for (inner = 10; inner != 0; inner--) {
                        GameMap[pt.y][pt.x].field_0 = ((struct Ride *)PathControlObject)->element;
                        GameMap[pt.y][pt.x].field_10 |= 1;
                        GameMap[pt.y][pt.x].field_10 &= 0xfd;
                        GameMap[pt.y][pt.x].flags |= 0x48;
                        GameMap[pt.y][pt.x].flags &= 0x7fff;
                        GameMap[pt.y][pt.x].field_8 = *(unsigned short *)PathSprite;
                        GameMap[pt.y][pt.x].field_4 = (unsigned char)pt.x;
                        GameMap[pt.y][pt.x].field_5 = (unsigned char)pt.y;
                        AddPathTile(&pt, *(unsigned short *)PathSprite);
                        pt.x++;
                    }
                    pt.x -= 10;
                    pt.y++;
                }
            }
        }
    }
    MapStats.bridge_open[index] = 1;
}

// FUNCTION: LEGOLAND 0x004608c0
void DrawMapTiles(struct Point *pos, RECT *clip) {
    struct Overlay *ov;
    int dx;
    int dy;
    int size;
    int dbl;
    int sx;
    int half_x;
    int half_y;
    int mx;
    int row;
    int phase;
    int which;
    int ybot;
    int xright;
    int draw_y;
    int col;
    int x2;
    int cell[2];
    int row_x;
    int row_y;
    struct MapElement *tile;
    struct Sprite *sprite;
    int frame;
    int k;
    int ox;
    int oy;

    ov = OverlayList;
    dx = pos->x - clip->left;
    dy = pos->y - clip->top;
    SetClipping(clip);
    dbl = (short)(((struct Sprite *)TileSpriteArray[LevelBaseTileId])->height * 2);
    half_x = (dbl + 1) >> 1;
    size = ((struct Sprite *)TileSpriteArray[LevelBaseTileId])->height;
    sx = pos->x / dbl;
    half_y = (size + 1) >> 1;
    mx = pos->x % dbl;
    row = (pos->y - half_y) / size;
    phase = (pos->y - half_y) % size;
    cell[0] = row + sx - 3;
    cell[1] = row - sx;
    which = (mx >= half_x) + 1;
    if (phase > half_y) {
        which += 2;
    }
    switch (which) {
    case 1:
        if (mx < half_x - phase * 2) {
            cell[0]--;
            mx += half_x;
            phase += half_y;
        }
        break;
    case 2:
        if (mx >= half_x + phase * 2) {
            cell[1]--;
            mx -= half_x;
            phase += half_y;
        }
        break;
    case 3:
        if (mx < half_x + (phase - size) * 2) {
            cell[1]++;
            mx += half_x;
            phase -= half_y;
        }
        break;
    case 4:
        if (mx >= half_x + (size - phase) * 2) {
            cell[0]++;
            mx -= half_x;
            phase -= half_y;
        }
        break;
    }
    ybot = clip->bottom + size * 2;
    xright = clip->right + dbl * 2;
    PushRenderingStatusAndLockVideoSurface();
    for (draw_y = clip->top - size * 2 - phase; draw_y < ybot; draw_y += size) {
        row_x = cell[0];
        row_y = cell[1];
        for (col = clip->left - dbl * 2 - mx; col < xright; col += dbl) {
            if (cell[0] >= 0 && cell[0] < lpConfig->width && cell[1] >= 0 && cell[1] < lpConfig->height) {
                tile = &GameMap[cell[1]][cell[0]];
            } else {
                tile = 0;
            }
            if (tile != 0 && tile->field_8 != 0) {
                if ((tile->flags & 3) && (tile->flags & 8) && tile->field_0 != 0) {
                    if (((Element *)tile->field_0)->obj->cb_sprite == 0) {
                        PrintSpriteSimple((struct Sprite *)TileSpriteArray[tile->field_8], col, draw_y);
                    }
                } else {
                    PrintSpriteSimple((struct Sprite *)TileSpriteArray[tile->field_8], col, draw_y);
                    if (IsPathTile((struct MapElement *)tile) != 0) {
                        DrawPathTile(cell, col, draw_y, 0);
                    }
                }
            }
            cell[0]++;
            if (tile != 0) {
                if (cell[0] == lpConfig->width) {
                    break;
                }
                tile++;
            }
            if (tile == 0) {
                if (cell[0] >= 0 && cell[0] < lpConfig->width && cell[1] >= 0 && cell[1] < lpConfig->height) {
                    tile = &GameMap[cell[1]][cell[0]];
                } else {
                    tile = 0;
                }
            }
            if (tile != 0 && tile->field_8 != 0) {
                if ((tile->flags & 3) && (tile->flags & 8) && tile->field_0 != 0) {
                    if (((Element *)tile->field_0)->obj->cb_sprite == 0) {
                        PrintSpriteSimple((struct Sprite *)TileSpriteArray[tile->field_8], col, draw_y);
                    }
                } else {
                    x2 = col + half_x;
                    PrintSpriteSimple((struct Sprite *)TileSpriteArray[tile->field_8], x2, draw_y + half_y);
                    if (IsPathTile((struct MapElement *)tile) != 0) {
                        DrawPathTile(cell, x2, draw_y + half_y, 0);
                    }
                }
            }
            cell[1]--;
        }
        cell[0] = row_x + 1;
        cell[1] = row_y + 1;
    }
    for (; ov != 0; ov = ov->next) {
        if (ov->sprite == 0) {
            continue;
        }
        PrintSpriteSimple(ov->sprite, ov->x - dx, ov->y - dy);
        frame = ov->field_10;
        if ((frame & 0xff00) == 0 || MapStats.bridge_open[(frame >> 8) - 1] == 0) {
            continue;
        }
        if ((frame & 0xff) == 0) {
            ox = ov->x - dx;
            oy = ov->y - dy;
            PrintSpriteSimple(BridgesData->sprites[(frame + 2) & 0xff], ox + Bridge0SpriteOffsetX,
                oy + Bridge0SpriteOffsetY);
            k = (ov->field_10 + 4) & 0xff;
            if (BridgesData->count > k) {
                sprite = BridgesData->sprites[k];
                SortSprite(sprite, ox + Bridge0OverlayOffsetX, oy + Bridge0OverlayOffsetY, (short)sprite->height + oy + Bridge0OverlayOffsetY, 0, 0);
            }
        } else if ((frame & 0xff) == 1) {
            ox = ov->x - dx;
            oy = ov->y - dy;
            PrintSpriteSimple(BridgesData->sprites[(frame + 2) & 0xff], ox + Bridge1SpriteOffsetX,
                oy + Bridge1SpriteOffsetY);
            k = (ov->field_10 + 4) & 0xff;
            if (BridgesData->count > k) {
                sprite = BridgesData->sprites[k];
                SortSprite(sprite, ox + Bridge1OverlayOffsetX, oy + Bridge1OverlayOffsetY, (short)sprite->height + oy + Bridge1OverlayOffsetY, 0, 0);
            }
        }
    }
    PopRenderingStatus();
    /* The original keeps half_x in memory (address-taken): it is reloaded after stores and is not
       strength-reduced in the column loop. Taking its address in dead code reproduces that. */
    if (0) {
        DrawPathTile(&half_x, 0, 0, 0);
    }
}

// FUNCTION: LEGOLAND 0x00460e00
void DrawViewMapTiles(void) {
    int sx;
    int sy;
    struct Point pos;
    RECT clip;

    sx = ScrollX >> 8;
    sy = ScrollY >> 8;
    pos.x = sx;
    pos.y = sy;
    clip.left = lpConfig->view_x;
    clip.top = lpConfig->view_y;
    clip.right = lpConfig->view_width + clip.left;
    clip.bottom = lpConfig->view_height + clip.top;
    DrawMapTiles(&pos, &clip);
    UnusedViewMapTileY = sy;
    UnusedViewMapTileX = sx;
    BackgroundOffsetX = 0;
    BackgroundOffsetY = 0;
    BGFullUpdate = 0;
}

// FUNCTION: LEGOLAND 0x00460e90
void DrawPathTile(int *coords, unsigned int x, unsigned int y, unsigned int param_4) {
    unsigned char dir = GetPathNeighbourMask(coords);
    unsigned int mode = GetPathDiagonalGaps(dir, coords);

    PrintSpriteSimple(TileSpriteArray[3 + (unsigned int)dir + *(int *)PathSprite], x, y);
    switch (mode) {
    case 1:
        PrintSpriteSimple(TileSpriteArray[19 + *(int *)PathSprite], x, y);
        break;
    case 2:
        PrintSpriteSimple(TileSpriteArray[20 + *(int *)PathSprite], x, y);
        break;
    case 3:
        PrintSpriteSimple(TileSpriteArray[21 + *(int *)PathSprite], x, y);
        break;
    }
}

// FUNCTION: LEGOLAND 0x00460f50
void DrawPathTileEx(int *coords, unsigned int x, unsigned int y, unsigned int param_4) {
    unsigned char dir = GetPathNeighbourMask(coords);
    unsigned int mode = GetPathDiagonalGaps(dir, coords);

    PrintSpriteSimple(TileSpriteArray[3 + (unsigned int)dir + *(int *)PathSprite], x, y);
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
void DrawDeferredSprites(void) {
    int i;

    i = 0;
    DeferredSpriteCursor = DeferredSprites;
    if (DeferredSpriteCount > 0) {
        do {
            PrintSprite(DeferredSpriteCursor->sprite, DeferredSpriteCursor->x, DeferredSpriteCursor->y, DeferredSpriteCursor->flags, 0);
            i++;
            DeferredSpriteCursor++;
        } while (i < DeferredSpriteCount);
    }
    DeferredSpriteCount = 0;
    DeferredSpriteCursor = DeferredSprites;
}

// FUNCTION: LEGOLAND 0x00461080
void DrawPathTileAt(int *coord, unsigned int param2, unsigned int param3, unsigned int param4) {
    struct MapElement *tile;

    if (coord[0] >= 0 && coord[0] < lpConfig->width &&
        coord[1] >= 0 && coord[1] < lpConfig->height) {
        tile = MapTileGrid[coord[1]];
        tile = tile + coord[0];
        if (tile != 0 && IsPathTile(tile) != 0 && tile->field_8 != 0) {
            DrawPathTileEx(coord, param2, param3, param4);
        }
    }
}

// FUNCTION: LEGOLAND 0x004610f0
void DrawPathTilesInRect(struct Point *param_1, int *param_2) {
    short size;
    int dbl;
    short half_x;
    short half_y;
    struct Point start;
    struct Point coord;
    struct Point row;
    int out[4];
    int ix;
    int iy;

    size = ((struct Sprite *)TileSpriteArray[LevelBaseTileId])->height;
    dbl = (short)(size * 2);
    half_x = (dbl + 1) >> 1;
    half_y = (size + 1) >> 1;
    start.x = param_1->x - dbl;
    param_1->x = start.x;
    start.y = param_1->y;
    coord = PlayfieldToMap(start);
    GetTileBounds(&coord, out);
    row = coord;
    for (iy = out[1]; iy < param_2[3]; iy += size) {
        for (ix = out[0]; ix < half_x + param_2[2]; ix += dbl) {
            DrawPathTileAt((int *)&coord, ix, iy, 0);
            coord.y++;
            DrawPathTileAt((int *)&coord, ix - half_x, half_y + iy, 0);
            coord.y -= 2;
            coord.x++;
        }
        row.x++;
        row.y++;
        coord = row;
    }
}

// FUNCTION: LEGOLAND 0x00461220
void DrawViewPathTiles(void) {
    int local[6];

    local[0] = ScrollX >> 8;
    local[1] = ScrollY >> 8;
    local[2] = lpConfig->view_x;
    local[3] = lpConfig->view_y;
    local[4] = lpConfig->view_width + local[2];
    local[5] = lpConfig->view_height + local[3];
    DrawPathTilesInRect((struct Point *)&local[0], &local[2]);
}

// FUNCTION: LEGOLAND 0x00461290
void ClampScrollPosition(int param_1, int param_2, int param_3, int param_4) {
    int marginTopRight; /* ScrollMarginTopRight / 2 */
    int marginBottomLeft; /* ScrollMarginBottomLeft / 2 */
    int marginTopLeft; /* ScrollMarginTopLeft / 2 */
    int marginBottomRight; /* ScrollMarginBottomRight / 2 */
    int marginTop; /* ScrollMarginTop / 2 */
    int marginBottom; /* ScrollMarginBottom / 2 */
    int marginLeft; /* ScrollMarginLeft / 2 */
    int maxX; /* ScrollMaxX / 2, then the right clamp */
    int tileW;
    int tileH;
    int rightX; /* right corner of the map diamond */
    int rightY;
    int leftX; /* left corner */
    int leftY;
    int bottomX; /* bottom corner */
    int bottomY;
    int x;
    int y;
    int over;

    marginTopRight = ScrollMarginTopRight >> 1;
    marginBottomLeft = ScrollMarginBottomLeft >> 1;
    marginTopLeft = ScrollMarginTopLeft >> 1;
    marginBottomRight = ScrollMarginBottomRight >> 1;
    marginTop = ScrollMarginTop >> 1;
    marginBottom = ScrollMarginBottom >> 1;
    marginLeft = ScrollMarginLeft >> 1;
    maxX = ScrollMaxX >> 1;
    GetTileDimensions(&tileW, &tileH);
    tileW <<= 7;
    tileH = tileW >> 1;
    rightX = lpConfig->width * tileW;
    rightY = lpConfig->width * tileH;
    leftX = -(lpConfig->height * tileW);
    leftY = lpConfig->height * tileH;
    bottomX = leftX + rightX;
    bottomY = leftY + rightY;
    x = param_3 + ScrollX;
    y = param_4 + ScrollY;
    /* clamp to the bounding box */
    maxX += rightX - param_1;
    if (x > maxX) {
        x = maxX;
    }
    if (x < leftX - marginLeft) {
        x = leftX - marginLeft;
    }
    if (y > bottomY - param_2 + marginBottom) {
        y = bottomY - param_2 + marginBottom;
    }
    if (y < -marginTop) {
        y = -marginTop;
    }
    /* push back inside each diagonal edge */
    if (y < rightY && param_1 + x > 0) {
        over = x - y * 2 - marginTopRight + param_1;
        if (over > 0) {
            x -= over >> 1;
            y += over >> 2;
        }
    }
    if (y < leftY && x < 0) {
        over = -x - y * 2 - marginTopLeft;
        if (over > 0) {
            x += over >> 1;
            y += over >> 2;
        }
    }
    if (param_2 + y > rightY && param_1 + x > bottomX) {
        over = (y - rightY + param_2) * 2 - rightX - marginBottomRight + x + param_1;
        if (over > 0) {
            x -= over >> 1;
            y -= over >> 2;
        }
    }
    if (param_2 + y > leftY && x < bottomX) {
        over = (y - leftY + param_2) * 2 - x - marginBottomLeft + leftX;
        if (over > 0) {
            x += over >> 1;
            y -= over >> 2;
        }
    }
    ScrollX = x - param_3;
    ScrollY = y - param_4;
}

// FUNCTION: LEGOLAND 0x004614a0
LEGO_EXPORT void ProcessScrolling(unsigned int a, unsigned int b) {
    ScrollX = ScrollX + a;
    ScrollY = ScrollY + b;
    ClampScrollPosition(lpConfig->view_width << 8, lpConfig->view_height << 8, 0, 0);
}

// FUNCTION: LEGOLAND 0x004614f0
LEGO_EXPORT void MouseScrollMap(void) {
    struct Point mouse;

    mouse = MousePos;
    if (mouse.x < lpConfig->scroll_border_x) {
        if (ScrollSpeedX > -(int)lpConfig->scroll_max_x) {
            ScrollSpeedX -= lpConfig->scroll_accel_x;
        }
    } else if (mouse.x > lpConfig->screen_width - lpConfig->scroll_border_x) {
        if (ScrollSpeedX < lpConfig->scroll_max_x) {
            ScrollSpeedX += lpConfig->scroll_accel_x;
        }
    } else {
        ScrollSpeedX = 0;
    }
    if (mouse.y < lpConfig->scroll_border_y) {
        if (ScrollSpeedY > -(int)lpConfig->scroll_max_y) {
            ScrollSpeedY -= lpConfig->scroll_accel_y;
        }
    } else if (mouse.y > lpConfig->screen_height - lpConfig->scroll_border_y) {
        if (ScrollSpeedY < lpConfig->scroll_max_y) {
            ScrollSpeedY += lpConfig->scroll_accel_y;
        }
    } else {
        ScrollSpeedY = 0;
    }
    ProcessScrolling((int)(LastFrameMS * ScrollSpeedX) / 0x100, (int)(LastFrameMS * ScrollSpeedY) / 0x100);
    LastMouseScrollFrameMS = LastFrameMS;
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
    struct MapElement *tile;

    tile = MapTileGrid[y >> 8];
    return tile[x >> 8].field_10;
}

// FUNCTION: LEGOLAND 0x00461630
LEGO_EXPORT unsigned char GetCurrentRFFlags(int x, int y) {
    struct MapElement *tile;
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
        tile = &MapTileGrid[ty][tx];
    }
    callback = TileSpriteInfo[tile->field_8].src->get_rf_flags;
    if (callback != 0) {
        return callback(x, y);
    }
    return 2;
}

// FUNCTION: LEGOLAND 0x004616e0
LEGO_EXPORT void Set_RFFlags(int x, int y, unsigned char value) {
    struct MapElement *tile;

    tile = MapTileGrid[y >> 8];
    tile[x >> 8].field_10 = value;
}

// FUNCTION: LEGOLAND 0x00461710
LEGO_EXPORT unsigned short Get_UserFlags(int x, int y) {
    struct MapElement *tile;

    tile = MapTileGrid[y >> 8];
    return tile[x >> 8].user_flags;
}

// FUNCTION: LEGOLAND 0x00461730
LEGO_EXPORT void Set_UserFlags(int x, int y, unsigned short value) {
    struct MapElement *tile;

    tile = MapTileGrid[y >> 8];
    tile[x >> 8].user_flags = value;
}

// FUNCTION: LEGOLAND 0x00461760
LEGO_EXPORT unsigned short Get_MapFlags(int x, int y) {
    struct MapElement *tile;

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
    MapTileGrid[y][x].field_8 = value;
}

// FUNCTION: LEGOLAND 0x004617d0
LEGO_EXPORT unsigned short GetMapFlags(int x, int y) {
    struct MapElement *tile;

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
LEGO_EXPORT void *GetObjectClassAndInstance(int *coords, unsigned short *out) {
    struct MapElement local;

    if (coords[0] >= 0 && coords[0] < lpConfig->width && coords[1] >= 0 && coords[1] < lpConfig->height) {
        local = GameMap[coords[1]][coords[0]];
        if (out != 0) {
            *out = *(unsigned short *)&local.field_4;
        }
        if ((local.flags & 0x88) != 0 && local.field_0 != 0) {
            return local.field_0->data;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004618d0
void SetBridgeParamsByName(const char *param_1) {
    // STRING: LEGOLAND 0x004b9c14
    if (strcmp(param_1, "CASTLE BRIDGES") == 0) {
        Bridge1SpriteOffsetX = 4;
        Bridge1SpriteOffsetY = 0x6d;
        Bridge0SpriteOffsetX = 0x3b;
        Bridge0SpriteOffsetY = 0x6d;
        Bridge1OverlayOffsetX = 0;
        Bridge1OverlayOffsetY = 0;
        Bridge0OverlayOffsetX = 0;
        Bridge0OverlayOffsetY = 0;
        return;
    }
    // STRING: LEGOLAND 0x004b9c00
    if (strcmp(param_1, "EXPLORER BRIDGES") == 0) {
        Bridge1SpriteOffsetY = 0x6a;
        Bridge0SpriteOffsetY = 0x6a;
        Bridge1SpriteOffsetX = 1;
        Bridge0SpriteOffsetX = 0x40;
        Bridge1OverlayOffsetX = 0xe;
        Bridge1OverlayOffsetY = 0x89;
        Bridge0OverlayOffsetX = 0x86;
        Bridge0OverlayOffsetY = 0x89;
        return;
    }
    // STRING: LEGOLAND 0x004b9bf0
    if (strcmp(param_1, "WESTERN BRIDGES") == 0) {
        Bridge1SpriteOffsetX = 2;
        Bridge1SpriteOffsetY = 0x5b;
        Bridge0SpriteOffsetY = 0x5b;
        Bridge0SpriteOffsetX = 0x3e;
        Bridge1OverlayOffsetX = 0xe;
        Bridge1OverlayOffsetY = 0x79;
        Bridge0OverlayOffsetX = 0x85;
        Bridge0OverlayOffsetY = 0x79;
    }
}

#define TILE_REF(tm, hi, lo) (*(tm)[(unsigned char)(((((hi) << 8) - 1) | (lo)) >> 8)].tiles + (unsigned short)(lo))
// FUNCTION: LEGOLAND 0x00461a50
LEGO_EXPORT unsigned int LoadBaseMap(const char *param_1) {
    struct ResFile *file;
    struct Element *elem;
    unsigned int len;
    struct TileMap *tilemap;
    int i;
    int col;
    int row;
    unsigned int blocklen; /* RLE block size; later the overlay-tile repeat count */
    int phase;
    struct MapElement *tile;
    struct Element *obj;
    int cury;
    int curx;
    char hdr[200];
    char namebuf[512];
    char strbuf[512];
    int n; /* count of placed objects */
    int idx; /* object index */
    struct Point pos;
    unsigned int rf; /* RF flag byte, widened before the call */

    if (MapDataLoaded != 0) {
        return 0xffffffff;
    }
    if (LLIDB_FindElement(param_1, &elem, 0) != 0) {
        return 0xfffffffe;
    }
    LevelMapHandle = elem;
    // STRING: LEGOLAND 0x004b9c30
    sprintf(namebuf, ".\\LevelMaps\\%s", elem->path);
    file = RES_OpenFile(namebuf);
    if (!file) {
        return 0xfffffffd;
    }
    LoadInProgress = 1;
    ResetCoverageCounters();
    RES_ReadFile(file, &len, 4);
    RES_ReadFile(file, namebuf, len);
    RES_ReadFile(file, &len, 4);
    RES_ReadFile(file, namebuf, len);
    namebuf[len] = 0;
    LLIDB_FindElement(namebuf, &elem, 0);
    LevelTileMapHandle = elem;
    tilemap = (struct TileMap *)LLIDB_LoadData((void *)elem);
    LevelBaseTileId = *(unsigned int *)tilemap[0].tiles;
    RES_ReadFile(file, &len, 4);
    RES_ReadFile(file, namebuf, len);
    namebuf[len] = 0;
    LLIDB_FindElement(namebuf, &elem, 0);
    OverlayILFHandle = elem;
    OverlayILF = LLIDB_LoadData((void *)elem);
    RES_ReadFile(file, &lpConfig->width, 2);
    RES_ReadFile(file, &lpConfig->height, 2);
    if (RES_ReadFile(file, &MapObjectElementCount, 4) != 4) {
        RES_CloseFile(file);
        MapDataLoaded = 1;
        LoadInProgress = 0;
        return 1;
    }
    for (row = 0; row < lpConfig->height; row++) {
        col = 0;
        while (col < lpConfig->width) {
            GameMap[row][col].flags = 0;
            GameMap[row][col].field_12 = 0;
            col++;
        }
    }
    MapObjectElements = malloc(MapObjectElementCount * sizeof(*MapObjectElements));
    for (i = 0; i < MapObjectElementCount; i++) {
        DrawWatchSprite();
        RES_ReadFile(file, &len, 4);
        RES_ReadFile(file, strbuf, len);
        strbuf[len] = 0;
        LLIDB_FindElement(strbuf, &elem, 0);
        MapObjectElements[i] = elem;
        LLIDB_LoadData((void *)elem);
        if (elem->data != 0) {
            elem->flags = elem->flags | 4;
        }
    }
    LLIDB_FindElement(PathControlName, &elem, 0);
    LLIDB_LoadData((void *)elem);
    PathControlObject = elem->data;
    RES_ReadFile(file, &n, 4);
    i = 0;
    while (i < n) {
        DrawWatchSprite();
        RES_ReadFile(file, &idx, 4);
        RES_ReadFile(file, &pos, 8);
        MapObjectElements[idx]->ride->footprint.next = NULL;
        obj = MapObjectElements[idx];
        PutObjOnMap(obj->ride, obj, &pos);
        i++;
    }
    RES_ReadFile(file, &curx, 4);
    BaseMapEntryCount = curx;
    MapTileSets = malloc(sizeof(*MapTileSets) * curx);
    for (i = 0; i < curx; i++) {
        DrawWatchSprite();
        RES_ReadFile(file, &len, 4);
        RES_ReadFile(file, strbuf, len);
        strbuf[len] = 0;
        LLIDB_FindElement(strbuf, &elem, 0);
        LLIDB_LoadData((void *)elem);
        MapTileSets[i] = elem->data;
        obj = MapTileSets[i]->element;
        if (obj != NULL && obj->data != NULL) {
            obj->flags = obj->flags | 4;
        }
    }

    {
        int prev; /* last tile-set selector (rle type 0) */
        int runlen;
        int rle;
        unsigned char *stream;

        curx = 0;
        cury = 0;
        prev = 0;
        RES_ReadFile(file, &blocklen, 4);
        stream = malloc(blocklen);
        RES_ReadFile(file, stream, blocklen);
        phase = 2;
        while (cury < lpConfig->height) {
            DrawWatchSprite();
            rle = stream[phase++];
            runlen = rle & 0x3f;
            if ((rle & 0xc0) != 0 && runlen == 0) {
                runlen = 0x40;
            }
            switch (rle & 0xc0) {
            case 0:
                prev = runlen;
                break;
            case 0x40:
                while (runlen--) {
                    if (prev & 0x20) {
                        GameMap[cury][curx].field_a = (unsigned short)LevelBaseTileId;
                        pos.x = curx;
                        pos.y = cury;
                        if (pos.x >= 0 && pos.x < lpConfig->width && pos.y >= 0 && pos.y < lpConfig->height) {
                            tile = &GameMap[pos.y][pos.x];
                        } else {
                            tile = 0;
                        }
                        obj = MapTileSets[prev & 0x1f]->element;
                        tile->field_0 = obj;
                        PutObjOnMap(obj->ride, obj, &pos);
                        phase++;
                    } else {
                        GameMap[cury][curx].field_a = TILE_REF(tilemap, prev, stream[phase]);
                        SetMapTile(curx, cury, TILE_REF(tilemap, prev, stream[phase]));
                        phase++;
                    }
                    curx++;
                    if (curx >= lpConfig->width) {
                        curx = 0;
                        cury = cury + 1;
                    }
                }
                break;
            case 0x80:
                while (runlen--) {
                    if (prev & 0x20) {
                        GameMap[cury][curx].field_a = (unsigned short)LevelBaseTileId;
                        pos.x = curx;
                        pos.y = cury;
                        if (pos.x >= 0 && pos.x < lpConfig->width && pos.y >= 0 && pos.y < lpConfig->height) {
                            tile = &GameMap[pos.y][pos.x];
                        } else {
                            tile = 0;
                        }
                        obj = MapTileSets[prev & 0x1f]->element;
                        tile->field_0 = obj;
                        PutObjOnMap(obj->ride, obj, &pos);
                    } else {
                        GameMap[cury][curx].field_a = TILE_REF(tilemap, prev, stream[phase]);
                        SetMapTile(curx, cury, TILE_REF(tilemap, prev, stream[phase]));
                    }
                    curx++;
                    if (curx >= lpConfig->width) {
                        curx = 0;
                        cury++;
                    }
                }
                phase = phase + 1;
                break;
            case 0xc0:
                while (runlen--) {
                    GameMap[cury][curx].field_a = 0;
                    curx++;
                    if (curx >= lpConfig->width) {
                        curx = 0;
                        cury++;
                    }
                    SetMapTile(curx, cury, 0);
                }
                break;
            }
        }
        free(stream);
    }

    {
        int runlen;
        int rle;
        int v;
        unsigned char *stream;

        curx = 0;
        cury = 0;
        RES_ReadFile(file, &blocklen, 4);
        stream = malloc(blocklen);
        RES_ReadFile(file, stream, blocklen);
        phase = 0;
        while (cury < lpConfig->height) {
            DrawWatchSprite();
            rle = stream[phase++];
            runlen = rle & 0x3f;
            if (runlen == 0) {
                runlen = 0x40;
            }
            switch (0xc0 & rle) {
            case 0x80:
                v = stream[phase++];
                while (runlen--) {
                    tile = &GameMap[cury][curx];
                    if (TileSpriteInfo[tile->field_8].sprite & 0x20) {
                        SetMapFlags(curx, cury, v | tile->flags);
                    } else {
                        SetMapFlags(curx, cury, tile->flags | v);
                    }
                    curx++;
                    if (curx >= lpConfig->width) {
                        curx = 0;
                        cury++;
                    }
                }
                break;
            case 0x40:
                while (runlen--) {
                    v = stream[phase++];
                    tile = &GameMap[cury][curx];
                    if (TileSpriteInfo[tile->field_8].sprite & 0x20) {
                        SetMapFlags(curx, cury, v | tile->flags);
                    } else {
                        SetMapFlags(curx, cury, tile->flags | v);
                    }
                    curx++;
                    if (curx >= lpConfig->width) {
                        curx = 0;
                        cury++;
                    }
                }
                break;
            }
        }
        free(stream);
    }

    {
        int runlen;
        int rle;
        unsigned char *stream;
        struct Point gfx1, sq1, gfx2, sq2;

        curx = 0;
        cury = 0;
        RES_ReadFile(file, &blocklen, 4);
        stream = malloc(blocklen);
        RES_ReadFile(file, stream, blocklen);
        phase = 0;
        while (cury < lpConfig->height) {
            DrawWatchSprite();
            rle = stream[phase++];
            runlen = rle & 0x3f;
            if (runlen == 0) {
                runlen = 0x40;
            }
            switch (rle & 0xc0) {
            case 0x80:
                while (runlen--) {
                    len = GetMapFlags(curx, cury) & 0xffff;
                    Set_RFFlags(curx << 8, cury << 8, stream[phase]);
                    if ((len & 8) == 0) {
                        if ((len & 0x10) != 0) {
                            gfx1.x = curx;
                            gfx1.y = cury;
                            AddPathTileGFX(&gfx1, *(unsigned short *)PathSprite);
                        }
                    }
                    tile = &GameMap[cury][curx];
                    if ((tile->field_10 & 1) || ((tile->flags & 0x10) && (2 & tile->field_10) == 0)) {
                        sq1.x = curx;
                        sq1.y = cury;
                        AddPathSquare(&sq1);
                    }
                    curx++;
                    if (curx >= lpConfig->width) {
                        curx = 0;
                        cury++;
                    }
                }
                phase++;
                break;
            case 0x40:
                while (runlen--) {
                    len = GetMapFlags(curx, cury) & 0xffff;
                    rf = stream[phase++];
                    Set_RFFlags(curx << 8, cury << 8, rf);
                    if ((len & 8) == 0 && (len & 0x10) != 0) {
                        gfx2.x = curx;
                        gfx2.y = cury;
                        AddPathTileGFX(&gfx2, *(unsigned short *)PathSprite);
                    }
                    tile = &GameMap[cury][curx];
                    if ((tile->field_10 & 1) || ((0x10 & tile->flags) && (2 & tile->field_10) == 0)) {
                        sq2.x = curx;
                        sq2.y = cury;
                        AddPathSquare(&sq2);
                    }
                    curx++;
                    if (curx >= lpConfig->width) {
                        curx = 0;
                        cury++;
                    }
                }
                break;
            }
        }
        free(stream);
    }

    {
        int rle;
        int runlen;
        unsigned char *stream;

        curx = 0;
        cury = 0;
        phase = 0;
        RES_ReadFile(file, &blocklen, 4);
        stream = malloc(blocklen);
        RES_ReadFile(file, stream, blocklen);
        while (cury < lpConfig->height) {
            DrawWatchSprite();
            rle = stream[phase++];
            runlen = rle & 0x3f;
            if (!runlen) {
                runlen = 0x40;
            }
            switch (rle & 0xc0) {
            case 0x80:
                while (runlen--) {
                    Set_UserFlags(curx << 8, cury << 8, stream[phase]);
                    curx++;
                    if (curx >= lpConfig->width) {
                        curx = 0;
                        cury++;
                    }
                }
                phase++;
                break;
            case 0x40:
                while (runlen--) {
                    Set_UserFlags(curx << 8, cury << 8, stream[phase++]);
                    curx++;
                    if (curx >= lpConfig->width) {
                        curx = 0;
                        cury++;
                    }
                }
                break;
            }
        }
        free(stream);
    }

    {
        int sample; /* 16-bit tile code read into a zeroed int */
        int mode;
        unsigned int filepos;
        int ovdata[5];
        int count;

        RES_ReadFile(file, &count, 4);
        for (i = 0; i < count; i++) {
            RES_ReadFile(file, ovdata, 0x14);
            AddOverlay((struct OverlayParam *)ovdata);
        }

        mode = 0;
        blocklen = 0;
        sample = 0;
        filepos = RES_GetFilePointer(file);
        hdr[0] = 0;
        memset(hdr + 1, 0, 199);
        RES_ReadFile(file, hdr, 8);
        // STRING: LEGOLAND 0x004b9c24
        if (memcmp(hdr, "BRIDGES!", 8) == 0) {
            /* The name length goes into sample (dead until the overlay loop zeroes its high half by reading 2
             * bytes over it): the original keeps both in one frame slot. */
            if (RES_ReadFile(file, &sample, 4) == 4) {
                RES_ReadFile(file, hdr, sample);
                hdr[sample] = 0;
                SetBridgeParamsByName(hdr);
                BridgesHandle = (void *)ElemID(hdr);
                BridgesData = LLIDB_LoadData(BridgesHandle);
            }
        } else {
            RES_SetFilePointer(file, filepos);
        }

        /* overlay-tile RLE: mode 0 reads a repeat byte, 1 reads a tile code, 2 skips cells */
        for (row = 0; row < lpConfig->height; row++) {
            DrawWatchSprite();
            for (col = 0; col < lpConfig->width; col++) {
                for (;;) {
                    switch (mode) {
                    case 2:
                        if (blocklen == 0) {
                            mode = 1;
                            continue;
                        }
                        blocklen--;
                        break;
                    case 0:
                        if (RES_ReadFile(file, &blocklen, 1) != 1) {
                            goto fail;
                        }
                        mode = (blocklen != 0) + 1;
                        continue;
                    case 1:
                        RES_ReadFile(file, &sample, 2);
                        if (sample == 0xffff) {
                            mode = 0;
                        } else {
                            GameMap[row][col].field_a = *tilemap[((sample - 0x100) >> 8) & 0xff].tiles + (sample & 0xff);
                        }
                        break;
                    }
                    break;
                }
            }
        }
    }
fail: /* shared exit; also taken when the overlay-tile stream ends early */
    RES_CloseFile(file);
    MapDataLoaded = 1;
    LoadInProgress = 0;
    LevelLoadedFromSave = 0;
    StartOverlayAnims();
    return 1;
}

// FUNCTION: LEGOLAND 0x004629e0
unsigned int UnloadBaseMap(void) {
    int row;
    int col;
    int off;
    int count;
    int i;
    struct Element *elem;
    struct Element *elem2;

    if (MapDataLoaded == 0) {
        return 0;
    }
    ResetCoverageCounters();
    row = 0;
    if (lpConfig->height > 0) {
        do {
            col = 0;
            if (lpConfig->width > 0) {
                off = 0;
                do {
                    col = col + 1;
                    off = off + sizeof(struct MapElement);
                    *(unsigned short *)((char *)GameMap[row] + off - sizeof(struct MapElement) + offsetof(struct MapElement, flags)) = 0;
                } while (col < (int)(unsigned int)lpConfig->width);
            }
            row = row + 1;
        } while (row < (int)(unsigned int)lpConfig->height);
    }
    LLIDB_UnLoadData(LevelTileMapHandle);
    LLIDB_UnLoadData(OverlayILFHandle);
    if (BridgesHandle != 0) {
        LLIDB_UnLoadData(BridgesHandle);
        BridgesHandle = 0;
    }
    count = LLIDB_GetCount();
    i = 0;
    if (0 < count) {
        do {
            LLIDB_GetElement(i, &elem);
            if ((elem->flags & 0x10) != 0 && (elem->flags & 1) != 0) {
                LLIDB_UnLoadData(elem);
            }
            i = i + 1;
        } while (i < count);
    }
    free(MapObjectElements);
    i = 0;
    if (0 < BaseMapEntryCount) {
        do {
            LLIDB_FindElementFromDataPtr(MapTileSets[i], &elem2, 0);
            elem2->flags = elem2->flags & 0xfffcfff1;
            LLIDB_UnLoadData(elem2);
            i = i + 1;
        } while (i < BaseMapEntryCount);
    }
    free(MapTileSets);
    ClearOverlays();
    FreeBestNodeList();
    MapDataLoaded = 0;
    return 1;
}

// FUNCTION: LEGOLAND 0x00462b30
LEGO_EXPORT void AddOvSav(struct OverlayParam *param) {
    struct Overlay *cur;
    struct Overlay *node;
    struct Sprite *sample;
    struct SpriteAnim *lls;

    cur = OverlayList;
    while (cur != 0 && cur->next != 0) {
        cur = cur->next;
    }
    if (OverlayILF != 0 && ((param->field_10 & 0xff00) == 0 || BridgesData != 0)) {
        node = (struct Overlay *)malloc(sizeof(struct Overlay));
        node->next = 0;
        if (cur != 0) {
            cur->next = node;
        } else {
            OverlayList = node;
        }
        node->x = param->x;
        node->y = param->y;
        node->param = *param;
        if (param->field_10 & 0xff00) {
            sample = BridgesData->sprites[param->field_10 & 0xff];
        } else {
            sample = OverlayILF->sprites[param->field_10 & 0xff];
        }
        node->sprite = sample;
        lls = sample->anim;
        if ((lls->type == 2 || lls->type == 3) && 1 < lls->lls->frame_count) {
            LLSPlay(lls->lls, lls);
        }
    }
}

// FUNCTION: LEGOLAND 0x00462c00
void AddOverlay(struct OverlayParam *param) {
    struct Overlay *current;
    struct Overlay *node;

    current = OverlayList;
    while (current != 0 && current->next != 0) {
        current = current->next;
    }
    if (OverlayILF != 0) {
        node = (struct Overlay *)malloc(sizeof(struct Overlay));
        node->next = 0;
        if (current != 0) {
            current->next = node;
        } else {
            OverlayList = node;
        }
        node->x = param->x;
        node->y = param->y;
        node->param = *param;
    }
}

// FUNCTION: LEGOLAND 0x00462c60
void StartOverlayAnims(void) {
    struct Overlay *node;
    struct Sprite *sample;
    struct SpriteAnim *lls;

    for (node = OverlayList; node != 0; node = node->next) {
        if (node->field_10 & 0xff00) {
            if (BridgesData != 0) {
                sample = BridgesData->sprites[*(volatile unsigned int *)&node->field_10 & 0xff];
            } else {
                node->sprite = 0;
                continue;
            }
        } else {
            sample = OverlayILF->sprites[*(volatile unsigned int *)&node->field_10 & 0xff];
        }
        node->sprite = sample;
        if (sample != 0) {
            lls = sample->anim;
            if ((lls->type == 2 || lls->type == 3) && lls->lls->frame_count > 1) {
                LLSPlay(lls->lls, lls);
            }
        }
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
LEGO_EXPORT void PlayAppropriateBuildEffect(struct Ride *obj, int *coords) {
    unsigned int flags;
    int *param;
    int buf[4];
    int i;

    if (coords != 0) {
        buf[0] = 2;
        buf[2] = coords[0];
        buf[3] = coords[1];
        param = buf;
    } else {
        param = 0;
    }
    flags = ((struct Ride *)obj)->flags;
    if (flags & 0x40000) {
        PlayInstanceOfSample(GameFX[FX_DRILL].sample, 0, 1, param);
        return;
    }
    if (flags & 0x80000) {
        if (flags & 0x200000) {
            PlayInstanceOfSample(GameFX[FX_FLOWERS].sample, 0, 1, param);
            return;
        }
        i = rand() % 5 + 3;
        PlayInstanceOfSample(GameFX[i].sample, 0, 1, param);
        return;
    }
    PlayInstanceOfSample(GameFX[FX_FLOWERS].sample, 0, 1, param);
}

// FUNCTION: LEGOLAND 0x00462dd0
LEGO_EXPORT void ResetMapAI(void) {
    memset(&MapStats, 0, sizeof(MapStats));
    MapStats.capacity = 0;
    MapStats.field_0 = 0x1fff;
    memset(MapStats.leave_ratings, 2, sizeof(MapStats.leave_ratings));
    MapStats.mood_threshold0 = 0xffffe0c0;
    MapStats.mood_threshold1 = 0xfffffc18;
    MapStats.mood_threshold2 = 100;
    MapStats.mood_threshold3 = 1000;
    MapStats.mood_threshold4 = 5000;
    MapStats.leave_rating_index = 0;
    MapStats.field_184 = 0;
}

// FUNCTION: LEGOLAND 0x00462e50
void SetClassPercent(unsigned int index, unsigned int value) {
    MapStats.classes[index].percent = value;
}

// FUNCTION: LEGOLAND 0x00462e70
void SetClassLimit(unsigned int index, unsigned int value) {
    MapStats.classes[index].limit = value;
}

// FUNCTION: LEGOLAND 0x00462e90
void SetDefaultClassLimits(void) {
    MapStats.classes[0].percent = 0x32;
    MapStats.classes[0].limit = 0x14;
    MapStats.classes[1].percent = 0x21;
    MapStats.classes[1].limit = 0x32;
    MapStats.classes[2].percent = 0;
    MapStats.classes[2].limit = 0;
    MapStats.classes[3].percent = 0;
    MapStats.classes[3].limit = 0;
    MapStats.classes[4].percent = 0x21;
    MapStats.classes[4].limit = 0x28;
    MapStats.classes[5].percent = 0x21;
    MapStats.classes[5].limit = 0x28;
}

// FUNCTION: LEGOLAND 0x00462ef0
LEGO_EXPORT void DoMapAI(void) {
    int i;
    struct MapElement *tile;
    struct Ride *obj;
    TileId id;
    unsigned char tx;
    unsigned char ty;
    int sum;
    int v;
    struct Ride *cls;

    if (FrameCounter & 0x3f) {
        for (i = 0; i < 0x100; i++) {
            switch (MapStats.scan_stage) {
            case 0: {
                int j;
                for (j = 0; j < 6; j++) {
                    MapStats.classes[j].scan_built = 0;
                    MapStats.classes[j].scan_tiles = 0;
                    MapStats.classes[j].scan_salvage = 0;
                    MapStats.classes[j].scan_capacity = 0;
                }
                MapStats.scan_x = 0;
                MapStats.scan_y = 0;
                MapStats.scan_stage++;
                break;
            }
            case 1:
                if (MapStats.scan_x >= 0 && MapStats.scan_x < lpConfig->width && MapStats.scan_y >= 0 &&
                    MapStats.scan_y < lpConfig->height) {
                    tile = &GameMap[MapStats.scan_y][MapStats.scan_x];
                } else {
                    tile = 0;
                }
                if (tile->field_10 & 1) {
                    MapStats.classes[0].scan_built++;
                    MapStats.classes[0].scan_tiles++;
                    MapStats.classes[0].scan_capacity++;
                } else if ((tile->flags & 0x88) && tile->field_0 != 0) {
                    obj = ((Element *)tile->field_0)->obj;
                    if (obj->type != 0) {
                        MapStats.classes[obj->type].scan_tiles++;
                        tx = tile->field_4;
                        if (tx == MapStats.scan_x && (ty = tile->field_5) == MapStats.scan_y) {
                            id.pos.x = tx;
                            id.pos.y = ty;
                            if (IsRideWorking(obj, (TileId *)&id) != 0) {
                                MapStats.classes[obj->type].scan_built++;
                                MapStats.classes[obj->type].scan_salvage +=
                                    GetObjSalvageValue(obj, tile->durability_level);
                                MapStats.classes[obj->type].scan_capacity += obj->seats;
                            }
                        }
                    }
                }
                MapStats.scan_x++;
                if (MapStats.scan_x >= lpConfig->width) {
                    MapStats.scan_x = 0;
                    MapStats.scan_y++;
                    if (MapStats.scan_y >= lpConfig->height) {
                        MapStats.scan_stage++;
                    }
                }
                break;
            case 2:
                MapStats.scan_stage = 0;
                MapStats.total_tiles = 0;
                for (i = 0; i < 6; i++) {
                    MapStats.classes[i].salvage = MapStats.classes[i].scan_salvage;
                    MapStats.classes[i].tiles = MapStats.classes[i].scan_tiles;
                    MapStats.classes[i].built = MapStats.classes[i].scan_built;
                    MapStats.classes[i].capacity = MapStats.classes[i].scan_capacity;
                    MapStats.total_tiles += MapStats.classes[i].tiles;
                }
                MapStats.classes[0].capacity /= 100;
                v = MapStats.classes[0].tiles * MapStats.classes[0].percent / 100;
                sum = MapStats.classes[0].limit * 100;
                if (v < sum) {
                    sum = v;
                }
                v = MapStats.classes[1].capacity * MapStats.classes[1].percent;
                if (v >= MapStats.classes[1].limit * 100) {
                    v = MapStats.classes[1].limit * 100;
                }
                sum += v;
                v = MapStats.classes[4].capacity * MapStats.classes[4].percent;
                if (v >= MapStats.classes[4].limit * 100) {
                    v = MapStats.classes[4].limit * 100;
                }
                sum += v;
                v = MapStats.classes[5].capacity * MapStats.classes[5].percent;
                if (v >= MapStats.classes[5].limit * 100) {
                    v = MapStats.classes[5].limit * 100;
                }
                MapStats.capacity = (v + sum) / 100;
                if ((int)MapStats.capacity < (int)MapStats.capacity_min) {
                    MapStats.capacity = MapStats.capacity_min;
                }
                if ((int)MapStats.capacity > (int)MapStats.capacity_max) {
                    MapStats.capacity = MapStats.capacity_max;
                }
                if ((int)MapStats.capacity > (int)lpConfig->max_blokes) {
                    MapStats.capacity = lpConfig->max_blokes;
                }
                break;
            }
        }
    } else {
        for (i = 0; i < 6; i++) {
            if (i != 0) {
                MapStats.classes[i].classes = 0;
            }
        }
        for (cls = (struct Ride *)ObjectClassList; cls != 0; cls = cls->next) {
            if (cls->type != 0 && cls->field_8 != 0) {
                MapStats.classes[cls->type].classes++;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x004632b0
void PrintCapacityStats(void) {
    int total;
    int i;
    int prod;
    int capped;
    char buf[500];

    total = 0;
    for (i = 0; i < 6; i++) {
        prod = MapStats.classes[i].capacity * MapStats.classes[i].percent;
        if (prod < MapStats.classes[i].limit * 100) {
            capped = prod;
        } else {
            capped = MapStats.classes[i].limit * 100;
        }
        // STRING: LEGOLAND 0x004b9c6c
        sprintf(buf, "[%s x %d]: Cap %d  x %d%% = %.2f (Capped %d) = %.2f", ((char **)&ScriptCapacityClassNames)[i], MapStats.classes[i].built, MapStats.classes[i].capacity, MapStats.classes[i].percent,
            (double)((float)prod * FloatOnePercent), MapStats.classes[i].limit, (double)((float)capped * FloatOnePercent));
        Print(SPRITE_ClipRect.left + 8, (i + 1) * 0x14 + SPRITE_ClipRect.top, buf, 2);
        total += capped;
    }
    // STRING: LEGOLAND 0x004b9c40
    sprintf(buf, "Tot Capacity = %.2f (limit %d - %d) = %d", (double)((float)total * FloatOnePercent),
        MapStats.capacity_min, MapStats.capacity_max, MapStats.capacity);
    Print(SPRITE_ClipRect.left + 8, SPRITE_ClipRect.top + 0x96, buf, 2);
}

// FUNCTION: LEGOLAND 0x004633f0
LEGO_EXPORT void RateBlokeOnLeaving(int param_1) {
    int rating;

    if (param_1 < MapStats.mood_threshold0) {
        rating = 0;
    } else if (param_1 < MapStats.mood_threshold2) {
        rating = 1;
    } else if (param_1 < MapStats.mood_threshold3) {
        rating = 2;
    } else {
        rating = (param_1 >= MapStats.mood_threshold4) + 3;
    }
    MapStats.leave_ratings[MapStats.leave_rating_index] = (char)rating;
    MapStats.leave_rating_index = MapStats.leave_rating_index + 1;
    if (MapStats.leave_rating_index == 0x19) {
        MapStats.leave_rating_index = 0;
    }
}

// FUNCTION: LEGOLAND 0x00463460
void DegradeObject(struct MapElement *tile, struct Point *pos) {
    unsigned int flags;
    Ride *ride;
    int power;
    volatile int threshold;

    int ptmp5;
    if (tile->durability_level != 0) {
        ride = tile->field_0->ride;
        flags = tile->flags;
        tile->flags &= 0xfdff;
        ptmp5 = ride->durability;
        threshold = ptmp5 >> 2;
        if (tile->durability_level < threshold) {
            if (tile->flags & 4) {
                tile->flags |= 0x200;
                tile->durability_level = threshold;
                return;
            }
            if (lpConfig->repair_orders_enabled != 0 && (tile->flags & 0x4000) == 0) {
                if (AddRepairOrderForObject(ride, *pos) != 0) {
                    tile->flags |= 0x4000;
                }
            }
            if ((flags & 0x200) == 0) {
                power = FindObjectsPower(ride);
                if (power > 0) {
                    MapStats.power_supply -= power;
                    if (MapStats.power_demand - (int)MapStats.unpowered_demand > MapStats.power_supply) {
                        CutPowerToObjects();
                    }
                }
            }
            tile->flags |= 0x200;
        }
    }
}

// FUNCTION: LEGOLAND 0x00463520
int IsRideWearDue(void) {
    int curr;
    int diff;
    int quotient;

    if (MapStats.ride_wear == 0) {
        return 0;
    }
    curr = GetGameTimerFrames();
    diff = curr - LastRideWearFrame;
    quotient = 0x168 / (int)MapStats.ride_wear;
    if (diff > quotient) {
        LastRideWearFrame = GetGameTimerFrames();
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00463560
void ResetDamageTimer(void) {
    DamageTimerStart = GetGameTimer();
    LastRideWearFrame = GetGameTimerFrames();
}

// FUNCTION: LEGOLAND 0x00463580
LEGO_EXPORT void ProcessDamage(void) {
    TileId id;
    struct Point pt;
    int amount;
    struct MapElement *tile;

    id.id = RenderOrderFirstAnchor;
    amount = IsRideWearDue();
    if (amount != 0) {
        if (id.id == 0) {
            pt.x = 0;
            pt.y = 0;
            if (pt.x >= 0 && pt.x < lpConfig->width && pt.y >= 0 && pt.y < lpConfig->height) {
                tile = &GameMap[pt.y][pt.x];
            } else {
                tile = 0;
            }
            if ((tile->flags & 0xa0) == 0) {
                return;
            }
        }
        do {
            pt.x = id.pos.x;
            pt.y = id.pos.y;
            if (pt.x >= 0 && pt.x < lpConfig->width && pt.y >= 0 && pt.y < lpConfig->height) {
                tile = &GameMap[pt.y][pt.x];
            } else {
                tile = 0;
            }
            if (tile->flags & 0x80) {
                if (tile->durability_level > 1) {
                    if (tile->durability_level > amount) {
                        tile->durability_level -= amount;
                    } else {
                        tile->durability_level = 1;
                    }
                }
                DegradeObject(tile, &pt);
            }
            id = tile->next;
        } while (id.id != 0);
    }
}

// FUNCTION: LEGOLAND 0x00463680
void ClearGameMap(void) {
    int x;
    int y;

    for (y = 0; y < 0x100; y++) {
        for (x = 0; x < 0x100; x++) {
            memset(&GameMap[y][x], 0, sizeof(struct MapElement));
        }
    }
    ClearOverlays();
}

// FUNCTION: LEGOLAND 0x004636c0
int GetMapTileCount(void) {
    int total;
    int rows;

    total = 0;
    rows = lpConfig->height;
    if (rows > 0) {
        int width = lpConfig->width;
        do {
            if (width > 0) {
                total = total + width;
            }
            rows = rows - 1;
        } while (rows != 0);
    }
    return total;
}
