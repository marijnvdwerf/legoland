#include "legoland.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ddraw.h>
#include "globals.h"
#include "worker_mouse.h"
#include "popupinfo.h"
#include "icon.h"
#include "text.h"

#pragma intrinsic(strlen, strcpy, memcpy)

struct Sprite;

struct InfoIcon {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ short x;
    /* 0x0e */ short y;
    /* 0x10 */ short field_10;
    /* 0x12 */ short field_12;
    /* 0x14 */ unsigned char pad_14[0x2c - 0x14];
    /* 0x2c */ void (*handler)(void);
    /* 0x30 */ unsigned char pad_30[0x4];
    /* 0x34 */ unsigned int flags;
    /* 0x38 */ char *string;
    /* 0x3c */ unsigned int string_id;
};

struct InfoObjData {
    /* 0x00 */ unsigned char pad_0[0x4];
    /* 0x04 */ unsigned int field_4;
    /* 0x08 */ unsigned int field_8;
    /* 0x0c */ unsigned int field_c;
    /* 0x10 */ unsigned char pad_10[0x18 - 0x10];
    /* 0x18 */ int field_18;
    /* 0x1c */ struct InfoObjInner *field_1c;
};

struct InfoObjInner {
    /* 0x00 */ unsigned char pad_0[0x60];
    /* 0x60 */ unsigned char field_60;
};


#include "image_sprite.h"
#include "stream.h"
#include "worker.h"
#include "timer.h"
#include "help.h"
#include "print_sprite.h"
#include "debug_alloc.h"
#include "llidb.h"
#include "string.h"
#include "gamemap.h"
#include "map_object.h"
#include "controller.h"
#include "draw.h"
#include "sound_music.h"
#include "tilemap.h"

struct NewObjInfo {
    /* 0x00 */ unsigned char pad_0[0x26];
    /* 0x26 */ short field_26;
    /* 0x28 */ unsigned char pad_28[0x78 - 0x28];
    /* 0x78 */ char *field_78;
    /* 0x7c */ unsigned char pad_7c[0x80 - 0x7c];
    /* 0x80 */ char *field_80;
    /* 0x84 */ unsigned char pad_84[0xc4 - 0x84];
    /* 0xc4 */ char **name;
};


// FUNCTION: LEGOLAND 0x00470bb0
LEGO_EXPORT void InitPopUpInfo(void) {
    unsigned int *puVar3;
    int iVar2;

    puVar3 = &DAT_007fdec0;
    for (iVar2 = 0x40; iVar2 != 0; iVar2 = iVar2 + -1) {
        *puVar3 = 0;
        puVar3 = puVar3 + 1;
    }
    // STRING: LEGOLAND 0x004b89ac
    if (LLIDB_FindElement("POTTING SHED", &DAT_007fdfb0, 0) != 0) {
        _exit(1);
    }
    // STRING: LEGOLAND 0x004b899c
    if (LLIDB_FindElement("MECHANICS HUT", &DAT_007fdfb4, 0) != 0) {
        _exit(1);
    }
    // STRING: LEGOLAND 0x004b8a70
    if (LLIDB_FindElement("PATH CONTROL", &DAT_007fdfb8, 0) != 0) {
        _exit(1);
    }
    // STRING: LEGOLAND 0x004b83d0
    if (LLIDB_FindElement("ENTRANCE 1", &DAT_007fdfbc, 0) != 0) {
        _exit(1);
    }
    if (DAT_00668958 == NULL) {
        DAT_00668958 = (struct Sprite *)1;
        // STRING: LEGOLAND 0x004baca4
        DAT_006688e0 = LoadSprite("PU_BGMain.lls", 4);
        // STRING: LEGOLAND 0x004bac90
        DAT_006688e4 = LoadSprite("PU_BGCentreTop.lls", 4);
        // STRING: LEGOLAND 0x004bac7c
        DAT_006688e8 = LoadSprite("PU_BGRightTop.lls", 4);
        // STRING: LEGOLAND 0x004bac68
        DAT_006688ec = LoadSprite("PU_BGLeftMid.lls", 4);
        // STRING: LEGOLAND 0x004bac54
        DAT_006688f0 = LoadSprite("PU_BGCentreMid.lls", 4);
        // STRING: LEGOLAND 0x004bac40
        DAT_006688f4 = LoadSprite("PU_BGRightMid.lls", 4);
        // STRING: LEGOLAND 0x004bac2c
        DAT_006688f8 = LoadSprite("PU_BGLeftBtm.lls", 4);
        // STRING: LEGOLAND 0x004bac18
        DAT_006688fc = LoadSprite("Pu_BGCentreBtm.lls", 4);
        // STRING: LEGOLAND 0x004bac04
        DAT_00668900 = LoadSprite("PU_BGRightBtm.lls", 4);
        // STRING: LEGOLAND 0x004babf4
        DAT_00668910 = LoadSprite("NewPopMock.lls", 4);
        // STRING: LEGOLAND 0x004babe0
        DAT_007fe004 = LoadSprite("ObjectRepairOK.lls", 4);
        // STRING: LEGOLAND 0x004babcc
        DAT_007fdeb0 = LoadSprite("ObjectNoRepair1.lls", 4);
        // STRING: LEGOLAND 0x004babb4
        DAT_00668914 = LoadSprite("PU_DeleteObjectON.lls", 4);
        // STRING: LEGOLAND 0x004baba0
        DAT_00668918 = LoadSprite("PU_DeleteObject.lls", 4);
        // STRING: LEGOLAND 0x004bab8c
        DAT_0066891c = LoadSprite("PU_ClosePopUpON.lls", 4);
        // STRING: LEGOLAND 0x004bab78
        DAT_00668920 = LoadSprite("PU_ClosePopUp.lls", 4);
        // STRING: LEGOLAND 0x004bab60
        DAT_00668944 = LoadSprite("PU_AddGardenerON.lls", 4);
        // STRING: LEGOLAND 0x004bab4c
        DAT_00668948 = LoadSprite("PU_AddGardener.lls", 4);
        // STRING: LEGOLAND 0x004bab34
        DAT_0066894c = LoadSprite("PU_AddMechanicsON.lls", 4);
        // STRING: LEGOLAND 0x004bab20
        DAT_00668950 = LoadSprite("PU_AddMechanics.lls", 4);
        // STRING: LEGOLAND 0x004bab10
        DAT_00668928 = LoadSprite("NextIcon.lls", 4);
        // STRING: LEGOLAND 0x004bab00
        DAT_00668924 = LoadSprite("NextIconOn.lls", 4);
        // STRING: LEGOLAND 0x004baaf0
        DAT_00668930 = LoadSprite("PrevIcon.lls", 4);
        // STRING: LEGOLAND 0x004baae0
        DAT_0066892c = LoadSprite("PrevIconOn.lls", 4);
        // STRING: LEGOLAND 0x004baad4
        DAT_007fdfc8 = LoadSprite("i_sad.lls", 0);
        // STRING: LEGOLAND 0x004baac8
        DAT_007fdfe4 = LoadSprite("i_norm.lls", 0);
        // STRING: LEGOLAND 0x004baabc
        DAT_007fe018 = LoadSprite("i_happy.lls", 0);
        // STRING: LEGOLAND 0x004baaac
        DAT_007fdfd0 = LoadSprite("i_hungry.lls", 0);
        // STRING: LEGOLAND 0x004baa9c
        DAT_007fe008 = LoadSprite("i_peckish.lls", 0);
        // STRING: LEGOLAND 0x004baa90
        DAT_007fdeac = LoadSprite("i_full.lls", 0);
    }
    DAT_007fdfe0 = (struct InfoIcon *)InsertIcon(0, 0, 0x2c3, DAT_00668948);
    DAT_007fdfe0->string_id = 0x6e;
    DAT_007fdfe0->string = GetString(0x6e);
    DAT_007fdfe0->flags |= 0x2000;
    DAT_007fdfe0->flags |= 0x4002;
    DAT_007fdfe0->flags |= 0x400;
    DAT_007fdfe0->handler = FUN_004733f0;
    DAT_007fdea4 = (struct InfoIcon *)InsertIcon(0, 0, 0x2c3, DAT_00668950);
    DAT_007fdea4->string_id = 0x6f;
    DAT_007fdea4->string = GetString(0x6f);
    DAT_007fdea4->flags |= 0x2000;
    DAT_007fdea4->flags |= 0x4002;
    DAT_007fdea4->flags |= 0x400;
    DAT_007fdea4->handler = FUN_00473460;
    DAT_007fdfdc = (struct InfoIcon *)InsertIcon(0, 0, 0x2c3, DAT_00668918);
    DAT_007fdfdc->string_id = 0x70;
    DAT_007fdfdc->string = GetString(0x70);
    DAT_007fdfdc->flags |= 0x2000;
    DAT_007fdfdc->flags |= 0x4002;
    DAT_007fdfdc->flags |= 0x400;
    DAT_007fdfdc->handler = FUN_004731a0;
    DAT_007fdfc0 = (struct InfoIcon *)InsertIcon(0, 0, 0x2c3, DAT_00668920);
    DAT_007fdfc0->string_id = 0x73;
    DAT_007fdfc0->string = GetString(0x73);
    DAT_007fdfc0->flags |= 0x2000;
    DAT_007fdfc0->flags |= 0x4002;
    DAT_007fdfc0->flags |= 0x400;
    DAT_007fdfc0->handler = FUN_004730f0;
    DAT_007fdfc4 = (struct InfoIcon *)InsertIcon(0, 0, 0x2c3, DAT_00668928);
    DAT_007fdfc4->string_id = 0x88e;
    DAT_007fdfc4->string = GetString(0x88e);
    DAT_007fdfc4->flags |= 0x2000;
    DAT_007fdfc4->flags |= 0x4002;
    DAT_007fdfc4->flags |= 0x400;
    DAT_007fdfc4->handler = FUN_00473360;
    DAT_007fdfe8 = (struct InfoIcon *)InsertIcon(0, 0, 0x2c3, DAT_00668930);
    DAT_007fdfe8->string_id = 0x88f;
    DAT_007fdfe8->string = GetString(0x88f);
    DAT_007fdfe8->flags |= 0x2000;
    DAT_007fdfe8->flags |= 0x4002;
    DAT_007fdfe8->flags |= 0x400;
    DAT_007fdfe8->handler = FUN_004733b0;
    // STRING: LEGOLAND 0x004baa7c
    DAT_007fdfd8 = (struct InfoIcon *)LoadSpriteIcon("PU_CornerMask.lls", 4, 0, 0, 0x2c3);
    DAT_007fdfd8->flags |= 0x400;
    DAT_007fdfcc = (struct InfoIcon *)InsertIcon(0, 0, 0x2c3, DAT_00668918);
    DAT_007fdfcc->string_id = 0xa1;
    DAT_007fdfcc->string = GetString(0xa1);
    DAT_007fdfcc->flags |= 0x2000;
    DAT_007fdfcc->flags |= 0x4002;
    DAT_007fdfcc->flags |= 0x400;
    DAT_007fdfcc->handler = FUN_004734d0;
    FUN_00470950(FUN_004731e0, FUN_00473310);
}

// FUNCTION: LEGOLAND 0x00471170
int FUN_00471170(void) {
    if (DAT_00668958 != NULL) {
        DAT_00668958 = NULL;

        if (DAT_006688e0 != NULL) {
            KillSprite(DAT_006688e0);
            DAT_006688e0 = NULL;
        }
        if (DAT_006688e4 != NULL) {
            KillSprite(DAT_006688e4);
            DAT_006688e4 = NULL;
        }
        if (DAT_006688e8 != NULL) {
            KillSprite(DAT_006688e8);
            DAT_006688e8 = NULL;
        }
        if (DAT_006688f0 != NULL) {
            KillSprite(DAT_006688f0);
            DAT_006688f0 = NULL;
        }
        if (DAT_006688ec != NULL) {
            KillSprite(DAT_006688ec);
            DAT_006688ec = NULL;
        }
        if (DAT_006688f4 != NULL) {
            KillSprite(DAT_006688f4);
            DAT_006688f4 = NULL;
        }
        if (DAT_006688f8 != NULL) {
            KillSprite(DAT_006688f8);
            DAT_006688f8 = NULL;
        }
        if (DAT_006688fc != NULL) {
            KillSprite(DAT_006688fc);
            DAT_006688fc = NULL;
        }
        if (DAT_00668900 != NULL) {
            KillSprite(DAT_00668900);
            DAT_00668900 = NULL;
        }
        if (DAT_007fe004 != NULL) {
            KillSprite(DAT_007fe004);
            DAT_007fe004 = NULL;
        }
        if (DAT_007fdeb0 != NULL) {
            KillSprite(DAT_007fdeb0);
            DAT_007fdeb0 = NULL;
        }
        if (DAT_00668914 != NULL) {
            KillSprite(DAT_00668914);
            DAT_00668914 = NULL;
        }
        if (DAT_00668918 != NULL) {
            KillSprite(DAT_00668918);
            DAT_00668918 = NULL;
        }
        if (DAT_0066891c != NULL) {
            KillSprite(DAT_0066891c);
            DAT_0066891c = NULL;
        }
        if (DAT_00668920 != NULL) {
            KillSprite(DAT_00668920);
            DAT_00668920 = NULL;
        }
        if (DAT_00668928 != NULL) {
            KillSprite(DAT_00668928);
            DAT_00668928 = NULL;
        }
        if (DAT_00668924 != NULL) {
            KillSprite(DAT_00668924);
            DAT_00668924 = NULL;
        }
        if (DAT_00668930 != NULL) {
            KillSprite(DAT_00668930);
            DAT_00668930 = NULL;
        }
        if (DAT_0066892c != NULL) {
            KillSprite(DAT_0066892c);
            DAT_0066892c = NULL;
        }
        if (DAT_00668944 != NULL) {
            KillSprite(DAT_00668944);
            DAT_00668944 = NULL;
        }
        if (DAT_00668948 != NULL) {
            KillSprite(DAT_00668948);
            DAT_00668948 = NULL;
        }
        if (DAT_0066894c != NULL) {
            KillSprite(DAT_0066894c);
            DAT_0066894c = NULL;
        }
        if (DAT_00668950 != NULL) {
            KillSprite(DAT_00668950);
            DAT_00668950 = NULL;
        }
        if (DAT_007fdfc8 != NULL) {
            KillSprite(DAT_007fdfc8);
            DAT_007fdfc8 = NULL;
        }
        if (DAT_007fdfe4 != NULL) {
            KillSprite(DAT_007fdfe4);
            DAT_007fdfe4 = NULL;
        }
        if (DAT_007fe018 != NULL) {
            KillSprite(DAT_007fe018);
            DAT_007fe018 = NULL;
        }
        if (DAT_007fdfd0 != NULL) {
            KillSprite(DAT_007fdfd0);
            DAT_007fdfd0 = NULL;
        }
        if (DAT_007fe008 != NULL) {
            KillSprite(DAT_007fe008);
            DAT_007fe008 = NULL;
        }
        if (DAT_007fdeac != NULL) {
            KillSprite(DAT_007fdeac);
            DAT_007fdeac = NULL;
        }
        FUN_00470b00();
    }
}

// FUNCTION: LEGOLAND 0x00471450
LEGO_EXPORT int UnLoad_PopUpInfo(void) {
    RemoveIconGroup(0x2c3);
    return FUN_00471170();
}

// FUNCTION: LEGOLAND 0x00471470
void FUN_00471470(void) {
    DAT_007fdfdc->handler = NULL;
    DAT_007fdfe0->handler = NULL;
    DAT_007fdea4->handler = NULL;
    DAT_007fdfcc->handler = NULL;
}

// FUNCTION: LEGOLAND 0x004714a0
void FUN_004714a0(void) {
    DAT_007fdfdc->handler = FUN_004731a0;
    DAT_007fdfc0->handler = FUN_004730f0;
    DAT_007fdfe0->handler = FUN_004733f0;
    DAT_007fdea4->handler = FUN_00473460;
    DAT_007fdfcc->handler = FUN_004734d0;
}

// FUNCTION: LEGOLAND 0x004714e0
void FUN_004714e0(void) {
    while (DAT_007fdf74 != 0) {
        FUN_00471ca0(DAT_007fded4);
    }
}

// FUNCTION: LEGOLAND 0x00471510
LEGO_EXPORT void ResetInfoStruct(void) {
    DAT_007fdf7c = 0;
    DAT_007fdf8c = 0;
    DAT_007fdf84 = 0;
    DAT_007fdf9c = 0;
    DAT_007fdfa0 = 0;
    DAT_007fdfa4 = 0;
    DAT_007fdfa8 = 0;
    DAT_007fdf98 = 0;
    DAT_007fdfac = 0;
    DisableInfoPopUPIcons();
    FUN_004714a0();
}

// FUNCTION: LEGOLAND 0x00471550
LEGO_EXPORT void PopInfoSizeMayChange(void) {
    if (DAT_007fdfa0 != 0) {
        DAT_007fdfa8 = 1;
    }
}

// FUNCTION: LEGOLAND 0x00471570
LEGO_EXPORT void StopFollowingBloke(void) {
    if (DAT_007fdf98 == 0) {
        return;
    }
    ResetInfoStruct();
}

// FUNCTION: LEGOLAND 0x00471580
LEGO_EXPORT void DisableInfoPopUPIcons(void) {
    DAT_007fdfdc->flags |= 0x400;
    DAT_007fdfc0->flags |= 0x400;
    DAT_007fdfd8->flags |= 0x400;
    DAT_007fdea8->flags |= 0x400;
    DAT_007fe000->flags |= 0x400;
    DAT_007fdfe0->flags |= 0x400;
    DAT_007fdea4->flags |= 0x400;
    DAT_007fdfcc->flags |= 0x400;
    DAT_007fdfc4->flags |= 0x400;
    DAT_007fdfe8->flags |= 0x400;
}

// FUNCTION: LEGOLAND 0x00471610
void FUN_00471610(void) {
    FUN_0046d680((struct IconNode *)DAT_007fdfdc, DAT_00668918);
    FUN_0046d680((struct IconNode *)DAT_007fdfe8, DAT_00668930);
    FUN_0046d680((struct IconNode *)DAT_007fdfc4, DAT_00668928);
    FUN_0046d680((struct IconNode *)DAT_007fdfc0, DAT_00668920);
    FUN_0046d680((struct IconNode *)DAT_007fdfe0, DAT_00668948);
    FUN_0046d680((struct IconNode *)DAT_007fdea4, DAT_00668950);
    FUN_0046d680((struct IconNode *)DAT_007fdfcc, DAT_00668918);
}

// FUNCTION: LEGOLAND 0x004716a0
LEGO_EXPORT void InfoPrintCent(int len, char *text, int font, RECT rc, int flag) {
    HRGN region = CreateRectRgn(SPRITE_ClipRect.left, SPRITE_ClipRect.top, SPRITE_ClipRect.right, SPRITE_ClipRect.bottom);
    HDC hdc;
    HGDIOBJ old_region;
    HGDIOBJ old_font;

    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &hdc);
    SetBkMode(hdc, 1);
    SetTextColor(hdc, 0xffffff);
    old_region = SelectObject(hdc, region);
    old_font = (HGDIOBJ)SelectFont(hdc, font);
    if (flag != 0) {
        DrawTextA(hdc, text, strlen(text), &rc, 0x11);
    } else {
        DrawTextA(hdc, text, strlen(text), &rc, 0x10);
    }
    SelectObject(hdc, old_font);
    SelectObject(hdc, old_region);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
    DeleteObject(region);
}

// FUNCTION: LEGOLAND 0x004717a0
int FUN_004717a0(const char *param_1, int param_2, int param_3, int param_4, int param_5, int param_6) {
    HDC hdc;
    int iVar2;
    int iVar4;
    RECT local_10;

    local_10.left = 0;
    local_10.top = 0;
    local_10.right = 0;
    local_10.bottom = 0;
    hdc = CreateCompatibleDC(NULL);
    iVar4 = param_2;
    param_2 = 0;
    local_10.right = param_4;
    SelectFont(hdc, param_6);
    while (1) {
        iVar2 = DrawTextA(hdc, param_1, strlen(param_1), &local_10, 0x411);
        if (iVar2 <= iVar4) {
            return param_2;
        }
        iVar4 = iVar4 + param_3;
        local_10.right = local_10.right + param_5;
        param_2 = param_2 + 1;
        if (iVar2 <= iVar4) {
            return param_2;
        }
    }
}

// FUNCTION: LEGOLAND 0x00471840
int FUN_00471840(const char *param_1, int param_2, int param_3, int param_4, int param_5, int param_6) {
    HDC hdc;
    RECT local_10;

    local_10.left = 0;
    local_10.top = 0;
    local_10.right = 0;
    local_10.bottom = 0;
    hdc = CreateCompatibleDC(NULL);
    local_10.right = param_4;
    SelectFont(hdc, param_6);
    DrawTextA(hdc, param_1, strlen(param_1), &local_10, 0x401);
    return (((local_10.right - local_10.left) - param_4) + 0x1f) >> 5;
}

// FUNCTION: LEGOLAND 0x004718c0
void FUN_004718c0(int param_1) {
    int x;
    int y;
    int iVar1;
    int edx;

    x = DAT_007fdecc;
    y = DAT_007fded0;
    iVar1 = param_1 * 0x20 + 0xc8;
    edx = param_1 * 0x14 + 0x96;
    if (x > 0x27b - iVar1) {
        x = x + (-5 - iVar1);
        if (x < 0) {
            x = 5;
            goto LAB_0047190b;
        }
    }
    if (x >= 0x82) {
        goto LAB_00471919;
    }
LAB_0047190b:
    if (DAT_007fdd80 != 2) {
        x = 0x82;
    }
LAB_00471919:
    if (y < 0x25) {
        DAT_007fdecc = x;
        DAT_007fded0 = 0x25;
        return;
    }
    iVar1 = 0x16f - edx;
    DAT_007fdecc = x;
    DAT_007fded0 = iVar1;
    if (y <= iVar1) {
        DAT_007fded0 = y;
    }
}

// FUNCTION: LEGOLAND 0x00471950
LEGO_EXPORT void PopUpInfoSetUp(int param_1, void *param_2, unsigned int param_3, unsigned int param_4, unsigned int param_5) {
    short sVar1;
    int iVar2;
    int x;
    int y;
    unsigned int uVar3;
    unsigned char *iVar4;
    int iVar5;

    uVar3 = param_3 & 0xffff;
    x = uVar3 & 0xff;
    y = uVar3 >> 8;
    if ((x < 0) || (lpConfig->width <= x) || (y < 0) || (lpConfig->height <= y)) {
        iVar4 = NULL;
    } else {
        iVar4 = (unsigned char *)GameMap[y] + x * 0x14;
    }
    ResetInfoStruct();
    DAT_007fdfa0 = 1;
    DAT_007fdfa8 = 1;
    DAT_007fdecc = param_4;
    DAT_007fded0 = param_5;
    DAT_007fdf88 = (unsigned short)param_3;
    DAT_007fdec0 = param_1;
    DAT_007fdec4 = param_2;
    DAT_007fdec8 = param_3;
    if (param_1 < 0x308) {
        if (param_1 == 0x307) {
            if (*(short *)((char *)param_2 + 0xc) != 5) {
                PlayInstanceOfSample(DAT_004b92e4, 0, 1, 0);
                FUN_00470100(0x307, param_2);
                ResetInfoStruct();
                return;
            }
        } else if (param_1 == 0x103) {
            if (param_2 == NULL) {
                DAT_007fdfa0 = 1;
                DAT_007fdfa8 = 1;
                return;
            }
            if (((unsigned int)param_2 != DAT_007fdfb8) && ((unsigned int)param_2 != DAT_007fdfbc)) {
                DAT_007fdf7c = *(unsigned int *)((char *)param_2 + 0xc);
                DAT_007fdf84 = iVar4;
                if (*(unsigned int *)(DAT_007fdf7c + 0xc4) == DAT_007fdfb0) {
                    DAT_007fdfa0 = 0;
                    DAT_007fdf9c = 10;
                    param_4 = iVar4[4];
                    param_5 = iVar4[5];
                    if (FUN_0049a120() == 0) {
                        return;
                    }
                    GenerateGardener(&param_4, 1);
                    return;
                }
                if (*(unsigned int *)(DAT_007fdf7c + 0xc4) != DAT_007fdfb4) {
                    DAT_007fdf9c = 0x103;
                    return;
                }
                DAT_007fdfa0 = 0;
                DAT_007fdf9c = 0x14;
                param_4 = iVar4[4];
                param_5 = iVar4[5];
                if (FUN_0049a160() == 0) {
                    return;
                }
                GenerateMechanic(&param_4, 1);
                return;
            }
        } else if (param_1 == 0x306) {
            DAT_007fdf9c = param_1;
            DAT_007fdf8c = (unsigned int)param_2;
            DAT_007fdf90 = *(unsigned int *)(*(int *)((char *)param_2 + 4) + 0x1c);
            DAT_007fdf94 = *(unsigned int *)(*(int *)((char *)param_2 + 4) + 0x20);
            return;
        }
    } else if ((param_1 == 0x308) && (sVar1 = *(short *)((char *)param_2 + 0xc), sVar1 != 5)) {
        if (((sVar1 == 0x13) && (0x6a < *(unsigned char *)((char *)param_2 + 0x60))) ||
            ((sVar1 == 0x16) && (0x6a < *(unsigned char *)((char *)param_2 + 0x60)))) {
            iVar4 = *(unsigned char **)((char *)param_2 + 0x50);
            iVar5 = *(int *)(iVar4 + 8);
            if ((iVar5 < 0) ||
                (((int)(unsigned int)lpConfig->width <= iVar5 ||
                  (iVar2 = *(int *)(iVar4 + 0xc), iVar2 < 0)) ||
                 ((int)(unsigned int)lpConfig->height <= iVar2))) {
                iVar5 = 0;
            } else {
                iVar5 = (int)GameMap[iVar2] + iVar5 * 0x14;
            }
            *(unsigned short *)(iVar5 + 0xc) = *(unsigned short *)(iVar5 + 0xc) & 0xbfff;
            FUN_00499eb0(iVar4);
        }
        PlayInstanceOfSample(DAT_004b9308, 0, 1, 0);
        FUN_00470100(0x308, param_2);
    }
    ResetInfoStruct();
}

// FUNCTION: LEGOLAND 0x00471bf0
void FUN_00471bf0(void) {
    if (DAT_007fdfa0 != 2) {
        DAT_007fdf78 = 0;
        DAT_007fdf74 = 0;
    }
}

// FUNCTION: LEGOLAND 0x00471c10
void FUN_00471c10(struct NewObjInfo *param_1) {
    struct Sprite *sprite;
    char local_200[512];

    if ((int)DAT_007fdf74 < 0x14) {
        // STRING: LEGOLAND 0x004bacd8
        sprintf(local_200, "NewObjIcons\\%s.bmp", *param_1->name);
        sprite = LoadSprite(local_200, 0);
        (&DAT_007fdf24)[DAT_007fdf74] = sprite;
        if ((&DAT_007fdf24)[DAT_007fdf74] != NULL) {
            (&DAT_007fded4)[DAT_007fdf74] = param_1;
            DAT_007fdf74 = DAT_007fdf74 + 1;
            return;
        }
        // STRING: LEGOLAND 0x004bacb4
        DBPrintf("Failied to open New Obj graphic %s\n", local_200);
    }
}

// FUNCTION: LEGOLAND 0x00471ca0
void FUN_00471ca0(void *param) {
    struct Sprite **puVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    struct Sprite **puVar6;
    int iVar7;

    if (0 < (int)DAT_007fdf74) {
        puVar6 = &DAT_007fdf24;
        iVar5 = DAT_007fdf74;
        iVar7 = 1;
        do {
            if (param == ((void **)puVar6)[-0x14]) {
                if (*puVar6 != NULL) {
                    KillSprite(*puVar6);
                    *puVar6 = NULL;
                    iVar5 = DAT_007fdf74;
                }
                puVar2 = puVar6;
                iVar3 = iVar7;
                iVar4 = iVar5;
                if (iVar7 < iVar5) {
                    do {
                        iVar3 = iVar3 + 1;
                        *puVar2 = puVar2[1];
                        ((void **)puVar2)[-0x14] = ((void **)puVar2)[-0x13];
                        puVar2 = puVar2 + 1;
                        iVar4 = DAT_007fdf74;
                    } while (iVar3 < DAT_007fdf74);
                }
                iVar5 = iVar4 + -1;
                if (iVar5 <= DAT_007fdf78) {
                    DAT_007fdf78 = iVar4 + -2;
                }
                DAT_007fdf74 = iVar5;
                if ((iVar5 == 0) && (DAT_007fdfa0 == 2)) {
                    DAT_007fdfa0 = 0;
                }
            }
            puVar6 = puVar6 + 1;
            iVar7 = iVar7 + 1;
        } while (iVar7 + -1 < iVar5);
    }
}

// FUNCTION: LEGOLAND 0x00471d40
void FUN_00471d40(void) {
    if (DAT_007fdf74 != 0) {
        ResetInfoStruct();
        DAT_007fdfa0 = 2;
    }
}

// FUNCTION: LEGOLAND 0x00471d60
void FUN_00471d60(void) {
    FUN_0046d680((struct IconNode *)DAT_007fdea8, DAT_00668938);
    FUN_0046d680((struct IconNode *)DAT_007fe000, DAT_0066893c);
}

// FUNCTION: LEGOLAND 0x00471d90
void FUN_00471d90(void) {
    int iVar1;
    short sVar2;
    char *str;
    unsigned int uVar4;
    int iVar5;
    int iVar6;
    struct PrintCtx ctx;
    char local_14[20];

    iVar6 = DAT_007fdecc;
    uVar4 = DAT_007fdfac & 0xff;
    iVar5 = uVar4 * 0x14;
    ctx.node = 0;
    iVar1 = DAT_007fded0 + 0x48 + iVar5;
    ctx.field_8 = 0;
    ctx.flags = 1;
    PrintSprite(DAT_00668904, DAT_007fdecc, iVar1, 0, (int *)&ctx);
    iVar6 = iVar6 + 0x7a;
    for (; uVar4 != 0; uVar4 = uVar4 - 1) {
        PrintSprite(DAT_00668908, iVar6, iVar1, 0, (int *)&ctx);
        iVar6 = iVar6 + 0x20;
    }
    PrintSprite(DAT_0066890c, iVar6, iVar1, 0, (int *)&ctx);
    DAT_007fdea8->flags = DAT_007fdea8->flags & 0xfffffbff;
    DAT_007fdea8->x = (short)iVar6 + 3;
    sVar2 = (short)iVar1 + 3;
    DAT_007fdea8->y = sVar2;
    DAT_007fe000->flags = DAT_007fe000->flags & 0xfffffbff;
    DAT_007fe000->x = (short)iVar6 + 0x27;
    DAT_007fe000->y = sVar2;
    str = GetString(0xa2);
    sprintf(local_14, (char *)DAT_004b8bbc, str);
    FUN_00455e50(local_14, DAT_007fdecc + 0xc, iVar1 + 6,
                 (DAT_007fdecc + 0x86 + iVar5) - (DAT_007fdecc + 0xc), (iVar1 + 0x21) - (iVar1 + 6), 1, 5,
                 0xff0000, 0xffffff);
    if ((DAT_007fe000->x + 0x24 < (int)DAT_00813a44) || ((int)DAT_00813a44 < DAT_007fdea8->x)) {
        FUN_00471d60();
    }
    if ((iVar1 + 0x1b < (int)DAT_00813a48) || ((int)DAT_00813a48 < iVar1)) {
        FUN_00471d60();
    }
    FUN_00471470();
}

// FUNCTION: LEGOLAND 0x00471f10
void FUN_00471f10(void) {
    int iVar1;
    int iVar2;
    unsigned int uVar3;
    unsigned int uVar4;
    int iVar5;
    int iVar6;
    int iVar7;
    unsigned int local_1c;
    struct PrintCtx ctx;

    iVar2 = DAT_007fded0;
    iVar1 = DAT_007fdecc;
    ctx.node = 0;
    ctx.flags = 1;
    ctx.field_8 = 0;
    uVar4 = DAT_007fdfac & 0xff;
    PrintSprite(DAT_006688e0, DAT_007fdecc, DAT_007fded0, 0, (int *)&ctx);
    iVar6 = iVar1 + 0xbc;
    iVar7 = iVar6;
    for (uVar3 = uVar4; uVar3 != 0; uVar3 = uVar3 - 1) {
        PrintSprite(DAT_006688e4, iVar7, iVar2, 0, (int *)&ctx);
        iVar7 = iVar7 + 0x20;
    }
    PrintSprite(DAT_006688e8, iVar7, iVar2, 0, (int *)&ctx);
    if (uVar4 != 0) {
        iVar7 = iVar2 + 99;
        local_1c = uVar4;
        do {
            PrintSprite(DAT_006688ec, iVar1, iVar7, 0, (int *)&ctx);
            uVar3 = uVar4;
            iVar5 = iVar6;
            do {
                PrintSprite(DAT_006688f0, iVar5, iVar7, 0, (int *)&ctx);
                iVar5 = iVar5 + 0x20;
                uVar3 = uVar3 - 1;
            } while (uVar3 != 0);
            PrintSprite(DAT_006688f4, iVar5, iVar7, 0, (int *)&ctx);
            iVar7 = iVar7 + 0x14;
            local_1c = local_1c - 1;
        } while (local_1c != 0);
    }
    iVar7 = iVar2 + 99 + uVar4 * 0x14;
    PrintSprite(DAT_00668900, iVar1, iVar7, 0, (int *)&ctx);
    for (; uVar4 != 0; uVar4 = uVar4 - 1) {
        PrintSprite(DAT_006688fc, iVar6, iVar7, 0, (int *)&ctx);
        iVar6 = iVar6 + 0x20;
    }
    PrintSprite(DAT_006688f8, iVar6, iVar7, 0, (int *)&ctx);
}

// FUNCTION: LEGOLAND 0x00472090
void FUN_00472090(void) {
}

// FUNCTION: LEGOLAND 0x004720a0
void FUN_004720a0(void) {
    short sVar2;
    short sVar3;
    short sVar4;
    unsigned int uVar5;
    struct NewObjInfo *obj;
    struct PrintCtx ctx;
    char local_80[128];

    ctx.node = 0;
    ctx.flags = 1;
    ctx.field_8 = 0;
    PrintSprite(DAT_00668910, 0xbe, 0x28, 0, (int *)&ctx);
    DAT_007fdfe8->flags = DAT_007fdfe8->flags & 0xfffffbff;
    DAT_007fdfe8->x = 0xc1;
    DAT_007fdfe8->y = 0x46;
    sVar2 = DAT_007fdfc0->field_10;
    sVar3 = DAT_00668910->width;
    DAT_007fdfc4->flags = DAT_007fdfc4->flags & 0xfffffbff;
    DAT_007fdfc4->x = (sVar3 - sVar2) + 0xbb;
    DAT_007fdfc4->y = 0x46;
    sVar2 = DAT_00668910->width;
    sVar3 = DAT_00668910->height;
    sVar4 = DAT_007fdfc0->field_12;
    DAT_007fdfc0->flags = DAT_007fdfc0->flags & 0xfffffbff;
    DAT_007fdfc0->x = (sVar2 - DAT_007fdfc0->field_10) + 0xbb;
    DAT_007fdfc0->y = (sVar3 - sVar4) + 0x25;
    PushRenderingStatusAndUnlockVideoSurface();
    if (DAT_007fdf74 == 1) {
        // STRING: LEGOLAND 0x004bad04
        sprintf(local_80, "You have a new object");
    } else {
        // STRING: LEGOLAND 0x004bacec
        sprintf(local_80, "You have %d new objects", DAT_007fdf74);
    }
    FUN_00455e50(local_80, 0xc1, 0x30, DAT_00668910->width + -6, 0x14, 2, 5, 0xff0000, 0xffffff);
    obj = (struct NewObjInfo *)(&DAT_007fded4)[DAT_007fdf78];
    FUN_00455e50(obj->field_78, 0xc1, 0x4b, DAT_00668910->width + -6, 0x14, 2, 5, 0xff0000, 0xffffff);
    obj = (struct NewObjInfo *)(&DAT_007fded4)[DAT_007fdf78];
    FUN_00455e50(obj->field_80, 0x13e, 0x68, 0xfc, 0x77, 2, 0x10, 0xff0000, 0xffffff);
    obj = (struct NewObjInfo *)(&DAT_007fded4)[DAT_007fdf78];
    // STRING: LEGOLAND 0x004b8a80
    sprintf(local_80, "%d", obj->field_26);
    FUN_00455e50(local_80, 0xf0, 0xd1, 0x43, 0x12, 2, 1, 0xff0000, 0xffffff);
    PopRenderingStatus();
    PrintSprite((&DAT_007fdf24)[DAT_007fdf78], 0xc4, 100, 0, 0);
    if (((unsigned int)(int)DAT_007fdfc0->field_10 <= (unsigned int)(DAT_00813a44 - DAT_007fdfc0->x)) ||
        ((unsigned int)(int)DAT_007fdfc0->field_12 <= (unsigned int)(DAT_00813a48 - DAT_007fdfc0->y))) {
        FUN_0046d680((struct IconNode *)DAT_007fdfc0, DAT_00668920);
    }
    if (((unsigned int)(int)DAT_007fdfc4->field_10 <= (unsigned int)(DAT_00813a44 - DAT_007fdfc4->x)) ||
        ((unsigned int)(int)DAT_007fdfc4->field_12 <= (unsigned int)(DAT_00813a48 - DAT_007fdfc4->y))) {
        FUN_0046d680((struct IconNode *)DAT_007fdfc4, DAT_00668928);
    }
    if (((unsigned int)(int)DAT_007fdfe8->field_10 <= (unsigned int)(DAT_00813a44 - DAT_007fdfe8->x)) ||
        ((unsigned int)(int)DAT_007fdfe8->field_12 <= (unsigned int)(DAT_00813a48 - DAT_007fdfe8->y))) {
        FUN_0046d680((struct IconNode *)DAT_007fdfe8, DAT_00668930);
    }
    if (DAT_007fdf78 == 0) {
        uVar5 = DAT_007fdfe8->flags | 0x400;
    } else {
        uVar5 = DAT_007fdfe8->flags & 0xfffffbff;
    }
    DAT_007fdfe8->flags = uVar5;
    if (DAT_007fdf78 == (int)(DAT_007fdf74 + -1)) {
        DAT_007fdfc4->flags = DAT_007fdfc4->flags | 0x400;
        return;
    }
    DAT_007fdfc4->flags = DAT_007fdfc4->flags & 0xfffffbff;
}

// FUNCTION: LEGOLAND 0x004723f0
void FUN_004723f0(void) {
    unsigned int local_8[2];
    struct Cursor local_cursor;
    void *saved_class;
    struct ObjClass *cls;
    unsigned int v;

    saved_class = QueryClass;
    v = DAT_007fdec8 & 0xffff;
    memcpy(&local_cursor, &QueryCursor, sizeof(struct Cursor));
    local_8[0] = v & 0xff;
    local_8[1] = v >> 8;
    cls = *(struct ObjClass **)((char *)DAT_007fdec4 + 0xc);
    QueryCursor.field_1408 = v >> 8;
    QueryClass = cls;
    QueryCursor.field_1404 = v & 0xff;
    cls->method_94(cls->field_c4, local_8);
    BuildCursorPtr(&QueryCursor, 0, 0);
    FUN_0045f4b0(&QueryCursor);
    memcpy(&QueryCursor, &local_cursor, sizeof(struct Cursor));
    QueryClass = saved_class;
}

// FUNCTION: LEGOLAND 0x004724a0
LEGO_EXPORT void DrawPopUpInfo(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004730f0
unsigned char FUN_004730f0(void *param1, unsigned char param2, unsigned int param3, unsigned int param4) {
    FUN_00471610();

    if (param1) {
        FUN_0046d680(param1, DAT_0066891c);
    }

    if (param2 & 0x2) {
        FUN_004714e0();
        ResetInfoStruct();
    }

    return 1;
}

// FUNCTION: LEGOLAND 0x00473130
unsigned int FUN_00473130(void) {
    if (DAT_007fdfa0 != 0) {
        FUN_004730f0(NULL, 0x2, 0, 0);
        return 1;
    } else {
        return 0;
    }
}

// FUNCTION: LEGOLAND 0x00473160
unsigned int FUN_00473160(void) {
    unsigned int state = DAT_007fdfa0;

    if (state == 1) {
        ResetInfoStruct();
        return 1;
    }

    if (state == 2) {
        FUN_00473360(NULL, state, 0, 0);
        return 1;
    }

    return 0;
}

// FUNCTION: LEGOLAND 0x004731a0
unsigned char FUN_004731a0(void *param_1, unsigned char param_2) {
    FUN_00471610();
    FUN_0046d680(param_1, DAT_00668914);
    if ((param_2 & 2) != 0) {
        FUN_00471470();
        DAT_007fdfa4 = 1;
        return 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004731e0
unsigned char FUN_004731e0(void *param_1, unsigned char flags) {
    unsigned int local_8[2];
    struct Cursor local_cursor;
    void *saved_class;
    struct ObjClass *cls;
    unsigned int v;

    FUN_00471d60();
    FUN_0046d680(param_1, DAT_00668934);
    if ((flags & 2) != 0) {
        v = DAT_007fdec8 & 0xffff;
        saved_class = QueryClass;
        memcpy(&local_cursor, &QueryCursor, sizeof(struct Cursor));
        local_8[0] = v & 0xff;
        local_8[1] = v >> 8;
        cls = *(struct ObjClass **)((char *)DAT_007fdec4 + 0xc);
        QueryCursor.field_1408 = v >> 8;
        *((unsigned char *)&QueryObj + 1) = (unsigned char)(v >> 8);
        QueryClass = cls;
        QueryCursor.field_1404 = v & 0xff;
        *(unsigned char *)&QueryObj = (unsigned char)(v & 0xff);
        cls->method_94(cls->field_c4, local_8);
        BuildCursorPtr(&QueryCursor, 0, 0);
        if (FUN_0045f4b0(&QueryCursor) != 0) {
            FUN_0045d3d0(QueryClass, local_8);
            cls = (struct ObjClass *)QueryClass;
            RemObjFromMap(QueryClass, cls->field_c4, QueryObj, &QueryCursor);
        }
        memcpy(&QueryCursor, &local_cursor, sizeof(struct Cursor));
        QueryClass = saved_class;
        ResetInfoStruct();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00473310
unsigned char FUN_00473310(void *param1, unsigned char param2) {
    FUN_00471d60();
    FUN_0046d680(param1, DAT_00668940);

    if (param2 & 0x2) {
        DAT_007fdea8->flags |= 0x400;
        DAT_007fe000->flags |= 0x400;
        FUN_004714a0();
        DAT_007fdfa4 = 0;
    }

    return 1;
}

// FUNCTION: LEGOLAND 0x00473360
unsigned char FUN_00473360(void *arg1, unsigned char flags, unsigned int arg3, unsigned int arg4) {
    FUN_00471d60();
    if (arg1) {
        FUN_0046d680(arg1, DAT_00668924);
    }
    if (flags & 2) {
        if (DAT_007fdf78 < (int)(DAT_007fdf74 - 1)) {
            DAT_007fdf78++;
        } else {
            ResetInfoStruct();
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004733b0
unsigned char FUN_004733b0(void *arg0, unsigned char flags) {
    FUN_00471d60();
    FUN_0046d680(arg0, DAT_0066892c);

    if (flags & 0x2) {
        if (DAT_007fdf78 > 0) {
            DAT_007fdf78--;
        }
    }

    return 1;
}

// FUNCTION: LEGOLAND 0x004733f0
unsigned char FUN_004733f0(void *param_1, unsigned char param_2) {
    unsigned int local_8[2];

    FUN_00471610();
    if (DAT_0079a8bc < 0xf) {
        FUN_0046d680(param_1, DAT_00668944);
        if ((param_2 & 2) != 0) {
            local_8[0] = DAT_007fdf84[4];
            local_8[1] = DAT_007fdf84[5];
            if (FUN_0049a120() != 0) {
                GenerateGardener(local_8, 1);
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00473460
unsigned char FUN_00473460(void *param_1, unsigned char param_2) {
    unsigned int local_8[2];

    FUN_00471610();
    if (DAT_0079a8cc < 0xf) {
        FUN_0046d680(param_1, DAT_0066894c);
        if ((param_2 & 2) != 0) {
            local_8[0] = DAT_007fdf84[4];
            local_8[1] = DAT_007fdf84[5];
            if (FUN_0049a160() != 0) {
                GenerateMechanic(local_8, 1);
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004734d0
unsigned char FUN_004734d0(void *param_1, unsigned char param_2) {
    FUN_00471610();
    FUN_0046d680(param_1, DAT_00668914);
    if ((param_2 & 2) != 0) {
        if (DAT_007fdf9c == 0x10b) {
            SetObjRectFlags(DAT_007fdf80->field_4, &DAT_007fdf80->field_8, 0);
            RemoveGardenersWorkOrderAt(DAT_007fdf80->field_8, DAT_007fdf80->field_c);
            ResetInfoStruct();
            return 1;
        }
        SetObjRectFlags(DAT_007fdf80->field_4, &DAT_007fdf80->field_8, 0);
        RemoveMechanicsWorkOrderAt(DAT_007fdf80->field_8, DAT_007fdf80->field_c);
        ResetInfoStruct();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004735b0
void FUN_004735b0(void) {
    DAT_00668960 = 0;
}

// FUNCTION: LEGOLAND 0x004735e0
unsigned int FUN_004735e0(unsigned int param) {
    struct InfoTimedEntry *entry;
    unsigned long now;

    entry = &DAT_004ba8e0[param];
    if ((int)DAT_00668960 < (int)param) {
        now = GetTicks();
        if ((int)(entry->interval + entry->last_time) < (int)now) {
            if (FUN_0046d280(entry->sample) != 0) {
                DAT_00668960 = param;
                entry->last_time = now;
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00473640
unsigned int FUN_00473640(unsigned int param_1) {
    return FUN_004735e0(DAT_004ba9ac[param_1]);
}

// FUNCTION: LEGOLAND 0x00473660
void FUN_00473660(void) {
    if (DAT_00668960 == 0) {
        return;
    }
    if (FUN_00498cf0() != 0) {
        return;
    }
    FUN_004735b0();
}

// FUNCTION: LEGOLAND 0x00473680
void FUN_00473680(int param_1, int param_2, char *param_3, unsigned int param_4, unsigned int param_5) {
    strcpy(DAT_00668968, param_3);
    DAT_007fe010 = param_1;
    DAT_007fe014 = param_2;
    FUN_00470950(param_4, param_5);
    DAT_00668d68 = 1;
}

// FUNCTION: LEGOLAND 0x004736e0
void FUN_004736e0(void) {
    FUN_00470b00();
    DAT_00668d68 = 0;
}

// FUNCTION: LEGOLAND 0x004736f0
void FUN_004736f0(void) {
    int iVar1;
    int iVar2;
    int iVar3;
    struct PrintCtx ctx;

    iVar1 = DAT_007fe014;
    iVar3 = DAT_007fe010;
    if (DAT_00668d68 != 0) {
        ctx.node = 0;
        ctx.flags = 1;
        ctx.field_8 = 0;
        PrintSprite(DAT_00668904, iVar3, iVar1, 0, (int *)&ctx);
        iVar3 = iVar3 + 0x7a;
        iVar2 = 0;
        if (0 < DAT_00668964) {
            do {
                PrintSprite(DAT_00668908, iVar3, iVar1, 0, (int *)&ctx);
                iVar3 = iVar3 + 0x20;
                iVar2 = iVar2 + 1;
            } while (iVar2 < DAT_00668964);
        }
        PrintSprite(DAT_0066890c, iVar3, iVar1, 0, (int *)&ctx);
        DAT_007fdea8->flags = DAT_007fdea8->flags & 0xfffffbff;
        DAT_007fdea8->x = (short)iVar3 + 3;
        DAT_007fdea8->y = (short)DAT_007fe014 + 3;
        DAT_007fe000->flags = DAT_007fe000->flags & 0xfffffbff;
        DAT_007fe000->x = (short)iVar3 + 0x27;
        DAT_007fe000->y = (short)DAT_007fe014 + 3;
        FUN_00455e50(DAT_00668968, DAT_007fe010 + 0xc, DAT_007fe014 + 6,
                     (DAT_007fe010 + 0x86 + DAT_00668964 * 0x14) - (DAT_007fe010 + 0xc),
                     (DAT_007fe014 + 0x21) - (DAT_007fe014 + 6), 1, 5, 0xff0000, 0xffffff);
        if ((DAT_007fe000->x + 0x24 < (int)DAT_00813a44) || ((int)DAT_00813a44 < DAT_007fdea8->x)) {
            FUN_00471d60();
        }
        if ((iVar1 + 0x1b < (int)DAT_00813a48) || ((int)DAT_00813a48 < iVar1)) {
            FUN_00471d60();
        }
    }
}
