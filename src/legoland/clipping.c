#include <windows.h>
#include <string.h>
#include "legoland.h"

#include "clipping.h"
#include "globals.h"
#include "profile_io.h"

struct ClippedObject {
    char *name;
    unsigned char pad_4[4];
    unsigned char flags;
};

// GLOBAL: LEGOLAND 0x004bdeb8
struct ClipQueryResult DAT_004bdeb8[16];

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
LEGO_EXPORT void SetClipping(RECT *rect) {
    SPRITE_ClipRect.top = rect->top < 0 ? 0 : rect->top;
    SPRITE_ClipRect.bottom = rect->bottom > (int)lpConfig->field_2 ? lpConfig->field_2 : rect->bottom;
    SPRITE_ClipRect.left = rect->left < 0 ? 0 : rect->left;
    SPRITE_ClipRect.right = rect->right > (int)lpConfig->field_0 ? lpConfig->field_0 : rect->right;
}

// FUNCTION: LEGOLAND 0x0048a630
LEGO_EXPORT void GetClipping(RECT *dest) {
    dest->left = SPRITE_ClipRect.left;
    dest->top = SPRITE_ClipRect.top;
    dest->right = SPRITE_ClipRect.right;
    dest->bottom = SPRITE_ClipRect.bottom;
}

// FUNCTION: LEGOLAND 0x0048a660
LEGO_EXPORT void StoreClipping(void) {
    DAT_00798630 = SPRITE_ClipRect.left;
    DAT_00798634 = SPRITE_ClipRect.top;
    DAT_00798638 = SPRITE_ClipRect.right;
    DAT_0079863c = SPRITE_ClipRect.bottom;
}

// FUNCTION: LEGOLAND 0x0048a690
LEGO_EXPORT void RestoreClipping(void) {
    SPRITE_ClipRect.left = DAT_00798630;
    SPRITE_ClipRect.top = DAT_00798634;
    SPRITE_ClipRect.right = DAT_00798638;
    SPRITE_ClipRect.bottom = DAT_0079863c;
}

// FUNCTION: LEGOLAND 0x0048a6c0
LEGO_EXPORT int ClipThisRect(RECT *lpRect) {
    return IntersectRect(lpRect, lpRect, &SPRITE_ClipRect);
}

// FUNCTION: LEGOLAND 0x0048a6e0
void FUN_0048a6e0(struct ClippedObject *object) {
    struct ClipQueryResult *entry;

    for (entry = DAT_004bdeb8; strlen(entry->name) != 0; entry++) {
        if (_stricmp(object->name, entry->name) == 0) {
            if (DAT_0080ffe6[entry->id] == 0) {
                DAT_0080ffe6[entry->id] = 1;
                UpDateCurrentProfile();
            }
            return;
        }
    }
}

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
void FUN_0048a780(void *arg) {
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
void FUN_0048a800(void) {
    struct ClipQueryResult *entry;

    if (strlen(DAT_004bdeb8[0].name) == 0)
        return;

    entry = DAT_004bdeb8;
    do {
        entry->field_c = 0;
        entry++;
    } while (strlen(entry->name) != 0);
}

// FUNCTION: LEGOLAND 0x0048a840
unsigned int FUN_0048a840(unsigned int arg, struct ClipQueryResult **out) {
    struct ClipQueryResult *entry;

    for (entry = DAT_004bdeb8; strlen(entry->name) != 0; entry++) {
        if (_stricmp((char *)arg, entry->name) == 0) {
            if (out != NULL) {
                *out = entry;
            }
            return entry->field_8;
        }
    }
    return 0;
}
