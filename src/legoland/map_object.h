#pragma once

#include "legoland.h"

// Per-TU header for map_object.c — canonical declarations for functions that are
// pre-declared elsewhere (called/address-taken from another TU). map_object already
// owns struct MapTile etc.; struct Cursor lives in gamemap.h.

struct Cursor;
struct CursorObj;
struct EditObject;

void IncrementObjectCount(void *obj);
void FUN_0045e4a0(int element, void *data);
LEGO_EXPORT unsigned int AddBasicObject(unsigned int param1, unsigned int param2);
LEGO_EXPORT void AddObjectToMap(struct EditObject *param_1, unsigned int param_2, int param_3);
LEGO_EXPORT void StandardRemoveObject(unsigned int a, unsigned int b, unsigned int c);
LEGO_EXPORT void SetEditCursorFootPrint(void *src);
unsigned char FUN_0045f4b0(struct Cursor *cursor);
LEGO_EXPORT void SetObjRectFlags();
void FUN_0045f460(struct Cursor *cursor);
void FUN_0045f480(struct Cursor *cursor, int param);
void FUN_0045f4d0(struct Cursor *cursor);
LEGO_EXPORT void BuildCursorPtr(struct Cursor *cursor, unsigned int param_2, unsigned int param_3);
LEGO_EXPORT void ValidateCursor(struct Cursor *cursor, unsigned int param);
LEGO_EXPORT void CalcBasicObjectCursor(struct CursorObj *obj, unsigned int a2, unsigned int a3);
LEGO_EXPORT void GetTileDimensions(int *width, int *height);
LEGO_EXPORT int Get_XScroll(void);
LEGO_EXPORT int Get_YScroll(void);
void FUN_00460560(int index);
/* True signature unknown (still a STUB): declared with unspecified params so callers
 * don't force argument conversions that diverge from the original codegen. */
void FUN_00461290();
LEGO_EXPORT void MouseScrollMap(void);
LEGO_EXPORT void ProcessScrolling(unsigned int a, unsigned int b);
LEGO_EXPORT unsigned char Get_RFFlags(int x, int y);
LEGO_EXPORT short Get_UserFlags(int x, int y);
LEGO_EXPORT void Set_UserFlags(int x, int y, unsigned short value);
LEGO_EXPORT void SetMapTile(int x, int y, unsigned short value);
LEGO_EXPORT unsigned int LoadBaseMap(unsigned int param_1);
void FUN_004629e0(void);
void FUN_00462e50(unsigned int index, unsigned int value);
void FUN_00462e70(unsigned int index, unsigned int value);
void FUN_00462e90(void);
void FUN_00463560(void);
int FUN_004636c0();
