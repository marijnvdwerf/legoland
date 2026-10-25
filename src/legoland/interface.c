#include "interface.h"
#include <stdlib.h>
#include <string.h>
#include "challenge.h"
#include "controller.h"
#include "debug.h"
#include "debug_alloc.h"
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
#include "saveload.h"
#include "screens.h"
#include "sound_music.h"
#include "sound_sfx.h"
#include "string.h"
#include "timer.h"
#include "title.h"
#include "wndenv.h"
#include "worker_mouse.h"

struct MovieHandle {
    /* 0x00 */ unsigned int frame_count;
    /* 0x04 */ unsigned int frame_rate;
    /* 0x08 */ int width;
    /* 0x0c */ int height;
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

#include "image_sprite.h"
#include "imports.h"
#include "mapscreen.h"
#include "obj_instance.h"
#include "stream.h"

// FUNCTION: LEGOLAND 0x004741f0
LEGO_EXPORT void Load_Interface_ControlIcons(void) {
    if (InterfaceControlIconsLoaded != 0) {
        return;
    }
    InterfaceControlIconsLoaded = 1;
    // STRING: LEGOLAND 0x004bb3b4
    InterfaceBgSprite = LoadSprite("InterfaceBG.lls", 4);
    // STRING: LEGOLAND 0x004bb3a4
    NoEnergySprite = LoadSprite("No_Energy.lls", 4);
    // STRING: LEGOLAND 0x004bb394
    BarPointerSprite = LoadSprite("Bar_pointer.lls", 4);
    // STRING: LEGOLAND 0x004bb37c
    IfPathIconPressedSprite = LoadSprite("IF_PathIconPressed.lls", 4);
    // STRING: LEGOLAND 0x004bb36c
    IfPathIconSprite = LoadSprite("IF_PathIcon.lls", 4);
    // STRING: LEGOLAND 0x004bb354
    IfQueryIconPressedSprite = LoadSprite("IF_QueryIconPressed.lls", 4);
    // STRING: LEGOLAND 0x004bb340
    IfQueryiconSprite = LoadSprite("IF_Queryicon.lls", 4);
    // STRING: LEGOLAND 0x004bb324
    IfEraserIconPressedSprite = LoadSprite("IF_EraserIconPressed.lls", 4);
    // STRING: LEGOLAND 0x004bb310
    IfEraserIconSprite = LoadSprite("IF_EraserIcon.lls", 4);
    // STRING: LEGOLAND 0x004bb2f8
    IfMapIconPressedSprite = LoadSprite("IF_MapIconPressed.lls", 4);
    // STRING: LEGOLAND 0x004bb2e8
    IfMapiconSprite = LoadSprite("IF_Mapicon.lls", 4);
    // STRING: LEGOLAND 0x004bb2cc
    IfOptionsIconPressedSprite = LoadSprite("IF_OptionsIconPressed.lls", 4);
    // STRING: LEGOLAND 0x004bb2b8
    IfOptionsIconSprite = LoadSprite("IF_OptionsIcon.lls", 4);
    // STRING: LEGOLAND 0x004bb29c
    AttractHighlightOnSprite = LoadSprite("Attract_Highlight_On.lls", 4);
    // STRING: LEGOLAND 0x004bb280
    AttractHighlightOffSprite = LoadSprite("Attract_Highlight_Off.lls", 4);
    // STRING: LEGOLAND 0x004bb26c
    AttractNewOffSprite = LoadSprite("Attract_New_Off.lls", 4);
    // STRING: LEGOLAND 0x004bb258
    AttractNewOnSprite = LoadSprite("Attract_New_On.lls", 4);
    // STRING: LEGOLAND 0x004bb240
    SideScrollDownLitSprite = LoadSprite("Side_ScrollDown_Lit.lls", 4);
    // STRING: LEGOLAND 0x004bb228
    SideScrollUpLitSprite = LoadSprite("Side_ScrollUp_Lit.lls", 4);
    // STRING: LEGOLAND 0x004bb218
    LinkMiddleSprite = LoadSprite("Link_Middle.lls", 4);
    // STRING: LEGOLAND 0x004bb208
    LinkBottomSprite = LoadSprite("Link_Bottom.lls", 4);
    // STRING: LEGOLAND 0x004bb1f8
    BriefIcon2Sprite = LoadSprite("BriefIcon2.lls", 4);
    // STRING: LEGOLAND 0x004bb1e8
    BriefIconSprite = LoadSprite("BriefIcon.lls", 4);
    // STRING: LEGOLAND 0x004bb1d8
    ScriptEndSprite = LoadSprite("ScriptEnd.lls", 4);
}

// FUNCTION: LEGOLAND 0x004743b0
LEGO_EXPORT void UnLoad_Interface_ControlIcons(void) {
    if (InterfaceControlIconsLoaded != 0) {
        memset(ThemeIcons, 0, sizeof(ThemeIcons));
        InterfaceControlIconsLoaded = 0;
        KillSprite(InterfaceBgSprite);
        InterfaceBgSprite = NULL;
        KillSprite(NoEnergySprite);
        NoEnergySprite = NULL;
        KillSprite(BarPointerSprite);
        BarPointerSprite = NULL;
        KillSprite(IfPathIconPressedSprite);
        IfPathIconPressedSprite = NULL;
        KillSprite(IfPathIconSprite);
        IfPathIconSprite = NULL;
        KillSprite(IfQueryIconPressedSprite);
        IfQueryIconPressedSprite = NULL;
        KillSprite(IfQueryiconSprite);
        IfQueryiconSprite = NULL;
        KillSprite(IfEraserIconPressedSprite);
        IfEraserIconPressedSprite = NULL;
        KillSprite(IfEraserIconSprite);
        IfEraserIconSprite = NULL;
        KillSprite(IfMapIconPressedSprite);
        IfMapIconPressedSprite = NULL;
        KillSprite(IfMapiconSprite);
        IfMapiconSprite = NULL;
        KillSprite(IfOptionsIconPressedSprite);
        IfOptionsIconPressedSprite = NULL;
        KillSprite(IfOptionsIconSprite);
        IfOptionsIconSprite = NULL;
        KillSprite(AttractNewOffSprite);
        AttractNewOffSprite = NULL;
        KillSprite(AttractNewOnSprite);
        AttractNewOnSprite = NULL;
        KillSprite(AttractHighlightOnSprite);
        AttractHighlightOnSprite = NULL;
        KillSprite(AttractHighlightOffSprite);
        AttractHighlightOffSprite = NULL;
        KillSprite(SideScrollDownLitSprite);
        SideScrollDownLitSprite = NULL;
        KillSprite(SideScrollUpLitSprite);
        SideScrollUpLitSprite = NULL;
        KillSprite(LinkMiddleSprite);
        LinkMiddleSprite = NULL;
        KillSprite(LinkBottomSprite);
        LinkBottomSprite = NULL;
        KillSprite(BriefIcon2Sprite);
        BriefIcon2Sprite = NULL;
        KillSprite(BriefIconSprite);
        BriefIconSprite = NULL;
        KillSprite(ScriptEndSprite);
        ScriptEndSprite = NULL;
    }
}

// FUNCTION: LEGOLAND 0x00474590
void ResetThemeIconStates(void) {
    SelectedThemeIcon = 0;
    AdventurersThemeDeselected = 1;
    WesternThemeDeselected = 1;
    CastleThemeDeselected = 1;
    LegolandThemeDeselected = 1;
}

// FUNCTION: LEGOLAND 0x004745c0
LEGO_EXPORT void Load_Interface_ThemeIcons(void) {
    if (ThemeIconsLoaded != 0) {
        return;
    }
    ThemeIconsLoaded = 1;
    // STRING: LEGOLAND 0x004bb464
    LegolandThemeOnSprite = LoadSprite("legoland_themeON.lls", 4);
    // STRING: LEGOLAND 0x004bb44c
    LegolandThemeOffSprite = LoadSprite("legoland_themeOFF.lls", 4);
    // STRING: LEGOLAND 0x004bb438
    CastleThemeOnSprite = LoadSprite("castle_themeON.lls", 4);
    // STRING: LEGOLAND 0x004bb424
    CastleThemeOffSprite = LoadSprite("castle_themeOFF.lls", 4);
    // STRING: LEGOLAND 0x004bb410
    WesternThemeOnSprite = LoadSprite("western_themeON.lls", 4);
    // STRING: LEGOLAND 0x004bb3f8
    WesternThemeOffSprite = LoadSprite("western_themeOFF.lls", 4);
    // STRING: LEGOLAND 0x004bb3e0
    AdventurersThemeOnSprite = LoadSprite("adventurers_themeON.lls", 4);
    // STRING: LEGOLAND 0x004bb3c4
    AdventurersThemeOffSprite = LoadSprite("adventurers_themeOFF.lls", 4);
}

// FUNCTION: LEGOLAND 0x00474670
LEGO_EXPORT void UnLoad_Interface_ThemeIcons(void) {
    unsigned int sprite;

    if (ThemeIconsLoaded != 0) {
        sprite = ThemeIconsLoaded;
        ThemeIconsLoaded = 0;
        if (sprite != 0) {
            if (LegolandThemeOnSprite != 0) {
                KillSprite(LegolandThemeOnSprite);
                LegolandThemeOnSprite = 0;
            }
            if (LegolandThemeOffSprite != 0) {
                KillSprite(LegolandThemeOffSprite);
                LegolandThemeOffSprite = 0;
            }
            if (CastleThemeOnSprite != 0) {
                KillSprite(CastleThemeOnSprite);
                CastleThemeOnSprite = 0;
            }
            if (CastleThemeOffSprite != 0) {
                KillSprite(CastleThemeOffSprite);
                CastleThemeOffSprite = 0;
            }
            if (WesternThemeOnSprite != 0) {
                KillSprite(WesternThemeOnSprite);
                WesternThemeOnSprite = 0;
            }
            if (WesternThemeOffSprite != 0) {
                KillSprite(WesternThemeOffSprite);
                WesternThemeOffSprite = 0;
            }
            if (AdventurersThemeOnSprite != 0) {
                KillSprite(AdventurersThemeOnSprite);
                AdventurersThemeOnSprite = 0;
            }
            if (AdventurersThemeOffSprite != 0) {
                KillSprite(AdventurersThemeOffSprite);
                AdventurersThemeOffSprite = 0;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00474750
void DeselectThemeIcon(void) {
    if (LegolandThemeDeselected == 0) {
        LegolandThemeDeselected = 1;
        SetIconSprite((struct IconNode *)SelectedThemeIcon, LegolandThemeOffSprite);
    } else if (CastleThemeDeselected == 0) {
        CastleThemeDeselected = 1;
        SetIconSprite((struct IconNode *)SelectedThemeIcon, CastleThemeOffSprite);
    } else if (WesternThemeDeselected == 0) {
        WesternThemeDeselected = 1;
        SetIconSprite((struct IconNode *)SelectedThemeIcon, WesternThemeOffSprite);
    } else if (AdventurersThemeDeselected == 0) {
        AdventurersThemeDeselected = 1;
        SetIconSprite((struct IconNode *)SelectedThemeIcon, AdventurersThemeOffSprite);
    }
}

// FUNCTION: LEGOLAND 0x00474800
LEGO_EXPORT void UnLoad_Interface_Icons(void) {
    UnLoad_Interface_ControlIcons();
    DestroyIconGroup(0xd2);
    UnLoad_PopUpInfo();
}

// FUNCTION: LEGOLAND 0x00474820
unsigned char DefaultOkKeyHandler(unsigned int dummy, unsigned char flags) {
    if ((flags & 2) != 0) {
        HandlePopUpOkKey();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00474830
unsigned char DefaultCancelKeyHandler(unsigned int a, unsigned int flags, unsigned int c, unsigned int d) {
    if ((flags & 2) != 0) {
        if (TryClosePopUp() == 0) {
            if (WorkerOnMouseActive != 0) {
                CheckWorkerOnMouseStatus(1);
            } else {
                return InterfaceOptionsClicked(a, flags, c, d);
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00474880
void SetDefaultMouseHandlers(void) {
    OkKeyHandler = DefaultOkKeyHandler;
    CancelKeyHandler = DefaultCancelKeyHandler;
}

// FUNCTION: LEGOLAND 0x004748a0
void UpdateScriptEndIcon(void *a) {
    if (ScriptEndIcon == 0) {
        return;
    }
    if (IsScriptStopped() == 0) {
        if (a != NULL) {
            ((struct IconNode *)ScriptEndIcon)->flags |= 0x2002;
        } else {
            ((struct IconNode *)ScriptEndIcon)->flags &= 0xffffdffd;
        }
        return;
    }
    ((struct IconNode *)ScriptEndIcon)->flags |= 0x2002;
    ((struct IconNode *)ScriptEndIcon)->event_handler = (void *)ScriptEndIconHandler;
    ((struct IconNode *)ScriptEndIcon)->string_id = 0x8fc;
    ((struct IconNode *)ScriptEndIcon)->string = GetString(0x8fc);
}

// FUNCTION: LEGOLAND 0x00474920
int SaveObjectiveIcons(void) {
    int i;

    for (i = 0; i < 4; i++) {
        if (((struct IconNode *)ThemeIcons[i])->flags & 0x400) {
            SavedThemeIconEnabled[i] = 0;
        } else {
            SavedThemeIconEnabled[i] = 1;
        }
    }
    return SaveGameWrite(SavedThemeIconEnabled, 0x10) != 0;
}

// FUNCTION: LEGOLAND 0x00474970
int LoadObjectiveIcons(void) {
    return SaveGameRead(SavedThemeIconEnabled, 0x10) != 0;
}

// FUNCTION: LEGOLAND 0x00474990
void RestoreObjectiveIcons(void) {
    int i;

    for (i = 0; i < 4; i++) {
        if (SavedThemeIconEnabled[i] != 0) {
            ((struct IconNode *)ThemeIcons[i])->flags &= 0xfffffbff;
        } else {
            ((struct IconNode *)ThemeIcons[i])->flags |= 0x400;
        }
    }
}

// FUNCTION: LEGOLAND 0x004749d0
LEGO_EXPORT int InitGameInterface(int a) {
    struct Element *element;
    struct Ride *obj;
    struct IconNode *icon;
    struct IconNode *bar;

    if (GameInterfaceLoaded == 0) {
        GameInterfaceLoaded = 1;
        LLIDB_FindElement(PathControlName, &element, 0);
        obj = element->obj;
        PathControlObject = obj;
        icon = InsertIcon((short)InterfaceIconPos[8], (short)InterfaceIconPos[9], 0x93, IfPathIconSprite);
        icon->string = (char *)obj->field_7c;
        icon->string_id = 0xffffffff;
        icon->field_18 = 1;
        icon->field_1c = IfPathIconPressedSprite;
        icon->field_20p = IfPathIconSprite;
        icon->field_8 = obj;
        icon->render_func = (void *)RenderGBarSpriteIcon;
        icon->event_handler = (void *)SetEditObjectIconHandler;
        icon->flags = (icon->flags & 0xfffffdff) | 0x300a;

        icon = InsertIcon((short)InterfaceIconPos[10], (short)InterfaceIconPos[11], 0x93, IfQueryiconSprite);
        icon->string_id = 0x5a;
        icon->string = GetString(0x5a);
        icon->flags |= 0x6002;
        icon->event_handler = (void *)InterfaceQueryClicked;
        icon->field_18 = 2;
        icon->field_1c = IfQueryIconPressedSprite;
        icon->field_20p = IfQueryiconSprite;

        icon = InsertIcon((short)InterfaceIconPos[12], (short)InterfaceIconPos[13], 0x93, IfEraserIconSprite);
        icon->string_id = 0x5b;
        icon->string = GetString(0x5b);
        icon->flags |= 0x6002;
        icon->event_handler = (void *)InterfaceEraserClicked;
        icon->field_18 = 3;
        icon->field_1c = IfEraserIconPressedSprite;
        icon->field_20p = IfEraserIconSprite;

        icon = InsertIcon((short)InterfaceIconPos[14], (short)InterfaceIconPos[15], 0x93, IfMapiconSprite);
        icon->string_id = 0x5c;
        icon->string = GetString(0x5c);
        icon->flags |= 0x6002;
        icon->event_handler = (void *)ToggleMapScreen;
        icon->field_18 = 4;
        icon->field_1c = IfMapIconPressedSprite;
        icon->field_20p = IfMapiconSprite;

        icon = InsertIcon((short)InterfaceIconPos[16], (short)InterfaceIconPos[17], 0x93, IfOptionsIconSprite);
        icon->string_id = 0x5d;
        icon->string = GetString(0x5d);
        icon->flags |= 0x6002;
        icon->event_handler = (void *)InterfaceOptionsClicked;
        icon->field_20p = IfOptionsIconSprite;

        icon = InsertIcon((short)InterfaceIconPos[0], (short)InterfaceIconPos[1], 0x9a, LegolandThemeOffSprite);
        icon->string_id = 0x5e;
        icon->string = GetString(0x5e);
        icon->flags |= 0x6002;
        icon->event_handler = (void *)LegolandThemeIconHandler;
        ThemeIcons[0] = (struct IconNode *)icon;

        icon = InsertIcon((short)InterfaceIconPos[2], (short)InterfaceIconPos[3], 0x9a, WesternThemeOffSprite);
        WesternThemeIcon = icon;
        icon->string_id = 0x5f;
        icon->string = GetString(0x5f);
        icon->flags |= 0x6002;
        icon->event_handler = (void *)WesternThemeIconHandler;
        ThemeIcons[1] = (struct IconNode *)icon;

        icon = InsertIcon((short)InterfaceIconPos[4], (short)InterfaceIconPos[5], 0x9a, CastleThemeOffSprite);
        icon->string_id = 0x60;
        icon->string = GetString(0x60);
        icon->flags |= 0x6002;
        icon->event_handler = (void *)CastleThemeIconHandler;
        ThemeIcons[2] = (struct IconNode *)icon;

        icon = InsertIcon((short)InterfaceIconPos[6], (short)InterfaceIconPos[7], 0x9a, AdventurersThemeOffSprite);
        icon->string_id = 0x61;
        icon->string = GetString(0x61);
        icon->flags |= 0x6002;
        icon->event_handler = (void *)AdventurersThemeIconHandler;
        ThemeIcons[3] = (struct IconNode *)icon;

        // STRING: LEGOLAND 0x004bb48c
        bar = LoadSpriteIcon("Bar_Energy.lls", 4, 0x180, 6, 0x9a);
        bar->render_func = (void *)RenderEnergyBar;
        bar->flags |= 0x400a;

        // STRING: LEGOLAND 0x004bb47c
        bar = LoadSpriteIcon("Bar_Coins.lls", 4, 0x1c, 6, 0x9a);
        bar->render_func = (void *)RenderMoneyBar;
        bar->flags |= 0x400a;

        icon = InsertIcon(0x19e, 0x179, 0x9a, BriefIcon2Sprite);
        icon->string_id = 0x24e;
        icon->string = GetString(0x24e);
        icon->field_18p = BriefIconSprite;
        icon->flags |= 0x600a;
        icon->event_handler = (void *)InterfaceBriefingClicked;
        icon->render_func = (void *)RenderFocusableIcon;
        BriefingIcon = icon;
        icon->flags |= 0x400;
        HasIntervalReport(BriefingFileName);

        icon = InsertIcon(0x20a, 0x17a, 0x93, ScriptEndSprite);
        icon->field_20p = NULL;
        icon->field_1c = NULL;
        icon->string_id = 0x24f;
        icon->string = GetString(0x24f);
        icon->event_handler = (void *)InterfaceAdvisorClicked;
        icon->flags |= 0x4008;
        icon->render_func = (void *)RenderAdvisorIcon;
        ScriptEndIcon = icon;
        if (a != 0) {
            SetScriptStopped(0);
            if (CurrentProfile.field_45 == 2) {
                UpdateScriptEndIcon((void *)0);
            } else {
                UpdateScriptEndIcon((void *)1);
            }
        } else if (IsScriptStopped() != 0) {
            SetScriptStopped(1);
        } else {
            SetScriptStopped(0);
            if (CurrentProfile.field_45 == 2) {
                UpdateScriptEndIcon((void *)0);
            } else {
                UpdateScriptEndIcon((void *)1);
            }
        }
        InitPopUpInfo();
    }
    ResetThemeIconStates();
    LegolandCommonThemeCount = 0;
    AdventurersThemeCount = 0;
    CastleThemeCount = 0;
    WesternThemeCount = 0;
    ResetMoveAWorkerStruct();
    BuildSidePanelState = 2;
    BuildSidePanelOriginX = 0x86;
    lpConfig->view_x = 0;
    lpConfig->view_width = 0x280;
    BuildSidePanelSlideInPending = 1;
    BuildSidePanelViewportReserved = 0;
    SelectedBuildTheme = 5;
    DestroyIconGroup(0xd2);
    ShowProfileObjectiveIcons();
    if (LevelLoadedFromSave != 0) {
        RestoreObjectiveIcons();
    }
}

// FUNCTION: LEGOLAND 0x00474ed0
void UnloadGameInterface(void) {
    if (GameInterfaceLoaded != 0) {
        GameInterfaceLoaded = 0;
        RemoveIconGroup(0x93);
        ScriptEndIcon = 0;
        RemoveIconGroup(0x9a);
        BriefingIcon = NULL;
        RemoveIconRange(0xd2);
        UnLoad_PopUpInfo();
        BuildSidePanelState = 2;
        BuildSidePanelSlideInPending = 1;
        BuildSidePanelViewportReserved = 0;
        BuildSidePanelOriginX = 0x86;
    }
}

// FUNCTION: LEGOLAND 0x00474f40
unsigned char InterfaceBriefingClicked(void *context, unsigned int flags, const char *a, const char *b) {
    if (EditMode.unk4 != 1) {
        if (flags & 2) {
            EditMode.unk0 = 0;
            SetIntervalScreenIsBriefing(1);
            ShowIntervalScreen(BriefingFileName, HintsFileName);
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00474f80
unsigned char ScriptEndIconHandler(unsigned int a, unsigned int flags) {
    if ((flags & 2) != 0) {
        EndLevel(1);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00474fa0
unsigned char InterfaceAdvisorClicked(unsigned int a, unsigned char flags) {
    if (EditMode.unk4 != 1) {
        if ((flags & 2) != 0) {
            RepeatCurrentObjective();
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00474fc0
unsigned char SetEditObjectIconHandler(struct IconNode *a, unsigned int flags) {
    if (EditMode.unk4 == 1) {
        return 1;
    }
    if (flags & 2) {
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        SetEditObject(a->field_8);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00475000
unsigned char InterfaceQueryClicked(unsigned int a, unsigned int flags) {
    if (EditMode.unk4 != 1) {
        if (flags & 2) {
            PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
            GamePad = (GamePad & 0xffff00ff) | ((GamePad & 0xff00) & 0xef00);
            EditMode.unk0 = 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00475040
unsigned char InterfaceEraserClicked(unsigned int a, unsigned int flags) {
    if (EditMode.unk4 == 1) {
        return 1;
    }
    if ((flags & 2) == 0) {
        return 1;
    }
    PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
    GamePad &= ~0x400;
    EditMode.unk0 = 2;
    return 1;
}

// FUNCTION: LEGOLAND 0x00475080
unsigned char ToggleMapScreen(unsigned int a, unsigned char flags) {
    if ((flags & 2) != 0) {
        do {
            PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
            if (EditMode.unk4 != 1) {
                SpeechCloseFile();
                PreMapScreenMode = EditMode.unk4;
                EditMode.unk4 = 1;
                GamePad = GamePad & 0xffffebff;
                MapScreenOpenPending = 1;
                SpeechDelayFrames = 4;
                EditMode.unk0 = 0;
            } else {
                MapScreenClosePending = 1;
                EditMode.unk4 = PreMapScreenMode;
                PreMapScreenMode = 1;
                RestoreMouseHandlers();
            }
        } while (0);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00475120
unsigned char InterfaceOptionsClicked(unsigned int a, unsigned int flags, unsigned int c, unsigned int d) {
    if ((flags & 2) != 0 && EditMode.unk4 != 1) {
        if (WorkerOnMouseActive == 0) {
            PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
            GamePad = (GamePad & 0xffff00ff) | ((GamePad & 0xff00) & 0xeb00);
            EditMode.unk0 = 0;
            FrontEndSubScreenActive = 1;
            EditMode.unk4 = 2;
            FrontEndScreenMode.unk4 = 0xffffffff;
            FrontEndScreenMode.unk8 = 5;
            SpeechCloseFile();
            SpeechDelayFrames = 4;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004751a0
unsigned char LegolandThemeIconHandler(struct IconNode *param_1, unsigned char flags) {
    unsigned int saved_e34;
    unsigned int saved_ff8;
    int result;

    saved_e34 = ResearchTabSelected;
    saved_ff8 = SelectedBuildTheme;
    if (EditMode.unk4 != 1 && (flags & 2) != 0) {
        do {
            EditMode.unk0 = 0;
            GamePad = GamePad & 0xffffebff;
            PlayInstanceOfSample(GameFX[FX_THEME_CLICK].sample, 0, 1, 0);
            if (SelectedBuildTheme != 0) {
                SelectedBuildTheme = 0;
                ResearchTabSelected = 0;
                result = TestMenu(BuildThemeNames);
                if (result == 1) {
                    DeselectThemeIcon();
                    SelectedThemeIcon = param_1;
                    SetIconSprite(param_1, LegolandThemeOnSprite);
                    LegolandThemeDeselected = 0;
                } else {
                    SelectedBuildTheme = saved_ff8;
                    if (saved_ff8 != 5) {
                        ResearchTabSelected = saved_e34;
                        TestMenu(&BuildThemeNames[saved_ff8 * 5]);
                    }
                }
            } else {
                DeselectThemeIcon();
                LegolandThemeDeselected = 1;
                SelectedBuildTheme = 5;
                BuildSidePanelState = 1;
                BuildSidePanelSlideInPending = 1;
            }
        } while (0);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004752a0
unsigned char AdventurersThemeIconHandler(struct IconNode *param_1, unsigned char flags) {
    unsigned int saved_e34;
    int result;

    saved_e34 = ResearchTabSelected;
    if (EditMode.unk4 != 1 && (flags & 2) != 0) {
        GamePad = GamePad & 0xffffebff;
        EditMode.unk0 = 0;
        PlayInstanceOfSample(GameFX[FX_THEME_CLICK].sample, 0, 1, 0);
        if (SelectedBuildTheme != 3) {
            SelectedBuildTheme = 3;
            ResearchTabSelected = 0;
            result = TestMenu(&BuildThemeNames[15]);
            if (result == 1) {
                DeselectThemeIcon();
                SelectedThemeIcon = param_1;
                SetIconSprite(param_1, AdventurersThemeOnSprite);
                AdventurersThemeDeselected = 0;
                return 1;
            }
            ResearchTabSelected = saved_e34;
            TestMenu(&BuildThemeNames[SelectedBuildTheme * 5]);
            return 1;
        }
        DeselectThemeIcon();
        AdventurersThemeDeselected = 1;
        SelectedBuildTheme = 5;
        BuildSidePanelState = 1;
        BuildSidePanelSlideInPending = 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004753a0
unsigned char CastleThemeIconHandler(struct IconNode *param_1, unsigned char flags) {
    unsigned int saved_e34;
    int saved_ff8;
    int result;

    saved_e34 = ResearchTabSelected;
    saved_ff8 = SelectedBuildTheme;
    SelectedBuildTheme = saved_ff8;
    if (EditMode.unk4 != 1 && (flags & 2) != 0) {
        GamePad = GamePad & 0xffffebff;
        EditMode.unk0 = 0;
        PlayInstanceOfSample(GameFX[FX_THEME_CLICK].sample, 0, 1, 0);
        if (SelectedBuildTheme != 2) {
            SelectedBuildTheme = 2;
            ResearchTabSelected = 0;
            result = TestMenu(&BuildThemeNames[10]);
            if (result == 1) {
                DeselectThemeIcon();
                SelectedThemeIcon = param_1;
                SetIconSprite(param_1, CastleThemeOnSprite);
                CastleThemeDeselected = 0;
                return 1;
            }
            SelectedBuildTheme = saved_ff8;
            if (saved_ff8 != 5) {
                ResearchTabSelected = saved_e34;
                TestMenu(&BuildThemeNames[saved_ff8 * 5]);
                return 1;
            }
        } else {
            DeselectThemeIcon();
            CastleThemeDeselected = 1;
            SelectedBuildTheme = 5;
            BuildSidePanelState = 1;
            BuildSidePanelSlideInPending = 1;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004754b0
unsigned char WesternThemeIconHandler(struct IconNode *param_1, unsigned char flags) {
    unsigned int saved_e34;
    int saved_ff8;
    int result;

    saved_e34 = ResearchTabSelected;
    saved_ff8 = SelectedBuildTheme;
    SelectedBuildTheme = saved_ff8;
    if (EditMode.unk4 != 1 && (flags & 2) != 0) {
        GamePad = GamePad & 0xffffebff;
        EditMode.unk0 = 0;
        PlayInstanceOfSample(GameFX[FX_THEME_CLICK].sample, 0, 1, 0);
        if (SelectedBuildTheme != 1) {
            SelectedBuildTheme = 1;
            ResearchTabSelected = 0;
            result = TestMenu(&BuildThemeNames[5]);
            if (result == 1) {
                DeselectThemeIcon();
                SelectedThemeIcon = param_1;
                SetIconSprite(param_1, WesternThemeOnSprite);
                WesternThemeDeselected = 0;
                return 1;
            }
            SelectedBuildTheme = saved_ff8;
            if (saved_ff8 != 5) {
                ResearchTabSelected = saved_e34;
                TestMenu(&BuildThemeNames[saved_ff8 * 5]);
                return 1;
            }
        } else {
            DeselectThemeIcon();
            WesternThemeDeselected = 1;
            SelectedBuildTheme = 5;
            BuildSidePanelState = 1;
            BuildSidePanelSlideInPending = 1;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004755c0
void InsertObjectSortedByCost(void *data) {
    struct InterfaceListNode *node;
    struct InterfaceListNode *current;
    struct InterfaceListNode *prev;
    int current_cost;

    current = BuildObjectList;
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
    BuildObjectList = node;
    node->next = current;
}

// FUNCTION: LEGOLAND 0x00475630
LEGO_EXPORT void InsertChildIntoList(struct Ride *param_1) {
    struct InterfaceListNode *node;
    struct InterfaceListNode *current;
    struct InterfaceListNode *prev;
    void *parent;

    current = BuildObjectList;
    node = (struct InterfaceListNode *)malloc(sizeof(struct InterfaceListNode));
    node->data = param_1;
    node->flag = 0;
    node->next = NULL;
    parent = param_1->parent_element;
    while (current != NULL) {
        if (((struct Ride *)current->data)->element == parent) {
            prev = current;
            current = current->next;
            while (current != NULL) {
                if (((struct Ride *)node->data)->parent_element != ((struct Ride *)current->data)->parent_element) {
                    break;
                }
                if (GetObjCost(node->data) <= GetObjCost(current->data)) {
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
    if (ResearchTabSelected != 0) {
        InsertObjectSortedByCost(param_1);
    }
    free(node);
}

// FUNCTION: LEGOLAND 0x004756e0
LEGO_EXPORT void DelObjectList(void) {
    struct InterfaceListNode *current;
    struct InterfaceListNode *next;

    current = BuildObjectList;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    BuildObjectList = NULL;
}

// FUNCTION: LEGOLAND 0x00475710
LEGO_EXPORT unsigned int TestMenu(unsigned int *entry) {
    return ObjectLinkedList(entry);
}

// FUNCTION: LEGOLAND 0x00475720
LEGO_EXPORT unsigned int ObjectLinkedList(unsigned int *entry) {
    struct Element *build_elem;
    struct Element *theme_elem;
    struct Element *param_elem;
    struct Element *menu_elem;
    struct Element *elem;
    struct Ride *obj;
    char *menu_name;
    int count;
    int i;
    int matched;

    matched = 1;
    DelObjectList();
    if (LLIDB_FindElement(BuildMenuName, &build_elem, 0) != 0) {
        exit(1);
    }
    if (LLIDB_FindElement("COMMON THEME", &theme_elem, 0) != 0) {
        exit(1);
    }
    if (LLIDB_FindElement((const char *)entry, &param_elem, 0) != 0) {
        exit(1);
    }
    count = LLIDB_GetCount();
    for (menu_name = BuildCategoryNames[0]; (INT_PTR)menu_name < (INT_PTR)InterfaceIconPos; menu_name += 0x14) {
        if (LLIDB_FindElement(menu_name, &menu_elem, 0) != 0) {
            exit(1);
        }
        for (i = 0; i < count; i++) {
            LLIDB_GetElement(i, (struct Element **)&elem);
            if ((elem->flags & 0x13) == 0x13) {
                obj = elem->obj;
                if (obj->parent_element == build_elem) {
                    if ((obj->theme_element == param_elem && obj->menu_element == menu_elem) ||
                        (obj->theme_element == theme_elem && SelectedBuildTheme == 0 && obj->menu_element == menu_elem)) {
                        InsertObjectSortedByCost(obj);
                        matched++;
                    }
                }
            }
        }
    }
    for (i = 0; i < count; i++) {
        LLIDB_GetElement(i, (struct Element **)&elem);
        if ((elem->flags & 0x13) == 0x13) {
            obj = elem->obj;
            if (obj->theme_element == param_elem && obj->parent_element != build_elem && obj->parent_element != NULL) {
                InsertChildIntoList(obj);
            }
        }
    }
    BuildPanelScrollIndex = (unsigned char)SelectedBuildTheme;
    if (matched != 0) {
        MakeUpObjectList(0xd2, 3, 0x21, 0x154);
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004758c0
LEGO_EXPORT void UpdateMenu(void) {
    if (SelectedBuildTheme == 5) {
        return;
    }
    TestMenu(&BuildThemeNames[SelectedBuildTheme * 5]);
}

// FUNCTION: LEGOLAND 0x004758e0
LEGO_EXPORT void RedrawObjectList(struct InterfacePanel *panel, int param_2, int delta) {
    int top;

    if (delta == 0) {
        MoveIcons(0xffff, panel->group, 0, 0);
        return;
    }
    top = panel->content_top;
    if (top + delta > panel->clip_top) {
        delta = panel->clip_top - top;
    } else if (panel->content_bottom + delta < panel->clip_bottom) {
        delta = panel->clip_bottom - panel->content_bottom;
    }
    panel->content_top = top + delta;
    panel->content_bottom = panel->content_bottom + delta;
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

    node = BuildObjectList;
    DestroyIconGroup(param_1);
    panel = (struct InterfacePanel *)malloc(sizeof(struct InterfacePanel));
    if (panel == NULL) {
        return 0;
    }
    panel_arg = param_2;
    if (BuildSidePanelSlideInPending == 1) {
        panel_arg = 0xffffff86;
        BuildSidePanelState = 0;
    }
    icon = SetupInterfacePanelIcons(panel, panel_arg, param_3, 1, param_4, param_1);
    panel->icon = icon;
    x = icon->x;
    panel->clip_left = x;
    panel->content_left = x;
    y = icon->y;
    panel->clip_top = y;
    panel->content_top = y;
    panel->clip_right = icon->width + icon->x;
    panel->clip_bottom = icon->height + icon->y;
    panel->field_4 = 1;
    panel->group = (short)param_1;
    SetNewGroup_Callbacks(0, (void *)RenderBuildObjectIcon, (void *)ObjectListIconHandler);
    while (node != NULL) {
        if (node->flag != 0) {
            AddGBarClassIcon(panel, (struct InfoSource *)node->data, x, y, param_1, 1);
            y = y + 0x42;
            ctx = node;
            node = node->next;
        } else if ((((struct Ride *)ctx->data)->element->flags & 8) != 0) {
            y -= 10;
            CloseChildrenBar(ctx, param_1, (short)x, (short)y);
            y += 0x1a;
            while (node != NULL && node->flag == 0) {
                last_icon = AddGBarClassIcon(panel, (struct InfoSource *)node->data, x, y, param_1, 1);
                last_icon->field_20b = 1;
                ctx = node;
                node = node->next;
                y = y + 0x38;
            }
            y += 10;
            last_icon->field_20b = 2;
        } else {
            y -= 10;
            ListChildrenBar(ctx, param_1, (short)x, (short)y);
            y += 0x24;
            while (node != NULL && node->flag == 0) {
                ctx = node;
                node = node->next;
            }
        }
    }
    AddFullScreenIcon(param_1 + 6);
    panel->field_14 = x;
    panel->content_bottom = y;
    if (y < panel->icon->height + panel->icon->y) {
        icon = FindIcon((unsigned short)(param_1 + 4));
        if (icon != NULL) {
            icon->y = (short)param_3 + (short)param_4 - 0x1e;
            icon->flags |= 0x400;
        }
        icon = FindIcon((unsigned short)(param_1 + 3));
        if (icon != NULL) {
            icon->flags |= 0x400;
        }
        BuildPanelScrollOffsets[*(unsigned int *)&BuildPanelScrollIndex & 0xff] = 0;
    } else {
        RedrawObjectList(panel, 0, BuildPanelScrollOffsets[*(unsigned int *)&BuildPanelScrollIndex & 0xff]);
    }
    BuildSidePanelSlideInPending = 0;
    return 1;
}

// FUNCTION: LEGOLAND 0x00475bb0
LEGO_EXPORT void ListChildrenBar(struct InterfaceListNode *node, int group, short x, short y) {
    struct IconNode *icon;

    // STRING: LEGOLAND 0x004bb4a8
    icon = LoadSpriteIcon("ListChildrenBar.lls", 4, x, y, group);
    icon->string_id = 100;
    icon->string = GetString(100);
    icon->flags |= 0x2002;
    icon->event_handler = (void *)ListChildrenIconHandler;
    icon->field_18p = node;
}

// FUNCTION: LEGOLAND 0x00475c00
LEGO_EXPORT void CloseChildrenBar(struct InterfaceListNode *node, int group, short x, short y) {
    struct IconNode *icon;

    // STRING: LEGOLAND 0x004bb4bc
    icon = LoadSpriteIcon("CloseChildrenBar.lls", 4, x, y, group);
    icon->string_id = 0x65;
    icon->string = GetString(0x65);
    icon->flags |= 0x2002;
    icon->event_handler = (void *)CloseChildrenIconHandler;
    icon->field_18p = node;
}

// FUNCTION: LEGOLAND 0x00475c50
char ListChildrenIconHandler(struct IconNode *param_1, unsigned char param_2) {
    if (param_2 & 2) {
        Hover.type = 0x100;
        ((struct Ride *)((struct InterfaceListNode *)param_1->field_18p)->data)->element->flags |= 8;
        MakeUpObjectList(0xd2, 3, 0x21, 0x154);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00475c90
char CloseChildrenIconHandler(struct IconNode *param_1, unsigned char param_2) {
    if (param_2 & 2) {
        Hover.type = 0x100;
        ((struct Ride *)((struct InterfaceListNode *)param_1->field_18p)->data)->element->flags &= ~8;
        MakeUpObjectList(0xd2, 3, 0x21, 0x154);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00475cd0
LEGO_EXPORT int RAndDLinkedList(unsigned int *entry) {
    struct Element *elem;
    struct Element *build_elem;
    struct Element *param_elem;
    struct Element *menu_elem;
    struct Element *theme_elem;
    struct Ride *obj;
    int count;
    int menu_index;
    int i;

    DelObjectList();
    if (LLIDB_FindElement(BuildMenuName, &build_elem, 0) != 0) {
        exit(1);
    }
    if (LLIDB_FindElement("COMMON THEME", &theme_elem, 0) != 0) {
        exit(1);
    }
    if (LLIDB_FindElement((const char *)entry, &param_elem, 0) != 0) {
        exit(1);
    }
    count = LLIDB_GetCount();
    for (menu_index = 0; menu_index < 4; menu_index++) {
        if (LLIDB_FindElement(BuildCategoryNames[menu_index], &menu_elem, 0) != 0) {
            exit(1);
        }
        for (i = 0; i < count; i++) {
            LLIDB_GetElement(i, (struct Element **)&elem);
            if ((elem->flags & 0x10011) == 0x10011) {
                obj = elem->obj;
                if (obj->parent_element == build_elem) {
                    if (obj->theme_element == param_elem && obj->menu_element == menu_elem) {
                        if ((obj->flags & 0xc000000) == 0) {
                            obj->flags |= 0x4000000;
                        }
                        InsertObjectSortedByCost(obj);
                    } else if (obj->theme_element == theme_elem && SelectedBuildTheme == 0 && menu_index == 0) {
                        if ((obj->flags & 0xc000000) == 0) {
                            obj->flags |= 0x4000000;
                        }
                        InsertObjectSortedByCost(obj);
                    }
                }
            }
        }
    }
    for (i = 0; i < count; i++) {
        LLIDB_GetElement(i, (struct Element **)&elem);
        if ((elem->flags & 0x10011) == 0x10011) {
            obj = elem->obj;
            if (obj->parent_element != build_elem && obj->parent_element != NULL && obj->theme_element == param_elem) {
                if ((obj->flags & 0xc000000) == 0) {
                    obj->flags |= 0x4000000;
                }
                InsertChildIntoList(obj);
            }
        }
    }
    BuildPanelScrollIndex = (unsigned char)SelectedBuildTheme + 4;
    return 1;
}

// FUNCTION: LEGOLAND 0x00475e90
LEGO_EXPORT void DisableSidePanelIcons(void) {
    struct IconNode *node;

    node = (struct IconNode *)IconListHead;
    while (node != NULL) {
        if (node->id == 0xd2 || node->id == 0xd5 || node->id == 0xd6 || node->id == 0xd7) {
            node->flags |= 0x400;
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00475ed0
LEGO_EXPORT void EnableSidePanelIcons(void) {
    struct IconNode *node;

    node = (struct IconNode *)IconListHead;
    while (node != NULL) {
        if (node->id == 0xd2 || node->id == 0xd5 || node->id == 0xd6 || node->id == 0xd7) {
            node->flags &= 0xfffffbff;
        }
        node = node->next;
    }
}

/* Recreates the selected theme build menu after loading a save, unless theme 5 (no menu) is selected; resets the
 * panel motion value to 3. */
// FUNCTION: LEGOLAND 0x00475f10
void RestoreBuildMenuAfterLoad(void) {
    if (SelectedBuildTheme != 5) {
        TestMenu(&BuildThemeNames[SelectedBuildTheme * 5]);
        BuildSidePanelOriginX = 3;
    }
}

/* Selects the build element corresponding to the current edit object, using the object-name mapping unless flag
 * 0x2000000 allows direct reuse. If unavailable, exits placement mode and clears placement input bits. */
// FUNCTION: LEGOLAND 0x00475f40
void SelectMatchingBuildElement(void) {
    struct Ride *obj;
    struct Element *info;
    struct Element *match;
    char **pair;

    obj = (struct Ride *)EditMode.unk8;
    match = NULL;
    info = obj->element;
    if ((obj->flags & 0x2000000) != 0) {
        match = info;
    } else {
        pair = &BuildElementNameMap[0].elem_name;
        do {
            if (_stricmp(pair[-1], info->name) == 0) {
                if (*pair != NULL) {
                    match = (struct Element *)ElemID(*pair);
                } else {
                    match = info;
                }
            }
            pair += 2;
        } while (pair < (char **)&InterfaceSlotValues[1]);
    }
    if (match != NULL && (match->flags & 2) != 0) {
        SetEditObject(match->data);
        return;
    }
    EditMode.unk0 = 0;
    IgnoreNextButtonRelease = 1;
    GamePad = GamePad & 0xffffebff;
}

/* Stores value in slot index (0-3) of InterfaceSlotValues; nothing in the game reads those slots. */
// FUNCTION: LEGOLAND 0x00475fe0
void SetInterfaceSlotValue(int index, unsigned int value) {
    if (index < 0 || index >= 4) {
        return;
    }
    InterfaceSlotValues[index] = value;
}

/* Zeroes the four InterfaceSlotValues. */
// FUNCTION: LEGOLAND 0x00476000
void ClearInterfaceSlotValues(void) {
    int i;

    for (i = 0; i < 4; i++) {
        SetInterfaceSlotValue(i, 0);
    }
}

/* Empty in the original (does nothing). */
// FUNCTION: LEGOLAND 0x00476020
void Empty_00476020(void) {}

// FUNCTION: LEGOLAND 0x00476030
void SetButtonFlashState(int index, unsigned int value) {
    if (index >= 0 && index < 9) {
        ButtonFlashStates[index] = value;
    }
}

// FUNCTION: LEGOLAND 0x00476050
void ClearButtonFlashStates(void) {
    int i;

    i = 0;
    while (i < 9) {
        SetButtonFlashState(i, 0);
        i++;
    }
}

// FUNCTION: LEGOLAND 0x00476070
void SetButtonFlashMask(int mask, unsigned int value) {
    int i;
    int bit;

    i = 0;
    bit = 1;
    while (1) {
        if (mask & bit) {
            SetButtonFlashState(i, value);
        }
        i++;
        bit <<= 1;
        if (i >= 9) {
            break;
        }
    }
}

// FUNCTION: LEGOLAND 0x004760a0
void DrawFlashingButtons(void) {
    int *coords;
    int played;
    int i;

    played = 0;
    Empty_00476020();
    if (EditMode.unk4 == 3) {
        coords = InterfaceIconPos;
        i = 0;
        do {
            if (ButtonFlashStates[i] != 0) {
                if (GetBlink() != 0) {
                    PrintSprite((&LegolandThemeOnSprite)[i], coords[0], coords[1], 0, 0);
                    played = 1;
                } else {
                    PrintSprite((&LegolandThemeOffSprite)[i], coords[0], coords[1], 0, 0);
                }
            }
            coords = coords + 2;
            i = i + 1;
        } while ((INT_PTR)coords < (INT_PTR)&LegolandThemeDeselected);
        if (played != 0 && ButtonFlashWasActive == 0) {
            PlayInstanceOfSample(GameFX[FX_WARNING].sample, 0, 1, 0);
        }
        ButtonFlashWasActive = played;
    }
}

// FUNCTION: LEGOLAND 0x00476140
void SetObjectiveIconShown(int index, int value) {
    struct IconNode *obj;

    obj = (struct IconNode *)ThemeIcons[index];
    if (obj != NULL) {
        if (value != 0) {
            obj->flags &= 0xfffffbff;
            CurrentProfile.flags[index] = 1;
            UpDateCurrentProfile();
        } else {
            obj->flags |= 0x400;
        }
    }
}

// FUNCTION: LEGOLAND 0x00476180
void ShowProfileObjectiveIcons(void) {
    unsigned char *flags;
    struct IconNode **items;
    unsigned int counter;

    flags = CurrentProfile.flags;
    items = (struct IconNode **)ThemeIcons;
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

/* Does nothing but return 1. */
// FUNCTION: LEGOLAND 0x00476220
unsigned char ReturnOne_00476220(void) {
    return 1;
}

/* Does nothing but return 1. */
// FUNCTION: LEGOLAND 0x00476230
unsigned char ReturnOne_00476230(void) {
    return 1;
}

/* Does nothing but return 1. */
// FUNCTION: LEGOLAND 0x00476240
unsigned char ReturnOne_00476240(void) {
    return 1;
}

// FUNCTION: LEGOLAND 0x00476250
void SaveResearchList(void) {
    struct InterfaceResearchNode *node;
    int count;
    int len;

    count = 0;
    for (node = ResearchList; node != NULL; node = node->next) {
        count = count + 1;
    }
    SaveGameWrite(&count, 4);
    node = ResearchList;
    while (node != NULL) {
        len = strlen(((struct Ride *)node->data)->element->name);
        SaveGameWrite(&len, 4);
        SaveGameWrite(((struct Ride *)node->data)->element->name, len);
        SaveGameWrite(&node->field_8, 4);
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x004762f0
void LoadResearchList(void) {
    struct InterfaceResearchNode *node;
    struct Ride *obj;
    char buf[512];
    int count;
    int len;

    node = NULL;
    ResearchList = NULL;
    SaveGameRead(&count, 4);
    while (count--) {
        if (node != NULL) {
            node = node->next = (struct InterfaceResearchNode *)malloc(sizeof(struct InterfaceResearchNode));
        } else {
            node = (struct InterfaceResearchNode *)malloc(sizeof(struct InterfaceResearchNode));
            ResearchList = node;
        }
        SaveGameRead(&len, 4);
        SaveGameRead(buf, len);
        buf[len] = 0;
        obj = ((struct Element *)ElemID(buf))->obj;
        node->data = obj;
        obj->flags &= 0xfbffffff;
        ((struct Ride *)node->data)->flags |= 0x8000000;
        SaveGameRead(&node->field_8, 4);
    }
    if (node != NULL) {
        node->next = NULL;
    }
}

// FUNCTION: LEGOLAND 0x004763d0
LEGO_EXPORT void CleanUpReseachList(void) {
    struct InterfaceResearchNode *node;
    struct InterfaceResearchNode *prev;

    node = ResearchList;
    if (node->field_8 == 0) {
        ResearchList = node->next;
        free(node);
        return;
    }
    prev = node;
    for (node = node->next; node != NULL; node = node->next) {
        if (node->field_8 == 0) {
            prev->next = node->next;
            free(node);
            return;
        }
        prev = node;
    }
}

// FUNCTION: LEGOLAND 0x00476420
LEGO_EXPORT void DeleteReseachList(void) {
    struct InterfaceResearchNode *current;
    struct InterfaceResearchNode *next;

    current = ResearchList;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    ResearchList = NULL;
}

// FUNCTION: LEGOLAND 0x00476460
struct MovieHandle *OpenAviMovie(const char *filename) {
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

    int left;
    video_stream = NULL;
    audio_stream = NULL;
    if (AviOpenCount == 0) {
        AVIFileInit();
    }
    if (AVIFileOpenA(&file, filename, 0, 0) == 0) {
        file_info.streams = 0;
        AVIFileInfoA(file, &file_info, 0x6c);
        for (i = 0; i < file_info.streams; i++) {
            if (AVIFileGetStream(file, &stream, 0, i) != 0) {
                break;
            }
            if (AVIStreamInfoA(stream, &stream_info, 0x8c) == 0) {
                if (stream_info.type == 0x73646976) {
                    video_stream = stream;
                    AVIStreamAddRef(stream);
                    length = stream_info.length;
                    left = stream_info.frame_left;
                    width = stream_info.frame_right - left;
                    rate = stream_info.rate / stream_info.scale;
                    height = stream_info.frame_bottom - stream_info.frame_top;
                } else if (stream_info.type == 0x73647561) {
                    AviSoundBufferFrames = 0x16;
                    audio_stream = stream;
                    AVIStreamAddRef(stream);
                }
            }
        }
        if (video_stream == NULL) {
            if (audio_stream != NULL) {
                AVIStreamRelease(audio_stream);
            }
            AVIFileRelease(file);
        } else {
            handle = (struct MovieHandle *)malloc(sizeof(struct MovieHandle));
            if (handle == NULL) {
                AVIStreamRelease(video_stream);
                if (audio_stream != NULL) {
                    AVIStreamRelease(audio_stream);
                }
                AVIFileRelease(file);
                if (AviOpenCount == 0) {
                    AVIFileExit();
                }
                return NULL;
            } else {
                handle->frame_count = length;
                handle->frame_rate = rate;
                handle->width = width;
                handle->height = height;
                handle->file = file;
                handle->frame = NULL;
                handle->audio_stream = audio_stream;
                handle->video_stream = video_stream;
                ++AviOpenCount;
                return handle;
            }
        }
    }
    if (AviOpenCount == 0) {
        AVIFileExit();
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00476630
void CloseAviMovie(struct MovieHandle *h) {
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
    AviOpenCount = AviOpenCount - 1;
    if (AviOpenCount == 0) {
        AVIFileExit();
    }
}

// FUNCTION: LEGOLAND 0x00476680
int GetPerformanceTime(void) {
    LARGE_INTEGER freq;
    LARGE_INTEGER count;
    unsigned int state;

    state = PerfCounterState;
    if (state == 0) {
        if (QueryPerformanceFrequency(&freq) == 0) {
            state = 1;
            PerfCounterState = state;
        } else {
            state = 2;
            PerfCounterState = state;
            PerfCounterScale = FloatMsPerSecond / (float)freq.QuadPart;
        }
    }
    switch (state) {
    case 2:
        QueryPerformanceCounter(&count);
        return (float)count.QuadPart * PerfCounterScale;
    default:
        return GetTickCount();
    }
}

// FUNCTION: LEGOLAND 0x004766f0
int PlayMovieFrames(struct MovieHandle *handle, void *param_2, int param_3) {
    void *frame;
    int started;
    int frame_index;
    unsigned int target;
    unsigned int prev;
    int audio;

    frame_index = -1;
    target = 0;
    started = 0;
    prev = 0;
    if (handle == NULL) {
        return 0;
    }
    if (handle->video_stream == NULL) {
        return 0;
    }
    audio = OpenMovieAudio(handle);
    AviFrameFormat.biBitCount = 0x10;
    AviFrameFormat.biWidth = handle->width;
    AviFrameFormat.biHeight = handle->height;
    AviFrameFormat.biSizeImage = AviFrameFormat.biHeight * AviFrameFormat.biWidth * 2;
    handle->frame = AVIStreamGetFrameOpen(handle->video_stream, &AviFrameFormat);
    while ((int)target < (int)handle->frame_count) {
        if (param_3 != 0) {
            if (ProcessSystemEvents() == 0) {
                break;
            }
            ReadGameButtons();
            if ((MouseClickFlags & 1) != 0) {
                break;
            }
            if ((CancelKeyState & 7) != 0) {
                MovieSkipRequested = 1;
                break;
            }
            if ((KeyboardState[0x39] & 0x80) != 0) {
                break;
            }
        } else {
            ProcessSystemEvents();
            if (((KeyboardState[0x1d] | KeyboardState[0x9d]) & 0x80) != 0 && (KeyboardState[0x10] & 0x80) != 0) {
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
        DrawMovieFrameDoubled(frame);
        PopRenderingStatus();
        if (started == 0) {
            if (audio != 0) {
                StartMovieAudio(handle);
            }
            started = GetPerformanceTime();
        }
        RenderingComplete();
        if ((unsigned int)((GetPerformanceTime() - started) * handle->frame_rate) / 1000 == target) {
            frame_index = target + 1;
            if (frame_index < (int)handle->frame_count) {
                frame = AVIStreamGetFrame(handle->frame, frame_index);
            }
        } else {
            frame_index = -1;
        }
        while (target == prev) {
            target = (unsigned int)((GetPerformanceTime() - started) * handle->frame_rate) / 1000;
        }
        if (audio != 0) {
            UpdateAviAudioBuffer(target, prev);
        }
        prev = target;
    }
    StopMovieAudio();
    do {
        ProcessSystemEvents();
        ReadGameButtons();
    } while ((MouseClickFlags & 6) != 0);
    AVIStreamGetFrameClose(handle->frame);
    handle->frame = NULL;
    return 1;
}

// FUNCTION: LEGOLAND 0x00476910
int OpenMovieAudio(struct MovieHandle *handle) {
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
    AviAudioSampleSize = info.sample_size;
    AVIStreamReadFormat(handle->audio_stream, 0, 0, &fmt_size);
    fmt = (WAVEFORMATEX *)malloc(fmt_size);
    if (fmt == NULL) {
        return 0;
    }
    AVIStreamReadFormat(handle->audio_stream, 0, fmt, &fmt_size);
    AviPcmFormat.wFormatTag = 1;
    AviPcmFormat.nChannels = fmt->nChannels;
    AviPcmFormat.nSamplesPerSec = fmt->nSamplesPerSec;
    AviPcmFormat.nBlockAlign = fmt->nChannels << 1;
    AviPcmFormat.nAvgBytesPerSec = (AviPcmFormat.nBlockAlign & 0xffff) * fmt->nSamplesPerSec;
    AviPcmFormat.wBitsPerSample = 0x10;
    AviPcmFormat.cbSize = 0;
    if (fmt->wFormatTag != 2 && fmt->wFormatTag != 0x11) {
        AviAudioNeedsAcm = 0;
        AviAudioReady = 0;
        AviSoundBufferSlotCount = AviSoundBufferFrames;
        AviSoundBytesPerFrame = fmt->nAvgBytesPerSec / handle->frame_rate;
        AviSamplesPerFrame = fmt->nSamplesPerSec / handle->frame_rate;
        AviAudioBitsPerSample = fmt->wBitsPerSample;
        AviAudioStream = handle->audio_stream;
        AviSoundBuffer = KLIBAUDIO_CreateAVISoundBuffer(fmt, AviSoundBytesPerFrame * AviSoundBufferFrames);
        return 1;
    }
    AviAudioNeedsAcm = 1;
    AviPcmFormat.wFormatTag = 1;
    AviPcmFormat.nChannels = fmt->nChannels;
    AviPcmFormat.nSamplesPerSec = fmt->nSamplesPerSec;
    AviPcmFormat.nBlockAlign = fmt->nChannels << 1;
    AviPcmFormat.nAvgBytesPerSec = (AviPcmFormat.nBlockAlign & 0xffff) * fmt->nSamplesPerSec;
    AviPcmFormat.wBitsPerSample = 0x10;
    AviPcmFormat.cbSize = 0;
    AcmStreamOpenResult = acmStreamOpen(&AviAcmStream, 0, fmt, &AviPcmFormat, 0, 0, 0, 4);
    if (AcmStreamOpenResult != 0) {
        return 0;
    }
    AviAudioReady = 0;
    AviSoundBufferSlotCount = AviSoundBufferFrames;
    AviSoundBytesPerFrame = AviPcmFormat.nAvgBytesPerSec / handle->frame_rate;
    AviSamplesPerFrame = AviPcmFormat.nSamplesPerSec / handle->frame_rate;
    AviAudioBitsPerSample = AviPcmFormat.wBitsPerSample;
    AviAudioStream = handle->audio_stream;
    AviSoundBuffer = KLIBAUDIO_CreateAVISoundBuffer(&AviPcmFormat, AviSoundBytesPerFrame * AviSoundBufferFrames);
    p = (unsigned int *)&AviAcmStreamHeader;
    for (i = sizeof(AviAcmStreamHeader) / 4; i != 0; i--) {
        *p = 0;
        p++;
    }
    AviAcmStreamHeader.cbDstLength = AviSoundBytesPerFrame;
    AviAcmStreamHeader.cbStruct = sizeof(AviAcmStreamHeader);
    acmStreamSize(AviAcmStream, AviSoundBytesPerFrame, &AviAcmStreamHeader.cbSrcLength, 1);
    AviAcmStreamHeader.pbSrc = malloc(AviAcmStreamHeader.cbSrcLength);
    AviAcmStreamHeader.pbDst = malloc(AviAcmStreamHeader.cbDstLength * 3);
    AviAcmDstBase = AviAcmStreamHeader.pbDst;
    AviAcmDstOffset = 0;
    if (AviAcmStreamHeader.pbSrc != NULL && AviAcmStreamHeader.pbDst != NULL) {
        return 1;
    }
    acmStreamClose(AviAcmStream, 0);
    if (AviAcmStreamHeader.pbSrc != NULL) {
        free(AviAcmStreamHeader.pbSrc);
    }
    if (AviAcmStreamHeader.pbDst != NULL) {
        free(AviAcmStreamHeader.pbDst);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00476bf0
int StartMovieAudio(struct MovieHandle *handle) {
    unsigned int start;
    unsigned int len_start;

    if (handle->audio_stream != NULL && AviSoundBuffer != NULL) {
        start = AVIStreamStart(handle->audio_stream);
        len_start = AVIStreamLength(handle->audio_stream) + AVIStreamStart(handle->audio_stream);
        AviAudioStartSample = start;
        AviAudioSamplePos = start;
        AviAudioEndSample = len_start;
        AviAudioReady = 1;
        UpdateAviAudioBuffer(0, 0);
        KLIBAUDIO_SetAVIVolume(AviSoundBuffer, (int)AviAudioVolume);
        KLIBAUDIO_PlayAVISoundBuffer(AviSoundBuffer, 0);
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00476c90
int StopMovieAudio(void) {
    if (AviAudioReady != 0) {
        AviAudioReady = 0;
        KLIBAUDIO_StopAVISoundBuffer(AviSoundBuffer);
        if (AviAudioNeedsAcm != 0) {
            if (AviAcmStreamHeader.pbSrc != NULL) {
                free(AviAcmStreamHeader.pbSrc);
                AviAcmStreamHeader.pbSrc = NULL;
            }
            if (AviAcmDstBase != NULL) {
                free(AviAcmDstBase);
                AviAcmDstBase = NULL;
            }
            acmStreamClose(AviAcmStream, 0);
        }
        return KLIBAUDIO_DestroyAVISoundBuffer(AviSoundBuffer);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00476d20
int UpdateAviAudioBuffer(unsigned int param_1, int param_2) {
    char *dst;
    int rem;
    int produced;
    int play;
    unsigned int loops;
    register int play_pos;
    int bytes_out;
    unsigned int count;

    play = 0;
    if (AviAudioReady == 0 || AviAudioStream == NULL) {
        return 0;
    }
    do {
        if (param_1 == 0 && param_2 == 0) {
            loops = 0xb;
            AviAudioSamplePos = 0;
            AviSoundBufferSlot = 0;
        } else {
            loops = param_1 - param_2;
            if (loops >= AviSoundBufferSlotCount) {
                KLIBAUDIO_StopAVISoundBuffer(AviSoundBuffer);
                AviAudioSamplePos = AviSamplesPerFrame * param_1;
                play = 1;
                loops = 0xb;
                AviSoundBufferSlot = param_1 % AviSoundBufferSlotCount;
                play_pos = AviSamplesPerFrame * AviSoundBufferSlot;
            } else if (loops == 0) {
                return 1;
            }
        }
        count = loops;
        do {
            dst = (char *)KLIBAUDIO_LockAVISoundBuffer(AviSoundBuffer, AviSoundBytesPerFrame * AviSoundBufferSlot, AviSoundBytesPerFrame);
            if (AviAudioSamplePos < AviAudioEndSample && AviAudioSamplePos >= 0) {
                if (AviAudioNeedsAcm != 0) {
                    produced = 0;
                    if (AviAcmDstBytes != 0) {
                        if (AviAcmDstBytes < AviSoundBytesPerFrame) {
                            memcpy(dst, (char *)AviAcmDstBase + AviAcmDstOffset, AviAcmDstBytes);
                            rem = AviSoundBytesPerFrame - AviAcmDstBytes;
                            AviAcmDstBytes = 0;
                            AviAcmDstOffset = 0;
                            if (AviAudioSamplePos >= AviAudioEndSample) {
                                if (AviAudioBitsPerSample == 8) {
                                    memset(dst + (AviSoundBytesPerFrame - rem), 0x80, rem);
                                } else {
                                    memset(dst + (AviSoundBytesPerFrame - rem), 0, rem);
                                }
                            } else {
                                while (AviAcmDstBytes < AviSoundBytesPerFrame) {
                                    AVIStreamRead(AviAudioStream, AviAudioSamplePos, 0x100, AviAcmStreamHeader.pbSrc, AviSoundBytesPerFrame >> 2, &bytes_out, &param_2);
                                    AviAcmStreamHeader.pbDst = (unsigned char *)AviAcmDstBase + produced;
                                    acmStreamPrepareHeader(AviAcmStream, &AviAcmStreamHeader, 0);
                                    acmStreamConvert(AviAcmStream, &AviAcmStreamHeader, 0x10);
                                    acmStreamUnprepareHeader(AviAcmStream, &AviAcmStreamHeader, 0);
                                    produced += AviAcmStreamHeader.cbDstLengthUsed;
                                    AviAcmDstBytes += AviAcmStreamHeader.cbDstLengthUsed;
                                    AviAudioSamplePos++;
                                }
                                memcpy(dst + (AviSoundBytesPerFrame - rem), AviAcmDstBase, rem);
                                AviAcmDstOffset += rem;
                                AviAcmDstBytes -= rem;
                            }
                        } else {
                            memcpy(dst, (char *)AviAcmDstBase + AviAcmDstOffset, AviSoundBytesPerFrame);
                            AviAcmDstBytes -= AviSoundBytesPerFrame;
                            if (AviAcmDstBytes != 0) {
                                AviAcmDstOffset += AviSoundBytesPerFrame;
                            } else {
                                AviAcmDstOffset = 0;
                            }
                        }
                    } else {
                        while (AviAcmDstBytes < AviSoundBytesPerFrame) {
                            AVIStreamRead(AviAudioStream, AviAudioSamplePos, 0x100, AviAcmStreamHeader.pbSrc, AviSoundBytesPerFrame >> 2, &bytes_out, &param_2);
                            AviAcmStreamHeader.pbDst = (unsigned char *)AviAcmDstBase + produced;
                            acmStreamPrepareHeader(AviAcmStream, &AviAcmStreamHeader, 0);
                            acmStreamConvert(AviAcmStream, &AviAcmStreamHeader, 0x10);
                            acmStreamUnprepareHeader(AviAcmStream, &AviAcmStreamHeader, 0);
                            produced += AviAcmStreamHeader.cbDstLengthUsed;
                            AviAcmDstBytes += AviAcmStreamHeader.cbDstLengthUsed;
                            AviAudioSamplePos++;
                        }
                        memcpy(dst, (char *)AviAcmDstBase + AviAcmDstOffset, AviSoundBytesPerFrame);
                        AviAcmDstOffset += AviSoundBytesPerFrame;
                        AviAcmDstBytes -= AviSoundBytesPerFrame;
                    }
                } else {
                    AVIStreamRead(AviAudioStream, AviAudioSamplePos, AviSamplesPerFrame, dst, AviSoundBytesPerFrame, &bytes_out, &param_2);
                }
            } else {
                if (AviAudioBitsPerSample == 8) {
                    memset(dst, 0x80, AviSoundBytesPerFrame);
                } else {
                    memset(dst, 0, AviSoundBytesPerFrame);
                }
                if (AviAudioNeedsAcm != 0) {
                    AviAudioSamplePos++;
                }
            }
            KLIBAUDIO_UnLockAVISoundBuffer(AviSoundBuffer);
            if (AviAudioNeedsAcm == 0) {
                AviAudioSamplePos += AviSamplesPerFrame;
            }
            AviSoundBufferSlot++;
            if (AviSoundBufferSlot >= AviSoundBufferSlotCount) {
                AviSoundBufferSlot = 0;
            }
            count--;
        } while (count != 0);
    } while (0);
    if (play != 0) {
        KLIBAUDIO_PlayAVISoundBuffer(AviSoundBuffer, play_pos);
        KLIBAUDIO_SetAVIVolume(AviSoundBuffer, (int)AviAudioVolume);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004771f0
int PlayMovie(char *filename, unsigned int param_2, int param_3) {
    int rect[4];
    char path[0x80];
    struct MovieHandle *handle;
    int result;

    rect[0] = 0;
    rect[1] = 0;
    rect[2] = 0x140;
    rect[3] = 0xf0;
    if (param_3 != 0) {
        MovieSkipRequested = 0;
    } else if (MovieSkipRequested != 0) {
        return 0;
    }
    if (lpConfig->field_40 == 0) {
        SpeechCloseFile();
        // STRING: LEGOLAND 0x004bb588
        strcpy(path, "FMV\\");
        SpeechDelayFrames = 4;
        strcat(path, filename);
        // STRING: LEGOLAND 0x004bb56c
        DebugTrace("Attempting to open Movie %s", path);
        handle = OpenAviMovie(path);
        if (handle == NULL) {
            strcpy(path, CdDrivePath);
            strcat(path, filename);
            DebugTrace("Attempting to open Movie %s", path);
            handle = OpenAviMovie(path);
        }
        Empty_0047f850();
        if (handle != NULL) {
            // STRING: LEGOLAND 0x004bb554
            DebugTrace("Movie openned OK (%s)", path);
            Empty_0047f850();
            PauseAllSamples();
            StopInteractiveMusic();
            PushRenderingStatusAndUnlockVideoSurface();
            // STRING: LEGOLAND 0x004bb538
            DebugTrace("Attempting to play movie..");
            Empty_0047f850();
            // STRING: LEGOLAND 0x004bb528
            DBPrintf("Starting Movie\n");
            result = PlayMovieFrames(handle, rect, param_2);
            // STRING: LEGOLAND 0x004bb518
            DBPrintf("Stopping Movie\n");
            // STRING: LEGOLAND 0x004bb508
            DebugTrace("Stopping movie");
            Empty_0047f850();
            CloseAviMovie(handle);
            PopRenderingStatus();
            do {
                ProcessSystemEvents();
                ReadGameButtons();
            } while ((CancelKeyState & 7) != 0);
            ResumeAllSamples();
            ReloadCurrentInteractiveMusicTheme();
            return result;
        }
        return 1;
    }
    return 0;
}
