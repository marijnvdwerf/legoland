#include <string.h>
#include "legoland.h"

#include "gamemap.h"
#include "globals.h"
#include "map_object.h"
#include "objclass.h"

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x00406240
void FUN_00406240(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004062a0
void FUN_004062a0(void) {
    struct Sprite *sprite = DAT_004c11cc;
    if (sprite != NULL) {
        KillSprite(sprite);
    }
}

// FUNCTION: LEGOLAND 0x004062c0
void FUN_004062c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004064d0
void FUN_004064d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00406660
void FUN_00406660(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00406820
void FUN_00406820(void) {
    DAT_008119b8 = (void *)DAT_004c11dc;
    EditMode = 1;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&((struct EditCursorData *)DAT_008119b8)->field_3c);
}

// FUNCTION: LEGOLAND 0x00406860
unsigned int FUN_00406860(unsigned int param1, unsigned int param2) {
    return AddBasicObject(param1, param2);
}

// FUNCTION: LEGOLAND 0x00406880
void FUN_00406880(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004068b0
void FUN_004068b0(struct ClassNode *name, struct CallbackTable *ci) {
    // STRING: LEGOLAND 0x004b45a0
    if (_stricmp("FORT", name->name) != 0) {
        return;
    }
    ci->cb_a4 = FUN_00406240;
    ci->cb_ac = FUN_004062a0;
    ci->cb_8c = FUN_00406820;
    ci->cb_a8 = FUN_00406660;
    ci->cb_b0 = FUN_004062c0;
    ci->cb_9c = FUN_00406880;
    ci->cb_98 = FUN_00406860;
}
