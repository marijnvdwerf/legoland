#include "legoland.h"

#include "ride_queue.h"
#include "ride_bloke.h"

struct DrivingSchoolNode {
    struct DrivingSchoolNode *next;
    unsigned char pad_4[4];
    unsigned short field_8;
    unsigned char pad_a[14];
    unsigned int field_18;
};

struct RetStruct {
    unsigned char pad_0[4];
    unsigned int field_4;
    unsigned char pad_8[13];
    unsigned char field_15;
};

struct Node0 {
    void *next;
};

struct Node8 {
    unsigned char pad_0[8];
    void *next;
};

struct DSCarLayer {
    unsigned char pad_0[12];
    struct DSCarSub *field_c;
};

struct DSCarSub {
    unsigned char pad_0[20];
    unsigned int field_14;
    unsigned int field_18;
    unsigned char pad_1c[72];
    struct DSCarInner *field_64;
};

struct DSCarInner {
    unsigned char pad_0[16];
    unsigned int field_10;
};

struct CountNode {
    unsigned short field_0;
    unsigned char pad_2[2];
    int field_4;
    struct CountNode *next;
};

extern struct DrivingSchoolNode *DAT_004cbeac;
extern unsigned int DAT_004c11c4;
extern unsigned int DAT_004c11c8;
extern void *DAT_004c10d4;
extern void *DAT_004c11bc;
extern unsigned int DAT_0082c6c0;
extern unsigned int DAT_0082c6bc;
extern unsigned int DAT_0082c6b8;
extern unsigned int DAT_0082c690;
extern unsigned int DAT_00830f94;
extern unsigned int DAT_0082c694;
extern unsigned int DAT_0082c6a0;
extern unsigned int DAT_0082c6a4;
extern unsigned int DAT_0082c6a8;
extern unsigned short DAT_0082c6ac;
extern char DRIVING_SCHOOL_SFX[];

extern unsigned int LLIDB_FindElement(char *name, unsigned int *out, unsigned int param_3);
extern void LLIDB_UnLoadData(unsigned int data);
extern void FUN_00411bd0(void);
extern void Kill_FXList(char *list, unsigned int count);
extern void KillSprite(unsigned int sprite);
extern void FUN_0049e4d0(unsigned int ptr);

// FUNCTION: LEGOLAND 0x004051a0
unsigned int FUN_004051a0(unsigned int param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x00405310
void FUN_00405310(unsigned int param_1) {
    struct DrivingSchoolNode *cur;
    struct RetStruct *ret;
    unsigned int tmp;
    unsigned short param_16;

    cur = DAT_004cbeac;
    param_16 = (unsigned short)param_1;
    while (cur != NULL) {
        if (cur->field_8 == param_16) {
            cur->field_18 = 0;
        }
        cur = cur->next;
    }

    /* FUN_00412650 returns struct RideQueueEntry*; driving_school views the same
       object through its own partial RetStruct layout (fields at 0x4/0x15 that
       RideQueueEntry doesn't model), so the pointer is reinterpreted here. */
    ret = (struct RetStruct *)FUN_00412650((unsigned short)param_1);
    ret->field_15 = 0;
    ret->field_4 = 0;

    DAT_004c11c4 = (unsigned int)ret;
    DAT_004c11c8 = 0;

    do {
        FUN_004051a0(param_1);
        tmp = DAT_004c11c8;
        DAT_004c11c4 = tmp;
        DAT_004c11c8 = 0;
    } while (tmp != 0);
}

// FUNCTION: LEGOLAND 0x00405370
void FUN_00405370(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00405460
void FUN_00405460(void) {
    unsigned int local;

    // STRING: LEGOLAND 0x004b454c
    if (LLIDB_FindElement("DSCHOOL MAPPING", &local, 0) == 0) {
        LLIDB_UnLoadData(local);
    }

    // STRING: LEGOLAND 0x004b4538
    if (LLIDB_FindElement("DSCHOOL BLUE CAR", &local, 0) == 0) {
        LLIDB_UnLoadData(local);
    }

    while (DAT_004c10d4 != NULL) {
        FUN_00401c60(DAT_004c10d4);
    }

    FUN_00411bd0();

    while (DAT_004cbeac != NULL) {
        void *next = ((struct Node0 *)DAT_004cbeac)->next;
        FUN_0049e4d0((unsigned int)DAT_004cbeac);
        DAT_004cbeac = next;
    }

    while (DAT_004c11bc != NULL) {
        void *next = ((struct Node8 *)DAT_004c11bc)->next;
        FUN_0049e4d0((unsigned int)DAT_004c11bc);
        DAT_004c11bc = next;
    }

    Kill_FXList(DRIVING_SCHOOL_SFX, 6);
    KillSprite(DAT_0082c6c0);
    FUN_0049e4d0(DAT_0082c6bc);
    FUN_0049e4d0(DAT_0082c6b8);
    FUN_0049e4d0(DAT_0082c690);
    KillSprite(DAT_00830f94);
    DAT_0082c694 = 0;
}

// FUNCTION: LEGOLAND 0x00405570
void FUN_00405570(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00405630
void FUN_00405630(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00405740
void FUN_00405740(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004058a0
void FUN_004058a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00405940
void FUN_00405940(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00405ad0
unsigned int *FUN_00405ad0(struct DSCarLayer *arg1, unsigned short arg2) {
    struct DSCarSub *sub = arg1->field_c;
    struct DSCarInner *inner = sub->field_64;

    DAT_0082c6a0 = (unsigned int)inner;
    DAT_0082c6a4 = sub->field_14;
    DAT_0082c6a8 = sub->field_18;
    DAT_0082c6ac = arg2;

    inner = sub->field_64;
    inner->field_10 |= 0x2000;

    return &DAT_0082c6a0;
}

// FUNCTION: LEGOLAND 0x00405b10
void FUN_00405b10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00405bd0
void FUN_00405bd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00405e70
void FUN_00405e70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00406020
void FUN_00406020(unsigned short arg1, unsigned int arg2) {
    struct CountNode *current = (struct CountNode *)DAT_004c11bc;
    while (current != NULL) {
        if (current->field_0 == arg1) {
            if (current != NULL) {
                current->field_4 += arg2;
            }
            break;
        }
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x00406050
int FUN_00406050(void) {
    struct CountNode *current = (struct CountNode *)DAT_004c11bc;
    int max = 0;

    while (current != NULL) {
        if (current->field_4 > max) {
            max = current->field_4;
        }
        current = current->next;
    }

    return max;
}

// FUNCTION: LEGOLAND 0x00406070
void FUN_00406070(void) { STUB(); }
