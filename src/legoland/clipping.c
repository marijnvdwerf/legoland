#include "legoland.h"
#include <windows.h>

#include "clipping.h"
#include "globals.h"

struct ClippedObject {
    unsigned char pad_0[8];
    unsigned char flags;
};

struct ObjectClassNode {
    struct ObjectClassNode *next;
    unsigned char pad_4[0xc0];
    struct ClippedObject *object;
};

typedef void (*ClipNodeMethod)(struct ClipNode *node, int a1, int a2, int a3);

struct ClipNode {
    struct ClipNode *next;
    unsigned char pad_4[0x10];
    unsigned short field_14;
    unsigned char pad_16[6];
    unsigned int field_1c;
    unsigned char pad_20[0xc];
    ClipNodeMethod field_2c;
};

// FUNCTION: LEGOLAND 0x0048a5c0
void SetClipping(int *rect) { STUB(); }

// FUNCTION: LEGOLAND 0x0048a630
void GetClipping(struct ClipRect *dest) {
    dest->left = SPRITE_ClipRect.left;
    dest->top = SPRITE_ClipRect.top;
    dest->right = SPRITE_ClipRect.right;
    dest->bottom = SPRITE_ClipRect.bottom;
}

// FUNCTION: LEGOLAND 0x0048a660
void StoreClipping(void) {
    DAT_00798630 = SPRITE_ClipRect.left;
    DAT_00798634 = SPRITE_ClipRect.top;
    DAT_00798638 = SPRITE_ClipRect.right;
    DAT_0079863c = SPRITE_ClipRect.bottom;
}

// FUNCTION: LEGOLAND 0x0048a690
void RestoreClipping(void) {
    SPRITE_ClipRect.left = DAT_00798630;
    SPRITE_ClipRect.top = DAT_00798634;
    SPRITE_ClipRect.right = DAT_00798638;
    SPRITE_ClipRect.bottom = DAT_0079863c;
}

// FUNCTION: LEGOLAND 0x0048a6c0
int ClipThisRect(RECT *lpRect) {
    return IntersectRect(lpRect, lpRect, (RECT *)&SPRITE_ClipRect);
}

// FUNCTION: LEGOLAND 0x0048a6e0
void FUN_0048a6e0(struct ClippedObject *object) { STUB(); }

// FUNCTION: LEGOLAND 0x0048a750
void FUN_0048a750(void) {
    struct ObjectClassNode *current = (struct ObjectClassNode *)ObjectClassList;
    while (current != NULL) {
        struct ClippedObject *object = current->object;
        if ((object->flags & 0x2) == 0) {
            FUN_0048a6e0(object);
        }
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x0048a780
void FUN_0048a780(void) {
}

// FUNCTION: LEGOLAND 0x0048a790
void FUN_0048a790(void) {
    struct ClipNode *node = (struct ClipNode *)DAT_006687c8;
    DAT_00798648 = 1;

    while (node != NULL) {
        unsigned short type = node->field_14;
        if (type == 0xc8 || type == 0x1f4 || type == 0x190 || type == 0x12c) {
            struct ClipQueryResult *result;
            FUN_0048a840(node->field_1c, &result);
            if (result->field_c) {
                node->field_2c(node, 2, 0, 0);
            }
        }
        node = node->next;
    }

    DAT_00798648 = 0;
}

// FUNCTION: LEGOLAND 0x0048a800
void FUN_0048a800(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048a840
unsigned int FUN_0048a840(unsigned int arg, struct ClipQueryResult **out) { STUB(); }
