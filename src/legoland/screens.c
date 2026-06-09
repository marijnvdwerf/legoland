#include "legoland.h"
#include "crt.h"
#include "globals.h"

#include "gamemap.h"
#include "bricks.h"
#include "build.h"
#include "clipping.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "profile_io.h"
#include "screens.h"
#include "popupinfo.h"
#include "draw.h"
#include "nerps.h"
#include "objclass.h"
#include "bloke.h"
#include "worker.h"
#include "icon.h"
#include "interface.h"
#include "map_object.h"
#include "string.h"
#include "sound_sfx.h"
#include "sound_music.h"
#include "saveload.h"
#include "mapscreen.h"
#include "profile.h"
#include "savegame_ui.h"
#include "options.h"
#include "progress.h"
#include "freeplay.h"
#include "title.h"
#include "help.h"
#include "controller.h"

struct ScreenConfig {
    unsigned short width;
    unsigned short height;
    unsigned char pad_4[0x28 - 0x4];
    unsigned int slot;
};

#include "image_sprite.h"
#include "stream.h"

void RenderFrontEndScreen(unsigned char param_1);
void FUN_00458ee0(void);
void FUN_00459360(void);


// FUNCTION: LEGOLAND 0x004585c0
void FUN_004585c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00458640
LEGO_EXPORT void InitScreens(unsigned char param_1) {
    int id = (char)param_1;

    if (DAT_0080ff84 != (unsigned int)id) {
        FUN_00498920();
        DAT_006687b0 = 4;
        FUN_004585c0();
        DAT_0080ff80 = 0;
        switch (id) {
        case 0:
            InitListProfiles();
            break;
        case 1:
            DeletePlayableSamples(0);
            InitTitleScreen();
            break;
        case 5:
            DAT_00667c78 = 1;
            FUN_00492830();
            InitOptionScreen();
            break;
        case 4:
            InitSavedGameScreen();
            break;
        case 3:
            InitFreePlayScreen();
            break;
        case 6:
            DeletePlayableSamples(0);
            InitProgressScreen();
            break;
        case 7:
            DAT_00667c78 = 1;
            FUN_00492830();
            FUN_00490c70();
            break;
        case 8:
            FUN_00490350();
            break;
        case 9:
            FUN_00490150();
            break;
        default:
            break;
        }
        DAT_0080ff84 = id;
    }
    if (SPRITE_TitleScreenBk != 0) {
        RenderFrontEndScreen(param_1);
    }
}

// FUNCTION: LEGOLAND 0x00458740
void RenderFrontEndScreen(unsigned char param_1) {
    ResetHitInfo();
    PushRenderingStatusAndLockVideoSurface();
    PrintSprite(SPRITE_TitleScreenBk, 0, 0, 0, 0);
    if (DAT_00668e38 != 0) {
        RenderIcons2(7, 0xe, 0);
        if (DAT_0080ff88 == 6) {
            RenderIcons2(0x1c, 0x23, 0);
        }
    } else {
        RenderIcons();
    }
    switch (DAT_0080ff88) {
    case 0:
        PrintProfileDetails();
        break;
    case 4:
        PrintSavedGameDetails();
        break;
    case 1:
    case 5:
        PrintExitCheckBox();
        break;
    case 7:
        FUN_004910f0();
        break;
    case 8:
        FUN_00490410();
        break;
    case 6:
        FUN_0048c100();
        break;
    default:
        break;
    }
    FUN_0048fc00();
    ProcessFrontEndHelp();
    UpdateFocussedIconPtr();
    PopRenderingStatus();
    if (FocussedIconPtr != 0) {
        SetPointer(6);
    }
    CheckFocussedIcon();
    RenderingComplete();
}

// FUNCTION: LEGOLAND 0x00458830
void FUN_00458830(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004588c0
void FUN_004588c0(void)
{
    struct ScreenConfig *config;
    struct Sprite *sprite;

    config = (struct ScreenConfig *)lpConfig;
    DAT_007fe020[0] = 0;
    DAT_007fe020[1] = 0;
    DAT_007fe020[2] = config->width;
    DAT_007fe020[3] = config->height;

    // STRING: LEGOLAND 0x004b913c
    sprite = LoadSprite("TitleScreen1.lls", 0);
    PushRenderingStatusAndLockVideoSurface();
    PrintSprite(sprite, 0, 0, 0, 0);
    PopRenderingStatus();
    RenderingComplete();

    if (sprite != NULL) {
        KillSprite(sprite);
    }
}

// FUNCTION: LEGOLAND 0x00458930
void FUN_00458930(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00458940
void FUN_00458940(void)
{
    EditMode = 0;
    DAT_008119b4 = 3;
    FUN_00474880();
    DAT_008119b8 = 0;
    DefaultCursor(&EditCursor);
    BuildCursorPtr(&EditCursor, 0x8f8, 0);
    GamePad = (GamePad & 0xffff0000) | ((((GamePad >> 8) & 0xeb) << 8) | ((GamePad & 0xff) | 0x20));
}

// FUNCTION: LEGOLAND 0x004589a0
void FUN_004589a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00458a50
void FUN_00458a50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00458b20
void FUN_00458b20(void)
{
    if (DAT_00667c7c != 0) {
        FUN_00481170();
        FUN_0046fb40(0xd2);
        DelObjectList();
        FreeBlokeCounters();
        FUN_0048a040();
        DAT_00667cd8 = 1;
        FUN_0046cb20();
        DAT_00667cd8 = 0;
        FUN_00457870(0);
        FUN_00489ee0();
        FUN_00483090();
        FUN_004714e0();
        ResetInfoStruct();
        ClearBuildObjList();
        FUN_0049cfc0();
        FUN_00474ed0();
        FUN_00457870(1);
        GamePad = (GamePad & 0xffff00ff) | (((GamePad >> 8) & 0xeb) << 8);
        DAT_00667c7c = 0;
    }
}

// FUNCTION: LEGOLAND 0x00458bb0
void FUN_00458bb0(unsigned int param_1) { DAT_00667c7c = param_1; }

// FUNCTION: LEGOLAND 0x00458bc0
void FUN_00458bc0(void)
{
    DAT_008119b4 = 2;
    DAT_0080ff84 = 0xffffffff;
    DAT_0080ff88 = 0;
}

// FUNCTION: LEGOLAND 0x00458be0
void FUN_00458be0(void)
{
    int slot = ((struct ScreenConfig *)lpConfig)->slot;

    if (slot < 0xf) {
        DAT_0080ffa0.field_34[slot] = 1;
    }
    FUN_0048a750();
    UpDateCurrentProfile();
}

// FUNCTION: LEGOLAND 0x00458c00
int FUN_00458c00(void) {
    char path[256];

    FUN_00498b40();
    if (DAT_008119bc != 0) {
        FUN_00499380();
        FUN_00492830();
        DAT_006687bc = 0;
        GamePad = GamePad & 0xffffffdf;
        DAT_006687c0 = 0;
        InitMapScreen();
        DisableInfoPopUPIcons();
        DisableSidePanelIcons();
        DAT_008119bc = 0;
    } else if (DAT_0080ff70 != 0) {
        FUN_00492850();
        DAT_006687bc = 0;
        GamePad = GamePad | 0x20;
        DAT_006687c0 = 0;
        KillMapScreen();
        FUN_00474880();
        EnableSidePanelIcons();
        DAT_0080ff70 = 0;
        FUN_004993c0();
    } else if (DAT_00667c64 != 0) {
        FUN_00499380();
        FUN_00499410();
        FUN_0047f810();
        DAT_006687bc = 0;
        DAT_006687c0 = 0;
        DAT_0079a8d0 = 0;
        DAT_00667c78 = 1;
        FUN_00492830();
        if (DAT_00667c80 != 0) {
            DeletePlayableSamples(0);
            sprintf(path, "%s\\%dsave%d.sav", "profiles", DAT_0080ffa0.field_43, DAT_0080ffa0.field_44 & 0xff);
            FUN_00466360(0, 0);
            LoadGame(path);
            DAT_00667c80 = 0;
            FUN_004663c0();
            InitGameInterface(0);
            FUN_00474880();
        } else {
            FUN_0046c5c0();
            FUN_00458a50();
        }
        UpdateSoundVols();
        DAT_00667c64 = 0;
        DAT_00832ba0 = 0;
        DAT_008119b4 = 3;
        FUN_00474880();
        FUN_004993c0();
    }

    switch (DAT_008119b4) {
    case 2:
        SetPointer(5);
        InitScreens(DAT_0080ff88 & 0xff);
        break;
    case 3:
        if (DAT_00667c78 != 0) {
            FUN_00492850();
            DAT_00667c78 = 0;
        }
        FUN_00458ee0();
        break;
    case 1:
        FUN_00459360();
        break;
    case 0:
        return 0;
    default:
        break;
    }

    if (DAT_00667c64 == 0) {
        // STRING: LEGOLAND 0x004b9160
        DAT_00667c40 = "SFX";
        FUN_004969d0();
        ReadGameButtons();
        DAT_008119a4 = DAT_008119a4 + 1;
        if (DAT_00832ba0 != 0 && DAT_008119b4 == 3) {
            if (DAT_00832bac != 0) {
                SetPointer(0);
                FUN_004771f0(DAT_008100c0, 1, 1);
                DAT_00832bac = 0;
                SetPointer(5);
            }
            if (DAT_00832ba0 != 3) {
                if (DAT_00832ba0 == 1) {
                    ((struct ScreenConfig *)lpConfig)->slot += 1;
                }
                FUN_0048a750();
                FUN_00458b20();
                DAT_00668e38 = 1;
                DAT_008119b4 = 2;
                DAT_0080ff84 = 0xffffffff;
                DAT_0080ff88 = 6;
                FUN_0046d110();
                return 1;
            }
            DAT_00668e38 = 0;
            DAT_008119b4 = 2;
            DAT_0080ff84 = 0xffffffff;
            DAT_0080ff88 = 1;
            FUN_00458b20();
        }
        FUN_0046d110();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00458ee0
void FUN_00458ee0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00459360
void FUN_00459360(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004594e0
void FUN_004594e0(void)
{
    switch (DAT_008119b4) {
    case 2:
        FUN_004585c0();
        break;
    default:
        break;
    }
}

// FUNCTION: LEGOLAND 0x004594f0
void FUN_004594f0(void)
{
    int i;

    for (i = 0; i < 68; i = i + 1) {
        ((unsigned int *)&DAT_0080ffa0)[i] = 0;
    }

    DAT_0080ffa0.field_24 = 0x4b;
    DAT_0080ffa0.field_28 = 0x64;
    DAT_0080ffa0.field_2c = 0x4b;
}

// FUNCTION: LEGOLAND 0x00459520
void FUN_00459520(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00459710
void FUN_00459710(char *s) { STUB(); }

// FUNCTION: LEGOLAND 0x004597e0
void FUN_004597e0(int param0, const char *param1)
{
    char *buffer;

    if (param0) {
        buffer = DAT_00832998;
    } else {
        buffer = DAT_00832a98;
    }

    if (param1) {
        strncpy(buffer, param1, 256);
        buffer[255] = 0;
    } else {
        buffer[0] = 0;
    }
}

// FUNCTION: LEGOLAND 0x00459820
void FUN_00459820(unsigned int a1)
{
    DAT_00832ba0 = a1;
    if (a1 == 1) {
        FUN_00459710(DAT_00832998);
        return;
    }
    if (a1 == 2) {
        FUN_00459710(DAT_00832a98);
    }
}
