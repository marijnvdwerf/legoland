#include "legoland.h"
#include "crt.h"
#include "globals.h"
#include "string.h"
#include "worker_mouse.h"
#include "profile_io.h"
#include "screens.h"
#include "title.h"
#include "nerps.h"
#include "popupinfo.h"
#include "objclass.h"
#include "icon.h"
#include "interface.h"
#include "sound_music.h"
#include "llidb.h"

struct ProfileObj {
    unsigned char pad_0[0x34];
    unsigned int flags;
};

struct InterfaceObj {
    unsigned char pad_0[0x2c];
    void *field_2c;
    unsigned char pad_30[0x34 - 0x30];
    unsigned int field_34;
    unsigned int field_38;
    unsigned int field_3c;
};

struct ObjectClassInfo {
    /* 0x00 */ char *name;
    /* 0x04 */ unsigned char pad_4[0x8 - 0x4];
    /* 0x08 */ unsigned int flags;
};

struct BuildObject;

struct LLDBElem {
    unsigned char pad_0[0xc];
    /* 0x0c */ struct BuildObject *obj;
};

struct BuildObject {
    unsigned char pad_0[0x1c];
    /* 0x1c */ unsigned int field_1c;
    unsigned char pad_20[0x58 - 0x20];
    /* 0x58 */ void *field_58;
    /* 0x5c */ void *field_5c;
    /* 0x60 */ void *field_60;
    unsigned char pad_64[0xc4 - 0x64];
    /* 0xc4 */ struct ObjectClassInfo *field_c4;
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
#include "stream.h"

char FUN_00475c50(int param_1, unsigned char param_2);
char FUN_00475c90(int param_1, unsigned char param_2);
extern void FUN_004562e0(void);
void FUN_00474750(void);
LEGO_EXPORT unsigned int TestMenu(unsigned int *entry);


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
LEGO_EXPORT int InitGameInterface(int a) { STUB(); }

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
LEGO_EXPORT unsigned int ObjectLinkedList(unsigned int *entry) { STUB(); }

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
LEGO_EXPORT void MakeUpObjectList(int param_1, int param_2, int param_3, int param_4) { STUB(); }

// FUNCTION: LEGOLAND 0x00475bb0
LEGO_EXPORT void ListChildrenBar(void *node, int group, short x, short y) {
    struct SpriteIcon *icon;

    // STRING: LEGOLAND 0x004bb4a8
    icon = LoadSpriteIcon("ListChildrenBar.lls", 4, x, y, group);
    icon->field_3c = 100;
    icon->field_38 = (unsigned int)GetString(100);
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
    icon->field_38 = (unsigned int)GetString(0x65);
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
LEGO_EXPORT void RAndDLinkedList(void) { STUB(); }

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
void FUN_00475f40(void) { STUB(); }

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
void FUN_004760a0(void) { STUB(); }

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
void FUN_00476460(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00476630
void FUN_00476630(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00476680
void FUN_00476680(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004766f0
void FUN_004766f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00476910
void FUN_00476910(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00476bf0
void FUN_00476bf0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00476c90
void FUN_00476c90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00476d20
void FUN_00476d20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004771f0
void FUN_004771f0(const char *filename, unsigned int param_2, unsigned int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x00477400
void FUN_00477400(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00477410
void FUN_00477410(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00477440
void FUN_00477440(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004775b0
struct CastleObj *FUN_004775b0(unsigned int size, unsigned int a, unsigned int b, unsigned int c) { STUB(); }

// FUNCTION: LEGOLAND 0x004775d0
void FUN_004775d0(unsigned int param) { STUB(); }

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
void FUN_00477790(void) { STUB(); }
