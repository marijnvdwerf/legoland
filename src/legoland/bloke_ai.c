#include "bloke_ai.h"
#include <stdlib.h>
#include "bloke.h"
#include "globals.h"
#include "legoland.h"
#include "llidb.h"
#include "man3d.h"
#include "math.h"
#include "objclass.h"
#include "saveload.h"
#include "worker.h"

struct BlokeList {
    unsigned char pad_0[0x2e];
    short field_2e;
    unsigned char pad_30[0xcc - 0x30];
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
    unsigned char pad_10[0x14 - 0x10];
    struct ClassNode *field_14;
    unsigned char pad_18[0x54 - 0x18];
    unsigned int field_54;
    int field_58;
    unsigned char pad_5c[0x60 - 0x5c];
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

struct MapObj {
    unsigned char pad_0[0xc];
    struct ClassOffset *field_c;
};

struct ClassOffset {
    unsigned char pad_0[0xc];
    int field_c;
    int field_10;
};

// FUNCTION: LEGOLAND 0x0044f180
int FUN_0044f180(int *pos, struct ClassOffset *cls) {
    struct MapElement *element;
    int x;
    int y;
    int row;

    x = pos[0] >> 8;
    y = pos[1] >> 8;
    row = y - 1;
    if (x >= 0 && x < lpConfig->width && row >= 0 && row < lpConfig->height &&
        (element = GameMap[row] + x) != 0 && (element->flags & 0x80) != 0 && element->field_0 != 0 &&
        ((struct MapObj *)element->field_0)->field_c == cls && element->field_4 + cls->field_c == x &&
        element->field_5 + cls->field_10 == y) {
        return 1;
    }
    row = y + 1;
    if (x >= 0 && x < lpConfig->width && row >= 0 && row < lpConfig->height &&
        (element = GameMap[row] + x) != 0 && (element->flags & 0x80) != 0 && element->field_0 != 0 &&
        ((struct MapObj *)element->field_0)->field_c == cls && element->field_4 + cls->field_c == x &&
        element->field_5 + cls->field_10 == y) {
        return 1;
    }
    row = x - 1;
    if (row >= 0 && row < lpConfig->width && y >= 0 && y < lpConfig->height &&
        (element = GameMap[y] + row) != 0 && (element->flags & 0x80) != 0 && element->field_0 != 0 &&
        ((struct MapObj *)element->field_0)->field_c == cls && element->field_4 + cls->field_c == x &&
        element->field_5 + cls->field_10 == y) {
        return 1;
    }
    row = x + 1;
    if (row >= 0 && row < lpConfig->width && y >= 0 && y < lpConfig->height &&
        (element = GameMap[y] + row) != 0 && (element->flags & 0x80) != 0 && element->field_0 != 0 &&
        ((struct MapObj *)element->field_0)->field_c == cls && element->field_4 + cls->field_c == x &&
        element->field_5 + cls->field_10 == y) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0044f360
int FUN_0044f360(unsigned int param_1, unsigned char *param_2) {
    struct MapElement *element;
    int x;
    int y;

    x = param_2[0];
    y = param_2[1];
    if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
        element = GameMap[y] + x;
    } else {
        element = 0;
    }
    if ((element->flags & 0x200) != 0) {
        return 0;
    }
    if (DAT_0083298c != 0 && (element->flags & 0x100) != 0) {
        return 0;
    }
    return 1;
}

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
    return count >= list->field_2e;
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
void FUN_00450250(struct BlokeRideState *bloke) {
    int state;

    switch (bloke->field_60) {
    case 0:
        NewDirForAction((struct ActionState *)bloke, 4);
        if (bloke->ride[2] == 1) {
            *((unsigned char *)&bloke->flags + 1) |= 1;
            BlokeSetAnim((struct Bloke *)bloke, 2);
            BlokeSetFrame((struct Bloke *)bloke, 0);
            bloke->field_60++;
            return;
        }
        bloke->field_60 = 2;
        return;
    case 1:
        if (PlayBlokeAnim((struct Bloke *)bloke) != 0) {
            BlokeWalkAnim((struct Bloke *)bloke);
            BlokeSetFrame((struct Bloke *)bloke, 0);
            bloke->flags &= 0xfeff;
            bloke->field_60++;
            return;
        }
        break;
    case 2:
        bloke->field_e = 0xd;
        bloke->field_60++;
        return;
    case 3:
        state = bloke->ride[2];
        switch (state) {
        case 2:
            NewLongTermAction((struct Bloke *)bloke, 0x10);
            return;
        case 3:
            NewLongTermAction((struct Bloke *)bloke, 0x11);
            return;
        }
        NewLongTermAction((struct Bloke *)bloke, 6);
    }
}

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
void FUN_004503a0(struct Bloke *bloke, int *box) {
    int x0;
    int y0;
    int tx;
    int ty;

    x0 = bloke->field_2c;
    tx = bloke->field_68 >> 8;
    ty = bloke->field_6c >> 8;
    y0 = bloke->field_30;
    if (tx > box[2] + x0) {
        if (ty < box[1] + y0) {
            bloke->field_72 = 6;
            return;
        }
        if (ty > box[3] + y0) {
            bloke->field_72 = 0;
            return;
        }
        bloke->field_72 = 7;
        return;
    }
    if (tx < box[0] + x0) {
        if (ty < box[1] + y0) {
            bloke->field_72 = 4;
            return;
        }
        if (ty > box[3] + y0) {
            bloke->field_72 = 2;
            return;
        }
        bloke->field_72 = 3;
        return;
    }
    if (ty < box[1] + y0) {
        bloke->field_72 = 5;
        return;
    }
    bloke->field_72 = 1;
}

// FUNCTION: LEGOLAND 0x00450450
void FUN_00450450(struct BlokeRideState *bloke) {
    int num;
    int counter;

    switch (bloke->field_60) {
    case 0:
        FUN_004503a0((struct Bloke *)bloke, (int *)((char *)bloke->field_14->iface + 0x3c));
        bloke->field_58 = (rand() & 0x1f) + 10;
        bloke->field_60++;
        break;
    case 1:
        counter = bloke->field_58 - 1;
        bloke->field_58 = counter;
        if (counter < 0) {
            bloke->field_60 = 2;
            return;
        }
        break;
    case 2:
        num = GetBlokeNum((struct Bloke *)bloke);
        IncrementBlokeCounter((struct ObjectClass *)bloke->field_14->iface, num);
        NewLongTermAction((struct Bloke *)bloke, 6);
        return;
    }
}

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
int FUN_00450500(int *a, int *b) {
    unsigned int sx;
    unsigned int sy;

    sx = (unsigned int)(a[0] - b[0]) >> 0x1f;
    sy = (unsigned int)(a[1] - b[1]) >> 0x1f;
    return (((a[1] - b[1]) ^ sy) - sy) + (((a[0] - b[0]) ^ sx) - sx) == 1;
}

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
void FUN_00450a80(void) {
    int *entry;
    int count;
    int record[3];
    int handle;

    count = 0;
    entry = (int *)&DAT_006664f8;
    do {
        if (*entry != 0) {
            count++;
        }
        entry += 3;
    } while ((int)entry < (int)&DAT_006670f8);
    SaveGameWrite(&count, 4);
    entry = (int *)&DAT_006664f8;
    do {
        if (*entry != 0) {
            record[0] = entry[0];
            record[1] = entry[1];
            record[2] = entry[2];
            handle = *(int *)(*entry + 0xc4);
            FindeIneList(&handle);
            record[0] = handle;
            SaveGameWrite(record, 0xc);
        }
        entry += 3;
    } while ((int)entry < (int)&DAT_006670f8);
}

// FUNCTION: LEGOLAND 0x00450b10
void FUN_00450b10(void) { STUB(); }
