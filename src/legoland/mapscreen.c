#include "legoland.h"

#include "globals.h"
#include "interface.h"
#include "mapscreen.h"

struct MapPoint {
    int field0;
    int field1;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x004562c0
void FUN_004562c0(void) {
    DAT_00667c10 = DAT_006687bc;
    DAT_00667c28 = DAT_006687c0;
}

// FUNCTION: LEGOLAND 0x004562e0
void FUN_004562e0(void) {
    DAT_006687bc = DAT_00667c10;
    DAT_006687c0 = DAT_00667c28;
}

// FUNCTION: LEGOLAND 0x00456300
LEGO_EXPORT void InitMapScreen(void) {
    if (DAT_00667c30 != 0) {
        return;
    }
    DAT_00667c2c = CreateFunctionBasedSprite((int (*)(struct Sprite *))RenderFullMap, 0x280, 0x154);
    if (DAT_00667c2c != NULL) {
        // STRING: LEGOLAND 0x004b90b4
        DAT_00667c34 = LoadSprite("mapSpanner.lls", 0);
        DAT_00667c2c->src_x = 0;
        DAT_00667c2c->src_y = 0;
    }
    FUN_004562c0();
    DAT_006687c0 = (unsigned int)FUN_00475080;
    DAT_006687bc = (unsigned int)FUN_00475080;
}

// FUNCTION: LEGOLAND 0x00456370
LEGO_EXPORT void KillMapScreen(void) {
    if (DAT_00667c2c != NULL) {
        KillSprite(DAT_00667c2c);
        DAT_00667c2c = NULL;
        if (DAT_00667c34 != 0) {
            KillSprite(DAT_00667c34);
            DAT_00667c34 = 0;
        }
    }
    DAT_00667c30 = 0;
}

// FUNCTION: LEGOLAND 0x004563b0
void FUN_004563b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00456460
LEGO_EXPORT void RenderMouseBounds(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004565b0
LEGO_EXPORT void MapScreenSetScrollPos(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004566f0
LEGO_EXPORT void DrawMapScreen(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00456770
void FUN_00456770(struct MapPoint *arg) {
    int v0 = arg->field0;
    int v1 = arg->field1;

    if (v0 < 0) {
        v0 = -((-v0) >> 1);
    } else {
        v0 = v0 >> 1;
    }

    if (v1 < 0) {
        v1 = -((-v1) >> 1);
    } else {
        v1 = v1 >> 1;
    }

    arg->field0 = v0;
    arg->field1 = v1;
}

// FUNCTION: LEGOLAND 0x004567a0
LEGO_EXPORT void RenderFullMap(void) { STUB(); }
