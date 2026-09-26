#pragma once

#include "legoland.h"
#include "math.h"

// Opaque types; full definitions live in gamemap.c (only its bodies need them).
struct MapElement;
struct Element;
struct Ride;

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

/* An object's footprint rectangle (tile offsets from its origin), chained via next. */
struct Footprint {
    union {
        int v[5];
        struct {
            int x0;
            int y0;
            int x1;
            int y1;
            struct Footprint *next;
        };
    };
};
typedef struct Footprint Footprint;

/* LLIDB sprite set: count, sprite table and per-sprite draw offsets. */
struct SpriteSet {
    /* 0x00 */ int field_0;
    /* 0x04 */ int count;
    /* 0x08 */ struct Sprite **sprites;
    /* 0x0c */ int *offset_x;
    /* 0x10 */ int *offset_y;
};

/* Sprite and draw offset a ride's cb_a0 callback returns for rendering. */
struct RideSpriteInfo {
    /* 0x00 */ void *sprite;
    /* 0x04 */ unsigned int x;
    /* 0x08 */ unsigned int y;
    /* 0x0c */ unsigned short id;
    /* 0x0e */ unsigned short pad_e;
    /* 0x10 */ unsigned int field_10;
};
typedef struct RideSpriteInfo RideSpriteInfo;

/* LLIDB tile mapping: tile id tables selected by the high byte of a map entry. */
struct TileMap {
    /* 0x00 */ unsigned char pad_0[4];
    /* 0x04 */ short *tiles;
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
    union {
        unsigned int field_1414[5];
        struct Footprint footprint;
    };
    unsigned char pad_1428[0x1828 - 0x1428];
    unsigned int field_1828;
    unsigned char pad_182c[0x1830 - 0x182c];
    union {
        unsigned int field_1830;
        struct Cursor *next; /* further cursors placed together (e.g. PathCursor) */
    };
};
typedef struct Cursor Cursor;

struct Point;
struct ObjClass;

void FUN_004598d0(struct Point *coord, int *param_2, int *param_3);
void FUN_00459960(void);
void FUN_00459970(void);
LEGO_EXPORT void PutObjOnMap(struct ObjClass *obj, unsigned int classid, struct Point *pos);
LEGO_EXPORT int FindObjectsPower(struct Ride *ride);
void FUN_0045a030(int power, struct MapElement *object);
void FUN_0045a060(void);
void FUN_0045a0d0(void);
LEGO_EXPORT void AddObjectsPowerStats(unsigned int classid, struct Point *pos);
LEGO_EXPORT void RemoveObjectsPowerStats(unsigned int classid, unsigned int coords);
LEGO_EXPORT void DefaultCursor(struct Cursor *cursor);
void FUN_0045a3e0(int *param);
void FUN_0045a430(short param_1, int *param_2);
LEGO_EXPORT void CalculateMapRenderOrder(void);
LEGO_EXPORT struct MapElement *GetFirstRenderObject(void);
LEGO_EXPORT struct MapElement *GetNextRenderObject(struct MapElement *object);
LEGO_EXPORT struct MapElement *GetFirstObjectMatching(struct Element *cls);
LEGO_EXPORT struct MapElement *GetNextObjectMatching(struct MapElement *object, struct Element *cls);
LEGO_EXPORT void RemObjFromMap(struct ObjClass *obj, unsigned int classid, unsigned short coords, void *cursor);
void FUN_00459880(void);
LEGO_EXPORT struct Point PlayfieldToMap(struct Point pos);
