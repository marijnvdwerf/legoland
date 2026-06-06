#include "legoland.h"
#include <windows.h>

#include "timer.h"
#include "text.h"
#include "nerps.h"
#include "popupinfo.h"
#include "objectives.h"
#include "challenge.h"
#include "icon.h"
#include "interface.h"

struct HelpAdvisor {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

extern unsigned int DAT_007fe040;
extern unsigned int DAT_007fe044;
extern unsigned int DAT_007fe048;
extern unsigned int DAT_007fe04c;
extern unsigned int DAT_007fe050;
extern unsigned int DAT_007fe054;
extern unsigned int DAT_007fe920;
extern struct HelpAdvisor *DAT_00668724;
extern unsigned int DAT_00668618;
extern unsigned int DAT_004b9f88;
extern unsigned int DAT_004b9f8c;
extern unsigned int DAT_006687a4;
extern unsigned int DAT_006687a8;
extern unsigned int DAT_006687ac;
extern unsigned int DAT_004b9f78[4];

extern void FUN_0049e4d0(void *block);
extern void FUN_004988c0(void);
extern unsigned int FUN_00498cf0(void);

// FUNCTION: LEGOLAND 0x0046ce20
void FUN_0046ce20(void) {
    if ((DAT_007fe040 & 0x3) != 0) {
        FUN_0049e4d0((void *)DAT_007fe048);
        FUN_004748a0((void *)1);
    }
    DAT_007fe040 &= ~0x3;
    FUN_00476000();
    FUN_00444070(0, 0);
    FUN_004988c0();
}

// FUNCTION: LEGOLAND 0x0046ce60
void DisplayAdvisorHelp(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046cee0
unsigned int FUN_0046cee0(void) {
    unsigned int result = GetGameTimer() - DAT_007fe050;
    if ((int)result >= 0) {
        DAT_00668618 = 0;
        return 1;
    }
    if (DAT_00668724 != NULL) {
        if (DAT_00668724->field_c == 0) {
            if (DAT_00668618 == 0) {
                return 0;
            }
        }
    } else {
        if (DAT_00668618 == 0) {
            return 0;
        }
    }
    DAT_00668618 = 0;
    return 1;
}

// FUNCTION: LEGOLAND 0x0046cf20
unsigned int FUN_0046cf20(void) {
    int diff = (int)(GetGameTimer() - DAT_007fe04c);
    if (diff > 0x7530) {
        goto return_1;
    }
    if (DAT_007fe044 == 0) {
        goto return_0;
    }
    if (DAT_00668724 == NULL) {
        goto return_0;
    }
    if (DAT_00668724->field_c == 0) {
        goto return_0;
    }
return_1:
    return 1;
return_0:
    return 0;
}

// FUNCTION: LEGOLAND 0x0046cf60
void ProcessInGameHelp(void) {
    int delta;

    FUN_00469400();

    delta = (int)(GetGameTimer() - DAT_007fe054);
    if (delta > 0xc8) {
        FUN_0046b2d0();
        DAT_007fe054 = GetGameTimer();
    }

    if ((DAT_007fe040 & 0x1) != 0) {
        if (FUN_0046cf20() != 0) {
            FUN_0046ce20();
            DAT_007fe050 = GetGameTimer();
        } else {
            BubbleHelp(DAT_004b9f78, DAT_007fe048, 2);
            DAT_007fe050 = GetGameTimer();
        }
    } else {
        if (FUN_0046cee0() != 0) {
            FUN_00468c00();
        }
    }

    RenderHelpIcons();
    FUN_00473660();
}

// FUNCTION: LEGOLAND 0x0046cff0
void FUN_0046cff0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046d080
void ProcessFrontEndHelp(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046d110
void FUN_0046d110(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046d230
void FUN_0046d230(unsigned int a1) {
    if (FUN_00498cf0() == 0) {
        if (a1 == 0xffffffff) {
            return;
        }
        if (a1 != DAT_004b9f8c) {
            DAT_004b9f8c = a1;
            DAT_007fe920 = GetTickCount();
            DAT_006687a4 = 0;
            DAT_004b9f88 = 1;
            FUN_004735b0();
        }
    }
    DAT_006687a8 = 1;
}

// FUNCTION: LEGOLAND 0x0046d280
unsigned int FUN_0046d280(unsigned int a1) {
    if (FUN_00498cf0() == 0) {
        if (a1 != 0xffffffff && a1 != DAT_004b9f8c) {
            DAT_004b9f8c = a1;
            DAT_007fe920 = GetTickCount();
            DAT_006687a4 = 0;
            DAT_004b9f88 = 1;
            DAT_006687ac = 1;
            FUN_004735b0();
            DAT_006687a8 = 1;
            FUN_0046d110();
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046d2f0
void FUN_0046d2f0(unsigned int a1) {
    if (FUN_00498cf0() != 0) {
        DAT_006687a8 = 1;
        return;
    }
    if (a1 != 0) {
        if (a1 != DAT_004b9f8c) {
            DAT_004b9f8c = a1;
            DAT_007fe920 = GetTickCount();
            DAT_006687a4 = 1;
            DAT_004b9f88 = 1;
        }
        DAT_006687a8 = 1;
    }
}
