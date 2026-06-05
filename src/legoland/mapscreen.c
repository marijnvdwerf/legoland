#include "legoland.h"

struct Sprite {
    unsigned char pad_0[0x18];
    unsigned short field_18;
    unsigned short field_1a;
};

struct MapPoint {
    int field0;
    int field1;
};

extern unsigned int DAT_006687bc;
extern unsigned int DAT_006687c0;
extern unsigned int DAT_00667c10;
extern unsigned int DAT_00667c28;
extern struct Sprite *DAT_00667c2c;
extern unsigned int DAT_00667c30;
extern unsigned int DAT_00667c34;

extern struct Sprite *CreateFunctionBasedSprite(unsigned int source, unsigned short a, unsigned short b);
extern unsigned int LoadSprite(const char *filename, int flags);
extern void KillSprite(unsigned int sprite);
extern void RenderFullMap(void);
extern void FUN_00475080(void);

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
void InitMapScreen(void) {
    if (DAT_00667c30 != 0) {
        return;
    }
    DAT_00667c2c = CreateFunctionBasedSprite((unsigned int)RenderFullMap, 0x280, 0x154);
    if (DAT_00667c2c != NULL) {
        // STRING: LEGOLAND 0x004b90b4
        DAT_00667c34 = LoadSprite("mapSpanner.lls", 0);
        DAT_00667c2c->field_18 = 0;
        DAT_00667c2c->field_1a = 0;
    }
    FUN_004562c0();
    DAT_006687c0 = (unsigned int)FUN_00475080;
    DAT_006687bc = (unsigned int)FUN_00475080;
}

// FUNCTION: LEGOLAND 0x00456370
void KillMapScreen(void) {
    if (DAT_00667c2c != NULL) {
        KillSprite((unsigned int)DAT_00667c2c);
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
void RenderMouseBounds(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004565b0
void MapScreenSetScrollPos(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004566f0
void DrawMapScreen(void) { STUB(); }

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
void RenderFullMap(void) { STUB(); }
