#include "legoland.h"
#include "crt.h"
#include <windows.h>
#include <ddraw.h>
#include "globals.h"
#include "worker_mouse.h"
#include "popupinfo.h"
#include "icon.h"
#include "text.h"

#pragma intrinsic(strlen, strcpy)

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

extern int FUN_0049a120(void);
extern int FUN_0049a160(void);
extern void SetObjRectFlags();
extern void FUN_00470950();
extern void FUN_00455e50(char *text, int x, int y, int w, int h, int a6, int a7, unsigned int color1, unsigned int color2);

// FUNCTION: LEGOLAND 0x00470bb0
LEGO_EXPORT void InitPopUpInfo(void) { STUB(); }

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
LEGO_EXPORT void InfoPrintCent(void) { STUB(); }

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
LEGO_EXPORT void PopUpInfoSetUp(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471bf0
void FUN_00471bf0(void) {
    if (DAT_007fdfa0 != 2) {
        DAT_007fdf78 = 0;
        DAT_007fdf74 = 0;
    }
}

// FUNCTION: LEGOLAND 0x00471c10
void FUN_00471c10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471ca0
void FUN_00471ca0(unsigned int param) { STUB(); }

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
void FUN_00471d90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471f10
void FUN_00471f10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00472090
void FUN_00472090(void) {
}

// FUNCTION: LEGOLAND 0x004720a0
void FUN_004720a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004723f0
void FUN_004723f0(void) { STUB(); }

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
void FUN_004731e0(void) { STUB(); }

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
