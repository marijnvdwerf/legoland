#include "legoland.h"
#include <windows.h>

extern unsigned int DAT_004b9f8c;
extern unsigned int DAT_004b9f88;
extern unsigned int DAT_007fe920;
extern unsigned int DAT_006687a4;
extern unsigned int DAT_006687a8;

extern int FUN_00498cf0(void);

// FUNCTION: LEGOLAND 0x0046d340
void FUN_0046d340(unsigned int param_1) {
    if (FUN_00498cf0() != 0)
        goto set_flag;
    if (param_1 == 0)
        return;
    if (param_1 == DAT_004b9f8c)
        goto set_flag;
    DAT_004b9f8c = param_1;
    DAT_007fe920 = GetTickCount();
    DAT_006687a4 = 2;
    DAT_004b9f88 = 1;
set_flag:
    DAT_006687a8 = 1;
}

// FUNCTION: LEGOLAND 0x0046d390
void FUN_0046d390(void) {
    DAT_006687a4 = 5;
}
