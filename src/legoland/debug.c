#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdcheck.h"
#include "debug.h"
#include "debug_alloc.h"
#include "draw.h"
#include "globals.h"
#include "image_sprite.h"
#include "input.h"
#include "legoland.h"
#include "llidb.h"
#include "resource.h"
#include "saveload.h"
#include "screens.h"
#include "stream.h"
#include "string.h"

// FUNCTION: LEGOLAND 0x0047f870
void DebugTrace(const char *fmt, ...) {}

// Strings used by GameMain.
// GLOBAL: LEGOLAND 0x004bcd44
static const char GameLogFileName[] = "legoland.log";
// GLOBAL: LEGOLAND 0x004bcd28
static const char ResourceOpenErrorFormat[] = "Failed to open resource %s";
// GLOBAL: LEGOLAND 0x004bcd18
static const char ErrorBoxTitle[] = "LEGOLAND Error";
// GLOBAL: LEGOLAND 0x004bcd08
static const char EraseItSpriteName[] = "erase it.lls";
// GLOBAL: LEGOLAND 0x004bccf8
static const char EraseIt2SpriteName[] = "erase it2.lls";
// GLOBAL: LEGOLAND 0x004bcce8
static const char NoBuildSpriteName[] = "no build.lls";
// GLOBAL: LEGOLAND 0x004bccd8
static const char YesBuildSpriteName[] = "yes build.lls";
// GLOBAL: LEGOLAND 0x004bccc4
static const char RabOverIconSpriteName[] = "rab over icon.lls";
// GLOBAL: LEGOLAND 0x004bccb0
static const char RabOverIcon2SpriteName[] = "rab over icon2.lls";
// GLOBAL: LEGOLAND 0x004bcca0
static const char QuestionItSpriteName[] = "question it.lls";
// GLOBAL: LEGOLAND 0x004bcc8c
static const char QuestionIt2SpriteName[] = "question it2.lls";
// GLOBAL: LEGOLAND 0x004bcc78
static const char AttractionsMenuName[] = "ATTRACTIONS MENU";
// GLOBAL: LEGOLAND 0x004bcc64
static const char FoodStoresMenuName[] = "FOOD STORES MENU";
// GLOBAL: LEGOLAND 0x004bcc54
static const char SceneryMenuName[] = "SCENERY MENU";
// GLOBAL: LEGOLAND 0x004bcc48
static const char ShopsMenuName[] = "SHOPS MENU";
// GLOBAL: LEGOLAND 0x004bcc28
static const char ShutdownTraceMessage[] = "Finished shutting stuff down";

// GameMain: the game's top level. Opens the log, checks the CD, opens the three resource volumes,
// starts graphics and input, loads the shared pointer sprites and registers the build-menu
// elements, runs the game (RunGameApplication), then shuts everything down again.
// Returns 0 on a normal exit, 1 if any start-up step failed.
// Still differs: the "open the three volumes" loop ends with a signed `jl` here and an unsigned `jb` in
// the original (docs/attempts/0x0047f880-GameMain.md).

// FUNCTION: LEGOLAND 0x0047f880
int GameMain(void) {
    char buffer[1024];
    int i;
    int j;
    struct ResVolume **vol;

    lpConfig->field_1e = 1;

    if (OpenLogFile(GameLogFileName) == 0) {
        return 1;
    }
    if (WaitForLegolandCd(1) == 0) {
        return 1;
    }

    for (i = 0; i < 3; i++) {
        ResourceVolumes[i] = RES_OpenVolume(ResourceFileNames[i]);
        if (ResourceVolumes[i] == NULL) {
            sprintf(buffer, ResourceOpenErrorFormat, ResourceFileNames[i]);
            MessageBoxA(GetDesktopWindow(), buffer, ErrorBoxTitle, 0x30);
            for (j = 0; j < i; j++) {
                RES_CloseVolume(ResourceVolumes[j]);
            }
            return 1;
        }
    }

    LoadStringTable();
    InitHostSystemGPU();
    if (InitScreen() == 0) {
        MessageBoxA(GetDesktopWindow(), GetString(0xcc), GetString(0xcb), 0x30);
        KillHostSystemGPU();
        for (vol = ResourceVolumes; vol < ResourceVolumes + 3; vol++) {
            RES_CloseVolume(*vol);
        }
        return 1;
    }

    if (InitInputSystem() == 0) {
        MessageBoxA(GetDesktopWindow(), GetString(0x9c4), GetString(0xcb), 0x30);
        KillInputSystem();
        KillHostSystemGPU();
        for (vol = ResourceVolumes; vol < ResourceVolumes + 3; vol++) {
            RES_CloseVolume(*vol);
        }
        return 1;
    }

    PointerSprites[0] = 0;
    PointerSprites[1] = LoadSprite(EraseItSpriteName, 0);
    PointerSprites[2] = LoadSprite(EraseIt2SpriteName, 0);
    PointerSprites[3] = LoadSprite(NoBuildSpriteName, 0);
    PointerSprites[4] = LoadSprite(YesBuildSpriteName, 0);
    PointerSprites[5] = LoadSprite(RabOverIconSpriteName, 0);
    PointerSprites[6] = LoadSprite(RabOverIcon2SpriteName, 0);
    PointerSprites[7] = LoadSprite(QuestionItSpriteName, 0);
    PointerSprites[8] = LoadSprite(QuestionIt2SpriteName, 0);

    LLIDB_LoadICM();

    LLIDB_RegisterNewElement(BuildMenuName, 0, 0x200);
    LLIDB_RegisterNewElement(AttractionsMenuName, 0, 0x200);
    LLIDB_RegisterNewElement(FoodStoresMenuName, 0, 0x200);
    LLIDB_RegisterNewElement(SceneryMenuName, 0, 0x200);
    LLIDB_RegisterNewElement(ShopsMenuName, 0, 0x200);

    RunGameApplication();

    KillHostSystemGPU();

    for (vol = ResourceVolumes; vol < ResourceVolumes + 3; vol++) {
        if (*vol != 0) {
            RES_CloseVolume(*vol);
        }
    }

    DebugTrace(ShutdownTraceMessage);

    CloseLogFile();
    DeleteStrings();
    LLIDB_CloseICM();

    if (PointerSprites[3] != 0) {
        KillSprite(PointerSprites[3]);
        PointerSprites[3] = 0;
    }
    if (PointerSprites[4] != 0) {
        KillSprite(PointerSprites[4]);
        PointerSprites[4] = 0;
    }
    if (PointerSprites[7] != 0) {
        KillSprite(PointerSprites[7]);
        PointerSprites[7] = 0;
    }
    if (PointerSprites[8] != 0) {
        KillSprite(PointerSprites[8]);
        PointerSprites[8] = 0;
    }
    if (PointerSprites[1] != 0) {
        KillSprite(PointerSprites[1]);
        PointerSprites[1] = 0;
    }
    if (PointerSprites[2] != 0) {
        KillSprite(PointerSprites[2]);
        PointerSprites[2] = 0;
    }
    if (PointerSprites[5] != 0) {
        KillSprite(PointerSprites[5]);
        PointerSprites[5] = 0;
    }
    if (PointerSprites[6] != 0) {
        KillSprite(PointerSprites[6]);
        PointerSprites[6] = 0;
    }

    return 0;
}

// FUNCTION: LEGOLAND 0x0047fc20
LEGO_EXPORT unsigned int mystrlen(const char *s) {
    if (s == NULL) {
        return 0;
    }
    return strlen(s);
}

// FUNCTION: LEGOLAND 0x0047fc40
char *StrIStr(const char *haystack, const char *needle) {
    char *upper_haystack;
    char *upper_needle;
    char *match;

    upper_haystack = (char *)malloc(strlen(haystack) + 1);
    if (upper_haystack == NULL) {
        return NULL;
    }
    strcpy(upper_haystack, haystack);
    _strupr(upper_haystack);

    upper_needle = (char *)malloc(strlen(needle) + 1);
    if (upper_needle == NULL) {
        free(upper_haystack);
        return NULL;
    }
    strcpy(upper_needle, needle);
    _strupr(upper_needle);

    match = strstr(upper_haystack, upper_needle);
    if (match != NULL) {
        match = (char *)(haystack + (match - upper_haystack));
    }
    free(upper_haystack);
    free(upper_needle);
    return match;
}

// FUNCTION: LEGOLAND 0x0047fd10
int __cdecl wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HANDLE mutex;
    SECURITY_ATTRIBUTES sa;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = FALSE;
    // STRING: LEGOLAND 0x004bcdb0
    mutex = CreateMutexA(&sa, TRUE, "LegolandGameMutex");
    if (mutex == NULL) {
        // STRING: LEGOLAND 0x004bcd98
        DBPrintf("Couldn't create Mutex\n");
        return 0;
    }

    if (WaitForSingleObject(mutex, 0) == WAIT_TIMEOUT) {
        // STRING: LEGOLAND 0x004bcd7c
        DBPrintf("Program already running.\n");
        CloseHandle(mutex);
        return 0;
    }

    // STRING: LEGOLAND 0x004bcd70
    if (StrIStr(lpCmdLine, "WINDEBUG")) {
        BlitFrameFunc = BlitFrameToWindow;
        WinDebugMode = 1;
        // STRING: LEGOLAND 0x004bcd6c
    } else if (StrIStr(lpCmdLine, "BLT")) {
        BlitFrameFunc = BlitFrameToWindow;
    }

    // STRING: LEGOLAND 0x004bcd60
    if (StrIStr(lpCmdLine, "-nointro")) {
        lpConfig->field_40 = 1;
    } else {
        lpConfig->field_40 = 0;
    }

    // STRING: LEGOLAND 0x004bcd54
    MusicEnabled = StrIStr(lpCmdLine, "-nomusic") ? 0 : 1;
    g_hInstance = hInstance;
    g_nCmdShow = nCmdShow;

    if (CheckHostSystemGPU() == 0) {
        return 0;
    }
    return GameMain();
}
