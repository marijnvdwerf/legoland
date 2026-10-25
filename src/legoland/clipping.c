#include <windows.h>
#include <string.h>
#include "legoland.h"

#include "clipping.h"
#include "globals.h"
#include "llidb.h"
#include "obj_instance.h"
#include "profile_io.h"

// GLOBAL: LEGOLAND 0x004bdeb8
struct ClipQueryResult FreePlayObjectTable[0x86];

struct ClipNode;

typedef void (*ClipNodeMethod)(struct ClipNode *node, int a1, int a2, int a3);

struct ClipNode {
    struct ClipNode *next;
    unsigned char pad_4[0x10];
    unsigned short field_14;
    unsigned char pad_16[6];
    const char *field_1c;
    unsigned char pad_20[0xc];
    ClipNodeMethod field_2c;
};

// FUNCTION: LEGOLAND 0x0048a5c0
LEGO_EXPORT void SetClipping(RECT *rect) {
    SPRITE_ClipRect.top = rect->top < 0 ? 0 : rect->top;
    SPRITE_ClipRect.bottom = rect->bottom > (int)lpConfig->screen_height ? lpConfig->screen_height : rect->bottom;
    SPRITE_ClipRect.left = rect->left < 0 ? 0 : rect->left;
    SPRITE_ClipRect.right = rect->right > (int)lpConfig->screen_width ? lpConfig->screen_width : rect->right;
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
    SavedClipLeft = SPRITE_ClipRect.left;
    SavedClipTop = SPRITE_ClipRect.top;
    SavedClipRight = SPRITE_ClipRect.right;
    SavedClipBottom = SPRITE_ClipRect.bottom;
}

// FUNCTION: LEGOLAND 0x0048a690
LEGO_EXPORT void RestoreClipping(void) {
    SPRITE_ClipRect.left = SavedClipLeft;
    SPRITE_ClipRect.top = SavedClipTop;
    SPRITE_ClipRect.right = SavedClipRight;
    SPRITE_ClipRect.bottom = SavedClipBottom;
}

// FUNCTION: LEGOLAND 0x0048a6c0
LEGO_EXPORT int ClipThisRect(RECT *lpRect) {
    return IntersectRect(lpRect, lpRect, &SPRITE_ClipRect);
}

// FUNCTION: LEGOLAND 0x0048a6e0
void UnlockFreePlayObject(struct Element *object) {
    struct ClipQueryResult *entry;

    for (entry = FreePlayObjectTable; strlen(entry->name) != 0; entry++) {
        if (_stricmp(object->name, entry->name) == 0) {
            if (CurrentProfile.field_46[entry->id] == 0) {
                CurrentProfile.field_46[entry->id] = 1;
                UpDateCurrentProfile();
            }
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x0048a750
void UnlockAllFreePlayObjects(void) {
    struct Ride *current = (struct Ride *)ObjectClassList;
    while (current != NULL) {
        struct Element *object = current->element;
        if ((object->flags & 0x2) == 0) {
            UnlockFreePlayObject(object);
        }
        current = current->next;
    }
}

/* Empty in the original (does nothing). */
// FUNCTION: LEGOLAND 0x0048a780
void Empty_0048a780(void *arg) {
}

/* Runs after the free play lists are built: sends a click to every object icon whose entry is still selected
 * (ClipQueryResult field_c), restoring the previous selection with the click sound muted. */
// FUNCTION: LEGOLAND 0x0048a790
void RestoreFreePlaySelection(void) {
    struct ClipNode *node = (struct ClipNode *)IconListHead;
    FreePlayRestoringSelection = 1;

    while (node != NULL) {
        unsigned short type = node->field_14;
        if (type == 0xc8 || type == 0x1f4 || type == 0x190 || type == 0x12c) {
            struct ClipQueryResult *result;
            GetFreePlayObjectCost(node->field_1c, &result);
            if (result->field_c) {
                node->field_2c(node, 2, 0, 0);
            }
        }
        node = node->next;
    }

    FreePlayRestoringSelection = 0;
}

// FUNCTION: LEGOLAND 0x0048a800
void ClearFreePlaySelection(void) {
    struct ClipQueryResult *entry;

    if (strlen(FreePlayObjectTable[0].name) == 0)
        return;

    entry = FreePlayObjectTable;
    do {
        entry->field_c = 0;
        entry++;
    } while (strlen(entry->name) != 0);
}

// FUNCTION: LEGOLAND 0x0048a840
unsigned int GetFreePlayObjectCost(const char *arg, struct ClipQueryResult **out) {
    struct ClipQueryResult *entry;

    for (entry = FreePlayObjectTable; strlen(entry->name) != 0; entry++) {
        if (_stricmp(arg, entry->name) == 0) {
            if (out != NULL) {
                *out = entry;
            }
            return entry->field_8;
        }
    }
    return 0;
}
