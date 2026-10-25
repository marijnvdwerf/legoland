#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma intrinsic(strcpy, strlen)
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "bloke_ai.h"
#include "bricks.h"
#include "build.h"
#include "challenge.h"
#include "clipping.h"
#include "controller.h"
#include "draw.h"
#include "freeplay.h"
#include "game_util.h"
#include "gamemap.h"
#include "help.h"
#include "icon.h"
#include "input.h"
#include "interface.h"
#include "llidb.h"
#include "man3d.h"
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
#include "text.h"
#include "tilemap.h"
#include "timer.h"
#include "title.h"
#include "tooltip.h"
#include "wndenv.h"
#include "worker.h"

struct ScreenConfig {
    unsigned short width;
    unsigned short height;
    unsigned char pad_4[0x28 - 0x4];
    unsigned int slot;
};

#include "image_sprite.h"
#include "stream.h"
#include "worker_mouse.h"

// FUNCTION: LEGOLAND 0x004585c0
void CloseFrontEndScreen(void) {
    if (SPRITE_TitleScreenBk != NULL) {
        KillSprite(SPRITE_TitleScreenBk);
        SPRITE_TitleScreenBk = NULL;
    }
    RemoveIconGroup(7);
    if (FrontEndScreenMode.unk4 != 0xffffffff) {
        switch (FrontEndScreenMode.unk4) {
        case 0:
            LoadCurrentProfileFromList();
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
        OkKeyHandler = 0;
        CancelKeyHandler = 0;
    }
}

// FUNCTION: LEGOLAND 0x00458640
LEGO_EXPORT void InitScreens(unsigned char param_1) {
    int id = (char)param_1;

    if (FrontEndScreenMode.unk4 != (unsigned int)id) {
        SpeechCloseFile();
        SpeechDelayFrames = 4;
        CloseFrontEndScreen();
        FrontEndScreenMode.unk0 = 0;
        switch (id) {
        case 0:
            InitListProfiles();
            break;
        case 1:
            DeletePlayableSamples(0);
            InitTitleScreen();
            break;
        case 5:
            SamplesPaused = 1;
            PauseAllSamples();
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
            SamplesPaused = 1;
            PauseAllSamples();
            InitIntervalScreen();
            break;
        case 8:
            InitCertificateScreen();
            break;
        case 9:
            InitAdvertScreen();
            break;
        default:
            break;
        }
        FrontEndScreenMode.unk4 = id;
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
    if (FrontEndSubScreenActive != 0) {
        RenderIcons2(7, 0xe, 0);
        if (FrontEndScreenMode.unk8 == 6) {
            RenderIcons2(0x1c, 0x23, 0);
        }
    } else {
        RenderIcons();
    }
    switch (FrontEndScreenMode.unk8) {
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
        RenderIntervalScreen();
        break;
    case 8:
        RenderCertificateScreen();
        break;
    case 6:
        DrawTutorialList();
        break;
    default:
        break;
    }
    PlayPendingFrontEndSpeech();
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
void GetProductVersion(char *param_1) {
    DWORD size;
    DWORD handle;
    char *value;
    char *buf;

    // STRING: LEGOLAND 0x004b912c
    size = GetFileVersionInfoSizeA("Legoland.exe", &handle);
    buf = (char *)malloc(size);
    GetFileVersionInfoA("Legoland.exe", handle, size, buf);
    // STRING: LEGOLAND 0x004b9104
    VerQueryValueA(buf, "\\StringFileInfo\\080904B0\\ProductVersion", (void **)&value, (UINT *)&size);
    strcpy(param_1, value);
    free(buf);
}

// FUNCTION: LEGOLAND 0x004588c0
void PrintTitleScreen1(void) {
    struct ScreenConfig *config;
    struct Sprite *sprite;

    config = (struct ScreenConfig *)lpConfig;
    ScreenRect.left = 0;
    ScreenRect.top = 0;
    ScreenRect.right = config->width;
    ScreenRect.bottom = config->height;

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
    __asm fistp dword ptr[FtolScratch] __asm mov eax, [FtolScratch]
}

// FUNCTION: LEGOLAND 0x00458940
void ResetEditState(void) {
    EditMode.unk0 = 0;
    EditMode.unk4 = 3;
    SetDefaultMouseHandlers();
    EditMode.unk8 = 0;
    DefaultCursor(&EditCursor);
    BuildCursorPtr(&EditCursor, 0x8f8, 0);
    GamePad = (GamePad & 0xffff0000) | ((((GamePad >> 8) & 0xeb) << 8) | ((GamePad & 0xff) | 0x20));
}

// FUNCTION: LEGOLAND 0x004589a0
void InitMouse(void) {
    unsigned int work[3];

    CONTROLLERBUFFER->prev_x = 0;
    CONTROLLERBUFFER->prev_y = 0;
    CONTROLLERBUFFER->x = 0;
    CONTROLLERBUFFER->y = 0;
    CONTROLLERBUFFER->delta_x = 0;
    CONTROLLERBUFFER->delta_y = 0;
    CONTROLLERBUFFER->x = lpConfig->screen_width >> 1;
    CONTROLLERBUFFER->y = lpConfig->screen_height >> 1;
    SystemParametersInfoA(3, 0, work, 0);
    CONTROLLERBUFFER->mouse_threshold1 = work[0];
    CONTROLLERBUFFER->mouse_threshold2 = work[1];
    CONTROLLERBUFFER->mouse_accel = work[2];
    SetPointer(5);
}

// FUNCTION: LEGOLAND 0x00458a50
void StartLevelGame(void) {
    char buf[0x34];

    if (MapLoaded == 0) {
        QueryClass = 0;
        CastlePlacedFlag = 0;
        PauseGameTimer();
        ResetGameTimer();
        MarkGameTimer();
        // STRING: LEGOLAND 0x004b9150
        sprintf(buf, "objlist%d.txt", lpConfig->level);
        SetBricksLimited(0);
        ResetMapAI();
        LevelScriptLoadResult = LoadLevelScript(buf);
        SetBricksLimited(1);
        AllocBlokeCounters(lpConfig->max_blokes);
        ResetEditState();
        ClearObjInstanceTable();
        MapStats.level_end = 0;
        UpdateMenu();
        SetIntervalScreenIsBriefing(1);
        ShowIntervalScreen(BriefingFileName, 0);
        MapLoaded = 1;
        ResumeGameTimer();
        ClearFreePlaySelection();
    }
}

// FUNCTION: LEGOLAND 0x00458b20
void UnloadMap(void) {
    if (MapLoaded != 0) {
        ResetClassRideList();
        DestroyIconGroup(0xd2);
        DelObjectList();
        FreeBlokeCounters();
        FreeAllObjectClassInstances();
        MapBatchUpdate = 1;
        UnloadLevelData();
        MapBatchUpdate = 0;
        SetBricksLimited(0);
        ClearObjInstanceTable();
        DestroyAllBlokes();
        RemoveAllNewObjects();
        ResetInfoStruct();
        ClearBuildObjList();
        FreeAllWorkOrdersAndWorkers();
        UnloadGameInterface();
        SetBricksLimited(1);
        GamePad = (GamePad & 0xffff00ff) | (((GamePad >> 8) & 0xeb) << 8);
        MapLoaded = 0;
    }
}

// FUNCTION: LEGOLAND 0x00458bb0
void SetMapLoaded(unsigned int param_1) { MapLoaded = param_1; }

/* Selects front-end mode, invalidates the previous screen, and requests initial screen zero. */
// FUNCTION: LEGOLAND 0x00458bc0
void SelectInitialFrontEndScreen(void) {
    EditMode.unk4 = 2;
    FrontEndScreenMode.unk4 = 0xffffffff;
    FrontEndScreenMode.unk8 = 0;
}

// FUNCTION: LEGOLAND 0x00458be0
void MarkLevelComplete(void) {
    int slot = ((struct ScreenConfig *)lpConfig)->slot;

    if (slot < 0xf) {
        CurrentProfile.flags[4 + slot] = 1;
    }
    UnlockAllFreePlayObjects();
    UpDateCurrentProfile();
}

/* Processes pending map-screen and level changes, dispatches the active game/front-end/map screen, and updates
 * input and sound. Returns zero when application mode is zero. */
// FUNCTION: LEGOLAND 0x00458c00
int UpdateApplicationFrame(void) {
    char path[256];

    SpeechStreamUpdate();
    if (MapScreenOpenPending != 0) {
        PauseGameTimer();
        PauseAllSamples();
        OkKeyHandler = 0;
        GamePad = GamePad & 0xffffffdf;
        CancelKeyHandler = 0;
        InitMapScreen();
        DisableInfoPopUPIcons();
        DisableSidePanelIcons();
        MapScreenOpenPending = 0;
    } else if (MapScreenClosePending != 0) {
        ResumeAllSamples();
        OkKeyHandler = 0;
        GamePad = GamePad | 0x20;
        CancelKeyHandler = 0;
        KillMapScreen();
        SetDefaultMouseHandlers();
        EnableSidePanelIcons();
        MapScreenClosePending = 0;
        ResumeGameTimer();
    } else if (GameStartPending != 0) {
        PauseGameTimer();
        ResetGameTimer();
        MarkGameTimer();
        OkKeyHandler = 0;
        CancelKeyHandler = 0;
        CastlePlacedFlag = 0;
        SamplesPaused = 1;
        PauseAllSamples();
        if (SaveGameLoadPending != 0) {
            DeletePlayableSamples(0);
            sprintf(path, "%s\\%dsave%d.sav", "profiles", CurrentProfile.profile_slot, *(unsigned int *)&CurrentProfile.save_slot & 0xff);
            LoadWatchSprite(0, 0);
            LoadGame(path);
            SaveGameLoadPending = 0;
            UnloadWatchSprite();
            InitGameInterface(0);
            SetDefaultMouseHandlers();
        } else {
            FreeAllScriptLists();
            StartLevelGame();
        }
        UpdateSoundVols();
        GameStartPending = 0;
        MapStats.level_end = 0;
        EditMode.unk4 = 3;
        SetDefaultMouseHandlers();
        ResumeGameTimer();
    }

    switch (EditMode.unk4) {
    case 2:
        SetPointer(5);
        InitScreens(FrontEndScreenMode.unk8 & 0xff);
        break;
    case 3:
        if (SamplesPaused != 0) {
            ResumeAllSamples();
            SamplesPaused = 0;
        }
        GameProc();
        break;
    case 1:
        RenderMapScreenFrame();
        break;
    case 0:
        return 0;
    default:
        break;
    }

    if (GameStartPending == 0) {
        // STRING: LEGOLAND 0x004b9160
        AdvisorDebugStage = "SFX";
        UpdateSound();
        ReadGameButtons();
        FrameCounter = FrameCounter + 1;
        if (MapStats.level_end != 0 && EditMode.unk4 == 3) {
            if (MapStats.field_3ac != 0) {
                SetPointer(0);
                PlayMovie(LevelEndMovieName, 1, 1);
                MapStats.field_3ac = 0;
                SetPointer(5);
            }
            if (MapStats.level_end != 3) {
                if (MapStats.level_end == 1) {
                    ((struct ScreenConfig *)lpConfig)->slot += 1;
                }
                UnlockAllFreePlayObjects();
                UnloadMap();
                FrontEndSubScreenActive = 1;
                EditMode.unk4 = 2;
                FrontEndScreenMode.unk4 = 0xffffffff;
                FrontEndScreenMode.unk8 = 6;
                UpdateSpeechPlayback();
                return 1;
            }
            FrontEndSubScreenActive = 0;
            EditMode.unk4 = 2;
            FrontEndScreenMode.unk4 = 0xffffffff;
            FrontEndScreenMode.unk8 = 1;
            UnloadMap();
        }
        UpdateSpeechPlayback();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00458ee0
void GameProc(void) {
    struct HitInfo hit = {1};
    RECT help_rect;
    struct HoverInfo saved;

    // STRING: LEGOLAND 0x004b91e4
    AdvisorDebugStage = "AI";
    HandleRideAI();
    DoMapAI();
    ControlPeople();
    ControlWorkers();
    if (WorkerOnMouseActive != 0) {
        CheckWorkerOnMouseStatus(0);
    }
    // STRING: LEGOLAND 0x004b91d4
    AdvisorDebugStage = "ProcessStuff";
    ProcessBuildingTimes();
    ProcessDamage();
    saved.type = Hover.type;
    SetPointer(5);
    // STRING: LEGOLAND 0x004b91cc
    AdvisorDebugStage = "Zoning";
    ResetHitInfo();
    // STRING: LEGOLAND 0x004b91c0
    AdvisorDebugStage = "Rendering";
    PushRenderingStatusAndLockVideoSurface();
    RenderView();
    if ((saved.type & 0x100) != 0 || (GamePad & 0x1000) != 0) {
        ProcessMapEditing();
    }
    saved = Hover;
    PrintSprite(InterfaceBgSprite, 0, 0, 0, &hit.type);
    RenderIconsExceptId(0x2c3);
    UpdateEditModeButtonSprites();
    // STRING: LEGOLAND 0x004b91b0
    AdvisorDebugStage = "In Game Help";
    ProcessInGameHelp();
    DrawPopUpInfo();
    RenderIcons2(0x2c3, 0, 0);
    ShowFocusedIconHelp();
    if ((GamePad & 0x1000) != 0) {
        Hover = saved;
    }
    if ((Hover.type & 0x100) != 0 && (EditMode.unk0 == 0 || EditMode.unk0 == 2)) {
        help_rect.left = MousePos.x;
        help_rect.top = MousePos.y - 0xa;
        help_rect.right = MousePos.x;
        help_rect.bottom = MousePos.y;
        if (EditMode.unk0 == 0) {
            switch (Hover.type) {
            case 0x103:
                SetPointer(8);
                if (QueryClass != NULL) {
                    HTBubbleHelp(&help_rect, QueryClass->name, 2);
                    ShowHelpText(QueryClass->element->name);
                }
                break;
            case 0x10a:
                HTBubbleHelp(&help_rect, GetString(0xd4), 2);
                ShowHelpById(0xd4);
                SetPointer(8);
                break;
            case 0x10b:
                HTBubbleHelp(&help_rect, GetString(0xd2), 2);
                ShowHelpById(0xd2);
                SetPointer(7);
                break;
            case 0x10c:
                HTBubbleHelp(&help_rect, GetString(0xd3), 2);
                ShowHelpById(0xd3);
                SetPointer(7);
                break;
            case 0x10d:
                HTBubbleHelp(&help_rect, GetString(0x7e4), 2);
                ShowHelpById(0x7e4);
                SetPointer(8);
                break;
            case 0x306:
                HTBubbleHelpWithMood(&help_rect, GetVisitorName(Hover.ptr), 2, GetBlokeMoodIconIndex(Hover.ptr));
                SetPointer(8);
                PauseHoveredBloke(Hover.ptr);
                break;
            case 0x307:
                HTBubbleHelp(&help_rect, GetString(0x90), 2);
                ShowHelpById(0x90);
                SetPointer(8);
                PauseHoveredBloke(Hover.ptr);
                break;
            case 0x308:
                HTBubbleHelp(&help_rect, GetString(0x92), 2);
                ShowHelpById(0x92);
                SetPointer(8);
                PauseHoveredBloke(Hover.ptr);
                break;
            default:
                SetPointer(7);
                break;
            }
        } else {
            if (QueryClass != NULL) {
                HTBubbleHelp(&help_rect, QueryClass->name, 2);
                ShowHelpText(QueryClass->element->name);
            }
        }
    }
    if (Hover.type == 5 && (MouseClickFlags & 2) != 0) {
        ClearAdvisorHelp();
        MouseClickConsumed = 1;
    }
    // STRING: LEGOLAND 0x004b91a4
    AdvisorDebugStage = "Appraisals";
    CheckAppraisalDue();
    // STRING: LEGOLAND 0x004b9194
    AdvisorDebugStage = "Appraisals Over";
    UpdateFocussedIconPtr();
    if ((GamePad & 0x1000) == 0 && MouseClickConsumed == 0) {
        if (FocussedIconPtr != NULL && WorkerOnMouseActive == 0) {
            SetPointer(6);
        }
        CheckFocussedIcon();
    }
    if (WorkerOnMouseActive != 0) {
        RenderWorkerOnMouse();
    }
    PopRenderingStatus();
    if (MapStats.show_capacity != 0) {
        if (IsLeftShiftDown() != 0 || IsRightShiftDown() != 0) {
            PrintCapacityStats();
        }
    }
    RenderingComplete();
    // STRING: LEGOLAND 0x004b9180
    AdvisorDebugStage = "Exiting GameProc";
}

// One frame of the map screen: draws the map, the interface background and the icons. While the
// mouse is over the map (Hover.type bit 0x100) it draws the view bounds, scrolls to the mouse on a
// click (MouseClickFlags bit 0) and closes the map on a double click (bit 1 twice within 500 ms
// and 5 pixels): MapScreenClosePending = 1 and the edit mode saved on entry is restored.
// FUNCTION: LEGOLAND 0x00459360
void RenderMapScreenFrame(void) {
    struct HoverInfo hit = {1};
    RECT saved_clip;
    int ticks;

    ResetHitInfo();
    PushRenderingStatusAndLockVideoSurface();
    DrawMapScreen();
    SetPointer(5);
    PrintSprite(InterfaceBgSprite, 0, 0, 0, (int *)&hit);
    UpdateEditModeButtonSprites();
    RenderIcons();
    CheckFocussedIcon();
    if (Hover.type == 2) {
        SetPointer(6);
        UpdateFocussedIconPtr();
        PopRenderingStatus();
        RenderingComplete();
        return;
    }
    if ((Hover.type & 0x100) != 0) {
        RECT map_clip = {0, 0x20, 0x280, 0x174};

        GetClipping(&saved_clip);
        SetClipping(&map_clip);
        RenderMouseBounds();
        SetClipping(&saved_clip);
        if ((MouseClickFlags & 1) != 0) {
            MapScreenSetScrollPos(&MousePos);
        }
        if ((MouseClickFlags & 2) != 0) {
            ticks = GetTicks();
            if (ticks - LastMapClickTicks < 500 && abs(MousePos.x - LastMapClickX) < 5 &&
                abs(MousePos.y - LastMapClickY) < 5) {
                MapScreenClosePending = 1;
                EditMode.unk4 = PreMapScreenMode;
                PreMapScreenMode = 1;
            }
            LastMapClickX = MousePos.x;
            LastMapClickY = MousePos.y;
            LastMapClickTicks = ticks;
        }
    }
    UpdateFocussedIconPtr();
    PopRenderingStatus();
    RenderingComplete();
}

/* Closes the front-end screen when the application is in front-end mode. */
// FUNCTION: LEGOLAND 0x004594e0
void CloseActiveApplicationScreen(void) {
    switch (EditMode.unk4) {
    case 2:
        CloseFrontEndScreen();
        break;
    default:
        break;
    }
}

// FUNCTION: LEGOLAND 0x004594f0
void InitDefaultProfile(void) {
    int i;

    for (i = 0; i < 68; i = i + 1) {
        ((unsigned int *)&CurrentProfile)[i] = 0;
    }

    CurrentProfile.speech_volume = 0x4b;
    CurrentProfile.music_volume = 0x64;
    CurrentProfile.fx_volume = 0x4b;
}

/* Initializes shared game resources, runs the application frame loop, then releases controllers, graphics,
 * visitors, sound and input. */
// FUNCTION: LEGOLAND 0x00459520
void RunGameApplication(void) {
    HMODULE ir50;
    MSG msg;

    // STRING: LEGOLAND 0x004b7138
    HedgeObjectClass = ElemID("HEDGE")->data;
    InitDefaultProfile();
    InitSoundSystem();
    SetMusicGrooveLevel(1);
    SuspendMusicThread();
    SetupControllers();
    LLIDB_ClearOnLevel();
    InitMouse();
    SetPointer(0);
    ProcessSystemEvents();
    // STRING: LEGOLAND 0x004b9200
    PlayMovie("lmi.avi", 0, 1);
    PrintTitleScreen1();
    ResumeMusicThread();
    LoadWatchSprite(0, 0);

    while (MusicThreadReady == 0) {
        PeekMessageA(&msg, NULL, 0, 0, 0);
        Sleep(100);
        DrawWatchSprite();
    }

    LoadGBarSprites();
    LoadWorkerInterfaceGFX();
    LoadBubbleHelpGFX();
    InitialiseBlokes();
    InitGameMap();
    SetPointer(0);
    // STRING: LEGOLAND 0x004b91f4
    PlayMovie("Intro.avi", 1, 0);
    RequestMusicTheme(0);
    SetPointer(5);
    FrameCounter = 0;
    Load_Interface_ControlIcons();
    DrawWatchSprite();
    Load_Interface_ThemeIcons();
    FreeTileSpace(0, 0x800);
    EditMode.unk4 = 3;
    LoadMapTiles();
    DrawWatchSprite();
    InitMan();
    DrawWatchSprite();
    CreateObjectClasses();
    DrawWatchSprite();
    SelectInitialFrontEndScreen();
    DrawWatchSprite();
    // STRING: LEGOLAND 0x004b91e8
    ir50 = LoadLibraryA("Ir50_32.dll");
    DrawWatchSprite();
    LoadAdvisorAnims();
    DrawWatchSprite();
    UnloadWatchSprite();
    ResumeMusicThread();
    while (UpdateApplicationFrame() != 0) {
    }
    CloseActiveApplicationScreen();
    SpeechCloseFile();
    if (SPRITE_TitleScreenBk != NULL) {
        KillSprite(SPRITE_TitleScreenBk);
        SPRITE_TitleScreenBk = NULL;
    }
    FreeControllers();
    UnloadGBarSprites();
    UnloadBubbleHelpGFX();
    FreeBlokePool();
    KillGameMap();
    UnLoad_Interface_ControlIcons();
    UnLoad_Interface_ThemeIcons();
    UnloadMapTiles();
    UnInitMan();
    FreeAdvisorAnims();
    FreeLibrary(ir50);
    FreeBlokeCounters();
    KillHelp();
    KillSoundSystem();
    KillInputSystem();
}

// FUNCTION: LEGOLAND 0x00459710
void ShowLevelEndScreens(char *s) {
    char buffer[0x80];
    char *semicolon;
    char *source;

    source = s;
    semicolon = strchr(source, ';');
    if (semicolon != NULL) {
        *semicolon = 0;
        strcpy(buffer, source);
        semicolon++;
        semicolon[-1] = ';';
        if (strlen(semicolon) != 0) {
            strcpy(LevelEndMovieName, semicolon);
            MapStats.field_3ac = 1;
        }
    }
    SetIntervalScreenIsBriefing(0);
    if (LoadIntervalReport(buffer) != 0) {
        FrontEndSubScreenActive = 1;
        EditMode.unk4 = 2;
        FrontEndScreenMode.unk4 = 0xffffffff;
        FrontEndScreenMode.unk8 = 7;
    }
}

/* Sets one of the two level-end screen filenames (nonzero selector: result 1; zero: result 2). Copies at most 255
 * characters plus NUL, or clears the name for NULL. */
// FUNCTION: LEGOLAND 0x004597e0
void SetLevelEndScreenFile(int param0, const char *param1) {
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
void EndLevel(unsigned int a1) {
    MapStats.level_end = a1;
    if (a1 == 1) {
        ShowLevelEndScreens(MapStats.field_198);
        return;
    }
    if (a1 == 2) {
        ShowLevelEndScreens(MapStats.field_298);
    }
}
