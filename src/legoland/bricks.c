#include "legoland.h"

extern unsigned int DAT_004b90fc;
extern unsigned int DAT_004b90f8;

extern int SaveGameWrite(const void *data, unsigned int size);
extern int SaveGameRead(void *data, unsigned int size);

// FUNCTION: LEGOLAND 0x00457870
void FUN_00457870(int param_1) {
    DAT_004b90fc = (param_1 == 0);
}

// FUNCTION: LEGOLAND 0x00457890
int FUN_00457890(void) {
    return DAT_004b90fc == 0;
}

// FUNCTION: LEGOLAND 0x004578a0
void AddBricks(unsigned int param_1) {
    if (DAT_004b90fc == 0) {
        DAT_004b90f8 += param_1;
    }
}

// FUNCTION: LEGOLAND 0x004578c0
void UseBricks(unsigned int param_1) {
    if (DAT_004b90fc == 0) {
        DAT_004b90f8 -= param_1;
    }
}

// FUNCTION: LEGOLAND 0x004578e0
unsigned int GetBrickCount(void) {
    if (DAT_004b90fc != 0) {
        return 0x7fffffff;
    }
    return DAT_004b90f8;
}

// FUNCTION: LEGOLAND 0x00457900
void FUN_00457900(unsigned int param_1) {
    DAT_004b90f8 = param_1;
}

// FUNCTION: LEGOLAND 0x00457910
int FUN_00457910(void) {
    if (SaveGameWrite(&DAT_004b90fc, 4) == 0) {
        return 0;
    }
    return SaveGameWrite(&DAT_004b90f8, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00457940
int FUN_00457940(void) {
    if (SaveGameRead(&DAT_004b90fc, 4) == 0) {
        return 0;
    }
    return SaveGameRead(&DAT_004b90f8, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00457970
void FUN_00457970(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00457a70
void FUN_00457a70(void) { STUB(); }
