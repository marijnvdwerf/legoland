#include "legoland.h"

#include "gamemap.h"
#include "money.h"

struct Building {
    unsigned char pad_0[0x1c];
    unsigned int flags;
};

struct MapObject {
    unsigned char pad_0[0xc];
    struct Building *building;
};

struct JailCell {
    struct JailCell *next;
    unsigned char pad_4[0x1c - 0x4];
};

extern unsigned int LoadSprite(const char *filename, unsigned int param_2);
extern void KillSprite(unsigned int sprite);
extern void SetEditCursorFootPrint(void *param_1);
extern unsigned int SaveGameWrite(const void *buffer, unsigned int size);
extern unsigned int SaveGameRead(void *buffer, unsigned int size);
extern void *_malloc(unsigned int size);
extern void FUN_0049e4d0(void *ptr);

extern struct Building *DAT_0081cb30;
extern unsigned int DAT_0081cb08;
extern unsigned int DAT_0081cb24;
extern struct Building *DAT_0081cb14;
extern unsigned int DAT_0081cb38;
extern struct Building *DAT_0081cb2c;
extern unsigned int DAT_0081cb34;
extern struct Building *DAT_0081cb1c;
extern unsigned int DAT_0081cb00;
extern unsigned int DAT_0081cb04;
extern unsigned int DAT_0081cb0c;
extern struct JailCell *DAT_0062fd3c;

extern unsigned int EditMode;
extern void *DAT_008119b8;
extern struct Cursor EditCursor;

// FUNCTION: LEGOLAND 0x004375d0
void FUN_004375d0(struct MapObject *obj) {
    struct Building *building = obj->building;
    DAT_0081cb30 = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b74a4
    DAT_0081cb08 = LoadSprite("G_Store Matte.LLS", 1);
    // STRING: LEGOLAND 0x004b7490
    DAT_0081cb24 = LoadSprite("G_Store Matte2.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x00437610
void FUN_00437610(void) {
    KillSprite(DAT_0081cb08);
    KillSprite(DAT_0081cb24);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00437630
void FUN_00437630(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb30;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00437670
void FUN_00437670(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004378e0
void FUN_004378e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00437ba0
void FUN_00437ba0(struct MapObject *obj) {
    struct Building *building = obj->building;
    DAT_0081cb14 = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b74b8
    DAT_0081cb38 = LoadSprite("Sherifshut Matte.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x00437bd0
void FUN_00437bd0(void) {
    KillSprite(DAT_0081cb38);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00437bf0
void FUN_00437bf0(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb14;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00437c30
void FUN_00437c30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00437c90
void FUN_00437c90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00437f10
void FUN_00437f10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00437f60
void FUN_00437f60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00437f90
void FUN_00437f90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00437fc0
void FUN_00437fc0(struct JailCell *cell) { STUB(); }

// FUNCTION: LEGOLAND 0x00438000
void FUN_00438000(void) {
    while (DAT_0062fd3c != NULL) {
        FUN_00437fc0(DAT_0062fd3c);
    }
}

// FUNCTION: LEGOLAND 0x00438020
void FUN_00438020(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00438070
void FUN_00438070(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004380f0
void FUN_004380f0(void) {
    KillSprite(DAT_0081cb0c);
    FUN_00438000();
}

// FUNCTION: LEGOLAND 0x00438110
void FUN_00438110(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00438150
void FUN_00438150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00438430
void FUN_00438430(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00438780
unsigned int SaveJailCells(void) {
    struct JailCell *cell = DAT_0062fd3c;
    unsigned int marker = 1;
    unsigned int terminator = 0;

    while (cell != NULL) {
        if (SaveGameWrite(&marker, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(cell, 0x1c) == 0) {
            return 0;
        }
        cell = cell->next;
    }
    return SaveGameWrite(&terminator, 4) != 0;
}

// FUNCTION: LEGOLAND 0x004387f0
unsigned int LoadJailCells(void) {
    unsigned int marker;
    struct JailCell *prev = NULL;

    if (SaveGameRead(&marker, 4) == 0) {
        return 0;
    }
    while (marker != 0) {
        struct JailCell *cell = _malloc(0x1c);
        if (SaveGameRead(cell, 0x1c) == 0) {
            return 0;
        }
        cell->next = NULL;
        if (prev != NULL) {
            prev->next = cell;
        } else {
            DAT_0062fd3c = cell;
        }
        prev = cell;
        if (SaveGameRead(&marker, 4) == 0) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00438870
void FUN_00438870(struct MapObject *obj) {
    struct Building *building = obj->building;
    DAT_0081cb2c = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b74e4
    DAT_0081cb34 = LoadSprite("Bank Matte.lls", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x004388a0
void FUN_004388a0(void) {
    KillSprite(DAT_0081cb34);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x004388c0
void FUN_004388c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00438900
void FUN_00438900(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00438960
void FUN_00438960(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00438c60
void FUN_00438c60(struct MapObject *obj) {
    struct Building *building = obj->building;
    DAT_0081cb1c = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b7508
    DAT_0081cb00 = LoadSprite("SaloonMatte1.LLS", 1);
    // STRING: LEGOLAND 0x004b74f4
    DAT_0081cb04 = LoadSprite("SaloonMatte2.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x00438ca0
void FUN_00438ca0(void) {
    KillSprite(DAT_0081cb00);
    KillSprite(DAT_0081cb04);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00438cc0
void FUN_00438cc0(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb1c;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00438d00
void FUN_00438d00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00438f10
void FUN_00438f10(void) { STUB(); }
