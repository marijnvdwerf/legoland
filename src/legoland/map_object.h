#pragma once

#include "legoland.h"

// Per-TU header for map_object.c — canonical declarations for functions that are
// pre-declared elsewhere (called/address-taken from another TU). map_object already
// owns struct MapTile etc.; struct Cursor lives in gamemap.h.

struct Cursor;
struct CursorObj;
struct EditObject;
struct Point;
struct ObjClass;
struct ObjInstance;

int AddObjectToBuildList();
void FUN_00477bd0(int x, int y, int a, int b);
unsigned int *FUN_00482b00(void);
void AddInstanceToList();
int CheckForPeople();
void AddPathTile(struct Point *p, unsigned short param1);
void AddPathTileGFX(struct Point *p, unsigned short param1);
void FUN_004663f0(void);
struct OverlayParam;
void FUN_00462c00(struct OverlayParam *param);
void FUN_00462c60(void);
struct ObjClass;
struct WorkOrder;
LEGO_EXPORT struct WorkOrder *AddRepairOrderForObject(struct ObjClass *cls, char x, unsigned char y);
LEGO_EXPORT void Print(int x, int y, const char *text, int font);
void RemoveInstanceFromList();
void RestoreBaseMap();
LEGO_EXPORT struct Point PlayfieldToMap(int a, int b);
LEGO_EXPORT void SetMapFlags(int x, int y, short value);
LEGO_EXPORT void Set_RFFlags(int x, int y, unsigned char value);
LEGO_EXPORT void ApplyConsTileMap(struct EditObject *editObj, unsigned int coords);
LEGO_EXPORT void ApplyDestrTileMap(struct EditObject *editObj, unsigned int coords);
LEGO_EXPORT int BuildObject(struct EditObject *editObj, int *coords);
LEGO_EXPORT void ObjectIsBuilt(struct ObjClass *obj, unsigned int coords);
LEGO_EXPORT void ObjectIsBuilding(struct ObjClass *obj, unsigned int coords);
LEGO_EXPORT void PlayAppropriateBuildEffect(struct ObjClass *obj, int *coords);
void FUN_0045e300(struct EditObject *editObj, struct Point *pos);
void FUN_0045e4a0(struct EditObject *editObj, struct Point *pos);
LEGO_EXPORT unsigned int AddBasicObject(struct EditObject *editObj, int *coords);
LEGO_EXPORT void AddObjectToMap(struct EditObject *param_1, unsigned int param_2, int param_3);
LEGO_EXPORT void RemoveObjectFromMap(unsigned int coords);
LEGO_EXPORT unsigned int GetObjectClassAndInstance(int *coords, unsigned short *out);
void FUN_004618d0(const char *param_1);
LEGO_EXPORT void StandardRemoveObject(struct EditObject *editObj, unsigned int coords, struct Cursor *cursor);
LEGO_EXPORT void SetEditCursorFootPrint(void *src);
unsigned char FUN_0045f4b0(struct Cursor *cursor);
LEGO_EXPORT void SetObjRectFlags();
void FUN_0045f460(struct Cursor *cursor);
void FUN_0045f480(struct Cursor *cursor, int param);
void FUN_0045f4d0(struct Cursor *cursor);
struct Cursor *FUN_0045f540(struct Cursor *cursor);
void FUN_0045fad0(int *surface, int dir, int px, int py, unsigned char *color, int count);
void FUN_0045fca0(int *surface, int dir, int px, int py, unsigned char *color, int count);
LEGO_EXPORT void BuildCursorPtr(struct Cursor *cursor, unsigned int param_2, int param_3);
LEGO_EXPORT void ValidateCursor(struct Cursor *cursor, unsigned int param);
LEGO_EXPORT void CalcBasicObjectCursor(struct CursorObj *obj, unsigned int a2, unsigned int a3);
LEGO_EXPORT void GetTileDimensions(int *width, int *height);
LEGO_EXPORT int Get_XScroll(void);
LEGO_EXPORT int Get_YScroll(void);
void FUN_00460560(int index);
void FUN_00460e90(int *coords, unsigned int x, unsigned int y, unsigned int param_4);
void FUN_00461290(int param_1, int param_2, int param_3, int param_4);
LEGO_EXPORT void MouseScrollMap(void);
LEGO_EXPORT void ProcessScrolling(unsigned int a, unsigned int b);
LEGO_EXPORT unsigned char Get_RFFlags(int x, int y);
LEGO_EXPORT short Get_UserFlags(int x, int y);
LEGO_EXPORT void Set_UserFlags(int x, int y, unsigned short value);
LEGO_EXPORT void SetMapTile(int x, int y, unsigned short value);
LEGO_EXPORT unsigned int LoadBaseMap(unsigned int param_1);
struct OverlayParam;
LEGO_EXPORT void AddOvSav(struct OverlayParam *param);
unsigned int FUN_004629e0(void);
void FUN_00459880(void);
void FUN_004828f0(void);
LEGO_EXPORT void ClearOverlays(void);
void FUN_00462e50(unsigned int index, unsigned int value);
void FUN_00462e70(unsigned int index, unsigned int value);
void FUN_00462e90(void);
void FUN_00463560(void);
int FUN_004636c0(void);
