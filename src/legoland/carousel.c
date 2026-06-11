#include <stdlib.h>
#include "globals.h"
#include "legoland.h"

#include "binv.h"
#include "gamemap.h"
#include "llidb.h"
#include "map_object.h"
#include "sound_music.h"

struct CarouselNode {
    struct CarouselNode *next;
};

struct CarouselObject {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

struct CarouselBlockData {
    unsigned char pad_0[0x10];
    unsigned int field_10;
};

struct CarouselBlock {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[0x48];
    struct CarouselBlockData *field_64;
};

struct CarouselRoot {
    unsigned char pad_0[0xc];
    struct CarouselBlock *field_c;
};

struct CarouselSlotOwner {
    unsigned char pad_0[0x8];
    unsigned char *field_8;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x0042bbc0
void FUN_0042bbc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042bc00
void FUN_0042bc00(struct CarouselNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x0042bc40
void FUN_0042bc40(void) {
    while (DAT_006160c4 != NULL) {
        FUN_0042bc00(DAT_006160c4);
    }
}

// FUNCTION: LEGOLAND 0x0042bc60
void FUN_0042bc60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042bc90
void FUN_0042bc90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042bcf0
void FUN_0042bcf0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042c210
void FUN_0042c210(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042c280
void FUN_0042c280(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042c3f0
void FUN_0042c3f0(struct CarouselObject *input) {
    DAT_006160bc = input->field_c;
    KillSprite(DAT_0061606c);
    KillSprite(DAT_00616070);
    KillSprite(DAT_006160c0);
    FUN_0042bc40();
    Kill_FXList(CAROUSSEL_SFX, 2);
    FreeBinV(DAT_00616090);
    FreeBinV(DAT_00616094);
    FreeBinV(DAT_00616098);
}

// FUNCTION: LEGOLAND 0x0042c460
void FUN_0042c460(void) {
    EditMode = 1;
    DAT_008119b8 = (void *)DAT_006160bc;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x0042c4a0
void FUN_0042c4a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042c520
void FUN_0042c520(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042c550
unsigned int *FUN_0042c550(struct CarouselRoot *param1, unsigned short param2) {
    struct CarouselBlock *block = param1->field_c;

    DAT_006160a0 = (unsigned int)block->field_64;
    DAT_006160a4 = block->field_14;
    DAT_006160a8 = block->field_18;
    DAT_006160ac = param2;
    block->field_64->field_10 |= 0x2000;

    return &DAT_006160a0;
}

// FUNCTION: LEGOLAND 0x0042c590
int FUN_0042c590(void) {
    struct CarouselNode *current = DAT_006160c4;
    unsigned int flag = 1;
    unsigned int terminator = 0;

    while (current != NULL) {
        if (!SaveGameWrite(&flag, 4)) {
            return 0;
        }
        if (!SaveGameWrite(current, 0x2c)) {
            return 0;
        }
        current = current->next;
    }

    if (SaveGameWrite(&terminator, 4)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0042c600
void FUN_0042c600(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042c6d0
void FUN_0042c6d0(struct CarouselNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x0042c800
void FUN_0042c800(void) {
    struct CarouselNode *current = DAT_006160c4;

    while (current != NULL) {
        FUN_0042c6d0(current);
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x0042c820
void FUN_0042c820(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042cd20
int FUN_0042cd20(struct CarouselSlotOwner *param1, unsigned char *param2, signed char divisor) {
    int count = divisor;
    int eax = rand();
    int index = eax % count;

    while (param2[0x20 + index] != 0) {
        index++;
        if (index >= count) {
            index = 0;
        }
    }

    param2[0x20 + index] = 1;
    param1->field_8[0x36] = (unsigned char)(index + 1);
    return index + 1;
}
