#include "legoland.h"
#include <stdlib.h>
#include <string.h>
#include "globals.h"

#include "gamemap.h"
#include "map_object.h"
#include "print_sprite.h"
#include "timer.h"
#include "tilemap.h"

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
    unsigned int field_c;
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


// FUNCTION: LEGOLAND 0x0045dd80
LEGO_EXPORT void AddObjectToMap(int param_1, unsigned short param_2, int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x0045dee0
LEGO_EXPORT void SetObjRectFlags(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045e080
void FUN_0045e080(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045e300
void FUN_0045e300(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045e4a0
void FUN_0045e4a0(int element, void *data) { STUB(); }

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
LEGO_EXPORT void BuildObject(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045ed30
LEGO_EXPORT void ObjectIsBuilt(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045ef50
LEGO_EXPORT void ObjectIsBuilding(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045efc0
LEGO_EXPORT void ApplyConsTileMap(void) {
}

// FUNCTION: LEGOLAND 0x0045efd0
LEGO_EXPORT void ApplyDestrTileMap(void) {
}

// FUNCTION: LEGOLAND 0x0045efe0
LEGO_EXPORT unsigned int AddBasicObject(unsigned int param1, unsigned int param2) { STUB(); }

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
LEGO_EXPORT void PlayAppropriateBuildEffect(void) { STUB(); }

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
void FUN_004636c0(void) { STUB(); }
