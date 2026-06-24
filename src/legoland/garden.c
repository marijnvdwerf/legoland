#include "legoland.h"

#include "gamemap.h"
#include "garden.h"
#include "globals.h"
#include "llidb.h"
#include "map_object.h"

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

// FUNCTION: LEGOLAND 0x00432480
void FUN_00432480(struct GardenLayer *arg0) {
    struct GardenInner *temp = arg0->field_c;
    DAT_0081cd08 = (unsigned int)temp;
    temp->field_1c |= 0x404;
    // STRING: LEGOLAND 0x004b7114
    if (LLIDB_FindElement("HEDGE IMAGES", &DAT_0061615c, 0) != 0) {
        return;
    }
    DAT_0061614c = (unsigned int)LLIDB_LoadData((void *)DAT_0061615c); /* TODO: fold — LLIDB_LoadData handle stored as uint global */
}

// FUNCTION: LEGOLAND 0x004324c0
void FUN_004324c0(void) {
    LLIDB_UnLoadData(DAT_0061615c);
}

// FUNCTION: LEGOLAND 0x004324d0
void FUN_004324d0(void) {
    void *var = (void *)DAT_0081cd08;
    EditMode.unk0 = 1;
    EditMode.unk8 = var;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&((struct EditTarget *)EditMode.unk8)->field_3c);
}

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
    DAT_00616158 = (unsigned int)LLIDB_LoadData((void *)DAT_00616150);
}

// FUNCTION: LEGOLAND 0x004328b0
void FUN_004328b0(void) {
    LLIDB_UnLoadData(DAT_00616150);
}

// FUNCTION: LEGOLAND 0x004328c0
void FUN_004328c0(void) {
    void *var = DAT_0081cd04;
    EditMode.unk0 = 1;
    EditMode.unk8 = var;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&((struct EditTarget *)EditMode.unk8)->field_3c);
}

// FUNCTION: LEGOLAND 0x00432900
void FUN_00432900(int param_1, int *param_2) {
    unsigned short packed;
    *((unsigned char *)&packed) = (unsigned char)param_2[0];
    *((unsigned char *)&packed + 1) = (unsigned char)param_2[1];
    AddObjectToMap(param_1, packed, 0);
    if (DAT_00616158 != 0) {
        int r = rand();
        Set_UserFlags(param_2[0] << 8, param_2[1] << 8, (unsigned short)(r % *(int *)(DAT_00616158 + 4)));
    }
}

// FUNCTION: LEGOLAND 0x00432960
void FUN_00432960(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004329c0
void FUN_004329c0(void) { STUB(); }
