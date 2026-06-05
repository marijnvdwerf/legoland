#include "legoland.h"

struct PottingLayer {
    unsigned char pad_0[12];
    unsigned int field_c;
    unsigned char pad_10[12];
    unsigned int field_1c;
    unsigned char pad_20[68];
    unsigned int field_64;
};

struct EditTarget {
    unsigned char pad_0[0x3c];
    unsigned char field_3c;
};

struct PSCarLayer {
    unsigned char pad_0[0xc];
    struct PSCarSub *field_c;
};

struct PSCarSub {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[0x48];
    struct PSCarInner *field_64;
};

struct PSCarInner {
    unsigned char pad_0[0x10];
    unsigned int field_10;
};

struct Cursor;
struct EditFootprint;

extern void *DAT_0081caf0;
extern unsigned int DAT_0062fe48;
extern unsigned int DAT_0062fe4c;
extern unsigned int DAT_0062fe10;
extern unsigned int DAT_0062fe14;
extern unsigned int DAT_0062fe18;
extern unsigned short DAT_0062fe1c;
extern unsigned int EditMode;
extern unsigned int EditCursor;
extern void *DAT_008119b8;

extern unsigned int LoadSprite(const char *lpFileName, unsigned int flags);
extern unsigned int AddBasicObject(unsigned int param_1, unsigned int param_2);
extern void KillSprite(unsigned int sprite);
extern void DefaultCursor(struct Cursor *cursor);
extern void SetEditCursorFootPrint(struct EditFootprint *src);

// FUNCTION: LEGOLAND 0x0043ce60
int FUN_0043ce60(int a1) {
    struct PottingLayer *p;

    DAT_0081caf0 = *(void **)((unsigned char *)a1 + 12);
    p = (struct PottingLayer *)DAT_0081caf0;
    p->field_1c |= 0x420;
    p = (struct PottingLayer *)DAT_0081caf0;
    DAT_0062fe48 = p->field_64;
    p->field_1c |= 0x2000;
    // STRING: LEGOLAND 0x004b7994
    DAT_0062fe4c = LoadSprite("gshedmatte.lls", 1);
    return (int)DAT_0062fe4c;
}

// FUNCTION: LEGOLAND 0x0043ceb0
unsigned int FUN_0043ceb0(unsigned int param_1, unsigned int param_2) {
    return AddBasicObject(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x0043ced0
void FUN_0043ced0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043cf00
void FUN_0043cf00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043d0b0
void FUN_0043d0b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0043d1c0
void FUN_0043d1c0(void) {
    KillSprite(DAT_0062fe4c);
}

// FUNCTION: LEGOLAND 0x0043d1d0
void FUN_0043d1d0(void) {
    void *p = DAT_0081caf0;
    EditMode = 1;
    DAT_008119b8 = p;
    DefaultCursor((struct Cursor *)&EditCursor);
    SetEditCursorFootPrint((struct EditFootprint *)&((struct EditTarget *)DAT_008119b8)->field_3c);
}

// FUNCTION: LEGOLAND 0x0043d210
unsigned int *FUN_0043d210(struct PSCarLayer *param1, unsigned short param2) {
    struct PSCarSub *s2 = param1->field_c;
    struct PSCarInner *s3;

    DAT_0062fe10 = (unsigned int)s2->field_64;
    DAT_0062fe14 = s2->field_14;
    DAT_0062fe18 = s2->field_18;
    DAT_0062fe1c = param2;

    s3 = s2->field_64;
    s3->field_10 |= 0x2000;

    return &DAT_0062fe10;
}
