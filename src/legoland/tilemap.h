#pragma once

#include "legoland.h"

struct MapTile;
struct Point;
struct PathFootprint;

LEGO_EXPORT void GetTileBounds(struct Point *ref, int *out);
LEGO_EXPORT void GetTileCentre(struct Point *ref, int *out);
LEGO_EXPORT void FreeTileSpace(unsigned short index, unsigned short count);
LEGO_EXPORT unsigned int *AllocTileSpace(void *manager, int count, unsigned int *out);
LEGO_EXPORT unsigned int ScreenToMapRef();
LEGO_EXPORT unsigned int ScreenToMapRef2();
int FUN_0045ce10(struct MapTile *tile);
int FUN_0045ce30(int *param_1);
unsigned char FUN_0045ceb0(int *coords);
unsigned char FUN_0045d080(unsigned char flags, int *coords);
void FUN_0045d260(struct Point *param);
LEGO_EXPORT void RemovePathTile(int *param_1, unsigned short param_2);
void FUN_0045d3d0(struct PathFootprint *param_1, int *param_2);
LEGO_EXPORT unsigned char Bit_To_Dir(unsigned char bit);
