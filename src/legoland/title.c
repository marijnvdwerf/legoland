#include "legoland.h"
#include "tooltip.h"

struct SpriteIcon {
    unsigned char pad_0[0x2c];
    unsigned char (*event_handler)(unsigned int param_1, unsigned int param_2);
    unsigned char pad_30[0x34 - 0x30];
    unsigned int field_34;
    unsigned int field_38;
    unsigned int field_3c;
};

struct PopUp {
    unsigned char pad_0[0x34];
    unsigned int field_34;
};

extern void PlayInstanceOfSample(void *sample, unsigned int param_2, unsigned int param_3, unsigned int param_4);
extern void KillSprite(unsigned int sprite);
extern void RemoveIconGroup(unsigned int group);
extern void SetPointer(unsigned int index);
extern unsigned int LoadSprite(const char *filename, unsigned int param_2);
extern struct SpriteIcon *LoadSpriteIcon(const char *filename, unsigned int param_2, unsigned int param_3, unsigned int param_4, unsigned int param_5);
extern unsigned int GetString(unsigned int id);
extern void FUN_0048fa40(const char *param_1, const char *param_2, const char *param_3);
extern void FUN_00498920(void);
extern void FUN_004771f0(const char *filename, unsigned int param_2, unsigned int param_3);
extern void FUN_00498630(const char *param_1);
extern void FUN_00498b00(void);
extern void FUN_0046d680(unsigned int param_1, unsigned int param_2);
extern void FUN_00474880(void);
extern void FUN_004993c0(void);
extern void FUN_00492850(void);
extern void FUN_0046ce20(void);
extern void FUN_0046b760(void);
extern void FUN_0049e4d0(void *ptr);
extern int FUN_0049e573(char *buffer, const char *format, ...);

extern unsigned int DAT_004bef9c;
extern void *PTR_004b92c0;
extern unsigned int DAT_007cb324;
extern unsigned int DAT_007cb328;
extern unsigned int DAT_0080ff88;
extern unsigned int DAT_006687b0;
extern unsigned int SPRITE_TitleScreenBk;
extern char DAT_007cb2f0[];
extern char DAT_007cb300[];
extern char DAT_007cb30c[];
extern unsigned int DAT_00798764;
extern unsigned int DAT_00798768;
extern unsigned int DAT_0079876c;
extern unsigned int DAT_00798770;
extern unsigned int DAT_00798878;
extern unsigned int DAT_0079887c;
extern unsigned int DAT_00798880;
extern unsigned int DAT_004bf670;
extern void *DAT_007cafa0;
extern void *DAT_007cb140;
extern unsigned int DAT_0081c02c;
extern unsigned int DAT_0081c034;
extern unsigned int DAT_0081c080;
extern unsigned int DAT_0081c084;
extern unsigned int DAT_007caf80;
extern unsigned int DAT_007cb1c4;
extern unsigned int DAT_00668e38;
extern unsigned int DAT_008119b4;
extern char DAT_00798778[];
extern char DAT_004d8bb0[];
extern char DAT_007cae80[];
extern char DAT_007cb1e0[];
extern unsigned int DAT_007cb2e0;
extern struct PopUp *DAT_00668e9c;

void FUN_0048ffb0(void);
void FUN_00490270(void);
unsigned char FUN_004902c0(unsigned int param_1, unsigned int param_2);
unsigned char FUN_00490300(unsigned int param_1, unsigned int param_2);
void FUN_00490600(unsigned int param_1);
void FUN_00490610(const char *param_1);
unsigned int FUN_00490680(char *param_1, void **param_2, unsigned int param_3);
void FUN_00490740(const char *param_1);
void FUN_00490770(const char *param_1);
unsigned int FUN_004907a0(const char *param_1);
void FUN_00490850(void);
void FUN_00490880(void);
void FUN_004908b0(void);
void FUN_00490aa0(void);

// FUNCTION: LEGOLAND 0x0048fc40
void InitTitleScreen(void) { STUB(); }

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
void FUN_0048feb0(void) { STUB(); }

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
void FUN_0048ffe0(void) { STUB(); }

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
void FUN_00490150(void) { STUB(); }

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
    unsigned int result;

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
    return result;
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
    FUN_0049e573(buffer, "Intervals\\%s", param_1);
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
    FUN_0049e573(buffer, "Intervals\\%s", param_1);
    DAT_00798880 = FUN_00490680(buffer, &DAT_007cb140, 32);
}

// FUNCTION: LEGOLAND 0x00490850
void FUN_00490850(void) {
    if (DAT_0079887c != 0) {
        FUN_0049e4d0(DAT_007cafa0);
    }
    DAT_0079887c = 0;
}

// FUNCTION: LEGOLAND 0x00490880
void FUN_00490880(void) {
    if (DAT_00798880 != 0) {
        FUN_0049e4d0(DAT_007cb140);
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
    FUN_0049e573(buffer, "%s%02d.wav", DAT_007cae80, param_1 + 1);
    FUN_00498920();
    FUN_00498630(buffer);
    FUN_00498b00();
    FUN_0046d390();
}

// FUNCTION: LEGOLAND 0x00490a60
void FUN_00490a60(unsigned int param_1) {
    char buffer[256];

    FUN_0049e573(buffer, "%s%02d.wav", DAT_007cb1e0, param_1 + 1);
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
    FUN_0046d680(DAT_007cb2e0, DAT_0081c084);
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
void FUN_00490fa0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00491080
void FUN_00491080(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004910f0
void FUN_004910f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004911c0
void FUN_004911c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00491240
unsigned int FUN_00491240(const char *param_1) {
    unsigned int result;

    if (DAT_00668e9c == 0) {
        return 0;
    }
    if (FUN_004907a0(param_1) != 0) {
        DAT_00668e9c->field_34 &= 0xfffffbff;
        result = 1;
    } else {
        DAT_00668e9c->field_34 |= 0x400;
        result = 0;
    }
    FUN_00490850();
    return result;
}

// FUNCTION: LEGOLAND 0x00491290
void InitNewProfilePoPUp(void) { STUB(); }
