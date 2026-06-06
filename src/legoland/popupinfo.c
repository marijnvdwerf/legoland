#include "legoland.h"
#include "worker_mouse.h"

struct Sprite;

struct InfoIcon {
    unsigned char pad_0[0x2c];
    void (*handler)(void);
    unsigned char pad_30[0x4];
    unsigned int flags;
};

extern struct Sprite *DAT_00668958;
extern struct Sprite *DAT_006688e0;
extern struct Sprite *DAT_006688e4;
extern struct Sprite *DAT_006688e8;
extern struct Sprite *DAT_006688f0;
extern struct Sprite *DAT_006688ec;
extern struct Sprite *DAT_006688f4;
extern struct Sprite *DAT_006688f8;
extern struct Sprite *DAT_006688fc;
extern struct Sprite *DAT_00668900;
extern struct Sprite *DAT_007fe004;
extern struct Sprite *DAT_007fdeb0;
extern struct Sprite *DAT_00668914;
extern struct Sprite *DAT_00668918;
extern struct Sprite *DAT_0066891c;
extern struct Sprite *DAT_00668920;
extern struct Sprite *DAT_00668928;
extern struct Sprite *DAT_00668924;
extern struct Sprite *DAT_00668930;
extern struct Sprite *DAT_0066892c;
extern struct Sprite *DAT_00668938;
extern struct Sprite *DAT_0066893c;
extern struct Sprite *DAT_00668940;
extern struct Sprite *DAT_00668944;
extern struct Sprite *DAT_00668948;
extern struct Sprite *DAT_0066894c;
extern struct Sprite *DAT_00668950;
extern struct Sprite *DAT_007fdfc8;
extern struct Sprite *DAT_007fdfe4;
extern struct Sprite *DAT_007fe018;
extern struct Sprite *DAT_007fdfd0;
extern struct Sprite *DAT_007fe008;
extern struct Sprite *DAT_007fdeac;

extern struct InfoIcon *DAT_007fdfdc;
extern struct InfoIcon *DAT_007fdfe0;
extern struct InfoIcon *DAT_007fdea4;
extern struct InfoIcon *DAT_007fdfcc;
extern struct InfoIcon *DAT_007fdfc0;
extern struct InfoIcon *DAT_007fdfd8;
extern struct InfoIcon *DAT_007fdea8;
extern struct InfoIcon *DAT_007fe000;
extern struct InfoIcon *DAT_007fdfc4;
extern struct InfoIcon *DAT_007fdfe8;

extern unsigned int DAT_007fdf74;
extern int DAT_007fdf78;
extern unsigned int DAT_007fded4;
extern unsigned int DAT_007fdf7c;
extern unsigned int DAT_007fdf8c;
extern unsigned int DAT_007fdf84;
extern unsigned int DAT_007fdf9c;
extern unsigned int DAT_007fdfa0;
extern unsigned int DAT_007fdfa4;
extern unsigned int DAT_007fdfa8;
extern unsigned int DAT_007fdf98;
extern unsigned char DAT_007fdfac;

extern unsigned int DAT_00668960;
extern unsigned int DAT_00668d68;
extern unsigned int DAT_004ba9ac[];

extern void KillSprite(struct Sprite *sprite);
extern int RemoveIconGroup(unsigned int group);
extern void FUN_0046d680(void *a, struct Sprite *b);
extern unsigned int FUN_00498cf0(void);

int FUN_00471170(void);
void DisableInfoPopUPIcons(void);
void FUN_004714a0(void);
void ResetInfoStruct(void);
void FUN_00471610(void);
void FUN_004714e0(void);
void FUN_00471d60(void);
unsigned char FUN_004730f0(void *param1, unsigned char param2, unsigned int param3, unsigned int param4);
void FUN_004735b0(void);
unsigned char FUN_00473360(void *arg1, unsigned char flags, unsigned int arg3, unsigned int arg4);
void FUN_00471ca0(unsigned int param);
unsigned int FUN_004735e0(unsigned int param);
void FUN_004731a0(void);
void FUN_004733f0(void);
void FUN_00473460(void);
void FUN_004734d0(void);

// FUNCTION: LEGOLAND 0x00470bb0
void InitPopUpInfo(void) { STUB(); }

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
int UnLoad_PopUpInfo(void) {
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
void ResetInfoStruct(void) {
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
void PopInfoSizeMayChange(void) {
    if (DAT_007fdfa0 != 0) {
        DAT_007fdfa8 = 1;
    }
}

// FUNCTION: LEGOLAND 0x00471570
void StopFollowingBloke(void) {
    if (DAT_007fdf98 == 0) {
        return;
    }
    ResetInfoStruct();
}

// FUNCTION: LEGOLAND 0x00471580
void DisableInfoPopUPIcons(void) {
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
    FUN_0046d680(DAT_007fdfdc, DAT_00668918);
    FUN_0046d680(DAT_007fdfe8, DAT_00668930);
    FUN_0046d680(DAT_007fdfc4, DAT_00668928);
    FUN_0046d680(DAT_007fdfc0, DAT_00668920);
    FUN_0046d680(DAT_007fdfe0, DAT_00668948);
    FUN_0046d680(DAT_007fdea4, DAT_00668950);
    FUN_0046d680(DAT_007fdfcc, DAT_00668918);
}

// FUNCTION: LEGOLAND 0x004716a0
void InfoPrintCent(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004717a0
void FUN_004717a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471840
void FUN_00471840(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004718c0
void FUN_004718c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471950
void PopUpInfoSetUp(void) { STUB(); }

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
    FUN_0046d680(DAT_007fdea8, DAT_00668938);
    FUN_0046d680(DAT_007fe000, DAT_0066893c);
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
void DrawPopUpInfo(void) { STUB(); }

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
void FUN_004731a0(void) { STUB(); }

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
void FUN_004733f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00473460
void FUN_00473460(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004734d0
void FUN_004734d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004735b0
void FUN_004735b0(void) {
    DAT_00668960 = 0;
}

// FUNCTION: LEGOLAND 0x004735e0
unsigned int FUN_004735e0(unsigned int param) { STUB(); }

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
void FUN_00473680(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004736e0
void FUN_004736e0(void) {
    FUN_00470b00();
    DAT_00668d68 = 0;
}

// FUNCTION: LEGOLAND 0x004736f0
void FUN_004736f0(void) { STUB(); }
