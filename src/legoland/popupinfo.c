#include "legoland.h"

struct Sprite;

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

extern void KillSprite(struct Sprite *sprite);  /* 0x00497bd0 */
extern void FUN_00470b00(void);                 /* 0x00470b00 */

// FUNCTION: LEGOLAND 0x00470bb0
void InitPopUpInfo(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471170
void FUN_00471170(void) {
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
void UnLoad_PopUpInfo(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471470
void FUN_00471470(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004714a0
void FUN_004714a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004714e0
void FUN_004714e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471510
void ResetInfoStruct(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471550
void PopInfoSizeMayChange(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471570
void StopFollowingBloke(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471580
void DisableInfoPopUPIcons(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471610
void FUN_00471610(void) { STUB(); }

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
void FUN_00471bf0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471c10
void FUN_00471c10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471ca0
void FUN_00471ca0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471d40
void FUN_00471d40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471d60
void FUN_00471d60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471d90
void FUN_00471d90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00471f10
void FUN_00471f10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00472090
void FUN_00472090(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004720a0
void FUN_004720a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004723f0
void FUN_004723f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004724a0
void DrawPopUpInfo(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004730f0
void FUN_004730f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00473130
void FUN_00473130(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00473160
void FUN_00473160(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004731a0
void FUN_004731a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004731e0
void FUN_004731e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00473310
void FUN_00473310(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00473360
void FUN_00473360(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004733b0
void FUN_004733b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004733f0
void FUN_004733f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00473460
void FUN_00473460(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004734d0
void FUN_004734d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004735b0
void FUN_004735b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004735e0
void FUN_004735e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00473640
void FUN_00473640(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00473660
void FUN_00473660(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00473680
void FUN_00473680(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004736e0
void FUN_004736e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004736f0
void FUN_004736f0(void) { STUB(); }
