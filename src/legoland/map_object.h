#pragma once

// Per-TU header for map_object.c — canonical declarations for functions that are
// pre-declared elsewhere (called/address-taken from another TU). map_object already
// owns struct MapTile etc.; struct Cursor lives in gamemap.h.

struct Cursor;
struct CursorObj;

void FUN_0045e4a0(int element, void *data);
unsigned int AddBasicObject(unsigned int param1, unsigned int param2);
void StandardRemoveObject(unsigned int a, unsigned int b, unsigned int c);
void SetEditCursorFootPrint(void *src);
void FUN_0045f460(struct Cursor *cursor);
void FUN_0045f480(struct Cursor *cursor, int param);
void FUN_0045f4d0(struct Cursor *cursor);
void BuildCursorPtr(struct Cursor *cursor, unsigned int param_2, unsigned int param_3);
void ValidateCursor(struct Cursor *cursor, unsigned int param);
void CalcBasicObjectCursor(struct CursorObj *obj, unsigned int a2, unsigned int a3);
void GetTileDimensions(int *width, int *height);
void FUN_00460560(int index);
unsigned char Get_RFFlags(int x, int y);
void Set_UserFlags(int x, int y, unsigned short value);
void SetMapTile(int x, int y, unsigned short value);
unsigned int LoadBaseMap(unsigned int param_1);
void FUN_004629e0(void);
void FUN_00462e50(unsigned int index, unsigned int value);
void FUN_00462e70(unsigned int index, unsigned int value);
void FUN_00462e90(void);
void FUN_00463560(void);
