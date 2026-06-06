#include "legoland.h"

#include "binv.h"

struct JoustNode {
    unsigned char pad_0[4];
    struct JoustNode *next;
};

struct JoustObject {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

struct JoustBlockData {
    unsigned char pad_0[0x10];
    unsigned int field_10;
};

struct JoustBlock {
    unsigned char pad_0[0x14];
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[0x48];
    struct JoustBlockData *field_64;
};

struct JoustRoot {
    unsigned char pad_0[0xc];
    struct JoustBlock *field_c;
};

typedef void (*JoustMethod)(void);

struct JoustInterface {
    unsigned char pad_0[0x8c];
    JoustMethod field_8c;
    unsigned char pad_90[8];
    JoustMethod field_98;
    JoustMethod field_9c;
    JoustMethod field_a0;
    JoustMethod field_a4;
    JoustMethod field_a8;
    JoustMethod field_ac;
    JoustMethod field_b0;
    unsigned char pad_b4[4];
    JoustMethod field_b8;
    JoustMethod field_bc;
};

extern struct JoustNode *DAT_004c1250;
extern unsigned int DAT_004c121c;
extern void *DAT_004c1218;
extern unsigned int DAT_004c1240;
extern unsigned int DAT_004c1244;
extern unsigned int DAT_004c1248;
extern unsigned int DAT_004c124c;
extern unsigned int DAT_004c1228;
extern unsigned int DAT_004c122c;
extern unsigned int DAT_004c1230;
extern unsigned short DAT_004c1234;
extern unsigned char JOUST_SFX[12];
extern unsigned int EditMode;
extern void *DAT_008119b8;
extern unsigned int EditCursor;

extern void KillSprite(unsigned int sprite);
extern void Kill_FXList(unsigned char *list, unsigned int count);
extern void DefaultCursor(unsigned int *cursor);
extern void SetEditCursorFootPrint(void *foot_print);
extern int SaveGameWrite(void *data, unsigned int size);
extern int __strcmpi(const char *s1, const char *s2);

// FUNCTION: LEGOLAND 0x00407970
void FUN_00407970(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004079e0
void FUN_004079e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00407a20
void FUN_00407a20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00407a50
void FUN_00407a50(struct JoustNode *node) { STUB(); }

// FUNCTION: LEGOLAND 0x00407ab0
void FUN_00407ab0(void) {
    while (DAT_004c1250 != NULL) {
        FUN_00407a50(DAT_004c1250);
    }
}

// FUNCTION: LEGOLAND 0x00407ad0
void FUN_00407ad0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00407b50
void FUN_00407b50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00407c20
unsigned int FUN_00407c20(unsigned char param_1) {
    return (unsigned int)(param_1 != 0);
}

// FUNCTION: LEGOLAND 0x00407c30
void FUN_00407c30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00408580
void FUN_00408580(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00408bc0
void FUN_00408bc0(void) {
    EditMode = 1;
    DAT_008119b8 = (void *)DAT_004c121c;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00408c00
void FUN_00408c00(void) {
    Kill_FXList(JOUST_SFX, 1);
    KillSprite(DAT_004c1244);
    KillSprite(DAT_004c1248);
    KillSprite(DAT_004c124c);
    FreeBinV(DAT_004c1218);
    KillSprite(DAT_004c1240);
    FUN_00407ab0();
}

// FUNCTION: LEGOLAND 0x00408c50
unsigned int *FUN_00408c50(struct JoustRoot *param1, unsigned short param2) {
    struct JoustBlock *block = param1->field_c;

    DAT_004c1228 = (unsigned int)block->field_64;
    DAT_004c122c = block->field_14;
    DAT_004c1230 = block->field_18;
    DAT_004c1234 = param2;
    block->field_64->field_10 |= 0x2000;

    return &DAT_004c1228;
}

// FUNCTION: LEGOLAND 0x00408c90
int SaveJoust(void) {
    struct JoustNode *current = DAT_004c1250;
    unsigned int flag = 1;
    unsigned int terminator = 0;

    while (current != NULL) {
        if (!SaveGameWrite(&flag, 4)) {
            return 0;
        }
        if (!SaveGameWrite(current, 0x24)) {
            return 0;
        }
        current = current->next;
    }

    if (SaveGameWrite(&terminator, 4)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00408d00
void LoadJoust(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00408db0
void Joust_GetInterfaces(const char **interface_name, struct JoustInterface *pInterface) {
    // STRING: LEGOLAND 0x004b4718
    if (__strcmpi("JOUST", *interface_name) == 0) {
        pInterface->field_a4 = (JoustMethod)FUN_00407b50;
        pInterface->field_ac = (JoustMethod)FUN_00408c00;
        pInterface->field_8c = (JoustMethod)FUN_00408bc0;
        pInterface->field_a8 = (JoustMethod)FUN_00407c30;
        pInterface->field_b0 = (JoustMethod)FUN_00408580;
        pInterface->field_9c = (JoustMethod)FUN_00407ad0;
        pInterface->field_98 = (JoustMethod)FUN_004079e0;
        pInterface->field_a0 = (JoustMethod)FUN_00408c50;
        pInterface->field_bc = (JoustMethod)SaveJoust;
        pInterface->field_b8 = (JoustMethod)LoadJoust;
    }
}
