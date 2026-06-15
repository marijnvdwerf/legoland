#include "bloke_ai.h"
#include <stdlib.h>
#include "bloke.h"
#include "globals.h"
#include "legoland.h"
#include "llidb.h"
#include "man3d.h"
#include "math.h"
#include "worker.h"

struct BlokeList {
    unsigned char pad_0[0xcc];
    struct Bloke *head;
};

struct AttractionElement {
    unsigned char pad_0[8];
    unsigned int field_8;
    struct AttractionInfo *field_c;
};

struct AttractionInfo {
    unsigned char pad_0[0x20];
    short field_20;
};

struct BlokeRideState {
    unsigned char pad_0[0x4];
    int *ride;
    unsigned char pad_8[0xe - 0x8];
    unsigned short field_e;
    unsigned char pad_10[0x54 - 0x10];
    unsigned int field_54;
    unsigned char pad_58[0x60 - 0x58];
    unsigned char field_60;
    unsigned char pad_61[0x1];
    unsigned short flags;
    unsigned char field_64;
    unsigned char pad_65[0x73 - 0x65];
    unsigned char field_73;
};

// FUNCTION: LEGOLAND 0x0044e760
LEGO_EXPORT void NewLongTermAction(struct Bloke *bloke, unsigned short action) {
    bloke->action = action;
    bloke->param_action = 0;
    bloke->field_10 = 0;
    bloke->field_e = 0;
    bloke->field_1c = 0;
    DoHighLevelAI(bloke);
}

// FUNCTION: LEGOLAND 0x0044e790
unsigned int FUN_0044e790(void) {
    struct AttractionElement *element;
    int count;
    int start;
    int index;
    unsigned int roll;
    int remaining;
    short field;

    count = LLIDB_GetCount();
    index = rand() % count;
    roll = rand();
    remaining = roll & 0x1f;
    start = index;
    if (remaining-- == 0) {
        return (unsigned int)element;
    }
    do {
        while (1) {
            while (1) {
                LLIDB_GetElement(index, (struct Element **)&element);
                if ((element->field_8 & 0x14) == 0x14) {
                    break;
                }
                index++;
                if (index >= count) {
                    index = 0;
                }
                if (index == start) {
                    return 0;
                }
            }
            field = element->field_c->field_20;
            if (field == 0 || field == 5) {
                break;
            }
            if (remaining-- == 0) {
                return (unsigned int)element;
            }
        }
        index++;
        if (index >= count) {
            index = 0;
        }
    } while (index != start);
    return 0;
}

// FUNCTION: LEGOLAND 0x0044e830
LEGO_EXPORT int IsFavouriteAttraction(struct Bloke *bloke, unsigned int attraction) {
    if (bloke->favourite_attraction_0 == attraction) {
        return 1;
    }
    if (bloke->favourite_attraction_1 == attraction) {
        return 1;
    }
    return bloke->favourite_attraction_2 == attraction;
}

// FUNCTION: LEGOLAND 0x0044e870
LEGO_EXPORT unsigned char IsFavouriteFood(struct Bloke *bloke, unsigned int food) {
    return bloke->favourite_food == food;
}

// FUNCTION: LEGOLAND 0x0044e890
unsigned int FUN_0044e890(void) {
    struct AttractionElement *element;
    int count;
    int start;
    int index;
    unsigned int roll;
    int remaining;

    count = LLIDB_GetCount();
    index = rand() % count;
    roll = rand();
    remaining = roll & 0x1f;
    start = index;
    if (remaining-- == 0) {
        return (unsigned int)element;
    }
    do {
        while (1) {
            while (1) {
                LLIDB_GetElement(index, (struct Element **)&element);
                if ((element->field_8 & 0x14) == 0x14) {
                    break;
                }
                index++;
                if (index >= count) {
                    index = 0;
                }
                if (index == start) {
                    return 0;
                }
            }
            if (element->field_c->field_20 != 5) {
                break;
            }
            if (remaining-- == 0) {
                return (unsigned int)element;
            }
        }
        index++;
        if (index >= count) {
            index = 0;
        }
    } while (index != start);
    return 0;
}

// FUNCTION: LEGOLAND 0x0044e920
LEGO_EXPORT void InitBlokeAI(struct Bloke *bloke) {
    if (DAT_006661c0 == 0) {
        DAT_006661c0 = ElemID("SHARK CAFE BROLLY");
    }
    if (DAT_006661c4 == 0) {
        DAT_006661c4 = ElemID("ENTRANCE 1");
    }
    DAT_006661bc++;
    bloke->field_7f = (unsigned char)Rand_Tween(12, 24);
    bloke->field_78 = (unsigned short)Rand_Max(DAT_00832918);
    bloke->field_7a = (short)Rand_Tween(10, 50);
    bloke->field_7e = (unsigned char)Rand_Tween(0, 140) - 0x14;
    bloke->field_80 = (unsigned char)Rand_Tween(5, 10);
    bloke->field_7c = (unsigned short)Rand_Tween(0, DAT_004b8338);
    bloke->field_14 = 0;
    bloke->field_18 = 0;
    bloke->field_81 = (unsigned char)DAT_004b8344;
    DAT_004b8344++;
    if (DAT_004b8344 > 'Z') {
        DAT_004b8344 = 'A';
    }
    FUN_00482c60((struct Person *)bloke);
    bloke->favourite_attraction_0 = FUN_0044e790();
    bloke->favourite_attraction_1 = FUN_0044e790();
    bloke->favourite_attraction_2 = FUN_0044e790();
    bloke->favourite_food = FUN_0044e890();
    NewLongTermAction(bloke, 2);
}

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
    bloke = MakeBloke();
    if (bloke != 0) {
        DAT_006661c8 = 0;
        bloke->field_72 = rand() & 0x7;
        bloke->field_74 = rand() & 0x7;
        bloke->field_75 = 1;
        InitBlokeAI(bloke);
    }
}

// FUNCTION: LEGOLAND 0x0044eab0
int FUN_0044eab0(struct Bloke *bloke) {
    return (int)(short)bloke->field_78 - (int)bloke->field_7a / 2 > DAT_00832918 * 2;
}

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
LEGO_EXPORT void PushLongTermAction(struct Bloke *bloke) {
    unsigned short action = bloke->action;
    unsigned char param = bloke->param_action;
    bloke->prev_param = action;
    bloke->prev_action = param;
}

// FUNCTION: LEGOLAND 0x0044ebd0
LEGO_EXPORT void PopLongTermAction(struct Bloke *bloke) {
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
int FUN_0044f360() { STUB(); }

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
LEGO_EXPORT void PutBlokeInList(struct BlokeList *list, struct Bloke *bloke) {
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
LEGO_EXPORT void RemoveBlokeFromList(struct BlokeList *list, struct Bloke *bloke) {
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
void FUN_00450330(struct BlokeRideState *bloke) {
    int state;

    switch (bloke->field_60) {
    case 0:
        NewDirForAction((struct ActionState *)bloke, 4);
        bloke->field_60++;
        break;
    case 1:
        bloke->field_e = 0xd;
        bloke->field_60 = 2;
        return;
    case 2:
        state = bloke->ride[2];
        switch (state) {
        case 2:
            NewLongTermAction((struct Bloke *)bloke, 0x10);
            return;
        case 3:
            NewLongTermAction((struct Bloke *)bloke, 0x11);
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x004503a0
void FUN_004503a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00450450
void FUN_00450450(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004504d0
LEGO_EXPORT void DoHighLevelAI(struct Bloke *bloke) {
    void (*fn)(struct Bloke *);

    fn = (void (*)(struct Bloke *))PTR_Bloke_DoNothing_004b8368[bloke->action];
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
LEGO_EXPORT void ControlPeople(void) { STUB(); }

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
