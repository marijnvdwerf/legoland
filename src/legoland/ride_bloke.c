#include "legoland.h"

#include "print_sprite.h"
#include "ride_queue.h"

extern void *DAT_004c10d4;
extern int DAT_004cbeb0;
extern int DAT_004cbeb4;
extern unsigned int DAT_0082c690;
extern unsigned int DAT_0082c6b8;
extern unsigned int DAT_0082c6bc;
extern unsigned int DAT_00830f94;

extern unsigned char Get_RFFlags(int x, int y);
extern void IP_RenderBlokeIn3DNow(unsigned int fn);
extern void SetOverridePalette(unsigned int param_1);
extern void SetOverrideFrame(unsigned int param_1);
extern void ClearOverrideFrame(void);
extern void ClearOverridePalette(void);

struct Sprite {
    unsigned char pad_0[0x14];
    unsigned short width;
    unsigned short height;
};

struct CountNode {
    struct CountNode *next;
    unsigned short var_4;
};

struct TimerStruct {
    unsigned char pad_0[0x30];
    unsigned int var_30[34];
    unsigned char pad_b8[3];
    unsigned char var_bb;
};

struct PairArg {
    unsigned int var_0;
    unsigned int var_4;
};

struct TileInfo {
    unsigned char pad_0[0x14];
    unsigned char var_14;
    unsigned char pad_15[0x1c - 0x15];
    unsigned char var_1c;
    unsigned char var_1d;
};

struct BlokeSprite {
    unsigned char pad_0[8];
    unsigned int var_8;
    unsigned int var_c;
    unsigned char pad_10[0xa9];
    unsigned char var_b9;
    unsigned char pad_ba[9];
    unsigned char var_c3;
    unsigned char pad_c4[8];
    unsigned int var_cc;
};

// FUNCTION: LEGOLAND 0x00401000
void FUN_00401000(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00401080
void FUN_00401080(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00401320
void FUN_00401320(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004015c0
void GetSpriteSize(struct Sprite *sprite, unsigned short *pWidth, unsigned short *pHeight) { *pWidth = sprite->width; *pHeight = sprite->height; }

// FUNCTION: LEGOLAND 0x004015e0
void FUN_004015e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00401660
void FUN_00401660(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004017c0
void MapToPlayfield(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00401800
unsigned int IsSemiPermiable(int param_1, int param_2) { unsigned char rf_flags = Get_RFFlags(param_1, param_2); return (rf_flags & 0x3) == 0x3; }

// FUNCTION: LEGOLAND 0x00401820
void SPGetRFFlags(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00401890
void SPEnter(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00401900
void SPLeave(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00401970
void FUN_00401970(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004019c0
void FUN_004019c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00401ae0
void FUN_00401ae0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00401c40
unsigned int FUN_00401c40(unsigned short arg0) {
    struct CountNode *current = (struct CountNode *)DAT_004c10d4;
    unsigned int count = 0;

    while (current != NULL) {
        if (current->var_4 == arg0) {
            count++;
        }
        current = current->next;
    }

    return count;
}

// FUNCTION: LEGOLAND 0x00401c60
void FUN_00401c60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00401cd0
void FUN_00401cd0(struct TimerStruct *p) {
    unsigned char temp_bb = p->var_bb;
    if (temp_bb == 0) {
        return;
    }
    temp_bb--;
    p->var_30[0] = p->var_30[2];
    p->var_30[1] = p->var_30[3];
    p->var_30[2] = p->var_30[4];
    p->var_30[3] = p->var_30[5];
    p->var_30[4] = p->var_30[6];
    p->var_30[5] = p->var_30[7];
    p->var_30[6] = p->var_30[8];
    p->var_30[7] = p->var_30[9];
    p->var_30[8] = p->var_30[10];
    p->var_30[9] = p->var_30[11];
    p->var_30[10] = p->var_30[12];
    p->var_30[11] = p->var_30[13];
    p->var_30[12] = p->var_30[14];
    p->var_30[13] = p->var_30[15];
    p->var_30[14] = p->var_30[16];
    p->var_30[15] = p->var_30[17];
    p->var_30[16] = p->var_30[18];
    p->var_30[17] = p->var_30[19];
    p->var_30[18] = p->var_30[20];
    p->var_30[19] = p->var_30[21];
    p->var_30[20] = p->var_30[22];
    p->var_30[21] = p->var_30[23];
    p->var_30[22] = p->var_30[24];
    p->var_30[23] = p->var_30[25];
    p->var_30[24] = p->var_30[26];
    p->var_30[25] = p->var_30[27];
    p->var_30[26] = p->var_30[28];
    p->var_30[27] = p->var_30[29];
    p->var_30[28] = p->var_30[30];
    p->var_30[29] = p->var_30[31];
    p->var_30[30] = p->var_30[32];
    p->var_30[31] = p->var_30[33];
    p->var_bb = temp_bb;
}

// FUNCTION: LEGOLAND 0x00401e00
void FUN_00401e00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00401f30
void FUN_00401f30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00402150
void FUN_00402150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00402340
int FUN_00402340(int a1) {
    if (DAT_004cbeb0 != 0) {
        if (a1 == 7 || a1 == 3)
            return 0;
    }
    if (DAT_004cbeb4 != 0) {
        if (a1 == 1 || a1 == 5)
            return 0;
    }
    if (DAT_004cbeb0 == 0 && DAT_004cbeb4 == 0)
        return 0;
    return 1;
}

// FUNCTION: LEGOLAND 0x00402390
void FUN_00402390(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00402430
int FUN_00402430(struct PairArg *a, struct PairArg *b) {
    struct TileInfo *p1 = FUN_004125f0(a->var_0, a->var_4);
    struct TileInfo *p2 = FUN_004125f0(b->var_0, b->var_4);

    if (p2 != p1) {
        if (p1 != 0) {
            if ((p1->var_14 & 0x10) && p1->var_1c == 0 && p1->var_1d != 0) {
                return 0;
            }

            if (p2 != 0 && p2->var_1c != 0) {
                p2->var_1c--;
            }

            p1->var_1c++;
        }
    }

    return 1;
}

// FUNCTION: LEGOLAND 0x00402490
void FUN_00402490(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00402550
void FUN_00402550(struct BlokeSprite *arg) {
    IP_RenderBlokeIn3DNow(arg->var_cc);

    switch (arg->var_c3) {
        case 1:
            SetOverridePalette(DAT_0082c6bc);
            break;
        case 2:
            SetOverridePalette(DAT_0082c6b8);
            break;
        case 3:
            SetOverridePalette(DAT_0082c690);
            break;
    }

    SetOverrideFrame(arg->var_b9 + 16);
    PrintSprite(DAT_00830f94, arg->var_8, arg->var_c, 0, 0);
    ClearOverrideFrame();
    ClearOverridePalette();
}

// FUNCTION: LEGOLAND 0x004025d0
void FUN_004025d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00402780
void FUN_00402780(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00402c10
void FUN_00402c10(void) { STUB(); }
