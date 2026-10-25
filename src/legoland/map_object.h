#pragma once

#include "legoland.h"
#include "llidb.h"
#include "obj_instance.h"

// Per-TU header for map_object.c — canonical declarations for functions that are
// pre-declared elsewhere (called/address-taken from another TU). map_object already
// owns struct MapTile etc.; struct Cursor lives in gamemap.h.

struct Cursor;
struct Element;
struct Point;
struct ObjInstance;

struct VideoArg;
LEGO_EXPORT void RenderCursor(struct Cursor *cursor);
struct OverlayParam {
    unsigned int x;
    unsigned int y;
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
};
void AddOverlay(struct OverlayParam *param);
void StartOverlayAnims(void);
struct WorkOrder;
LEGO_EXPORT void RateBlokeOnLeaving(int param_1);
LEGO_EXPORT void SetMapFlags(int x, int y, short value);
LEGO_EXPORT void Set_RFFlags(int x, int y, unsigned char value);
LEGO_EXPORT void ApplyConsTileMap(Element *editObj, TileId coords);
LEGO_EXPORT void ApplyDestrTileMap(Element *editObj, TileId coords);
LEGO_EXPORT int BuildObject(Element *editObj, int *coords);
struct Ride;
LEGO_EXPORT void ObjectIsBuilt(struct Ride *obj, TileId coords);
LEGO_EXPORT void ObjectIsBuilding(struct Ride *obj, TileId coords);
LEGO_EXPORT void PlayAppropriateBuildEffect(struct Ride *obj, int *coords);
void RemovePathsUnderObjectFootprint(Element *editObj, struct Point *pos);
void SetObjectFootprintMapTiles(Element *editObj, struct Point *pos);
LEGO_EXPORT unsigned int AddBasicObject(Element *editObj, int *coords);
LEGO_EXPORT void AddObjectToMap(Element *param_1, TileId param_2, int param_3);
LEGO_EXPORT void RemoveObjectFromMap(TileId coords);
void ClearObjectEntrances(struct Element *node, int *offset);
LEGO_EXPORT void *GetObjectClassAndInstance(int *coords, unsigned short *out);
void SetBridgeParamsByName(const char *param_1);
LEGO_EXPORT void StandardRemoveObject(Element *editObj, TileId coords, struct Cursor *cursor);
LEGO_EXPORT void SetEditCursorFootPrint(void *src);
int IsCursorBuildable(struct Cursor *cursor);
LEGO_EXPORT void SetObjRectFlags(Element *editObj, struct Point *pos, unsigned short flags);
void ResetCursorBuildable(struct Cursor *cursor);
void SetCursorBlocked(struct Cursor *cursor, int param);
void PropagateCursorBlocked(struct Cursor *cursor);
struct Cursor *GetOrAddPathCursor(struct Cursor *cursor);
void DrawCursorMarchingLine(struct VideoArg *surface, int dir, int px, int py, unsigned char *color, int count);
void DrawCursorDashedMarchingLine(struct VideoArg *surface, int dir, int px, int py, unsigned char *color, int count);
LEGO_EXPORT void BuildCursorPtr(struct Cursor *cursor, unsigned int param_2, int param_3);
LEGO_EXPORT void ValidateCursor(struct Cursor *cursor, void *param);
LEGO_EXPORT void CalcBasicObjectCursor(struct Element *obj, int *a2, unsigned int a3);
LEGO_EXPORT void GetTileDimensions(int *width, int *height);
LEGO_EXPORT int Get_XScroll(void);
LEGO_EXPORT int Get_YScroll(void);
void OpenBridge(int index);
void DrawPathTile(int *coords, unsigned int x, unsigned int y, unsigned int param_4);
void ClampScrollPosition(int param_1, int param_2, int param_3, int param_4);
LEGO_EXPORT void MouseScrollMap(void);
LEGO_EXPORT void ProcessScrolling(unsigned int a, unsigned int b);
LEGO_EXPORT unsigned char Get_RFFlags(int x, int y);
LEGO_EXPORT unsigned char GetCurrentRFFlags(int x, int y);
LEGO_EXPORT unsigned short Get_MapFlags(int x, int y);
LEGO_EXPORT unsigned short Get_UserFlags(int x, int y);
LEGO_EXPORT void Set_UserFlags(int x, int y, unsigned short value);
LEGO_EXPORT void SetMapTile(int x, int y, unsigned short value);
LEGO_EXPORT unsigned int LoadBaseMap(const char *param_1);
LEGO_EXPORT void AddOvSav(struct OverlayParam *param);
LEGO_EXPORT void ResetMapAI(void);
unsigned int UnloadBaseMap(void);
LEGO_EXPORT void ClearOverlays(void);
void SetClassPercent(unsigned int index, unsigned int value);
void SetClassLimit(unsigned int index, unsigned int value);
void SetDefaultClassLimits(void);
struct MapElement;
void DegradeObject(struct MapElement *tile, struct Point *pos);
void ResetDamageTimer(void);
void ClearGameMap(void);
int GetMapTileCount(void);
LEGO_EXPORT void DoMapAI(void);
void PrintCapacityStats(void);
LEGO_EXPORT void ProcessDamage(void);
void DrawViewMapTiles(void);
void DrawDeferredSprites(void);
void DrawViewPathTiles(void);
int IsObjectTypeNot0Or2(struct Ride *obj);
