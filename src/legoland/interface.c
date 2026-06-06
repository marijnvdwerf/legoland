#include "legoland.h"
#include "string.h"
#include "worker_mouse.h"
#include "profile_io.h"
#include "screens.h"
#include "title.h"
#include "popupinfo.h"

struct IconNode {
    struct IconNode *next;
    unsigned char pad_4[0x10];
    unsigned short id;
    unsigned char pad_16[0x34 - 0x16];
    unsigned int flags;
};

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

struct ListNode {
    struct ListNode *next;
};

struct ResearchNode {
    struct ResearchNode *next;
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

extern unsigned int DAT_00668ea4;
extern unsigned int DAT_00668e68;
extern unsigned int DAT_00668e6c;
extern unsigned int DAT_00668e70;
extern unsigned int DAT_007fdcd0;
extern unsigned int DAT_007fdd50;
extern unsigned int DAT_007fdcd4;
extern unsigned int DAT_007fdd54;
extern unsigned int DAT_007fdcd8;
extern unsigned int DAT_007fdd58;
extern unsigned int DAT_007fdcdc;
extern unsigned int DAT_007fdd5c;
extern unsigned int DAT_007fdce0;
extern unsigned int DAT_007fdd60;
extern unsigned int DAT_00668e74;
extern unsigned int DAT_00668e78;
extern unsigned int DAT_00668e7c;
extern unsigned int DAT_00668e80;
extern unsigned int DAT_00668e84;
extern unsigned int DAT_00668e88;
extern unsigned int DAT_00668e8c;
extern unsigned int DAT_00668e90;
extern unsigned int DAT_00668e94;
extern unsigned int DAT_00668e98;
extern unsigned int DAT_00668ea0;
extern unsigned int DAT_00668eb0;
extern unsigned int DAT_004bb0a0;
extern unsigned int DAT_004bb09c;
extern unsigned int DAT_004bb098;
extern unsigned int DAT_004bb094;
extern unsigned int DAT_00668eb4;
extern unsigned int DAT_007fdcc0;
extern unsigned int DAT_007fdd40;
extern unsigned int DAT_007fdcc8;
extern unsigned int DAT_007fdd48;
extern unsigned int DAT_007fdcc4;
extern unsigned int DAT_007fdd44;
extern unsigned int DAT_007fdccc;
extern unsigned int DAT_007fdd4c;
extern unsigned int DAT_006687bc;
extern unsigned int DAT_006687c0;
extern unsigned int DAT_00668eb8;
extern struct ProfileObj *DAT_007fdd70[4];
extern unsigned int DAT_00668e20[4];
extern unsigned int DAT_00668ebc;
extern void *DAT_00668e9c;
extern unsigned char DAT_007fdd80;
extern unsigned int DAT_007fdd84;
extern unsigned int DAT_007fdd88;
extern unsigned char DAT_007fdd8c;
extern unsigned int DAT_008119b4;
extern unsigned int EditMode;
extern unsigned int GamePad;
extern char DAT_0066869c;
extern char DAT_0066861c;
extern unsigned int DAT_00668954;
extern void *PTR_004b92c0;
extern unsigned int DAT_00668e38;
extern unsigned int DAT_0080ff84;
extern unsigned int DAT_0080ff88;
extern unsigned int DAT_006687b0;
extern struct ListNode *DAT_00668e40;
extern unsigned int DAT_004baff8;
extern unsigned int DAT_004bafa8[20];
extern unsigned int DAT_004bb18c[4];
extern unsigned int DAT_007fdd00[9];
extern unsigned char DAT_0080ffd0[4];
extern struct ResearchNode *DAT_00668ed8;
extern struct IconNode *DAT_006687c8;
extern struct LegoConfig *lpConfig;
extern struct QueryNode *DAT_00668fc4;
extern struct EventNode *DAT_00668fc0;

struct LegoConfig {
    unsigned char pad_0[0x14];
    unsigned short width;
    unsigned short height;
};

extern unsigned int LoadSprite(const char *name, unsigned int flags);
extern void KillSprite(unsigned int sprite);
extern void FUN_0046d680(unsigned int a, unsigned int b);
extern void UnLoad_Interface_ControlIcons(void);
extern void FUN_0046fb40(unsigned int a);
extern int FUN_0046b280(void);
extern unsigned int SaveGameWrite(unsigned int *data, unsigned int size);
extern unsigned int SaveGameRead(void *data, unsigned int size);
extern unsigned int RemoveIconGroup(unsigned int group);
extern unsigned int FUN_0046d590(unsigned int a);
extern void FUN_0046b700(void);
extern void PlayInstanceOfSample(void *sample, int a, int b, int c);
extern void SetEditObject(unsigned int obj);
extern void FUN_00498920(void);
extern void FUN_0049e4d0(void *block);
extern unsigned int ObjectLinkedList(unsigned int *entry);
extern unsigned int TestMenu(unsigned int *entry);

unsigned char FUN_00474820(unsigned int dummy, unsigned char flags);
unsigned char FUN_00474830(unsigned int a, unsigned int flags, unsigned int c, unsigned int d);
unsigned char FUN_00474f80(unsigned int a, unsigned int flags);
unsigned char FUN_00475120(unsigned int a, unsigned int flags, unsigned int c, unsigned int d);
void FUN_00475fe0(int index, unsigned int value);
void FUN_00476030(int index, unsigned int value);
void FUN_00476140(int index, int value);

// FUNCTION: LEGOLAND 0x004741f0
void Load_Interface_ControlIcons(void) {
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
void UnLoad_Interface_ControlIcons(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00474590
void FUN_00474590(void) {
    DAT_00668eb0 = 0;
    DAT_004bb0a0 = 1;
    DAT_004bb09c = 1;
    DAT_004bb098 = 1;
    DAT_004bb094 = 1;
}

// FUNCTION: LEGOLAND 0x004745c0
void Load_Interface_ThemeIcons(void) {
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
void UnLoad_Interface_ThemeIcons(void) {
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
        FUN_0046d680(DAT_00668eb0, DAT_007fdd40);
    } else if (DAT_004bb098 == 0) {
        DAT_004bb098 = 1;
        FUN_0046d680(DAT_00668eb0, DAT_007fdd48);
    } else if (DAT_004bb09c == 0) {
        DAT_004bb09c = 1;
        FUN_0046d680(DAT_00668eb0, DAT_007fdd44);
    } else if (DAT_004bb0a0 == 0) {
        DAT_004bb0a0 = 1;
        FUN_0046d680(DAT_00668eb0, DAT_007fdd4c);
    }
}

// FUNCTION: LEGOLAND 0x00474800
void UnLoad_Interface_Icons(void) {
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
        if (DAT_007fdd70[i]->flags & 0x400) {
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
            DAT_007fdd70[i]->flags &= 0xfffffbff;
        } else {
            DAT_007fdd70[i]->flags |= 0x400;
        }
    }
}

// FUNCTION: LEGOLAND 0x004749d0
void InitGameInterface(void) { STUB(); }

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
            FUN_004911c0(&DAT_0066861c, &DAT_0066869c);
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
        SetEditObject(*(unsigned int *)((char *)a + 8));
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
void FUN_00475080(void) { STUB(); }

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
void FUN_004751a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004752a0
void FUN_004752a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004753a0
void FUN_004753a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004754b0
void FUN_004754b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004755c0
void FUN_004755c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00475630
void InsertChildIntoList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004756e0
void DelObjectList(void) {
    struct ListNode *current;
    struct ListNode *next;

    current = DAT_00668e40;
    while (current != NULL) {
        next = current->next;
        FUN_0049e4d0(current);
        current = next;
    }
    DAT_00668e40 = NULL;
}

// FUNCTION: LEGOLAND 0x00475710
unsigned int TestMenu(unsigned int *entry) {
    return ObjectLinkedList(entry);
}

// FUNCTION: LEGOLAND 0x00475720
unsigned int ObjectLinkedList(unsigned int *entry) { STUB(); }

// FUNCTION: LEGOLAND 0x004758c0
void UpdateMenu(void) {
    if (DAT_004baff8 == 5) {
        return;
    }
    TestMenu(&DAT_004bafa8[DAT_004baff8 * 5]);
}

// FUNCTION: LEGOLAND 0x004758e0
void RedrawObjectList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00475960
void MakeUpObjectList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00475bb0
void ListChildrenBar(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00475c00
void CloseChildrenBar(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00475c50
void FUN_00475c50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00475c90
void FUN_00475c90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00475cd0
void RAndDLinkedList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00475e90
void DisableSidePanelIcons(void) {
    struct IconNode *node;

    node = DAT_006687c8;
    while (node != NULL) {
        if (node->id == 0xd2 || node->id == 0xd5 || node->id == 0xd6 || node->id == 0xd7) {
            node->flags |= 0x400;
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00475ed0
void EnableSidePanelIcons(void) {
    struct IconNode *node;

    node = DAT_006687c8;
    while (node != NULL) {
        if (node->id == 0xd2 || node->id == 0xd5 || node->id == 0xd6 || node->id == 0xd7) {
            node->flags &= 0xfffffbff;
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

    obj = DAT_007fdd70[index];
    if (obj != NULL) {
        if (value != 0) {
            obj->flags &= 0xfffffbff;
            DAT_0080ffd0[index] = 1;
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

    flags = DAT_0080ffd0;
    items = DAT_007fdd70;
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
void InitRAndDCheckBox(void) {}

// FUNCTION: LEGOLAND 0x004761d0
void Unload_RAndDCheckBox(void) {}

// FUNCTION: LEGOLAND 0x004761f0
void RenderRAndDCheckBox(void) {}

// FUNCTION: LEGOLAND 0x00476200
void CloseCheckBoxRAndD(void) {}

// FUNCTION: LEGOLAND 0x00476210
void DisableRAndDIcons(void) {}

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
void FUN_00476250(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004762f0
void FUN_004762f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004763d0
void CleanUpReseachList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00476420
void DeleteReseachList(void) {
    struct ResearchNode *current;
    struct ResearchNode *next;

    current = DAT_00668ed8;
    while (current != NULL) {
        next = current->next;
        FUN_0049e4d0(current);
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
void FUN_004771f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00477400
void FUN_00477400(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00477410
void FUN_00477410(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00477440
void FUN_00477440(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004775b0
void FUN_004775b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004775d0
void FUN_004775d0(void) { STUB(); }

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
    node->next = DAT_00668fc4;
    DAT_00668fc4 = node;
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

    node = DAT_00668fc4;
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
    node = DAT_00668fc4;
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
        DAT_00668fc4 = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00477790
void FUN_00477790(void) { STUB(); }
