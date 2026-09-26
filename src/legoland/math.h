#pragma once

#include "legoland.h"

// This header shadows the CRT <math.h>; pull the real one in explicitly.
#include "../../toolchain/msvc6/Include/math.h"

struct Point {
    /* 0x00 */ int x;
    /* 0x04 */ int y;
};
typedef struct Point Point;

/* Fixed-point line walker filled in by CalcMoveLine and stepped by NavigMoveLine. */
struct Navigator {
    /* 0x00 */ int x;
    /* 0x04 */ int y;
    /* 0x08 */ short dx;
    /* 0x0a */ short dy;
};
typedef struct Navigator Navigator;

struct RectNode {
    /* 0x00 */ int field_0;
    /* 0x04 */ int field_4;
    /* 0x08 */ int field_8;
    /* 0x0c */ int field_c;
    /* 0x10 */ struct RectNode *next;
};

LEGO_EXPORT int ArcTan256(int dx, int dy);
LEGO_EXPORT char CalcMoveLine(struct Point from, struct Point to, struct Navigator *nav);
void FUN_00480840(struct Point *src, struct Point *dst, int dir);
void FUN_004808d0(int *src, int *dst, int dir);
LEGO_EXPORT int GetRectArea(struct RectNode *list);
LEGO_EXPORT unsigned int Rand_Max(unsigned int max_value);
LEGO_EXPORT unsigned int Rand_Tween(unsigned int min_val, unsigned int max_val);
LEGO_EXPORT void NavigMoveLine(struct Navigator *nav, unsigned short a, struct Point *out);
