#include "legoland.h"
#include "path_control.h"
#include "tilemap.h"

struct PathSpriteHeader {
    unsigned short id;
};

extern struct PathSpriteHeader *PathSprite;

// FUNCTION: LEGOLAND 0x0045dbe0
void AddBasicPath(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045dc50
void AddRollerCoasterPath(int *coords) { STUB(); }

// FUNCTION: LEGOLAND 0x0045dc90
void RemoveBasicPath(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0045dcd0
void RemoveRollerCoasterPath(const struct Struct427f70Tmp *tmp) {
    RemovePathTile((unsigned int)tmp, PathSprite->id);
}

// FUNCTION: LEGOLAND 0x0045dcf0
void DrawBasicPath(void) { STUB(); }
