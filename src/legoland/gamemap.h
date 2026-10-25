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
    /* 0x14 */ void *data_14;
    /* 0x18 */ unsigned int field_18;
    /* 0x1c */ unsigned int field_1c;
    /* 0x20 */ unsigned int field_20;
};

/* A tile object set a level map names: its element is the object placed for the set's tiles. */
struct MapTileSet {
    /* 0x00 */ unsigned char pad_0[0x14];
    /* 0x14 */ struct Element *element;
};

/* Sprite and draw offset a ride's cb_a0 callback returns for rendering. */
struct RideSpriteInfo {
    /* 0x00 */ void *sprite;
    /* 0x04 */ int x;
    /* 0x08 */ int y;
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

// The map edit cursor. EditCursor / QueryCursor / ExtraEditCursor1 are instances of
// this 0x1834-byte structure. Only the fields actually touched are named; the
// gaps are explicit padding so the layout matches the original byte-for-byte.
struct Cursor {
    /* 0x0000 */ unsigned short count; /* the footprint's points */
    /* 0x0002 */ unsigned short xpts[0x400];
    /* 0x0802 */ unsigned short ypts[0x400];
    /* 0x1002 */ unsigned char fpts[0x400];
    /* 0x1402 */ unsigned char pad_1402[0x1404 - 0x1402];
    int tile_x;
    int tile_y;
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

void CountPathEdgeScenery(struct Point *coord, int *param_2, int *param_3);
void ResetPathSceneryTimer(void);
void UpdatePathSceneryPercent(void);
LEGO_EXPORT void PutObjOnMap(struct Ride *obj, struct Element *classid, struct Point *pos);
LEGO_EXPORT int FindObjectsPower(struct Ride *ride);
void MarkObjectUnpowered(int power, struct MapElement *object);
void RestorePowerToObjects(void);
void CutPowerToObjects(void);
LEGO_EXPORT void AddObjectsPowerStats(struct Element *classid, struct Point *pos);
LEGO_EXPORT void RemoveObjectsPowerStats(struct Element *classid, TileId coords);
LEGO_EXPORT void DefaultCursor(struct Cursor *cursor);
void PopRenderOrderColumnStart(int *param);
void PopRenderOrderEntryForAnchor(short param_1, int *param_2);
LEGO_EXPORT void CalculateMapRenderOrder(void);
void CalculateFullMapRenderOrder(void);
LEGO_EXPORT struct MapElement *GetFirstRenderObject(void);
LEGO_EXPORT struct MapElement *GetNextRenderObject(struct MapElement *object);
LEGO_EXPORT struct MapElement *GetFirstObjectMatching(struct Element *cls);
LEGO_EXPORT struct MapElement *GetNextObjectMatching(struct MapElement *object, struct Element *cls);
LEGO_EXPORT void RemObjFromMap(struct Ride *obj, struct Element *classid, TileId tile, void *cursor);
void ResetCoverageCounters(void);
LEGO_EXPORT struct Point PlayfieldToMap(struct Point pos);
LEGO_EXPORT void InitGameMap(void);
LEGO_EXPORT void KillGameMap(void);
