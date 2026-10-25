#include "title.h"
#include <ddraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "draw.h"
#include "globals.h"
#include "icon.h"
#include "interface.h"
#include "legoland.h"
#include "math.h"
#include "nerps.h"
#include "options.h"
#include "sound_music.h"
#include "sound_sfx.h"
#include "string.h"
#include "text.h"
#include "tooltip.h"

#include "certificate.h"
#include "help.h"
#include "image_sprite.h"
#include "print_sprite.h"
#include "profile.h"
#include "profile_io.h"
#include "resource.h"
#include "stream.h"
#include "timer.h"

// FUNCTION: LEGOLAND 0x0048fc40
LEGO_EXPORT void InitTitleScreen(void) {
    struct IconNode *icon;

    UnloadGameInterface();
    // STRING: LEGOLAND 0x004bf574
    SPRITE_TitleScreenBk = LoadSprite("TitleScreenBk.lls", 0);

    // STRING: LEGOLAND 0x004bf560
    icon = LoadSpriteIcon("New_on_Title.lls", 4, 0xca, 0x138, 7);
    icon->string_id = 7;
    icon->string = GetString(7);
    icon->flags |= 0x6002;
    icon->event_handler = TitleNewGameHandler;
    if (IsFreePlayUnlocked() == 0) {
        // STRING: LEGOLAND 0x004bf548
        icon->alt_sprite = LoadSprite("Dark_New_On_Title.lls", 4);
        icon->render_func = (void *)RenderBlinkIcon;
        icon->flags |= 8;
    } else {
        icon->alt_sprite = NULL;
    }
    OkKeyHandler = TitleNewGameHandler;

    // STRING: LEGOLAND 0x004bf534
    icon = LoadSpriteIcon("Free_on_Title.lls", 4, 0x9a, 8, 7);
    icon->string_id = 8;
    icon->string = GetString(8);
    icon->flags |= 0x6002;
    icon->event_handler = TitleFreePlayHandler;
    if (IsFreePlayUnlocked() == 0) {
        icon->flags |= 0x400;
    }

    // STRING: LEGOLAND 0x004bf520
    icon = LoadSpriteIcon("Reg_on_Title.lls", 4, 0x18, 0x71, 7);
    icon->string_id = 10;
    icon->string = GetString(10);
    icon->flags |= 0x6002;
    icon->event_handler = TitleRegistrationHandler;

    // STRING: LEGOLAND 0x004bf50c
    icon = LoadSpriteIcon("Exit_On_Title.lls", 4, 0x1e1, 0x13, 7);
    icon->string_id = 1000;
    icon->string = GetString(1000);
    icon->flags |= 0x6002;
    icon->event_handler = OptionsExitIconHandler;
    CancelKeyHandler = OptionsExitIconHandler;

    // STRING: LEGOLAND 0x004bf4f8
    icon = LoadSpriteIcon("Load_on_Title.lls", 4, 0x19, 0x118, 7);
    icon->string_id = 9;
    icon->string = GetString(9);
    icon->flags |= 0x6002;
    icon->event_handler = TitleLoadGameHandler;

    // STRING: LEGOLAND 0x004bf4e0
    icon = LoadSpriteIcon("MovieOn_On_Title.lls", 4, 0x10e, 0xa1, 7);
    icon->string_id = 0x2bf;
    icon->string = GetString(0x2bf);
    icon->flags |= 0x6002;
    icon->event_handler = TitleMovieHandler;
    FrontEndScreenMode.unk0 = 0x898;
}

// FUNCTION: LEGOLAND 0x0048fe20
unsigned char TitleLoadGameHandler(unsigned int param_1, unsigned char param_2) {
    if (FrontEndInputEnabled != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        LoadMode = 1;
        SavedGameScreenFromTitle = 1;
        FrontEndScreenMode.unk8 = 4;
    }
    return 1;
}

/* Handles the title screen New button: closes speech and title graphics, marks a new game, and opens the
 * level-selection screen. */
// FUNCTION: LEGOLAND 0x0048feb0
unsigned char TitleNewGameHandler(unsigned int param_1, unsigned int param_2) {
    if (FrontEndInputEnabled != 0 && (param_2 & 2) != 0) {
        SpeechCloseFile();
        SpeechDelayFrames = 4;
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        CurrentProfile.field_45 = 1;
        RemoveIconGroup(7);
        KillTitleScreenSprites();
        EditMode.unk4 = 2;
        FrontEndScreenMode.unk8 = 6;
        MapStats.level_end = 0;
    }
    return 1;
}

/* Handles the title screen Free Play button: closes speech and switches the front end to the free-play screen. */
// FUNCTION: LEGOLAND 0x0048ff20
unsigned char TitleFreePlayHandler(unsigned int param_1, unsigned int param_2) {
    if (FrontEndInputEnabled != 0 && (param_2 & 2) != 0) {
        SpeechCloseFile();
        SpeechDelayFrames = 4;
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        FrontEndScreenMode.unk8 = 3;
        return 2;
    }
    return 1;
}

/* Handles the title screen registration button by selecting front-end screen zero. */
// FUNCTION: LEGOLAND 0x0048ff70
unsigned char TitleRegistrationHandler(unsigned int param_1, unsigned int param_2) {
    if (FrontEndInputEnabled != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        FrontEndScreenMode.unk8 = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048ffb0
void KillTitleScreenBkSpriteAndRemoveIconGroup7(void) {
    if (SPRITE_TitleScreenBk != 0) {
        KillSprite(SPRITE_TitleScreenBk);
        SPRITE_TitleScreenBk = 0;
    }
    RemoveIconGroup(7);
}

/* Handles the title screen movie button: saves the current screen state and requests front-end screen nine. */
// FUNCTION: LEGOLAND 0x0048ffe0
unsigned char TitleMovieHandler(unsigned int param_1, unsigned int param_2) {
    if (FrontEndInputEnabled != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        SaveScreenState(&TitleSavedScreenFlag, &TitleSavedScreenMode, &TitleSavedEditMode);
        EditMode.unk4 = 2;
        FrontEndScreenMode.unk4 = 0xffffffff;
        FrontEndScreenMode.unk8 = 9;
        FrontEndSubScreenActive = 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490050
unsigned char AdvertGoBackHandler(unsigned int param_1, unsigned char param_2) {
    if (FrontEndInputEnabled != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        KillTitleScreenBkSpriteAndRemoveIconGroup7();
        RestoreScreenState(&TitleSavedScreenFlag, &TitleSavedScreenMode, &TitleSavedEditMode);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490090
unsigned char PlayBillundAvi(unsigned int param_1, unsigned char param_2) {
    if (FrontEndInputEnabled != 0 && (param_2 & 2) != 0) {
        SetPointer(0);
        // STRING: LEGOLAND 0x004bf58c
        PlayMovie("Billund.avi", 1, 1);
        SetPointer(6);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004900d0
unsigned char PlayWindsorAvi(unsigned int param_1, unsigned char param_2) {
    if (FrontEndInputEnabled != 0 && (param_2 & 2) != 0) {
        SetPointer(0);
        // STRING: LEGOLAND 0x004bf598
        PlayMovie("Windsor.avi", 1, 1);
        SetPointer(6);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490110
unsigned char PlayCaliforniaAvi(unsigned int param_1, unsigned char param_2) {
    if (FrontEndInputEnabled != 0 && (param_2 & 2) != 0) {
        SetPointer(0);
        // STRING: LEGOLAND 0x004bf5a4
        PlayMovie("California.avi", 1, 1);
        SetPointer(6);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490150
void InitAdvertScreen(void) {
    struct IconNode *icon;

    // STRING: LEGOLAND 0x004bf5f4
    SPRITE_TitleScreenBk = LoadSprite("AdvertScreen.lls", 0);

    // STRING: LEGOLAND 0x004bf5dc
    icon = LoadSpriteIcon("GoBack_On_Advert.lls", 4, 0x13, 0x14b, 7);
    icon->string_id = 0x26;
    icon->string = GetString(0x26);
    icon->flags |= 0x6002;
    icon->event_handler = AdvertGoBackHandler;
    CancelKeyHandler = AdvertGoBackHandler;
    OkKeyHandler = 0;

    // STRING: LEGOLAND 0x004bf5cc
    icon = LoadSpriteIcon("California.lls", 4, 0x1c, 0x24, 7);
    icon->string_id = 700;
    icon->string = GetString(700);
    icon->flags |= 0x6002;
    icon->event_handler = PlayCaliforniaAvi;

    // STRING: LEGOLAND 0x004bf5c0
    icon = LoadSpriteIcon("Windsor.lls", 4, 0xea, 0x24, 7);
    icon->string_id = 0x2bd;
    icon->string = GetString(0x2bd);
    icon->flags |= 0x6002;
    icon->event_handler = PlayWindsorAvi;

    // STRING: LEGOLAND 0x004bf5b4
    icon = LoadSpriteIcon("billund.lls", 4, 0x1b8, 0x24, 7);
    icon->string_id = 0x2be;
    icon->string = GetString(0x2be);
    icon->flags |= 0x6002;
    icon->event_handler = PlayBillundAvi;
}

// FUNCTION: LEGOLAND 0x00490270
void KillCertificateScreen(void) {
    if (SPRITE_TitleScreenBk != 0) {
        KillSprite(SPRITE_TitleScreenBk);
        SPRITE_TitleScreenBk = 0;
    }
    if (PrintInfoSprite != 0) {
        KillSprite(PrintInfoSprite);
        PrintInfoSprite = 0;
    }
    RemoveIconGroup(7);
}

// FUNCTION: LEGOLAND 0x004902c0
unsigned char CertificateGoBackHandler(unsigned int param_1, unsigned int param_2) {
    if (FrontEndInputEnabled != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        KillCertificateScreen();
        FrontEndScreenMode.unk8 = 1;
        CertificateScreenActive = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490300
unsigned char CertificatePrintHandler(unsigned int param_1, unsigned int param_2) {
    if (FrontEndInputEnabled != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        if (CertificatePrintResultTimer == 0 && CertificatePrintCountdown == 0) {
            CertificatePrintCountdown = 2;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490350
struct Sprite *InitCertificateScreen(void) {
    struct IconNode *icon;
    struct Sprite *result;

    // STRING: LEGOLAND 0x004bf654
    SPRITE_TitleScreenBk = LoadSprite("CertificateScreen.lls", 0);
    CertificateScreenActive = 1;

    // STRING: LEGOLAND 0x004bf638
    icon = LoadSpriteIcon("GoBack_On_Certificate.lls", 4, 12, 14, 7);
    icon->string_id = 0x26;
    icon->string = GetString(0x26);
    icon->flags |= 0x6002;
    icon->event_handler = CertificateGoBackHandler;

    // STRING: LEGOLAND 0x004bf61c
    icon = LoadSpriteIcon("Print_On_Certificate.lls", 4, 0x207, 14, 7);
    icon->string_id = 0x23d;
    icon->string = GetString(0x23d);
    icon->flags |= 0x6002;
    icon->event_handler = CertificatePrintHandler;

    // STRING: LEGOLAND 0x004bf60c
    result = LoadSprite("printinfo.lls", 4);
    PrintInfoSprite = result;
    CertificatePrintResultTimer = 0;
    CertificatePrintCountdown = 0;
    return result;
}

// FUNCTION: LEGOLAND 0x00490410
void RenderCertificateScreen(void) {
    RECT rc;
    int hit[3] = {1};
    if (CertificateScreenActive != 0) {
        rc.top = 0x102;
        rc.bottom = 0x130;
        rc.left = 0;
        rc.right = 0x280;
        NewPrintCent((char *)&CurrentProfile, 0, rc, 0);
        if (CertificatePrintCountdown != 0) {
            PrintSprite(PrintInfoSprite, 0xc6, 0x28, 0, hit);
            rc.top = 0x2d;
            rc.bottom = 0x43;
            rc.left = 0xc6;
            rc.right = (short)PrintInfoSprite->width + 0xc6;
            NewPrintCent(GetString(0x23a), 1, rc, 1);
            CertificatePrintCountdown = CertificatePrintCountdown + -1;
            if (CertificatePrintCountdown == 0) {
                SpeechCloseFile();
                SpeechDelayFrames = 4;
                CertificatePrintResultTimer = PrintCertificateWithCurrentDate() ? 0x8c : -0x8c;
            }
        } else if (0 < CertificatePrintResultTimer) {
            PrintSprite(PrintInfoSprite, 0xc6, 0x28, 0, hit);
            rc.top = 0x2d;
            rc.bottom = 0x43;
            rc.left = 0xc6;
            rc.right = (short)PrintInfoSprite->width + 0xc6;
            NewPrintCent(GetString(0x23b), 1, rc, 1);
            CertificatePrintResultTimer = CertificatePrintResultTimer + -1;
        } else if (CertificatePrintResultTimer < 0) {
            PrintSprite(PrintInfoSprite, 0xc6, 0x28, 0, hit);
            rc.top = 0x2d;
            rc.bottom = 0x43;
            rc.left = 0xc6;
            rc.right = (short)PrintInfoSprite->width + 0xc6;
            NewPrintCent(GetString(0x23c), 1, rc, 1);
            CertificatePrintResultTimer = CertificatePrintResultTimer + 1;
        }
    }
}

/* Sets IntervalScreenIsBriefing: 1 right before the level briefing is shown (level/free play start, briefing
 * button), 0 for mid-level interval reports and after the interval screen closes. */
// FUNCTION: LEGOLAND 0x00490600
void SetIntervalScreenIsBriefing(unsigned int param_1) {
    IntervalScreenIsBriefing = param_1;
}

// FUNCTION: LEGOLAND 0x00490610
void SetPendingMovie(const char *param_1) {
    if (strlen(param_1) > 0xff) {
        strncpy(IntervalMovieName, param_1, 0x100);
        IntervalMovieName[0xff] = 0;
    } else {
        strcpy(IntervalMovieName, param_1);
        (&IntervalMovieNameEnd)[strlen(param_1) + 1] = 0;
    }
}

// FUNCTION: LEGOLAND 0x00490680
unsigned int ReadResourceLines(char *param_1, void **param_2, unsigned int param_3) {
    struct ResFile *file;
    int size;
    char *buffer;
    int pos;
    int count;

    file = RES_OpenFile(param_1);
    if (file == NULL) {
        return 0;
    }
    size = RES_GetFileSize(file);
    if (size == 0) {
        RES_CloseFile(file);
        return 0;
    }
    buffer = malloc(size + 2);
    if (buffer == NULL) {
        RES_CloseFile(file);
        return 0;
    }
    RES_ReadFile(file, buffer, size);
    pos = 0;
    count = 0;
    while (count < (int)param_3 && pos < size) {
        *param_2 = buffer + pos;
        do {
        } while (buffer[pos++] != '\r' && pos < size);
        if (pos >= size) {
            pos++;
        }
        buffer[pos - 1] = 0;
        pos++;
        count++;
        param_2++;
    }
    RES_CloseFile(file);
    return (unsigned int)count;
}

// FUNCTION: LEGOLAND 0x00490740
void SetIntervalReportSpeechPrefix(const char *param_1) {
    char fname[64];

    _splitpath(param_1, 0, 0, fname, 0);
    // STRING: LEGOLAND 0x004bf674
    sprintf(IntervalReportSpeechPrefix, "%s_", fname);
}

// FUNCTION: LEGOLAND 0x00490770
void SetIntervalHintSpeechPrefix(const char *param_1) {
    char fname[64];

    _splitpath(param_1, 0, 0, fname, 0);
    sprintf(IntervalHintSpeechPrefix, "%s_", fname);
}

// FUNCTION: LEGOLAND 0x004907a0
unsigned int LoadIntervalReport(const char *param_1) {
    char buffer[128];
    unsigned int result;

    SetIntervalReportSpeechPrefix(param_1);
    FreeIntervalReport();
    // STRING: LEGOLAND 0x004bf678
    sprintf(buffer, "Intervals\\%s", param_1);
    result = ReadResourceLines(buffer, (void **)IntervalTextLines, 100);
    IntervalTextLastLine = result;
    if (result != 0) {
        IntervalPageFirstLine = 1;
        result = result - 1;
        IntervalTextLastLine = result;
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00490800
void LoadIntervalHints(const char *param_1) {
    char buffer[128];

    SetIntervalHintSpeechPrefix(param_1);
    FreeIntervalHints();
    sprintf(buffer, "Intervals\\%s", param_1);
    IntervalHintCount = ReadResourceLines(buffer, (void **)IntervalHintLines, 32);
}

// FUNCTION: LEGOLAND 0x00490850
void FreeIntervalReport(void) {
    if (IntervalTextLastLine != 0) {
        free(IntervalTextLines[0]);
    }
    IntervalTextLastLine = 0;
}

// FUNCTION: LEGOLAND 0x00490880
void FreeIntervalHints(void) {
    if (IntervalHintCount != 0) {
        free(IntervalHintLines[0]);
    }
    IntervalHintCount = 0;
}

// FUNCTION: LEGOLAND 0x004908b0
void KillIntervalScreen(void) {
    if (SPRITE_TitleScreenBk != 0) {
        KillSprite(SPRITE_TitleScreenBk);
        SPRITE_TitleScreenBk = 0;
    }
    if (NextPageSprite != 0) {
        KillSprite(NextPageSprite);
        NextPageSprite = 0;
    }
    if (NextPageLitSprite != 0) {
        KillSprite(NextPageLitSprite);
        NextPageLitSprite = 0;
    }
    if (PreviousPageSprite != 0) {
        KillSprite(PreviousPageSprite);
        PreviousPageSprite = 0;
    }
    if (PreviousPageLitSprite != 0) {
        KillSprite(PreviousPageLitSprite);
        PreviousPageLitSprite = 0;
    }
    if (RepHint1Sprite != 0) {
        KillSprite(RepHint1Sprite);
        RepHint1Sprite = 0;
    }
    if (RepHint2Sprite != 0) {
        KillSprite(RepHint2Sprite);
        RepHint2Sprite = 0;
    }
    RemoveIconGroup(7);
}

// FUNCTION: LEGOLAND 0x00490970
unsigned char IntervalScreenCloseHandler(unsigned int param_1, unsigned char param_2, unsigned int param_3, unsigned int param_4) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        FrontEndSubScreenActive = 0;
        KillIntervalScreen();
        EditMode.unk4 = 3;
        SetDefaultMouseHandlers();
        FreeIntervalReport();
        FreeIntervalHints();
        SetIntervalScreenIsBriefing(0);
        if (IntervalMovieName[0] != 0) {
            SetPointer(0);
            PlayMovie(IntervalMovieName, 1, 1);
            SetPointer(6);
            SetPendingMovie(EmptyString);
        }
        SpeechCloseFile();
        SpeechDelayFrames = 4;
        ResumeGameTimer();
        ResumeAllSamples();
        ClearAdvisorHelp();
        RepostScriptIntro();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490a20
void PlayIntervalReportSpeech(unsigned int param_1) {
    char buffer[256];

    // STRING: LEGOLAND 0x004bf688
    sprintf(buffer, "%s%02d.wav", IntervalReportSpeechPrefix, param_1 + 1);
    SpeechCloseFile();
    SpeechLoadWavFile(buffer);
    SpeechPlay();
    SetIntervalSpeechHelpKind();
}

// FUNCTION: LEGOLAND 0x00490a60
void PlayIntervalHintSpeech(unsigned int param_1) {
    char buffer[256];

    sprintf(buffer, "%s%02d.wav", IntervalHintSpeechPrefix, param_1 + 1);
    SpeechCloseFile();
    SpeechLoadWavFile(buffer);
    SpeechPlay();
    SetIntervalSpeechHelpKind();
}

// FUNCTION: LEGOLAND 0x00490aa0
void ShowIntervalReportPage(void) {
    if ((int)IntervalPageFirstLine <= 1) {
        IntervalPageFirstLine = 1;
        TitlePreviousPageIcon->flags = TitlePreviousPageIcon->flags | 0x400;
    } else {
        TitlePreviousPageIcon->flags = TitlePreviousPageIcon->flags & 0xfffffbff;
    }
    if ((int)(IntervalPageFirstLine + 0xe) > (int)IntervalTextLastLine) {
        TitleNextPageIcon->flags |= 0x400;
    } else {
        TitleNextPageIcon->flags &= 0xfffffbff;
    }
    PlayIntervalReportSpeech((int)IntervalPageFirstLine / 0xe);
}

// FUNCTION: LEGOLAND 0x00490b20
unsigned char IntervalNextPageHandler(unsigned int param_1, unsigned int param_2) {
    SetIconSprite(TitleNextPageIcon, NextPageLitSprite);
    if ((param_2 & 2) != 0) {
        if ((TitleNextPageIcon->flags & 0x400) != 0) {
            return IntervalScreenCloseHandler(0, param_2, 0, 0);
        }
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        IntervalPageFirstLine = IntervalPageFirstLine + 0xe;
        ShowIntervalReportPage();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490b90
unsigned char IntervalPreviousPageHandler(unsigned int param_1, unsigned int param_2) {
    SetIconSprite(TitlePreviousPageIcon, PreviousPageLitSprite);
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        IntervalPageFirstLine -= 14;
        ShowIntervalReportPage();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490be0
unsigned char IntervalHintHandler(struct IconNode *param_1, unsigned int param_2) {
    SetIconSprite(param_1, RepHint2Sprite);
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(GameFX[FX_BUTTON4].sample, 0, 1, 0);
        do {
            IntervalHintIndex = IntervalHintIndex + 1;
            if ((int)IntervalHintIndex >= (int)IntervalHintCount) {
                IntervalHintIndex = 0;
            }
        } while (strlen(IntervalHintLines[IntervalHintIndex]) == 0);
        IntervalHintBubbleEndTime = GetTicks() + 8000;
        PlayIntervalHintSpeech(IntervalHintIndex);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00490c70
void InitIntervalScreen(void) {
    struct IconNode *accept;
    struct IconNode *icon;

    PauseGameTimer();
    PauseAllSamples();
    // STRING: LEGOLAND 0x004bf6cc
    SPRITE_TitleScreenBk = LoadSprite("Interval_Screen.lls", 0);
    // STRING: LEGOLAND 0x004b8198
    NextPageSprite = LoadSprite("NextPage.lls", 4);
    // STRING: LEGOLAND 0x004b8188
    NextPageLitSprite = LoadSprite("NextPageLit.lls", 4);
    // STRING: LEGOLAND 0x004b8174
    PreviousPageSprite = LoadSprite("PreviousPage.lls", 4);
    // STRING: LEGOLAND 0x004b8160
    PreviousPageLitSprite = LoadSprite("PreviousPageLit.lls", 4);
    // STRING: LEGOLAND 0x004bf6bc
    RepHint1Sprite = LoadSprite("Rep_Hint1.lls", 4);
    // STRING: LEGOLAND 0x004bf6ac
    RepHint2Sprite = LoadSprite("Rep_Hint2.lls", 4);

    // STRING: LEGOLAND 0x004bf694
    accept = LoadSpriteIcon("Accept_On_Report.lls", 4, 0x20a, 0x16c, 7);
    if (IntervalScreenIsBriefing != 0) {
        accept->string_id = 0x244;
        accept->string = GetString(0x244);
    } else {
        accept->string_id = 0x245;
        accept->string = GetString(0x245);
    }
    accept->flags |= 0x6002;
    accept->event_handler = IntervalScreenCloseHandler;
    CancelKeyHandler = IntervalScreenCloseHandler;

    icon = InsertIcon(0x1b9, 0x50, 7, RepHint1Sprite);
    icon->string_id = 0x2d0;
    icon->string = GetString(0x2d0);
    icon->event_handler = (void *)IntervalHintHandler;
    icon->flags |= 0x6002;
    RepHint1Icon = icon;
    if (IntervalHintCount == 0) {
        icon->flags |= 0x400;
    }

    IntervalHintBubbleEndTime = 0;
    IntervalHintIndex = 0xffffffff;
    TitleNextPageIcon = InsertIcon(0x1b9, 0x1ae, 7, NextPageSprite);
    TitleNextPageIcon->string_id = 0x88e;
    TitleNextPageIcon->string = GetString(0x88e);
    TitleNextPageIcon->flags |= 0x2000;
    TitleNextPageIcon->flags |= 0x4002;
    TitleNextPageIcon->event_handler = (void *)IntervalNextPageHandler;
    OkKeyHandler = IntervalNextPageHandler;

    TitlePreviousPageIcon = InsertIcon(6, 0x1ae, 7, PreviousPageSprite);
    TitlePreviousPageIcon->string_id = 0x88f;
    TitlePreviousPageIcon->string = GetString(0x88f);
    TitlePreviousPageIcon->flags |= 0x2000;
    TitlePreviousPageIcon->flags |= 0x4002;
    TitlePreviousPageIcon->event_handler = (void *)IntervalPreviousPageHandler;
    IntervalPageFirstLine = 1;
    ShowIntervalReportPage();
}

// FUNCTION: LEGOLAND 0x00490ea0
void UpdateIntervalScreenIcons(void) {
    unsigned int _x = MousePos.x;
    unsigned int _y = MousePos.y;
    if ((int)_x < TitleNextPageIcon->x ||
        (int)MousePos.x > TitleNextPageIcon->width + TitleNextPageIcon->x ||
        (int)MousePos.y < TitleNextPageIcon->y ||
        (int)MousePos.y > TitleNextPageIcon->height + TitleNextPageIcon->y) {
        if (GetBlink() != 0) {
            SetIconSprite(TitleNextPageIcon, NextPageSprite);
        } else {
            SetIconSprite(TitleNextPageIcon, NextPageLitSprite);
        }
    }
    if ((int)MousePos.x < TitlePreviousPageIcon->x ||
        (int)MousePos.x > TitlePreviousPageIcon->width + TitlePreviousPageIcon->x ||
        (int)MousePos.y < TitlePreviousPageIcon->y ||
        (int)MousePos.y > TitlePreviousPageIcon->height + TitlePreviousPageIcon->y) {
        SetIconSprite(TitlePreviousPageIcon, PreviousPageSprite);
    }
    if ((int)MousePos.x < RepHint1Icon->x ||
        (int)MousePos.x > RepHint1Icon->width + RepHint1Icon->x ||
        (int)MousePos.y < RepHint1Icon->y ||
        (int)MousePos.y > RepHint1Icon->height + RepHint1Icon->y) {
        SetIconSprite(RepHint1Icon, RepHint1Sprite);
    }
}

// FUNCTION: LEGOLAND 0x00490fa0
void DrawClippedText(char *text, int font, RECT rc, INT_PTR color_flag) {
    HRGN region = CreateRectRgn(SPRITE_ClipRect.left, SPRITE_ClipRect.top, SPRITE_ClipRect.right, SPRITE_ClipRect.bottom);
    HDC hdc;
    HGDIOBJ old_region;

    PushRenderingStatusAndUnlockVideoSurface();
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &hdc);
    SetBkMode(hdc, 1);
    if ((char)color_flag == 1) {
        SetTextColor(hdc, 0xffffff);
    }
    old_region = SelectObject(hdc, region);
    color_flag = (INT_PTR)SelectFont(hdc, font); /* the old font */
    DrawTextA(hdc, text, strlen(text), &rc, 0x24);
    SelectObject(hdc, (HGDIOBJ)color_flag);
    SelectObject(hdc, old_region);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
    PopRenderingStatus();
    DeleteObject(region);
}

// FUNCTION: LEGOLAND 0x00491080
void PrintIntervalLine(char *param_1, int param_2, int param_3, int param_4, int param_5) {
    RECT rc;

    if (param_1 != NULL) {
        rc.left = param_2;
        rc.top = param_3;
        rc.right = param_2 + 0x1cc;
        rc.bottom = param_4 + param_3;
        if (param_5 != 0) {
            NewPrintCent(param_1, 3, rc, 0);
        } else {
            DrawClippedText(param_1, 2, rc, 0);
        }
    }
}

// FUNCTION: LEGOLAND 0x004910f0
void RenderIntervalScreen(void) {
    int rc[4];
    char **line;
    int y;
    int n;
    int idx;

    PrintIntervalLine(IntervalTextLines[0], 10, 0x45, 0x27, 1);
    y = 0x6b;
    n = 0;
    line = &IntervalTextLines[IntervalPageFirstLine];
    idx = IntervalPageFirstLine;
    while (n < 0xe && idx <= (int)IntervalTextLastLine) {
        PrintIntervalLine(*line, 10, y, 0x16, 0);
        n = n + 1;
        idx = idx + 1;
        line = line + 1;
        y = y + 0x18;
    }
    UpdateIntervalScreenIcons();
    if (IntervalHintBubbleEndTime != 0) {
        rc[0] = 0x1db;
        rc[1] = 5;
        rc[2] = 0x280;
        rc[3] = 0x78;
        if ((int)GetTicks() > (int)IntervalHintBubbleEndTime) {
            IntervalHintBubbleEndTime = 0;
        }
        BubbleHelp(rc, IntervalHintLines[IntervalHintIndex], 2);
    }
}

// FUNCTION: LEGOLAND 0x004911c0
unsigned int ShowIntervalScreen(const char *a, const char *b) {
    if (LoadIntervalReport(a) != 0) {
        if (b != 0) {
            LoadIntervalHints(b);
        }
        FrontEndSubScreenActive = 1;
        EditMode.unk4 = 2;
        FrontEndScreenMode.unk4 = 0xffffffff;
        FrontEndScreenMode.unk8 = 7;
        if (BriefingIcon != 0) {
            ((struct IconNode *)BriefingIcon)->flags &= 0xfffffbff;
        }
        return 1;
    }
    if (BriefingIcon != 0) {
        ((struct IconNode *)BriefingIcon)->flags |= 0x400;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00491240
unsigned int HasIntervalReport(const char *param_1) {
    unsigned int result;

    if (BriefingIcon == 0) {
        return 0;
    }
    if (LoadIntervalReport(param_1) != 0) {
        ((struct IconNode *)BriefingIcon)->flags &= 0xfffffbff;
        result = 1;
    } else {
        ((struct IconNode *)BriefingIcon)->flags |= 0x400;
        result = 0;
    }
    FreeIntervalReport();
    return result;
}

// FUNCTION: LEGOLAND 0x00491290
LEGO_EXPORT void InitNewProfilePoPUp(struct IconNode *profile) {
    struct IconNode *icon = profile;
    struct IconNode *popup;

    // STRING: LEGOLAND 0x004bf6e4
    popup = LoadSpriteIcon("Reg_NewProfileBK.lls", 4, icon->x, icon->y - 0x1b, 0x15);
    popup->string_id = 0x50;
    popup->string = GetString(0x50);
    popup->flags |= 0x2000;
    EnterNewProfileCheckBoxIcons(popup);
    ResetTempProfile();
}
