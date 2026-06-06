#include "legoland.h"
#include "sound_music.h"


extern unsigned int DAT_00667114;
extern unsigned int DAT_00667118;
extern unsigned int DAT_0066711c;
extern unsigned char DAT_004b8710[0x40];
extern unsigned char DAT_004b8750[0x40];
extern unsigned char DINO_SFX[];

// FUNCTION: LEGOLAND 0x00452990
void FUN_00452990(void) {
    unsigned int counter;

    counter = DAT_00667114;
    DAT_00667114++;
    if (counter == 0) {
        Load_FXList(DAT_004b8710, 5);
    }
}

// FUNCTION: LEGOLAND 0x004529c0
void FUN_004529c0(void) {
    DAT_00667114--;
    if (DAT_00667114 == 0) {
        Kill_FXList(DAT_004b8710, 5);
    }
}

// FUNCTION: LEGOLAND 0x004529e0
void FUN_004529e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00452a30
void RemoveSoundObject(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00452a80
void FUN_00452a80(void) {
    unsigned int counter;

    counter = DAT_00667118;
    DAT_00667118++;
    if (counter == 0) {
        Load_FXList(DAT_004b8750, 2);
    }
}

// FUNCTION: LEGOLAND 0x00452ab0
void FUN_00452ab0(void) {
    DAT_00667118--;
    if (DAT_00667118 == 0) {
        Kill_FXList(DAT_004b8750, 2);
    }
}

// FUNCTION: LEGOLAND 0x00452ad0
void FUN_00452ad0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00452b20
void FUN_00452b20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00452b70
void FUN_00452b70(void) {
    unsigned int counter;

    counter = DAT_0066711c;
    DAT_0066711c++;
    if (counter == 0) {
        Load_FXList(DINO_SFX, 5);
    }
}

// FUNCTION: LEGOLAND 0x00452ba0
void FUN_00452ba0(void) {
    DAT_0066711c--;
    if (DAT_0066711c == 0) {
        Kill_FXList(DINO_SFX, 5);
    }
}

// FUNCTION: LEGOLAND 0x00452bc0
void FUN_00452bc0(void) { STUB(); }
