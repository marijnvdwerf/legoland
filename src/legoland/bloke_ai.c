#include "legoland.h"
#include "bloke_ai.h"

struct Bloke {
    struct Bloke *next;
    struct Bloke *prev;
    unsigned char prev_action;
    unsigned char pad_9[0x1];
    unsigned short prev_param;
    unsigned short action;
    unsigned short field_e;
    unsigned short field_10;
    unsigned char pad_12[0x1c - 0x12];
    unsigned int field_1c;
    unsigned char pad_20[0x2e - 0x20];
    short field_2e;
    unsigned char pad_30[0x54 - 0x30];
    unsigned int field_54;
    unsigned char pad_58[0x60 - 0x58];
    unsigned char param_action;
    unsigned char pad_61[0x1];
    unsigned short flags;
    unsigned char field_64;
    unsigned char pad_65[0x72 - 0x65];
    unsigned char field_72;
    unsigned char pad_73[0x1];
    unsigned char field_74;
    unsigned char field_75;
    unsigned char pad_76[0x78 - 0x76];
    unsigned short field_78;
    unsigned char pad_7a[0x7c - 0x7a];
    unsigned short field_7c;
    unsigned char pad_7e[0x80 - 0x7e];
    unsigned char field_80;
    unsigned char pad_81[0x88 - 0x81];
    unsigned int favourite_attraction_0;
    unsigned int favourite_attraction_1;
    unsigned int favourite_attraction_2;
    unsigned int favourite_food;
};

struct BlokeList {
    unsigned char pad_0[0xcc];
    struct Bloke *head;
};

struct BlokeRideState {
    unsigned char pad_0[0x4];
    int *ride;
    unsigned char pad_8[0x54 - 0x8];
    unsigned int field_54;
    unsigned char pad_58[0x62 - 0x58];
    unsigned short flags;
};

extern struct Bloke *MakeBloke(int param_1);
extern unsigned int FUN_0049e4b2(void);

extern unsigned int DAT_0083291c;
extern int DAT_006661c8;
extern int DAT_006661bc;
extern unsigned int DAT_00832990;
extern int DAT_004b8334;
extern int DAT_004b8340;
extern int DAT_004b8344;
extern unsigned int DAT_008119a4;
extern void (*PTR_Bloke_DoNothing_004b8368[16])(struct Bloke *);

// FUNCTION: LEGOLAND 0x0044e760
void NewLongTermAction(struct Bloke *bloke, unsigned short action) {
    bloke->action = action;
    bloke->param_action = 0;
    bloke->field_10 = 0;
    bloke->field_e = 0;
    bloke->field_1c = 0;
    DoHighLevelAI(bloke);
}

// FUNCTION: LEGOLAND 0x0044e790
void FUN_0044e790(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044e830
int IsFavouriteAttraction(struct Bloke *bloke, unsigned int attraction) {
    if (bloke->favourite_attraction_0 == attraction) {
        return 1;
    }
    if (bloke->favourite_attraction_1 == attraction) {
        return 1;
    }
    return bloke->favourite_attraction_2 == attraction;
}

// FUNCTION: LEGOLAND 0x0044e870
unsigned char IsFavouriteFood(struct Bloke *bloke, unsigned int food) {
    return bloke->favourite_food == food;
}

// FUNCTION: LEGOLAND 0x0044e890
void FUN_0044e890(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044e920
void InitBlokeAI(struct Bloke *bloke) { STUB(); }

// FUNCTION: LEGOLAND 0x0044ea40
unsigned int FUN_0044ea40(void) {
    return DAT_0083291c;
}

// FUNCTION: LEGOLAND 0x0044ea50
void FUN_0044ea50(void) {
    struct Bloke *bloke;

    DAT_006661c8++;
    if (DAT_006661c8 < 0x1e) {
        return;
    }
    if (DAT_006661bc >= (int)FUN_0044ea40()) {
        return;
    }
    bloke = MakeBloke(0);
    if (bloke != 0) {
        DAT_006661c8 = 0;
        bloke->field_72 = FUN_0049e4b2() & 0x7;
        bloke->field_74 = FUN_0049e4b2() & 0x7;
        bloke->field_75 = 1;
        InitBlokeAI(bloke);
    }
}

// FUNCTION: LEGOLAND 0x0044eab0
int FUN_0044eab0(struct Bloke *bloke) { STUB(); }

// FUNCTION: LEGOLAND 0x0044eae0
void FUN_0044eae0(struct Bloke *bloke) {
    bloke->field_78++;
    if ((bloke->flags & 0x8) != 0) {
        return;
    }
    if (FUN_0044eab0(bloke) != 0) {
        NewLongTermAction(bloke, 3);
    }
}

// FUNCTION: LEGOLAND 0x0044eb10
unsigned char FUN_0044eb10(struct Bloke *bloke) {
    int *entry;
    int index;
    int value;

    if (DAT_00832990 == 0) {
        return 1;
    }
    value = bloke->field_7c;
    index = 1;
    entry = &DAT_004b8334;
    while ((int)entry < (int)&DAT_004b8344) {
        if (value < *entry) {
            return index - 1;
        }
        entry++;
        index++;
    }
    return 4;
}

// FUNCTION: LEGOLAND 0x0044eb50
void FUN_0044eb50(struct Bloke *bloke) {
    unsigned short value;
    int extended;

    if (DAT_00832990 == 0) {
        return;
    }
    value = bloke->field_7c;
    extended = value & 0xffff;
    if (extended >= DAT_004b8340 && (bloke->flags & 0x28) == 0) {
        NewLongTermAction(bloke, 3);
        return;
    }
    if ((DAT_008119a4 & 0x1f) != 0xf) {
        return;
    }
    if (extended > DAT_004b8340) {
        return;
    }
    bloke->field_7c = (unsigned short)(bloke->field_80 + value);
}

// FUNCTION: LEGOLAND 0x0044ebb0
void PushLongTermAction(struct Bloke *bloke) {
    unsigned short action = bloke->action;
    unsigned char param = bloke->param_action;
    bloke->prev_param = action;
    bloke->prev_action = param;
}

// FUNCTION: LEGOLAND 0x0044ebd0
void PopLongTermAction(struct Bloke *bloke) {
    unsigned short action = bloke->prev_param;
    unsigned char param = bloke->prev_action;
    bloke->action = action;
    bloke->param_action = param;
}

// FUNCTION: LEGOLAND 0x0044ebf0
void FUN_0044ebf0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044ed00
void FUN_0044ed00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044ed70
void FUN_0044ed70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044f170
void FUN_0044f170(struct Bloke *bloke) {
    bloke->field_e = 4;
}

// FUNCTION: LEGOLAND 0x0044f180
void FUN_0044f180(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044f360
void FUN_0044f360(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044f3d0
unsigned int FUN_0044f3d0(struct BlokeList *list, unsigned short *value) {
    struct Bloke *node;
    unsigned short target;
    unsigned int count;

    count = 0;
    node = list->head;
    if (node == 0) {
        return 0;
    }
    target = *value;
    do {
        if (node->action == target) {
            count++;
        }
        node = node->next;
    } while (node != 0);
    return count;
}

// FUNCTION: LEGOLAND 0x0044f400
unsigned int FUN_0044f400(struct BlokeList *list, unsigned short *value) {
    int count;

    count = FUN_0044f3d0(list, value);
    return count >= ((struct Bloke *)list)->field_2e;
}

// FUNCTION: LEGOLAND 0x0044f430
void PutBlokeInList(struct BlokeList *list, struct Bloke *bloke) {
    struct Bloke *tail;

    if (list->head == 0) {
        list->head = bloke;
        return;
    }
    tail = list->head;
    while (tail->next != 0) {
        tail = tail->next;
    }
    tail->next = bloke;
    bloke->prev = tail;
}

// FUNCTION: LEGOLAND 0x0044f470
void RemoveBlokeFromList(struct BlokeList *list, struct Bloke *bloke) {
    if (bloke->prev == 0) {
        list->head = bloke->next;
    } else {
        bloke->prev->next = bloke->next;
    }
    if (bloke->next != 0) {
        bloke->next->prev = bloke->prev;
    }
}

// FUNCTION: LEGOLAND 0x0044f4a0
void FUN_0044f4a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044f610
void FUN_0044f610(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044fe10
void FUN_0044fe10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044fe80
void FUN_0044fe80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00450250
void FUN_00450250(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00450330
void FUN_00450330(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004503a0
void FUN_004503a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00450450
void FUN_00450450(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004504d0
void DoHighLevelAI(struct Bloke *bloke) {
    void (*fn)(struct Bloke *);

    fn = PTR_Bloke_DoNothing_004b8368[bloke->action];
    if (fn != 0) {
        fn(bloke);
    }
    bloke->field_75 = 1;
    bloke->field_64 = 0;
}

// FUNCTION: LEGOLAND 0x00450500
void FUN_00450500(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00450530
void FUN_00450530(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00450990
void ControlPeople(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00450a40
void FUN_00450a40(struct BlokeRideState *bloke) {
    int *ride;
    int state;

    if ((bloke->flags & 0x28) != 0) {
        return;
    }
    ride = bloke->ride;
    state = ride[2];
    if (state < 2 || state > 3) {
        bloke->flags |= 0x8;
        NewLongTermAction((struct Bloke *)bloke, 14);
        return;
    }
    bloke->field_54 = DAT_008119a4;
}

// FUNCTION: LEGOLAND 0x00450a80
void FUN_00450a80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00450b10
void FUN_00450b10(void) { STUB(); }
