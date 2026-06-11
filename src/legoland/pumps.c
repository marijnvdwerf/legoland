#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "gamemap.h"
#include "globals.h"
#include "map_object.h"
#include "pumps.h"

struct PumpSource {
    unsigned char pad_0[0xc];
    unsigned int var_c;
};

// FUNCTION: LEGOLAND 0x00411a10
void FUN_00411a10(struct PumpSource *param_1) {
    DAT_004cbe9c = param_1->var_c;
}

// FUNCTION: LEGOLAND 0x00411a20
void FUN_00411a20(void) {
    unsigned int eax_temp;

    eax_temp = DAT_004cbe9c;
    EditMode = 1;
    DAT_008119b8 = (void *)eax_temp;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(DAT_004b4bd0);
    EditCursor.field_1828 = EditCursor.field_1828 | 8;
    BuildCursorPtr(&EditCursor, 0x8f8, 0);
    DefaultCursor(&DAT_0082f760);
    memcpy(&DAT_00830b74, DAT_004b4bd0, 0x14);
    DAT_00830f88 = 0x34;
}

// FUNCTION: LEGOLAND 0x00411aa0
struct PumpNode *FUN_00411aa0(unsigned int arg1, unsigned int arg2) {
    struct PumpNode *node;

    node = (struct PumpNode *)DAT_004cbea4;
    while (node) {
        if (node->var_4 == arg1 && node->var_8 == arg2) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00411ad0
void FUN_00411ad0(struct PumpNode *node) {
    struct PumpNode *next;
    struct PumpNode *current;

    if (DAT_004cbea4) {
        next = node->next;
        free(node);
        if (DAT_004cbea4 == node) {
            DAT_004cbea4 = next;
        } else {
            current = (struct PumpNode *)DAT_004cbea4;
            if (current->next != node) {
                do {
                    current = current->next;
                } while (current && current->next != node);
            }
            if (current) {
                current->next = next;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00411b20
void FUN_00411b20(struct PumpNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00411ba0
void FUN_00411ba0(unsigned short param) {
    struct PumpNode *cur;
    struct PumpNode *next;

    cur = (struct PumpNode *)DAT_004cbea4;
    if (cur) {
        do {
            next = cur->next;
            if (cur->var_2 == param) {
                FUN_00411b20(cur);
            }
            cur = next;
        } while (cur);
    }
}

// FUNCTION: LEGOLAND 0x00411bd0
void FUN_00411bd0(void) {
    struct PumpNode *current;
    struct PumpNode *next;

    current = (struct PumpNode *)DAT_004cbea4;
    if (current == NULL) {
        return;
    }
    while (current != NULL) {
        next = current->next;
        FUN_00411b20(current);
        current = next;
    }
}

// FUNCTION: LEGOLAND 0x00411bf0
void FUN_00411bf0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00411c70
void FUN_00411c70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00411cd0
void FUN_00411cd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00411dc0
void FUN_00411dc0(void) { STUB(); }
