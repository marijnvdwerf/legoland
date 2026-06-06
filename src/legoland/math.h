#pragma once

// Per-TU header for math.c — RNG, trig, and geometry helpers.

struct Navigator {
    int x;
    int y;
    short dx;
    short dy;
};

struct Point {
    int x;
    int y;
};

struct RectNode {
    int field_0;
    int field_4;
    int field_8;
    int field_c;
    struct RectNode *next;
};

unsigned int Rand_Max(unsigned int max_value);
unsigned int Rand_Tween(unsigned int min_val, unsigned int max_val);
void ArcTan256(void);
void CalcMoveLine(void);
void NavigMoveLine(struct Navigator *nav, unsigned short a, struct Point *out);
void FUN_00480840(void);
void FUN_004808d0(void);
int GetRectArea(struct RectNode *list);
