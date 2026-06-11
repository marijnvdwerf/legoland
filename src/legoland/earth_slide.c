#include "legoland.h"
#include <stdlib.h>
#include <string.h>

#include "gamemap.h"
#include "man3d.h"
#include "render3d.h"
#include "map_object.h"
#include "globals.h"

struct EarthNode {
    unsigned short field_0;
    unsigned char pad_2[2];
    unsigned int field_4;
    unsigned char field_8;
    unsigned char field_9;
    unsigned char field_a;
    unsigned char field_b;
    unsigned int field_c;
    unsigned int field_10;
    unsigned int field_14;
    unsigned char pad_18[8];
};

struct QueueB {
    struct QueueB *next;
};

struct QueueA {
    unsigned char pad_0[0x1c];
    struct QueueB *field_1c;
    struct QueueB *field_20;
};

struct CellInner {
    unsigned char pad_0[8];
    unsigned int field_8;
};

struct CellMid {
    unsigned char pad_0[4];
    struct CellInner *field_4;
};

struct CellOuter {
    unsigned char pad_0[0x1c];
    struct CellMid *field_1c;
};

struct PoolNode {
    unsigned int next;
};

struct Pool {
    unsigned char pad_0[0x18];
    unsigned char field_18;
    unsigned char pad_19[3];
    unsigned int field_1c;
    unsigned int field_20;
};

struct EarthArg {
    unsigned char pad_0[12];
    unsigned int field_c;
};

struct CursorRoot {
    unsigned char pad_0[0x3c];
    unsigned int field_3c;
};

struct ListNode {
    struct ListNode *next;
};

struct ListNodeC {
    unsigned char pad_0[0xc];
    struct ListNodeC *next;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x0042cd70
void FUN_0042cd70(unsigned short *a1)
{
    struct EarthNode *v = (struct EarthNode *)malloc(0x24);
    if (v != NULL) {
        memset(v, 0, 0x24);
        v->field_0 = *a1;
        v->field_c = (unsigned int)DAT_006160e8;
        v->field_10 = 0;
        v->field_14 = 0;
        v->field_b = 0;
        v->field_8 = 0;
        v->field_a = 0;
        v->field_4 = 1;
        DAT_006160e8 = v;
    }
}

// FUNCTION: LEGOLAND 0x0042cdc0
void FUN_0042cdc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042ce20
void FUN_0042ce20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042ce50
void FUN_0042ce50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042ce90
void FUN_0042ce90(struct QueueA *p, struct QueueB *value)
{
    if (p->field_1c == NULL && p->field_20 == NULL) {
        p->field_1c = value;
        p->field_20 = value;
    } else {
        p->field_20->next = value;
        p->field_20 = value;
    }
}

// FUNCTION: LEGOLAND 0x0042cec0
void FUN_0042cec0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042cf40
int FUN_0042cf40(struct CellOuter *param1, unsigned int param2)
{
    if (param1->field_1c != NULL) {
        if (param1->field_1c->field_4->field_8 == param2) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0042cf70
void FUN_0042cf70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042d040
void FUN_0042d040(struct Pool *list)
{
    if (list->field_1c != 0) {
        struct PoolNode *node = (struct PoolNode *)list->field_1c;
        list->field_1c = node->next;
        if (list->field_20 == (unsigned int)node) {
            list->field_20 = 0;
        }
        list->field_18--;
    }
}

// FUNCTION: LEGOLAND 0x0042d070
void FUN_0042d070(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042d100
void FUN_0042d100(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042d1f0
void FUN_0042d1f0(struct EarthArg *arg1)
{
    DAT_006160d0 = arg1->field_c;
    UnLoadRin(DAT_006160d4);
    UnloadPos(DAT_006160e4);
    KillSprite(DAT_006160d8);
    KillSprite(DAT_006160e0);
}

// FUNCTION: LEGOLAND 0x0042d230
void FUN_0042d230(void)
{
    struct CursorRoot *temp = (struct CursorRoot *)DAT_006160d0;
    EditMode = 1;
    DAT_008119b8 = temp;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&((struct CursorRoot *)DAT_008119b8)->field_3c);
}

// FUNCTION: LEGOLAND 0x0042d270
void FUN_0042d270(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042d2c0
void FUN_0042d2c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042d2f0
void FUN_0042d2f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042d3e0
unsigned int FUN_0042d3e0(struct ListNode *param_1, struct ListNode *param_2)
{
    unsigned int count = 0;
    struct ListNode *node = param_1;

    while (node != NULL && node != param_2) {
        node = node->next;
        count++;
    }
    return count;
}

// FUNCTION: LEGOLAND 0x0042d400
void FUN_0042d400(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042d540
void FUN_0042d540(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0042d560
void FUN_0042d560(struct ListNodeC *param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x0042d5f0
void FUN_0042d5f0(void)
{
    struct ListNodeC *node = (struct ListNodeC *)DAT_006160e8;
    while (node != NULL) {
        FUN_0042d560(node);
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x0042d610
void FUN_0042d610(void) { STUB(); }
