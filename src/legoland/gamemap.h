#pragma once

#include "legoland.h"

// Opaque types; full definitions live in gamemap.c (only its bodies need them).
struct RenderObject;
struct RenderObjectVtable;

struct MapRenderOrderEntry {
    /* 0x00 */ unsigned int flag;
    /* 0x04 */ unsigned short coords;
    /* 0x06 */ unsigned char x;
    /* 0x07 */ unsigned char height;
};

struct EditCursorData {
    /* 0x00 */ unsigned char pad_0[0x3c];
    /* 0x3c */ unsigned char field_3c;
};

struct Footprint {
    int v[5];
};

// The map edit cursor. EditCursor / QueryCursor / DAT_0082f760 are instances of
// this 0x1834-byte structure. Only the fields actually touched are named; the
// gaps are explicit padding so the layout matches the original byte-for-byte.
struct Cursor {
    /* 0x000 */ unsigned char pad_0[0x14];
    /* 0x014 */ unsigned int field_14;
    /* 0x018 */ unsigned int field_18;
    /* 0x01c */ unsigned int field_1c;
    /* 0x020 */ unsigned char pad_20[0x3c - 0x20];
    /* 0x03c */ struct Footprint field_3c;
    /* 0x050 */ unsigned char pad_50[0x64 - 0x50];
    /* 0x064 */ unsigned int field_64;
    /* 0x068 */ unsigned char pad_68[0xc4 - 0x68];
    /* 0x0c4 */ unsigned int field_c4;
    /* 0x0c8 */ unsigned char pad_c8[0xcc - 0xc8];
    /* 0x0cc */ void *field_cc;
    /* 0x0d0 */ unsigned char pad_d0[0x1404 - 0xd0];
    unsigned int field_1404;
    unsigned int field_1408;
    int field_140c;
    int field_1410;
    unsigned int field_1414[5];
    unsigned char pad_1428[0x1828 - 0x1428];
    unsigned int field_1828;
    unsigned char pad_182c[0x1830 - 0x182c];
    unsigned int field_1830;
};

struct Point;
struct ObjClass;

void FUN_004598d0(struct Point *coord, int *param_2, int *param_3);
void FUN_00459960(void);
void FUN_00459970(void);
LEGO_EXPORT void PutObjOnMap(struct ObjClass *obj, unsigned int classid, struct Point *pos);
LEGO_EXPORT int FindObjectsPower(void *object);
void FUN_0045a030(int power, struct RenderObject *object);
void FUN_0045a060(void);
void FUN_0045a0d0(void);
LEGO_EXPORT void AddObjectsPowerStats(unsigned int classid, struct Point *pos);
LEGO_EXPORT void RemoveObjectsPowerStats(unsigned int classid, unsigned int coords);
LEGO_EXPORT void DefaultCursor(struct Cursor *cursor);
void FUN_0045a3e0(int *param);
void FUN_0045a430(short param_1, int *param_2);
LEGO_EXPORT void CalculateMapRenderOrder(void);
LEGO_EXPORT struct RenderObject *GetFirstRenderObject(void);
LEGO_EXPORT struct RenderObject *GetNextRenderObject(struct RenderObject *object);
LEGO_EXPORT struct RenderObject *GetFirstObjectMatching(struct RenderObjectVtable *vtable);
LEGO_EXPORT void RemObjFromMap(struct ObjClass *obj, unsigned int classid, unsigned int coords, void *cursor);
