#include "legoland.h"

struct GardenLayer {
    unsigned char pad_0[0xc];
    struct GardenInner *field_c;
};

struct GardenInner {
    unsigned char pad_0[0x1c];
    unsigned int field_1c;
};

struct EditTarget {
    unsigned char pad_0[0x3c];
    unsigned int field_3c;
};

struct Cursor;
struct EditFootprint;

extern unsigned int DAT_0061615c;
extern unsigned int DAT_0081cd08;
extern unsigned int DAT_0061614c;
extern unsigned int DAT_00616150;
extern unsigned int DAT_00616158;
extern void *DAT_0081cd04;
extern unsigned int EditMode;
extern unsigned int EditCursor;
extern void *DAT_008119b8;

extern int LLIDB_FindElement(const char *name, unsigned int *out, int param_3);
extern unsigned int LLIDB_LoadData(unsigned int id);
extern void LLIDB_UnLoadData(unsigned int id);
extern void DefaultCursor(struct Cursor *cursor);
extern void SetEditCursorFootPrint(struct EditFootprint *src);

// FUNCTION: LEGOLAND 0x00432480
void FUN_00432480(struct GardenLayer *arg0) {
    struct GardenInner *temp = arg0->field_c;
    DAT_0081cd08 = (unsigned int)temp;
    temp->field_1c |= 0x404;
    // STRING: LEGOLAND 0x004b7114
    if (LLIDB_FindElement("HEDGE IMAGES", &DAT_0061615c, 0) != 0) {
        return;
    }
    DAT_0061614c = LLIDB_LoadData(DAT_0061615c);
}

// FUNCTION: LEGOLAND 0x004324c0
void FUN_004324c0(void) {
    LLIDB_UnLoadData(DAT_0061615c);
}

// FUNCTION: LEGOLAND 0x004324d0
void FUN_004324d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00432510
void FUN_00432510(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004325e0
void FUN_004325e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00432700
void FUN_00432700(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00432810
void FUN_00432810(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00432870
void FUN_00432870(struct GardenLayer *param) {
    struct GardenInner *temp = param->field_c;
    DAT_0081cd04 = temp;
    temp->field_1c |= 0x404;
    // STRING: LEGOLAND 0x004b7124
    if (LLIDB_FindElement("FLOWERS 1", &DAT_00616150, 0) != 0) {
        return;
    }
    DAT_00616158 = LLIDB_LoadData(DAT_00616150);
}

// FUNCTION: LEGOLAND 0x004328b0
void FUN_004328b0(void) {
    LLIDB_UnLoadData(DAT_00616150);
}

// FUNCTION: LEGOLAND 0x004328c0
void FUN_004328c0(void) {
    void *var = DAT_0081cd04;
    EditMode = 1;
    DAT_008119b8 = var;
    DefaultCursor((struct Cursor *)&EditCursor);
    SetEditCursorFootPrint((struct EditFootprint *)&((struct EditTarget *)DAT_008119b8)->field_3c);
}

// FUNCTION: LEGOLAND 0x00432900
void FUN_00432900(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00432960
void FUN_00432960(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004329c0
void FUN_004329c0(void) { STUB(); }
