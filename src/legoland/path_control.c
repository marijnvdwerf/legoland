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

// FUNCTION: LEGOLAND 0x0045dbe0
LEGO_EXPORT void AddBasicPath(struct Element *editObj, int *coords) {
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
LEGO_EXPORT void RemoveBasicPath(struct Element *editObj, int dummy, struct Cursor *cursor) {
    struct Ride *obj_val = editObj->ride;
    AddBricks(GetObjSalvageValue(obj_val, 0));
    RemovePathTile(&cursor->tile_x, ((struct PathSpriteHeader *)PathSprite)->id);
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
