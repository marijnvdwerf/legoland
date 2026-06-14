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

void IncrementObjectCount();
void DecrementObjectCount();
int AddObjectToBuildList(struct ObjClass *obj, short id);
void FUN_00477bd0(int x, int y, int a, int b);
unsigned int *FUN_00482b00(void);
int CreateObjectInstance(unsigned int obj, unsigned short *id);
void AddInstanceToList(int instance);
unsigned int GetObjSalvageValue(unsigned int obj, unsigned int salvage);
int CheckForPeople(void *rect);
void AddPathTile(struct Point *p, unsigned short param1);
int AddRepairOrderForObject(int obj, int x, int y);
int GetInstanceOfClass(unsigned int obj, unsigned short *uid);
void RemoveInstanceFromList(int node);
LEGO_EXPORT unsigned short RestoreBaseMap(int tile_x, int row_y);
LEGO_EXPORT void SetMapFlags(int x, int y, short value);
LEGO_EXPORT void Set_RFFlags(int x, int y, unsigned char value);
LEGO_EXPORT void ApplyConsTileMap(struct EditObject *editObj, unsigned int coords);
LEGO_EXPORT void ApplyDestrTileMap(struct EditObject *editObj, unsigned int coords);
LEGO_EXPORT int BuildObject(struct EditObject *editObj, int *coords);
LEGO_EXPORT void ObjectIsBuilt(struct ObjClass *obj, unsigned int coords);
LEGO_EXPORT void ObjectIsBuilding(struct ObjClass *obj, unsigned int coords);
LEGO_EXPORT void PlayAppropriateBuildEffect(struct ObjClass *obj, int *coords);
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
LEGO_EXPORT void RenderCursor(struct Cursor *cursor);
LEGO_EXPORT void BuildCursorPtr(struct Cursor *cursor, unsigned int param_2, int param_3);
LEGO_EXPORT void ValidateCursor(struct Cursor *cursor, unsigned int param);
LEGO_EXPORT void CalcBasicObjectCursor(struct CursorObj *obj, unsigned int a2, unsigned int a3);
LEGO_EXPORT void GetTileDimensions(int *width, int *height);
LEGO_EXPORT int Get_XScroll(void);
LEGO_EXPORT int Get_YScroll(void);
void FUN_00460560(int index);
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
void FUN_004629e0(void);
void FUN_00462e50(unsigned int index, unsigned int value);
void FUN_00462e70(unsigned int index, unsigned int value);
void FUN_00462e90(void);
void FUN_00463560(void);
int FUN_004636c0(void);
