#include "legoland.h"

#include "gamemap.h"
#include "binv.h"
#include "llidb.h"

struct BalloonC {
    unsigned char pad_0[0x10];
    unsigned int var_10;
};

struct BalloonB {
    unsigned char pad_0[0x14];
    unsigned int var_14;
    unsigned int var_18;
    unsigned char pad_1c[0x48];
    struct BalloonC *var_64;
};

struct BalloonA {
    unsigned char pad_0[0xc];
    struct BalloonB *structB_member;
};

struct BalloonNode {
    struct BalloonNode *next;
    unsigned char pad_4[0x1c];
};

struct CursorState {
    unsigned char pad_0[0x3c];
    unsigned int var_3c;
};

extern void KillSprite(unsigned int sprite);
extern void SetEditCursorFootPrint(unsigned int *foot_print);

extern void *DAT_00616060;
extern void *DAT_00616018;
extern unsigned int DAT_00616028;
extern unsigned int DAT_0061602c;
extern unsigned int DAT_00616030;
extern unsigned short DAT_00616034;
extern unsigned int DAT_00616048;
extern unsigned int DAT_0061604c;
extern unsigned int DAT_00616050;
extern unsigned int DAT_00616054;
extern unsigned int DAT_00616058;
extern unsigned int DAT_0061605c;
extern unsigned int DAT_0081cde8;
extern struct CursorState *DAT_0081cde4;
extern struct CursorState *DAT_008119b8;
extern struct Cursor EditCursor;
extern unsigned int EditMode;

// FUNCTION: LEGOLAND 0x0042a7b0
void FUN_0042a7b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a8f0
void FUN_0042a8f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a950
void FUN_0042a950(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a980
void FUN_0042a980(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a9b0
void FUN_0042a9b0(void *param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x0042a9f0
void FUN_0042a9f0(void) {
    void *current_handle;

    current_handle = DAT_00616060;
    if (current_handle == NULL) {
        return;
    }
    do {
        FUN_0042a9b0(current_handle);
        current_handle = DAT_00616060;
    } while (current_handle != NULL);
}

// FUNCTION: LEGOLAND 0x0042aa10
void FUN_0042aa10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042aa60
void FUN_0042aa60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042aa90
void FUN_0042aa90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042b2a0
unsigned int *FUN_0042b2a0(struct BalloonA *arg1, unsigned short arg2) {
    struct BalloonB *p;
    struct BalloonC *p2;

    p = arg1->structB_member;
    DAT_00616028 = (unsigned int)p->var_64;
    DAT_0061602c = p->var_14;
    DAT_00616030 = p->var_18;
    DAT_00616034 = arg2;
    p2 = p->var_64;
    p2->var_10 |= 0x2000;
    return &DAT_00616028;
}

// FUNCTION: LEGOLAND 0x0042b2e0
void FUN_0042b2e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042b9d0
void FUN_0042b9d0(void) {
    KillSprite(DAT_00616048);
    KillSprite(DAT_0061604c);
    KillSprite(DAT_00616050);
    KillSprite(DAT_00616054);
    KillSprite(DAT_00616058);
    KillSprite(DAT_0061605c);
    KillSprite(DAT_0081cde8);
    FreeBinV(DAT_00616018);
    FUN_0042a9f0();
}

// FUNCTION: LEGOLAND 0x0042ba40
void FUN_0042ba40(void) {
    struct CursorState *temp;

    temp = DAT_0081cde4;
    EditMode = 1;
    DAT_008119b8 = temp;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&DAT_008119b8->var_3c);
}

// FUNCTION: LEGOLAND 0x0042ba80
unsigned int FUN_0042ba80(void) {
    unsigned int marker;
    unsigned int terminator;
    struct BalloonNode *current;

    marker = 1;
    terminator = 0;
    current = (struct BalloonNode *)DAT_00616060;
    while (current != NULL) {
        if (SaveGameWrite(&marker, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(current, 0x20) == 0) {
            return 0;
        }
        current = current->next;
    }
    if (SaveGameWrite(&terminator, 4) != 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0042baf0
void FUN_0042baf0(void) { STUB(); }
