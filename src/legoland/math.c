#include "legoland.h"
#include "crt.h"

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


// FUNCTION: LEGOLAND 0x004806a0
LEGO_EXPORT unsigned int Rand_Max(unsigned int max_value) {
    return (unsigned int)FUN_0049e4b2() % (max_value + 1);
}

// FUNCTION: LEGOLAND 0x004806c0
LEGO_EXPORT unsigned int Rand_Tween(unsigned int min_val, unsigned int max_val) {
    return Rand_Max(max_val - min_val) + min_val;
}

// FUNCTION: LEGOLAND 0x004806e0
LEGO_EXPORT void ArcTan256(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00480740
LEGO_EXPORT void CalcMoveLine(void) { STUB(); }

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
void FUN_00480840(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004808d0
void FUN_004808d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00480960
LEGO_EXPORT int GetRectArea(struct RectNode *list) {
    int total = 0;
    while (list != NULL) {
        total += (list->field_c - list->field_4 + 1) * (list->field_8 - list->field_0 + 1);
        list = list->next;
    }
    return total;
}
