#include "legoland.h"

#include "bricks.h"
#include "build.h"
#include "clipping.h"
#include "obj_instance.h"

struct ScreenConfig {
    unsigned short width;
    unsigned short height;
    unsigned char pad_4[0x28 - 0x4];
    unsigned int slot;
};

extern struct ScreenConfig *lpConfig;
extern unsigned int DAT_007fe020[4];
extern unsigned int EditMode;
extern unsigned int DAT_008119b4;
extern void *DAT_008119b8;
extern unsigned int GamePad;
extern unsigned int EditCursor;
extern unsigned int DAT_00667c7c;
extern unsigned int DAT_00667cd8;
extern unsigned int DAT_0080ff84;
extern unsigned int DAT_0080ff88;
extern unsigned char DAT_0080ffd4[0xf];
extern unsigned int DAT_0080ffa0[68];
extern unsigned int DAT_0080ffc4;
extern unsigned int DAT_0080ffc8;
extern unsigned int DAT_0080ffcc;
extern unsigned int DAT_00832ba0;
extern char DAT_00832998[256];
extern char DAT_00832a98[256];

extern unsigned int LoadSprite(const char *name, unsigned int flags);
extern void PushRenderingStatusAndLockVideoSurface(void);
extern void PrintSprite(unsigned int sprite, unsigned int a, unsigned int b, unsigned int c, unsigned int d);
extern void PopRenderingStatus(void);
extern void RenderingComplete(void);
extern void KillSprite(unsigned int sprite);
extern void FUN_00474880(void);
extern void DefaultCursor(void *cursor);
extern void BuildCursorPtr(void *cursor, unsigned short a, unsigned short b);
extern void FUN_00481170(void);
extern void FUN_0046fb40(unsigned int a);
extern void DelObjectList(void);
extern void FreeBlokeCounters(void);
extern void FUN_0046cb20(void);
extern void FUN_00483090(void);
extern void FUN_004714e0(void);
extern void ResetInfoStruct(void);
extern void FUN_0049cfc0(void);
extern void FUN_00474ed0(void);
extern void UpDateCurrentProfile(void);
extern char *_strncpy(char *dest, const char *src, unsigned int count);

// FUNCTION: LEGOLAND 0x004585c0
void FUN_004585c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00458640
void InitScreens(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00458740
void RenderFrontEndScreen(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00458830
void FUN_00458830(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004588c0
void FUN_004588c0(void)
{
    struct ScreenConfig *config;
    unsigned int sprite;

    config = lpConfig;
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

    if (sprite != 0) {
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
    int slot = lpConfig->slot;

    if (slot < 0xf) {
        DAT_0080ffd4[slot] = 1;
    }
    FUN_0048a750();
    UpDateCurrentProfile();
}

// FUNCTION: LEGOLAND 0x00458c00
void FUN_00458c00(void) { STUB(); }

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
        DAT_0080ffa0[i] = 0;
    }

    DAT_0080ffc4 = 0x4b;
    DAT_0080ffc8 = 0x64;
    DAT_0080ffcc = 0x4b;
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
        _strncpy(buffer, param1, 256);
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
