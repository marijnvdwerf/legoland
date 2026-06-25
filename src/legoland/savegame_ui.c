#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "debug_alloc.h"
#include "draw.h"
#include "icon.h"
#include "input.h"
#include "main.h"
#include "math.h"
#include "options.h"
#include "profile.h"
#include "profile_io.h"
#include "savegame_ui.h"
#include "saveload.h"
#include "screens.h"
#include "sound_music.h"
#include "string.h"
#include "timer.h"
#include "title.h"

#include "image_sprite.h"
#include "stream.h"

#pragma intrinsic(memset, strcpy)

struct SaveNode {
    struct SaveNode *next;
    struct ProfileData data;
    int has_header;
    unsigned char slot;
};

struct SaveScreenIcon {
    unsigned char pad_0[0x18];
    void *field_18;
    unsigned char field_1c;
    unsigned char pad_1d[0x20 - 0x1d];
    unsigned char field_20;
    unsigned char pad_21[0x2c - 0x21];
    void *field_2c;
    unsigned char pad_30[0x34 - 0x30];
    unsigned int field_34;
    unsigned int field_38;
    unsigned int field_3c;
};

struct SaveScreenNode {
    struct SaveScreenNode *next;
    char name[0x24];
    unsigned char field_28;
    unsigned char pad_29[0x114 - 0x29];
    int has_header;
    unsigned char slot;
};

// FUNCTION: LEGOLAND 0x0048d4b0
LEGO_EXPORT void InitSavedGameScreen(void) {
    struct SaveScreenIcon *icon;
    struct SaveScreenNode *node;

    // STRING: LEGOLAND 0x004bf2a0
    SPRITE_TitleScreenBk = LoadSprite("Saved_Game_Screen.lls", 0);
    DAT_0080ffe4 = 0;
    // STRING: LEGOLAND 0x004bf28c
    DAT_00798704 = LoadSprite("RegSaveSlotOn.lls", 4);
    // STRING: LEGOLAND 0x004bf278
    DAT_00798708 = LoadSprite("RegSaveOff_1.lls", 4);
    // STRING: LEGOLAND 0x004bf264
    DAT_0079870c = LoadSprite("RegSaveOff_2.lls", 4);
    // STRING: LEGOLAND 0x004bf250
    DAT_00798710 = LoadSprite("RegSaveOff_3.lls", 4);
    // STRING: LEGOLAND 0x004bf23c
    DAT_00798714 = LoadSprite("RegSaveOff_4.lls", 4);
    // STRING: LEGOLAND 0x004bf228
    DAT_00798718 = LoadSprite("RegSaveOff_5.lls", 4);
    // STRING: LEGOLAND 0x004bf214
    DAT_0079871c = LoadSprite("RegSaveOff_6.lls", 4);
    // STRING: LEGOLAND 0x004bf200
    DAT_00798720 = LoadSprite("RegSaveOff_7.lls", 4);
    // STRING: LEGOLAND 0x004bf1ec
    DAT_00798724 = LoadSprite("RegSaveOff_8.lls", 4);
    // STRING: LEGOLAND 0x004bf1d8
    DAT_00798728 = LoadSprite("SaveType_Normal.lls", 4);
    // STRING: LEGOLAND 0x004bf1c4
    DAT_0079872c = LoadSprite("SaveType_Free.lls", 4);
    // STRING: LEGOLAND 0x004bf1b4
    DAT_0079868c = LoadSprite("RegDeleteON.lls", 4);
    // STRING: LEGOLAND 0x004bf104
    DAT_00798690 = LoadSprite("RegDelete.lls", 4);
    // STRING: LEGOLAND 0x004bf19c
    DAT_007986b8 = LoadSprite("RegSaveDouble_PopUp.lls", 4);
    // STRING: LEGOLAND 0x004bf188
    DAT_00798730 = LoadSprite("RegCornerMask.lls", 4);

    // STRING: LEGOLAND 0x004bf170
    icon = (struct SaveScreenIcon *)LoadSpriteIcon("GoBack_on_SavedGame.lls", 4, 0x1c2, 0x13, 7);
    icon->field_34 |= 0x6002;
    if (DAT_007cb324) {
        icon->field_2c = (void *)FUN_0048fb80;
        icon->field_3c = 0x26;
        icon->field_38 = GetString(0x26);
    } else {
        icon->field_2c = (void *)FUN_0048db10;
        icon->field_3c = 0x28;
        icon->field_38 = GetString(0x28);
    }
    DAT_006687c0 = (unsigned int)icon->field_2c;

    if (!DAT_007cb328) {
        // STRING: LEGOLAND 0x004bf15c
        DAT_007986e0 = (unsigned int)LoadSpriteIcon("Accept_On_Save.lls", 4, 0x1dc, 0x142, 7);
        ((struct SaveScreenIcon *)DAT_007986e0)->field_34 |= 0x2000;
        ((struct SaveScreenIcon *)DAT_007986e0)->field_34 |= 0x4002;
        ((struct SaveScreenIcon *)DAT_007986e0)->field_34 |= 0x400;
        if (DAT_007cb310) {
            ((struct SaveScreenIcon *)DAT_007986e0)->field_2c = (void *)FUN_0048f5a0;
            ((struct SaveScreenIcon *)DAT_007986e0)->field_3c = 0x29;
            ((struct SaveScreenIcon *)DAT_007986e0)->field_38 = GetString(0x29);
        } else {
            ((struct SaveScreenIcon *)DAT_007986e0)->field_2c = (void *)FUN_0048da50;
            ((struct SaveScreenIcon *)DAT_007986e0)->field_3c = 0x2a;
            ((struct SaveScreenIcon *)DAT_007986e0)->field_38 = GetString(0x2a);
        }
    } else {
        // STRING: LEGOLAND 0x004bf15c
        DAT_007986e0 = (unsigned int)LoadSpriteIcon("Accept_On_Save.lls", 4, 0x1dc, 0x142, 7);
        ((struct SaveScreenIcon *)DAT_007986e0)->field_3c = 0x2b;
        ((struct SaveScreenIcon *)DAT_007986e0)->field_38 = GetString(0x2b);
        ((struct SaveScreenIcon *)DAT_007986e0)->field_34 |= 0x2000;
        ((struct SaveScreenIcon *)DAT_007986e0)->field_34 |= 0x4002;
        ((struct SaveScreenIcon *)DAT_007986e0)->field_34 |= 0x400;
        ((struct SaveScreenIcon *)DAT_007986e0)->field_2c = (void *)FUN_0048d970;
    }
    DAT_006687bc = (unsigned int)((struct SaveScreenIcon *)DAT_007986e0)->field_2c;

    FUN_0048d470();
    DeleteSavedGameList();
    LoadSavedGamesList(DAT_0080ffe3);

    node = (struct SaveScreenNode *)DAT_00798734;
    if (node) {
        do {
            if (node->has_header) {
                icon = (struct SaveScreenIcon *)InsertIcon(0x51, (short)(node->slot * 38 + 0x6f), 7, GetSavePanelBK(node->slot));
                icon->field_2c = (void *)FUN_0048e4a0;
                icon->field_34 |= 0x4002;
                icon->field_18 = node->name;
                icon->field_1c = node->slot;
                if (node->field_28 == 1) {
                    icon->field_20 |= 3;
                    icon->field_3c = 0x2c;
                    icon->field_38 = GetString(0x2c);
                } else {
                    icon->field_20 |= 5;
                    icon->field_3c = 0x2d;
                    icon->field_38 = GetString(0x2d);
                }
            } else {
                icon = (struct SaveScreenIcon *)InsertIcon(0x51, (short)(node->slot * 38 + 0x6f), 7, GetSavePanelBK(node->slot));
                icon->field_3c = 0x2e;
                icon->field_38 = GetString(0x2e);
                icon->field_34 |= 0x6002;
                icon->field_2c = (void *)FUN_0048e4f0;
                // STRING: LEGOLAND 0x004befa0
                icon->field_18 = "EMPTY";
                icon->field_1c = node->slot;
                icon->field_20 |= 1;
            }
            node = node->next;
        } while (node);
    }

    DAT_007cb360 = (unsigned int)InsertIcon(0, 0, 7, DAT_00798690);
    ((struct SaveScreenIcon *)DAT_007cb360)->field_3c = 5;
    ((struct SaveScreenIcon *)DAT_007cb360)->field_38 = GetString(5);
    ((struct SaveScreenIcon *)DAT_007cb360)->field_34 |= 0x2000;
    ((struct SaveScreenIcon *)DAT_007cb360)->field_34 |= 0x4002;
    ((struct SaveScreenIcon *)DAT_007cb360)->field_34 |= 0x400;
    ((struct SaveScreenIcon *)DAT_007cb360)->field_2c = (void *)FUN_0048cc30;
}

// FUNCTION: LEGOLAND 0x0048d8f0
LEGO_EXPORT int LoadDateIntoTempProfile(int a1, int a2) {
    char buffer[132];
    void *file;

    // STRING: LEGOLAND 0x004bf2bc
    sprintf(buffer, "profiles\\%dsave%d.sh", a1, a2);

    if (!Goto_ProfileDir()) {
        return 0;
    }

    // STRING: LEGOLAND 0x004bf2b8
    file = fopen(buffer, "r");
    if (file != 0) {
        fread(&DAT_007cad60, sizeof(struct ProfileData), 1, file);
        fclose(file);
        ReturnFrom_ProfileDir();
        return 1;
    }

    return 0;
}

// FUNCTION: LEGOLAND 0x0048d970
unsigned char FUN_0048d970(unsigned int a1, unsigned char flags) {
    if (DAT_004bef9c != 0 && (flags & 2) && DAT_0080ffa0.field_44 != 0) {
        FUN_00498920();
        DAT_006687b0 = 4;
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        if (LoadDateIntoTempProfile(DAT_0080ffe3, DAT_0080ffa0.field_44 & 0xff) != 0) {
            DAT_0080ffe5 = DAT_007cad60.field_24;
            DAT_0080ffa0.field_20 = DAT_007cad60.field_20;
            DAT_0080ffa0.field_24 = DAT_007cad60.field_28;
            DAT_0080ffa0.field_28 = DAT_007cad60.field_2c;
            DAT_0080ffa0.field_2c = DAT_007cad60.field_30;
            DAT_00667c64 = 1;
            DAT_00667c80 = 1;
        }
        RemoveIconGroup(7);
        KillSaveScreenSprites();
        KillTitleScreenSprites();
        DeleteSavedGameList();
        if (DAT_007cb324 == 0) {
            FUN_00458b20();
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048da50
unsigned char FUN_0048da50(unsigned int a1, unsigned int flags, unsigned int a3, unsigned int a4) {
    if (DAT_004bef9c == 0) {
        int r = FUN_0048e720(0, flags, a3, a4);
        if (DAT_0080ff80.unk4 != 0xffffffff) {
            return r;
        }
        DAT_00668e38 = 0;
        RemoveIconGroup(7);
    } else {
        if (!(flags & 2) || DAT_0080ffe4 == 0) {
            return 1;
        }
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        FUN_00498920();
        DAT_006687b0 = 4;
        StoreNewSaveGameToDisk();
        DAT_00668e38 = 0;
        RemoveIconGroup(7);
    }
    KillTitleScreenSprites();
    EditMode.unk4 = 3;
    FUN_00474880();
    FUN_004993c0();
    DAT_004bef9c = 1;
    return 1;
}

// FUNCTION: LEGOLAND 0x0048db10
unsigned char FUN_0048db10(int param1, unsigned char flags) {
    if (DAT_004bef9c != 0 && (flags & 0x2)) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_0080ff80.unk8 = 5;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048db50
LEGO_EXPORT struct Sprite *GetSavePanelBK(signed char slot) {
    switch (slot) {
    case 1:
        return DAT_00798708;
    case 2:
        return DAT_0079870c;
    case 3:
        return DAT_00798710;
    case 4:
        return DAT_00798714;
    case 5:
        return DAT_00798718;
    case 6:
        return DAT_0079871c;
    case 7:
        return DAT_00798720;
    case 8:
        return DAT_00798724;
    default:
        return 0;
    }
}

// FUNCTION: LEGOLAND 0x0048dbc0
LEGO_EXPORT void KillSaveScreenSprites(void) {
    if (DAT_00798704 != 0) {
        KillSprite(DAT_00798704);
        DAT_00798704 = 0;
    }
    if (DAT_00798708 != 0) {
        KillSprite(DAT_00798708);
        DAT_00798708 = 0;
    }
    if (DAT_0079870c != 0) {
        KillSprite(DAT_0079870c);
        DAT_0079870c = 0;
    }
    if (DAT_00798710 != 0) {
        KillSprite(DAT_00798710);
        DAT_00798710 = 0;
    }
    if (DAT_00798714 != 0) {
        KillSprite(DAT_00798714);
        DAT_00798714 = 0;
    }
    if (DAT_00798718 != 0) {
        KillSprite(DAT_00798718);
        DAT_00798718 = 0;
    }
    if (DAT_0079871c != 0) {
        KillSprite(DAT_0079871c);
        DAT_0079871c = 0;
    }
    if (DAT_00798720 != 0) {
        KillSprite(DAT_00798720);
        DAT_00798720 = 0;
    }
    if (DAT_00798724 != 0) {
        KillSprite(DAT_00798724);
        DAT_00798724 = 0;
    }
    if (DAT_00798728 != 0) {
        KillSprite(DAT_00798728);
        DAT_00798728 = 0;
    }
    if (DAT_0079872c != 0) {
        KillSprite(DAT_0079872c);
        DAT_0079872c = 0;
    }
    if (DAT_00798730 != 0) {
        KillSprite(DAT_00798730);
        DAT_00798730 = 0;
    }
    if (DAT_0079868c != 0) {
        KillSprite(DAT_0079868c);
        DAT_0079868c = 0;
    }
}

// FUNCTION: LEGOLAND 0x0048dd00
LEGO_EXPORT void PrintSavedGameDetails(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048e0c0
void FUN_0048e0c0(int has_header, struct ProfileData *header, unsigned char slot) {
    struct SaveNode *node = (struct SaveNode *)malloc(sizeof(struct SaveNode));
    memset(node, 0, sizeof(struct SaveNode));
    if (has_header != 0) {
        node->has_header = 1;
        strcpy(node->data.name, header->name);
        node->slot = slot;
        node->data.field_24 = header->field_24;
        node->next = (struct SaveNode *)DAT_00798734;
        DAT_00798734 = node;
        return;
    }
    node->has_header = 0;
    node->slot = slot;
    node->next = (struct SaveNode *)DAT_00798734;
    DAT_00798734 = node;
}

// FUNCTION: LEGOLAND 0x0048e160
LEGO_EXPORT void DeleteSavedGameList(void) {
    struct SaveNode *current = (struct SaveNode *)DAT_00798734;
    while (current != NULL) {
        struct SaveNode *next = current->next;
        free(current);
        current = next;
    }
    DAT_00798734 = NULL;
}

// FUNCTION: LEGOLAND 0x0048e190
LEGO_EXPORT unsigned char LoadSavedGamesList(unsigned char profile) {
    char path[120];
    struct ProfileData header;
    char slot;
    int n;
    int rc;
    void *file;

    if (!Goto_ProfileDir()) {
        return 0xff;
    }

    slot = 8;
    n = 8;
    do {
        sprintf(path, "profiles\\%dsave%d.sh", profile, n);
        file = fopen(path, "r");
        if (file == 0) {
            FUN_0048e0c0(0, &header, slot);
        } else {
            fread(&header, sizeof(struct ProfileData), 1, file);
            FUN_0048e0c0(1, &header, slot);
            fclose(file);
        }
        memset(&header, 0, sizeof(struct ProfileData));
        slot = slot - 1;
        n = n - 1;
    } while (slot != 0);

    rc = ReturnFrom_ProfileDir();
    return (rc != 0) - 1;
}

// FUNCTION: LEGOLAND 0x0048e280
LEGO_EXPORT void InitNewSaveGamePOPUP(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048e3d0
void FUN_0048e3d0(const char *name) {
    strcpy(DAT_007cad60.name, name);
    DAT_007cad60.name_len = (unsigned char)strlen(name);
    DAT_007986f0 = 1;
}

// FUNCTION: LEGOLAND 0x0048e420
void FUN_0048e420(void) {
    FUN_0046d680((struct IconNode *)DAT_007986d8, DAT_00798678);
    FUN_0046d680((struct IconNode *)DAT_007986dc, DAT_0079867c);
}

// FUNCTION: LEGOLAND 0x0048e450
unsigned char FUN_0048e450(unsigned int a1, unsigned int a2) {
    if ((a2 & 2) != 0 && DAT_0080ffe4 != 0) {
        CloseFontEndCheckBox();
        DAT_007986e4 = 0;
        RemoveSaveGame(DAT_0080ffe4);
        DAT_0080ff80.unk4 = 0xffffffffu;
        DAT_0080ffe4 = 0;
    }
    return 1;
}

struct SaveIcon {
    unsigned char pad_0[0x1c];
    unsigned char field_1c;
};

// FUNCTION: LEGOLAND 0x0048e4a0
unsigned char FUN_0048e4a0(struct SaveIcon *icon, unsigned int flags, unsigned int a3, unsigned int a4) {
    if (DAT_004bef9c != 0 && (flags & 2)) {
        if (DAT_007cb328 != 0) {
            DAT_0080ffe4 = icon->field_1c;
        } else {
            return FUN_0048e4f0(icon, flags, a3, a4);
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048e4f0
unsigned char FUN_0048e4f0(struct SaveIcon *icon, unsigned int a2, unsigned int a3, unsigned int a4) { STUB(); }

struct EditSprite {
    unsigned char pad_0[0xc];
    short field_c;
    short field_e;
};

// FUNCTION: LEGOLAND 0x0048e550
LEGO_EXPORT void EnterSaveGameDetails(struct EditSprite *sprite) {
    char cursor_str[10];
    unsigned char count;
    char input;
    int left;
    int right;
    int bottom;
    int center_x;
    int top;
    char *blink;
    struct EditSprite *focus;

    *(short *)cursor_str = *(short *)"|";
    count = DAT_007cad60.name_len;
    cursor_str[2] = count;
    input = GetInputChar();
    if (input != '\0') {
        if (input == -1 && count != 0) {
            count--;
            cursor_str[2] = count;
            ((char *)&DAT_007cad60)[count] = 0;
        }
        if (count < 0x1f && DAT_00798738 < 0xcb) {
            if (input > '\0') {
                unsigned int index = (unsigned int)(unsigned char)cursor_str[2];
                count++;
                cursor_str[2] = count;
                ((char *)&DAT_007cad60)[index] = input;
                ((char *)&DAT_007cad60)[count] = 0;
            } else if (input == ' ') {
                if (count != 0) {
                    unsigned int index = (unsigned int)(unsigned char)cursor_str[2];
                    count++;
                    cursor_str[2] = count;
                    ((char *)&DAT_007cad60)[index] = ' ';
                    ((char *)&DAT_007cad60)[count] = 0;
                }
            }
        }
    }
    left = sprite->field_c + 0x28;
    bottom = sprite->field_e + 0x35;
    right = sprite->field_c + 0xff;
    if (count != 0) {
        RECT rc;
        rc.left = left;
        rc.top = sprite->field_e + 0x24;
        rc.right = right;
        rc.bottom = bottom;
        center_x = FUN_00491e40(DAT_007cad60.name, 2, rc, 1);
    } else {
        center_x = (right + left) >> 1;
    }
    DAT_00798738 = (center_x - ((right + left) >> 1)) * 2;
    top = sprite->field_e + 0x20;
    blink = "|";
    if (GetBlink() == 0) {
        blink = " ";
    }
    strcpy(cursor_str, blink);
    {
        RECT rc;
        rc.left = center_x;
        rc.top = top;
        rc.right = center_x + 100;
        rc.bottom = bottom;
        FUN_00490fa0(cursor_str, 2, rc, 1);
    }
    DAT_007cad60.name_len = count;
    focus = (struct EditSprite *)DAT_007986d8;
    if (focus->field_c + 0x48 < (int)DAT_00813a44.x || (int)DAT_00813a44.x < focus->field_c) {
        FUN_0048e420();
    }
    if (focus->field_e + 0x1b < (int)DAT_00813a44.y || (int)DAT_00813a44.y < focus->field_e) {
        FUN_0048e420();
    }
}

// FUNCTION: LEGOLAND 0x0048e720
int FUN_0048e720(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4) {
    STUB();
    return 0;
}

// FUNCTION: LEGOLAND 0x0048e810
unsigned char FUN_0048e810(struct IconNode *icon, unsigned char flags) {
    FUN_0048e420();
    if (icon == 0) {
        icon = (struct IconNode *)DAT_007986dc;
    }
    FUN_0046d680(icon, DAT_00798680);
    if (flags & 2) {
        DAT_0080ffe4 = 0;
        RemoveIconGroup(7);
        InitSavedGameScreen();
        DAT_00798700 = 0;
        DAT_004bef9c = 1;
        FUN_0048d490();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048e870
LEGO_EXPORT unsigned char StoreNewSaveGameToDisk(void) {
    char header_path[200];
    char save_path[256];
    void *file;

    sprintf(save_path, "%s\\%dsave%d.sav", "profiles", DAT_0080ffe3, DAT_0080ffe4 & 0xff);
    FUN_00466360(0, 0);
    FUN_0047f810();
    if (SaveGame(save_path) == 0) {
        // STRING: LEGOLAND 0x004bf360
        FUN_00453ce0("Failed to save game %s", save_path);
        DAT_0080ffe4 = 0;
        remove(save_path);
        FUN_004663c0();
        return 0xff;
    }
    FUN_004663c0();

    DAT_007cad60.field_24 = DAT_0080ffe5;
    DAT_007cad60.field_20 = DAT_0080ffa0.field_20;
    DAT_007cad60.field_28 = DAT_0080ffa0.field_24;
    DAT_007cad60.field_2c = DAT_0080ffa0.field_28;
    DAT_007cad60.field_30 = DAT_0080ffa0.field_2c;

    sprintf(header_path, "profiles\\%dsave%d.sh", DAT_0080ffe3, DAT_0080ffe4 & 0xff);
    if (!Goto_ProfileDir()) {
        // STRING: LEGOLAND 0x004bf33c
        FUN_00453ce0("Failed to move to profile folder");
        return 0xff;
    }

    // STRING: LEGOLAND 0x004beb70
    file = fopen(header_path, "w+");
    if (file == 0) {
        // STRING: LEGOLAND 0x004bf320
        FUN_00453ce0("\ncannot open output file %s", header_path);
    } else {
        if (fwrite(&DAT_007cad60, sizeof(struct ProfileData), 1, file) == 0) {
            // STRING: LEGOLAND 0x004bf2fc
            FUN_00453ce0("Failed to write to save header %s", header_path);
        }
        fclose(file);
    }

    if (!ReturnFrom_ProfileDir()) {
        return 0xff;
    }
    // STRING: LEGOLAND 0x004bf2f0
    FUN_00453ce0("Saved OK %s", header_path);
    return 1;
}

// FUNCTION: LEGOLAND 0x0048ea10
LEGO_EXPORT void RemoveSaveGame(unsigned char slot) {
    char path[132];

    if (!Goto_ProfileDir()) {
        return;
    }

    // STRING: LEGOLAND 0x004b9164
    sprintf(path, "%s\\%dsave%d.sav", "profiles", DAT_0080ffe3, slot);
    if (remove(path) != 0) {
        // STRING: LEGOLAND 0x004bf3b0
        DBPrintf("Failed to delete saved game %s\n", path);
    }

    // STRING: LEGOLAND 0x004bf3a0
    sprintf(path, "%s\\%dsave%d.sh", "profiles", DAT_0080ffe3, slot);
    if (remove(path) != 0) {
        // STRING: LEGOLAND 0x004bf378
        DBPrintf("Failed to delete saved game Header %s\n", path);
    }

    ReturnFrom_ProfileDir();
}

// FUNCTION: LEGOLAND 0x0048eac0
int FUN_0048eac0(int param) {
    return (100 * (param - 124)) / 241;
}

// FUNCTION: LEGOLAND 0x0048eaf0
int FUN_0048eaf0(int param) {
    return (241 * param) / 100 + 124;
}

// FUNCTION: LEGOLAND 0x0048eb20
void FUN_0048eb20(void) {
    DAT_00798740 = DAT_006687bc;
    DAT_0079873c = DAT_006687c0;
}
