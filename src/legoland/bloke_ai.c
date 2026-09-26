#include "bloke_ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include "screens.h"
#include "worker.h"
#include "worker_mouse.h"

// FUNCTION: LEGOLAND 0x0044e760
LEGO_EXPORT void NewLongTermAction(Bloke *bloke, unsigned short action) {
    bloke->action = action;
    bloke->param_action = 0;
    bloke->field_10 = 0;
    bloke->field_e = 0;
    bloke->field_1c = 0;
    DoHighLevelAI(bloke);
}

// FUNCTION: LEGOLAND 0x0044e790
Element *FUN_0044e790(void) {
    Element *element;
    Ride *ride;
    int count;
    int start;
    int index;
    int remaining;

    count = LLIDB_GetCount();
    index = rand() % count;
    remaining = rand() & 0x1f;
    start = index;
    while (remaining-- != 0) {
        for (;;) {
            LLIDB_GetElement(index, &element);
            if ((element->flags & 0x14) != 0x14) {
                index++;
                if (index >= count) {
                    index = 0;
                }
                if (index == start) {
                    return NULL;
                }
                continue;
            }
            ride = element->data;
            if (ride->type != 0 && ride->type != 5) {
                break;
            }
            index++;
            if (index >= count) {
                index = 0;
            }
            if (index == start) {
                return NULL;
            }
        }
    }
    return element;
}

// FUNCTION: LEGOLAND 0x0044e830
LEGO_EXPORT int IsFavouriteAttraction(Bloke *bloke, Element *attraction) {
    if (bloke->favourite_attraction_0 == attraction) {
        return 1;
    }
    if (bloke->favourite_attraction_1 == attraction) {
        return 1;
    }
    return bloke->favourite_attraction_2 == attraction;
}

// FUNCTION: LEGOLAND 0x0044e870
LEGO_EXPORT int IsFavouriteFood(Bloke *bloke, Element *food) {
    return bloke->favourite_food == food;
}

// FUNCTION: LEGOLAND 0x0044e890
Element *FUN_0044e890(void) {
    Element *element;
    Ride *ride;
    int count;
    int start;
    int index;
    int remaining;

    count = LLIDB_GetCount();
    index = rand() % count;
    remaining = rand() & 0x1f;
    start = index;
    while (remaining-- != 0) {
        for (;;) {
            LLIDB_GetElement(index, &element);
            if ((element->flags & 0x14) != 0x14) {
                index++;
                if (index >= count) {
                    index = 0;
                }
                if (index == start) {
                    return NULL;
                }
                continue;
            }
            ride = element->data;
            if (ride->type == 5) {
                break;
            }
            index++;
            if (index >= count) {
                index = 0;
            }
            if (index == start) {
                return NULL;
            }
        }
    }
    return element;
}

// FUNCTION: LEGOLAND 0x0044e920
LEGO_EXPORT void InitBlokeAI(Bloke *bloke) {
    if (DAT_006661c0 == 0) {
        DAT_006661c0 = ElemID("SHARK CAFE BROLLY");
    }
    if (DAT_006661c4 == 0) {
        DAT_006661c4 = ElemID("ENTRANCE 1");
    }
    DAT_006661bc++;
    bloke->field_7f = Rand_Tween(12, 24);
    bloke->field_78 = Rand_Max(MapStats.total_tiles);
    bloke->field_7a = Rand_Tween(10, 50);
    bloke->field_7e = Rand_Tween(0, 140) - 0x14;
    bloke->field_80 = Rand_Tween(5, 10);
    bloke->field_7c = Rand_Tween(0, DAT_004b8334[1]);
    bloke->target = NULL;
    bloke->last_ride = NULL;
    bloke->field_81 = DAT_004b8344;
    DAT_004b8344++;
    if (DAT_004b8344 > 'Z') {
        DAT_004b8344 = 'A';
    }
    FUN_00482c60(bloke);
    bloke->favourite_attraction_0 = FUN_0044e790();
    bloke->favourite_attraction_1 = FUN_0044e790();
    bloke->favourite_attraction_2 = FUN_0044e790();
    bloke->favourite_food = FUN_0044e890();
    NewLongTermAction(bloke, 2);
}

// FUNCTION: LEGOLAND 0x0044ea40
int FUN_0044ea40(void) {
    return MapStats.capacity;
}

// FUNCTION: LEGOLAND 0x0044ea50
void FUN_0044ea50(void) {
    Bloke *bloke;

    if (++DAT_006661c8 >= 0x1e && DAT_006661bc < FUN_0044ea40()) {
        bloke = MakeBloke(0);
        if (bloke != NULL) {
            DAT_006661c8 = 0;
            bloke->field_72 = rand() & 7;
            bloke->field_74 = rand() & 7;
            bloke->field_75 = 1;
            InitBlokeAI(bloke);
        }
    }
}

// FUNCTION: LEGOLAND 0x0044eab0
int FUN_0044eab0(Bloke *bloke) {
    return bloke->field_78 - bloke->field_7a / 2 > MapStats.total_tiles * 2;
}

// FUNCTION: LEGOLAND 0x0044eae0
void FUN_0044eae0(Bloke *bloke) {
    bloke->field_78++;
    if ((bloke->flags & 0x8) != 0) {
        return;
    }
    if (FUN_0044eab0(bloke) != 0) {
        NewLongTermAction(bloke, 3);
    }
}

// FUNCTION: LEGOLAND 0x0044eb10
char FUN_0044eb10(Bloke *bloke) {
    int i;

    if (MapStats.field_190 == 0) {
        return 1;
    }
    for (i = 1; i < 5; i++) {
        if (bloke->field_7c < DAT_004b8334[i - 1]) {
            return i - 1;
        }
    }
    return 4;
}

// FUNCTION: LEGOLAND 0x0044eb50
void FUN_0044eb50(Bloke *bloke) {
    unsigned short value;
    int extended;

    if (MapStats.field_190 == 0) {
        return;
    }
    value = bloke->field_7c;
    extended = value & 0xffff;
    if (extended >= DAT_004b8334[3] && (bloke->flags & 0x28) == 0) {
        NewLongTermAction(bloke, 3);
        return;
    }
    if ((DAT_008119a4 & 0x1f) != 0xf) {
        return;
    }
    if (extended > DAT_004b8334[3]) {
        return;
    }
    bloke->field_7c = bloke->field_80 + value;
}

// FUNCTION: LEGOLAND 0x0044ebb0
LEGO_EXPORT void PushLongTermAction(Bloke *bloke) {
    unsigned short action = bloke->action;
    unsigned char param = bloke->param_action;
    bloke->prev_param = action;
    bloke->prev_action = param;
}

// FUNCTION: LEGOLAND 0x0044ebd0
LEGO_EXPORT void PopLongTermAction(Bloke *bloke) {
    unsigned short action = bloke->prev_param;
    unsigned char param = bloke->prev_action;
    bloke->action = action;
    bloke->param_action = param;
}

// FUNCTION: LEGOLAND 0x0044ebf0
void FUN_0044ebf0(Bloke *bloke) {
    MapElement *object;
    Ride *ride;
    unsigned char dir;

    switch (bloke->param_action) {
    case 0:
        object = GetFirstObjectMatching(DAT_006661c4);
        ride = DAT_006661c4->data;
        bloke->flags |= 8;
        bloke->dest.x = (object->field_4 + ride->footprint.x1 + 6) << 8;
        bloke->dest.y = (object->field_5 + ride->footprint.y1 - 5) << 8;
        bloke->pos.x = bloke->dest.x + DAT_004b8318.x;
        bloke->pos.y = bloke->dest.y + DAT_004b8318.y;
        dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
        bloke->field_e = 7;
        bloke->field_72 = (dir >> 5) + 3;
        bloke->param_action++;
        break;
    case 1:
        bloke->target = DAT_006661c4;
        if (FUN_0044f4a0(bloke, DAT_006661c4->data, 0) != 0) {
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
void FUN_0044ed00(char *msg) {
    int i;

    for (i = 0; i < 8; i++) {
        PTR_DAT_004b8348[i] = DAT_006661cc[(DAT_006664ec + i) & 7];
    }
    DAT_006664ec++;
    // STRING: LEGOLAND 0x004b8404
    sprintf(PTR_DAT_004b8348[7], "%c:%s", DAT_00813b08, msg);
    // STRING: LEGOLAND 0x004b83f0
    DBPrintf("[Bloke %c] - %s\n", DAT_00813b08, msg);
}

// FUNCTION: LEGOLAND 0x0044ed70
void FUN_0044ed70(Bloke *bloke) {
    MapElement *object;
    Ride *ride;
    unsigned char dir;
    Point out;
    char msg[100];

    switch (bloke->param_action) {
    case 0:
        bloke->flags |= 8;
        bloke->field_82 = 0;
        bloke->param_action = 1;
        /* fallthrough */
    case 1:
        switch (SuggestNextMove(&bloke->pos, &DAT_004b8320, &out)) {
        case -2:
            bloke->param_action = 5;
            return;
        case -3:
        case -1:
        case 0:
            bloke->field_e = 4;
            bloke->param_action = 2;
            if (++bloke->field_82 == 8) {
                bloke->param_action = 5;
                return;
            }
            break;
        case 2:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->field_e = 6;
            bloke->field_73 = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            if (bloke->field_64 != 0) {
                bloke->field_e = 4;
                bloke->param_action = 2;
                return;
            }
            bloke->param_action = 10;
            return;
        case 1:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->field_e = 6;
            bloke->field_73 = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            if (bloke->field_64 != 0) {
                bloke->field_e = 4;
                bloke->param_action = 2;
                return;
            }
            bloke->param_action = 0;
            return;
        }
        break;
    case 2:
        bloke->flags |= 8;
        bloke->param_action = 1;
        return;
    case 5:
        bloke->flags |= 8;
        // STRING: LEGOLAND 0x004b8434
        sprintf(msg, "Stuck, Routing Point To Point...");
        FUN_0044ed00(msg);
        switch (PTPSuggestNextMove(&bloke->pos, &DAT_004b8320, &out)) {
        case 2:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->field_e = 0xb;
            bloke->field_73 = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            bloke->param_action = (bloke->field_64 & 1) ? 6 : 10;
            return;
        case 1:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->field_e = 0xb;
            bloke->field_73 = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            if ((bloke->field_64 & 1) != 0) {
                bloke->param_action = 6;
                return;
            }
            break;
        case 0:
            bloke->field_e = 4;
            bloke->param_action = 6;
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
        if (FUN_0044f4a0(bloke, DAT_006661c4->data, 0) != 0) {
            bloke->flags |= 8;
            bloke->target = DAT_006661c4;
            bloke->param_action++;
            PushLongTermAction(bloke);
            NewLongTermAction(bloke, 5);
            return;
        }
        break;
    case 11:
        object = GetFirstObjectMatching(DAT_006661c4);
        ride = DAT_006661c4->data;
        bloke->dest.x = (object->field_4 + ride->footprint.x1 + 6) << 8;
        bloke->dest.y = (object->field_5 + ride->footprint.y0 + 8) << 8;
        dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
        bloke->field_e = 7;
        bloke->field_73 = dir;
        NewDirForAction(bloke, (dir >> 5) + 3);
        bloke->param_action++;
        break;
    case 12:
        RateBlokeOnLeaving(bloke->field_7a);
        bloke->dest.x += DAT_004b8328.x;
        bloke->dest.y += DAT_004b8328.y;
        dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
        bloke->field_e = 7;
        bloke->field_73 = dir;
        NewDirForAction(bloke, (dir >> 5) + 3);
        bloke->param_action++;
        return;
    case 13:
        // STRING: LEGOLAND 0x004b840c
        DBPrintf("Killing MiniFig: $%x\n", bloke);
        DestroyBloke(bloke);
        DAT_006661bc--;
    }
}

// FUNCTION: LEGOLAND 0x0044f170
void FUN_0044f170(Bloke *bloke) {
    bloke->field_e = 4;
}

// FUNCTION: LEGOLAND 0x0044f180
int FUN_0044f180(Point *pos, Ride *ride) {
    MapElement *element;
    Point tile;
    int n;
    Point origin;

    tile = *pos;
    tile.x >>= 8;
    tile.y >>= 8;
    n = tile.y - 1;
    if (tile.x >= 0 && tile.x < lpConfig->width && n >= 0 && n < lpConfig->height) {
        element = &GameMap[n][tile.x];
    } else {
        element = NULL;
    }
    if (element != NULL && (element->flags & 0x80) != 0 && element->field_0 != NULL && element->field_0->data == ride) {
        origin.x = element->field_4 + ride->x;
        origin.y = element->field_5 + ride->y;
        if (origin.x == tile.x && origin.y == tile.y) {
            return 1;
        }
    }
    n = tile.y + 1;
    if (tile.x >= 0 && tile.x < lpConfig->width && n >= 0 && n < lpConfig->height) {
        element = &GameMap[n][tile.x];
    } else {
        element = NULL;
    }
    if (element != NULL && (element->flags & 0x80) != 0 && element->field_0 != NULL && element->field_0->data == ride) {
        origin.x = element->field_4 + ride->x;
        origin.y = element->field_5 + ride->y;
        if (origin.x == tile.x && origin.y == tile.y) {
            return 1;
        }
    }
    n = tile.x - 1;
    if (n >= 0 && n < lpConfig->width && tile.y >= 0 && tile.y < lpConfig->height) {
        element = &GameMap[tile.y][n];
    } else {
        element = NULL;
    }
    if (element != NULL && (element->flags & 0x80) != 0 && element->field_0 != NULL && element->field_0->data == ride) {
        origin.x = element->field_4 + ride->x;
        origin.y = element->field_5 + ride->y;
        if (origin.x == tile.x && origin.y == tile.y) {
            return 1;
        }
    }
    n = tile.x + 1;
    if (n >= 0 && n < lpConfig->width && tile.y >= 0 && tile.y < lpConfig->height) {
        element = &GameMap[tile.y][n];
    } else {
        element = NULL;
    }
    if (element != NULL && (element->flags & 0x80) != 0 && element->field_0 != NULL && element->field_0->data == ride) {
        origin.x = element->field_4 + ride->x;
        origin.y = element->field_5 + ride->y;
        if (origin.x == tile.x && origin.y == tile.y) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0044f360
int FUN_0044f360(Ride *ride, TileId *tile) {
    MapElement *element;
    int x;
    int y;

    x = tile->pos.x;
    y = tile->pos.y;
    if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
        element = &GameMap[y][x];
    } else {
        element = NULL;
    }
    if ((element->flags & 0x200) != 0) {
        return 0;
    }
    if (MapStats.field_18c != 0 && (element->flags & 0x100) != 0) {
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0044f3d0
unsigned int FUN_0044f3d0(Ride *ride, TileId *tile) {
    RideNode *node;
    unsigned short id;
    unsigned int count;

    count = 0;
    node = ride->riders;
    if (node == NULL) {
        return 0;
    }
    id = tile->id;
    do {
        if (node->tile.id == id) {
            count++;
        }
        node = node->next;
    } while (node != NULL);
    return count;
}

// FUNCTION: LEGOLAND 0x0044f400
unsigned int FUN_0044f400(Ride *ride, TileId *tile) {
    int count;

    count = FUN_0044f3d0(ride, tile);
    return count >= ride->seats;
}

// FUNCTION: LEGOLAND 0x0044f430
LEGO_EXPORT void PutBlokeInList(Ride *ride, RideNode *node) {
    RideNode *tail;

    if (ride->riders == NULL) {
        ride->riders = node;
        return;
    }
    tail = ride->riders;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    tail->next = node;
    node->prev = tail;
}

// FUNCTION: LEGOLAND 0x0044f470
LEGO_EXPORT void RemoveBlokeFromList(Ride *ride, RideNode *node) {
    if (node->prev == NULL) {
        ride->riders = node->next;
    } else {
        node->prev->next = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
}

// FUNCTION: LEGOLAND 0x0044f4a0
int FUN_0044f4a0(Bloke *bloke, Ride *ride, int wait) {
    RideNode *node;
    MapElement *object;
    MapElement *element;
    Point at;

    node = malloc(sizeof(RideNode));
    if (node != NULL) {
        object = GetFirstObjectMatching(ride->element);
        if (object != NULL) {
            at.x = object->field_4;
            at.y = object->field_5;
            FUN_00489f90(&at);
            memset(node, 0, sizeof(RideNode));
            node->rider = bloke;
            bloke->field_58 = wait;
            node->person = bloke->person;
            bloke->flags |= 0x20;
            bloke->field_e = 0;
            bloke->field_10 = 0;
            bloke->field_35 = 0;
            if (ride == DAT_006661c4->data) {
                object = GetFirstObjectMatching(DAT_006661c4);
                node->tile.pos.x = object->field_4;
                node->tile.pos.y = object->field_5;
            } else {
                node->tile = GetObjectUID(&bloke->pos, ride);
            }
            at.x = node->tile.pos.x;
            at.y = node->tile.pos.y;
            if (at.x >= 0 && at.x < lpConfig->width && at.y >= 0 && at.y < lpConfig->height) {
                element = &GameMap[at.y][at.x];
            } else {
                element = NULL;
            }
            element->flags |= 4;
            PutBlokeInList(ride, node);
            DAT_00668610 |= 0x20;
            return 1;
        }
        // STRING: LEGOLAND 0x004b8480
        DBPrintf("Couldn't find instance of %s\n", ride->name);
        return 0;
    }
    // STRING: LEGOLAND 0x004b8458
    DBPrintf("Couldn't allocate BlokeOnRide for %s\n", ride->name);
    return 0;
}

// FUNCTION: LEGOLAND 0x0044f610
void FUN_0044f610(Bloke *bloke) {
    Ride *candidate;
    Ride *ride;
    int x;
    int y;
    MapElement *element;
    MapElement *object;
    ObjInstance *instance;
    unsigned char dir;
    int more;
    TileId tile;
    Point out;
    char msg[100];

    DAT_00813b08 = bloke->field_81;
    switch (bloke->param_action) {
    case 0:
        BuildObjInfoList();
        CalculateRideCodes(bloke);
        ResetBestPtr();
        if (ShuffleObjKeys(&bloke->goal, &candidate) != 0) {
            more = 1;
            do {
                if (bloke->last_ride == candidate->element) {
                    // STRING: LEGOLAND 0x004b858c
                    sprintf(msg, "I've just been on the %s.", candidate->name);
                    FUN_0044ed00(msg);
                } else if (Calc_Item_Attractiveness(candidate, bloke, 0) > 10) {
                    // STRING: LEGOLAND 0x004b8554
                    sprintf(msg, "(%d) I'll go to the %s", Calc_Item_Attractiveness(candidate, bloke, 0), candidate->name);
                    bloke->target = candidate->element;
                    FUN_0044ed00(msg);
                    bloke->param_action = 1;
                    bloke->field_82 = 0;
                    if (more != 0) {
                        return;
                    }
                    break;
                } else {
                    // STRING: LEGOLAND 0x004b856c
                    sprintf(msg, "The %s is not worth going on.", candidate->name);
                    FUN_0044ed00(msg);
                    if (GetBlokeCounter(candidate, GetBlokeNum(bloke)) == 0) {
                        IncrementBlokeCounter(candidate, GetBlokeNum(bloke));
                    }
                }
                more = ShuffleObjKeys(&bloke->goal, &candidate);
            } while (more != 0);
            // STRING: LEGOLAND 0x004b8524
            sprintf(msg, "I've been on everything and I want to go home.");
            FUN_0044ed00(msg);
            NewLongTermAction(bloke, 3);
            return;
        }
        bloke->param_action = 2;
        return;
    case 1:
        switch (SuggestNextMove(&bloke->pos, &bloke->goal, &out)) {
        case -2:
            bloke->field_e = 10;
            return;
        case -3:
        case -1:
        case 0:
            if (FUN_0044f180(&bloke->goal, bloke->target->data) != 0) {
                Ride *ride;
                int ox;
                int oy;

                ride = bloke->target->data;
                x = (bloke->goal.x >> 8) - ride->x;
                y = (bloke->goal.y >> 8) - ride->y;
                if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                    element = &GameMap[y][x];
                } else {
                    element = NULL;
                }
                object = GetNextObjectMatching(element, bloke->target);
                if (object == NULL && (object = GetFirstObjectMatching(bloke->target)) == NULL) {
                    // STRING: LEGOLAND 0x004b8424
                    sprintf(msg, "Wandering...");
                    FUN_0044ed00(msg);
                    bloke->field_e = 4;
                    bloke->param_action++;
                    return;
                }
                ox = object->field_4;
                oy = object->field_5;
                if (ox == x && oy == y) {
                    bloke->field_e = 4;
                    return;
                }
                bloke->goal.x = ((ride->x + ox) << 8) + 0x80;
                bloke->goal.y = ((ride->y + oy) << 8) + 0x80;
                return;
            }
            bloke->field_e = 4;
            return;
        case 2:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->field_e = 6;
            bloke->field_73 = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            bloke->param_action = bloke->field_64 != 0 ? 0 : 10;
            return;
        case 1:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->field_e = 6;
            bloke->field_73 = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            bloke->param_action = bloke->field_64 == 0;
            return;
        }
        break;
    case 2:
    case 3:
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
        switch (PTPSuggestNextMove(&bloke->pos, &bloke->goal, &out)) {
        case 2:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->field_e = 0xb;
            bloke->field_73 = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            bloke->param_action = (bloke->field_64 & 1) ? 6 : 10;
            return;
        case 1:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->field_e = 0xb;
            bloke->field_73 = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            if ((bloke->field_64 & 1) != 0) {
                bloke->param_action = 6;
                return;
            }
            break;
        case 0:
            bloke->field_e = 4;
            bloke->param_action = 6;
            return;
        }
        break;
    case 6:
        sprintf(msg, "Wandering...");
        FUN_0044ed00(msg);
        bloke->field_e = 4;
        bloke->param_action = 5;
        return;
    case 10:
        if (FUN_0044f180(&bloke->pos, bloke->target->data) != 0) {
            tile = GetObjectUID(&bloke->pos, bloke->target->data);
            if (FUN_0044f400(bloke->target->data, &tile) != 0) {
                // STRING: LEGOLAND 0x004b8500
                sprintf(msg, "I can't go on this ride. It is full");
                FUN_0044ed00(msg);
                ride = bloke->target->data;
                FUN_00482df0(bloke, 0, ride->field_3a);
                if (GetBlokeCounter(bloke->target->data, GetBlokeNum(bloke)) == 0) {
                    IncrementBlokeCounter(bloke->target->data, GetBlokeNum(bloke));
                }
                bloke->param_action = 2;
                return;
            }
            if (FUN_0044f360(bloke->target->data, &tile) == 0) {
                // STRING: LEGOLAND 0x004b84d4
                sprintf(msg, "I can't go on this ride. It's not working");
                FUN_0044ed00(msg);
                ride = bloke->target->data;
                FUN_00482df0(bloke, 1, ride->field_3a);
                if (GetBlokeCounter(bloke->target->data, GetBlokeNum(bloke)) == 0) {
                    IncrementBlokeCounter(bloke->target->data, GetBlokeNum(bloke));
                }
                bloke->param_action = 2;
                return;
            }
            x = tile.pos.x;
            y = tile.pos.y;
            if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                element = &GameMap[y][x];
            } else {
                element = NULL;
            }
            instance = GetInstanceOfClass(element->field_0->data, &tile);
            if ((instance->flags & 2) == 0) {
                // STRING: LEGOLAND 0x004b84bc
                sprintf(msg, "I'm going on the ride");
                FUN_0044ed00(msg);
                NewLongTermAction(bloke, 5);
                /* reads the flags of the *next* LLIDB element; the ride's own flags were
                   presumably meant */
                if ((bloke->target[1].flags & 0x100000) != 0) {
                    if (FUN_0044f4a0(bloke, bloke->target->data, 0) != 0) {
                        if (FUN_0044f400(bloke->target->data, &tile) == 0) {
                            return;
                        }
                        x = tile.pos.x;
                        y = tile.pos.y;
                        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                            element = &GameMap[y][x];
                        } else {
                            element = NULL;
                        }
                        instance = GetInstanceOfClass(element->field_0->data, &tile);
                        instance->flags |= 2;
                        return;
                    }
                } else {
                    x = tile.pos.x;
                    y = tile.pos.y;
                    if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                        element = &GameMap[y][x];
                    } else {
                        element = NULL;
                    }
                    GetInstanceOfClass(element->field_0->data, &tile);
                    if (FUN_0044f4a0(bloke, bloke->target->data, (rand() & 0x1ff) + 200) != 0) {
                        return;
                    }
                }
            }
            // STRING: LEGOLAND 0x004b84a0
            sprintf(msg, "I can't get on the ride.");
            FUN_0044ed00(msg);
            ride = bloke->target->data;
            FUN_00482df0(bloke, 0, ride->field_3a);
            if (GetBlokeCounter(bloke->target->data, GetBlokeNum(bloke)) == 0) {
                IncrementBlokeCounter(bloke->target->data, GetBlokeNum(bloke));
            }
            bloke->param_action = 2;
            return;
        }
        bloke->param_action = 2;
    }
}

// FUNCTION: LEGOLAND 0x0044fe10
void FUN_0044fe10(Bloke *bloke) {
    Point to;
    unsigned char dir;

    switch (bloke->param_action) {
    case 0:
        to.x = DAT_0066b460.x << 8;
        to.y = DAT_0066b460.y << 8;
        bloke->dest = to;
        dir = CalcMoveLine(bloke->pos, to, &bloke->nav) + 0x10;
        bloke->field_e = 0xf;
        bloke->field_73 = dir;
        NewDirForAction(bloke, (dir >> 5) + 3);
        bloke->param_action++;
        break;
    case 1:
        NewLongTermAction(bloke, 6);
        return;
    }
}

// FUNCTION: LEGOLAND 0x0044fe80
void FUN_0044fe80(Bloke *bloke) {
    MapElement *object;
    MapElement *element;
    Ride *ride;
    unsigned short flags;
    unsigned char dir;
    int x;
    int y;
    Point out;

    switch (bloke->param_action) {
    case 0:
        for (object = GetFirstObjectMatching(DAT_006661c0); object != NULL; object = GetNextObjectMatching(object, DAT_006661c0)) {
            ride = object->field_0->data;
            if ((object->flags & 1) == 0) {
                bloke->brolly = object->anchor;
                bloke->goal.x = (ride->x + object->field_4) << 8;
                bloke->goal.y = (object->field_5 + ride->y) << 8;
                bloke->param_action++;
                break;
            }
        }
        if (object != NULL) {
            return;
        }
        NewLongTermAction(bloke, 6);
        return;
    case 1:
        switch (SuggestNextMove(&bloke->pos, &bloke->goal, &out)) {
        case -2:
            bloke->field_e = 10;
            return;
        case -3:
        case -1:
        case 0:
            bloke->field_e = 4;
            return;
        case 2:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->field_e = 6;
            bloke->field_73 = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            if (bloke->field_64 == 0) {
                bloke->param_action = 2;
                return;
            }
            break;
        case 1:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->field_e = 6;
            bloke->field_73 = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            if (bloke->field_64 == 0) {
                bloke->param_action = 1;
                return;
            }
            break;
        default:
            return;
        }
        break;
    case 2:
        x = bloke->brolly.pos.x;
        y = bloke->brolly.pos.y;
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            element = &GameMap[y][x];
        } else {
            element = NULL;
        }
        if (element->field_0 == DAT_006661c0 && (flags = element->flags, (flags & 0x80) != 0)) {
            if ((flags & 1) != 0) {
                bloke->param_action = 0;
                return;
            }
            element->flags = flags | 1;
            bloke->flags |= 8;
            bloke->dest.x = bloke->goal.x - 0x80;
            bloke->dest.y = bloke->goal.y;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
            bloke->field_73 = dir;
            bloke->field_e = 7;
            NewDirForAction(bloke, 7);
            bloke->field_5c = 0;
            bloke->param_action++;
            return;
        }
        break;
    case 3:
        x = bloke->brolly.pos.x;
        y = bloke->brolly.pos.y;
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            element = &GameMap[y][x];
        } else {
            element = NULL;
        }
        if (element->field_0 == DAT_006661c0 && (element->flags & 0x80) != 0) {
            if (bloke->field_5c <= 300) {
                return;
            }
            bloke->param_action++;
            return;
        }
        break;
    case 4:
        x = bloke->brolly.pos.x;
        y = bloke->brolly.pos.y;
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            element = &GameMap[y][x];
        } else {
            element = NULL;
        }
        if (element->field_0 == DAT_006661c0 && (element->flags & 0x80) != 0) {
            element->flags &= 0xfffe;
            bloke->dest.x = bloke->goal.x + 0x80;
            bloke->dest.y = bloke->goal.y;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
            bloke->field_e = 7;
            bloke->field_73 = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
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
void FUN_00450250(Bloke *bloke) {
    int state;

    switch (bloke->param_action) {
    case 0:
        NewDirForAction(bloke, 4);
        if (bloke->person->field_8 == 1) {
            bloke->flags |= 0x100;
            BlokeSetAnim(bloke, 2);
            BlokeSetFrame(bloke, 0);
            bloke->param_action++;
            return;
        }
        bloke->param_action = 2;
        return;
    case 1:
        if (PlayBlokeAnim(bloke) != 0) {
            BlokeWalkAnim(bloke);
            BlokeSetFrame(bloke, 0);
            bloke->flags &= 0xfeff;
            bloke->param_action++;
            return;
        }
        break;
    case 2:
        bloke->field_e = 0xd;
        bloke->param_action++;
        return;
    case 3:
        state = bloke->person->field_8;
        switch (state) {
        case 2:
            NewLongTermAction(bloke, 0x10);
            return;
        case 3:
            NewLongTermAction(bloke, 0x11);
            return;
        }
        NewLongTermAction(bloke, 6);
    }
}

// FUNCTION: LEGOLAND 0x00450330
void FUN_00450330(Bloke *bloke) {
    int state;

    switch (bloke->param_action) {
    case 0:
        NewDirForAction(bloke, 4);
        bloke->param_action++;
        break;
    case 1:
        bloke->field_e = 0xd;
        bloke->param_action++;
        return;
    case 2:
        state = bloke->person->field_8;
        switch (state) {
        case 2:
            NewLongTermAction(bloke, 0x10);
            return;
        case 3:
            NewLongTermAction(bloke, 0x11);
            return;
        }
    }
}

// FUNCTION: LEGOLAND 0x004503a0
void FUN_004503a0(Bloke *bloke, Footprint *box) {
    int ox;
    int oy;
    int tx;
    int ty;

    ox = bloke->goal.x;
    tx = bloke->pos.x >> 8;
    ty = bloke->pos.y >> 8;
    oy = bloke->goal.y;
    if (tx > box->x1 + ox) {
        if (ty < box->y0 + oy) {
            bloke->field_72 = 6;
            return;
        }
        if (ty > box->y1 + oy) {
            bloke->field_72 = 0;
            return;
        }
        bloke->field_72 = 7;
        return;
    }
    if (tx < box->x0 + ox) {
        if (ty < box->y0 + oy) {
            bloke->field_72 = 4;
            return;
        }
        if (ty > box->y1 + oy) {
            bloke->field_72 = 2;
            return;
        }
        bloke->field_72 = 3;
        return;
    }
    if (ty < box->y0 + oy) {
        bloke->field_72 = 5;
        return;
    }
    bloke->field_72 = 1;
}

// FUNCTION: LEGOLAND 0x00450450
void FUN_00450450(Bloke *bloke) {
    Ride *ride;

    switch (bloke->param_action) {
    case 0:
        ride = bloke->target->data;
        FUN_004503a0(bloke, &ride->footprint);
        bloke->field_58 = (rand() & 0x1f) + 10;
        bloke->param_action++;
        break;
    case 1:
        if (--bloke->field_58 < 0) {
            bloke->param_action++;
            return;
        }
        break;
    case 2:
        IncrementBlokeCounter(bloke->target->data, GetBlokeNum(bloke));
        NewLongTermAction(bloke, 6);
        return;
    }
}

// FUNCTION: LEGOLAND 0x004504d0
LEGO_EXPORT void DoHighLevelAI(Bloke *bloke) {
    void (*fn)(Bloke *);

    fn = PTR_Bloke_DoNothing_004b8368[bloke->action];
    if (fn != 0) {
        fn(bloke);
    }
    bloke->field_75 = 1;
    bloke->field_64 = 0;
}

// FUNCTION: LEGOLAND 0x00450500
int FUN_00450500(Point *a, Point *b) {
    Point d;

    d.x = abs(a->x - b->x);
    d.y = abs(a->y - b->y);
    return d.x + d.y == 1;
}

// FUNCTION: LEGOLAND 0x00450530
void FUN_00450530(Bloke *bloke) {
    MapElement *element;
    Ride *cls;
    int counter;
    int dist;
    int dx;
    Point t;
    int dy;
    float chance;
    int rides;
    int blocked;
    int shops;
    int food_score;
    int toilets;
    int rate;
    Point pos;
    Point origin;
    Point food;
    Point spot;

    toilets = 0;
    rides = 0;
    food_score = 0;
    shops = 0;
    blocked = 0;
    origin.x = bloke->pos.x >> 8;
    origin.y = bloke->pos.y >> 8;
    for (pos.y = origin.y - 4; pos.y <= origin.y + 4; pos.y++) {
        for (pos.x = origin.x - 4; pos.x <= origin.x + 4; pos.x++) {
            if (pos.x < 0 || pos.x >= lpConfig->width || pos.y < 0 || pos.y >= lpConfig->height ||
                (element = &GameMap[pos.y][pos.x]) == NULL) {
                blocked++;
                continue;
            }
            if (element->field_0 != 0 && (element->flags & 0x80) != 0) {
                cls = element->field_0->data;
                if (abs(origin.x - pos.x) <= cls->range && abs(origin.y - pos.y) <= cls->range) {
                    switch (cls->type) {
                    case 2:
                        rides += cls->value;
                        break;
                    case 3:
                        shops += cls->value >> GetBlokeCounter(cls, GetBlokeNum(bloke));
                        if ((bloke->flags & 0x20) == 0 && bloke->target != cls->element) {
                            chance = 15 / (GetBlokeCounter(cls, GetBlokeNum(bloke)) + 1);
                            if (rand() % 100 < chance && bloke->field_e != 0xf) {
                                t.x = bloke->pos.x >> 8;
                                t.y = bloke->pos.y >> 8;
                                dx = abs(t.x - pos.x);
                                dy = abs(t.y - pos.y);
                                dist = sqrt(dy * dy + dx * dx);
                                if (dist <= 1) {
                                    bloke->target = cls->element;
                                    bloke->goal.x = element->field_4;
                                    bloke->goal.y = element->field_5;
                                    NewLongTermAction(bloke, 0xf);
                                }
                            }
                        }
                        break;
                    case 4:
                    case 5:
                        food_score += cls->value >> GetBlokeCounter(cls, GetBlokeNum(bloke));
                        food.x = element->field_4 + cls->x;
                        food.y = element->field_5 + cls->y;
                        if (FUN_00450500(&origin, &food) != 0 && FUN_00450500(&pos, &food) != 0 &&
                            Calc_Item_Attractiveness(cls, bloke, 1) > 0x32 && bloke->action == 6 &&
                            (bloke->goal.x >> 8 != food.x || bloke->goal.y >> 8 != food.y)) {
                            bloke->goal.x = food.x << 8;
                            bloke->goal.y = food.y << 8;
                            bloke->param_action = 1;
                            bloke->field_e = 0;
                            bloke->target = cls->element;
                        }
                        break;
                    case 1:
                        toilets += cls->value >> GetBlokeCounter(cls, GetBlokeNum(bloke));
                        spot.x = element->field_4 + cls->x;
                        spot.y = element->field_5 + cls->y;
                        if (FUN_00450500(&spot, &origin) != 0 && FUN_00450500(&pos, &spot) != 0 &&
                            Calc_Item_Attractiveness(cls, bloke, 1) > 0x32 && bloke->action == 6 &&
                            (bloke->goal.x >> 8 != spot.x || bloke->goal.y >> 8 != spot.y)) {
                            bloke->goal.x = spot.x << 8;
                            bloke->goal.y = spot.y << 8;
                            bloke->param_action = 1;
                            bloke->field_e = 0;
                            bloke->target = cls->element;
                        }
                        break;
                    }
                }
            } else if (element->field_0 == 0) {
                dist = abs(origin.x - pos.x) + abs(origin.y - pos.y);
                if (dist != 0) {
                    rides += -20 / dist;
                }
            }
        }
    }
    FUN_00482df0(bloke, 2, blocked);
    FUN_00482df0(bloke, 3, rides);
    FUN_00482df0(bloke, 4, shops);
    FUN_00482df0(bloke, 5, food_score);
    FUN_00482df0(bloke, 6, toilets);
    rate = FUN_0044eb10(bloke);
    if (rate >= 3) {
        FUN_00482df0(bloke, 7, rate - 2);
    }
}

// FUNCTION: LEGOLAND 0x00450990
LEGO_EXPORT void ControlPeople(void) {
    Bloke *bloke;
    Bloke *next;

    FUN_0044ea50();
    for (bloke = FirstBloke; bloke != NULL; bloke = next) {
        next = bloke->next;
        DAT_00813b08 = bloke->field_81;
        if (FUN_004700c0(bloke) != 0) {
            // STRING: LEGOLAND 0x004b85a8
            DBPrintf("Processing your bloke\n");
        }
        FUN_0044eb50(bloke);
        if ((bloke->flags & 0x20) != 0) {
            if (bloke->field_e != 0) {
                DoLowLevelAI(bloke);
            }
        } else {
            FUN_0044eae0(bloke);
            if ((++bloke->field_5c & 0xf) == 0) {
                FUN_00482df0(bloke, 8, 1);
                FUN_00450530(bloke);
            }
            if (bloke->field_e == 0) {
                DoHighLevelAI(bloke);
            }
            if (bloke->field_e != 0) {
                DoLowLevelAI(bloke);
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00450a40
void FUN_00450a40(Bloke *bloke) {
    int state;

    if ((bloke->flags & 0x28) != 0) {
        return;
    }
    state = bloke->person->field_8;
    if (state < 2 || state > 3) {
        bloke->flags |= 0x8;
        NewLongTermAction(bloke, 14);
        return;
    }
    bloke->field_54 = DAT_008119a4;
}

// FUNCTION: LEGOLAND 0x00450a80
void FUN_00450a80(void) {
    int i;
    int count;
    union {
        Element *element;
        int index;
    } handle;
    BuildObj record;

    count = 0;
    for (i = 0; i < 256; i++) {
        if (DAT_006664f8[i].ride != NULL) {
            count++;
        }
    }
    SaveGameWrite(&count, 4);
    for (i = 0; i < 256; i++) {
        if (DAT_006664f8[i].ride != NULL) {
            record = DAT_006664f8[i];
            handle.element = DAT_006664f8[i].ride->element;
            FindeIneList(&handle.element);
            record.index = handle.index;
            SaveGameWrite(&record, sizeof(record));
        }
    }
}

// FUNCTION: LEGOLAND 0x00450b10
void FUN_00450b10(void) {
    int count;
    int i;

    count = 0;
    SaveGameRead(&count, 4);
    for (i = 0; i < count; i++) {
        SaveGameRead(&DAT_006664f8[i], sizeof(BuildObj));
        DAT_006664f8[i].ride = GeteListPtr(DAT_006664f8[i].index)->data;
    }
    for (; i < 256; i++) {
        DAT_006664f8[i].ride = NULL;
    }
}
