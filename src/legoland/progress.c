#include "legoland.h"
#include "freeplay.h"
#include "screens.h"
#include "options.h"
#include "draw.h"
#include "interface.h"
#include "sound_music.h"
#include "globals.h"

struct LegoConfig {
    unsigned char pad_0[0x28];
    unsigned int field_28;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x0048b7e0
LEGO_EXPORT void InitProgressScreen(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048bb60
void FUN_0048bb60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048bc20
void FUN_0048bc20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048bd00
void FUN_0048bd00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048bd40
void FUN_0048bd40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048bd70
void FUN_0048bd70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048bde0
void FUN_0048bde0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048bf90
unsigned char FUN_0048bf90(unsigned int arg0, unsigned int arg1) {
    if ((arg1 & 2) != 0) {
        DAT_006687c0 = 0;
        DAT_006687bc = 0;
        FUN_00498920();
        DAT_006687b0 = 4;
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        FUN_0048bd70();
        FUN_00466360(0x186, 0x18b);
        DAT_00668e38 = 0;
        InitGameInterface(1);
        DAT_008119b4 = 3;
        FUN_00474880();
        FUN_00458a50();
        FUN_004663c0();
        DAT_00798660 = 0;
        DAT_00798668 = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048c020
unsigned char FUN_0048c020(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4) {
    unsigned int temp;

    if ((a2 & 2) != 0) {
        DAT_006687c0 = 0;
        DAT_006687bc = 0;
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        temp = DAT_00798664;
        DAT_00798660 = 0;
        DAT_00798668 = 0;
        if (temp != 0) {
            FUN_0048bd70();
        } else {
            FUN_0048b770();
        }
        return FUN_0048fb80(a1, a2, a3, a4);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048c090
unsigned char FUN_0048c090(void *param1, unsigned char param2) {
    if (param2 & 2) {
        DAT_00798660 = 0;
        DAT_00798668 = 1;
        DAT_0080ff84 = 0xffffffff;
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        if (DAT_00798664 != 0) {
            FUN_0048bd70();
            lpConfig->field_28 = 6;
        } else {
            FUN_0048b770();
            lpConfig->field_28 = 1;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048c100
void FUN_0048c100(void) { STUB(); }
