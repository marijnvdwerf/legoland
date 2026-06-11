#include "title.h"
#include <ddraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "draw.h"
#include "globals.h"
#include "icon.h"
#include "interface.h"
#include "legoland.h"
#include "nerps.h"
#include "options.h"
#include "sound_music.h"
#include "sound_sfx.h"
#include "string.h"
#include "text.h"
#include "tooltip.h"

struct PopUp {
    unsigned char pad_0[0x34];
    unsigned int field_34;
};

#include "certificate.h"
#include "help.h"
#include "image_sprite.h"
#include "print_sprite.h"
#include "profile.h"
#include "profile_io.h"
#include "resource.h"
#include "stream.h"
#include "timer.h"

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
void FUN_00490410(void) {
    RECT rc;
    int hit[3];

    hit[0] = 0;
    hit[2] = 0;
    hit[0] = 1;
    if (DAT_00798770 != 0) {
        rc.left = 0;
        rc.top = 0x102;
        rc.right = 0x280;
        rc.bottom = 0x130;
        NewPrintCent((char *)&DAT_0080ffa0, 0, rc, 0);
        if (DAT_0079876c != 0) {
            PrintSprite(DAT_00798764, 0xc6, 0x28, 0, hit);
            rc.left = 0xc6;
            rc.top = 0x2d;
            rc.right = (short)DAT_00798764->width + 0xc6;
            rc.bottom = 0x43;
            NewPrintCent(GetString(0x23a), 1, rc, 1);
            DAT_0079876c = DAT_0079876c + -1;
            if (DAT_0079876c == 0) {
                FUN_00498920();
                DAT_006687b0 = 4;
                DAT_00798768 = (-(unsigned int)(FUN_00451e20() != 0) & 0x118) - 0x8c;
            }
        } else if (0 < DAT_00798768) {
            PrintSprite(DAT_00798764, 0xc6, 0x28, 0, hit);
            rc.left = 0xc6;
            rc.top = 0x2d;
            rc.right = (short)DAT_00798764->width + 0xc6;
            rc.bottom = 0x43;
            NewPrintCent(GetString(0x23b), 1, rc, 1);
            DAT_00798768 = DAT_00798768 + -1;
        } else if (DAT_00798768 < 0) {
            PrintSprite(DAT_00798764, 0xc6, 0x28, 0, hit);
            rc.left = 0xc6;
            rc.top = 0x2d;
            rc.right = (short)DAT_00798764->width + 0xc6;
            rc.bottom = 0x43;
            NewPrintCent(GetString(0x23c), 1, rc, 1);
            DAT_00798768 = DAT_00798768 + 1;
        }
    }
}

// FUNCTION: LEGOLAND 0x00490600
void FUN_00490600(unsigned int param_1) {
    DAT_00798878 = param_1;
}

// FUNCTION: LEGOLAND 0x00490610
void FUN_00490610(const char *param_1) {
    if (strlen(param_1) > 0xff) {
        strncpy(DAT_00798778, param_1, 0x100);
        DAT_00798778[0xff] = 0;
    } else {
        strcpy(DAT_00798778, param_1);
        (&DAT_00798777)[strlen(param_1) + 1] = 0;
    }
}

// FUNCTION: LEGOLAND 0x00490680
unsigned int FUN_00490680(char *param_1, void **param_2, unsigned int param_3) {
    struct ResFile *file;
    int size;
    char *buffer;
    int pos;
    char c;
    int count;

    file = RES_OpenFile(param_1);
    if (file == NULL) {
        return 0;
    }
    size = RES_GetFileSize(file);
    if (size == 0) {
        RES_CloseFile(file);
        return 0;
    }
    buffer = malloc(size + 2);
    if (buffer == NULL) {
        RES_CloseFile(file);
        return 0;
    }
    RES_ReadFile(file, buffer, size);
    pos = 0;
    count = 0;
    while (count < (int)param_3 && pos < size) {
        *param_2 = buffer + pos;
        do {
            c = buffer[pos];
            pos = pos + 1;
        } while (c != '\r' && pos < size);
        if (size <= pos) {
            pos = pos + 1;
        }
        buffer[pos + -1] = 0;
        pos = pos + 1;
        count = count + 1;
        param_2 = param_2 + 1;
    }
    RES_CloseFile(file);
    return (unsigned int)count;
}

// FUNCTION: LEGOLAND 0x00490740
void FUN_00490740(const char *param_1) {
    char fname[64];

    _splitpath(param_1, 0, 0, fname, 0);
    // STRING: LEGOLAND 0x004bf674
    sprintf(DAT_007cae80, "%s_", fname);
}

// FUNCTION: LEGOLAND 0x00490770
void FUN_00490770(const char *param_1) {
    char fname[64];

    _splitpath(param_1, 0, 0, fname, 0);
    sprintf(DAT_007cb1e0, "%s_", fname);
}

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
unsigned char FUN_00490970(unsigned int param_1, unsigned char param_2, unsigned int param_3, unsigned int param_4) {
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
void FUN_00490aa0(void) {
    if ((int)DAT_004bf670 <= 1) {
        DAT_004bf670 = 1;
        DAT_007cb2e0->field_34 = DAT_007cb2e0->field_34 | 0x400;
    } else {
        DAT_007cb2e0->field_34 = DAT_007cb2e0->field_34 & 0xfffffbff;
    }
    if ((int)(DAT_004bf670 + 0xe) > (int)DAT_0079887c) {
        DAT_007cb2e4->field_34 |= 0x400;
    } else {
        DAT_007cb2e4->field_34 &= 0xfffffbff;
    }
    FUN_00490a20((int)DAT_004bf670 / 0xe);
}

// FUNCTION: LEGOLAND 0x00490b20
unsigned char FUN_00490b20(unsigned int param_1, unsigned int param_2) {
    FUN_0046d680(DAT_007cb2e4, DAT_0081c034);
    if ((param_2 & 2) != 0) {
        if ((DAT_007cb2e4->field_34 & 0x400) != 0) {
            return FUN_00490970(0, param_2, 0, 0);
        }
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_004bf670 = DAT_004bf670 + 0xe;
        FUN_00490aa0();
    }
    return 1;
}

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
unsigned char FUN_00490be0(struct IconNode *param_1, unsigned int param_2) {
    FUN_0046d680(param_1, DAT_007cb1c4);
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        do {
            DAT_00798884 = DAT_00798884 + 1;
            if ((int)DAT_00798884 >= (int)DAT_00798880) {
                DAT_00798884 = 0;
            }
        } while (strlen(((char **)&DAT_007cb140)[DAT_00798884]) == 0);
        DAT_00798888 = GetTicks() + 8000;
        FUN_00490a60(DAT_00798884);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490c70
void FUN_00490c70(void) {
    struct SpriteIcon *accept;
    struct IconNode *icon;

    FUN_00499380();
    FUN_00492830();
    // STRING: LEGOLAND 0x004bf6cc
    SPRITE_TitleScreenBk = LoadSprite("Interval_Screen.lls", 0);
    // STRING: LEGOLAND 0x004b8198
    DAT_0081c02c = LoadSprite("NextPage.lls", 4);
    // STRING: LEGOLAND 0x004b8188
    DAT_0081c034 = LoadSprite("NextPageLit.lls", 4);
    // STRING: LEGOLAND 0x004b8174
    DAT_0081c080 = LoadSprite("PreviousPage.lls", 4);
    // STRING: LEGOLAND 0x004b8160
    DAT_0081c084 = LoadSprite("PreviousPageLit.lls", 4);
    // STRING: LEGOLAND 0x004bf6bc
    DAT_007caf80 = LoadSprite("Rep_Hint1.lls", 4);
    // STRING: LEGOLAND 0x004bf6ac
    DAT_007cb1c4 = LoadSprite("Rep_Hint2.lls", 4);

    // STRING: LEGOLAND 0x004bf694
    accept = LoadSpriteIcon("Accept_On_Report.lls", 4, 0x20a, 0x16c, 7);
    if (DAT_00798878 != 0) {
        accept->field_3c = 0x244;
        accept->field_38 = GetString(0x244);
    } else {
        accept->field_3c = 0x245;
        accept->field_38 = GetString(0x245);
    }
    accept->field_34 |= 0x6002;
    accept->event_handler = FUN_00490970;
    DAT_006687c0 = FUN_00490970;

    icon = InsertIcon(0x1b9, 0x50, 7, DAT_007caf80);
    icon->field_3c = 0x2d0;
    icon->field_38 = GetString(0x2d0);
    icon->field_2c = FUN_00490be0;
    icon->field_34 |= 0x6002;
    DAT_007cb1c0 = icon;
    if (DAT_00798880 == 0) {
        icon->field_34 |= 0x400;
    }

    DAT_00798888 = 0;
    DAT_00798884 = 0xffffffff;
    DAT_007cb2e4 = InsertIcon(0x1b9, 0x1ae, 7, DAT_0081c02c);
    DAT_007cb2e4->field_3c = 0x88e;
    DAT_007cb2e4->field_38 = GetString(0x88e);
    DAT_007cb2e4->field_34 |= 0x2000;
    DAT_007cb2e4->field_34 |= 0x4002;
    DAT_007cb2e4->field_2c = FUN_00490b20;
    DAT_006687bc = FUN_00490b20;

    DAT_007cb2e0 = InsertIcon(6, 0x1ae, 7, DAT_0081c080);
    DAT_007cb2e0->field_3c = 0x88f;
    DAT_007cb2e0->field_38 = GetString(0x88f);
    DAT_007cb2e0->field_34 |= 0x2000;
    DAT_007cb2e0->field_34 |= 0x4002;
    DAT_007cb2e0->field_2c = FUN_00490b90;
    DAT_004bf670 = 1;
    FUN_00490aa0();
}

// FUNCTION: LEGOLAND 0x00490ea0
void FUN_00490ea0(void) {
    if ((int)DAT_00813a44 < DAT_007cb2e4->field_c ||
        (int)DAT_00813a44 > DAT_007cb2e4->field_10 + DAT_007cb2e4->field_c ||
        (int)DAT_00813a48 < DAT_007cb2e4->field_e ||
        (int)DAT_00813a48 > DAT_007cb2e4->field_12 + DAT_007cb2e4->field_e) {
        if (GetBlink() != 0) {
            FUN_0046d680(DAT_007cb2e4, DAT_0081c02c);
        } else {
            FUN_0046d680(DAT_007cb2e4, DAT_0081c034);
        }
    }
    if ((int)DAT_00813a44 < DAT_007cb2e0->field_c ||
        (int)DAT_00813a44 > DAT_007cb2e0->field_10 + DAT_007cb2e0->field_c ||
        (int)DAT_00813a48 < DAT_007cb2e0->field_e ||
        (int)DAT_00813a48 > DAT_007cb2e0->field_12 + DAT_007cb2e0->field_e) {
        FUN_0046d680(DAT_007cb2e0, DAT_0081c080);
    }
    if ((int)DAT_00813a44 < DAT_007cb1c0->field_c ||
        (int)DAT_00813a44 > DAT_007cb1c0->field_10 + DAT_007cb1c0->field_c ||
        (int)DAT_00813a48 < DAT_007cb1c0->field_e ||
        (int)DAT_00813a48 > DAT_007cb1c0->field_12 + DAT_007cb1c0->field_e) {
        FUN_0046d680(DAT_007cb1c0, DAT_007caf80);
    }
}

// FUNCTION: LEGOLAND 0x00490fa0
void FUN_00490fa0(char *text, int font, RECT rc, int color_flag) {
    HRGN region = CreateRectRgn(SPRITE_ClipRect.left, SPRITE_ClipRect.top, SPRITE_ClipRect.right, SPRITE_ClipRect.bottom);
    HDC hdc;
    HGDIOBJ old_region;

    PushRenderingStatusAndUnlockVideoSurface();
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &hdc);
    SetBkMode(hdc, 1);
    if ((char)color_flag == 1) {
        SetTextColor(hdc, 0xffffff);
    }
    old_region = SelectObject(hdc, region);
    color_flag = (int)SelectFont(hdc, font);
    DrawTextA(hdc, text, strlen(text), &rc, 0x24);
    SelectObject(hdc, (HGDIOBJ)color_flag);
    SelectObject(hdc, old_region);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
    PopRenderingStatus();
    DeleteObject(region);
}

// FUNCTION: LEGOLAND 0x00491080
void FUN_00491080(char *param_1, int param_2, int param_3, int param_4, int param_5) {
    RECT rc;

    if (param_1 != NULL) {
        rc.left = param_2;
        rc.top = param_3;
        rc.right = param_2 + 0x1cc;
        rc.bottom = param_4 + param_3;
        if (param_5 != 0) {
            NewPrintCent(param_1, 3, rc, 0);
        } else {
            FUN_00490fa0(param_1, 2, rc, 0);
        }
    }
}

// FUNCTION: LEGOLAND 0x004910f0
void FUN_004910f0(void) {
    unsigned int rc[4];
    char **line;
    int y;
    int n;
    int idx;

    FUN_00491080(DAT_007cafa0, 10, 0x45, 0x27, 1);
    y = 0x6b;
    n = 0;
    line = (char **)&DAT_007cafa0 + DAT_004bf670;
    idx = DAT_004bf670;
    while (n < 0xe && idx <= (int)DAT_0079887c) {
        FUN_00491080(*line, 10, y, 0x16, 0);
        n = n + 1;
        idx = idx + 1;
        line = line + 1;
        y = y + 0x18;
    }
    FUN_00490ea0();
    if (DAT_00798888 != 0) {
        rc[0] = 0x1db;
        rc[1] = 5;
        rc[2] = 0x280;
        rc[3] = 0x78;
        if ((int)GetTicks() > (int)DAT_00798888) {
            DAT_00798888 = 0;
        }
        BubbleHelp(rc, (unsigned int)((char **)&DAT_007cb140)[DAT_00798884], 2);
    }
}

// FUNCTION: LEGOLAND 0x004911c0
unsigned int FUN_004911c0(const char *a, const char *b) {
    if (FUN_004907a0(a) != 0) {
        if (b != 0) {
            FUN_00490800(b);
        }
        DAT_00668e38 = 1;
        DAT_008119b4 = 2;
        DAT_0080ff84 = 0xffffffff;
        DAT_0080ff88 = 7;
        if (DAT_00668e9c != 0) {
            ((struct PopUp *)DAT_00668e9c)->field_34 &= 0xfffffbff;
        }
        return 1;
    }
    if (DAT_00668e9c != 0) {
        ((struct PopUp *)DAT_00668e9c)->field_34 |= 0x400;
    }
    return 0;
}

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
LEGO_EXPORT void InitNewProfilePoPUp(struct Profile *profile) {
    struct SpriteIcon *icon = (struct SpriteIcon *)profile;
    struct SpriteIcon *popup;

    // STRING: LEGOLAND 0x004bf6e4
    popup = LoadSpriteIcon("Reg_NewProfileBK.lls", 4, icon->x, icon->y - 0x1b, 0x15);
    popup->field_3c = 0x50;
    popup->field_38 = GetString(0x50);
    popup->field_34 |= 0x2000;
    EnterNewProfileCheckBoxIcons(popup);
    ResetTempProfile();
}
