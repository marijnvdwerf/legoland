#include "legoland.h"
#include "globals.h"
#include "string.h"
#include "profile.h"
#include "profile_io.h"
#include "savegame_ui.h"
#include "options.h"
#include "icon.h"
#include "interface.h"
#include "sound_music.h"

#include "image_sprite.h"
#include "help.h"

// FUNCTION: LEGOLAND 0x0048eb40
void FUN_0048eb40(void) {
    DAT_006687bc = DAT_00798740;
    DAT_006687c0 = DAT_0079873c;
}

// FUNCTION: LEGOLAND 0x0048eb60
void InitOptionScreen(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048ef10
unsigned char FUN_0048ef10(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        UpDateCurrentProfile();
        DAT_0080ff88 = 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048ef40
unsigned char FUN_0048ef40(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_0080ffc4 = DAT_007cb314;
        DAT_0080ffc8 = DAT_007cb31c;
        DAT_0080ffcc = DAT_007cb315;
        DAT_0080ff88 = 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048ef90
unsigned char FUN_0048ef90(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        CloseFontEndCheckBox();
        DAT_008119b4 = 0;
        RemoveIconGroup(7);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048efd0
unsigned char FUN_0048efd0(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        FUN_00498920();
        DAT_006687b0 = 4;
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        if (DAT_0080ffe4 != 0) {
            UpDateCurrentSaveSlotInfo();
        }
        DAT_00668e38 = 0;
        RemoveIconGroup(7);
        KillTitleScreenSprites();
        DAT_008119b4 = 3;
        FUN_00474880();
        UpDateCurrentProfile();
        FUN_004993c0();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f050
unsigned char FUN_0048f050(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_007cb328 = 0;
        DAT_007cb324 = 0;
        DAT_0080ff88 = 4;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f0a0
unsigned char FUN_0048f0a0(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        InitExitCheckBox(0xfa, 0x28);
        DAT_007cb320 = 1;
        DAT_004bef9c = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f0f0
void InitExitCheckBox(unsigned int param_1, unsigned int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x0048f2d0
void PrintExitCheckBox(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048f440
unsigned char FUN_0048f440(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_007cb320 = 0;
        CloseFontEndCheckBox();
        DAT_00832ba0 = 3;
        DAT_00668e38 = 0;
        RemoveIconGroup(7);
        FUN_0046fb40(0xd2);
        KillTitleScreenSprites();
        DAT_008119b4 = 3;
        FUN_0048eb40();
        FUN_00474880();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f4b0
unsigned char FUN_0048f4b0(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(DAT_004b929c, 0, 1, 0);
        DAT_007cb320 = 0;
        CloseFontEndCheckBox();
        FUN_0048eb40();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f4f0
unsigned char FUN_0048f4f0(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_007cb320 = 0;
        CloseFontEndCheckBox();
        DAT_007cb310 = 1;
        DAT_007cb328 = 0;
        DAT_007cb324 = 0;
        DAT_0080ff88 = 4;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f550
unsigned char FUN_0048f550(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_007cb328 = 1;
        DAT_007cb324 = 0;
        DAT_0080ff88 = 4;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f5a0
unsigned char FUN_0048f5a0(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        StoreNewSaveGameToDisk();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f5d0
unsigned char FUN_0048f5d0(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f5f0
void FUN_0048f5f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048f760
void FUN_0048f760(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048f8d0
void FUN_0048f8d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048f9f0
void FUN_0048f9f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048fa40
void FUN_0048fa40(const char *param_1, const char *param_2, const char *param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x0048faa0
void KillTitleScreenSprites(void) {
    if (SPRITE_TitleScreenBk != 0) {
        KillSprite(SPRITE_TitleScreenBk);
        SPRITE_TitleScreenBk = 0;
    }
    if (DAT_007986b8 != 0) {
        KillSprite(DAT_007986b8);
        DAT_007986b8 = 0;
    }
    DAT_007986d8 = 0;
    DAT_007986dc = 0;
    DAT_007cb360 = 0;
    DAT_007986e0 = 0;
}

// FUNCTION: LEGOLAND 0x0048faf0
void RenderScreen(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048fb80
unsigned char FUN_0048fb80(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        if (DAT_00668e38 != 0) {
            DAT_00668e38 = 0;
        }
        DAT_0080ff88 = 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048fbd0
unsigned char FUN_0048fbd0(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_0080ff88 = 5;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048fc00
void FUN_0048fc00(void) {
    if (DAT_0080ff80 != 0) {
        if (FUN_0046d280(DAT_0080ff80) != 0) {
            DAT_0080ff80 = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x0048fc30
unsigned char FUN_0048fc30(void) {
    return DAT_0080ffd9 != 0;
}
