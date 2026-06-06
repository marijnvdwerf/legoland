#include "legoland.h"

#include "gamemap.h"

struct NameNode {
    unsigned short value;
    unsigned char pad_2[0x3ee];
    struct NameNode *next;
};

extern struct NameNode *DAT_004cc03c;

extern unsigned int BasicObjectDCalcCursor(unsigned int param_1, unsigned int param_2);
extern void SetEditCursorFootPrint(unsigned int *footprint);

extern unsigned int EditMode;
extern struct Cursor EditCursor;
extern void *DAT_008119b8;
extern unsigned int DAT_0082c658;
extern unsigned int DAT_0082adf8;
extern void *DAT_0082adf0;
extern struct KeyNode *DAT_004d823c;
extern unsigned int *DAT_004cc088;
extern unsigned int *DAT_004cc070;
extern unsigned int DAT_004cc078;
extern unsigned int DAT_004cc060;
extern unsigned int DAT_004cc048;
extern unsigned int DAT_004b53c0[5];

struct RideNode {
    unsigned short id;
    unsigned char pad_2[6];
    unsigned int field_8;
    unsigned char pad_c[0x2c - 0xc];
    struct RideNode *next;
    unsigned int value;
};

extern struct RideNode *DAT_004cc074;

struct RideBuilding {
    unsigned char pad_0[0x3c];
    unsigned int footprint[5];
};

struct KeyNode {
    unsigned short key;
    unsigned char pad_2[0xe];
    struct KeyNode *next;
};

struct RideObject {
    unsigned char pad_0[0xc];
    struct RideBuilding *building;
};

// FUNCTION: LEGOLAND 0x00418e60
void FUN_00418e60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00418f90
void FUN_00418f90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00418fe0
void FUN_00418fe0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004192d0
unsigned int FUN_004192d0(unsigned short *param_1) {
    struct NameNode *node = DAT_004cc03c;
    unsigned int count = 0;

    if (node != NULL) {
        while (node != NULL) {
            unsigned short value = *param_1;
            node->value = value;
            if (value != 0) {
                count++;
            }
            node = node->next;
        }
    }

    return count;
}

// FUNCTION: LEGOLAND 0x004193c0
void FUN_004193c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00419420
void FUN_00419420(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00419520
void FUN_00419520(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004198a0
void FUN_004198a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00419d10
void FUN_00419d10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00419ef0
void FUN_00419ef0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041a000
void FUN_0041a000(void) {
    EditMode = 1;
    DAT_008119b8 = (void *)DAT_0082c658;
    DefaultCursor(&EditCursor);
    DAT_004cc088 = &DAT_004cc060;
    DAT_004cc070 = &DAT_004cc048;
    SetEditCursorFootPrint(&DAT_004cc078);
}

// FUNCTION: LEGOLAND 0x0041a040
void FUN_0041a040(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041a2f0
void FUN_0041a2f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041a3d0
void FUN_0041a3d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041a530
void FUN_0041a530(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041a720
void FUN_0041a720(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041abd0
void FUN_0041abd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041acf0
void FUN_0041acf0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041aee0
void FUN_0041aee0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041b0d0
void FUN_0041b0d0(unsigned short id, unsigned int value) {
    struct RideNode *node = DAT_004cc074;
    if (node == NULL) {
        return;
    }

    while (node != NULL && node->id != id) {
        node = node->next;
    }

    if (node != NULL) {
        node->value += value;
    }
}

// FUNCTION: LEGOLAND 0x0041b100
int FUN_0041b100(int dummy, int arg) {
    struct RideNode *node = DAT_004cc074;
    int result = 0;

    if (node != NULL) {
        do {
            int val = (int)node->value;
            if (val > result) {
                if (arg != 0) {
                    if (node->field_8 != 0) {
                        result = val;
                    }
                } else {
                    result = val;
                }
            }
            node = node->next;
        } while (node != NULL);
    }

    return result;
}

// FUNCTION: LEGOLAND 0x0041b150
void GetInterface(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041b250
void FUN_0041b250(struct RideObject *param_1) {
    DAT_0082adf8 = (unsigned int)param_1->building;
}

// FUNCTION: LEGOLAND 0x0041b260
void FUN_0041b260(void) {
    EditMode = 1;
    DAT_008119b8 = (void *)DAT_0082adf8;
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint((unsigned int *)(DAT_0082adf8 + 0x3c));
}

// FUNCTION: LEGOLAND 0x0041b2a0
void FUN_0041b2a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041b4c0
void FUN_0041b4c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041b6d0
unsigned int FUN_0041b6d0(unsigned int param_1, unsigned int param_2) {
    return BasicObjectDCalcCursor(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x0041b6f0
void FUN_0041b6f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041b830
void FUN_0041b830(struct RideObject *arg) {
    struct RideBuilding *building = arg->building;
    DAT_0082adf0 = building;
    DAT_004b53c0[1] += building->footprint[1];
    DAT_004b53c0[0] += building->footprint[0];
    DAT_004b53c0[2] += building->footprint[0];
    DAT_004b53c0[3] += building->footprint[1];
}

// FUNCTION: LEGOLAND 0x0041b880
void FUN_0041b880(void) {
    struct RideBuilding *state = (struct RideBuilding *)DAT_0082adf0;
    EditMode = 1;
    DAT_008119b8 = state;
    memcpy(state->footprint, DAT_004b53c0, 20);
    DefaultCursor(&EditCursor);
    EditCursor.field_1828 |= 0x8;
    SetEditCursorFootPrint((unsigned int *)((char *)DAT_008119b8 + 0x3c));
}

// FUNCTION: LEGOLAND 0x0041b8e0
void FUN_0041b8e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041bab0
void FUN_0041bab0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041bd40
void FUN_0041bd40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041bfb0
void FUN_0041bfb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041c130
void FUN_0041c130(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041c4c0
void FUN_0041c4c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041c620
void FUN_0041c620(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041c690
void FUN_0041c690(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041c890
struct KeyNode *FUN_0041c890(unsigned char a, unsigned char b) {
    struct KeyNode *current;
    unsigned short key;
    unsigned char stack_key[2];

    stack_key[0] = a;
    stack_key[1] = b;
    key = *(unsigned short *)stack_key;

    current = DAT_004d823c;
    while (current != NULL && current->key != key) {
        current = current->next;
    }

    return current;
}

// FUNCTION: LEGOLAND 0x0041c8c0
void FUN_0041c8c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041c940
void FUN_0041c940(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041caa0
void FUN_0041caa0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0041cb20
void FUN_0041cb20(void) { STUB(); }
