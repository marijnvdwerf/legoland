#include "path_control.h"
#include "bricks.h"
#include "globals.h"
#include "legoland.h"
#include "map_object.h"
#include "objclass.h"
#include "print_sprite.h"
#include "tilemap.h"

struct PathSpriteHeader {
    unsigned short id;
};

struct PathEditObject {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

struct PathCursor {
    unsigned char pad_0[0x1404];
    int coords[2];
};

// FUNCTION: LEGOLAND 0x0045dbe0
LEGO_EXPORT void AddBasicPath(struct EditObject *editObj, int *coords) {
    struct MapElement *cell;

    if (coords[0] < 0 || coords[0] >= (int)lpConfig->width)
        return;
    if (coords[1] < 0 || coords[1] >= (int)lpConfig->height)
        return;

    cell = &GameMap[coords[1]][coords[0]];
    if (cell == NULL)
        return;
    if (cell->flags & 0x8e8)
        return;

    AddBasicObject(editObj, coords);
    AddPathTile((struct Point *)coords, ((struct PathSpriteHeader *)PathSprite)->id);
}

// FUNCTION: LEGOLAND 0x0045dc50
LEGO_EXPORT void AddRollerCoasterPath(int *coords) {
    struct MapElement *cell;

    cell = &GameMap[coords[1]][coords[0]];
    cell->field_8 = ((struct PathSpriteHeader *)PathSprite)->id;
    AddPathTile((struct Point *)coords, ((struct PathSpriteHeader *)PathSprite)->id);
}

// FUNCTION: LEGOLAND 0x0045dc90
LEGO_EXPORT void RemoveBasicPath(struct PathEditObject *editObj, int dummy, struct PathCursor *cursor) {
    unsigned int obj_val = editObj->field_c;
    AddBricks(GetObjSalvageValue(obj_val, 0));
    RemovePathTile(cursor->coords, ((struct PathSpriteHeader *)PathSprite)->id);
}

// FUNCTION: LEGOLAND 0x0045dcd0
LEGO_EXPORT void RemoveRollerCoasterPath(int *coords) {
    RemovePathTile(coords, ((struct PathSpriteHeader *)PathSprite)->id);
}

// FUNCTION: LEGOLAND 0x0045dcf0
LEGO_EXPORT void DrawBasicPath(unsigned int idx, unsigned int x, unsigned int y, unsigned int mode) {
    unsigned int src_idx;

    PrintSprite((struct Sprite *)TileSpriteArray[idx], x, y, 0, 0);

    src_idx = *(unsigned int *)TileSpriteInfo[idx].src;

    switch (mode & 3) {
    case 1:
        PrintSprite((struct Sprite *)TileSpriteArray[16 + src_idx], x, y, 0, 0);
        break;
    case 2:
        PrintSprite((struct Sprite *)TileSpriteArray[17 + src_idx], x, y, 0, 0);
        break;
    case 3:
        PrintSprite((struct Sprite *)TileSpriteArray[18 + src_idx], x, y, 0, 0);
        break;
    }
}
