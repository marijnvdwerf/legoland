#include "legoland.h"
#include "crt.h"
#include "globals.h"
#include "tooltip.h"
#include "string.h"
#include "options.h"
#include "sound_sfx.h"
#include "title.h"
#include "draw.h"
#include "nerps.h"
#include "icon.h"
#include "interface.h"
#include "sound_music.h"

struct PopUp {
    unsigned char pad_0[0x34];
    unsigned int field_34;
};

#include "image_sprite.h"
#include "stream.h"
#include "help.h"


// FUNCTION: LEGOLAND 0x0048fc40
LEGO_EXPORT void InitTitleScreen(void) {
    struct SpriteIcon *icon;

    FUN_00474ed0();
    // STRING: LEGOLAND 0x004bf574
    SPRITE_TitleScreenBk = LoadSprite("TitleScreenBk.lls", 0);

    // STRING: LEGOLAND 0x004bf560
    icon = LoadSpriteIcon("New_on_Title.lls", 4, 0xca, 0x138, 7);
    icon->field_3c = 7;
    icon->field_38 = GetString(7);
    icon->field_34 |= 0x6002;
    icon->event_handler = FUN_0048feb0;
    if (FUN_0048fc30() == 0) {
        // STRING: LEGOLAND 0x004bf548
        icon->field_18 = LoadSprite("Dark_New_On_Title.lls", 4);
        icon->field_28 = (void *)FUN_0046e920;
        icon->field_34 |= 8;
    } else {
        icon->field_18 = NULL;
    }
    DAT_006687bc = FUN_0048feb0;

    // STRING: LEGOLAND 0x004bf534
    icon = LoadSpriteIcon("Free_on_Title.lls", 4, 0x9a, 8, 7);
    icon->field_3c = 8;
    icon->field_38 = GetString(8);
    icon->field_34 |= 0x6002;
    icon->event_handler = FUN_0048ff20;
    if (FUN_0048fc30() == 0) {
        icon->field_34 |= 0x400;
    }

    // STRING: LEGOLAND 0x004bf520
    icon = LoadSpriteIcon("Reg_on_Title.lls", 4, 0x18, 0x71, 7);
    icon->field_3c = 10;
    icon->field_38 = GetString(10);
    icon->field_34 |= 0x6002;
    icon->event_handler = FUN_0048ff70;

    // STRING: LEGOLAND 0x004bf50c
    icon = LoadSpriteIcon("Exit_On_Title.lls", 4, 0x1e1, 0x13, 7);
    icon->field_3c = 1000;
    icon->field_38 = GetString(1000);
    icon->field_34 |= 0x6002;
    icon->event_handler = FUN_0048f0a0;
    DAT_006687c0 = FUN_0048f0a0;

    // STRING: LEGOLAND 0x004bf4f8
    icon = LoadSpriteIcon("Load_on_Title.lls", 4, 0x19, 0x118, 7);
    icon->field_3c = 9;
    icon->field_38 = GetString(9);
    icon->field_34 |= 0x6002;
    icon->event_handler = FUN_0048fe20;

    // STRING: LEGOLAND 0x004bf4e0
    icon = LoadSpriteIcon("MovieOn_On_Title.lls", 4, 0x10e, 0xa1, 7);
    icon->field_3c = 0x2bf;
    icon->field_38 = GetString(0x2bf);
    icon->field_34 |= 0x6002;
    icon->event_handler = FUN_0048ffe0;
    DAT_0080ff80 = 0x898;
}

// FUNCTION: LEGOLAND 0x0048fe20
unsigned char FUN_0048fe20(unsigned int param_1, unsigned char param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_007cb328 = 1;
        DAT_007cb324 = 1;
        DAT_0080ff88 = 4;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048feb0
unsigned char FUN_0048feb0(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        FUN_00498920();
        DAT_006687b0 = 4;
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_0080ffe5 = 1;
        RemoveIconGroup(7);
        KillTitleScreenSprites();
        DAT_008119b4 = 2;
        DAT_0080ff88 = 6;
        DAT_00832ba0 = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048ff20
unsigned char FUN_0048ff20(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        FUN_00498920();
        DAT_006687b0 = 4;
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_0080ff88 = 3;
        return 2;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048ff70
unsigned char FUN_0048ff70(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_0080ff88 = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048ffb0
void FUN_0048ffb0(void) {
    if (SPRITE_TitleScreenBk != 0) {
        KillSprite(SPRITE_TitleScreenBk);
        SPRITE_TitleScreenBk = 0;
    }
    RemoveIconGroup(7);
}

// FUNCTION: LEGOLAND 0x0048ffe0
unsigned char FUN_0048ffe0(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        FUN_0048f9f0(DAT_007cb30c, DAT_007cb300, DAT_007cb2f0);
        DAT_008119b4 = 2;
        DAT_0080ff84 = 0xffffffff;
        DAT_0080ff88 = 9;
        DAT_00668e38 = 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490050
unsigned char FUN_00490050(unsigned int param_1, unsigned char param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        FUN_0048ffb0();
        FUN_0048fa40(DAT_007cb30c, DAT_007cb300, DAT_007cb2f0);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490090
unsigned char FUN_00490090(unsigned int param_1, unsigned char param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        SetPointer(0);
        // STRING: LEGOLAND 0x004bf58c
        FUN_004771f0("Billund.avi", 1, 1);
        SetPointer(6);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004900d0
unsigned char FUN_004900d0(unsigned int param_1, unsigned char param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        SetPointer(0);
        // STRING: LEGOLAND 0x004bf598
        FUN_004771f0("Windsor.avi", 1, 1);
        SetPointer(6);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490110
unsigned char FUN_00490110(unsigned int param_1, unsigned char param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        SetPointer(0);
        // STRING: LEGOLAND 0x004bf5a4
        FUN_004771f0("California.avi", 1, 1);
        SetPointer(6);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490150
void FUN_00490150(void) {
    struct SpriteIcon *icon;

    // STRING: LEGOLAND 0x004bf5f4
    SPRITE_TitleScreenBk = LoadSprite("AdvertScreen.lls", 0);

    // STRING: LEGOLAND 0x004bf5dc
    icon = LoadSpriteIcon("GoBack_On_Advert.lls", 4, 0x13, 0x14b, 7);
    icon->field_3c = 0x26;
    icon->field_38 = GetString(0x26);
    icon->field_34 |= 0x6002;
    icon->event_handler = FUN_00490050;
    DAT_006687c0 = FUN_00490050;
    DAT_006687bc = 0;

    // STRING: LEGOLAND 0x004bf5cc
    icon = LoadSpriteIcon("California.lls", 4, 0x1c, 0x24, 7);
    icon->field_3c = 700;
    icon->field_38 = GetString(700);
    icon->field_34 |= 0x6002;
    icon->event_handler = FUN_00490110;

    // STRING: LEGOLAND 0x004bf5c0
    icon = LoadSpriteIcon("Windsor.lls", 4, 0xea, 0x24, 7);
    icon->field_3c = 0x2bd;
    icon->field_38 = GetString(0x2bd);
    icon->field_34 |= 0x6002;
    icon->event_handler = FUN_004900d0;

    // STRING: LEGOLAND 0x004bf5b4
    icon = LoadSpriteIcon("billund.lls", 4, 0x1b8, 0x24, 7);
    icon->field_3c = 0x2be;
    icon->field_38 = GetString(0x2be);
    icon->field_34 |= 0x6002;
    icon->event_handler = FUN_00490090;
}

// FUNCTION: LEGOLAND 0x00490270
void FUN_00490270(void) {
    if (SPRITE_TitleScreenBk != 0) {
        KillSprite(SPRITE_TitleScreenBk);
        SPRITE_TitleScreenBk = 0;
    }
    if (DAT_00798764 != 0) {
        KillSprite(DAT_00798764);
        DAT_00798764 = 0;
    }
    RemoveIconGroup(7);
}

// FUNCTION: LEGOLAND 0x004902c0
unsigned char FUN_004902c0(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        FUN_00490270();
        DAT_0080ff88 = 1;
        DAT_00798770 = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490300
unsigned char FUN_00490300(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        if (DAT_00798768 == 0 && DAT_0079876c == 0) {
            DAT_0079876c = 2;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490350
unsigned int FUN_00490350(void) {
    struct SpriteIcon *icon;
    struct Sprite *result;

    // STRING: LEGOLAND 0x004bf654
    SPRITE_TitleScreenBk = LoadSprite("CertificateScreen.lls", 0);
    DAT_00798770 = 1;

    // STRING: LEGOLAND 0x004bf638
    icon = LoadSpriteIcon("GoBack_On_Certificate.lls", 4, 12, 14, 7);
    icon->field_3c = 0x26;
    icon->field_38 = GetString(0x26);
    icon->field_34 |= 0x6002;
    icon->event_handler = FUN_004902c0;

    // STRING: LEGOLAND 0x004bf61c
    icon = LoadSpriteIcon("Print_On_Certificate.lls", 4, 0x207, 14, 7);
    icon->field_3c = 0x23d;
    icon->field_38 = GetString(0x23d);
    icon->field_34 |= 0x6002;
    icon->event_handler = FUN_00490300;

    // STRING: LEGOLAND 0x004bf60c
    result = LoadSprite("printinfo.lls", 4);
    DAT_00798764 = result;
    DAT_00798768 = 0;
    DAT_0079876c = 0;
    return (unsigned int)result;
}

// FUNCTION: LEGOLAND 0x00490410
void FUN_00490410(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00490600
void FUN_00490600(unsigned int param_1) {
    DAT_00798878 = param_1;
}

// FUNCTION: LEGOLAND 0x00490610
void FUN_00490610(const char *param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x00490680
unsigned int FUN_00490680(char *param_1, void **param_2, unsigned int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x00490740
void FUN_00490740(const char *param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x00490770
void FUN_00490770(const char *param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x004907a0
unsigned int FUN_004907a0(const char *param_1) {
    char buffer[128];
    unsigned int result;

    FUN_00490740(param_1);
    FUN_00490850();
    // STRING: LEGOLAND 0x004bf678
    sprintf(buffer, "Intervals\\%s", param_1);
    result = FUN_00490680(buffer, &DAT_007cafa0, 100);
    DAT_0079887c = result;
    if (result != 0) {
        DAT_004bf670 = 1;
        result = result - 1;
        DAT_0079887c = result;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00490800
void FUN_00490800(const char *param_1) {
    char buffer[128];

    FUN_00490770(param_1);
    FUN_00490880();
    sprintf(buffer, "Intervals\\%s", param_1);
    DAT_00798880 = FUN_00490680(buffer, &DAT_007cb140, 32);
}

// FUNCTION: LEGOLAND 0x00490850
void FUN_00490850(void) {
    if (DAT_0079887c != 0) {
        free(DAT_007cafa0);
    }
    DAT_0079887c = 0;
}

// FUNCTION: LEGOLAND 0x00490880
void FUN_00490880(void) {
    if (DAT_00798880 != 0) {
        free(DAT_007cb140);
    }
    DAT_00798880 = 0;
}

// FUNCTION: LEGOLAND 0x004908b0
void FUN_004908b0(void) {
    if (SPRITE_TitleScreenBk != 0) {
        KillSprite(SPRITE_TitleScreenBk);
        SPRITE_TitleScreenBk = 0;
    }
    if (DAT_0081c02c != 0) {
        KillSprite(DAT_0081c02c);
        DAT_0081c02c = 0;
    }
    if (DAT_0081c034 != 0) {
        KillSprite(DAT_0081c034);
        DAT_0081c034 = 0;
    }
    if (DAT_0081c080 != 0) {
        KillSprite(DAT_0081c080);
        DAT_0081c080 = 0;
    }
    if (DAT_0081c084 != 0) {
        KillSprite(DAT_0081c084);
        DAT_0081c084 = 0;
    }
    if (DAT_007caf80 != 0) {
        KillSprite(DAT_007caf80);
        DAT_007caf80 = 0;
    }
    if (DAT_007cb1c4 != 0) {
        KillSprite(DAT_007cb1c4);
        DAT_007cb1c4 = 0;
    }
    RemoveIconGroup(7);
}

// FUNCTION: LEGOLAND 0x00490970
unsigned char FUN_00490970(unsigned int param_1, unsigned char param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_00668e38 = 0;
        FUN_004908b0();
        DAT_008119b4 = 3;
        FUN_00474880();
        FUN_00490850();
        FUN_00490880();
        FUN_00490600(0);
        if (DAT_00798778[0] != 0) {
            SetPointer(0);
            FUN_004771f0(DAT_00798778, 1, 1);
            SetPointer(6);
            FUN_00490610(DAT_004d8bb0);
        }
        FUN_00498920();
        DAT_006687b0 = 4;
        FUN_004993c0();
        FUN_00492850();
        FUN_0046ce20();
        FUN_0046b760();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490a20
void FUN_00490a20(unsigned int param_1) {
    char buffer[256];

    // STRING: LEGOLAND 0x004bf688
    sprintf(buffer, "%s%02d.wav", DAT_007cae80, param_1 + 1);
    FUN_00498920();
    FUN_00498630(buffer);
    FUN_00498b00();
    FUN_0046d390();
}

// FUNCTION: LEGOLAND 0x00490a60
void FUN_00490a60(unsigned int param_1) {
    char buffer[256];

    sprintf(buffer, "%s%02d.wav", DAT_007cb1e0, param_1 + 1);
    FUN_00498920();
    FUN_00498630(buffer);
    FUN_00498b00();
    FUN_0046d390();
}

// FUNCTION: LEGOLAND 0x00490aa0
void FUN_00490aa0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00490b20
void FUN_00490b20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00490b90
unsigned char FUN_00490b90(unsigned int param_1, unsigned int param_2) {
    FUN_0046d680((struct IconNode *)DAT_007cb2e0, DAT_0081c084);
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_004bf670 -= 14;
        FUN_00490aa0();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490be0
void FUN_00490be0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00490c70
void FUN_00490c70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00490ea0
void FUN_00490ea0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00490fa0
void FUN_00490fa0(char *text, int font, RECT rc, int color_flag) { STUB(); }

// FUNCTION: LEGOLAND 0x00491080
void FUN_00491080(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004910f0
void FUN_004910f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004911c0
void FUN_004911c0(const char *a, const char *b) { STUB(); }

// FUNCTION: LEGOLAND 0x00491240
unsigned int FUN_00491240(const char *param_1) {
    unsigned int result;

    if (DAT_00668e9c == 0) {
        return 0;
    }
    if (FUN_004907a0(param_1) != 0) {
        ((struct PopUp *)DAT_00668e9c)->field_34 &= 0xfffffbff;
        result = 1;
    } else {
        ((struct PopUp *)DAT_00668e9c)->field_34 |= 0x400;
        result = 0;
    }
    FUN_00490850();
    return result;
}

// FUNCTION: LEGOLAND 0x00491290
LEGO_EXPORT void InitNewProfilePoPUp(struct Profile *profile) { STUB(); }
