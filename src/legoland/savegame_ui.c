#include "legoland.h"
#include "crt.h"
#include "globals.h"

#include "profile.h"
#include "profile_io.h"
#include "savegame_ui.h"
#include "icon.h"
#include "sound_music.h"

#include "image_sprite.h"

struct SaveNode {
    struct SaveNode *next;
};

// FUNCTION: LEGOLAND 0x0048d4b0
void InitSavedGameScreen(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048d8f0
int LoadDateIntoTempProfile(int a1, int a2) {
    char buffer[132];
    void *file;

    // STRING: LEGOLAND 0x004bf2bc
    FUN_0049e573(buffer, "profiles\\%dsave%d.sh", a1, a2);

    if (!Goto_ProfileDir()) {
        return 0;
    }

    // STRING: LEGOLAND 0x004bf2b8
    file = FUN_0049f330(buffer, "r");
    if (file != 0) {
        FUN_0049f044(&DAT_007cad60, 0x110, 1, file);
        FUN_0049efee(file);
        ReturnFrom_ProfileDir();
        return 1;
    }

    return 0;
}

// FUNCTION: LEGOLAND 0x0048d970
void FUN_0048d970(void) { STUB(); }

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
void GetSavePanelBK(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048dbc0
void KillSaveScreenSprites(void) {
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
void PrintSavedGameDetails(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048e0c0
void FUN_0048e0c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048e160
void DeleteSavedGameList(void) {
    struct SaveNode *current = (struct SaveNode *)DAT_00798734;
    while (current != NULL) {
        struct SaveNode *next = current->next;
        FUN_0049e4d0(current);
        current = next;
    }
    DAT_00798734 = NULL;
}

// FUNCTION: LEGOLAND 0x0048e190
void LoadSavedGamesList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048e280
void InitNewSaveGamePOPUP(void) { STUB(); }

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

// FUNCTION: LEGOLAND 0x0048e550
void EnterSaveGameDetails(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048e720
void FUN_0048e720(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048e810
void FUN_0048e810(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048e870
void StoreNewSaveGameToDisk(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048ea10
void RemoveSaveGame(unsigned char slot) { STUB(); }

// FUNCTION: LEGOLAND 0x0048eac0
void FUN_0048eac0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048eaf0
void FUN_0048eaf0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048eb20
void FUN_0048eb20(void) {
    DAT_00798740 = DAT_006687bc;
    DAT_0079873c = DAT_006687c0;
}
