#pragma once

#include "legoland.h"

struct Point {
    /* 0x00 */ int x;
    /* 0x04 */ int y;
};

struct RectNode {
    /* 0x00 */ int field_0;
    /* 0x04 */ int field_4;
    /* 0x08 */ int field_8;
    /* 0x0c */ int field_c;
    /* 0x10 */ struct RectNode *next;
};

void FUN_00480840(struct Point *src, struct Point *dst, int dir);
void FUN_004808d0(int *src, int *dst, int dir);
LEGO_EXPORT int GetRectArea(struct RectNode *list);
LEGO_EXPORT unsigned int Rand_Max(unsigned int max_value);
struct Navigator;
LEGO_EXPORT void NavigMoveLine(struct Navigator *nav, unsigned short a, struct Point *out);
