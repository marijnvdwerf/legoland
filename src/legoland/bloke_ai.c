#include "bloke_ai.h"
#include <stdio.h>
#include <stdlib.h>
#include "bloke.h"
#include "debug_alloc.h"
#include "gamemap.h"
#include "globals.h"
#include "legoland.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "objclass.h"
#include "pathfind.h"
#include "saveload.h"
#include "worker.h"
#include "worker_mouse.h"

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
    if (remaining-- != 0) {
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
    return (unsigned int)element;
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
    if (remaining-- != 0) {
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
    return (unsigned int)element;
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

struct RideOrigin {
    unsigned char pad_0[0x44];
    int field_44;
    int field_48;
};

// FUNCTION: LEGOLAND 0x0044ebf0
void FUN_0044ebf0(struct Bloke *bloke) {
    int obj;
    struct RideOrigin *origin;
    int ty;
    char dir;

    switch (bloke->param_action) {
    case 0:
        obj = (int)GetFirstObjectMatching((struct RenderObjectVtable *)DAT_006661c4);
        origin = (struct RideOrigin *)*(int *)(DAT_006661c4 + 0xc);
        *(unsigned char *)((char *)bloke + 0x62) |= 8;
        bloke->field_24 = (*(unsigned char *)(obj + 4) + 6 + origin->field_44) * 0x100;
        ty = ((*(unsigned char *)(obj + 5) - 5) + origin->field_48) * 0x100;
        bloke->field_28 = ty;
        bloke->field_68 = bloke->field_24 + DAT_004b8318;
        bloke->field_6c = ty + DAT_004b831c;
        dir = CalcMoveLine(bloke->field_68, bloke->field_6c, bloke->field_24, bloke->field_28, bloke->field_98);
        bloke->field_e = 7;
        bloke->field_72 = ((unsigned char)(dir + 0x10) >> 5) + 3;
        bloke->param_action++;
        break;
    case 1:
        bloke->field_14 = DAT_006661c4;
        if (FUN_0044f4a0(bloke, *(int *)(DAT_006661c4 + 0xc), 0) != 0) {
            bloke->param_action++;
            PushLongTermAction(bloke);
            NewLongTermAction(bloke, 5);
            DAT_00668610 |= 0x40;
            return;
        }
        break;
    case 2:
        bloke->flags &= 0xfff7;
        NewLongTermAction(bloke, 6);
        return;
    }
}

// FUNCTION: LEGOLAND 0x0044ed00
void FUN_0044ed00(char *param_1) {
    char **slot;
    int base;
    int i;

    i = 0;
    base = DAT_006664ec;
    slot = PTR_DAT_004b8348;
    do {
        *slot = &DAT_006661cc + ((base + i) & 7) * 100;
        slot++;
        i++;
    } while ((int)slot < (int)PTR_Bloke_DoNothing_004b8368);
    DAT_006664ec = base + 1;
    // STRING: LEGOLAND 0x004b8404
    sprintf(PTR_DAT_004b8348[7], "%c:%s", *(unsigned int *)(DAT_00813b04 + 4), param_1);
    // STRING: LEGOLAND 0x004b83f0
    DBPrintf("[Bloke %c] - %s\n", *(unsigned int *)(DAT_00813b04 + 4), param_1);
}

// FUNCTION: LEGOLAND 0x0044ed70
void FUN_0044ed70(struct Bloke *bloke) {
    int obj;
    int iface;
    char dir;
    int result;
    int tx;
    int ty;
    int out[2];
    char msg[100];

    switch (bloke->param_action) {
    case 0:
        *(unsigned char *)((char *)bloke + 0x62) |= 8;
        bloke->field_82 = 0;
        bloke->param_action = 1;
        /* fallthrough */
    case 1:
        result = SuggestNextMove(&bloke->field_68, &DAT_004b8320, out);
        switch (result) {
        case 1:
            bloke->field_24 = out[0];
            bloke->field_28 = out[1];
            dir = CalcMoveLine(bloke->field_68, bloke->field_6c, out[0], out[1], bloke->field_98);
            bloke->field_e = 6;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            if (bloke->field_64 != 0) {
                bloke->field_e = 4;
                bloke->param_action = 2;
                return;
            }
            bloke->param_action = 0;
            return;
        case 2:
            bloke->field_24 = out[0];
            bloke->field_28 = out[1];
            dir = CalcMoveLine(bloke->field_68, bloke->field_6c, out[0], out[1], bloke->field_98);
            bloke->field_e = 6;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            if (bloke->field_64 != 0) {
                bloke->field_e = 4;
                bloke->param_action = 2;
                return;
            }
            bloke->param_action = 10;
            return;
        case -3:
        case -1:
        case 0:
            bloke->field_e = 4;
            bloke->field_82++;
            bloke->param_action = 2;
            if (bloke->field_82 == 8) {
                bloke->param_action = 5;
                return;
            }
            break;
        case -2:
            bloke->param_action = 5;
            return;
        }
        break;
    case 2:
        *(unsigned char *)((char *)bloke + 0x62) |= 8;
        bloke->param_action = 1;
        return;
    case 5:
        *(unsigned char *)((char *)bloke + 0x62) |= 8;
        // STRING: LEGOLAND 0x004b8434
        sprintf(msg, "Stuck, Routing Point To Point...");
        FUN_0044ed00(msg);
        result = PTPSuggestNextMove(&bloke->field_68, &DAT_004b8320, out);
        if (result == 0) {
            bloke->field_e = 4;
            bloke->param_action = 6;
            return;
        }
        if (result == 1) {
            bloke->field_24 = out[0];
            bloke->field_28 = out[1];
            dir = CalcMoveLine(bloke->field_68, bloke->field_6c, out[0], out[1], bloke->field_98);
            bloke->field_e = 0xb;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            if ((bloke->field_64 & 1) != 0) {
                bloke->param_action = 6;
                return;
            }
        } else if (result == 2) {
            bloke->field_24 = out[0];
            bloke->field_28 = out[1];
            dir = CalcMoveLine(bloke->field_68, bloke->field_6c, out[0], out[1], bloke->field_98);
            bloke->field_e = 0xb;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action = (-((bloke->field_64 & 1) != 0) & 0xfc) + 10;
            return;
        }
        break;
    case 6:
        // STRING: LEGOLAND 0x004b8424
        sprintf(msg, "Wandering...");
        FUN_0044ed00(msg);
        bloke->field_e = 4;
        bloke->param_action = 5;
        return;
    case 10:
        if (FUN_0044f4a0(bloke, *(int *)(bloke->field_14 + 0xc), 0) != 0) {
            *(unsigned char *)((char *)bloke + 0x62) |= 8;
            bloke->field_14 = DAT_006661c4;
            bloke->param_action++;
            PushLongTermAction(bloke);
            NewLongTermAction(bloke, 5);
            return;
        }
        break;
    case 0xb:
        obj = (int)GetFirstObjectMatching((struct RenderObjectVtable *)DAT_006661c4);
        iface = *(int *)(DAT_006661c4 + 0xc);
        bloke->field_24 = (*(unsigned char *)(obj + 4) + 6 + *(int *)(iface + 0x44)) * 0x100;
        ty = (*(unsigned char *)(obj + 5) + 8 + *(int *)(iface + 0x40)) * 0x100;
        bloke->field_28 = ty;
        tx = bloke->field_24;
        dir = CalcMoveLine(bloke->field_68, bloke->field_6c, tx, ty, bloke->field_98);
        bloke->field_e = 7;
        bloke->field_73 = dir + 0x10;
        NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
        bloke->param_action++;
        return;
    case 0xc:
        RateBlokeOnLeaving((int)bloke->field_7a);
        tx = bloke->field_24 + DAT_004b8328;
        bloke->field_24 = tx;
        bloke->field_28 = bloke->field_28 + DAT_004b832c;
        ty = bloke->field_28;
        dir = CalcMoveLine(bloke->field_68, bloke->field_6c, tx, ty, bloke->field_98);
        bloke->field_e = 7;
        bloke->field_73 = dir + 0x10;
        NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
        bloke->param_action++;
        return;
    case 0xd:
        // STRING: LEGOLAND 0x004b840c
        DBPrintf("Killing MiniFig: $%x\n", bloke);
        DestroyBloke(bloke);
        DAT_006661bc--;
    }
}

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
int FUN_0044f4a0(struct Bloke *bloke, int objclass, int param_3) {
    int *node;
    int obj;
    unsigned short uid;
    struct MapElement *element;
    unsigned int x;
    unsigned int y;

    node = (int *)malloc(0x14);
    if (node == 0) {
        // STRING: LEGOLAND 0x004b8458
        DBPrintf("Couldn't allocate BlokeOnRide for %s\n", *(unsigned int *)((char *)bloke + 0x78));
        return 0;
    }
    obj = (int)GetFirstObjectMatching(*(struct RenderObjectVtable **)(objclass + 0xc4));
    if (obj == 0) {
        // STRING: LEGOLAND 0x004b8480
        DBPrintf("Couldn't find instance of %s\n", *(unsigned int *)((char *)bloke + 0x78));
        return 0;
    }
    x = *(unsigned char *)(obj + 4);
    y = *(unsigned char *)(obj + 5);
    FUN_00489f90((const struct ObjClassKey *)&x);
    node[0] = 0;
    node[1] = 0;
    node[2] = 0;
    node[3] = 0;
    node[4] = 0;
    node[2] = (int)bloke;
    bloke->field_58 = param_3;
    node[4] = *(int *)((char *)bloke + 4);
    *(unsigned char *)&bloke->flags |= 0x20;
    bloke->field_e = 0;
    bloke->field_10 = 0;
    bloke->field_35 = 0;
    if (objclass == *(int *)(DAT_006661c4 + 0xc)) {
        obj = (int)GetFirstObjectMatching((struct RenderObjectVtable *)DAT_006661c4);
        *(unsigned char *)(node + 3) = *(unsigned char *)(obj + 4);
        *((unsigned char *)node + 0xd) = *(unsigned char *)(obj + 5);
    } else {
        uid = (unsigned short)GetObjectUID((int *)((char *)bloke + 0x68), objclass);
        *(unsigned short *)(node + 3) = uid;
    }
    x = *(unsigned char *)(node + 3);
    y = *((unsigned char *)node + 0xd);
    if (x < lpConfig->width && y < lpConfig->height) {
        element = GameMap[y] + x;
    } else {
        element = 0;
    }
    *(unsigned char *)&element->flags |= 4;
    PutBlokeInList((struct BlokeList *)objclass, (struct Bloke *)node);
    DAT_00668610 |= 0x20;
    return 1;
}

// FUNCTION: LEGOLAND 0x0044f610
void FUN_0044f610(struct Bloke *bloke) {
    int key;
    int rate;
    int result;
    int iface;
    char dir;
    int num;
    int counter;
    int rnd;
    short sval;
    int kind;
    unsigned char uid[2];
    int out[2];
    char msg[100];
    int *element;

    *(unsigned int *)(DAT_00813b04 + 4) = bloke->field_81;
    switch (bloke->param_action) {
    case 0:
        BuildObjInfoList();
        CalculateRideCodes((unsigned int)bloke);
        ResetBestPtr();
        if (ShuffleObjKeys(&bloke->field_2c, (void **)&key) != 0) {
            result = 1;
            do {
                if (bloke->field_18 == *(unsigned int *)(key + 0xc4)) {
                    // STRING: LEGOLAND 0x004b858c
                    sprintf(msg, "I've just been on the %s.", *(unsigned int *)(key + 0x78));
                    FUN_0044ed00(msg);
                } else {
                    rate = Calc_Item_Attractiveness(key, (unsigned int)bloke, 0);
                    if (10 < rate) {
                        // STRING: LEGOLAND 0x004b8554
                        sprintf(msg, "(%d) I'll go to the %s", Calc_Item_Attractiveness(key, (unsigned int)bloke, 0), *(unsigned int *)(key + 0x78));
                        bloke->field_14 = *(unsigned int *)(key + 0xc4);
                        FUN_0044ed00(msg);
                        bloke->param_action = 1;
                        bloke->field_82 = 0;
                        if (result != 0) {
                            return;
                        }
                        break;
                    }
                    // STRING: LEGOLAND 0x004b856c
                    sprintf(msg, "The %s is not worth going on.");
                    FUN_0044ed00(msg);
                    num = GetBlokeNum(bloke);
                    if (GetBlokeCounter((struct ObjectClass *)key, num) == 0) {
                        num = GetBlokeNum(bloke);
                        IncrementBlokeCounter((struct ObjectClass *)key, num);
                    }
                }
                result = ShuffleObjKeys(&bloke->field_2c, (void **)&key);
            } while (result != 0);
            // STRING: LEGOLAND 0x004b8524
            sprintf(msg, "I've been on everything and I want to go home.");
            FUN_0044ed00(msg);
            NewLongTermAction(bloke, 3);
            return;
        }
        bloke->param_action = 2;
        return;
    case 1:
        result = SuggestNextMove(&bloke->field_68, &bloke->field_2c, out);
        switch (result) {
        case 1:
            bloke->field_24 = out[0];
            bloke->field_28 = out[1];
            dir = CalcMoveLine(bloke->field_68, bloke->field_6c, out[0], out[1], bloke->field_98);
            bloke->field_e = 6;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action = bloke->field_64 == 0;
            return;
        case 2:
            bloke->field_24 = out[0];
            bloke->field_28 = out[1];
            dir = CalcMoveLine(bloke->field_68, bloke->field_6c, out[0], out[1], bloke->field_98);
            bloke->field_e = 6;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action = (-(bloke->field_64 != 0) & 0xf6) + 10;
            return;
        case -3:
        case -1:
        case 0:
            result = FUN_0044f180((int *)&bloke->field_2c, *(int *)(bloke->field_14 + 0xc));
            if (result != 0) {
                iface = *(int *)(bloke->field_14 + 0xc);
                num = (bloke->field_2c >> 8) - *(int *)(iface + 0xc);
                counter = (bloke->field_30 >> 8) - *(int *)(iface + 0x10);
                if (num < 0 || lpConfig->width <= num || counter < 0 || lpConfig->height <= counter) {
                    element = 0;
                } else {
                    element = (int *)(GameMap[counter] + num);
                }
                element = (int *)GetNextObjectMatching((struct RenderObject *)element, (struct RenderObjectVtable *)bloke->field_14);
                if (element == 0 && (element = (int *)GetFirstObjectMatching((struct RenderObjectVtable *)bloke->field_14)) == 0) {
                    // STRING: LEGOLAND 0x004b8424
                    sprintf(msg, "Wandering...");
                    FUN_0044ed00(msg);
                    bloke->field_e = 4;
                    bloke->param_action++;
                    return;
                }
                if (*(unsigned char *)((char *)element + 4) != num || *(unsigned char *)((char *)element + 5) != counter) {
                    bloke->field_2c = (*(int *)(iface + 0xc) + *(unsigned char *)((char *)element + 4)) * 0x100 + 0x80;
                    bloke->field_30 = (*(int *)(iface + 0x10) + *(unsigned char *)((char *)element + 5)) * 0x100 + 0x80;
                    return;
                }
            }
            bloke->field_e = 4;
            return;
        case -2:
            bloke->field_e = 10;
            return;
        }
        break;
    case 2:
    case 3:
        // STRING: LEGOLAND 0x004b8424
        sprintf(msg, "Wandering...");
        FUN_0044ed00(msg);
        bloke->field_e = 4;
        bloke->param_action++;
        return;
    case 4:
        bloke->param_action = 0;
        return;
    case 5:
        // STRING: LEGOLAND 0x004b8434
        sprintf(msg, "Stuck, Routing Point To Point...");
        FUN_0044ed00(msg);
        result = PTPSuggestNextMove(&bloke->field_68, &bloke->field_2c, out);
        if (result == 0) {
            bloke->field_e = 4;
            bloke->param_action = 6;
            return;
        }
        if (result == 1) {
            bloke->field_24 = out[0];
            bloke->field_28 = out[1];
            dir = CalcMoveLine(bloke->field_68, bloke->field_6c, out[0], out[1], bloke->field_98);
            bloke->field_e = 0xb;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            if ((bloke->field_64 & 1) != 0) {
                bloke->param_action = 6;
                return;
            }
        } else if (result == 2) {
            bloke->field_24 = out[0];
            bloke->field_28 = out[1];
            dir = CalcMoveLine(bloke->field_68, bloke->field_6c, out[0], out[1], bloke->field_98);
            bloke->field_e = 0xb;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action = (-((bloke->field_64 & 1) != 0) & 0xfc) + 10;
            return;
        }
        break;
    case 6:
        // STRING: LEGOLAND 0x004b8424
        sprintf(msg, "Wandering...");
        FUN_0044ed00(msg);
        bloke->field_e = 4;
        bloke->param_action = 5;
        return;
    case 10:
        if (FUN_0044f180((int *)&bloke->field_68, *(int *)(bloke->field_14 + 0xc)) != 0) {
            uid[0] = (unsigned char)GetObjectUID((int *)&bloke->field_68, *(int *)(bloke->field_14 + 0xc));
            uid[1] = (unsigned char)(GetObjectUID((int *)&bloke->field_68, *(int *)(bloke->field_14 + 0xc)) >> 8);
            if (FUN_0044f400((struct BlokeList *)*(int *)(bloke->field_14 + 0xc), (unsigned short *)uid) == 0) {
                if (FUN_0044f360(*(int *)(bloke->field_14 + 0xc), uid) != 0) {
                    if (uid[0] < lpConfig->width && uid[1] < lpConfig->height) {
                        element = (int *)(GameMap[uid[1]] + uid[0]);
                    } else {
                        element = 0;
                    }
                    result = (int)GetInstanceOfClass((struct ObjClassNode *)*element, (const unsigned short *)uid);
                    if ((*(unsigned char *)(result + 0xc) & 2) == 0) {
                        // STRING: LEGOLAND 0x004b84bc
                        sprintf(msg, "I'm going on the ride");
                        FUN_0044ed00(msg);
                        NewLongTermAction(bloke, 5);
                        if ((*(unsigned int *)(bloke->field_14 + 0x1c) & 0x100000) == 0) {
                            if (uid[0] < lpConfig->width && uid[1] < lpConfig->height) {
                                element = (int *)(GameMap[uid[1]] + uid[0]);
                            } else {
                                element = 0;
                            }
                            GetInstanceOfClass((struct ObjClassNode *)*element, (const unsigned short *)uid);
                            rnd = rand();
                            if (FUN_0044f4a0(bloke, *(int *)(bloke->field_14 + 0xc), (rnd & 0x1ff) + 200) != 0) {
                                return;
                            }
                        } else {
                            if (FUN_0044f4a0(bloke, *(int *)(bloke->field_14 + 0xc), 0) != 0) {
                                if (FUN_0044f400((struct BlokeList *)*(int *)(bloke->field_14 + 0xc), (unsigned short *)uid) == 0) {
                                    return;
                                }
                                if (uid[0] < lpConfig->width && uid[1] < lpConfig->height) {
                                    element = (int *)(GameMap[uid[1]] + uid[0]);
                                } else {
                                    element = 0;
                                }
                                result = (int)GetInstanceOfClass((struct ObjClassNode *)*element, (const unsigned short *)uid);
                                *(unsigned short *)(result + 0xc) |= 2;
                                return;
                            }
                        }
                        // STRING: LEGOLAND 0x004b84a0
                        sprintf(msg, "I can't get on the ride.");
                        FUN_0044ed00(msg);
                        FUN_00482df0(bloke, 0, *(short *)(*(int *)(bloke->field_14 + 0xc) + 0x3a));
                        num = GetBlokeNum(bloke);
                        if (GetBlokeCounter((struct ObjectClass *)*(int *)(bloke->field_14 + 0xc), num) == 0) {
                            num = GetBlokeNum(bloke);
                            IncrementBlokeCounter((struct ObjectClass *)*(int *)(bloke->field_14 + 0xc), num);
                        }
                        bloke->param_action = 2;
                        return;
                    }
                    // STRING: LEGOLAND 0x004b84d4
                    sprintf(msg, "I can't go on this ride. It's not working");
                    FUN_0044ed00(msg);
                    sval = *(short *)(*(int *)(bloke->field_14 + 0xc) + 0x3a);
                    kind = 1;
                } else {
                    // STRING: LEGOLAND 0x004b8500
                    sprintf(msg, "I can't go on this ride. It is full");
                    FUN_0044ed00(msg);
                    sval = *(short *)(*(int *)(bloke->field_14 + 0xc) + 0x3a);
                    kind = 0;
                }
                FUN_00482df0(bloke, kind, sval);
                num = GetBlokeNum(bloke);
                if (GetBlokeCounter((struct ObjectClass *)*(int *)(bloke->field_14 + 0xc), num) == 0) {
                    num = GetBlokeNum(bloke);
                    IncrementBlokeCounter((struct ObjectClass *)*(int *)(bloke->field_14 + 0xc), num);
                }
            }
        }
        bloke->param_action = 2;
    }
}

// FUNCTION: LEGOLAND 0x0044fe10
void FUN_0044fe10(struct Bloke *bloke) {
    int ty;
    int tx;
    char dir;

    switch (bloke->param_action) {
    case 0:
        tx = DAT_0066b460 << 8;
        ty = DAT_0066b464 << 8;
        bloke->field_24 = tx;
        bloke->field_28 = ty;
        dir = CalcMoveLine(bloke->field_68, bloke->field_6c, tx, ty, bloke->field_98);
        bloke->field_e = 0xf;
        bloke->field_73 = dir + 0x10;
        NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
        bloke->param_action++;
        break;
    case 1:
        NewLongTermAction(bloke, 6);
        return;
    }
}

// FUNCTION: LEGOLAND 0x0044fe80
void FUN_0044fe80(struct Bloke *bloke) {
    int obj;
    struct MapElement *element;
    unsigned short flags;
    int result;
    char dir;
    int out[2];

    switch (bloke->param_action) {
    case 0:
        obj = (int)GetFirstObjectMatching((struct RenderObjectVtable *)DAT_006661c0);
        if (obj != 0) {
            while (result = *(int *)(*(int *)obj + 0xc), (*(unsigned char *)(obj + 0xc) & 1) != 0) {
                obj = (int)GetNextObjectMatching((struct RenderObject *)obj, (struct RenderObjectVtable *)DAT_006661c0);
                if (obj == 0) {
                    NewLongTermAction(bloke, 6);
                    return;
                }
            }
            bloke->field_46 = (short)*(int *)(obj + 4);
            bloke->field_2c = (*(int *)(result + 0xc) + *(unsigned char *)(obj + 4)) * 0x100;
            bloke->field_30 = (*(unsigned char *)(obj + 5) + *(int *)(result + 0x10)) * 0x100;
            bloke->param_action++;
            if (obj != 0) {
                return;
            }
            NewLongTermAction(bloke, 6);
            return;
        }
        break;
    case 1:
        result = SuggestNextMove(&bloke->field_68, &bloke->field_2c, out);
        switch (result) {
        case 1:
            bloke->field_24 = out[0];
            bloke->field_28 = out[1];
            dir = CalcMoveLine(bloke->field_68, bloke->field_6c, out[0], out[1], bloke->field_98);
            bloke->field_e = 6;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            if (bloke->field_64 == 0) {
                bloke->param_action = 1;
                return;
            }
            break;
        case 2:
            bloke->field_24 = out[0];
            bloke->field_28 = out[1];
            dir = CalcMoveLine(bloke->field_68, bloke->field_6c, out[0], out[1], bloke->field_98);
            bloke->field_e = 6;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            if (bloke->field_64 == 0) {
                bloke->param_action = 2;
                return;
            }
            break;
        case -3:
        case -1:
        case 0:
            bloke->field_e = 4;
            return;
        case -2:
            bloke->field_e = 10;
            return;
        default:
            return;
        }
        break;
    case 2:
        if ((unsigned char)bloke->field_46 < lpConfig->width && *((unsigned char *)&bloke->field_46 + 1) < lpConfig->height) {
            element = GameMap[*((unsigned char *)&bloke->field_46 + 1)] + (unsigned char)bloke->field_46;
        } else {
            element = 0;
        }
        if (*(int *)element == DAT_006661c0 && (flags = element->flags, (flags & 0x80) != 0)) {
            if ((flags & 1) == 0) {
                element->flags = flags | 1;
                *(unsigned char *)((char *)bloke + 0x62) |= 8;
                bloke->field_24 = bloke->field_2c - 0x80;
                bloke->field_28 = bloke->field_30;
                dir = CalcMoveLine(bloke->field_68, bloke->field_6c, bloke->field_24, bloke->field_30, bloke->field_98);
                bloke->field_73 = dir + 0x10;
                bloke->field_e = 7;
                NewDirForAction((struct ActionState *)bloke, 7);
                bloke->field_5c = 0;
                bloke->param_action++;
                return;
            }
            bloke->param_action = 0;
            return;
        }
        break;
    case 3:
        if ((unsigned char)bloke->field_46 < lpConfig->width && *((unsigned char *)&bloke->field_46 + 1) < lpConfig->height) {
            element = GameMap[*((unsigned char *)&bloke->field_46 + 1)] + (unsigned char)bloke->field_46;
        } else {
            element = 0;
        }
        if (*(int *)element == DAT_006661c0 && (element->flags & 0x80) != 0) {
            if ((int)bloke->field_5c < 0x12d) {
                return;
            }
            bloke->param_action++;
            return;
        }
        break;
    case 4:
        if ((unsigned char)bloke->field_46 < lpConfig->width && *((unsigned char *)&bloke->field_46 + 1) < lpConfig->height) {
            element = GameMap[*((unsigned char *)&bloke->field_46 + 1)] + (unsigned char)bloke->field_46;
        } else {
            element = 0;
        }
        if (*(int *)element == DAT_006661c0 && (element->flags & 0x80) != 0) {
            element->flags &= 0xfffe;
            bloke->field_24 = bloke->field_2c + 0x80;
            bloke->field_28 = bloke->field_30;
            dir = CalcMoveLine(bloke->field_68, bloke->field_6c, bloke->field_24, bloke->field_30, bloke->field_98);
            bloke->field_e = 7;
            bloke->field_73 = dir + 0x10;
            NewDirForAction((struct ActionState *)bloke, ((unsigned char)(dir + 0x10) >> 5) + 3);
            bloke->param_action++;
            return;
        }
        break;
    case 5:
        bloke->flags &= 0xfff7;
        break;
    default:
        return;
    }
    NewLongTermAction(bloke, 6);
}

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
            bloke->field_60++;
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
void FUN_00450530(struct Bloke *bloke) {
    int *element;
    unsigned char counter;
    char rate;
    int obj;
    int tile_y;
    int tile_x;
    int dist;
    int local_38;
    int local_34;
    int local_30;
    int local_2c;
    int local_28;
    int row;
    int col;
    int center_x;
    int spot[2];
    int origin[2];
    int food[2];

    local_28 = 0;
    local_38 = 0;
    local_2c = 0;
    local_30 = 0;
    local_34 = 0;
    tile_y = bloke->field_6c >> 8;
    tile_x = bloke->field_68 >> 8;
    row = tile_y - 4;
    origin[0] = tile_x;
    center_x = tile_y;
    if (row <= tile_y + 4) {
        do {
            col = tile_x - 4;
            if (col <= tile_x + 4) {
                do {
                    if (col < 0 || lpConfig->width <= col || row < 0 || lpConfig->height <= row ||
                        (element = (int *)(GameMap[row] + col)) == 0) {
                        local_34++;
                        goto next_col;
                    }
                    obj = *element;
                    if (obj == 0) {
                        goto empty;
                    }
                    if ((*(unsigned char *)((char *)element + 0xc) & 0x80) == 0) {
                        if (obj == 0) {
                        empty:
                            dist = abs(tile_x - col) + abs(center_x - row);
                            if (dist != 0) {
                                local_38 += (int)(-0x14 / (__int64)dist);
                            }
                        }
                    } else {
                        obj = *(int *)(obj + 0xc);
                        if ((int)abs(origin[0] - col) <= (int)*(short *)(obj + 0x2a) &&
                            (int)abs(center_x - row) <= (int)*(short *)(obj + 0x2a)) {
                            switch (*(short *)(obj + 0x20)) {
                            case 1:
                                counter = GetBlokeCounter((struct ObjectClass *)obj, GetBlokeNum(bloke));
                                spot[0] = *(unsigned char *)((char *)element + 4) + *(int *)(obj + 0xc);
                                local_28 += *(short *)(obj + 0x36) >> (counter & 0x1f);
                                spot[1] = *(unsigned char *)((char *)element + 5) + *(int *)(obj + 0x10);
                                if (FUN_00450500(spot, origin) != 0 && FUN_00450500(&col, spot) != 0 &&
                                    Calc_Item_Attractiveness(obj, (unsigned int)bloke, 1) > 0x32 && bloke->action == 6) {
                                    tile_y = spot[0];
                                    dist = spot[1];
                                    if (bloke->field_2c >> 8 != spot[0] || bloke->field_30 >> 8 != dist) {
                                        bloke->field_2c = tile_y << 8;
                                        bloke->field_30 = dist << 8;
                                        bloke->param_action = 1;
                                        bloke->field_e = 0;
                                        bloke->field_14 = *(unsigned int *)(obj + 0xc4);
                                    }
                                }
                                break;
                            case 2:
                                local_38 += *(short *)(obj + 0x36);
                                break;
                            case 3:
                                counter = GetBlokeCounter((struct ObjectClass *)obj, GetBlokeNum(bloke));
                                local_30 += *(short *)(obj + 0x36) >> (counter & 0x1f);
                                if ((bloke->flags & 0x20) == 0 && bloke->field_14 != *(unsigned int *)(obj + 0xc4)) {
                                    dist = GetBlokeCounter((struct ObjectClass *)obj, GetBlokeNum(bloke));
                                    if ((float)(rand() % 100) < (float)(int)(0xf / (__int64)(dist + 1)) &&
                                        bloke->field_e != 0xf && FUN_00458930(0.0f) < 2) {
                                        bloke->field_14 = *(unsigned int *)(obj + 0xc4);
                                        bloke->field_2c = *(unsigned char *)((char *)element + 4);
                                        bloke->field_30 = *(unsigned char *)((char *)element + 5);
                                        NewLongTermAction(bloke, 0xf);
                                    }
                                }
                                break;
                            case 4:
                            case 5:
                                counter = GetBlokeCounter((struct ObjectClass *)obj, GetBlokeNum(bloke));
                                food[0] = *(unsigned char *)((char *)element + 4) + *(int *)(obj + 0xc);
                                local_2c += *(short *)(obj + 0x36) >> (counter & 0x1f);
                                food[1] = *(unsigned char *)((char *)element + 5) + *(int *)(obj + 0x10);
                                if (FUN_00450500(origin, food) != 0 && FUN_00450500(&col, food) != 0 &&
                                    Calc_Item_Attractiveness(obj, (unsigned int)bloke, 1) > 0x32 && bloke->action == 6) {
                                    tile_y = food[0];
                                    dist = food[1];
                                    if (bloke->field_2c >> 8 != food[0] || bloke->field_30 >> 8 != dist) {
                                        bloke->field_2c = tile_y << 8;
                                        bloke->field_30 = dist << 8;
                                        bloke->param_action = 1;
                                        bloke->field_e = 0;
                                        bloke->field_14 = *(unsigned int *)(obj + 0xc4);
                                    }
                                }
                            }
                        }
                    }
                next_col:
                    col++;
                    origin[0] = center_x;
                    tile_x = bloke->field_68 >> 8;
                } while (col <= origin[0] + 4);
            }
            row++;
        } while (row <= center_x + 4);
    }
    FUN_00482df0(bloke, 2, local_34);
    FUN_00482df0(bloke, 3, local_38);
    FUN_00482df0(bloke, 4, local_30);
    FUN_00482df0(bloke, 5, local_2c);
    FUN_00482df0(bloke, 6, local_28);
    rate = FUN_0044eb10(bloke);
    if (2 < rate) {
        FUN_00482df0(bloke, 7, rate - 2);
    }
}

// FUNCTION: LEGOLAND 0x00450990
LEGO_EXPORT void ControlPeople(void) {
    struct Bloke *bloke;
    struct Bloke *next;
    int ticks;

    FUN_0044ea50();
    next = (struct Bloke *)FirstBloke;
    while (1) {
        bloke = next;
        if (bloke == 0) {
            return;
        }
        next = bloke->next;
        *(unsigned int *)(DAT_00813b04 + 4) = bloke->field_81;
        if (FUN_004700c0((unsigned int)bloke) != 0) {
            // STRING: LEGOLAND 0x004b85a8
            DBPrintf("Processing your bloke\n");
        }
        FUN_0044eb50(bloke);
        if ((*(unsigned char *)((char *)bloke + 0x62) & 0x20) == 0) {
            break;
        }
        if (bloke->field_e != 0) {
            DoLowLevelAI((struct Worker *)bloke);
        }
    }
    FUN_0044eae0(bloke);
    ticks = bloke->field_5c;
    bloke->field_5c = ticks + 1;
    if ((ticks + 1 & 0xf) == 0) {
        FUN_00482df0(bloke, 8, 1);
        FUN_00450530(bloke);
    }
    if (bloke->field_e == 0) {
        DoHighLevelAI(bloke);
    }
    if (bloke->field_e != 0) {
        DoLowLevelAI((struct Worker *)bloke);
    }
}

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
void FUN_00450b10(void) {
    int *entry;
    int count;
    int i;

    count = 0;
    SaveGameRead(&count, 4);
    i = 0;
    if (0 < count) {
        entry = (int *)&DAT_006664f8;
        do {
            SaveGameRead(entry, 0xc);
            *entry = *(int *)(GeteListPtr(*entry) + 0xc);
            i++;
            entry += 3;
        } while (i < count);
        if (i >= 0x100) {
            return;
        }
    }
    entry = (int *)&DAT_006664f8[i * 3];
    do {
        *entry = 0;
        entry += 3;
    } while ((int)entry < (int)&DAT_006670f8);
}
