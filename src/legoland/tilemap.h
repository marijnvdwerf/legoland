#pragma once

#include "legoland.h"

struct MapTile;

LEGO_EXPORT void FreeTileSpace(unsigned short index, unsigned short count);
LEGO_EXPORT void ScreenToMapRef(unsigned int x, void *out, unsigned int y);
int FUN_0045ce10(struct MapTile *tile);
LEGO_EXPORT void RemovePathTile(unsigned int param_1, unsigned short param_2);
