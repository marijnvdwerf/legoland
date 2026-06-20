#pragma once

#include "legoland.h"

struct MapTile;
struct Point;
struct PathFootprint;
struct Cursor;

LEGO_EXPORT unsigned short RestoreBaseMap(int tile_x, int row_y);
void FUN_0045d770(struct Cursor *param_1);
LEGO_EXPORT void GetTileBounds(struct Point *ref, int *out);
LEGO_EXPORT void GetTileCentre(struct Point *ref, int *out);
LEGO_EXPORT void FreeTileSpace(unsigned short index, unsigned short count);
LEGO_EXPORT unsigned int *AllocTileSpace(void *manager, int count, unsigned int *out);
LEGO_EXPORT unsigned int ScreenToMapRef(int *param_1, int *out, unsigned int param_3);
LEGO_EXPORT unsigned int ScreenToMapRef2(int *param_1, int *out);
int FUN_0045ce10(struct MapTile *tile);
int FUN_0045ce30(int *param_1);
unsigned char FUN_0045ceb0(int *coords);
unsigned char FUN_0045d080(unsigned char flags, int *coords);
void FUN_0045d260(struct Point *param);
LEGO_EXPORT void RemovePathTile(int *param_1, unsigned short param_2);
void FUN_0045d3d0(struct PathFootprint *param_1, int *param_2);
LEGO_EXPORT unsigned char Bit_To_Dir(unsigned char bit);
LEGO_EXPORT unsigned char Dir_To_Bit(unsigned char param);
LEGO_EXPORT unsigned char Get_Path_Directions(int *param_1, char *param_2, char *param_3);
LEGO_EXPORT unsigned char ExcludeIsolatedDiags(unsigned char param);
LEGO_EXPORT void AddPathTile(struct Point *p, unsigned short param1);
LEGO_EXPORT void AddPathTileGFX(struct Point *p, unsigned short param1);
