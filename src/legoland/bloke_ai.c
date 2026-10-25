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
    bloke->pending_action = 0;
    bloke->low_level_action = 0;
    bloke->field_1c = 0;
    DoHighLevelAI(bloke);
}

/* Picks a random LLIDB element (start at a random index, skip up to 31 matches) whose flags have 0x14 set and whose
 * object type is neither 0 nor 5 (food). Used for the three favourite attractions of a new visitor; NULL when the
 * whole database was walked without a match. */
// FUNCTION: LEGOLAND 0x0044e790
Element *PickRandomFavouriteAttraction(void) {
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

/* Same walk as PickRandomFavouriteAttraction, but only accepts objects of type 5 (food outlets): the new visitor's
 * favourite food. */
// FUNCTION: LEGOLAND 0x0044e890
Element *PickRandomFavouriteFood(void) {
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
    if (SharkCafeBrollyElem == 0) {
        SharkCafeBrollyElem = ElemID("SHARK CAFE BROLLY");
    }
    if (Entrance1Elem == 0) {
        Entrance1Elem = ElemID("ENTRANCE 1");
    }
    ParkVisitorCount++;
    bloke->speed = Rand_Tween(12, 24);
    bloke->field_78 = Rand_Max(MapStats.total_tiles);
    bloke->mood = Rand_Tween(10, 50);
    bloke->field_7e = Rand_Tween(0, 140) - 0x14;
    bloke->field_80 = Rand_Tween(5, 10);
    bloke->field_7c = Rand_Tween(0, BlokeTirednessThresholds[1]);
    bloke->target = NULL;
    bloke->last_ride = NULL;
    bloke->field_81 = NextBlokeLetter;
    NextBlokeLetter++;
    if (NextBlokeLetter > 'Z') {
        NextBlokeLetter = 'A';
    }
    RandomiseBlokeName(bloke);
    bloke->favourite_attraction_0 = PickRandomFavouriteAttraction();
    bloke->favourite_attraction_1 = PickRandomFavouriteAttraction();
    bloke->favourite_attraction_2 = PickRandomFavouriteAttraction();
    bloke->favourite_food = PickRandomFavouriteFood();
    NewLongTermAction(bloke, 2);
}

// FUNCTION: LEGOLAND 0x0044ea40
int GetMapCapacity(void) {
    return MapStats.capacity;
}

// FUNCTION: LEGOLAND 0x0044ea50
void SpawnVisitor(void) {
    Bloke *bloke;

    if (++VisitorSpawnCounter >= 0x1e && ParkVisitorCount < GetMapCapacity()) {
        bloke = MakeBloke(0);
        if (bloke != NULL) {
            VisitorSpawnCounter = 0;
            bloke->dir = rand() & 7;
            bloke->frame = rand() & 7;
            bloke->field_75 = 1;
            InitBlokeAI(bloke);
        }
    }
}

// FUNCTION: LEGOLAND 0x0044eab0
int IsBlokeReadyToLeave(Bloke *bloke) {
    return bloke->field_78 - bloke->mood / 2 > MapStats.total_tiles * 2;
}

// FUNCTION: LEGOLAND 0x0044eae0
void UpdateBlokeTimeInPark(Bloke *bloke) {
    bloke->field_78++;
    if ((bloke->flags & 0x8) != 0) {
        return;
    }
    if (IsBlokeReadyToLeave(bloke) != 0) {
        NewLongTermAction(bloke, 3);
    }
}

// FUNCTION: LEGOLAND 0x0044eb10
char GetBlokeHungerLevel(Bloke *bloke) {
    int i;

    if (MapStats.field_190 == 0) {
        return 1;
    }
    for (i = 1; i < 5; i++) {
        if (bloke->field_7c < BlokeTirednessThresholds[i - 1]) {
            return i - 1;
        }
    }
    return 4;
}

// FUNCTION: LEGOLAND 0x0044eb50
void UpdateBlokeTiredness(Bloke *bloke) {
    unsigned short value;
    int extended;

    if (MapStats.field_190 == 0) {
        return;
    }
    value = bloke->field_7c;
    extended = value & 0xffff;
    if (extended >= BlokeTirednessThresholds[3] && (bloke->flags & 0x28) == 0) {
        NewLongTermAction(bloke, 3);
        return;
    }
    if ((FrameCounter & 0x1f) != 0xf) {
        return;
    }
    if (extended > BlokeTirednessThresholds[3]) {
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
void BlokeAIEnterPark(Bloke *bloke) {
    MapElement *object;
    Ride *ride;
    unsigned char dir;

    switch (bloke->param_action) {
    case 0:
        object = GetFirstObjectMatching(Entrance1Elem);
        ride = Entrance1Elem->data;
        bloke->flags |= 8;
        bloke->dest.x = (object->field_4 + ride->footprint.x1 + 6) << 8;
        bloke->dest.y = (object->field_5 + ride->footprint.y1 - 5) << 8;
        bloke->pos.x = bloke->dest.x + ParkEntranceWalkInOffset.x;
        bloke->pos.y = bloke->dest.y + ParkEntranceWalkInOffset.y;
        dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
        bloke->low_level_action = 7;
        bloke->dir = (dir >> 5) + 3;
        bloke->param_action++;
        break;
    case 1:
        bloke->target = Entrance1Elem;
        if (PutBlokeOnRide(bloke, Entrance1Elem->data, 0) != 0) {
            bloke->param_action++;
            PushLongTermAction(bloke);
            NewLongTermAction(bloke, 5);
            ScriptDirtyCategories |= 0x40;
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
void LogBlokeAiMessage(char *msg) {
    int i;

    for (i = 0; i < 8; i++) {
        BlokeAiLogLinePtrs[i] = BlokeAiLogLines[(BlokeAiLogIndex + i) & 7];
    }
    BlokeAiLogIndex++;
    // STRING: LEGOLAND 0x004b8404
    sprintf(BlokeAiLogLinePtrs[7], "%c:%s", CurrentBlokeLetter, msg);
    // STRING: LEGOLAND 0x004b83f0
    DBPrintf("[Bloke %c] - %s\n", CurrentBlokeLetter, msg);
}

// FUNCTION: LEGOLAND 0x0044ed70
void BlokeAILeavePark(Bloke *bloke) {
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
        switch (SuggestNextMove(&bloke->pos, &ParkExitPos, &out)) {
        case -2:
            bloke->param_action = 5;
            return;
        case -3:
        case -1:
        case 0:
            bloke->low_level_action = 4;
            bloke->param_action = 2;
            if (++bloke->field_82 == 8) {
                bloke->param_action = 5;
                return;
            }
            break;
        case 2:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->low_level_action = 6;
            bloke->target_dir = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            if (bloke->field_64 != 0) {
                bloke->low_level_action = 4;
                bloke->param_action = 2;
                return;
            }
            bloke->param_action = 10;
            return;
        case 1:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->low_level_action = 6;
            bloke->target_dir = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            if (bloke->field_64 != 0) {
                bloke->low_level_action = 4;
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
        LogBlokeAiMessage(msg);
        switch (PTPSuggestNextMove(&bloke->pos, &ParkExitPos, &out)) {
        case 2:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->low_level_action = 0xb;
            bloke->target_dir = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            bloke->param_action = (bloke->field_64 & 1) ? 6 : 10;
            return;
        case 1:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->low_level_action = 0xb;
            bloke->target_dir = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            if ((bloke->field_64 & 1) != 0) {
                bloke->param_action = 6;
                return;
            }
            break;
        case 0:
            bloke->low_level_action = 4;
            bloke->param_action = 6;
            return;
        }
        break;
    case 6:
        // STRING: LEGOLAND 0x004b8424
        sprintf(msg, "Wandering...");
        LogBlokeAiMessage(msg);
        bloke->low_level_action = 4;
        bloke->param_action = 5;
        return;
    case 10:
        if (PutBlokeOnRide(bloke, Entrance1Elem->data, 0) != 0) {
            bloke->flags |= 8;
            bloke->target = Entrance1Elem;
            bloke->param_action++;
            PushLongTermAction(bloke);
            NewLongTermAction(bloke, 5);
            return;
        }
        break;
    case 11:
        object = GetFirstObjectMatching(Entrance1Elem);
        ride = Entrance1Elem->data;
        bloke->dest.x = (object->field_4 + ride->footprint.x1 + 6) << 8;
        bloke->dest.y = (object->field_5 + ride->footprint.y0 + 8) << 8;
        dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
        bloke->low_level_action = 7;
        bloke->target_dir = dir;
        NewDirForAction(bloke, (dir >> 5) + 3);
        bloke->param_action++;
        break;
    case 12:
        RateBlokeOnLeaving(bloke->mood);
        bloke->dest.x += ParkExitStep.x;
        bloke->dest.y += ParkExitStep.y;
        dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
        bloke->low_level_action = 7;
        bloke->target_dir = dir;
        NewDirForAction(bloke, (dir >> 5) + 3);
        bloke->param_action++;
        return;
    case 13:
        // STRING: LEGOLAND 0x004b840c
        DBPrintf("Killing MiniFig: $%x\n", bloke);
        DestroyBloke(bloke);
        ParkVisitorCount--;
    }
}

/* Long-term action 1: just switch the low-level action to BlokeActionRandomWalk (4). */
// FUNCTION: LEGOLAND 0x0044f170
void BlokeAIRandomWalk(Bloke *bloke) {
    bloke->low_level_action = 4;
}

/* True when the tile containing pos (1/256-tile units) is the entrance of an instance of ride on one of the four
 * orthogonally adjacent tiles: an object tile (flag 0x80) of that class whose anchor plus the class entrance offset
 * (ride->x, ride->y) equals that tile. */
// FUNCTION: LEGOLAND 0x0044f180
int IsAtRideEntrance(Point *pos, Ride *ride) {
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

/* Whether the object instance at the tile can operate: false when the tile has flag 0x200 (drawn blinking red on
 * the map) or, with the power feature enabled, flag 0x100. Visitors refuse a ride with "It's not working" when this
 * fails. The ride argument is unused. */
// FUNCTION: LEGOLAND 0x0044f360
int IsRideWorking(Ride *ride, TileId *tile) {
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
    if (MapStats.power_enabled != 0 && (element->flags & 0x100) != 0) {
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0044f3d0
unsigned int CountRidersAtTile(Ride *ride, TileId *tile) {
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
unsigned int IsRideFull(Ride *ride, TileId *tile) {
    int count;

    count = CountRidersAtTile(ride, tile);
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
int PutBlokeOnRide(Bloke *bloke, Ride *ride, int wait) {
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
            IncObjInstanceCount(&at);
            memset(node, 0, sizeof(RideNode));
            node->rider = bloke;
            bloke->field_58 = wait;
            node->person = bloke->person;
            bloke->flags |= 0x20;
            bloke->low_level_action = 0;
            bloke->pending_action = 0;
            bloke->field_35 = 0;
            if (ride == Entrance1Elem->data) {
                object = GetFirstObjectMatching(Entrance1Elem);
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
            ScriptDirtyCategories |= 0x20;
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
void BlokeAIWander(Bloke *bloke) {
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

    CurrentBlokeLetter = bloke->field_81;
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
                    LogBlokeAiMessage(msg);
                } else if (Calc_Item_Attractiveness(candidate, bloke, 0) > 10) {
                    // STRING: LEGOLAND 0x004b8554
                    sprintf(msg, "(%d) I'll go to the %s", Calc_Item_Attractiveness(candidate, bloke, 0), candidate->name);
                    bloke->target = candidate->element;
                    LogBlokeAiMessage(msg);
                    bloke->param_action = 1;
                    bloke->field_82 = 0;
                    if (more != 0) {
                        return;
                    }
                    break;
                } else {
                    // STRING: LEGOLAND 0x004b856c
                    sprintf(msg, "The %s is not worth going on.", candidate->name);
                    LogBlokeAiMessage(msg);
                    if (GetBlokeCounter(candidate, GetBlokeNum(bloke)) == 0) {
                        IncrementBlokeCounter(candidate, GetBlokeNum(bloke));
                    }
                }
                more = ShuffleObjKeys(&bloke->goal, &candidate);
            } while (more != 0);
            // STRING: LEGOLAND 0x004b8524
            sprintf(msg, "I've been on everything and I want to go home.");
            LogBlokeAiMessage(msg);
            NewLongTermAction(bloke, 3);
            return;
        }
        bloke->param_action = 2;
        return;
    case 1:
        switch (SuggestNextMove(&bloke->pos, &bloke->goal, &out)) {
        case -2:
            bloke->low_level_action = 10;
            return;
        case -3:
        case -1:
        case 0:
            if (IsAtRideEntrance(&bloke->goal, bloke->target->data) != 0) {
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
                    LogBlokeAiMessage(msg);
                    bloke->low_level_action = 4;
                    bloke->param_action++;
                    return;
                }
                ox = object->field_4;
                oy = object->field_5;
                if (ox == x && oy == y) {
                    bloke->low_level_action = 4;
                    return;
                }
                bloke->goal.x = ((ride->x + ox) << 8) + 0x80;
                bloke->goal.y = ((ride->y + oy) << 8) + 0x80;
                return;
            }
            bloke->low_level_action = 4;
            return;
        case 2:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->low_level_action = 6;
            bloke->target_dir = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            bloke->param_action = bloke->field_64 != 0 ? 0 : 10;
            return;
        case 1:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->low_level_action = 6;
            bloke->target_dir = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            bloke->param_action = bloke->field_64 == 0;
            return;
        }
        break;
    case 2:
    case 3:
        sprintf(msg, "Wandering...");
        LogBlokeAiMessage(msg);
        bloke->low_level_action = 4;
        bloke->param_action++;
        return;
    case 4:
        bloke->param_action = 0;
        return;
    case 5:
        // STRING: LEGOLAND 0x004b8434
        sprintf(msg, "Stuck, Routing Point To Point...");
        LogBlokeAiMessage(msg);
        switch (PTPSuggestNextMove(&bloke->pos, &bloke->goal, &out)) {
        case 2:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->low_level_action = 0xb;
            bloke->target_dir = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            bloke->param_action = (bloke->field_64 & 1) ? 6 : 10;
            return;
        case 1:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->low_level_action = 0xb;
            bloke->target_dir = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            if ((bloke->field_64 & 1) != 0) {
                bloke->param_action = 6;
                return;
            }
            break;
        case 0:
            bloke->low_level_action = 4;
            bloke->param_action = 6;
            return;
        }
        break;
    case 6:
        sprintf(msg, "Wandering...");
        LogBlokeAiMessage(msg);
        bloke->low_level_action = 4;
        bloke->param_action = 5;
        return;
    case 10:
        if (IsAtRideEntrance(&bloke->pos, bloke->target->data) != 0) {
            tile = GetObjectUID(&bloke->pos, bloke->target->data);
            if (IsRideFull(bloke->target->data, &tile) != 0) {
                // STRING: LEGOLAND 0x004b8500
                sprintf(msg, "I can't go on this ride. It is full");
                LogBlokeAiMessage(msg);
                ride = bloke->target->data;
                ApplyMoodDelta(bloke, 0, ride->field_3a);
                if (GetBlokeCounter(bloke->target->data, GetBlokeNum(bloke)) == 0) {
                    IncrementBlokeCounter(bloke->target->data, GetBlokeNum(bloke));
                }
                bloke->param_action = 2;
                return;
            }
            if (IsRideWorking(bloke->target->data, &tile) == 0) {
                // STRING: LEGOLAND 0x004b84d4
                sprintf(msg, "I can't go on this ride. It's not working");
                LogBlokeAiMessage(msg);
                ride = bloke->target->data;
                ApplyMoodDelta(bloke, 1, ride->field_3a);
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
                LogBlokeAiMessage(msg);
                NewLongTermAction(bloke, 5);
                /* reads the flags of the *next* LLIDB element; the ride's own flags were
                   presumably meant */
                if ((bloke->target[1].flags & 0x100000) != 0) {
                    if (PutBlokeOnRide(bloke, bloke->target->data, 0) != 0) {
                        if (IsRideFull(bloke->target->data, &tile) == 0) {
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
                    if (PutBlokeOnRide(bloke, bloke->target->data, (rand() & 0x1ff) + 200) != 0) {
                        return;
                    }
                }
            }
            // STRING: LEGOLAND 0x004b84a0
            sprintf(msg, "I can't get on the ride.");
            LogBlokeAiMessage(msg);
            ride = bloke->target->data;
            ApplyMoodDelta(bloke, 0, ride->field_3a);
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
void BlokeAIGoToEntrance(Bloke *bloke) {
    Point to;
    unsigned char dir;

    switch (bloke->param_action) {
    case 0:
        to.x = Entrance1Point.x << 8;
        to.y = Entrance1Point.y << 8;
        bloke->dest = to;
        dir = CalcMoveLine(bloke->pos, to, &bloke->nav) + 0x10;
        bloke->low_level_action = 0xf;
        bloke->target_dir = dir;
        NewDirForAction(bloke, (dir >> 5) + 3);
        bloke->param_action++;
        break;
    case 1:
        NewLongTermAction(bloke, 6);
        return;
    }
}

// FUNCTION: LEGOLAND 0x0044fe80
void BlokeAIGoToBrolly(Bloke *bloke) {
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
        for (object = GetFirstObjectMatching(SharkCafeBrollyElem); object != NULL; object = GetNextObjectMatching(object, SharkCafeBrollyElem)) {
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
            bloke->low_level_action = 10;
            return;
        case -3:
        case -1:
        case 0:
            bloke->low_level_action = 4;
            return;
        case 2:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->low_level_action = 6;
            bloke->target_dir = dir;
            NewDirForAction(bloke, (dir >> 5) + 3);
            if (bloke->field_64 == 0) {
                bloke->param_action = 2;
                return;
            }
            break;
        case 1:
            bloke->dest = out;
            dir = CalcMoveLine(bloke->pos, out, &bloke->nav) + 0x10;
            bloke->low_level_action = 6;
            bloke->target_dir = dir;
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
        if (element->field_0 == SharkCafeBrollyElem && (flags = element->flags, (flags & 0x80) != 0)) {
            if ((flags & 1) != 0) {
                bloke->param_action = 0;
                return;
            }
            element->flags = flags | 1;
            bloke->flags |= 8;
            bloke->dest.x = bloke->goal.x - 0x80;
            bloke->dest.y = bloke->goal.y;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
            bloke->target_dir = dir;
            bloke->low_level_action = 7;
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
        if (element->field_0 == SharkCafeBrollyElem && (element->flags & 0x80) != 0) {
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
        if (element->field_0 == SharkCafeBrollyElem && (element->flags & 0x80) != 0) {
            element->flags &= 0xfffe;
            bloke->dest.x = bloke->goal.x + 0x80;
            bloke->dest.y = bloke->goal.y;
            dir = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav) + 0x10;
            bloke->low_level_action = 7;
            bloke->target_dir = dir;
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

/* Long-term action 14, entered from PauseHoveredBloke while the cursor is over a visitor: face direction 4, blokes
 * with character 1 play animation 2 once, then BlokeActionWaitWhileHovered (low-level 13). When that ends, workers
 * go back to their idle action (16/17), everyone else to wandering (6). */
// FUNCTION: LEGOLAND 0x00450250
void BlokeAIStopWhileHovered(Bloke *bloke) {
    int state;

    switch (bloke->param_action) {
    case 0:
        NewDirForAction(bloke, 4);
        if (bloke->person->character == 1) {
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
        bloke->low_level_action = 0xd;
        bloke->param_action++;
        return;
    case 3:
        state = bloke->person->character;
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

/* Long-term action 20: the worker version of BlokeAIStopWhileHovered (face direction 4, wait while hovered, then
 * back to Gardener_Idle / Mechanic_Idle). */
// FUNCTION: LEGOLAND 0x00450330
void WorkerAIStopWhileHovered(Bloke *bloke) {
    int state;

    switch (bloke->param_action) {
    case 0:
        NewDirForAction(bloke, 4);
        bloke->param_action++;
        break;
    case 1:
        bloke->low_level_action = 0xd;
        bloke->param_action++;
        return;
    case 2:
        state = bloke->person->character;
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
void FaceBlokeTowardsFootprint(Bloke *bloke, Footprint *box) {
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
            bloke->dir = 6;
            return;
        }
        if (ty > box->y1 + oy) {
            bloke->dir = 0;
            return;
        }
        bloke->dir = 7;
        return;
    }
    if (tx < box->x0 + ox) {
        if (ty < box->y0 + oy) {
            bloke->dir = 4;
            return;
        }
        if (ty > box->y1 + oy) {
            bloke->dir = 2;
            return;
        }
        bloke->dir = 3;
        return;
    }
    if (ty < box->y0 + oy) {
        bloke->dir = 5;
        return;
    }
    bloke->dir = 1;
}

/* Long-term action 15, started by UpdateBlokeMoodFromSurroundings next to a type 3 object: face the target object's
 * footprint, wait 10-41 ticks, count the visit with IncrementBlokeCounter, then wander. */
// FUNCTION: LEGOLAND 0x00450450
void BlokeAIStopAtObject(Bloke *bloke) {
    Ride *ride;

    switch (bloke->param_action) {
    case 0:
        ride = bloke->target->data;
        FaceBlokeTowardsFootprint(bloke, &ride->footprint);
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

    fn = BlokeLongTermActionTable[bloke->action];
    if (fn != 0) {
        fn(bloke);
    }
    bloke->field_75 = 1;
    bloke->field_64 = 0;
}

// FUNCTION: LEGOLAND 0x00450500
int ArePointsOrthogonallyAdjacent(Point *a, Point *b) {
    Point d;

    d.x = abs(a->x - b->x);
    d.y = abs(a->y - b->y);
    return d.x + d.y == 1;
}

/* Every 16 ticks per visitor (ControlPeople): scans the 9x9 tiles around the visitor and sums the value of each
 * object type in range (reduced by how often this visitor already used it), applying the sums as mood deltas 2-7.
 * Also may redirect a wandering visitor to an adjacent food outlet or type 1 object it finds attractive, or start
 * BlokeAIStopAtObject next to a type 3 object. */
// FUNCTION: LEGOLAND 0x00450530
void UpdateBlokeMoodFromSurroundings(Bloke *bloke) {
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
                            if (rand() % 100 < chance && bloke->low_level_action != 0xf) {
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
                        if (ArePointsOrthogonallyAdjacent(&origin, &food) != 0 && ArePointsOrthogonallyAdjacent(&pos, &food) != 0 &&
                            Calc_Item_Attractiveness(cls, bloke, 1) > 0x32 && bloke->action == 6 &&
                            (bloke->goal.x >> 8 != food.x || bloke->goal.y >> 8 != food.y)) {
                            bloke->goal.x = food.x << 8;
                            bloke->goal.y = food.y << 8;
                            bloke->param_action = 1;
                            bloke->low_level_action = 0;
                            bloke->target = cls->element;
                        }
                        break;
                    case 1:
                        toilets += cls->value >> GetBlokeCounter(cls, GetBlokeNum(bloke));
                        spot.x = element->field_4 + cls->x;
                        spot.y = element->field_5 + cls->y;
                        if (ArePointsOrthogonallyAdjacent(&spot, &origin) != 0 && ArePointsOrthogonallyAdjacent(&pos, &spot) != 0 &&
                            Calc_Item_Attractiveness(cls, bloke, 1) > 0x32 && bloke->action == 6 &&
                            (bloke->goal.x >> 8 != spot.x || bloke->goal.y >> 8 != spot.y)) {
                            bloke->goal.x = spot.x << 8;
                            bloke->goal.y = spot.y << 8;
                            bloke->param_action = 1;
                            bloke->low_level_action = 0;
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
    ApplyMoodDelta(bloke, 2, blocked);
    ApplyMoodDelta(bloke, 3, rides);
    ApplyMoodDelta(bloke, 4, shops);
    ApplyMoodDelta(bloke, 5, food_score);
    ApplyMoodDelta(bloke, 6, toilets);
    rate = GetBlokeHungerLevel(bloke);
    if (rate >= 3) {
        ApplyMoodDelta(bloke, 7, rate - 2);
    }
}

// FUNCTION: LEGOLAND 0x00450990
LEGO_EXPORT void ControlPeople(void) {
    Bloke *bloke;
    Bloke *next;

    SpawnVisitor();
    for (bloke = FirstBloke; bloke != NULL; bloke = next) {
        next = bloke->next;
        CurrentBlokeLetter = bloke->field_81;
        if (IsBlokeHovered(bloke) != 0) {
            // STRING: LEGOLAND 0x004b85a8
            DBPrintf("Processing your bloke\n");
        }
        UpdateBlokeTiredness(bloke);
        if ((bloke->flags & 0x20) != 0) {
            if (bloke->low_level_action != 0) {
                DoLowLevelAI(bloke);
            }
        } else {
            UpdateBlokeTimeInPark(bloke);
            if ((++bloke->field_5c & 0xf) == 0) {
                ApplyMoodDelta(bloke, 8, 1);
                UpdateBlokeMoodFromSurroundings(bloke);
            }
            if (bloke->low_level_action == 0) {
                DoHighLevelAI(bloke);
            }
            if (bloke->low_level_action != 0) {
                DoLowLevelAI(bloke);
            }
        }
    }
}

/* Called while the cursor hovers a visitor (screens.c help code): an idle visitor is stopped (flags |= 8, long-term
 * action 14 = BlokeAIStopWhileHovered); for workers only field_54 is reset, which delays
 * BlokeActionRandomWalkDelayed by 50 frames. */
// FUNCTION: LEGOLAND 0x00450a40
void PauseHoveredBloke(Bloke *bloke) {
    int state;

    if ((bloke->flags & 0x28) != 0) {
        return;
    }
    state = bloke->person->character;
    if (state < 2 || state > 3) {
        bloke->flags |= 0x8;
        NewLongTermAction(bloke, 14);
        return;
    }
    bloke->field_54 = FrameCounter;
}

// FUNCTION: LEGOLAND 0x00450a80
void SaveBuildObjList(void) {
    int i;
    int count;
    union SavedElement handle;
    BuildObj record;

    count = 0;
    for (i = 0; i < 256; i++) {
        if (BuildObjArray[i].ride != NULL) {
            count++;
        }
    }
    SaveGameWrite(&count, 4);
    for (i = 0; i < 256; i++) {
        if (BuildObjArray[i].ride != NULL) {
            record = BuildObjArray[i];
            handle.element = BuildObjArray[i].ride->element;
            FindeIneList(&handle);
            record.index = handle.index;
            SaveGameWrite(&record, sizeof(record));
        }
    }
}

// FUNCTION: LEGOLAND 0x00450b10
void LoadBuildObjArray(void) {
    int count;
    int i;

    count = 0;
    SaveGameRead(&count, 4);
    for (i = 0; i < count; i++) {
        SaveGameRead(&BuildObjArray[i], sizeof(BuildObj));
        BuildObjArray[i].ride = GeteListPtr(BuildObjArray[i].index)->data;
    }
    for (; i < 256; i++) {
        BuildObjArray[i].ride = NULL;
    }
}
