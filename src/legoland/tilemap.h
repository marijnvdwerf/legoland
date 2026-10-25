#pragma once

#include "legoland.h"

struct MapTile;
struct FXSpriteList;
int CollectTileSpriteLists(struct FXSpriteList **out);
struct Point;
struct Ride;
struct Cursor;

LEGO_EXPORT void RestoreBaseMap(int tile_x, int row_y);
LEGO_EXPORT void AdjustTileRFFlags(int *param_1);
void LayCursorPathFootprint(struct Cursor *param_1);
LEGO_EXPORT void GetTileBounds(struct Point *ref, int *out);
LEGO_EXPORT void GetTileCentre(struct Point *ref, int *out);
LEGO_EXPORT void FreeTileSpace(unsigned short index, unsigned short count);
LEGO_EXPORT unsigned int *AllocTileSpace(void *manager, int count, unsigned int *out);
LEGO_EXPORT void PointToIsoPlane(int *param_1, int *out);
LEGO_EXPORT unsigned int ScreenToMapRef(int *param_1, int *out, unsigned int param_3);
LEGO_EXPORT unsigned int ScreenToMapRef2(struct Point *screen, struct Point *out);
int IsPathTile(struct MapElement *tile);
int IsPavedPathTile(int *param_1);
unsigned char GetPathNeighbourMask(int *coords);
unsigned char GetPathDiagonalGaps(unsigned char flags, int *coords);
void AdjustPathTileAndNeighbours(struct Point *param);
LEGO_EXPORT void RemovePathTile(int *param_1, unsigned short param_2);
void ClearPathUnderFootprint(struct Ride *param_1, int *param_2);
LEGO_EXPORT unsigned char Bit_To_Dir(unsigned char bit);
LEGO_EXPORT unsigned char Dir_To_Bit(unsigned char param);
LEGO_EXPORT unsigned char Get_Path_Directions(struct Point *pos, char *param_2, char *param_3);
LEGO_EXPORT unsigned char ExcludeIsolatedDiags(unsigned char param);
LEGO_EXPORT void AddPathTile(struct Point *p, unsigned short param1);
LEGO_EXPORT void AddPathTileGFX(struct Point *p, unsigned short param1);
unsigned int UnloadMapTiles(void);
LEGO_EXPORT unsigned int LoadMapTiles(void);
void Empty_0045b170(struct Point *pt);
LEGO_EXPORT void RenderView(void);
