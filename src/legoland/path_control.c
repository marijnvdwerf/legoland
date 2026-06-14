#include "path_control.h"
#include "globals.h"
#include "legoland.h"
#include "tilemap.h"

struct PathSpriteHeader {
    unsigned short id;
};

// FUNCTION: LEGOLAND 0x0045dbe0
LEGO_EXPORT void AddBasicPath(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045dc50
LEGO_EXPORT void AddRollerCoasterPath(int *coords) { STUB(); }

// FUNCTION: LEGOLAND 0x0045dc90
LEGO_EXPORT void RemoveBasicPath(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045dcd0
LEGO_EXPORT void RemoveRollerCoasterPath(int *coords) {
    RemovePathTile(coords, ((struct PathSpriteHeader *)PathSprite)->id);
}

// FUNCTION: LEGOLAND 0x0045dcf0
LEGO_EXPORT unsigned int *DrawBasicPath(void) { STUB(); }
