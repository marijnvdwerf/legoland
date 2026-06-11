#include "legoland.h"
#include <string.h>

#include "gamemap.h"
#include "map_object.h"
#include "globals.h"

typedef void (*CastleVtblFn)(void);

struct CastleLevelInterface {
    unsigned char pad_0[0x8c];
    CastleVtblFn field_8c;
    CastleVtblFn field_90;
    CastleVtblFn field_94;
    CastleVtblFn field_98;
    CastleVtblFn field_9c;
    CastleVtblFn field_a0;
    CastleVtblFn field_a4;
    CastleVtblFn field_a8;
    CastleVtblFn field_ac;
    CastleVtblFn field_b0;
};


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
void FUN_00403080(const char **name, struct CastleLevelInterface *ci) {
    // STRING: LEGOLAND 0x004b408c
    if (_stricmp("CASTLE LEVEL 1", *name) == 0) {
        ci->field_a4 = FUN_00402ca0;
        ci->field_ac = FUN_00402ce0;
        ci->field_8c = FUN_00402ff0;
        ci->field_98 = (CastleVtblFn)FUN_00403060;
        ci->field_9c = FUN_00403030;
        ci->field_a8 = FUN_00402dc0;
        ci->field_b0 = FUN_00402d00;
    }
}
