#include "legoland.h"
#include "crt.h"
#include "globals.h"

#include "profile.h"
#include "profile_io.h"
#include "savegame_ui.h"
#include "saveload.h"
#include "draw.h"
#include "main.h"
#include "debug_alloc.h"
#include "icon.h"
#include "sound_music.h"
#include "options.h"
#include "screens.h"
#include "title.h"
#include "input.h"
#include "timer.h"

#include "image_sprite.h"

#pragma intrinsic(memset, strcpy)

struct SaveNode {
    struct SaveNode *next;
    char name[0x110];
    int has_header;
    unsigned char slot;
};

LEGO_EXPORT void KillSaveScreenSprites(void);
LEGO_EXPORT void DeleteSavedGameList(void);

// FUNCTION: LEGOLAND 0x0048d4b0
LEGO_EXPORT void InitSavedGameScreen(void) { STUB(); }

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
        fread(&DAT_007cad60, 0x110, 1, file);
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
        if (LoadDateIntoTempProfile(DAT_0080ffa0.field_43, DAT_0080ffa0.field_44 & 0xff) != 0) {
            DAT_0080ffa0.field_45 = DAT_007cad60.field_24;
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
void FUN_0048da50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048db10
unsigned char FUN_0048db10(int param1, unsigned char flags) {
    if (DAT_004bef9c != 0 && (flags & 0x2)) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_0080ff88 = 5;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048db50
LEGO_EXPORT void GetSavePanelBK(void) { STUB(); }

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
void FUN_0048e0c0(int has_header, char *header, unsigned char slot) {
    struct SaveNode *node = (struct SaveNode *)malloc(0x11c);
    memset(node, 0, 0x11c);
    if (has_header != 0) {
        node->has_header = 1;
        strcpy(node->name, header);
        node->slot = slot;
        node->name[0x24] = header[0x24];
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
    char header[0x110];
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
            FUN_0048e0c0(0, header, slot);
        } else {
            fread(header, 0x110, 1, file);
            FUN_0048e0c0(1, header, slot);
            fclose(file);
        }
        memset(header, 0, 0x110);
        slot = slot - 1;
        n = n - 1;
    } while (slot != 0);

    rc = ReturnFrom_ProfileDir();
    return (rc != 0) - 1;
}

// FUNCTION: LEGOLAND 0x0048e280
LEGO_EXPORT void InitNewSaveGamePOPUP(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048e3d0
void FUN_0048e3d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048e420
void FUN_0048e420(void) {
    FUN_0046d680((struct IconNode *)DAT_007986d8, DAT_00798678);
    FUN_0046d680((struct IconNode *)DAT_007986dc, DAT_0079867c);
}

// FUNCTION: LEGOLAND 0x0048e450
unsigned char FUN_0048e450(unsigned int a1, unsigned int a2) {
    if ((a2 & 2) != 0 && DAT_0080ffa0.field_44 != 0) {
        CloseFontEndCheckBox();
        DAT_007986e4 = 0;
        RemoveSaveGame(DAT_0080ffa0.field_44);
        DAT_0080ff84 = 0xffffffffu;
        DAT_0080ffa0.field_44 = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048e4a0
void FUN_0048e4a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048e4f0
void FUN_0048e4f0(void) { STUB(); }

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
    count = DAT_007cad60.field_1e;
    cursor_str[2] = count;
    input = GetInputChar();
    if (input != '\0') {
        if (input == -1 && count != 0) {
            count--;
            cursor_str[2] = count;
            *((char *)&DAT_007cad60 + count) = 0;
        }
        if (count < 0x1f && DAT_00798738 < 0xcb) {
            if (input > '\0') {
                unsigned int index = (unsigned int)(unsigned char)cursor_str[2];
                count++;
                cursor_str[2] = count;
                *((char *)&DAT_007cad60 + index) = input;
                *((char *)&DAT_007cad60 + count) = 0;
            } else if (input == ' ') {
                if (count != 0) {
                    unsigned int index = (unsigned int)(unsigned char)cursor_str[2];
                    count++;
                    cursor_str[2] = count;
                    *((char *)&DAT_007cad60 + index) = ' ';
                    *((char *)&DAT_007cad60 + count) = 0;
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
        center_x = FUN_00491e40((char *)&DAT_007cad60, 2, rc, 1);
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
    DAT_007cad60.field_1e = count;
    focus = (struct EditSprite *)DAT_007986d8;
    if (focus->field_c + 0x48 < (int)DAT_00813a44 || (int)DAT_00813a44 < focus->field_c) {
        FUN_0048e420();
    }
    if (focus->field_e + 0x1b < (int)DAT_00813a48 || (int)DAT_00813a48 < focus->field_e) {
        FUN_0048e420();
    }
}

// FUNCTION: LEGOLAND 0x0048e720
void FUN_0048e720(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048e810
void FUN_0048e810(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048e870
LEGO_EXPORT unsigned char StoreNewSaveGameToDisk(void) {
    char header_path[200];
    char save_path[256];
    void *file;

    // STRING: LEGOLAND 0x004b9174
    sprintf(save_path, "%s\\%dsave%d.sav", "profiles", DAT_0080ffa0.field_43, DAT_0080ffa0.field_44 & 0xff);
    FUN_00466360(0, 0);
    FUN_0047f810();
    if (SaveGame(save_path) == 0) {
        // STRING: LEGOLAND 0x004bf360
        FUN_00453ce0("Failed to save game %s", save_path);
        DAT_0080ffa0.field_44 = 0;
        _rmdir(save_path);
        FUN_004663c0();
        return 0xff;
    }
    FUN_004663c0();

    DAT_007cad60.field_24 = DAT_0080ffa0.field_45;
    DAT_007cad60.field_20 = DAT_0080ffa0.field_20;
    DAT_007cad60.field_28 = DAT_0080ffa0.field_24;
    DAT_007cad60.field_2c = DAT_0080ffa0.field_28;
    DAT_007cad60.field_30 = DAT_0080ffa0.field_2c;

    sprintf(header_path, "profiles\\%dsave%d.sh", DAT_0080ffa0.field_43, DAT_0080ffa0.field_44 & 0xff);
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
        if (fwrite(&DAT_007cad60, 0x110, 1, file) == 0) {
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
    sprintf(path, "%s\\%dsave%d.sav", "profiles", DAT_0080ffa0.field_43, slot);
    if (_rmdir(path) != 0) {
        // STRING: LEGOLAND 0x004bf3b0
        DBPrintf("Failed to delete saved game %s\n", path);
    }

    // STRING: LEGOLAND 0x004bf3a0
    sprintf(path, "%s\\%dsave%d.sh", "profiles", DAT_0080ffa0.field_43, slot);
    if (_rmdir(path) != 0) {
        // STRING: LEGOLAND 0x004bf378
        DBPrintf("Failed to delete saved game Header %s\n", path);
    }

    ReturnFrom_ProfileDir();
}

// FUNCTION: LEGOLAND 0x0048eac0
void FUN_0048eac0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048eaf0
void FUN_0048eaf0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048eb20
void FUN_0048eb20(void) {
    DAT_00798740 = DAT_006687bc;
    DAT_0079873c = DAT_006687c0;
}
