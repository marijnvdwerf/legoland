#include <stdlib.h>
#include "globals.h"
#include "legoland.h"

#include "llidb.h"
#include "ride_bloke.h"
#include "ride_queue.h"
#include "sound_music.h"

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

#include "image_sprite.h"
#include "pumps.h"

// FUNCTION: LEGOLAND 0x004051a0
void FUN_004051a0(short param_1) {
    struct RideQueueEntry *node;
    struct RideQueueEntry *e0;
    struct RideQueueEntry *e1;
    struct RideQueueEntry *e2;
    struct RideQueueEntry *e3;

    for (node = DAT_004c11c4; node != NULL; node = node->field_4) {
        e0 = FUN_004125a0(node->x, node->y - 4);
        e1 = FUN_004125a0(node->x + 4, node->y);
        e2 = FUN_004125a0(node->x, node->y + 4);
        e3 = FUN_004125a0(node->x - 4, node->y);
        if ((node->field_14 & 0xf) == 6) {
            e2 = NULL;
            e3 = NULL;
            e0 = NULL;
        } else if (e0 != NULL && ((short)e0->field_8 != param_1 || e0->field_18 != NULL)) {
            e0 = NULL;
        }
        if (e1 != NULL && ((short)e1->field_8 != param_1 || e1->field_18 != NULL)) {
            e1 = NULL;
        }
        if (e2 != NULL && ((short)e2->field_8 != param_1 || e2->field_18 != NULL)) {
            e2 = NULL;
        }
        if (e3 != NULL && ((short)e3->field_8 != param_1 || e3->field_18 != NULL)) {
            e3 = NULL;
        }
        if (e0 != NULL) {
            e0->field_18 = node;
            e0->field_15 = node->field_15 + 1;
        }
        if (e1 != NULL) {
            e1->field_18 = node;
            e1->field_15 = node->field_15 + 1;
        }
        if (e2 != NULL) {
            e2->field_18 = node;
            e2->field_15 = node->field_15 + 1;
        }
        if (e3 != NULL) {
            e3->field_18 = node;
            e3->field_15 = node->field_15 + 1;
        }
        if (e0 != NULL) {
            e0->field_4 = DAT_004c11c8;
            DAT_004c11c8 = e0;
        }
        if (e1 != NULL) {
            e1->field_4 = DAT_004c11c8;
            DAT_004c11c8 = e1;
        }
        if (e2 != NULL) {
            e2->field_4 = DAT_004c11c8;
            DAT_004c11c8 = e2;
        }
        if (e3 != NULL) {
            e3->field_4 = DAT_004c11c8;
            DAT_004c11c8 = e3;
        }
    }
}

// FUNCTION: LEGOLAND 0x00405310
void FUN_00405310(unsigned int param_1) {
    struct RideQueueEntry *cur;
    struct RideQueueEntry *ret;
    struct RideQueueEntry *tmp;

    cur = DAT_004cbeac;
    while (cur != NULL) {
        if (cur->field_8 == (unsigned short)param_1) {
            cur->field_18 = NULL;
        }
        cur = cur->next;
    }

    ret = FUN_00412650((unsigned short)param_1);
    ret->field_15 = 0;
    ret->field_4 = NULL;

    DAT_004c11c4 = ret;
    DAT_004c11c8 = NULL;

    do {
        FUN_004051a0(param_1);
        tmp = DAT_004c11c8;
        DAT_004c11c4 = tmp;
        DAT_004c11c8 = NULL;
    } while (tmp != NULL);
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
        free((unsigned int)DAT_004cbeac);
        DAT_004cbeac = (struct RideQueueEntry *)next;
    }

    while (DAT_004c11bc != NULL) {
        void *next = ((struct Node8 *)DAT_004c11bc)->next;
        free((unsigned int)DAT_004c11bc);
        DAT_004c11bc = next;
    }

    Kill_FXList(DRIVING_SCHOOL_SFX, 6);
    KillSprite(DAT_0082c6c0);
    free(DAT_0082c6bc);
    free(DAT_0082c6b8);
    free(DAT_0082c690);
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
