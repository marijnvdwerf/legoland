#pragma once

#include "legoland.h"

struct MapTile;
struct Point;

LEGO_EXPORT void GetTileBounds(struct Point *ref, int *out);
LEGO_EXPORT void FreeTileSpace(unsigned short index, unsigned short count);
LEGO_EXPORT unsigned int *AllocTileSpace(void *manager, int count, unsigned int *out);
LEGO_EXPORT void ScreenToMapRef(unsigned int x, void *out, unsigned int y);
int FUN_0045ce10(struct MapTile *tile);
unsigned char FUN_0045ceb0(int *coords);
unsigned char FUN_0045d080(unsigned char flags, int *coords);
LEGO_EXPORT void RemovePathTile(unsigned int param_1, unsigned short param_2);
