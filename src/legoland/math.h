#pragma once

struct FVec3 {
    float x;
    float y;
    float z;
};

#include "legoland.h"

// This header shadows the CRT <math.h>; pull the real one in explicitly.
#include "../../toolchain/msvc6/Include/math.h"

struct Point {
    /* 0x00 */ int x;
    /* 0x04 */ int y;
};
typedef struct Point Point;

typedef struct Vector3 {
    float x;
    float y;
    float z;
} Vector3;

/* Fixed-point line walker filled in by CalcMoveLine and stepped by NavigMoveLine. */
struct Navigator {
    /* 0x00 */ int x;
    /* 0x04 */ int y;
    /* 0x08 */ short dx;
    /* 0x0a */ short dy;
    /* 0x0c */ int field_c;
    /* 0x10 */ int field_10;
};
typedef struct Navigator Navigator;

struct RectNode {
    /* 0x00 */ int x0;
    /* 0x04 */ int y0;
    /* 0x08 */ int x1;
    /* 0x0c */ int y1;
    /* 0x10 */ struct RectNode *next;
};

LEGO_EXPORT int ArcTan256(int dx, int dy);
LEGO_EXPORT char CalcMoveLine(struct Point from, struct Point to, struct Navigator *nav);
/* CastleObjUpdate's view of CalcMoveLine: it uses the result with no sign extension (add eax,0x10;
 * mov [..],al at 0x425304 / 0x4253da), i.e. as an int. */
typedef int (*CalcMoveLineIntFn)(struct Point from, struct Point to, struct Navigator *nav);
void MovePointInDirection(struct Point *src, struct Point *dst, int dir);
void StepPointByDirection(int *src, int *dst, int dir);
LEGO_EXPORT int GetRectArea(struct RectNode *list);
LEGO_EXPORT unsigned int Rand_Max(unsigned int max_value);
LEGO_EXPORT unsigned int Rand_Tween(unsigned int min_val, unsigned int max_val);
LEGO_EXPORT void NavigMoveLine(struct Navigator *nav, unsigned short a, struct Point *out);
