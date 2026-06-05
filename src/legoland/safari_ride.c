#include "legoland.h"

extern void *DAT_004cbf0c;
extern unsigned int DAT_004cbec4;
extern unsigned int DAT_004cbef8;
extern unsigned int DAT_004cbefc;
extern unsigned int DAT_004cbf00;
extern unsigned int DAT_004cbf08;
extern unsigned int DAT_004cbed0;
extern unsigned int DAT_004cbed4;
extern unsigned int DAT_004cbed8;
extern unsigned short DAT_004cbedc;
extern unsigned char SAFARI_SFX[8];
extern unsigned int EditMode;
extern void *DAT_008119b8;
extern unsigned int EditCursor;

extern void KillSprite(unsigned int sprite);
extern void FreeBinV(unsigned int binv);
extern void Kill_FXList(unsigned char *list, unsigned int count);
extern void DefaultCursor(unsigned int *cursor);
extern void SetEditCursorFootPrint(void *foot_print);
extern int SaveGameWrite(void *data, unsigned int size);
extern void *_malloc(unsigned int size);

struct SafariNode {
    unsigned short field_0;
    unsigned char pad_2[14];
    struct SafariNode *next;
    unsigned char pad_14[0x14];
};

struct SafariBlockData {
    unsigned char pad_0[0x10];
    unsigned int field_10;
};

struct SafariBlock {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[0x48];
    struct SafariBlockData *field_64;
};

struct SafariRoot {
    unsigned char pad_0[0xc];
    struct SafariBlock *field_c;
};

struct SafariObject {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

void FUN_00414b10(struct SafariNode *node);
void FUN_004150c0(struct SafariNode *node);

// FUNCTION: LEGOLAND 0x004149c0
void FUN_004149c0(struct SafariNode *param) {
    struct SafariNode *s = (struct SafariNode *)_malloc(sizeof(struct SafariNode));
    if (s) {
        memset(s, 0, sizeof(struct SafariNode));
        s->field_0 = param->field_0;
        s->next = (struct SafariNode *)DAT_004cbf0c;
        DAT_004cbf0c = s;
        FUN_00414b10(s);
    }
}

// FUNCTION: LEGOLAND 0x00414a00
void FUN_00414a00(struct SafariNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00414a60
void FUN_00414a60(void) {
    while (DAT_004cbf0c != NULL) {
        FUN_00414a00(DAT_004cbf0c);
    }
}

// FUNCTION: LEGOLAND 0x00414a80
void FUN_00414a80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414ab0
void FUN_00414ab0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414b10
void FUN_00414b10(struct SafariNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00414b80
void FUN_00414b80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414d90
void FUN_00414d90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414ea0
void FUN_00414ea0(struct SafariObject *a1) {
    DAT_004cbec4 = a1->field_c;
    FUN_00414a60();
    Kill_FXList(SAFARI_SFX, 1);
    FreeBinV(DAT_004cbef8);
    FreeBinV(DAT_004cbefc);
    FreeBinV(DAT_004cbf00);
    KillSprite(DAT_004cbf08);
}

// FUNCTION: LEGOLAND 0x00414f00
void FUN_00414f00(void) {
    EditMode = 1;
    DAT_008119b8 = (void *)DAT_004cbec4;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00414f40
void FUN_00414f40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414fc0
void FUN_00414fc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00414ff0
unsigned int *FUN_00414ff0(struct SafariRoot *p1, unsigned short arg2) {
    struct SafariBlock *pB = p1->field_c;
    DAT_004cbed0 = (unsigned int)pB->field_64;
    DAT_004cbed4 = pB->field_14;
    DAT_004cbed8 = pB->field_18;
    DAT_004cbedc = arg2;
    pB->field_64->field_10 |= 0x2000;
    return &DAT_004cbed0;
}

// FUNCTION: LEGOLAND 0x00415030
void FUN_00415030(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004150c0
void FUN_004150c0(struct SafariNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00415200
void FUN_00415200(void) {
    struct SafariNode *current = (struct SafariNode *)DAT_004cbf0c;
    if (current != NULL) {
        while (current != NULL) {
            FUN_004150c0(current);
            current = current->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x00415220
void FUN_00415220(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00415760
void FUN_00415760(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004157b0
int SaveSafariRide(void) {
    struct SafariNode *current = (struct SafariNode *)DAT_004cbf0c;
    unsigned int value1 = 1;
    unsigned int value0 = 0;

    while (current != NULL) {
        if (SaveGameWrite(&value1, 0x4) == 0) {
            return 0;
        }
        if (SaveGameWrite(current, 0x28) == 0) {
            return 0;
        }
        current = current->next;
    }

    if (SaveGameWrite(&value0, 0x4) == 0) {
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00415820
void LoadSafariRide(void) { STUB(); }
