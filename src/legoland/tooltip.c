#include "tooltip.h"
#include <windows.h>
#include "globals.h"
#include "legoland.h"

#include "image_sprite.h"
#include "stream.h"

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
