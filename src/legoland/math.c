#include "math.h"
#include <stdlib.h>
#include "legoland.h"

struct Navigator {
    int x;
    int y;
    short dx;
    short dy;
};

// FUNCTION: LEGOLAND 0x004806a0
LEGO_EXPORT unsigned int Rand_Max(unsigned int max_value) {
    return (unsigned int)rand() % (max_value + 1);
}

// FUNCTION: LEGOLAND 0x004806c0
LEGO_EXPORT unsigned int Rand_Tween(unsigned int min_val, unsigned int max_val) {
    return Rand_Max(max_val - min_val) + min_val;
}

// FUNCTION: LEGOLAND 0x004806e0
LEGO_EXPORT int ArcTan256(int dx, int dy) { STUB(); }

// FUNCTION: LEGOLAND 0x00480740
LEGO_EXPORT char CalcMoveLine(int x1, int y1, int x2, int y2, void *out) { STUB(); }

// FUNCTION: LEGOLAND 0x004807f0
LEGO_EXPORT void NavigMoveLine(struct Navigator *nav, unsigned short a, struct Point *out) {
    nav->x += nav->dx * a;
    nav->y += nav->dy * a;
    if (out != NULL) {
        out->x = nav->x >> 8;
        out->y = nav->y >> 8;
    }
}

// FUNCTION: LEGOLAND 0x00480840
void FUN_00480840(struct Point *src, struct Point *dst, int dir) {
    switch (dir) {
    case 1:
        dst->x = src->x;
        dst->y = src->y - 2;
        break;
    case 3:
        dst->x = src->x + 2;
        dst->y = src->y;
        break;
    case 5:
        dst->x = src->x;
        dst->y = src->y + 2;
        break;
    case 7:
        dst->x = src->x - 2;
        dst->y = src->y;
        break;
    }
}

// FUNCTION: LEGOLAND 0x004808d0
void FUN_004808d0(int *src, int *dst, int dir) {
    switch (dir) {
    case 1:
        dst[0] = src[0];
        dst[1] = src[1] - 4;
        break;
    case 3:
        dst[0] = src[0] + 4;
        dst[1] = src[1];
        break;
    case 5:
        dst[0] = src[0];
        dst[1] = src[1] + 4;
        break;
    case 7:
        dst[0] = src[0] - 4;
        dst[1] = src[1];
        break;
    }
}

// FUNCTION: LEGOLAND 0x00480960
LEGO_EXPORT int GetRectArea(struct RectNode *list) {
    int total = 0;
    while (list != NULL) {
        total += (list->field_c - list->field_4 + 1) * (list->field_8 - list->field_0 + 1);
        list = list->next;
    }
    return total;
}
