#include <string.h>
#include "legoland.h"

#include "gamemap.h"
#include "globals.h"
#include "map_object.h"
#include "objclass.h"

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x00402ca0
void FUN_00402ca0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00402ce0
void FUN_00402ce0(void) {
    if (DAT_004c10e4 != 0) {
        KillSprite(DAT_004c10e4);
    }
}

// FUNCTION: LEGOLAND 0x00402d00
void FUN_00402d00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00402dc0
void FUN_00402dc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00402ff0
void FUN_00402ff0(void) {
    DAT_008119b8 = DAT_004c10dc;
    EditMode = 1;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&((struct EditCursorData *)DAT_008119b8)->field_3c);
}

// FUNCTION: LEGOLAND 0x00403030
void FUN_00403030(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00403060
void FUN_00403060(unsigned int param1, unsigned int param2) {
    AddBasicObject(param1, param2);
}

// FUNCTION: LEGOLAND 0x00403080
void FUN_00403080(struct ClassNode *name, struct CallbackTable *ci) {
    // STRING: LEGOLAND 0x004b408c
    if (_stricmp("CASTLE LEVEL 1", name->name) == 0) {
        ci->cb_a4 = FUN_00402ca0;
        ci->cb_ac = FUN_00402ce0;
        ci->cb_8c = FUN_00402ff0;
        ci->cb_98 = FUN_00403060;
        ci->cb_9c = FUN_00403030;
        ci->cb_a8 = FUN_00402dc0;
        ci->cb_b0 = FUN_00402d00;
    }
}
