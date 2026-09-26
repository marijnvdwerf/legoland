#include <stdio.h>
#include <string.h>
#pragma intrinsic(strcpy, strlen)
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "bricks.h"
#include "build.h"
#include "clipping.h"
#include "controller.h"
#include "draw.h"
#include "freeplay.h"
#include "gamemap.h"
#include "help.h"
#include "icon.h"
#include "interface.h"
#include "llidb.h"
#include "map_object.h"
#include "mapscreen.h"
#include "math.h"
#include "nerps.h"
#include "obj_instance.h"
#include "objclass.h"
#include "options.h"
#include "popupinfo.h"
#include "print_sprite.h"
#include "profile.h"
#include "profile_io.h"
#include "progress.h"
#include "savegame_ui.h"
#include "saveload.h"
#include "screens.h"
#include "sound_music.h"
#include "sound_sfx.h"
#include "string.h"
#include "timer.h"
#include "title.h"
#include "worker.h"

struct ScreenConfig {
    unsigned short width;
    unsigned short height;
    unsigned char pad_4[0x28 - 0x4];
    unsigned int slot;
};

#include "image_sprite.h"
#include "stream.h"

// FUNCTION: LEGOLAND 0x004585c0
void FUN_004585c0(void) {
    if (SPRITE_TitleScreenBk != NULL) {
        KillSprite(SPRITE_TitleScreenBk);
        SPRITE_TitleScreenBk = NULL;
    }
    RemoveIconGroup(7);
    if (DAT_0080ff80.unk4 != 0xffffffff) {
        switch (DAT_0080ff80.unk4) {
        case 0:
            FUN_0048d230();
            UpdateSoundVols();
            DeleteProfileList();
            KillListProfileSprite();
            break;
        case 3:
            CleanUpFreePlay();
            break;
        case 4:
            KillSaveScreenSprites();
            KillTitleScreenSprites();
            DeleteSavedGameList();
            break;
        default:
            break;
        }
        DAT_006687bc = 0;
        DAT_006687c0 = 0;
    }
}

// FUNCTION: LEGOLAND 0x00458640
LEGO_EXPORT void InitScreens(unsigned char param_1) {
    int id = (char)param_1;

    if (DAT_0080ff80.unk4 != (unsigned int)id) {
        FUN_00498920();
        DAT_006687b0 = 4;
        FUN_004585c0();
        DAT_0080ff80.unk0 = 0;
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
        DAT_0080ff80.unk4 = id;
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
        if (DAT_0080ff80.unk8 == 6) {
            RenderIcons2(0x1c, 0x23, 0);
        }
    } else {
        RenderIcons();
    }
    switch (DAT_0080ff80.unk8) {
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
void FUN_00458830(unsigned int param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x004588c0
void FUN_004588c0(void) {
    struct ScreenConfig *config;
    struct Sprite *sprite;

    config = (struct ScreenConfig *)lpConfig;
    DAT_007fe020.left = 0;
    DAT_007fe020.top = 0;
    DAT_007fe020.right = config->width;
    DAT_007fe020.bottom = config->height;

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
int _ftol() {
    __asm fistp dword ptr[DAT_00667c3c] __asm mov eax, [DAT_00667c3c]
}

// FUNCTION: LEGOLAND 0x00458940
void FUN_00458940(void) {
    EditMode.unk0 = 0;
    EditMode.unk4 = 3;
    FUN_00474880();
    EditMode.unk8 = 0;
    DefaultCursor(&EditCursor);
    BuildCursorPtr(&EditCursor, 0x8f8, 0);
    GamePad = (GamePad & 0xffff0000) | ((((GamePad >> 8) & 0xeb) << 8) | ((GamePad & 0xff) | 0x20));
}

// FUNCTION: LEGOLAND 0x004589a0
void FUN_004589a0(void) {
    unsigned int work[3];

    CONTROLLERBUFFER->field_0 = 0;
    CONTROLLERBUFFER->field_4 = 0;
    CONTROLLERBUFFER->field_8 = 0;
    CONTROLLERBUFFER->field_c = 0;
    CONTROLLERBUFFER->field_10 = 0;
    CONTROLLERBUFFER->field_14 = 0;
    CONTROLLERBUFFER->field_8 = lpConfig->field_0 >> 1;
    CONTROLLERBUFFER->field_c = lpConfig->field_2 >> 1;
    SystemParametersInfoA(3, 0, work, 0);
    CONTROLLERBUFFER->field_1c = work[0];
    CONTROLLERBUFFER->field_20 = work[2];
    CONTROLLERBUFFER->field_24 = work[3];
    SetPointer(5);
}

// FUNCTION: LEGOLAND 0x00458a50
void FUN_00458a50(void) {
    char buf[0x34];

    if (DAT_00667c7c == 0) {
        QueryClass = 0;
        DAT_0079a8d0 = 0;
        FUN_00499380();
        FUN_00499410();
        FUN_0047f810();
        // STRING: LEGOLAND 0x004b9150
        sprintf(buf, "objlist%d.txt", lpConfig->field_28);
        FUN_00457870(0);
        ResetMapAI();
        DAT_00667c4c = FUN_0047afb0(buf);
        FUN_00457870(1);
        AllocBlokeCounters(lpConfig->field_1a);
        FUN_00458940();
        FUN_00489ee0();
        MapStats.field_3a0 = 0;
        UpdateMenu();
        FUN_00490600(1);
        FUN_004911c0(DAT_0066861c, 0);
        DAT_00667c7c = 1;
        FUN_004993c0();
        FUN_0048a800();
    }
}

// FUNCTION: LEGOLAND 0x00458b20
void FUN_00458b20(void) {
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
void FUN_00458bc0(void) {
    EditMode.unk4 = 2;
    DAT_0080ff80.unk4 = 0xffffffff;
    DAT_0080ff80.unk8 = 0;
}

// FUNCTION: LEGOLAND 0x00458be0
void FUN_00458be0(void) {
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
            sprintf(path, "%s\\%dsave%d.sav", "profiles", DAT_0080ffe3, DAT_0080ffa0.field_44 & 0xff);
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
        MapStats.field_3a0 = 0;
        EditMode.unk4 = 3;
        FUN_00474880();
        FUN_004993c0();
    }

    switch (EditMode.unk4) {
    case 2:
        SetPointer(5);
        InitScreens(DAT_0080ff80.unk8 & 0xff);
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
        if (MapStats.field_3a0 != 0 && EditMode.unk4 == 3) {
            if (MapStats.field_3ac != 0) {
                SetPointer(0);
                FUN_004771f0(DAT_008100c0, 1, 1);
                MapStats.field_3ac = 0;
                SetPointer(5);
            }
            if (MapStats.field_3a0 != 3) {
                if (MapStats.field_3a0 == 1) {
                    ((struct ScreenConfig *)lpConfig)->slot += 1;
                }
                FUN_0048a750();
                FUN_00458b20();
                DAT_00668e38 = 1;
                EditMode.unk4 = 2;
                DAT_0080ff80.unk4 = 0xffffffff;
                DAT_0080ff80.unk8 = 6;
                FUN_0046d110();
                return 1;
            }
            DAT_00668e38 = 0;
            EditMode.unk4 = 2;
            DAT_0080ff80.unk4 = 0xffffffff;
            DAT_0080ff80.unk8 = 1;
            FUN_00458b20();
        }
        FUN_0046d110();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00458ee0
void FUN_00458ee0(void) {
    struct {
        int outgoing[3];
        RECT help_rect;
    } frame;
    unsigned int input;
    char *value;
    unsigned int saved_value;
    unsigned int saved_action;
    unsigned int *class_data;
    struct ObjClass *query_class;

    // STRING: LEGOLAND 0x004b91e4
    DAT_00667c40 = "AI";
    HandleRideAI(1, 0, 0);
    DoMapAI();
    ControlPeople();
    ControlWorkers();
    if (DAT_00668954 != 0) {
        CheckWorkerOnMouseStatus(0);
    }
    // STRING: LEGOLAND 0x004b91d4
    DAT_00667c40 = "ProcessStuff";
    ProcessBuildingTimes();
    ProcessDamage();
    input = DAT_004bdd00;
    SetPointer(5);
    // STRING: LEGOLAND 0x004b91cc
    DAT_00667c40 = "Zoning";
    ResetHitInfo();
    // STRING: LEGOLAND 0x004b91c0
    DAT_00667c40 = "Rendering";
    PushRenderingStatusAndLockVideoSurface();
    RenderView();
    if ((input & 0x100) != 0 || (GamePad & 0x1000) != 0) {
        FUN_00457a70();
    }
    saved_value = DAT_004bdd04;
    saved_action = DAT_004bdd08;
    PrintSprite(DAT_00668e68, DAT_004bdd00, saved_value, saved_action, frame.outgoing);
    FUN_0046f100(0x2c3);
    FUN_0046ee00();
    // STRING: LEGOLAND 0x004b91b0
    DAT_00667c40 = "In Game Help";
    ProcessInGameHelp();
    DrawPopUpInfo();
    RenderIcons2(0x2c3, 0, 0);
    FUN_0046cff0();
    if ((GamePad & 0x1000) != 0) {
        DAT_004bdd00 = input;
        DAT_004bdd04 = saved_value;
        DAT_004bdd08 = saved_action;
    }
    input = DAT_004bdd00;
    if ((input & 0x100) != 0 && (EditMode.unk0 == 0 || EditMode.unk0 == 2)) {
        frame.help_rect.left = DAT_00813a44.x;
        frame.help_rect.top = DAT_00813a44.y - 0xa;
        frame.help_rect.right = DAT_00813a44.x;
        frame.help_rect.bottom = DAT_00813a44.y;
        if (EditMode.unk0 == 0) {
            if (input == 0x7e4) {
                value = GetString(0x7e4);
                HTBubbleHelp(&frame.help_rect, value, 2);
                FUN_0046d230(0x7e4);
                SetPointer(8);
            } else {
                switch (input) {
                case 0x103:
                    SetPointer(8);
                    query_class = QueryClass;
                    if (query_class != NULL) {
                        HTBubbleHelp(&frame.help_rect, query_class->name, 2);
                        class_data = query_class->field_c4;
                        FUN_0046d340(*class_data);
                    }
                    break;
                case 0x10a:
                    value = GetString(0xd4);
                    HTBubbleHelp(&frame.help_rect, value, 2);
                    FUN_0046d230(0xd4);
                    SetPointer(8);
                    break;
                case 0x10b:
                    value = GetString(0xd2);
                    HTBubbleHelp(&frame.help_rect, value, 2);
                    FUN_0046d230(0xd2);
                    SetPointer(7);
                    break;
                case 0x10c:
                    value = GetString(0xd3);
                    HTBubbleHelp(&frame.help_rect, value, 2);
                    FUN_0046d230(0xd3);
                    SetPointer(7);
                    break;
                case 0x306:
                    FUN_00455fc0(&frame.help_rect, GetVisitorName(DAT_004bdd04), 2, FUN_00482cb0(DAT_004bdd04));
                    FUN_00450a40(DAT_004bdd04);
                    SetPointer(8);
                    break;
                case 0x307:
                    value = GetString(0x90);
                    HTBubbleHelp(&frame.help_rect, value, 2);
                    FUN_0046d230(0x90);
                    SetPointer(8);
                    FUN_00450a40(DAT_004bdd04);
                    break;
                case 0x308:
                    value = GetString(0x92);
                    HTBubbleHelp(&frame.help_rect, value, 2);
                    FUN_0046d230(0x92);
                    SetPointer(8);
                    FUN_00450a40(DAT_004bdd04);
                    break;
                case 0x104:
                case 0x105:
                case 0x106:
                case 0x107:
                case 0x108:
                case 0x109:
                default:
                    SetPointer(7);
                    break;
                }
            }
        } else {
            query_class = QueryClass;
            if (query_class != NULL) {
                HTBubbleHelp(&frame.help_rect, query_class->name, 2);
                class_data = query_class->field_c4;
                FUN_0046d340(*class_data);
            }
        }
    }
    if (DAT_004bdd00 == 5 && (DAT_00813ac4 & 2) != 0) {
        FUN_0046ce20();
        DAT_00667c48 = 1;
    }
    // STRING: LEGOLAND 0x004b91a4
    DAT_00667c40 = "Appraisals";
    FUN_0044db90();
    // STRING: LEGOLAND 0x004b9194
    DAT_00667c40 = "Appraisals Over";
    UpdateFocussedIconPtr();
    if ((GamePad & 0x1000) == 0 && DAT_00667c48 == 0 && FocussedIconPtr != NULL && DAT_00668954 == 0) {
        SetPointer(6);
    }
    CheckFocussedIcon();
    if (DAT_00668954 != 0) {
        RenderWorkerOnMouse();
    }
    PopRenderingStatus();
    if (MapStats.field_194 != 0) {
        if (FUN_00474070() != 0 && FUN_00474080() != 0) {
            FUN_004632b0();
        }
    }
    RenderingComplete();
    // STRING: LEGOLAND 0x004b9180
    DAT_00667c40 = "Exiting GameProc";
}

// FUNCTION: LEGOLAND 0x00459360
void FUN_00459360(void) {
    unsigned int var_4;
    struct HitInfo hit_info;
    RECT clipping;
    RECT old_clipping;

    var_4 = 1;
    hit_info.field_0 = 0;
    hit_info.field_4 = 0;
    ResetHitInfo();
    PushRenderingStatusAndLockVideoSurface();
    DrawMapScreen();
    SetPointer(5);
    PrintSprite(DAT_00668e68, 0, 0, 0, &hit_info.field_0);
    FUN_0046ee00();
    RenderIcons();
    CheckFocussedIcon();
    if (DAT_004bdd00 == 2) {
        SetPointer(6);
        UpdateFocussedIconPtr();
        PopRenderingStatus();
        RenderingComplete();
        return;
    }
    if ((DAT_004bdd00 & 0x100) != 0) {
        GetClipping(&clipping);
        SetClipping(&old_clipping);
        RenderMouseBounds();
        SetClipping(&old_clipping);
        if ((DAT_00813ac4 & 1) != 0) {
            MapScreenSetScrollPos(&DAT_00813a44);
        }
        if ((DAT_00813ac4 & 2) != 0) {
            hit_info.field_8 = GetTicks();
            if (hit_info.field_8 - DAT_00667c68 < 0x1f4 && abs(DAT_00813a44.x - DAT_00667c70) < 5 &&
                abs(DAT_00813a44.y - DAT_00667c74) < 5) {
                DAT_0080ff70 = 1;
                EditMode.unk4 = DAT_00667c60;
                DAT_00667c60 = 1;
            }
            DAT_00667c70 = DAT_00813a44.x;
            DAT_00667c74 = DAT_00813a44.y;
            DAT_00667c68 = hit_info.field_8;
        }
    }
    UpdateFocussedIconPtr();
    PopRenderingStatus();
    RenderingComplete();
}

// FUNCTION: LEGOLAND 0x004594e0
void FUN_004594e0(void) {
    switch (EditMode.unk4) {
    case 2:
        FUN_004585c0();
        break;
    default:
        break;
    }
}

// FUNCTION: LEGOLAND 0x004594f0
void FUN_004594f0(void) {
    int i;

    for (i = 0; i < 68; i = i + 1) {
        ((unsigned int *)&DAT_0080ffa0)[i] = 0;
    }

    DAT_0080ffa0.field_24 = 0x4b;
    DAT_0080ffa0.field_28 = 0x64;
    DAT_0080ffa0.field_2c = 0x4b;
}

// FUNCTION: LEGOLAND 0x00459520
void FUN_00459520(void) {
    HMODULE ir50;
    MSG msg;

    // STRING: LEGOLAND 0x004b7138
    DAT_0081cd08 = ElemID("HEDGE")->data;
    FUN_004594f0();
    InitSoundSystem();
    SetMusicGrooveLevel(1);
    FUN_00492c60();
    SetupControllers();
    LLIDB_ClearOnLevel();
    FUN_004589a0();
    SetPointer(0);
    ProcessSystemEvents();
    // STRING: LEGOLAND 0x004b9200
    FUN_004771f0("lmi.avi", 0, 1);
    FUN_004588c0();
    FUN_00492c80();
    FUN_00466360(0, 0);

    while (DAT_007988bc == 0) {
        PeekMessageA(&msg, NULL, 0, 0, 0);
        Sleep(100);
        FUN_004663f0();
    }

    FUN_0046f890();
    LoadWorkerInterfaceGFX();
    LoadBubbleHelpGFX();
    InitialiseBlokes();
    InitGameMap();
    SetPointer(0);
    // STRING: LEGOLAND 0x004b91f4
    FUN_004771f0("Intro.avi", 1, 0);
    FUN_00492ca0(0);
    SetPointer(5);
    DAT_008119a4 = 0;
    Load_Interface_ControlIcons();
    FUN_004663f0();
    Load_Interface_ThemeIcons();
    FreeTileSpace(0, 0x800);
    EditMode.unk4 = 3;
    LoadMapTiles();
    FUN_004663f0();
    InitMan();
    FUN_004663f0();
    CreateObjectClasses();
    FUN_004663f0();
    FUN_00458bc0();
    FUN_004663f0();
    // STRING: LEGOLAND 0x004b91e8
    ir50 = LoadLibraryA("Ir50_32.dll");
    FUN_004663f0();
    FUN_00444090();
    FUN_004663f0();
    FUN_004663c0();
    FUN_00492c80();
    while (FUN_00458c00() != 0) {
    }
    FUN_004594e0();
    FUN_00498920();
    if (SPRITE_TitleScreenBk != NULL) {
        KillSprite(SPRITE_TitleScreenBk);
        SPRITE_TitleScreenBk = NULL;
    }
    FUN_00451f40();
    FUN_0046f920();
    FUN_00454a10();
    FUN_00482ec0();
    KillGameMap();
    UnLoad_Interface_ControlIcons();
    UnLoad_Interface_ThemeIcons();
    FUN_0045ac20();
    UnInitMan();
    FUN_00444150();
    FreeLibrary(ir50);
    FreeBlokeCounters();
    KillHelp();
    KillSoundSystem();
    KillInputSystem();
}

// FUNCTION: LEGOLAND 0x00459710
void FUN_00459710(char *s) {
    char buffer[0x80];
    char *semicolon;
    char *source;

    source = s;
    semicolon = strchr(source, ';');
    if (semicolon != NULL) {
        *semicolon = 0;
        strcpy(buffer, source);
        semicolon++;
        if (strlen(semicolon) != 0) {
            strcpy(DAT_008100c0, semicolon);
            MapStats.field_3ac = 1;
        }
        semicolon[-1] = ';';
    }
    FUN_00490600(0);
    if (FUN_004907a0(buffer) != 0) {
        DAT_00668e38 = 1;
        EditMode.unk4 = 2;
        DAT_0080ff80.unk4 = 0xffffffff;
        DAT_0080ff80.unk8 = 7;
    }
}

// FUNCTION: LEGOLAND 0x004597e0
void FUN_004597e0(int param0, const char *param1) {
    char *buffer;

    if (param0) {
        buffer = MapStats.field_198;
    } else {
        buffer = MapStats.field_298;
    }

    if (param1) {
        strncpy(buffer, param1, 256);
        buffer[255] = 0;
    } else {
        buffer[0] = 0;
    }
}

// FUNCTION: LEGOLAND 0x00459820
void FUN_00459820(unsigned int a1) {
    MapStats.field_3a0 = a1;
    if (a1 == 1) {
        FUN_00459710(MapStats.field_198);
        return;
    }
    if (a1 == 2) {
        FUN_00459710(MapStats.field_298);
    }
}
