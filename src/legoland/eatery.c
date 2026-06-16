#include <stdlib.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "bloke_ai.h"
#include "eatery.h"
#include "gamemap.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "money.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render3d.h"
#include "sound_music.h"

#include "image_sprite.h"

struct EateryObj;
struct EateryFX;
struct EateryInner;
struct BlokeNode;

struct EateryFX {
    /* 0x00 */ unsigned char pad_0[0x14];
    /* 0x14 */ unsigned int field_14;
    /* 0x18 */ unsigned int field_18;
    /* 0x1c */ unsigned int flags_1c;
    /* 0x20 */ unsigned char pad_20[0x44];
    /* 0x64 */ struct EateryInner *inner_64;
};

struct EateryInner {
    unsigned char pad_0[0x10];
    unsigned int flags_10;
};

struct EateryObj {
    unsigned char pad_0[0xc];
    struct EateryFX *fx_c;
};

struct BrollyData {
    /* 0x00 */ unsigned char pad_0[4];
    /* 0x04 */ int count_4;
    /* 0x08 */ int *table_8;
    /* 0x0c */ int *table_c;
    /* 0x10 */ int *table_10;
};

struct BlokeNode {
    struct BlokeNode *next;
    unsigned char pad_4[4];
    unsigned int field_8;
    unsigned short field_c;
};

struct BrollyNode {
    /* 0x00 */ struct BrollyNode *next;
    /* 0x04 */ unsigned short value;
    /* 0x06 */ unsigned short field_6;
    /* 0x08 */ unsigned char field_8;
    /* 0x09 */ unsigned char field_9;
};

struct BlokeOwner {
    unsigned char pad_0[0xcc];
    struct BlokeNode *head_cc;
};

struct BlokeArg {
    unsigned char pad_0[0xc];
    struct BlokeOwner *owner_c;
};

struct EditArg {
    unsigned char pad_0[0xc];
    unsigned int var_c;
};

struct UserFlagsArg {
    int var_0;
    int var_4;
};

struct SaveBlock {
    /* 0x00 */ struct SaveBlock *next;
    /* 0x04 */ unsigned short value;
    /* 0x06 */ unsigned char field_6;
    /* 0x07 */ unsigned char field_7;
    /* 0x08 */ unsigned char field_8;
    /* 0x09 */ unsigned char field_9;
    /* 0x0a */ unsigned char pad_a[0xc - 0xa];
    /* 0x0c */ unsigned int field_c;
    /* 0x10 */ unsigned char field_10;
    /* 0x11 */ unsigned char field_11;
    /* 0x12 */ unsigned char pad_12[0x14 - 0x12];
    /* 0x14 */ unsigned int field_14;
    /* 0x18 */ unsigned int field_18;
    /* 0x1c */ unsigned int field_1c;
    /* 0x20 */ unsigned int field_20;
    /* 0x24 */ unsigned int field_24;
    /* 0x28 */ unsigned int field_28;
    /* 0x2c */ unsigned int field_2c;
    /* 0x30 */ unsigned int field_30;
    /* 0x34 */ unsigned int field_34;
    /* 0x38 */ unsigned int field_38;
    /* 0x3c */ unsigned int field_3c;
};

// FUNCTION: LEGOLAND 0x0042e220
void FUN_0042e220(struct EateryObj *obj) {
    struct EateryFX *fx = obj->fx_c;
    struct EateryInner *inner;
    DAT_0081cd44 = fx;
    fx->flags_1c |= 0x20;
    inner = DAT_0081cd44->inner_64;
    inner->flags_10 |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e250
void FUN_0042e250(void) { KillMoneySFX(); }

// FUNCTION: LEGOLAND 0x0042e260
void FUN_0042e260(struct BlokeArg *arg, unsigned int param2, unsigned int param3, unsigned short *value) {
    struct BlokeOwner *owner = arg->owner_c;
    struct BlokeNode *node = owner->head_cc;
    if (node != NULL) {
        while (node != NULL) {
            if (*value == node->field_c) {
                IP_RenderBlokeIn3DNow((struct Bloke *)node->field_8);
                GetScreenCoordsForObject((unsigned char *)value, owner);
                break;
            }
            node = node->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x0042e2a0
void FUN_0042e2a0(int param_1) {
    int ride = *(int *)(param_1 + 0xc);
    unsigned int *node = *(unsigned int **)(ride + 0xcc);
    unsigned int *next;
    int bloke;
    unsigned char *pos;
    int x;
    int y;
    char cv;
    int mx;
    int my;
    while (node != NULL) {
        next = (unsigned int *)*node;
        bloke = node[2];
        pos = (unsigned char *)(node + 3);
        x = (unsigned int)pos[0] + *(int *)(ride + 0xc);
        y = (unsigned int)pos[1] + *(int *)(ride + 0x10);
        if (*(short *)(bloke + 0xe) == 0) {
            switch (*(unsigned char *)(bloke + 0x60)) {
            case 0:
                *(unsigned char *)(bloke + 0x62) |= 8;
                *(int *)(bloke + 0x24) = x * 0x100;
                y = (y + 1) * 0x100;
                *(int *)(bloke + 0x28) = y;
                x = *(int *)(bloke + 0x24);
                my = *(int *)(bloke + 0x6c);
                mx = *(int *)(bloke + 0x68);
                goto calc;
            case 1:
                x = x * 0x100 + -0x80;
                *(int *)(bloke + 0x28) = (y + 1) * 0x100;
                *(int *)(bloke + 0x24) = x;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), x,
                    *(unsigned int *)(bloke + 0x28), bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)(bloke + 0x60) += 1;
            case 2:
                *(unsigned char *)(bloke + 0x72) = 7;
                *(char *)(bloke + 0x60) += 1;
                *(unsigned int *)(bloke + 0x58) = (rand() & 0x1f) + 4;
                break;
            case 3:
                if (*(int *)(bloke + 0x58) == 0) {
                    *(char *)(bloke + 0x60) += 1;
                    BuyItem(param_1, node + 3, 0);
                }
                *(int *)(bloke + 0x58) += -1;
                break;
            case 4:
                x = x * 0x100 + 0x80;
                *(int *)(bloke + 0x28) = y * 0x100 + 0x80;
                *(int *)(bloke + 0x24) = x;
                y = *(int *)(bloke + 0x28);
                my = *(int *)(bloke + 0x6c);
                mx = *(int *)(bloke + 0x68);
            calc:
                cv = CalcMoveLine(mx, my, x, y, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 5:
                RemoveBlokeFromRide((void *)ride, node);
                *(unsigned short *)(bloke + 0x62) &= 0xfff7;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x0042e460
void FUN_0042e460(struct EateryObj *obj) {
    DAT_0081cd38 = obj->fx_c;
    DAT_0081cd38->flags_1c |= 0x400;
    // STRING: LEGOLAND 0x004b6ec8
    if (LLIDB_FindElement("BROLLY IMAGES", &DAT_0061613c, 0) == 0) {
        DAT_00616140 = (struct BrollyData *)LLIDB_LoadData((void *)DAT_0061613c);
    }
}

// FUNCTION: LEGOLAND 0x0042e4b0
void FUN_0042e4b0(void) {
    LLIDB_UnLoadData(DAT_0061613c);
}

// FUNCTION: LEGOLAND 0x0042e4c0
void FUN_0042e4c0(void) {
    struct EateryFX *temp = DAT_0081cd38;
    EditMode = 1;
    DAT_008119b8 = temp;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((void *)((unsigned int)DAT_008119b8 + 0x3c));
}

// FUNCTION: LEGOLAND 0x0042e500
void FUN_0042e500(int param_1, unsigned char *param_2) {
    unsigned char id[2];
    id[0] = param_2[0];
    id[1] = param_2[4];
    AddObjectToMap(param_1, *(unsigned short *)id, 0);
    if (DAT_00616140 != NULL) {
        int r = rand();
        Set_UserFlags(*(int *)param_2 << 8, *(int *)(param_2 + 4) << 8, (unsigned short)(r % DAT_00616140->count_4));
    }
}

// FUNCTION: LEGOLAND 0x0042e560
unsigned int *FUN_0042e560(int param_1, unsigned int param_2) {
    unsigned char *b = (unsigned char *)&param_2;
    int idx = (Get_UserFlags((unsigned int)b[0] << 8, (unsigned int)b[1] << 8) & 0xff) * 4;
    DAT_0082c6a0 = *(int *)((char *)DAT_00616140->table_8 + idx);
    DAT_0082c6a4 = *(int *)((char *)DAT_00616140->table_c + idx) >> 1;
    DAT_0082c6b0 = 0;
    DAT_0082c6a8 = *(int *)((char *)DAT_00616140->table_10 + idx) >> 1;
    return &DAT_0082c6a0;
}

// FUNCTION: LEGOLAND 0x0042e5d0
void FUN_0042e5d0(struct EateryObj *obj) {
    struct EateryFX *fx = obj->fx_c;
    struct EateryInner *inner;
    DAT_0081cd18 = fx;
    DAT_0081cd18->flags_1c |= 0x20;
    inner = DAT_0081cd18->inner_64;
    inner->flags_10 |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e600
void FUN_0042e600(void) { KillMoneySFX(); }

// FUNCTION: LEGOLAND 0x0042e610
void FUN_0042e610(int param_1) {
    int ride = *(int *)(param_1 + 0xc);
    unsigned int *node = *(unsigned int **)(ride + 0xcc);
    unsigned int *next;
    int bloke;
    unsigned char b1;
    int x;
    int y;
    char cv;
    char state;
    while (node != NULL) {
        next = (unsigned int *)*node;
        bloke = node[2];
        y = *(int *)(ride + 0x10);
        x = (unsigned int)*(unsigned char *)(node + 3) + *(int *)(ride + 0xc);
        b1 = *((unsigned char *)node + 0xd);
        if (*(short *)(bloke + 0xe) == 0) {
            state = *(char *)(bloke + 0x60);
            switch (state) {
            case 0:
                *(unsigned char *)(bloke + 0x62) |= 8;
                x = x * 0x100 + -0x80;
                goto calc;
            case 1:
                *(unsigned char *)(bloke + 0x72) = 7;
                *(char *)(bloke + 0x60) = state + 1;
                *(unsigned int *)(bloke + 0x58) = (rand() & 0x1f) + 4;
                break;
            case 2:
                if (*(int *)(bloke + 0x58) == 0) {
                    *(char *)(bloke + 0x60) = state + 1;
                    BuyItem(param_1, node + 3, 1);
                }
                *(int *)(bloke + 0x58) += -1;
                break;
            case 3:
                x = x * 0x100 + 0x80;
            calc:
                y = ((unsigned int)b1 + y) * 0x100 + 0x80;
                *(int *)(bloke + 0x24) = x;
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c),
                    *(unsigned int *)(bloke + 0x24), y, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 4:
                RemoveBlokeFromRide((void *)ride, node);
                *(unsigned short *)(bloke + 0x62) &= 0xfff7;
                NewLongTermAction((struct Bloke *)bloke, 0xd);
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x0042e770
void FUN_0042e770(struct EateryObj *obj) {
    struct EateryFX *fx = obj->fx_c;
    struct EateryInner *inner;
    DAT_0081cd14 = (unsigned int)fx;
    fx->flags_1c |= 0x20;
    inner = ((struct EateryFX *)DAT_0081cd14)->inner_64;
    inner->flags_10 |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e7a0
void FUN_0042e7a0(void) { KillMoneySFX(); }

// FUNCTION: LEGOLAND 0x0042e7b0
void FUN_0042e7b0(struct EateryObj *obj) {
    struct EateryFX *fx = obj->fx_c;
    struct EateryInner *inner;
    DAT_0081cde0 = fx;
    fx->flags_1c |= 0x20;
    inner = DAT_0081cde0->inner_64;
    inner->flags_10 |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e7e0
void FUN_0042e7e0(void) { KillMoneySFX(); }

// FUNCTION: LEGOLAND 0x0042e7f0
void FUN_0042e7f0(struct EateryObj *obj) {
    struct EateryFX *fx = obj->fx_c;
    struct EateryInner *inner;
    DAT_0081cd3c = (unsigned int)fx;
    fx->flags_1c |= 0x20;
    inner = ((struct EateryFX *)DAT_0081cd3c)->inner_64;
    inner->flags_10 |= 0x2000;
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e820
void FUN_0042e820(void) { KillMoneySFX(); }

// FUNCTION: LEGOLAND 0x0042e830
void FUN_0042e830(struct BlokeArg *arg, unsigned int param2, unsigned int param3, unsigned short *value) {
    struct BlokeOwner *owner = arg->owner_c;
    struct BlokeNode *node = owner->head_cc;
    if (node == NULL) {
        return;
    }
    while (node != NULL) {
        if (*value == node->field_c) {
            IP_RenderBlokeIn3DNow((struct Bloke *)node->field_8);
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x0042e870
void FUN_0042e870(struct EateryObj *obj) {
    DAT_0081cd0c = (unsigned int)obj->fx_c;
    ((struct EateryFX *)DAT_0081cd0c)->flags_1c |= 0x20;
    DAT_0081cd10 = (unsigned int)((struct EateryFX *)DAT_0081cd0c)->inner_64;
    ((struct EateryInner *)DAT_0081cd10)->flags_10 |= 0x2000;
    Load_FXList(RESTAURANT_SFX, 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e8b0
void FUN_0042e8b0(void) {
    Kill_FXList(RESTAURANT_SFX, 1);
    DAT_0081cd0c = 0;
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042e8d0
void FUN_0042e8d0(struct EditArg *arg) {
    unsigned int temp = arg->var_c;
    EditMode = 1;
    DAT_008119b8 = (void *)temp;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((void *)((unsigned int)DAT_008119b8 + 0x3c));
}

// FUNCTION: LEGOLAND 0x0042e910
void FUN_0042e910(int param_1, unsigned int param_2, unsigned int param_3, short *param_4, unsigned int param_5, unsigned int param_6) {
    int ride = *(int *)(param_1 + 0xc);
    unsigned int *node;
    union {
        __int64 q;
        int i[2];
    } coords;
    union {
        __int64 q;
        int i[2];
    } offset;
    for (node = *(unsigned int **)(ride + 0xcc); node != NULL; node = (unsigned int *)*node) {
        if (*param_4 == *(short *)(node + 3)) {
            IP_RenderBlokeIn3DNow((struct Bloke *)node[2]);
        }
    }
    coords.q = GetScreenCoordsForObject((unsigned char *)param_4, (void *)ride);
    offset.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_0081cd10, 1);
    AdjustOffsetForViewMode((struct AdjustStruct *)&offset);
    if (GetSpriteForLayer((struct LayerContainer *)DAT_0081cd10, 1) != 0) {
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_0081cd10, 1),
            offset.i[0] + coords.i[0], offset.i[1] + coords.i[1], param_6, 0);
    }
}

// FUNCTION: LEGOLAND 0x0042e9c0
void FUN_0042e9c0(unsigned int param_1, unsigned int *param_2) {
    struct SampleParams params;
    AddBasicObject(param_1, (unsigned int)param_2);
    params.field_8 = *param_2;
    params.field_c = param_2[1];
    params.field_0 = 2;
    PlayInstanceOfSample(*(void **)(RESTAURANT_SFX + 8), 1, 1, &params);
}

// FUNCTION: LEGOLAND 0x0042ea10
void FUN_0042ea10(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    unsigned char *b = (unsigned char *)&param_2;
    struct SampleParams params;
    StandardRemoveObject(param_1, param_2, param_3);
    params.field_8 = b[0];
    params.field_c = b[1];
    params.field_0 = 2;
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
}

// FUNCTION: LEGOLAND 0x0042ea60
void FUN_0042ea60(int param_1) {
    int ride = *(int *)(param_1 + 0xc);
    unsigned int *node = *(unsigned int **)(ride + 0xcc);
    unsigned int *next;
    int bloke;
    unsigned char *pos;
    int x;
    int y;
    char cv;
    int mx;
    int my;
    while (node != NULL) {
        next = (unsigned int *)*node;
        bloke = node[2];
        pos = (unsigned char *)(node + 3);
        x = (unsigned int)pos[0] + *(int *)(ride + 0xc);
        y = (unsigned int)pos[1] + *(int *)(ride + 0x10);
        if (*(short *)(bloke + 0xe) == 0) {
            switch (*(char *)(bloke + 0x60)) {
            case 0:
                *(unsigned char *)(bloke + 0x62) |= 8;
                x = x * 0x100 + 0x280;
                goto calc05;
            case 1:
                x = x * 0x100 + 0x280;
                *(int *)(bloke + 0x28) = y * 0x100;
                *(int *)(bloke + 0x24) = x;
                y = *(int *)(bloke + 0x28);
                my = *(int *)(bloke + 0x6c);
                mx = *(int *)(bloke + 0x68);
                goto calc;
            case 2:
                *(unsigned int *)(bloke + 0x58) = (rand() & 0x1f) + 4;
                *(char *)(bloke + 0x60) += 1;
                break;
            case 3:
                if (*(int *)(bloke + 0x58) == 0) {
                    *(char *)(bloke + 0x60) += 1;
                    PlayMoneySFX(node + 3, 0, 0);
                }
                *(int *)(bloke + 0x58) += -1;
                break;
            case 4:
                *(int *)(bloke + 0x24) = x * 0x100 + 0x280;
                y = y * 0x100 + 0x80;
                *(int *)(bloke + 0x28) = y;
                x = *(int *)(bloke + 0x24);
                my = *(int *)(bloke + 0x6c);
                mx = *(int *)(bloke + 0x68);
                goto calc;
            case 5:
                x = x * 0x100 + 0x80;
            calc05:
                y = y * 0x100 + 0x80;
                *(int *)(bloke + 0x24) = x;
                *(int *)(bloke + 0x28) = y;
                x = *(int *)(bloke + 0x24);
                my = *(int *)(bloke + 0x6c);
                mx = *(int *)(bloke + 0x68);
            calc:
                cv = CalcMoveLine(mx, my, x, y, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 6:
                RemoveBlokeFromRide((void *)ride, node);
                *(unsigned short *)(bloke + 0x62) &= 0xfff7;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x0042ec10
void FUN_0042ec10(int param_1) {
    int ride = *(int *)(param_1 + 0xc);
    unsigned int *node = *(unsigned int **)(ride + 0xcc);
    unsigned int *next;
    int bloke;
    unsigned char *pos;
    int x;
    int y;
    char cv;
    char state;
    while (node != NULL) {
        next = (unsigned int *)*node;
        bloke = node[2];
        pos = (unsigned char *)(node + 3);
        x = (unsigned int)pos[0] + *(int *)(ride + 0xc);
        y = (unsigned int)pos[1] + *(int *)(ride + 0x10);
        if (*(short *)(bloke + 0xe) == 0) {
            state = *(char *)(bloke + 0x60);
            switch (state) {
            case 0:
                *(unsigned char *)(bloke + 0x62) |= 8;
                *(int *)(bloke + 0x24) = x * 0x100 + -0x80;
                x = y * 0x100 + 0x180;
                goto calc;
            case 1:
                *(unsigned char *)(bloke + 0x72) = 7;
                *(char *)(bloke + 0x60) = state + 1;
                *(unsigned int *)(bloke + 0x58) = (rand() & 0x1f) + 4;
                break;
            case 2:
                if (*(int *)(bloke + 0x58) == 0) {
                    *(char *)(bloke + 0x60) = state + 1;
                    BuyItem(param_1, node + 3, 1);
                }
                *(int *)(bloke + 0x58) += -1;
                break;
            case 3:
                *(unsigned char *)(bloke + 0x72) = 3;
                *(int *)(bloke + 0x24) = x * 0x100 + 0x80;
                x = y * 0x100 + 0x80;
            calc:
                *(int *)(bloke + 0x28) = x;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c),
                    *(unsigned int *)(bloke + 0x24), x, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 4:
                RemoveBlokeFromRide((void *)ride, node);
                *(unsigned short *)(bloke + 0x62) &= 0xfff7;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x0042ed70
void FUN_0042ed70(int param_1) {
    int ride = *(int *)(param_1 + 0xc);
    unsigned int *node = *(unsigned int **)(ride + 0xcc);
    unsigned int *next;
    int bloke;
    unsigned char b1;
    int x;
    int y;
    char cv;
    char state;
    while (node != NULL) {
        next = (unsigned int *)*node;
        bloke = node[2];
        y = *(int *)(ride + 0x10);
        x = (unsigned int)*(unsigned char *)(node + 3) + *(int *)(ride + 0xc);
        b1 = *((unsigned char *)node + 0xd);
        if (*(short *)(bloke + 0xe) == 0) {
            state = *(char *)(bloke + 0x60);
            switch (state) {
            case 0:
                *(unsigned char *)(bloke + 0x62) |= 8;
                x = x * 0x100 + -0x80;
                goto calc;
            case 1:
                *(unsigned char *)(bloke + 0x72) = 7;
                *(char *)(bloke + 0x60) = state + 1;
                *(unsigned int *)(bloke + 0x58) = (rand() & 0x1f) + 4;
                break;
            case 2:
                if (*(int *)(bloke + 0x58) == 0) {
                    *(char *)(bloke + 0x60) = state + 1;
                    BuyItem(param_1, node + 3, 1);
                }
                *(int *)(bloke + 0x58) += -1;
                break;
            case 3:
                x = x * 0x100 + 0x80;
            calc:
                y = ((unsigned int)b1 + y) * 0x100 + 0x80;
                *(int *)(bloke + 0x24) = x;
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c),
                    *(unsigned int *)(bloke + 0x24), y, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 4:
                RemoveBlokeFromRide((void *)ride, node);
                *(unsigned short *)(bloke + 0x62) &= 0xfff7;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x0042eec0
struct BrollyNode *FUN_0042eec0(unsigned short *param_1) {
    struct BrollyNode *node = malloc(sizeof(struct BrollyNode));
    if (node != NULL) {
        node->next = NULL;
        node->value = 0;
        node->field_6 = 0;
        node->field_8 = 0;
        node->field_9 = 0;
        node->value = *param_1;
        node->next = DAT_00616144;
        node->field_6 = 0;
        node->field_8 = 0;
        node->field_9 = 0;
        DAT_00616144 = node;
    }
    return node;
}

// FUNCTION: LEGOLAND 0x0042ef10
void FUN_0042ef10(unsigned int param_1, unsigned char *param_2) {
    unsigned char id[2];
    id[0] = param_2[0];
    id[1] = param_2[4];
    AddBasicObject(param_1, (unsigned int)param_2);
    FUN_0042eec0((unsigned short *)id);
}

// FUNCTION: LEGOLAND 0x0042ef40
struct BrollyNode *FUN_0042ef40(unsigned short *param_1) {
    struct BrollyNode *node = DAT_00616144;
    while (node != NULL) {
        if (node->value == *param_1) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0042ef70
void FUN_0042ef70(struct BrollyNode *node) {
    struct BrollyNode *head = DAT_00616144;
    if (head == node) {
        DAT_00616144 = node->next;
    } else {
        struct BrollyNode *current = head;
        if (current->next != node) {
            do {
                current = current->next;
            } while (current && current->next != node);
        }
        if (current) {
            current->next = node->next;
        }
    }
    free(node);
}

// FUNCTION: LEGOLAND 0x0042efb0
void FUN_0042efb0(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    struct SampleParams params;
    struct BrollyNode *node = FUN_0042ef40((unsigned short *)&param_2);
    if (node != NULL) {
        FUN_0042ef70(node);
    }
    StandardRemoveObject(param_1, param_2, param_3);
    RemoveAllBlokesFromRide((unsigned int)((struct EateryObj *)param_1)->fx_c, (void *)param_2);
    params.field_8 = ((unsigned char *)&param_2)[0];
    params.field_c = ((unsigned char *)&param_2)[1];
    params.field_0 = 2;
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
}

// FUNCTION: LEGOLAND 0x0042f030
void FUN_0042f030(struct EateryObj *obj) {
    DAT_0081cd40 = (unsigned int)obj->fx_c;
    ((struct EateryFX *)DAT_0081cd40)->flags_1c |= 0x20;
    DAT_0081cd2c = (unsigned int)((struct EateryFX *)DAT_0081cd40)->inner_64;
    ((struct EateryInner *)DAT_0081cd2c)->flags_10 |= 0x2000;
    // STRING: LEGOLAND 0x004b6f2c
    DAT_0081cd28 = LoadSprite("RestMask_Main.lls", 1);
    // STRING: LEGOLAND 0x004b6f14
    DAT_0081cd8c = LoadSprite("RestMaskLevel1aa.lls", 1);
    // STRING: LEGOLAND 0x004b6f00
    DAT_0081cd88 = LoadSprite("RestMaskLevel1.lls", 1);
    // STRING: LEGOLAND 0x004b6eec
    DAT_0081cd94 = LoadSprite("RestMaskLevel2.lls", 1);
    // STRING: LEGOLAND 0x004b6ed8
    DAT_0081cd90 = LoadSprite("RestMaskLevel3.lls", 1);
    HideLayer((struct LayerOwner *)DAT_0081cd2c, 1);
    StopLayerPlaying(DAT_0081cd2c, 1);
    LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_0081cd2c, 1), 0);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042f0f0
void FUN_0042f0f0(int param_1, int param_2, int param_3, int param_4) {
    int idx = (param_4 + (unsigned int)*(unsigned char *)(param_1 + 0x36) * 5) * 6;
    int y_add = DAT_004b66f4[idx + 1];
    int dir_flag = DAT_004b66f4[idx + 4];
    int frame = DAT_004b66f4[idx + 5];
    char cv;
    int y;
    *(int *)(param_1 + 0x24) = (DAT_004b66f4[idx] + param_2) * 0x100 + DAT_004b66f4[idx + 2];
    y = (param_3 + y_add) * 0x100 + DAT_004b66f4[idx + 3];
    *(int *)(param_1 + 0x28) = y;
    cv = CalcMoveLine(*(unsigned int *)(param_1 + 0x68), *(unsigned int *)(param_1 + 0x6c),
        *(unsigned int *)(param_1 + 0x24), y, param_1 + 0x98);
    *(unsigned char *)(param_1 + 0x73) = cv + 0x10;
    *(short *)(param_1 + 0xe) = 7;
    *(char *)(param_1 + 0x37) = (char)frame;
    if (dir_flag == 1) {
        NewDirForAction(param_1, ((unsigned char)(cv + 0x10) >> 5) + 3);
    }
}

// FUNCTION: LEGOLAND 0x0042f1a0
void FUN_0042f1a0(int param_1) {
    unsigned char *pos;
    int bloke;
    unsigned int *node;
    char cv;
    int x;
    int y;
    struct SaveBlock *state;
    char last_zero = 0;
    char zero_count = 0;
    char choices[3];
    char *pc;
    int ride = *(int *)(param_1 + 0xc);
    unsigned int *next;
    int arg;
    node = *(unsigned int **)(*(int *)(param_1 + 0xc) + 0xcc);
    while (node != NULL) {
        next = (unsigned int *)*node;
        bloke = node[2];
        pos = (unsigned char *)(node + 3);
        state = FUN_0042f9d0((unsigned short *)pos);
        if (state == NULL) {
            return;
        }
        *(short *)choices = *(short *)((char *)state + 6);
        choices[2] = *((char *)state + 8);
        x = *(int *)(ride + 0xc) + (unsigned int)*pos;
        y = (unsigned int)*((unsigned char *)node + 0xd) + *(int *)(ride + 0x10);
        if (*(short *)(bloke + 0xe) == 0) {
            cv = *(char *)(bloke + 0x60);
            switch (cv) {
            case 0:
                *(unsigned char *)(bloke + 0x62) |= 8;
                x = (x + -6) * 0x100;
                *(unsigned char *)(bloke + 0x37) = 3;
                *(int *)(bloke + 0x24) = x;
                *(int *)(bloke + 0x28) = y * 0x100;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), x, y * 0x100, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                cv = 0;
                pc = choices;
                do {
                    if (*pc == 0) {
                        zero_count = zero_count + 1;
                        last_zero = cv;
                    }
                    cv = cv + 1;
                    pc = pc + 1;
                } while (cv < 3);
                if (zero_count == 0) {
                    *(short *)(bloke + 0x46) = 4;
                    *(int *)(bloke + 0x5c) = 500;
                    *(char *)(bloke + 0x60) += 1;
                } else {
                    cv = last_zero;
                    if (zero_count == 3) {
                        cv = (char)(rand() % 3);
                    }
                    *(char *)(bloke + 0x36) = cv;
                    *(short *)(bloke + 0x46) = 3;
                    choices[(int)cv] = 1;
                    *(int *)(bloke + 0x5c) = 300;
                    *(char *)(bloke + 0x60) += 1;
                }
                break;
            case 1:
                BuyItem(param_1, pos, 1);
                if (*(short *)(bloke + 0x46) != 4) {
                    arg = 0;
                    goto move;
                }
                *(unsigned char *)(bloke + 0x60) = 8;
                break;
            case 2:
                arg = 1;
                goto move;
            case 3:
                arg = 2;
            move:
                FUN_0042f0f0(bloke, x, y, arg);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 4:
                *(unsigned char *)(bloke + 0x63) |= 1;
                *(short *)(bloke + 0x70) = 10;
                BlokeSitAnim(bloke);
                BlokeSetFrame(bloke, 0);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 5:
                arg = *(int *)(bloke + 0x5c);
                *(int *)(bloke + 0x5c) = arg + -1;
                if (arg < 0) {
                    *(char *)(bloke + 0x60) = cv + 1;
                }
                break;
            case 6:
                *(unsigned short *)(bloke + 0x62) &= 0xfeff;
                *(short *)(bloke + 0x70) = 0;
                BlokeWalkAnim((struct Bloke *)bloke);
                FUN_0042f0f0(bloke, x, y, 3);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 7:
                FUN_0042f0f0(bloke, x, y, 4);
                *(char *)(bloke + 0x60) += 2;
                choices[*(unsigned char *)(bloke + 0x36)] = 0;
                break;
            case 8:
                arg = *(int *)(bloke + 0x5c);
                *(int *)(bloke + 0x5c) = arg + -1;
                if (arg < 0) {
                    *(char *)(bloke + 0x60) = cv + 1;
                }
            case 9:
                y = (y + 1) * 0x100;
                *(unsigned char *)(bloke + 0x37) = 3;
                *(int *)(bloke + 0x24) = x * 0x100;
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), x * 0x100, y, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 10:
                RemoveBlokeFromRide((void *)ride, node);
                *(unsigned short *)(bloke + 0x62) &= 0xfff7;
            }
        }
        *(short *)((char *)state + 6) = *(short *)choices;
        *((char *)state + 8) = choices[2];
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x0042f4c0
void FUN_0042f4c0(int param_1, unsigned int param_2, unsigned int param_3, short *param_4, unsigned int param_5, unsigned int param_6) {
    int ride = *(int *)(param_1 + 0xc);
    int *node;
    int blokes[8];
    int count;
    int i;
    int *p;
    struct BrollyNode *state;
    union {
        __int64 q;
        int i[2];
    } coords;
    union {
        __int64 q;
        int i[2];
    } offset;
    struct {
        /* 0x00 */ int field_0;
        /* 0x04 */ int field_4;
        /* 0x08 */ short field_8;
    } cfg;
    node = *(int **)(ride + 0xcc);
    cfg.field_0 = 0x103;
    cfg.field_4 = *(int *)(ride + 0xc4);
    cfg.field_8 = *param_4;
    p = blokes;
    blokes[0] = 0;
    for (i = 7; p = p + 1, i != 0; i = i - 1) {
        *p = 0;
    }
    count = 0;
    coords.q = GetScreenCoordsForObject((unsigned char *)param_4, (void *)ride);
    if (node != NULL) {
        short v = *param_4;
        p = blokes;
        do {
            if (v == (short)node[3]) {
                count = count + 1;
                *p = node[2];
                p = p + 1;
            }
            node = (int *)*node;
        } while (node != NULL);
        if (count != 0) {
            for (i = 0; i < count; i = i + 1) {
                if (*(char *)(blokes[i] + 0x37) == 1) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
            }
            PrintSprite(DAT_0081cd8c, param_2, param_3, param_6, 0);
            for (i = 0; i < count; i = i + 1) {
                if (*(char *)(blokes[i] + 0x37) == 2) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
            }
            PrintSprite(DAT_0081cd88, param_2, param_3, param_6, 0);
            for (i = 0; i < count; i = i + 1) {
                if (*(char *)(blokes[i] + 0x37) == 3) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
            }
            PrintSprite(DAT_0081cd94, param_2, param_3, param_6, 0);
            for (i = 0; i < count; i = i + 1) {
                if (*(char *)(blokes[i] + 0x37) == 4) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
            }
            PrintSprite(DAT_0081cd90, param_2, param_3, param_6, 0);
            for (i = 0; i < count; i = i + 1) {
                if (*(char *)(blokes[i] + 0x37) == 5) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
            }
            PrintSprite(DAT_0081cd28, param_2, param_3, param_6, 0);
        }
    }
    state = FUN_0042ef40((unsigned short *)param_4);
    if (state != NULL) {
        char frame = state->field_9 + 1;
        if (frame > 0xf) {
            frame = 0;
        }
        LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_0081cd2c, 1), frame);
        offset.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_0081cd2c, 1);
        AdjustOffsetForViewMode((struct AdjustStruct *)&offset);
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_0081cd2c, 1),
            offset.i[0] + coords.i[0], offset.i[1] + coords.i[1], param_6, (int *)&cfg);
        state->field_9 = frame;
    }
}

// FUNCTION: LEGOLAND 0x0042f720
void FUN_0042f720(void) {
    KillSprite(DAT_0081cd28);
    KillSprite(DAT_0081cd8c);
    KillSprite(DAT_0081cd88);
    KillSprite(DAT_0081cd94);
    KillSprite(DAT_0081cd90);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x0042f770
void FUN_0042f770(struct EateryObj *obj) {
    Load_FXList(OCTOPUS_SFX, 3);
    LoadMoneySFX();
    DAT_0081cd30 = (unsigned int)obj->fx_c;
    ((struct EateryFX *)DAT_0081cd30)->flags_1c |= 0x420;
    DAT_00616118 = (unsigned int)((struct EateryFX *)DAT_0081cd30)->inner_64;
    ((struct EateryInner *)DAT_00616118)->flags_10 |= 0x2000;
    // STRING: LEGOLAND 0x004b6f70
    DAT_0081cd34 = LoadSprite("R2Fdoor_m.lls", 1);
    // STRING: LEGOLAND 0x004b6f60
    DAT_0081cd48 = LoadSprite("R2Fdoor_m1.lls", 1);
    // STRING: LEGOLAND 0x004b6f50
    DAT_0081cd84 = LoadSprite("R2Bdoor_m.lls", 1);
    // STRING: LEGOLAND 0x004b6f40
    DAT_0081cd20 = LoadSprite("R2Tower_m.lls", 1);
    HideLayer((struct LayerOwner *)DAT_00616118, 0);
    StopLayerPlaying(DAT_00616118, 0);
    LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 0), 0);
    HideLayer((struct LayerOwner *)DAT_00616118, 6);
    StopLayerPlaying(DAT_00616118, 6);
    LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 6), 0);
    HideLayer((struct LayerOwner *)DAT_00616118, 2);
    StopLayerPlaying(DAT_00616118, 2);
    LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 2), 0);
    HideLayer((struct LayerOwner *)DAT_00616118, 5);
    HideLayer((struct LayerOwner *)DAT_00616118, 1);
    StopLayerPlaying(DAT_00616118, 1);
    LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 1), 0);
    HideLayer((struct LayerOwner *)DAT_00616118, 3);
    StopLayerPlaying(DAT_00616118, 3);
    LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 3), 0);
}

// FUNCTION: LEGOLAND 0x0042f920
void FUN_0042f920(unsigned short *param_1) {
    struct SaveBlock *node = malloc(sizeof(struct SaveBlock));
    if (node != NULL) {
        unsigned int *p = (unsigned int *)node;
        int i;
        for (i = 0x10; i != 0; i = i - 1) {
            *p = 0;
            p = p + 1;
        }
        node->value = *param_1;
        node->next = DAT_00616148;
        node->field_6 = 0;
        node->field_7 = 0;
        node->field_8 = 0;
        node->field_9 = 0;
        node->field_c = 0;
        node->field_10 = 0;
        node->field_11 = 0;
        node->field_14 = 0;
        node->field_18 = 0;
        node->field_1c = 0;
        node->field_20 = 0;
        node->field_24 = 0;
        node->field_28 = 0;
        node->field_2c = 0;
        node->field_30 = 0;
        node->field_34 = 0;
        node->field_38 = 0x143;
        node->field_3c = 0;
        DAT_00616148 = node;
    }
}

// FUNCTION: LEGOLAND 0x0042f9a0
void FUN_0042f9a0(unsigned int param_1, unsigned char *param_2) {
    unsigned char id[2];
    id[0] = param_2[0];
    id[1] = param_2[4];
    AddBasicObject(param_1, (unsigned int)param_2);
    FUN_0042f920((unsigned short *)id);
}

// FUNCTION: LEGOLAND 0x0042f9d0
struct SaveBlock *FUN_0042f9d0(unsigned short *param) {
    struct SaveBlock *node = DAT_00616148;
    while (node != NULL) {
        if (node->value == *param) {
            return node;
        }
        if (node->next == NULL) {
            break;
        }
        node = node->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0042fa00
void FUN_0042fa00(struct SaveBlock *param) {
    struct SaveBlock *node;
    struct SaveBlock *prev;
    if (DAT_00616148 == param) {
        DAT_00616148 = param->next;
        goto done;
    }
    node = DAT_00616148->next;
    prev = DAT_00616148;
    while (node != param) {
        prev = prev->next;
        if (prev == NULL) {
            goto done;
        }
        node = prev->next;
    }
    if (prev != NULL) {
        prev->next = param->next;
    }
done:
    free(param);
}

// FUNCTION: LEGOLAND 0x0042fa40
void FUN_0042fa40(unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4, unsigned int arg5) {
    struct SaveBlock *result = FUN_0042f9d0((unsigned short *)&arg2);
    if (result != NULL) {
        FUN_0042fa00(result);
    }
    StandardRemoveObject(arg1, arg2, arg3);
    RemoveAllBlokesFromRide((unsigned int)((struct EateryObj *)arg1)->fx_c, (void *)arg2);
}

// FUNCTION: LEGOLAND 0x0042fa90
void FUN_0042fa90(int param_1, int param_2, int param_3) {
    int bloke = *(int *)(param_1 + 8);
    if (param_2 == 1) {
        *(int *)(bloke + 0x68) = *(int *)(bloke + 0x68) + DAT_004b6860[param_3] * -8;
        *(int *)(*(int *)(param_1 + 8) + 0x6c) = *(int *)(*(int *)(param_1 + 8) + 0x6c) + DAT_004b6860[param_3] * -8;
    } else {
        *(int *)(bloke + 0x68) = *(int *)(bloke + 0x68) + DAT_004b68e0[param_3] * 8;
        *(int *)(*(int *)(param_1 + 8) + 0x6c) = *(int *)(*(int *)(param_1 + 8) + 0x6c) + DAT_004b68e0[param_3] * 8;
    }
}

// FUNCTION: LEGOLAND 0x0042fb00
void FUN_0042fb00(unsigned int param_1) {
    unsigned char *b = (unsigned char *)&param_1;
    struct SampleParams params;
    params.field_8 = b[0];
    params.field_c = b[1];
    params.field_0 = 2;
    PlayInstanceOfSample(*(void **)(OCTOPUS_SFX + 8), 0, 1, &params);
    PlayInstanceOfSample(*(void **)(OCTOPUS_SFX + 0x14), 1, 1, &params);
}

// FUNCTION: LEGOLAND 0x0042fb60
void FUN_0042fb60(unsigned int param_1) {
    unsigned char *b = (unsigned char *)&param_1;
    struct SampleParams params;
    params.field_0 = 2;
    params.field_8 = b[0];
    params.field_c = b[1];
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
    PlayInstanceOfSample(*(void **)(OCTOPUS_SFX + 0x20), 0, 1, &params);
}

// FUNCTION: LEGOLAND 0x0042fbb0
void FUN_0042fbb0(int param_1) {
    unsigned char *pos;
    char cv;
    int bloke;
    unsigned int *node;
    unsigned int *next;
    struct SaveBlock *st;
    char f7;
    unsigned char f8;
    int fc;
    unsigned char f10;
    unsigned char f11;
    int f14;
    int f18;
    int f1c;
    int f20;
    int f24;
    int f28;
    int f2c;
    int f30;
    int f38;
    int f3c;
    int x;
    int y;
    int ride = *(int *)(param_1 + 0xc);
    int t;
    node = *(unsigned int **)(ride + 0xcc);
    while (node != NULL) {
        next = (unsigned int *)*node;
        bloke = node[2];
        pos = (unsigned char *)(node + 3);
        st = FUN_0042f9d0((unsigned short *)pos);
        if (st == NULL) {
            return;
        }
        f7 = st->field_7;
        f8 = st->field_8;
        fc = st->field_c;
        f10 = st->field_10;
        f11 = st->field_11;
        f14 = st->field_14;
        f18 = st->field_18;
        f1c = st->field_1c;
        f20 = st->field_20;
        f24 = st->field_24;
        f28 = st->field_28;
        f2c = st->field_2c;
        f30 = st->field_30;
        f38 = st->field_38;
        f3c = st->field_3c;
        x = *(int *)(ride + 0xc) + (unsigned int)*pos;
        y = (unsigned int)*((unsigned char *)node + 0xd) + *(int *)(ride + 0x10);
        if (*(short *)(bloke + 0xe) == 0) {
            cv = *(char *)(bloke + 0x60);
            switch (cv) {
            case 0:
                *(unsigned char *)(bloke + 0x62) |= 8;
                *(int *)(bloke + 0x24) = x * 0x100 + 0x3c8;
                y = (y + -2) * 0x100;
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), *(unsigned int *)(bloke + 0x24), y, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                fc = fc + 1;
                *(int *)(bloke + 0x5c) = 300;
                *(unsigned short *)(bloke + 0x44) = (unsigned short)*(unsigned char *)(bloke + 0x7f);
                *(unsigned char *)(bloke + 0x7f) = 0x15;
                *(char *)(bloke + 0x60) += 1;
                break;
            case 1:
                *(int *)(bloke + 0x24) = x * 0x100 + 0x3c8;
                y = (y + -4) * 0x100 + fc * 100;
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), *(unsigned int *)(bloke + 0x24), y, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 2:
                if (f10 < 3 && f20 != 0) {
                    x = x * 0x100 + 0x2ce;
                    *(int *)(bloke + 0x28) = y * 0x100 + -0x39c;
                    *(int *)(bloke + 0x24) = x;
                    cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), x, *(unsigned int *)(bloke + 0x28), bloke + 0x98);
                    *(short *)(bloke + 0xe) = 7;
                    *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                    NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                    f10 = f10 + 1;
                    *(char *)(bloke + 0x60) += 1;
                }
                break;
            case 3:
                *(int *)(bloke + 0x24) = x * 0x100 + 0x16a;
                y = y * 0x100 + -0x39c;
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), *(unsigned int *)(bloke + 0x24), y, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 4:
                *(int *)(bloke + 0x24) = x * 0x100 + 0x16a;
                y = y * 0x100 + -0x532 + (unsigned int)f10 * 100;
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), *(unsigned int *)(bloke + 0x24), y, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                fc = fc + -1;
                f14 = f14 + 1;
                *(char *)(bloke + 0x60) += 1;
                if (f10 == 3 || fc == 0) {
                    f20 = 0;
                    f14 = 3;
                }
                break;
            case 5:
                if (2 < f14) {
                    f24 = 1;
                    f14 = 0;
                }
                SetPersonDirection(node[4], 5);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 6:
                if (f18 == 2) {
                    if (f28 == 0) {
                        FUN_0042fa90((int)node, 1, (int)f7);
                    } else {
                        *(char *)(bloke + 0x60) = cv + 1;
                    }
                } else if (f28 != 0) {
                    *(char *)(bloke + 0x60) = cv + 1;
                }
                break;
            case 7:
                if (f2c != 0) {
                    *(int *)(bloke + 0x24) = x * 0x100 + -0x79c;
                    y = y * 0x100 + -0xc9c;
                    *(int *)(bloke + 0x28) = y;
                    cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), *(unsigned int *)(bloke + 0x24), *(int *)(bloke + 0x28), bloke + 0x98);
                    *(short *)(bloke + 0xe) = 7;
                    *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                    NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                    *(char *)(bloke + 0x60) += 1;
                }
                break;
            case 8:
                *(int *)(bloke + 0x24) = (x + -10) * 0x100;
                y = y * 0x100 + -0xc9c;
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), *(unsigned int *)(bloke + 0x24), y, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 9:
                if (f10 == 0) {
                    f2c = 0;
                }
                *(char *)(bloke + 0x60) = cv + 1;
                f10 = f10 - 1;
                break;
            case 10:
                t = *(int *)(bloke + 0x5c);
                y = t + -1;
                *(int *)(bloke + 0x5c) = y;
                if (t < 0) {
                    *(char *)(bloke + 0x60) = cv + 1;
                } else if (y == 0xfa) {
                    BuyItem(param_1, pos, 1);
                }
                break;
            case 11:
                if (f18 < 2 && f11 < 3) {
                    *(int *)(bloke + 0x68) = x * 0x100 + -0xa9c;
                    *(char *)(bloke + 0x60) = cv + 1;
                    *(unsigned int *)(bloke + 0x6c) = y * 0x100 + -0xd2c + (unsigned int)f11 * 100;
                    f11 = f11 + 1;
                }
                break;
            case 12:
                if (f11 == 0 || f18 != 0) {
                    if (f18 != 2) {
                        if (f28 != 0) {
                            *(char *)(bloke + 0x60) = cv + 1;
                        }
                    } else if (f28 == 0) {
                        FUN_0042fa90((int)node, 2, (int)f7);
                    } else {
                        *(char *)(bloke + 0x60) = cv + 1;
                    }
                } else if (fc == 0 && 100 < f1c++) {
                    f18 = 2;
                    f7 = 0;
                    f38 = 0x143;
                    f3c = 0;
                    f2c = 0;
                    f28 = 0;
                    FUN_0042fa90((int)node, 2, (int)f7);
                }
                break;
            case 13:
                if (f30 != 0) {
                    *(int *)(bloke + 0x24) = (x + -2) * 0x100;
                    y = y * 0x100 + -0x46a;
                    *(int *)(bloke + 0x28) = y;
                    cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), *(unsigned int *)(bloke + 0x24), *(int *)(bloke + 0x28), bloke + 0x98);
                    *(short *)(bloke + 0xe) = 7;
                    *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                    NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                    *(char *)(bloke + 0x60) += 1;
                }
                break;
            case 14:
                *(int *)(bloke + 0x24) = (x + -3) * 0x100;
                y = y * 0x100 + -0x46a;
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), *(unsigned int *)(bloke + 0x24), y, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                f11 = f11 - 1;
                *(unsigned char *)(bloke + 0x7f) = *(unsigned char *)(bloke + 0x44);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 15:
                RemoveBlokeFromRide((void *)ride, node);
                *(unsigned short *)(bloke + 0x62) &= 0xfff7;
            }
        }
        st->field_8 = f8;
        st->field_7 = f7;
        st->field_11 = f11;
        st->field_c = fc;
        st->field_1c = f1c;
        st->field_10 = f10;
        st->field_14 = f14;
        st->field_28 = f28;
        st->field_18 = f18;
        st->field_20 = f20;
        st->field_38 = f38;
        st->field_24 = f24;
        st->field_2c = f2c;
        st->field_30 = f30;
        st->field_3c = f3c;
        node = next;
    }
    {
        struct SaveBlock *v = DAT_00616148;
        unsigned char counter;
        char vf6;
        int vf18;
        char vf9;
        int vfc;
        char v8;
        char v11;
        int prev_fc = 0;
        while (v != NULL) {
            counter = v->field_9;
            v8 = v->field_8;
            vfc = v->field_c;
            vf18 = v->field_18;
            v11 = v->field_11;
            vf6 = v->field_6;
            f7 = v->field_7;
            vf9 = v->field_9;
            (void)vf9;
            f24 = v->field_24;
            f2c = v->field_2c;
            f1c = v->field_1c;
            f30 = v->field_30;
            f28 = v->field_28;
            f38 = v->field_38;
            f3c = v->field_3c;
            switch (v->field_18) {
            case 0:
                if (vfc != 0) {
                    if (counter < 9) {
                        counter = counter + 1;
                    } else {
                        vf18 = 1;
                        counter = 8;
                        f28 = 1;
                    }
                }
                break;
            case 1:
                if (f24 != 0) {
                    f28 = 0;
                    if ((char)counter < 0) {
                        vf6 = 0;
                        f24 = 0;
                        vf18 = 2;
                        f7 = 0;
                        f38 = 0x143;
                        f3c = 0;
                        FUN_0042fb00(v->value);
                    } else {
                        counter = counter - 1;
                    }
                }
                break;
            case 2:
                if (f7 < 0x21) {
                    f7 = f7 + 1;
                    f38 = f38 - (&DAT_004b685c)[f7];
                    f3c = f3c + DAT_004b68e0[f7];
                } else {
                    f30 = 1;
                    f1c = 0;
                    vf18 = 4;
                    vf6 = 0;
                    FUN_0042fb60(v->value);
                }
                break;
            case 3:
                if (vfc != 0) {
                    if (f7 < 0) {
                        f30 = 0;
                        vf18 = 0;
                        vfc = 0;
                        f7 = 0;
                        counter = 0;
                        vf6 = 0;
                        FUN_0042fb60(v->value);
                    } else {
                        f30 = 0;
                        f7 = f7 - 1;
                    }
                }
                break;
            case 4:
                if (vf6 < 9) {
                    vf6 = vf6 + 1;
                } else {
                    vf18 = 5;
                    f2c = 1;
                    f30 = 1;
                    vf6 = 8;
                }
                break;
            case 5:
                if (v8 == 0 && v11 == 0) {
                    f2c = 0;
                    if (vf6 < 0) {
                        vfc = 1;
                        vf18 = 3;
                        f7 = 0x20;
                        FUN_0042fb00(v->value);
                    } else {
                        vf6 = vf6 - 1;
                    }
                }
            }
            counter = counter + 1;
            if (0x1f < (char)counter) {
                counter = 0;
            }
            v->field_7 = f7;
            v->field_9 = counter;
            v->field_8 = v8;
            v->field_11 = v11;
            v->field_1c = f1c;
            v->field_30 = f30;
            v->field_24 = f24;
            v->field_2c = f2c;
            v->field_6 = vf6;
            v->field_18 = vf18;
            v->field_28 = f28;
            v->field_c = vfc;
            v->field_38 = f38;
            v->field_3c = f3c;
            prev_fc = vfc;
            (void)prev_fc;
            v = v->next;
        }
    }
}

// FUNCTION: LEGOLAND 0x004304a0
void *FUN_004304a0(struct EateryObj *obj, unsigned short a2) {
    struct EateryFX *fx = obj->fx_c;
    struct EateryInner *inner = fx->inner_64;
    DAT_00616120 = (unsigned int)inner;
    DAT_00616124 = fx->field_14;
    DAT_00616128 = fx->field_18;
    DAT_0061612c = a2;
    inner = fx->inner_64;
    inner->flags_10 |= 0x2000;
    return &DAT_00616120;
}

// FUNCTION: LEGOLAND 0x004304e0
void FUN_004304e0(unsigned short *param_1, int param_2, unsigned int param_3) {
    struct SaveBlock *state;
    union {
        __int64 q;
        int i[2];
    } coords;
    union {
        __int64 q;
        int i[2];
    } off;
    struct {
        /* 0x00 */ int field_0;
        /* 0x04 */ int field_4;
        /* 0x08 */ short field_8;
    } cfg;
    char c8;
    char c9;
    char c6;
    char c7;
    int sx;
    int sy;
    cfg.field_8 = *param_1;
    cfg.field_4 = *(int *)(param_2 + 0xc4);
    cfg.field_0 = 0x103;
    state = FUN_0042f9d0(param_1);
    if (state == NULL) {
        return;
    }
    coords.q = GetScreenCoordsForObject((unsigned char *)param_1, (void *)param_2);
    sy = coords.i[1];
    sx = coords.i[0];
    c8 = state->field_8;
    c9 = state->field_9;
    c6 = state->field_6;
    c7 = state->field_7;
    switch (state->field_18) {
    case 0:
    case 1:
        LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 1), c7);
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 1);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616118, 1), off.i[0] + sx, off.i[1] + sy, param_3, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 3), c9);
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 3);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616118, 3), off.i[0] + sx, off.i[1] + sy, param_3, (int *)&cfg);
        if (state->field_18 == 0 && state->field_c == 0) {
            return;
        }
        LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 6), c8);
        return;
    case 2:
        LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 1), c7);
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 1);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        sx = off.i[0] + sx;
        sy = off.i[1] + sy;
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616118, 1), sx, sy, param_3, (int *)&cfg);
        return;
    case 3:
        LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 1), c7);
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 1);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616118, 1), off.i[0] + sx, off.i[1] + sy, param_3, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 3), c9);
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 3);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        sx = off.i[0] + sx;
        sy = off.i[1] + sy;
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616118, 3), sx, sy, param_3, (int *)&cfg);
        return;
    case 4:
    case 5:
        LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 1), c7);
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 1);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616118, 1), off.i[0] + sx, off.i[1] + sy, param_3, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 2), c6);
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 2);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616118, 2), off.i[0] + sx, off.i[1] + sy, param_3, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 0), c6);
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 0);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        sx = off.i[0] + sx;
        sy = off.i[1] + sy;
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616118, 0), sx, sy, param_3, (int *)&cfg);
        return;
    }
}

// FUNCTION: LEGOLAND 0x00430b10
void FUN_00430b10(int param_1, unsigned int param_2, unsigned int param_3, short *param_4, unsigned int param_5, unsigned int param_6) {
    int ride = *(int *)(param_1 + 0xc);
    int *node = *(int **)(ride + 0xcc);
    int *p;
    int i;
    int count;
    struct SaveBlock *state;
    char c9;
    int s18;
    unsigned int f11;
    int f38;
    int f3c;
    int blokes[30];
    union {
        __int64 q;
        int i[2];
    } coords;
    union {
        __int64 q;
        int i[2];
    } off;
    struct {
        /* 0x00 */ int field_0;
        /* 0x04 */ int field_4;
        /* 0x08 */ short field_8;
    } cfg;
    int sx;
    int sy;
    cfg.field_4 = param_1;
    cfg.field_8 = *param_4;
    p = blokes;
    blokes[0] = 0;
    for (i = 0x1d; p = p + 1, i != 0; i = i - 1) {
        *p = 0;
    }
    cfg.field_0 = 0x103;
    count = 0;
    state = FUN_0042f9d0(param_4);
    if (state == NULL) {
        return;
    }
    c9 = state->field_9;
    s18 = state->field_18;
    f11 = state->field_11;
    f38 = state->field_38;
    f3c = state->field_3c;
    FUN_004304e0((unsigned short *)param_4, ride, param_6);
    coords.q = GetScreenCoordsForObject((unsigned char *)param_4, (void *)ride);
    if (node == NULL) {
        return;
    }
    p = blokes;
    do {
        if (*param_4 == (short)node[3]) {
            count = count + 1;
            *p = node[2];
            p = p + 1;
        }
        node = (int *)*node;
    } while (node != NULL);
    if (count == 0) {
        return;
    }
    sx = coords.i[0];
    sy = coords.i[1];
    if (s18 == 0 || s18 == 1) {
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 5);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616118, 5), off.i[0] + sx, off.i[1] + sy, param_6, (int *)&cfg);
        for (i = 0; i < count; i = i + 1) {
            if (*(char *)(blokes[i] + 0x60) == 5) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
        }
        for (i = 0; i < count; i = i + 1) {
            if (*(char *)(blokes[i] + 0x60) == 6) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
        }
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 6);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616118, 6), off.i[0] + sx, off.i[1] + sy, param_6, (int *)&cfg);
        for (i = 0; i < count; i = i + 1) {
            if (*(char *)(blokes[i] + 0x60) == 4) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
        }
        PrintSprite(DAT_0081cd48, off.i[0] + sx, off.i[1] + sy, param_6, (int *)&cfg);
    } else if (s18 == 2) {
        if (f11 != 0) {
            for (i = 0; i < count; i = i + 1) {
                if (*(char *)(blokes[i] + 0x60) == 0xc) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
            }
            PrintSprite(DAT_0081cd84, sx, f3c / 2 + sy, param_6, (int *)&cfg);
        }
        PrintSprite(DAT_0081cd20, sx, sy, param_6, (int *)&cfg);
        for (i = 0; i < count; i = i + 1) {
            if (*(char *)(blokes[i] + 0x60) == 6) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
        }
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 6);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        PrintSprite(DAT_0081cd34, off.i[0] + sx, f38 / 2 + sy, param_6, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 3), c9);
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 3);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        sy = off.i[1] + sy;
        sx = off.i[0] + sx;
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616118, 3), sx, sy, param_6, (int *)&cfg);
    } else if (s18 == 5 || s18 == 4) {
        for (i = 0; i < count; i = i + 1) {
            if (*(char *)(blokes[i] + 0x60) == 7) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
        }
        for (i = 0; i < count; i = i + 1) {
            if (*(char *)(blokes[i] + 0x60) == 8) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
        }
        for (i = 0; i < count; i = i + 1) {
            if (*(char *)(blokes[i] + 0x60) == 9) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
        }
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 0);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        for (i = 0; i < count; i = i + 1) {
            if (*(char *)(blokes[i] + 0x60) == 0xd) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
        }
        for (i = 0; i < count; i = i + 1) {
            if (*(char *)(blokes[i] + 0x60) == 0xe) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
        }
        for (i = 0; i < count; i = i + 1) {
            if (*(char *)(blokes[i] + 0x60) == 0xf) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
        }
        PrintSprite(DAT_0081cd84, sx, off.i[1] + sy, param_6, (int *)&cfg);
        PrintSprite(DAT_0081cd20, sx, sy, param_6, (int *)&cfg);
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 2);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        PrintSprite(DAT_0081cd34, off.i[0] + sx, off.i[1] + sy, param_6, (int *)&cfg);
        LLSSetFrame((struct LLS *)GetLLSForLayer(DAT_00616118, 3), c9);
        off.q = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616118, 3);
        AdjustOffsetForViewMode((struct AdjustStruct *)&off);
        sy = off.i[1] + sy;
        sx = off.i[0] + sx;
        PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616118, 3), sx, sy, param_6, (int *)&cfg);
    }
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x60) == 0) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x60) == 1) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x60) == 2) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x60) == 3) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
}

// FUNCTION: LEGOLAND 0x00431120
void FUN_00431120(void) {
    Kill_FXList(OCTOPUS_SFX, 3);
    KillMoneySFX();
    KillSprite(DAT_0081cd34);
    KillSprite(DAT_0081cd84);
    KillSprite(DAT_0081cd20);
    KillSprite(DAT_0081cd48);
}

// FUNCTION: LEGOLAND 0x00431170
void FUN_00431170(int param_1) {
    int ride = *(int *)(param_1 + 0xc);
    unsigned int *node = *(unsigned int **)(ride + 0xcc);
    unsigned int *next;
    int bloke;
    unsigned char *pos;
    int x;
    int y;
    char cv;
    char state;
    while (node != NULL) {
        next = (unsigned int *)*node;
        bloke = node[2];
        pos = (unsigned char *)(node + 3);
        x = (unsigned int)pos[0] + *(int *)(ride + 0xc);
        y = (unsigned int)pos[1] + *(int *)(ride + 0x10);
        if (*(short *)(bloke + 0xe) == 0) {
            state = *(char *)(bloke + 0x60);
            switch (state) {
            case 0:
                *(unsigned char *)(bloke + 0x62) |= 8;
                *(int *)(bloke + 0x24) = x * 0x100 + 0x80;
                x = y * 0x100 + -0x80;
                goto calc;
            case 1:
                *(char *)(bloke + 0x60) = state + 1;
                *(unsigned int *)(bloke + 0x58) = (rand() & 0x1f) + 4;
                break;
            case 2:
                if (*(int *)(bloke + 0x58) == 0) {
                    *(char *)(bloke + 0x60) = state + 1;
                    BuyItem(param_1, node + 3, 1);
                }
                *(int *)(bloke + 0x58) += -1;
                break;
            case 3:
                *(int *)(bloke + 0x24) = x * 0x100 + 0x80;
                x = y * 0x100 + 0x80;
            calc:
                *(int *)(bloke + 0x28) = x;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c),
                    *(unsigned int *)(bloke + 0x24), x, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 4:
                RemoveBlokeFromRide((void *)ride, node);
                *(unsigned short *)(bloke + 0x62) &= 0xfff7;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x004312c0
void FUN_004312c0(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    StandardRemoveObject(param_1, param_2, param_3);
    RemoveAllBlokesFromRide(*(unsigned int *)(param_1 + 0xc), (void *)param_2);
    StopMoneySFX(&param_2);
}

// FUNCTION: LEGOLAND 0x00431300
void FUN_00431300(struct EateryObj *obj) {
    struct EateryInner *inner;
    DAT_0081cd1c = obj->fx_c;
    DAT_0081cd1c->flags_1c |= 0x20;
    inner = DAT_0081cd1c->inner_64;
    inner->flags_10 |= 0x2000;
    // STRING: LEGOLAND 0x004b7100
    DAT_0081cd60 = LoadSprite("OctTentA.lls", 1);
    // STRING: LEGOLAND 0x004b70f0
    DAT_0081cd64 = LoadSprite("OctTentB.lls", 1);
    // STRING: LEGOLAND 0x004b70e0
    DAT_0081cd68 = LoadSprite("OctTentC.lls", 1);
    // STRING: LEGOLAND 0x004b70d0
    DAT_0081cd6c = LoadSprite("OctTentD.lls", 1);
    // STRING: LEGOLAND 0x004b70c0
    DAT_0081cd70 = LoadSprite("OctTentE.lls", 1);
    // STRING: LEGOLAND 0x004b70b0
    DAT_0081cd74 = LoadSprite("OctTentF.lls", 1);
    // STRING: LEGOLAND 0x004b70a0
    DAT_0081cd78 = LoadSprite("OctTentG.lls", 1);
    // STRING: LEGOLAND 0x004b7090
    DAT_0081cd7c = LoadSprite("OctTentH.lls", 1);
    // STRING: LEGOLAND 0x004b7080
    DAT_0081cd80 = LoadSprite("OctoKiosk.lls", 1);
    // STRING: LEGOLAND 0x004b7070
    DAT_0081cda0 = LoadSprite("OctTabAA.lls", 1);
    // STRING: LEGOLAND 0x004b7060
    DAT_0081cda4 = LoadSprite("OctTabAB.lls", 1);
    // STRING: LEGOLAND 0x004b7050
    DAT_0081cda8 = LoadSprite("OctTabBA.lls", 1);
    // STRING: LEGOLAND 0x004b7040
    DAT_0081cdac = LoadSprite("OctTabBB.lls", 1);
    // STRING: LEGOLAND 0x004b7030
    DAT_0081cdb0 = LoadSprite("OctTabCA.lls", 1);
    // STRING: LEGOLAND 0x004b7020
    DAT_0081cdb4 = LoadSprite("OctTabCB.lls", 1);
    // STRING: LEGOLAND 0x004b7010
    DAT_0081cdb8 = LoadSprite("OctTabDA.lls", 1);
    // STRING: LEGOLAND 0x004b7000
    DAT_0081cdbc = LoadSprite("OctTabDB.lls", 1);
    // STRING: LEGOLAND 0x004b6ff0
    DAT_0081cdc0 = LoadSprite("OctTabEA.lls", 1);
    // STRING: LEGOLAND 0x004b6fe0
    DAT_0081cdc4 = LoadSprite("OctTabEB.lls", 1);
    // STRING: LEGOLAND 0x004b6fd0
    DAT_0081cdc8 = LoadSprite("OctTabFA.lls", 1);
    // STRING: LEGOLAND 0x004b6fc0
    DAT_0081cdcc = LoadSprite("OctTabFB.lls", 1);
    // STRING: LEGOLAND 0x004b6fb0
    DAT_0081cdd0 = LoadSprite("OctTabGA.lls", 1);
    // STRING: LEGOLAND 0x004b6fa0
    DAT_0081cdd4 = LoadSprite("OctTabGB.lls", 1);
    // STRING: LEGOLAND 0x004b6f90
    DAT_0081cdd8 = LoadSprite("OctTabHA.lls", 1);
    // STRING: LEGOLAND 0x004b6f80
    DAT_0081cddc = LoadSprite("OctTabHB.lls", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x004314f0
void FUN_004314f0(unsigned int param_1, struct UserFlagsArg *param_2) {
    AddBasicObject(param_1, (unsigned int)param_2);
    Set_UserFlags(param_2->var_0, param_2->var_4, 0);
}

// FUNCTION: LEGOLAND 0x00431520
void FUN_00431520(void) {
    if (DAT_0081cd60) {
        KillSprite(DAT_0081cd60);
    }
    if (DAT_0081cd64) {
        KillSprite(DAT_0081cd64);
    }
    if (DAT_0081cd68) {
        KillSprite(DAT_0081cd68);
    }
    if (DAT_0081cd6c) {
        KillSprite(DAT_0081cd6c);
    }
    if (DAT_0081cd70) {
        KillSprite(DAT_0081cd70);
    }
    if (DAT_0081cd74) {
        KillSprite(DAT_0081cd74);
    }
    if (DAT_0081cd78) {
        KillSprite(DAT_0081cd78);
    }
    if (DAT_0081cd7c) {
        KillSprite(DAT_0081cd7c);
    }
    if (DAT_0081cd80) {
        KillSprite(DAT_0081cd80);
    }
    if (DAT_0081cda0) {
        KillSprite(DAT_0081cda0);
    }
    if (DAT_0081cda4) {
        KillSprite(DAT_0081cda4);
    }
    if (DAT_0081cda8) {
        KillSprite(DAT_0081cda8);
    }
    if (DAT_0081cdac) {
        KillSprite(DAT_0081cdac);
    }
    if (DAT_0081cdb0) {
        KillSprite(DAT_0081cdb0);
    }
    if (DAT_0081cdb4) {
        KillSprite(DAT_0081cdb4);
    }
    if (DAT_0081cdb8) {
        KillSprite(DAT_0081cdb8);
    }
    if (DAT_0081cdbc) {
        KillSprite(DAT_0081cdbc);
    }
    if (DAT_0081cdc0) {
        KillSprite(DAT_0081cdc0);
    }
    if (DAT_0081cdc4) {
        KillSprite(DAT_0081cdc4);
    }
    if (DAT_0081cdc8) {
        KillSprite(DAT_0081cdc8);
    }
    if (DAT_0081cdcc) {
        KillSprite(DAT_0081cdcc);
    }
    if (DAT_0081cdd0) {
        KillSprite(DAT_0081cdd0);
    }
    if (DAT_0081cdd4) {
        KillSprite(DAT_0081cdd4);
    }
    if (DAT_0081cdd8) {
        KillSprite(DAT_0081cdd8);
    }
    if (DAT_0081cddc) {
        KillSprite(DAT_0081cddc);
    }
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x004316f0
void FUN_004316f0(int param_1) {
    unsigned char *pos;
    int bloke;
    unsigned int *node;
    unsigned int *next;
    char cv;
    char state;
    int t;
    int u;
    int y;
    int f36;
    int ride = *(int *)(param_1 + 0xc);
    node = *(unsigned int **)(ride + 0xcc);
    while (node != NULL) {
        bloke = node[2];
        next = (unsigned int *)*node;
        pos = (unsigned char *)(node + 3);
        if (*(short *)(bloke + 0xe) == 0) {
            state = *(char *)(bloke + 0x60);
            switch (state) {
            case 0:
                cv = (char)Get_UserFlags((unsigned int)pos[0] << 8, (unsigned int)*((unsigned char *)node + 0xd) << 8);
                *(unsigned char *)(bloke + 0x62) |= 8;
                *(char *)(bloke + 0x36) = cv;
                *(short *)(bloke + 0x40) = 0;
                *(int *)(bloke + 0x5c) = 0x32;
                *(char *)(bloke + 0x60) += 1;
                Set_UserFlags((unsigned int)pos[0] << 8, (unsigned int)*((unsigned char *)node + 0xd) << 8, cv + 1 & 0x1f);
                break;
            case 1:
            case 2:
                if (state != 2 || (t = *(int *)(bloke + 0x5c), *(int *)(bloke + 0x5c) = t + -1, t < 0)) {
                    goto buy;
                }
                break;
            case 3:
            case 4:
            buy:
                BuyItem(param_1, pos, 1);
                f36 = *(unsigned char *)(bloke + 0x36);
                t = DAT_004b6a34[(unsigned int)*(unsigned char *)(bloke + 0x60) + (f36 >> 1) * 4];
                if (t != -1) {
                    goto move_to;
                }
                *(unsigned char *)(bloke + 0x60) += 1;
                break;
            case 5:
                t = *(int *)(bloke + 0x5c);
                *(int *)(bloke + 0x5c) = t + -1;
                if (t < 0) {
                    *(char *)(bloke + 0x60) = state + 1;
                }
                break;
            case 6:
                f36 = *(unsigned char *)(bloke + 0x36);
                u = DAT_004b6a44[(f36 >> 1) * 4];
                *(int *)(bloke + 0x68) = *(int *)(bloke + 0x24);
                *(int *)(bloke + 0x6c) = *(int *)(bloke + 0x28);
                t = DAT_004b6b38[DAT_004b6be8[f36] * 4 + 1];
                *(unsigned int *)(bloke + 0x24) = DAT_004b6990[u * 2] + (unsigned int)pos[0] * 0x100 + -0x80 + DAT_004b6b38[DAT_004b6be8[f36] * 4];
                y = (unsigned int)*((unsigned char *)node + 0xd) * 0x100 + DAT_004b6990[u * 2 + 1] + 0x80 + t;
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), *(unsigned int *)(bloke + 0x24), y, bloke + 0x98);
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                goto setdir;
            case 7:
                NewDirForAction(bloke, (*(char *)(bloke + 0x72) + -4) & 7);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 8:
                *(int *)(bloke + 0x68) = *(int *)(bloke + 0x24);
                *(int *)(bloke + 0x6c) = *(int *)(bloke + 0x28);
                *(short *)(bloke + 0x40) = 1;
                f36 = *(unsigned char *)(bloke + 0x36);
                t = DAT_004b6b38[DAT_004b6be8[f36] * 4 + 3];
                u = DAT_004b6a44[(f36 >> 1) * 4];
                *(unsigned int *)(bloke + 0x24) = DAT_004b6990[u * 2] + (unsigned int)pos[0] * 0x100 + -0x80 + DAT_004b6b38[DAT_004b6be8[f36] * 4 + 2];
                y = (unsigned int)*((unsigned char *)node + 0xd) * 0x100 + DAT_004b6990[u * 2 + 1] + 0x80 + t;
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), *(unsigned int *)(bloke + 0x24), y, bloke + 0x98);
                *(char *)(bloke + 0x73) = cv + 0x10;
                *(short *)(bloke + 0xe) = 7;
                *(char *)(bloke + 0x60) += 1;
                *(int *)(bloke + 0x5c) = 200;
                break;
            case 9:
                *(unsigned char *)(bloke + 0x63) |= 1;
                *(int *)(bloke + 0x68) = *(int *)(bloke + 0x24);
                *(int *)(bloke + 0x6c) = *(int *)(bloke + 0x28);
                *(short *)(bloke + 0x70) = 10;
                BlokeSitAnim(bloke);
                BlokeSetFrame(bloke, 0);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 10:
                t = *(int *)(bloke + 0x5c);
                *(int *)(bloke + 0x5c) = t + -1;
                if (t < 0) {
                    *(char *)(bloke + 0x60) = state + 1;
                }
                break;
            case 11:
                *(unsigned short *)(bloke + 0x62) &= 0xfeff;
                *(short *)(bloke + 0x70) = 0;
                BlokeWalkAnim((struct Bloke *)bloke);
                f36 = *(unsigned char *)(bloke + 0x36);
                t = DAT_004b6b38[DAT_004b6be8[f36] * 4 + 1];
                u = DAT_004b6a44[(f36 >> 1) * 4];
                *(unsigned int *)(bloke + 0x24) = DAT_004b6990[u * 2] + (unsigned int)pos[0] * 0x100 + -0x80 + DAT_004b6b38[DAT_004b6be8[f36] * 4];
                y = (unsigned int)*((unsigned char *)node + 0xd) * 0x100 + DAT_004b6990[u * 2 + 1] + 0x80 + t;
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), *(unsigned int *)(bloke + 0x24), y, bloke + 0x98);
                *(char *)(bloke + 0x73) = cv + 0x10;
                *(short *)(bloke + 0xe) = 7;
                *(char *)(bloke + 0x60) += 1;
                break;
            case 12:
                *(short *)(bloke + 0x40) = 0;
                f36 = *(unsigned char *)(bloke + 0x36);
                t = DAT_004b6a44[(f36 >> 1) * 4];
            move_to:
                *(unsigned int *)(bloke + 0x24) = DAT_004b6990[t * 2] + -0x80 + (unsigned int)pos[0] * 0x100;
                y = (unsigned int)*((unsigned char *)node + 0xd) * 0x100 + 0x80 + DAT_004b6990[t * 2 + 1];
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), *(unsigned int *)(bloke + 0x24), y, bloke + 0x98);
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
            setdir:
                *(short *)(bloke + 0xe) = 7;
                NewDirForAction(bloke, ((unsigned char)(*(unsigned char *)(bloke + 0x73)) >> 5) + 3);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 13:
            case 14:
            case 15:
            case 16:
                do {
                    t = DAT_004b6a78[(*(unsigned char *)(bloke + 0x36) >> 1) * 4 - (unsigned int)*(unsigned char *)(bloke + 0x60)];
                    *(unsigned char *)(bloke + 0x60) += 1;
                } while (t == -1);
                *(unsigned int *)(bloke + 0x24) = DAT_004b6990[t * 2] + -0x80 + (unsigned int)pos[0] * 0x100;
                y = (unsigned int)*((unsigned char *)node + 0xd) * 0x100 + 0x80 + DAT_004b6990[t * 2 + 1];
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), *(unsigned int *)(bloke + 0x24), y, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                break;
            case 17:
                BlokeWalkAnim((struct Bloke *)bloke);
                *(unsigned int *)(bloke + 0x24) = (*(int *)(ride + 0xc) + (unsigned int)pos[0]) * 0x100 + 0x80;
                y = ((unsigned int)*((unsigned char *)node + 0xd) + *(int *)(ride + 0x10)) * 0x100 + 0x80;
                *(int *)(bloke + 0x28) = y;
                cv = CalcMoveLine(*(unsigned int *)(bloke + 0x68), *(unsigned int *)(bloke + 0x6c), *(unsigned int *)(bloke + 0x24), y, bloke + 0x98);
                *(short *)(bloke + 0xe) = 7;
                *(unsigned char *)(bloke + 0x73) = cv + 0x10;
                NewDirForAction(bloke, ((unsigned char)(cv + 0x10) >> 5) + 3);
                *(char *)(bloke + 0x60) += 1;
                break;
            case 18:
                RemoveBlokeFromRide((void *)ride, node);
                *(unsigned short *)(bloke + 0x62) &= 0xfff7;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x00431c50
void FUN_00431c50(unsigned int param_1, unsigned int param_2, struct Sprite *param_3, struct Sprite *param_4, struct Sprite *param_5, int param_6, int param_7, int param_8, int param_9, unsigned int param_10) {
    PrintSprite(param_3, param_1, param_2, param_10, 0);
    if (param_6 == 0) {
        if (param_7 != 0) {
            goto render7;
        }
    } else {
        IP_RenderBlokeIn3DNow((struct Bloke *)param_6);
        if (param_7 != 0) {
        render7:
            IP_RenderBlokeIn3DNow((struct Bloke *)param_7);
            goto print4;
        }
    print4:
        PrintSprite(param_4, param_1, param_2, param_10, 0);
    }
    if (param_8 == 0) {
        if (param_9 == 0) {
            return;
        }
    } else {
        IP_RenderBlokeIn3DNow((struct Bloke *)param_8);
        if (param_9 == 0) {
            goto print5;
        }
    }
    IP_RenderBlokeIn3DNow((struct Bloke *)param_9);
print5:
    PrintSprite(param_5, param_1, param_2, param_10, 0);
}

// FUNCTION: LEGOLAND 0x00431d00
void FUN_00431d00(int param_1, unsigned int param_2, unsigned int param_3, unsigned char *param_4, unsigned int param_5, unsigned int param_6) {
    int buckets[0x20];
    int blokes[0x20];
    int count;
    int i;
    int *p;
    int bloke;
    int *node;
    p = buckets;
    buckets[0] = 0;
    for (i = 0x1f; p = p + 1, i != 0; i = i - 1) {
        *p = 0;
    }
    count = 0;
    node = *(int **)(*(int *)(param_1 + 0xc) + 0xcc);
    if (node != NULL) {
        int *dst = blokes;
        do {
            if (*(short *)param_4 == (short)node[3]) {
                bloke = node[2];
                if (*(short *)(bloke + 0x40) == 0) {
                    int gx = *(int *)(bloke + 0x68);
                    unsigned char b0 = *param_4;
                    *dst = bloke;
                    count = count + 1;
                    dst = dst + 1;
                    *(unsigned char *)(bloke + 0x37) =
                        DAT_004b6d58[((((gx >> 8) - (unsigned int)b0) * 0xb - (*(int *)(bloke + 0x6c) >> 8)) +
                                         (unsigned int)param_4[1]) *
                            4];
                } else {
                    buckets[*(unsigned char *)(bloke + 0x36)] = bloke;
                }
            }
            node = (int *)*node;
        } while (node != NULL);
    }
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 1) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 2) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    FUN_00431c50(param_2, param_3, DAT_0081cd70, DAT_0081cdc0, DAT_0081cdc4, buckets[14], buckets[15], buckets[12], buckets[13], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 3) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 4) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    FUN_00431c50(param_2, param_3, DAT_0081cd74, DAT_0081cdc8, DAT_0081cdcc, buckets[17], buckets[18], buckets[16], buckets[19], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 5) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 6) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    FUN_00431c50(param_2, param_3, DAT_0081cd6c, DAT_0081cdb8, DAT_0081cdbc, buckets[10], buckets[11], buckets[8], buckets[9], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 7) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    FUN_00431c50(param_2, param_3, DAT_0081cd68, DAT_0081cdb0, DAT_0081cdb4, buckets[4], buckets[7], buckets[5], buckets[6], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 8) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 9) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 10) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    PrintSprite(DAT_0081cd80, param_2, param_3, param_6, 0);
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 0xb) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 0xc) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    FUN_00431c50(param_2, param_3, DAT_0081cd78, DAT_0081cdd0, DAT_0081cdd4, buckets[21], buckets[22], buckets[20], buckets[23], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 0xd) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 0xe) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    FUN_00431c50(param_2, param_3, DAT_0081cd7c, DAT_0081cdd8, DAT_0081cddc, buckets[24], buckets[25], buckets[26], buckets[27], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 0xf) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 0x10) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    FUN_00431c50(param_2, param_3, DAT_0081cd64, DAT_0081cda8, DAT_0081cdac, buckets[0], buckets[3], buckets[1], buckets[2], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 0x11) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 0x12) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
    FUN_00431c50(param_2, param_3, DAT_0081cd60, DAT_0081cda0, DAT_0081cda4, buckets[28], buckets[29], buckets[30], buckets[31], param_6);
    for (i = 0; i < count; i = i + 1) {
        if (*(char *)(blokes[i] + 0x37) == 0x13) IP_RenderBlokeIn3DNow((struct Bloke *)blokes[i]);
    }
}

// FUNCTION: LEGOLAND 0x004322a0
int FUN_004322a0(void) {
    unsigned int one = 1;
    unsigned int zero = 0;
    struct BrollyNode *node = DAT_00616144;
    while (node != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(node, 0xc) == 0) {
            return 0;
        }
        node = node->next;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00432310
int FUN_00432310(void) {
    unsigned int count;
    struct BrollyNode *prev = NULL;
    struct BrollyNode *node;
    if (!SaveGameRead(&count, 4)) {
        return 0;
    }
    if (count == 0) {
        return 1;
    }
    while (count != 0) {
        node = malloc(sizeof(struct BrollyNode));
        if (!SaveGameRead(node, 0xc)) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            DAT_00616144 = node;
        }
        prev = node;
        if (!SaveGameRead(&count, 4)) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00432390
int FUN_00432390(void) {
    unsigned int one = 1;
    unsigned int zero = 0;
    struct SaveBlock *node = DAT_00616148;
    while (node != NULL) {
        if (SaveGameWrite(&one, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(node, 0x40) == 0) {
            return 0;
        }
        node = node->next;
    }
    return SaveGameWrite(&zero, 4) != 0;
}

// FUNCTION: LEGOLAND 0x00432400
int FUN_00432400(void) {
    unsigned int count;
    struct SaveBlock *current;
    struct SaveBlock *prev = NULL;

    if (!SaveGameRead(&count, 4)) {
        return 0;
    }
    if (count == 0) {
        return 1;
    }
    while (count != 0) {
        current = malloc(64);
        if (!SaveGameRead(current, 64)) {
            return 0;
        }
        current->next = 0;
        if (prev != NULL) {
            prev->next = current;
        } else {
            DAT_00616148 = current;
        }
        prev = current;
        if (!SaveGameRead(&count, 4)) {
            return 0;
        }
    }
    return 1;
}
