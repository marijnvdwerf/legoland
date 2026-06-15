#include "interface.h"
#include <stdlib.h>
#include <string.h>
#include "challenge.h"
#include "controller.h"
#include "draw.h"
#include "globals.h"
#include "icon.h"
#include "legoland.h"
#include "llidb.h"
#include "nerps.h"
#include "objclass.h"
#include "popupinfo.h"
#include "print_sprite.h"
#include "profile_io.h"
#include "screens.h"
#include "sound_music.h"
#include "string.h"
#include "timer.h"
#include "title.h"
#include "wndenv.h"
#include "worker_mouse.h"

struct ProfileObj {
    unsigned char pad_0[0x34];
    unsigned int flags;
};

struct InterfaceObj {
    unsigned char pad_0[0x2c];
    void *field_2c;
    unsigned char pad_30[0x34 - 0x30];
    unsigned int field_34;
    char *field_38;
    unsigned int field_3c;
};

struct ObjectClassInfo {
    /* 0x00 */ char *name;
    /* 0x04 */ unsigned char pad_4[0x8 - 0x4];
    /* 0x08 */ unsigned int flags;
    /* 0x0c */ struct EditObject *field_c;
};

struct BuildObject;

struct LLDBElem {
    unsigned char pad_0[0x8];
    /* 0x08 */ unsigned int flags;
    /* 0x0c */ struct BuildObject *obj;
};

struct MovieHandle {
    /* 0x00 */ unsigned int field_0;
    /* 0x04 */ unsigned int field_4;
    /* 0x08 */ int field_8;
    /* 0x0c */ int field_c;
    /* 0x10 */ void *file;
    /* 0x14 */ void *frame;
    /* 0x18 */ void *audio_stream;
    /* 0x1c */ void *video_stream;
    /* 0x20 */ unsigned char pad_20[0x28 - 0x20];
};

struct AviFileInfo {
    unsigned char pad_0[0xc];
    /* 0x0c */ int streams;
    unsigned char pad_10[0x6c - 0x10];
};

struct AviStreamInfo {
    /* 0x00 */ unsigned int type;
    unsigned char pad_4[0x14 - 0x4];
    /* 0x14 */ unsigned int scale;
    /* 0x18 */ unsigned int rate;
    unsigned char pad_1c[0x20 - 0x1c];
    /* 0x20 */ unsigned int length;
    unsigned char pad_24[0x30 - 0x24];
    /* 0x30 */ unsigned int sample_size;
    /* 0x34 */ int frame_left;
    /* 0x38 */ int frame_top;
    /* 0x3c */ int frame_right;
    /* 0x40 */ int frame_bottom;
    unsigned char pad_44[0x8c - 0x44];
};

struct BuildObject {
    unsigned char pad_0[0x1c];
    /* 0x1c */ unsigned int field_1c;
    unsigned char pad_20[0x58 - 0x20];
    /* 0x58 */ void *field_58;
    /* 0x5c */ void *field_5c;
    /* 0x60 */ void *field_60;
    unsigned char pad_64[0x7c - 0x64];
    /* 0x7c */ char *field_7c;
    unsigned char pad_80[0xc4 - 0x80];
    /* 0xc4 */ struct ObjectClassInfo *field_c4;
};

struct PathControlElem {
    unsigned char pad_0[0xc];
    /* 0x0c */ struct BuildObject *obj;
};

struct EventNode {
    struct EventNode *next;
    unsigned char pad_4[0x1c - 0x4];
    int sort_key;
};

struct QueryNode {
    struct QueryNode *next;
    unsigned char pad_4[0x8 - 0x4];
    unsigned int field_8;
    unsigned int field_c;
};

#include "image_sprite.h"
#include "imports.h"
#include "mapscreen.h"
#include "stream.h"

// FUNCTION: LEGOLAND 0x004741f0
LEGO_EXPORT void Load_Interface_ControlIcons(void) {
    if (DAT_00668ea4 != 0) {
        return;
    }
    DAT_00668ea4 = 1;
    // STRING: LEGOLAND 0x004bb3b4
    DAT_00668e68 = LoadSprite("InterfaceBG.lls", 4);
    // STRING: LEGOLAND 0x004bb3a4
    DAT_00668e6c = LoadSprite("No_Energy.lls", 4);
    // STRING: LEGOLAND 0x004bb394
    DAT_00668e70 = LoadSprite("Bar_pointer.lls", 4);
    // STRING: LEGOLAND 0x004bb37c
    DAT_007fdcd0 = LoadSprite("IF_PathIconPressed.lls", 4);
    // STRING: LEGOLAND 0x004bb36c
    DAT_007fdd50 = LoadSprite("IF_PathIcon.lls", 4);
    // STRING: LEGOLAND 0x004bb354
    DAT_007fdcd4 = LoadSprite("IF_QueryIconPressed.lls", 4);
    // STRING: LEGOLAND 0x004bb340
    DAT_007fdd54 = LoadSprite("IF_Queryicon.lls", 4);
    // STRING: LEGOLAND 0x004bb324
    DAT_007fdcd8 = LoadSprite("IF_EraserIconPressed.lls", 4);
    // STRING: LEGOLAND 0x004bb310
    DAT_007fdd58 = LoadSprite("IF_EraserIcon.lls", 4);
    // STRING: LEGOLAND 0x004bb2f8
    DAT_007fdcdc = LoadSprite("IF_MapIconPressed.lls", 4);
    // STRING: LEGOLAND 0x004bb2e8
    DAT_007fdd5c = LoadSprite("IF_Mapicon.lls", 4);
    // STRING: LEGOLAND 0x004bb2cc
    DAT_007fdce0 = LoadSprite("IF_OptionsIconPressed.lls", 4);
    // STRING: LEGOLAND 0x004bb2b8
    DAT_007fdd60 = LoadSprite("IF_OptionsIcon.lls", 4);
    // STRING: LEGOLAND 0x004bb29c
    DAT_00668e74 = LoadSprite("Attract_Highlight_On.lls", 4);
    // STRING: LEGOLAND 0x004bb280
    DAT_00668e78 = LoadSprite("Attract_Highlight_Off.lls", 4);
    // STRING: LEGOLAND 0x004bb26c
    DAT_00668e7c = LoadSprite("Attract_New_Off.lls", 4);
    // STRING: LEGOLAND 0x004bb258
    DAT_00668e80 = LoadSprite("Attract_New_On.lls", 4);
    // STRING: LEGOLAND 0x004bb240
    DAT_00668e84 = LoadSprite("Side_ScrollDown_Lit.lls", 4);
    // STRING: LEGOLAND 0x004bb228
    DAT_00668e88 = LoadSprite("Side_ScrollUp_Lit.lls", 4);
    // STRING: LEGOLAND 0x004bb218
    DAT_00668e8c = LoadSprite("Link_Middle.lls", 4);
    // STRING: LEGOLAND 0x004bb208
    DAT_00668e90 = LoadSprite("Link_Bottom.lls", 4);
    // STRING: LEGOLAND 0x004bb1f8
    DAT_00668e94 = LoadSprite("BriefIcon2.lls", 4);
    // STRING: LEGOLAND 0x004bb1e8
    DAT_00668e98 = LoadSprite("BriefIcon.lls", 4);
    // STRING: LEGOLAND 0x004bb1d8
    DAT_00668ea0 = LoadSprite("ScriptEnd.lls", 4);
}

// FUNCTION: LEGOLAND 0x004743b0
LEGO_EXPORT void UnLoad_Interface_ControlIcons(void) {
    if (DAT_00668ea4 != 0) {
        DAT_007fdd70[0] = NULL;
        DAT_007fdd70[1] = NULL;
        DAT_00668ea4 = 0;
        DAT_007fdd70[2] = NULL;
        DAT_007fdd70[3] = NULL;
        KillSprite(DAT_00668e68);
        DAT_00668e68 = NULL;
        KillSprite(DAT_00668e6c);
        DAT_00668e6c = NULL;
        KillSprite(DAT_00668e70);
        DAT_00668e70 = NULL;
        KillSprite(DAT_007fdcd0);
        DAT_007fdcd0 = NULL;
        KillSprite(DAT_007fdd50);
        DAT_007fdd50 = NULL;
        KillSprite(DAT_007fdcd4);
        DAT_007fdcd4 = NULL;
        KillSprite(DAT_007fdd54);
        DAT_007fdd54 = NULL;
        KillSprite(DAT_007fdcd8);
        DAT_007fdcd8 = NULL;
        KillSprite(DAT_007fdd58);
        DAT_007fdd58 = NULL;
        KillSprite(DAT_007fdcdc);
        DAT_007fdcdc = NULL;
        KillSprite(DAT_007fdd5c);
        DAT_007fdd5c = NULL;
        KillSprite(DAT_007fdce0);
        DAT_007fdce0 = NULL;
        KillSprite(DAT_007fdd60);
        DAT_007fdd60 = NULL;
        KillSprite(DAT_00668e7c);
        DAT_00668e7c = NULL;
        KillSprite(DAT_00668e80);
        DAT_00668e80 = NULL;
        KillSprite(DAT_00668e74);
        DAT_00668e74 = NULL;
        KillSprite(DAT_00668e78);
        DAT_00668e78 = NULL;
        KillSprite(DAT_00668e84);
        DAT_00668e84 = NULL;
        KillSprite(DAT_00668e88);
        DAT_00668e88 = NULL;
        KillSprite(DAT_00668e8c);
        DAT_00668e8c = NULL;
        KillSprite(DAT_00668e90);
        DAT_00668e90 = NULL;
        KillSprite(DAT_00668e94);
        DAT_00668e94 = NULL;
        KillSprite(DAT_00668e98);
        DAT_00668e98 = NULL;
        KillSprite(DAT_00668ea0);
        DAT_00668ea0 = NULL;
    }
}

// FUNCTION: LEGOLAND 0x00474590
void FUN_00474590(void) {
    DAT_00668eb0 = 0;
    DAT_004bb0a0 = 1;
    DAT_004bb09c = 1;
    DAT_004bb098 = 1;
    DAT_004bb094 = 1;
}

// FUNCTION: LEGOLAND 0x004745c0
LEGO_EXPORT void Load_Interface_ThemeIcons(void) {
    if (DAT_00668eb4 != 0) {
        return;
    }
    DAT_00668eb4 = 1;
    // STRING: LEGOLAND 0x004bb464
    DAT_007fdcc0 = LoadSprite("legoland_themeON.lls", 4);
    // STRING: LEGOLAND 0x004bb44c
    DAT_007fdd40 = LoadSprite("legoland_themeOFF.lls", 4);
    // STRING: LEGOLAND 0x004bb438
    DAT_007fdcc8 = LoadSprite("castle_themeON.lls", 4);
    // STRING: LEGOLAND 0x004bb424
    DAT_007fdd48 = LoadSprite("castle_themeOFF.lls", 4);
    // STRING: LEGOLAND 0x004bb410
    DAT_007fdcc4 = LoadSprite("western_themeON.lls", 4);
    // STRING: LEGOLAND 0x004bb3f8
    DAT_007fdd44 = LoadSprite("western_themeOFF.lls", 4);
    // STRING: LEGOLAND 0x004bb3e0
    DAT_007fdccc = LoadSprite("adventurers_themeON.lls", 4);
    // STRING: LEGOLAND 0x004bb3c4
    DAT_007fdd4c = LoadSprite("adventurers_themeOFF.lls", 4);
}

// FUNCTION: LEGOLAND 0x00474670
LEGO_EXPORT void UnLoad_Interface_ThemeIcons(void) {
    unsigned int sprite;

    if (DAT_00668eb4 != 0) {
        sprite = DAT_00668eb4;
        DAT_00668eb4 = 0;
        if (sprite != 0) {
            if (DAT_007fdcc0 != 0) {
                KillSprite(DAT_007fdcc0);
                DAT_007fdcc0 = 0;
            }
            if (DAT_007fdd40 != 0) {
                KillSprite(DAT_007fdd40);
                DAT_007fdd40 = 0;
            }
            if (DAT_007fdcc8 != 0) {
                KillSprite(DAT_007fdcc8);
                DAT_007fdcc8 = 0;
            }
            if (DAT_007fdd48 != 0) {
                KillSprite(DAT_007fdd48);
                DAT_007fdd48 = 0;
            }
            if (DAT_007fdcc4 != 0) {
                KillSprite(DAT_007fdcc4);
                DAT_007fdcc4 = 0;
            }
            if (DAT_007fdd44 != 0) {
                KillSprite(DAT_007fdd44);
                DAT_007fdd44 = 0;
            }
            if (DAT_007fdccc != 0) {
                KillSprite(DAT_007fdccc);
                DAT_007fdccc = 0;
            }
            if (DAT_007fdd4c != 0) {
                KillSprite(DAT_007fdd4c);
                DAT_007fdd4c = 0;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00474750
void FUN_00474750(void) {
    if (DAT_004bb094 == 0) {
        DAT_004bb094 = 1;
        FUN_0046d680((struct IconNode *)DAT_00668eb0, DAT_007fdd40);
    } else if (DAT_004bb098 == 0) {
        DAT_004bb098 = 1;
        FUN_0046d680((struct IconNode *)DAT_00668eb0, DAT_007fdd48);
    } else if (DAT_004bb09c == 0) {
        DAT_004bb09c = 1;
        FUN_0046d680((struct IconNode *)DAT_00668eb0, DAT_007fdd44);
    } else if (DAT_004bb0a0 == 0) {
        DAT_004bb0a0 = 1;
        FUN_0046d680((struct IconNode *)DAT_00668eb0, DAT_007fdd4c);
    }
}

// FUNCTION: LEGOLAND 0x00474800
LEGO_EXPORT void UnLoad_Interface_Icons(void) {
    UnLoad_Interface_ControlIcons();
    FUN_0046fb40(0xd2);
    UnLoad_PopUpInfo();
}

// FUNCTION: LEGOLAND 0x00474820
unsigned char FUN_00474820(unsigned int dummy, unsigned char flags) {
    if ((flags & 2) != 0) {
        FUN_00473160();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00474830
unsigned char FUN_00474830(unsigned int a, unsigned int flags, unsigned int c, unsigned int d) {
    if ((flags & 2) != 0) {
        if (FUN_00473130() == 0) {
            if (DAT_00668954 != 0) {
                CheckWorkerOnMouseStatus(1);
            } else {
                return FUN_00475120(a, flags, c, d);
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00474880
void FUN_00474880(void) {
    DAT_006687bc = (unsigned int)FUN_00474820;
    DAT_006687c0 = (unsigned int)FUN_00474830;
}

// FUNCTION: LEGOLAND 0x004748a0
void FUN_004748a0(void *a) {
    if (DAT_00668eb8 == 0) {
        return;
    }
    if (FUN_0046b280() == 0) {
        if (a != NULL) {
            ((struct InterfaceObj *)DAT_00668eb8)->field_34 |= 0x2002;
        } else {
            ((struct InterfaceObj *)DAT_00668eb8)->field_34 &= 0xffffdffd;
        }
        return;
    }
    ((struct InterfaceObj *)DAT_00668eb8)->field_34 |= 0x2002;
    ((struct InterfaceObj *)DAT_00668eb8)->field_2c = (void *)FUN_00474f80;
    ((struct InterfaceObj *)DAT_00668eb8)->field_3c = 0x8fc;
    ((struct InterfaceObj *)DAT_00668eb8)->field_38 = GetString(0x8fc);
}

// FUNCTION: LEGOLAND 0x00474920
int FUN_00474920(void) {
    int i;

    for (i = 0; i < 4; i++) {
        if (((struct ProfileObj *)DAT_007fdd70[i])->flags & 0x400) {
            DAT_00668e20[i] = 0;
        } else {
            DAT_00668e20[i] = 1;
        }
    }
    return SaveGameWrite(DAT_00668e20, 0x10) != 0;
}

// FUNCTION: LEGOLAND 0x00474970
int FUN_00474970(void) {
    return SaveGameRead(DAT_00668e20, 0x10) != 0;
}

// FUNCTION: LEGOLAND 0x00474990
void FUN_00474990(void) {
    int i;

    for (i = 0; i < 4; i++) {
        if (DAT_00668e20[i] != 0) {
            ((struct ProfileObj *)DAT_007fdd70[i])->flags &= 0xfffffbff;
        } else {
            ((struct ProfileObj *)DAT_007fdd70[i])->flags |= 0x400;
        }
    }
}

// FUNCTION: LEGOLAND 0x004749d0
LEGO_EXPORT int InitGameInterface(int a) {
    struct PathControlElem *element;
    struct BuildObject *obj;
    struct IconNode *icon;
    struct SpriteIcon *bar;

    if (DAT_00668ebc == 0) {
        DAT_00668ebc = 1;
        // STRING: LEGOLAND 0x004b8a70
        LLIDB_FindElement("PATH CONTROL", (unsigned int *)&element, 0);
        obj = element->obj;
        DAT_007fd624 = obj;
        icon = InsertIcon((short)DAT_004bb04c[8], (short)DAT_004bb04c[9], 0x93, DAT_007fdd50);
        icon->field_3c = 0xffffffff;
        icon->field_38 = (unsigned int)obj->field_7c;
        icon->field_18 = 1;
        icon->field_1c = DAT_007fdcd0;
        icon->field_20p = DAT_007fdd50;
        icon->field_8 = obj;
        icon->field_28 = (void *)RenderGBarSpriteIcon;
        icon->field_2c = (void *)FUN_00474fc0;
        icon->field_34 = (icon->field_34 & 0xfffffdff) | 0x300a;

        icon = InsertIcon((short)DAT_004bb04c[10], (short)DAT_004bb04c[11], 0x93, DAT_007fdd54);
        icon->field_3c = 0x5a;
        icon->field_38 = (unsigned int)GetString(0x5a);
        icon->field_34 |= 0x6002;
        icon->field_2c = (void *)FUN_00475000;
        icon->field_18 = 2;
        icon->field_1c = DAT_007fdcd4;
        icon->field_20p = DAT_007fdd54;

        icon = InsertIcon((short)DAT_004bb04c[12], (short)DAT_004bb04c[13], 0x93, DAT_007fdd58);
        icon->field_3c = 0x5b;
        icon->field_38 = (unsigned int)GetString(0x5b);
        icon->field_34 |= 0x6002;
        icon->field_2c = (void *)FUN_00475040;
        icon->field_18 = 3;
        icon->field_1c = DAT_007fdcd8;
        icon->field_20p = DAT_007fdd58;

        icon = InsertIcon((short)DAT_004bb04c[14], (short)DAT_004bb04c[15], 0x93, DAT_007fdd5c);
        icon->field_3c = 0x5c;
        icon->field_38 = (unsigned int)GetString(0x5c);
        icon->field_34 |= 0x6002;
        icon->field_2c = (void *)FUN_00475080;
        icon->field_18 = 4;
        icon->field_1c = DAT_007fdcdc;
        icon->field_20p = DAT_007fdd5c;

        icon = InsertIcon((short)DAT_004bb04c[16], (short)DAT_004bb04c[17], 0x93, DAT_007fdd60);
        icon->field_3c = 0x5d;
        icon->field_38 = (unsigned int)GetString(0x5d);
        icon->field_34 |= 0x6002;
        icon->field_2c = (void *)FUN_00475120;
        icon->field_20p = DAT_007fdd60;

        icon = InsertIcon((short)DAT_004bb04c[0], (short)DAT_004bb04c[1], 0x9a, DAT_007fdd40);
        icon->field_3c = 0x5e;
        icon->field_38 = (unsigned int)GetString(0x5e);
        icon->field_34 |= 0x6002;
        icon->field_2c = (void *)FUN_004751a0;
        DAT_007fdd70[0] = (struct InterfaceProfileObj *)icon;

        icon = InsertIcon((short)DAT_004bb04c[2], (short)DAT_004bb04c[3], 0x9a, DAT_007fdd44);
        DAT_00668e3c = icon;
        icon->field_3c = 0x5f;
        icon->field_38 = (unsigned int)GetString(0x5f);
        icon->field_34 |= 0x6002;
        icon->field_2c = (void *)FUN_004754b0;
        DAT_007fdd70[1] = (struct InterfaceProfileObj *)icon;

        icon = InsertIcon((short)DAT_004bb04c[4], (short)DAT_004bb04c[5], 0x9a, DAT_007fdd48);
        icon->field_3c = 0x60;
        icon->field_38 = (unsigned int)GetString(0x60);
        icon->field_34 |= 0x6002;
        icon->field_2c = (void *)FUN_004753a0;
        DAT_007fdd70[2] = (struct InterfaceProfileObj *)icon;

        icon = InsertIcon((short)DAT_004bb04c[6], (short)DAT_004bb04c[7], 0x9a, DAT_007fdd4c);
        icon->field_3c = 0x61;
        icon->field_38 = (unsigned int)GetString(0x61);
        icon->field_34 |= 0x6002;
        icon->field_2c = (void *)FUN_004752a0;
        DAT_007fdd70[3] = (struct InterfaceProfileObj *)icon;

        // STRING: LEGOLAND 0x004bb48c
        bar = LoadSpriteIcon("Bar_Energy.lls", 4, 0x180, 6, 0x9a);
        bar->field_28 = (void *)RenderEnergyBar;
        bar->field_34 |= 0x400a;

        // STRING: LEGOLAND 0x004bb47c
        bar = LoadSpriteIcon("Bar_Coins.lls", 4, 0x1c, 6, 0x9a);
        bar->field_28 = (void *)RenderMoneyBar;
        bar->field_34 |= 0x400a;

        icon = InsertIcon(0x19e, 0x179, 0x9a, DAT_00668e94);
        icon->field_3c = 0x24e;
        icon->field_38 = (unsigned int)GetString(0x24e);
        icon->field_18p = DAT_00668e98;
        icon->field_34 |= 0x600a;
        icon->field_2c = (void *)FUN_00474f40;
        icon->field_28 = (void *)FUN_0046e040;
        DAT_00668e9c = icon;
        icon->field_34 |= 0x400;
        FUN_00491240((const char *)&DAT_0066861c);

        icon = InsertIcon(0x20a, 0x17a, 0x93, DAT_00668ea0);
        icon->field_20p = NULL;
        icon->field_1c = NULL;
        icon->field_3c = 0x24f;
        icon->field_38 = (unsigned int)GetString(0x24f);
        icon->field_2c = (void *)FUN_00474fa0;
        icon->field_34 |= 0x4008;
        icon->field_28 = (void *)FUN_00443e30;
        DAT_00668eb8 = (unsigned int)icon;
        if (a != 0 || FUN_0046b280() == 0) {
            FUN_0046b240(0);
            if (DAT_0080ffe5 == 2) {
                FUN_004748a0((void *)0);
            } else {
                FUN_004748a0((void *)1);
            }
        } else {
            FUN_0046b240(1);
        }
        InitPopUpInfo();
    }
    FUN_00474590();
    DAT_007fe114 = 0;
    DAT_007fe117 = 0;
    DAT_007fe116 = 0;
    DAT_007fe115 = 0;
    ResetMoveAWorkerStruct();
    DAT_007fdd80 = 2;
    DAT_007fdd8c = 0x86;
    lpConfig->field_20 = 0;
    lpConfig->field_10 = 0x280;
    DAT_007fdd84 = 1;
    DAT_007fdd88 = 0;
    DAT_004baff8 = 5;
    FUN_0046fb40(0xd2);
    FUN_00476180();
    if (DAT_00810140 != 0) {
        FUN_00474990();
    }
}

// FUNCTION: LEGOLAND 0x00474ed0
void FUN_00474ed0(void) {
    if (DAT_00668ebc != 0) {
        DAT_00668ebc = 0;
        RemoveIconGroup(0x93);
        DAT_00668eb8 = 0;
        RemoveIconGroup(0x9a);
        DAT_00668e9c = NULL;
        FUN_0046d590(0xd2);
        UnLoad_PopUpInfo();
        DAT_007fdd80 = 2;
        DAT_007fdd84 = 1;
        DAT_007fdd88 = 0;
        DAT_007fdd8c = 0x86;
    }
}

// FUNCTION: LEGOLAND 0x00474f40
unsigned char FUN_00474f40(void *context, unsigned int flags, const char *a, const char *b) {
    if (DAT_008119b4 != 1) {
        if (flags & 2) {
            EditMode = 0;
            FUN_00490600(1);
            FUN_004911c0((const char *)&DAT_0066861c, (const char *)&DAT_0066869c);
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00474f80
unsigned char FUN_00474f80(unsigned int a, unsigned int flags) {
    if ((flags & 2) != 0) {
        FUN_00459820(1);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00474fa0
unsigned char FUN_00474fa0(unsigned int a, unsigned char flags) {
    if (DAT_008119b4 != 1) {
        if ((flags & 2) != 0) {
            FUN_0046b700();
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00474fc0
unsigned char FUN_00474fc0(void *a, unsigned int flags) {
    if (DAT_008119b4 == 1) {
        return 1;
    }
    if (flags & 2) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        SetEditObject(*(struct EditObject **)((char *)a + 8));
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00475000
unsigned char FUN_00475000(unsigned int a, unsigned int flags) {
    if (DAT_008119b4 != 1) {
        if (flags & 2) {
            PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
            GamePad = (GamePad & 0xffff00ff) | ((GamePad & 0xff00) & 0xef00);
            EditMode = 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00475040
unsigned char FUN_00475040(unsigned int a, unsigned int flags) {
    if (DAT_008119b4 == 1) {
        return 1;
    }
    if ((flags & 2) == 0) {
        return 1;
    }
    PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
    GamePad &= ~0x400;
    EditMode = 2;
    return 1;
}

// FUNCTION: LEGOLAND 0x00475080
unsigned char FUN_00475080(unsigned int a, unsigned char flags) {
    if ((flags & 2) == 0) {
        return 1;
    }
    PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
    if (DAT_008119b4 != 1) {
        FUN_00498920();
        DAT_00667c60 = DAT_008119b4;
        DAT_008119b4 = 1;
        GamePad = GamePad & 0xffffebff;
        DAT_008119bc = 1;
        DAT_006687b0 = 4;
        EditMode = 0;
        return 1;
    }
    DAT_0080ff70 = 1;
    DAT_008119b4 = DAT_00667c60;
    DAT_00667c60 = 1;
    FUN_004562e0();
    return 1;
}

// FUNCTION: LEGOLAND 0x00475120
unsigned char FUN_00475120(unsigned int a, unsigned int flags, unsigned int c, unsigned int d) {
    if ((flags & 2) != 0 && DAT_008119b4 != 1) {
        if (DAT_00668954 == 0) {
            PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
            GamePad = (GamePad & 0xffff00ff) | ((GamePad & 0xff00) & 0xeb00);
            EditMode = 0;
            DAT_00668e38 = 1;
            DAT_008119b4 = 2;
            DAT_0080ff84 = 0xffffffff;
            DAT_0080ff88 = 5;
            FUN_00498920();
            DAT_006687b0 = 4;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004751a0
unsigned char FUN_004751a0(struct IconNode *param_1, unsigned char flags) {
    unsigned int saved_e34;
    unsigned int saved_ff8;
    int result;

    saved_e34 = DAT_00668e34;
    saved_ff8 = DAT_004baff8;
    if (DAT_008119b4 == 1 || (flags & 2) == 0) {
        return 1;
    }
    GamePad = GamePad & 0xffffebff;
    EditMode = 0;
    PlayInstanceOfSample(PTR_004b9314, 0, 1, 0);
    if (DAT_004baff8 != 0) {
        DAT_004baff8 = 0;
        DAT_00668e34 = 0;
        result = TestMenu(DAT_004bafa8);
        if (result == 1) {
            FUN_00474750();
            DAT_00668eb0 = (unsigned int)param_1;
            FUN_0046d680(param_1, DAT_007fdcc0);
            DAT_004bb094 = 0;
            return 1;
        }
        DAT_004baff8 = saved_ff8;
        if (saved_ff8 != 5) {
            DAT_00668e34 = saved_e34;
            TestMenu(&DAT_004bafa8[saved_ff8 * 5]);
            return 1;
        }
    } else {
        FUN_00474750();
        DAT_004bb094 = 1;
        DAT_004baff8 = 5;
        DAT_007fdd80 = 1;
        DAT_007fdd84 = 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004752a0
unsigned char FUN_004752a0(struct IconNode *param_1, unsigned char flags) {
    unsigned int saved_e34;
    int result;

    saved_e34 = DAT_00668e34;
    if (DAT_008119b4 != 1 && (flags & 2) != 0) {
        GamePad = GamePad & 0xffffebff;
        EditMode = 0;
        PlayInstanceOfSample(PTR_004b9314, 0, 1, 0);
        if (DAT_004baff8 != 3) {
            DAT_004baff8 = 3;
            DAT_00668e34 = 0;
            result = TestMenu(&DAT_004bafa8[15]);
            if (result == 1) {
                FUN_00474750();
                DAT_00668eb0 = (unsigned int)param_1;
                FUN_0046d680(param_1, DAT_007fdccc);
                DAT_004bb0a0 = 0;
                return 1;
            }
            DAT_00668e34 = saved_e34;
            TestMenu(&DAT_004bafa8[DAT_004baff8 * 5]);
            return 1;
        }
        FUN_00474750();
        DAT_004bb0a0 = 1;
        DAT_004baff8 = 5;
        DAT_007fdd80 = 1;
        DAT_007fdd84 = 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004753a0
unsigned char FUN_004753a0(struct IconNode *param_1, unsigned char flags) {
    unsigned int saved_e34;
    int saved_ff8;
    int result;

    saved_e34 = DAT_00668e34;
    saved_ff8 = DAT_004baff8;
    DAT_004baff8 = saved_ff8;
    if (DAT_008119b4 != 1 && (flags & 2) != 0) {
        GamePad = GamePad & 0xffffebff;
        EditMode = 0;
        PlayInstanceOfSample(PTR_004b9314, 0, 1, 0);
        if (DAT_004baff8 != 2) {
            DAT_004baff8 = 2;
            DAT_00668e34 = 0;
            result = TestMenu(&DAT_004bafa8[10]);
            if (result == 1) {
                FUN_00474750();
                DAT_00668eb0 = (unsigned int)param_1;
                FUN_0046d680(param_1, DAT_007fdcc8);
                DAT_004bb098 = 0;
                return 1;
            }
            DAT_004baff8 = saved_ff8;
            if (saved_ff8 != 5) {
                DAT_00668e34 = saved_e34;
                TestMenu(&DAT_004bafa8[saved_ff8 * 5]);
                return 1;
            }
        } else {
            FUN_00474750();
            DAT_004bb098 = 1;
            DAT_004baff8 = 5;
            DAT_007fdd80 = 1;
            DAT_007fdd84 = 1;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004754b0
unsigned char FUN_004754b0(struct IconNode *param_1, unsigned char flags) {
    unsigned int saved_e34;
    int saved_ff8;
    int result;

    saved_e34 = DAT_00668e34;
    saved_ff8 = DAT_004baff8;
    DAT_004baff8 = saved_ff8;
    if (DAT_008119b4 != 1 && (flags & 2) != 0) {
        GamePad = GamePad & 0xffffebff;
        EditMode = 0;
        PlayInstanceOfSample(PTR_004b9314, 0, 1, 0);
        if (DAT_004baff8 != 1) {
            DAT_004baff8 = 1;
            DAT_00668e34 = 0;
            result = TestMenu(&DAT_004bafa8[5]);
            if (result == 1) {
                FUN_00474750();
                DAT_00668eb0 = (unsigned int)param_1;
                FUN_0046d680(param_1, DAT_007fdcc4);
                DAT_004bb09c = 0;
                return 1;
            }
            DAT_004baff8 = saved_ff8;
            if (saved_ff8 != 5) {
                DAT_00668e34 = saved_e34;
                TestMenu(&DAT_004bafa8[saved_ff8 * 5]);
                return 1;
            }
        } else {
            FUN_00474750();
            DAT_004bb09c = 1;
            DAT_004baff8 = 5;
            DAT_007fdd80 = 1;
            DAT_007fdd84 = 1;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004755c0
void FUN_004755c0(void *data) {
    struct InterfaceListNode *node;
    struct InterfaceListNode *current;
    struct InterfaceListNode *prev;
    int current_cost;

    current = DAT_00668e40;
    prev = NULL;
    node = (struct InterfaceListNode *)malloc(sizeof(struct InterfaceListNode));
    node->data = data;
    node->flag = 1;
    node->next = NULL;
    while (current != NULL) {
        current_cost = GetObjCost(current->data);
        if (current_cost > GetObjCost(node->data)) {
            break;
        }
        prev = current;
        current = current->next;
    }
    if (prev != NULL) {
        prev->next = node;
        node->next = current;
        return;
    }
    DAT_00668e40 = node;
    node->next = current;
}

// FUNCTION: LEGOLAND 0x00475630
LEGO_EXPORT void InsertChildIntoList(struct BuildObject *param_1) {
    struct InterfaceListNode *node;
    struct InterfaceListNode *current;
    struct InterfaceListNode *prev;

    current = DAT_00668e40;
    node = (struct InterfaceListNode *)malloc(sizeof(struct InterfaceListNode));
    node->data = param_1;
    node->flag = 0;
    node->next = NULL;
    while (current != NULL) {
        if (((struct BuildObject *)current->data)->field_c4 == param_1->field_58) {
            prev = current;
            current = current->next;
            while (current != NULL) {
                if (((struct BuildObject *)node->data)->field_58 != ((struct BuildObject *)current->data)->field_58) {
                    break;
                }
                if (GetObjCost((struct CostInfo *)node->data) <= GetObjCost((struct CostInfo *)current->data)) {
                    break;
                }
                prev = current;
                current = current->next;
            }
            prev->next = node;
            node->next = current;
            return;
        }
        current = current->next;
    }
    if (DAT_00668e34 != 0) {
        FUN_004755c0(param_1);
    }
    free(node);
}

// FUNCTION: LEGOLAND 0x004756e0
LEGO_EXPORT void DelObjectList(void) {
    struct InterfaceListNode *current;
    struct InterfaceListNode *next;

    current = DAT_00668e40;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    DAT_00668e40 = NULL;
}

// FUNCTION: LEGOLAND 0x00475710
LEGO_EXPORT unsigned int TestMenu(unsigned int *entry) {
    return ObjectLinkedList(entry);
}

// FUNCTION: LEGOLAND 0x00475720
LEGO_EXPORT unsigned int ObjectLinkedList(unsigned int *entry) {
    void *build_elem;
    void *theme_elem;
    void *param_elem;
    void *menu_elem;
    struct LLDBElem *elem;
    struct BuildObject *obj;
    char *menu_name;
    int count;
    int i;
    int matched;

    matched = 1;
    DelObjectList();
    if (LLIDB_FindElement("BUILD MENU", (unsigned int *)&build_elem, 0) != 0) {
        exit(1);
    }
    if (LLIDB_FindElement("COMMON THEME", (unsigned int *)&theme_elem, 0) != 0) {
        exit(1);
    }
    if (LLIDB_FindElement((const char *)entry, (unsigned int *)&param_elem, 0) != 0) {
        exit(1);
    }
    count = LLIDB_GetCount();
    menu_name = DAT_004baffc[0];
    do {
        if (LLIDB_FindElement(menu_name, (unsigned int *)&menu_elem, 0) != 0) {
            exit(1);
        }
        for (i = 0; i < count; i++) {
            LLIDB_GetElement(i, (struct Element **)&elem);
            if ((elem->flags & 0x13) == 0x13) {
                obj = elem->obj;
                if (obj->field_58 == build_elem) {
                    if ((obj->field_5c == param_elem && obj->field_60 == menu_elem) ||
                        (obj->field_5c == theme_elem && DAT_004baff8 == 0 && obj->field_60 == menu_elem)) {
                        FUN_004755c0(obj);
                        matched++;
                    }
                }
            }
        }
        menu_name += 0x14;
    } while (menu_name < (char *)DAT_004bb04c);
    for (i = 0; i < count; i++) {
        LLIDB_GetElement(i, (struct Element **)&elem);
        if ((elem->flags & 0x13) == 0x13) {
            obj = elem->obj;
            if (obj->field_5c == param_elem && obj->field_58 != build_elem && obj->field_58 != NULL) {
                InsertChildIntoList(obj);
            }
        }
    }
    DAT_00668e64 = (unsigned char)DAT_004baff8;
    if (matched == 0) {
        return 0;
    }
    MakeUpObjectList(0xd2, 3, 0x21, 0x154);
    return 1;
}

// FUNCTION: LEGOLAND 0x004758c0
LEGO_EXPORT void UpdateMenu(void) {
    if (DAT_004baff8 == 5) {
        return;
    }
    TestMenu(&DAT_004bafa8[DAT_004baff8 * 5]);
}

// FUNCTION: LEGOLAND 0x004758e0
LEGO_EXPORT void RedrawObjectList(struct InterfacePanel *panel, int param_2, int delta) {
    int top;

    if (delta == 0) {
        MoveIcons(0xffff, panel->group, 0, 0);
        return;
    }
    top = panel->field_10;
    if (top + delta > panel->field_20) {
        delta = panel->field_20 - top;
    } else if (panel->field_18 + delta < panel->field_28) {
        delta = panel->field_28 - panel->field_18;
    }
    panel->field_10 = top + delta;
    panel->field_18 = panel->field_18 + delta;
    MoveIcons(0xffff, panel->group, (short)param_2, (short)delta);
}

// FUNCTION: LEGOLAND 0x00475960
LEGO_EXPORT int MakeUpObjectList(int param_1, int param_2, int param_3, int param_4) {
    struct InterfaceListNode *node;
    struct InterfaceListNode *ctx;
    struct InterfacePanel *panel;
    struct IconNode *icon;
    struct IconNode *last_icon;
    int panel_arg;
    int x;
    int y;

    node = DAT_00668e40;
    FUN_0046fb40(param_1);
    panel = (struct InterfacePanel *)malloc(sizeof(struct InterfacePanel));
    if (panel == NULL) {
        return 0;
    }
    panel_arg = param_2;
    if (DAT_007fdd84 == 1) {
        panel_arg = 0xffffff86;
        DAT_007fdd80 = 0;
    }
    icon = SetupInterfacePanelIcons((unsigned int)panel, panel_arg, param_3, 1, param_4, param_1);
    panel->icon = icon;
    x = icon->field_c;
    panel->field_1c = x;
    panel->field_c = x;
    y = icon->field_e;
    panel->field_20 = y;
    panel->field_10 = y;
    panel->field_24 = icon->field_10 + icon->field_c;
    panel->field_28 = icon->field_12 + icon->field_e;
    panel->field_4 = 1;
    panel->group = (short)param_1;
    SetNewGroup_Callbacks(0, (void *)RenderBuildObjectIcon, (void *)FUN_00470000);
    ctx = (struct InterfaceListNode *)param_2;
    while (1) {
        if (node == NULL) {
            goto finish;
        }
        if (node->flag == 0) {
            if ((((struct BuildObject *)ctx->data)->field_c4->flags & 8) == 0) {
                ListChildrenBar(ctx, param_1, (short)x, (short)(y - 10));
                y = y + 0x1a;
                while (ctx = node, node->flag == 0) {
                    node = node->next;
                    if (ctx->next == NULL) {
                        goto finish;
                    }
                }
            } else {
                CloseChildrenBar(ctx, param_1, (short)x, (short)(y - 10));
                y = y + 0x10;
                do {
                    ctx = node;
                    if (node->flag != 0) {
                        break;
                    }
                    last_icon = AddGBarClassIcon((unsigned int)panel, (struct InfoSource *)node->data, x, y, param_1, 1);
                    last_icon->field_20b = 1;
                    node = node->next;
                    y = y + 0x38;
                } while (node != NULL);
                y = y + 10;
                last_icon->field_20b = 2;
            }
        } else {
            AddGBarClassIcon((unsigned int)panel, (struct InfoSource *)node->data, x, y, param_1, 1);
            y = y + 0x42;
            ctx = node;
            node = node->next;
        }
    }
finish:
    AddFullScreenIcon((void *)(param_2 + 6));
    panel->field_14 = x;
    panel->field_18 = y;
    if (y < panel->icon->field_12 + panel->icon->field_e) {
        icon = FindIcon((unsigned short)(param_2 + 4));
        if (icon != NULL) {
            icon->field_e = (short)param_4 - 0x1e + (short)param_3;
            icon->field_34 |= 0x400;
        }
        icon = FindIcon((unsigned short)(param_2 + 3));
        if (icon != NULL) {
            icon->field_34 |= 0x400;
        }
        DAT_007fdd84 = 0;
        DAT_00668e44[DAT_00668e64 & 0xff] = 0;
        return 1;
    }
    RedrawObjectList(panel, 0, DAT_00668e44[DAT_00668e64 & 0xff]);
    DAT_007fdd84 = 0;
    return 1;
}

// FUNCTION: LEGOLAND 0x00475bb0
LEGO_EXPORT void ListChildrenBar(void *node, int group, short x, short y) {
    struct SpriteIcon *icon;

    // STRING: LEGOLAND 0x004bb4a8
    icon = LoadSpriteIcon("ListChildrenBar.lls", 4, x, y, group);
    icon->field_3c = 100;
    icon->field_38 = GetString(100);
    icon->field_34 |= 0x2002;
    icon->event_handler = (unsigned char (*)(unsigned int, unsigned int))FUN_00475c50;
    icon->field_18 = (struct Sprite *)node;
}

// FUNCTION: LEGOLAND 0x00475c00
LEGO_EXPORT void CloseChildrenBar(void *node, int group, short x, short y) {
    struct SpriteIcon *icon;

    // STRING: LEGOLAND 0x004bb4bc
    icon = LoadSpriteIcon("CloseChildrenBar.lls", 4, x, y, group);
    icon->field_3c = 0x65;
    icon->field_38 = GetString(0x65);
    icon->field_34 |= 0x2002;
    icon->event_handler = (unsigned char (*)(unsigned int, unsigned int))FUN_00475c90;
    icon->field_18 = (struct Sprite *)node;
}

// FUNCTION: LEGOLAND 0x00475c50
char FUN_00475c50(int param_1, unsigned char param_2) {
    if (param_2 & 2) {
        DAT_004bdd00 = 0x100;
        *(unsigned int *)(*(int *)(*(int *)(*(int *)(param_1 + 0x18) + 4) + 0xc4) + 8) |= 8;
        MakeUpObjectList(0xd2, 3, 0x21, 0x154);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00475c90
char FUN_00475c90(int param_1, unsigned char param_2) {
    if (param_2 & 2) {
        DAT_004bdd00 = 0x100;
        *(unsigned int *)(*(int *)(*(int *)(*(int *)(param_1 + 0x18) + 4) + 0xc4) + 8) &= ~8;
        MakeUpObjectList(0xd2, 3, 0x21, 0x154);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00475cd0
LEGO_EXPORT int RAndDLinkedList(unsigned int *entry) {
    struct LLDBElem *elem;
    void *build_elem;
    void *param_elem;
    void *menu_elem;
    void *theme_elem;
    struct BuildObject *obj;
    unsigned int flags;
    int count;
    int menu_index;
    int i;

    DelObjectList();
    if (LLIDB_FindElement("BUILD MENU", (unsigned int *)&build_elem, 0) != 0) {
        exit(1);
    }
    if (LLIDB_FindElement("COMMON THEME", (unsigned int *)&theme_elem, 0) != 0) {
        exit(1);
    }
    if (LLIDB_FindElement((const char *)entry, (unsigned int *)&param_elem, 0) != 0) {
        exit(1);
    }
    count = LLIDB_GetCount();
    menu_index = 0;
    do {
        if (LLIDB_FindElement(DAT_004baffc[menu_index], (unsigned int *)&menu_elem, 0) != 0) {
            exit(1);
        }
        for (i = 0; i < count; i++) {
            LLIDB_GetElement(i, (struct Element **)&elem);
            if ((elem->flags & 0x10011) == 0x10011) {
                obj = elem->obj;
                if (obj->field_58 == build_elem) {
                    if (obj->field_5c == param_elem && obj->field_60 == menu_elem) {
                        flags = obj->field_1c;
                    } else if (obj->field_5c == theme_elem && DAT_004baff8 == 0 && menu_index == 0) {
                        flags = obj->field_1c;
                    } else {
                        continue;
                    }
                    if ((flags & 0xc000000) == 0) {
                        obj->field_1c = flags | 0x4000000;
                    }
                    FUN_004755c0(obj);
                }
            }
        }
        menu_index++;
    } while (menu_index < 4);
    for (i = 0; i < count; i++) {
        LLIDB_GetElement(i, (struct Element **)&elem);
        if ((elem->flags & 0x10011) == 0x10011) {
            obj = elem->obj;
            if (obj->field_58 != build_elem && obj->field_58 != NULL && obj->field_5c == param_elem) {
                if ((obj->field_1c & 0xc000000) == 0) {
                    obj->field_1c |= 0x4000000;
                }
                InsertChildIntoList(obj);
            }
        }
    }
    DAT_00668e64 = (unsigned char)DAT_004baff8 + 4;
    return 1;
}

// FUNCTION: LEGOLAND 0x00475e90
LEGO_EXPORT void DisableSidePanelIcons(void) {
    struct IconNode *node;

    node = (struct IconNode *)DAT_006687c8;
    while (node != NULL) {
        if (node->field_14 == 0xd2 || node->field_14 == 0xd5 || node->field_14 == 0xd6 || node->field_14 == 0xd7) {
            node->field_34 |= 0x400;
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00475ed0
LEGO_EXPORT void EnableSidePanelIcons(void) {
    struct IconNode *node;

    node = (struct IconNode *)DAT_006687c8;
    while (node != NULL) {
        if (node->field_14 == 0xd2 || node->field_14 == 0xd5 || node->field_14 == 0xd6 || node->field_14 == 0xd7) {
            node->field_34 &= 0xfffffbff;
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00475f10
void FUN_00475f10(void) {
    if (DAT_004baff8 != 5) {
        TestMenu(&DAT_004bafa8[DAT_004baff8 * 5]);
        DAT_007fdd8c = 3;
    }
}

// FUNCTION: LEGOLAND 0x00475f40
void FUN_00475f40(void) {
    struct BuildObject *obj;
    struct ObjectClassInfo *info;
    struct ObjectClassInfo *match;
    char **pair;

    obj = (struct BuildObject *)DAT_008119b8;
    match = NULL;
    info = obj->field_c4;
    if ((obj->field_1c & 0x2000000) != 0) {
        match = info;
    } else {
        pair = &DAT_004bb0a4[0].elem_name;
        do {
            if (_stricmp(pair[-1], info->name) == 0) {
                if (*pair != NULL) {
                    match = (struct ObjectClassInfo *)ElemID(*pair);
                } else {
                    match = info;
                }
            }
            pair += 2;
        } while (pair < (char **)&DAT_004bb18c[1]);
    }
    if (match != NULL && (match->flags & 2) != 0) {
        SetEditObject(match->field_c);
        return;
    }
    EditMode = 0;
    DAT_00667108 = 1;
    GamePad = GamePad & 0xffffebff;
}

// FUNCTION: LEGOLAND 0x00475fe0
void FUN_00475fe0(int index, unsigned int value) {
    if (index < 0 || index >= 4) {
        return;
    }
    DAT_004bb18c[index] = value;
}

// FUNCTION: LEGOLAND 0x00476000
void FUN_00476000(void) {
    int i;

    for (i = 0; i < 4; i++) {
        FUN_00475fe0(i, 0);
    }
}

// FUNCTION: LEGOLAND 0x00476020
void FUN_00476020(void) {}

// FUNCTION: LEGOLAND 0x00476030
void FUN_00476030(int index, unsigned int value) {
    if (index >= 0 && index < 9) {
        DAT_007fdd00[index] = value;
    }
}

// FUNCTION: LEGOLAND 0x00476050
void FUN_00476050(void) {
    int i;

    i = 0;
    while (i < 9) {
        FUN_00476030(i, 0);
        i++;
    }
}

// FUNCTION: LEGOLAND 0x00476070
void FUN_00476070(int mask, unsigned int value) {
    int i;
    int bit;

    i = 0;
    bit = 1;
    while (1) {
        if (mask & bit) {
            FUN_00476030(i, value);
        }
        i++;
        bit <<= 1;
        if (i >= 9) {
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x004760a0
void FUN_004760a0(void) {
    int *coords;
    int played;
    int i;

    played = 0;
    FUN_00476020();
    if (DAT_008119b4 == 3) {
        coords = DAT_004bb04c;
        i = 0;
        do {
            if (DAT_007fdd00[i] != 0) {
                if (GetBlink() != 0) {
                    PrintSprite((&DAT_007fdcc0)[i], coords[0], coords[1], 0, 0);
                    played = 1;
                } else {
                    PrintSprite((&DAT_007fdd40)[i], coords[0], coords[1], 0, 0);
                }
            }
            coords = coords + 2;
            i = i + 1;
        } while ((int)coords < (int)&DAT_004bb094);
        if (played != 0 && DAT_00668ec0 == 0) {
            PlayInstanceOfSample(PTR_004b9338, 0, 1, 0);
        }
    }
    DAT_00668ec0 = played;
}

// FUNCTION: LEGOLAND 0x00476140
void FUN_00476140(int index, int value) {
    struct ProfileObj *obj;

    obj = (struct ProfileObj *)DAT_007fdd70[index];
    if (obj != NULL) {
        if (value != 0) {
            obj->flags &= 0xfffffbff;
            DAT_0080ffa0.field_30[index] = 1;
            UpDateCurrentProfile();
        } else {
            obj->flags |= 0x400;
        }
    }
}

// FUNCTION: LEGOLAND 0x00476180
void FUN_00476180(void) {
    unsigned char *flags;
    struct ProfileObj **items;
    unsigned int counter;

    flags = DAT_0080ffa0.field_30;
    items = (struct ProfileObj **)DAT_007fdd70;
    counter = 4;
    while (counter != 0) {
        if (*flags != 0) {
            (*items)->flags &= 0xfffffbff;
        } else {
            (*items)->flags |= 0x400;
        }
        flags++;
        items++;
        counter--;
    }
}

// FUNCTION: LEGOLAND 0x004761c0
LEGO_EXPORT void InitRAndDCheckBox(void) {}

// FUNCTION: LEGOLAND 0x004761d0
LEGO_EXPORT void Unload_RAndDCheckBox(void) {}

// FUNCTION: LEGOLAND 0x004761f0
LEGO_EXPORT void RenderRAndDCheckBox(void) {}

// FUNCTION: LEGOLAND 0x00476200
LEGO_EXPORT void CloseCheckBoxRAndD(void) {}

// FUNCTION: LEGOLAND 0x00476210
LEGO_EXPORT void DisableRAndDIcons(void) {}

// FUNCTION: LEGOLAND 0x00476220
unsigned char FUN_00476220(void) {
    return 1;
}

// FUNCTION: LEGOLAND 0x00476230
unsigned char FUN_00476230(void) {
    return 1;
}

// FUNCTION: LEGOLAND 0x00476240
unsigned char FUN_00476240(void) {
    return 1;
}

// FUNCTION: LEGOLAND 0x00476250
void FUN_00476250(void) {
    struct InterfaceResearchNode *node;
    int count;
    int len;

    count = 0;
    for (node = DAT_00668ed8; node != NULL; node = node->next) {
        count = count + 1;
    }
    SaveGameWrite(&count, 4);
    node = DAT_00668ed8;
    while (node != NULL) {
        len = strlen(((struct BuildObject *)node->data)->field_c4->name);
        SaveGameWrite(&len, 4);
        SaveGameWrite(((struct BuildObject *)node->data)->field_c4->name, len);
        SaveGameWrite(&node->field_8, 4);
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x004762f0
void FUN_004762f0(void) {
    struct InterfaceResearchNode *node;
    struct InterfaceResearchNode *prev;
    struct BuildObject *obj;
    char buf[512];
    int count;
    int len;

    DAT_00668ed8 = NULL;
    SaveGameRead(&count, 4);
    prev = NULL;
    if (count != 0) {
        do {
            count = count - 1;
            if (prev != NULL) {
                node = (struct InterfaceResearchNode *)malloc(sizeof(struct InterfaceResearchNode));
                prev->next = node;
            } else {
                node = (struct InterfaceResearchNode *)malloc(sizeof(struct InterfaceResearchNode));
                DAT_00668ed8 = node;
            }
            SaveGameRead(&len, 4);
            SaveGameRead(buf, len);
            buf[len] = 0;
            obj = ((struct LLDBElem *)ElemID(buf))->obj;
            node->data = obj;
            obj->field_1c = obj->field_1c & 0xfbffffff;
            ((struct BuildObject *)node->data)->field_1c = ((struct BuildObject *)node->data)->field_1c | 0x8000000;
            SaveGameRead(&node->field_8, 4);
            prev = node;
        } while (count != 0);
        prev->next = NULL;
    }
}

// FUNCTION: LEGOLAND 0x004763d0
LEGO_EXPORT void CleanUpReseachList(void) {
    struct InterfaceResearchNode *node;
    struct InterfaceResearchNode *current;
    struct InterfaceResearchNode *prev;

    node = DAT_00668ed8;
    if (DAT_00668ed8->field_8 == 0) {
        DAT_00668ed8 = DAT_00668ed8->next;
        free(node);
        return;
    }
    node = DAT_00668ed8->next;
    prev = DAT_00668ed8;
    if (DAT_00668ed8->next != NULL) {
        while (current = node, current->field_8 != 0) {
            prev = current;
            node = current->next;
            if (node == NULL) {
                return;
            }
        }
        prev->next = current->next;
        free(current);
    }
}

// FUNCTION: LEGOLAND 0x00476420
LEGO_EXPORT void DeleteReseachList(void) {
    struct InterfaceResearchNode *current;
    struct InterfaceResearchNode *next;

    current = DAT_00668ed8;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    DAT_00668ed8 = NULL;
}

// FUNCTION: LEGOLAND 0x00476460
struct MovieHandle *FUN_00476460(const char *filename) {
    struct AviFileInfo file_info;
    struct AviStreamInfo stream_info;
    struct MovieHandle *handle;
    void *file;
    void *audio_stream;
    void *video_stream;
    void *stream;
    unsigned int rate;
    int width;
    int height;
    unsigned int length;
    int i;

    video_stream = NULL;
    audio_stream = NULL;
    if (DAT_00668f98 == 0) {
        AVIFileInit();
    }
    rate = 0;
    if (AVIFileOpenA(&file, filename, 0, 0) == 0) {
        file_info.streams = 0;
        AVIFileInfoA(file, &file_info, 0x6c);
        if (file_info.streams > 0) {
            length = 0;
            width = 0;
            height = 0;
            i = 0;
            do {
                if (AVIFileGetStream(file, &stream, 0, i) != 0) {
                    break;
                }
                if (AVIStreamInfoA(stream, &stream_info, 0x8c) == 0) {
                    if (stream_info.type == 0x73646976) {
                        AVIStreamAddRef(stream);
                        rate = stream_info.rate / stream_info.scale;
                        width = stream_info.frame_right - stream_info.frame_left;
                        height = stream_info.frame_bottom - stream_info.frame_top;
                        video_stream = stream;
                        length = stream_info.length;
                    } else if (stream_info.type == 0x73647561) {
                        DAT_00668fa4 = 0x16;
                        AVIStreamAddRef(stream);
                        audio_stream = stream;
                    }
                }
                i++;
            } while (i < file_info.streams);
            if (video_stream == NULL) {
                if (audio_stream != NULL) {
                    AVIStreamRelease(audio_stream);
                }
            } else {
                handle = (struct MovieHandle *)malloc(sizeof(struct MovieHandle));
                if (handle != NULL) {
                    handle->field_0 = length;
                    handle->field_4 = rate;
                    handle->field_8 = width;
                    handle->field_c = height;
                    handle->frame = NULL;
                    handle->file = file;
                    handle->audio_stream = audio_stream;
                    handle->video_stream = video_stream;
                    DAT_00668f98 = DAT_00668f98 + 1;
                    return handle;
                }
                AVIStreamRelease(video_stream);
                if (audio_stream != NULL) {
                    AVIStreamRelease(audio_stream);
                }
            }
        }
        AVIFileRelease(file);
    }
    if (DAT_00668f98 == 0) {
        AVIFileExit();
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00476630
void FUN_00476630(struct MovieHandle *h) {
    if (h->frame != NULL) {
        AVIStreamGetFrameClose(h->frame);
    }
    if (h->video_stream != NULL) {
        AVIStreamRelease(h->video_stream);
    }
    if (h->audio_stream != NULL) {
        AVIStreamRelease(h->audio_stream);
    }
    free(h);
    DAT_00668f98 = DAT_00668f98 - 1;
    if (DAT_00668f98 == 0) {
        AVIFileExit();
    }
}

// FUNCTION: LEGOLAND 0x00476680
int FUN_00476680(void) {
    LARGE_INTEGER freq;
    LARGE_INTEGER count;
    unsigned int state;

    state = DAT_00668fac;
    if (state == 0) {
        if (QueryPerformanceFrequency(&freq) == 0) {
            DAT_00668fac = state = 1;
        } else {
            DAT_00668fac = state = 2;
            DAT_007fdca8 = DAT_004ab528 / (float)freq.QuadPart;
        }
    }
    if (state == 2) {
        QueryPerformanceCounter(&count);
        return FUN_00458930((float)count.QuadPart * DAT_007fdca8);
    }
    return GetTickCount();
}

// FUNCTION: LEGOLAND 0x004766f0
int FUN_004766f0(struct MovieHandle *handle, int param_2) {
    void *frame;
    int started;
    int frame_index;
    unsigned int target;
    unsigned int prev;

    started = 0;
    frame_index = -1;
    target = 0;
    if (handle == NULL) {
        return 0;
    }
    if (handle->video_stream == NULL) {
        return 0;
    }
    FUN_00476910(handle);
    DAT_004bb4e0.biBitCount = 0x10;
    DAT_004bb4e0.biWidth = handle->field_8;
    DAT_004bb4e0.biHeight = handle->field_c;
    DAT_004bb4e0.biSizeImage = handle->field_c * handle->field_8 * 2;
    handle->frame = AVIStreamGetFrameOpen(handle->video_stream, &DAT_004bb4e0);
    prev = 0;
    if ((int)handle->field_0 > 0) {
        do {
            if (param_2 != 0) {
                if (ProcessSystemEvents() == 0) {
                    break;
                }
                ReadGameButtons();
                if ((DAT_00813ac4 & 1) != 0) {
                    break;
                }
                if ((DAT_007fdda0[0x34] & 7) != 0) {
                    DAT_00668fb0 = 1;
                    break;
                }
                if ((DAT_007fdda0[0x39] & 0x80) != 0) {
                    break;
                }
            } else {
                ProcessSystemEvents();
                if (((DAT_007fdda0[0x9d] | DAT_007fdda0[0x1d]) & 0x80) != 0 && (DAT_007fdda0[0x10] & 0x80) != 0) {
                    break;
                }
            }
            if (frame_index == -1) {
                frame = AVIStreamGetFrame(handle->frame, target);
            }
            if (frame == NULL) {
                handle->frame = NULL;
                return 0;
            }
            PushRenderingStatusAndLockVideoSurface();
            FUN_00465850(frame);
            PopRenderingStatus();
            if (started == 0) {
                if (param_2 != 0) {
                    FUN_00476bf0(handle);
                }
                started = FUN_00476680();
            }
            RenderingComplete();
            frame_index = FUN_00476680();
            if ((unsigned int)((frame_index - started) * handle->field_4) / 1000 == target) {
                frame_index = target + 1;
                if (frame_index < (int)handle->field_0) {
                    frame = AVIStreamGetFrame(handle->frame, frame_index);
                }
            } else {
                frame_index = -1;
            }
            while (target == prev) {
                target = (unsigned int)((FUN_00476680() - started) * handle->field_4) / 1000;
            }
            if (param_2 != 0) {
                FUN_00476d20(target, prev);
            }
            prev = target;
        } while ((int)target < (int)handle->field_0);
    }
    FUN_00476c90();
    do {
        ProcessSystemEvents();
        ReadGameButtons();
    } while ((DAT_00813ac4 & 6) != 0);
    AVIStreamGetFrameClose(handle->frame);
    handle->frame = NULL;
    return 1;
}

// FUNCTION: LEGOLAND 0x00476910
int FUN_00476910(struct MovieHandle *handle) {
    struct AviStreamInfo info;
    WAVEFORMATEX *fmt;
    unsigned int *p;
    int fmt_size;
    int i;

    if (handle->audio_stream == NULL) {
        return 0;
    }
    if (AVIStreamInfoA(handle->audio_stream, &info, 0x8c) != 0) {
        return 0;
    }
    DAT_00668f64 = info.sample_size;
    AVIStreamReadFormat(handle->audio_stream, 0, 0, &fmt_size);
    fmt = (WAVEFORMATEX *)malloc(fmt_size);
    if (fmt == NULL) {
        return 0;
    }
    AVIStreamReadFormat(handle->audio_stream, 0, fmt, &fmt_size);
    DAT_00668f70.wFormatTag = 1;
    DAT_00668f70.nChannels = fmt->nChannels;
    DAT_00668f70.nSamplesPerSec = fmt->nSamplesPerSec;
    DAT_00668f70.nBlockAlign = fmt->nChannels << 1;
    DAT_00668f70.nAvgBytesPerSec = (DAT_00668f70.nBlockAlign & 0xffff) * fmt->nSamplesPerSec;
    DAT_00668f70.wBitsPerSample = 0x10;
    DAT_00668f70.cbSize = 0;
    if (fmt->wFormatTag != 2 && fmt->wFormatTag != 0x11) {
        DAT_00668ee0 = 0;
        DAT_00668f9c = 0;
        DAT_00668f50 = DAT_00668fa4;
        DAT_00668f3c = fmt->nAvgBytesPerSec / handle->field_4;
        DAT_00668f90 = fmt->nSamplesPerSec / handle->field_4;
        DAT_00668f4c = fmt->wBitsPerSample;
        DAT_00668f84 = handle->audio_stream;
        DAT_00668f48 = KLIBAUDIO_CreateAVISoundBuffer(fmt, DAT_00668f3c * DAT_00668fa4);
        return 1;
    }
    DAT_00668ee0 = 1;
    DAT_00668f70.wFormatTag = 1;
    DAT_00668f70.nChannels = fmt->nChannels;
    DAT_00668f70.nSamplesPerSec = fmt->nSamplesPerSec;
    DAT_00668f70.nBlockAlign = fmt->nChannels << 1;
    DAT_00668f70.nAvgBytesPerSec = (DAT_00668f70.nBlockAlign & 0xffff) * fmt->nSamplesPerSec;
    DAT_00668f70.wBitsPerSample = 0x10;
    DAT_00668f70.cbSize = 0;
    DAT_00668f44 = acmStreamOpen(&DAT_00668f5c, 0, fmt, &DAT_00668f70, 0, 0, 0, 4);
    if (DAT_00668f44 != 0) {
        return 0;
    }
    DAT_00668f9c = 0;
    DAT_00668f50 = DAT_00668fa4;
    DAT_00668f3c = DAT_00668f70.nAvgBytesPerSec / handle->field_4;
    DAT_00668f90 = DAT_00668f70.nSamplesPerSec / handle->field_4;
    DAT_00668f4c = DAT_00668f70.wBitsPerSample;
    DAT_00668f84 = handle->audio_stream;
    DAT_00668f48 = KLIBAUDIO_CreateAVISoundBuffer(&DAT_00668f70, DAT_00668f3c * DAT_00668fa4);
    p = DAT_00668ee8;
    for (i = 0x15; i != 0; i--) {
        *p = 0;
        p++;
    }
    DAT_00668ee8[8] = DAT_00668f3c;
    DAT_00668ee8[0] = 0x54;
    acmStreamSize(DAT_00668f5c, DAT_00668f3c, &DAT_00668ee8[4], 1);
    DAT_00668ee8[3] = (unsigned int)malloc(DAT_00668ee8[4]);
    DAT_00668ee8[7] = (unsigned int)malloc(DAT_00668ee8[8] * 3);
    DAT_00668f8c = (void *)DAT_00668ee8[7];
    DAT_00668f54 = 0;
    if (DAT_00668ee8[3] != 0 && DAT_00668ee8[7] != 0) {
        return 1;
    }
    acmStreamClose(DAT_00668f5c, 0);
    if (DAT_00668ee8[3] != 0) {
        free((void *)DAT_00668ee8[3]);
    }
    if (DAT_00668ee8[7] != 0) {
        free((void *)DAT_00668ee8[7]);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00476bf0
int FUN_00476bf0(struct MovieHandle *handle) {
    unsigned int start;
    unsigned int len_start;

    if (handle->audio_stream != NULL && DAT_00668f48 != NULL) {
        start = AVIStreamStart(handle->audio_stream);
        len_start = AVIStreamLength(handle->audio_stream) + AVIStreamStart(handle->audio_stream);
        DAT_00668f58 = start;
        DAT_00668f60 = start;
        DAT_00668f88 = len_start;
        DAT_00668f9c = 1;
        FUN_00476d20(0, 0);
        KLIBAUDIO_SetAVIVolume(DAT_00668f48, FUN_00458930(DAT_004bb4dc));
        KLIBAUDIO_PlayAVISoundBuffer(DAT_00668f48, 0);
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00476c90
int FUN_00476c90(void) {
    if (DAT_00668f9c != 0) {
        DAT_00668f9c = 0;
        KLIBAUDIO_StopAVISoundBuffer(DAT_00668f48);
        if (DAT_00668ee0 != 0) {
            if (DAT_00668ee8[3] != 0) {
                free((void *)DAT_00668ee8[3]);
                DAT_00668ee8[3] = 0;
            }
            if (DAT_00668f8c != NULL) {
                free(DAT_00668f8c);
                DAT_00668f8c = NULL;
            }
            acmStreamClose(DAT_00668f5c, 0);
        }
        return KLIBAUDIO_DestroyAVISoundBuffer(DAT_00668f48);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00476d20
int FUN_00476d20(unsigned int param_1, int param_2) {
    unsigned int *dst;
    unsigned int *src;
    unsigned int *p;
    unsigned int fill;
    unsigned int chunk;
    unsigned int rem;
    int produced;
    int play;
    unsigned int loops;
    int play_pos;
    int bytes_out;

    play = 0;
    if (DAT_00668f9c == 0 || DAT_00668f84 == NULL) {
        return 0;
    }
    if (param_1 == 0 && param_2 == 0) {
        loops = 0xb;
        DAT_00668f60 = 0;
        DAT_00668fa0 = 0;
    } else {
        loops = param_1 - param_2;
        if (loops < DAT_00668f50) {
            if (loops == 0) {
                return 1;
            }
        } else {
            KLIBAUDIO_StopAVISoundBuffer(DAT_00668f48);
            DAT_00668f60 = DAT_00668f90 * param_1;
            DAT_00668fa0 = param_1 % DAT_00668f50;
            play = 1;
            loops = 0xb;
            play_pos = DAT_00668f90 * DAT_00668fa0;
        }
    }
    do {
        dst = (unsigned int *)KLIBAUDIO_LockAVISoundBuffer(DAT_00668f48, DAT_00668f3c * DAT_00668fa0, DAT_00668f3c);
        chunk = DAT_00668f3c;
        if (DAT_00668f60 < DAT_00668f88 && (int)DAT_00668f60 >= 0) {
            if (DAT_00668ee0 == 0) {
                AVIStreamRead(DAT_00668f84, DAT_00668f60, DAT_00668f90, dst, DAT_00668f3c, (int *)&bytes_out, &param_2);
            } else {
                produced = 0;
                if (DAT_00668fb4 == 0) {
                    if (DAT_00668f3c != 0) {
                        do {
                            AVIStreamRead(DAT_00668f84, DAT_00668f60, 0x100, (void *)DAT_00668ee8[3], DAT_00668f3c >> 2, (int *)&bytes_out, &param_2);
                            DAT_00668ee8[7] = (unsigned int)DAT_00668f8c + produced;
                            acmStreamPrepareHeader(DAT_00668f5c, &DAT_00668ee8, 0);
                            acmStreamConvert(DAT_00668f5c, &DAT_00668ee8, 0x10);
                            acmStreamUnprepareHeader(DAT_00668f5c, &DAT_00668ee8, 0);
                            produced = produced + DAT_00668ee8[9];
                            DAT_00668fb4 = DAT_00668fb4 + DAT_00668ee8[9];
                            DAT_00668f60 = DAT_00668f60 + 1;
                        } while (DAT_00668fb4 < DAT_00668f3c);
                    }
                    src = (unsigned int *)(DAT_00668f54 + (int)DAT_00668f8c);
                    for (chunk = DAT_00668f3c >> 2; chunk != 0; chunk--) {
                        *dst = *src;
                        src++;
                        dst++;
                    }
                    for (chunk = DAT_00668f3c & 3; chunk != 0; chunk--) {
                        *(unsigned char *)dst = *(unsigned char *)src;
                        src = (unsigned int *)((char *)src + 1);
                        dst = (unsigned int *)((char *)dst + 1);
                    }
                    DAT_00668f54 = DAT_00668f54 + DAT_00668f3c;
                    DAT_00668fb4 = DAT_00668fb4 - DAT_00668f3c;
                } else if (DAT_00668fb4 < DAT_00668f3c) {
                    src = (unsigned int *)(DAT_00668f54 + (int)DAT_00668f8c);
                    p = dst;
                    for (chunk = DAT_00668fb4 >> 2; chunk != 0; chunk--) {
                        *p = *src;
                        src++;
                        p++;
                    }
                    rem = DAT_00668fb4;
                    for (chunk = rem & 3; chunk != 0; chunk--) {
                        *(unsigned char *)p = *(unsigned char *)src;
                        src = (unsigned int *)((char *)src + 1);
                        p = (unsigned int *)((char *)p + 1);
                    }
                    DAT_00668fb4 = 0;
                    rem = DAT_00668f3c - DAT_00668fb4;
                    DAT_00668f54 = 0;
                    if (DAT_00668f60 < DAT_00668f88) {
                        if (DAT_00668f3c != 0) {
                            do {
                                AVIStreamRead(DAT_00668f84, DAT_00668f60, 0x100, (void *)DAT_00668ee8[3], DAT_00668f3c >> 2, (int *)&bytes_out, &param_2);
                                DAT_00668ee8[7] = (unsigned int)DAT_00668f8c + produced;
                                acmStreamPrepareHeader(DAT_00668f5c, &DAT_00668ee8, 0);
                                acmStreamConvert(DAT_00668f5c, &DAT_00668ee8, 0x10);
                                acmStreamUnprepareHeader(DAT_00668f5c, &DAT_00668ee8, 0);
                                produced = produced + DAT_00668ee8[9];
                                DAT_00668fb4 = DAT_00668fb4 + DAT_00668ee8[9];
                                DAT_00668f60 = DAT_00668f60 + 1;
                            } while (DAT_00668fb4 < DAT_00668f3c);
                        }
                        src = DAT_00668f8c;
                        dst = (unsigned int *)((DAT_00668f3c - rem) + (char *)dst);
                        for (chunk = rem >> 2; chunk != 0; chunk--) {
                            *dst = *src;
                            src++;
                            dst++;
                        }
                        for (chunk = rem & 3; chunk != 0; chunk--) {
                            *(unsigned char *)dst = *(unsigned char *)src;
                            src = (unsigned int *)((char *)src + 1);
                            dst = (unsigned int *)((char *)dst + 1);
                        }
                        DAT_00668f54 = DAT_00668f54 + rem;
                        DAT_00668fb4 = DAT_00668fb4 - rem;
                    } else if (DAT_00668f4c == 8) {
                        dst = (unsigned int *)((DAT_00668f3c - rem) + (char *)dst);
                        for (chunk = rem >> 2; chunk != 0; chunk--) {
                            *dst = 0x80808080;
                            dst++;
                        }
                        for (chunk = rem & 3; chunk != 0; chunk--) {
                            *(unsigned char *)dst = 0x80;
                            dst = (unsigned int *)((char *)dst + 1);
                        }
                    } else {
                        dst = (unsigned int *)((DAT_00668f3c - rem) + (char *)dst);
                        for (chunk = rem >> 2; chunk != 0; chunk--) {
                            *dst = 0;
                            dst++;
                        }
                        for (chunk = rem & 3; chunk != 0; chunk--) {
                            *(unsigned char *)dst = 0;
                            dst = (unsigned int *)((char *)dst + 1);
                        }
                    }
                } else {
                    src = (unsigned int *)(DAT_00668f54 + (int)DAT_00668f8c);
                    for (chunk = DAT_00668f3c >> 2; chunk != 0; chunk--) {
                        *dst = *src;
                        src++;
                        dst++;
                    }
                    for (chunk = DAT_00668f3c & 3; chunk != 0; chunk--) {
                        *(unsigned char *)dst = *(unsigned char *)src;
                        src = (unsigned int *)((char *)src + 1);
                        dst = (unsigned int *)((char *)dst + 1);
                    }
                    DAT_00668fb4 = DAT_00668fb4 - DAT_00668f3c;
                    if (DAT_00668fb4 == 0) {
                        DAT_00668f54 = 0;
                    } else {
                        DAT_00668f54 = DAT_00668f54 + DAT_00668f3c;
                    }
                }
            }
        } else {
            if (DAT_00668f4c == 8) {
                fill = 0x80808080;
            } else {
                fill = 0;
            }
            for (chunk = DAT_00668f3c >> 2; chunk != 0; chunk--) {
                *dst = fill;
                dst++;
            }
            for (chunk = DAT_00668f3c & 3; chunk != 0; chunk--) {
                *(char *)dst = (char)fill;
                dst = (unsigned int *)((char *)dst + 1);
            }
            if (DAT_00668ee0 != 0) {
                DAT_00668f60 = DAT_00668f60 + 1;
            }
        }
        KLIBAUDIO_UnLockAVISoundBuffer(DAT_00668f48);
        if (DAT_00668ee0 == 0) {
            DAT_00668f60 = DAT_00668f60 + DAT_00668f90;
        }
        DAT_00668fa0 = DAT_00668fa0 + 1;
        if (DAT_00668f50 <= DAT_00668fa0) {
            DAT_00668fa0 = 0;
        }
        loops = loops - 1;
    } while (loops != 0);
    if (play != 0) {
        KLIBAUDIO_PlayAVISoundBuffer(DAT_00668f48, play_pos);
        KLIBAUDIO_SetAVIVolume(DAT_00668f48, FUN_00458930(DAT_004bb4dc));
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004771f0
void FUN_004771f0(const char *filename, unsigned int param_2, unsigned int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x00477400
int FUN_00477400(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00477410
void FUN_00477410(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00477440
void FUN_00477440(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004775b0
void *FUN_004775b0(unsigned int size) { STUB(); }

// FUNCTION: LEGOLAND 0x004775d0
void FUN_004775d0(void *param) { STUB(); }

// FUNCTION: LEGOLAND 0x004775f0
void FUN_004775f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00477600
void FUN_00477600(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00477680
int FUN_00477680(int a, int b) {
    unsigned short limit;

    if (a < 0) {
        return 0;
    }
    if (b < 0) {
        return 0;
    }
    if (a >= lpConfig->width) {
        return 0;
    }
    limit = lpConfig->height;
    return b < limit;
}

// FUNCTION: LEGOLAND 0x004776c0
void FUN_004776c0(struct QueryNode *node) {
    node->next = (struct QueryNode *)DAT_00668fc4;
    DAT_00668fc4 = (struct InterfaceQueryNode *)node;
}

// FUNCTION: LEGOLAND 0x004776e0
void FUN_004776e0(struct EventNode *node) {
    struct EventNode *current;
    struct EventNode *previous;

    if (DAT_00668fc0 == NULL) {
        DAT_00668fc0 = node;
        node->next = NULL;
    } else {
        current = DAT_00668fc0;
        previous = NULL;
        while (current != NULL) {
            if (current->sort_key >= node->sort_key) {
                break;
            }
            previous = current;
            current = current->next;
        }
        if (previous != NULL) {
            node->next = previous->next;
            previous->next = node;
        } else {
            node->next = DAT_00668fc0;
            DAT_00668fc0 = node;
        }
    }
}

// FUNCTION: LEGOLAND 0x00477730
struct QueryNode *FUN_00477730(struct QueryNode *ctx) {
    struct QueryNode *node;

    node = (struct QueryNode *)DAT_00668fc4;
    if (node == NULL) {
        return NULL;
    }
    while (node != NULL) {
        if (node->field_8 == *(unsigned int *)ctx && node->field_c == *((unsigned int *)ctx + 1)) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00477760
void FUN_00477760(struct QueryNode *ctx) {
    struct QueryNode *prev;
    struct QueryNode *node;

    prev = NULL;
    node = (struct QueryNode *)DAT_00668fc4;
    while (node != NULL) {
        if (node == ctx) {
            break;
        }
        prev = node;
        node = node->next;
    }
    if (prev != NULL) {
        prev->next = node->next;
    } else {
        DAT_00668fc4 = (struct InterfaceQueryNode *)node->next;
    }
}

// FUNCTION: LEGOLAND 0x00477790
void FUN_00477790(struct EventNode *param_1) {
    struct EventNode *prev;
    struct EventNode *node;

    prev = NULL;
    node = DAT_00668fc0;
    while (node != NULL) {
        if (node == param_1) {
            break;
        }
        prev = node;
        node = node->next;
    }
    if (prev != NULL) {
        prev->next = node->next;
        return;
    }
    DAT_00668fc0 = node->next;
}
