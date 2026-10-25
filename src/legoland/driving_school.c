#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "bricks.h"
#include "driving_school.h"
#include "gamemap.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "objclass.h"
#include "print_sprite.h"
#include "render3d.h"
#include "ride_bloke.h"
#include "ride_queue.h"
#include "roads.h"
#include "sound_music.h"
#include "sound_sfx.h"
#include "tilemap.h"

struct Node8 {
    unsigned char pad_0[8];
    void *next;
};

struct DSSampleConfig {
    /* 0x00 */ int field_0;
    /* 0x04 */ int field_4;
};

struct DSObjClass {
    /* 0x00 */ unsigned char pad_0[0x26];
    /* 0x26 */ short cost;
    /* 0x28 */ unsigned char pad_28[0xc4 - 0x28];
    /* 0xc4 */ Element *element;
};

#include "image_sprite.h"
#include "pumps.h"

// FUNCTION: LEGOLAND 0x004051a0
void ComputeRoadDistancesStep(short param_1) {
    struct RideQueueEntry *node;
    struct RideQueueEntry *e0;
    struct RideQueueEntry *e1;
    struct RideQueueEntry *e2;
    struct RideQueueEntry *e3;

    for (node = RoadBfsFrontier; node != NULL; node = node->field_4) {
        e0 = FindRoadEntryAt(node->x, node->y - 4);
        e1 = FindRoadEntryAt(node->x + 4, node->y);
        e2 = FindRoadEntryAt(node->x, node->y + 4);
        e3 = FindRoadEntryAt(node->x - 4, node->y);
        if ((node->field_14 & 0xf) == 6) {
            e2 = NULL;
            e3 = NULL;
            e0 = NULL;
        } else if (e0 != NULL && ((short)e0->id != param_1 || e0->field_18 != NULL)) {
            e0 = NULL;
        }
        if (e1 != NULL && ((short)e1->id != param_1 || e1->field_18 != NULL)) {
            e1 = NULL;
        }
        if (e2 != NULL && ((short)e2->id != param_1 || e2->field_18 != NULL)) {
            e2 = NULL;
        }
        if (e3 != NULL && ((short)e3->id != param_1 || e3->field_18 != NULL)) {
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
            e0->field_4 = RoadBfsNextFrontier;
            RoadBfsNextFrontier = e0;
        }
        if (e1 != NULL) {
            e1->field_4 = RoadBfsNextFrontier;
            RoadBfsNextFrontier = e1;
        }
        if (e2 != NULL) {
            e2->field_4 = RoadBfsNextFrontier;
            RoadBfsNextFrontier = e2;
        }
        if (e3 != NULL) {
            e3->field_4 = RoadBfsNextFrontier;
            RoadBfsNextFrontier = e3;
        }
    }
}

// FUNCTION: LEGOLAND 0x00405310
void ComputeRoadDistances(TileId tile) {
    struct RideQueueEntry *cur;
    struct RideQueueEntry *ret;
    struct RideQueueEntry *tmp;

    cur = RoadEntryList;
    while (cur != NULL) {
        if (cur->id == tile.id) {
            cur->field_18 = NULL;
        }
        cur = cur->next;
    }

    ret = FindQueueEntryById(tile.id);
    ret->field_15 = 0;
    ret->field_4 = NULL;

    RoadBfsFrontier = ret;
    RoadBfsNextFrontier = NULL;

    do {
        ComputeRoadDistancesStep(tile.id);
        tmp = RoadBfsNextFrontier;
        RoadBfsFrontier = tmp;
        RoadBfsNextFrontier = NULL;
    } while (tmp != NULL);
}

// FUNCTION: LEGOLAND 0x00405370
void LoadDrivingSchoolResources(struct Element *param_1) {
    struct LLS *lls;

    DrivingSchoolRide = param_1->ride;
    if (LLIDB_FindElement("DSCHOOL MAPPING", (struct Element **)&param_1, 0) == 0) {
        DSchoolMappingData = LLIDB_LoadData((struct Element *)param_1);
    }
    if (LLIDB_FindElement("DSCHOOL BLUE CAR", (struct Element **)&param_1, 0) == 0) {
        DSchoolBlueCarData = LLIDB_LoadData((struct Element *)param_1);
    }
    DrivingSchoolRide->flags |= 0x420;
    Load_FXList(DRIVING_SCHOOL_SFX, 6);
    // STRING: LEGOLAND 0x004b4524
    DSchoolMatteSprite = LoadSprite("DSchool Matte.lls", 1);
    // STRING: LEGOLAND 0x004b4510
    DSchoolBluePalette = LoadPalette(".\\3ddata\\blu.col");
    // STRING: LEGOLAND 0x004b44fc
    DSchoolYellowPalette = LoadPalette(".\\3ddata\\yel.col");
    // STRING: LEGOLAND 0x004b44e8
    DSchoolRedPalette = LoadPalette(".\\3ddata\\red.col");
    // STRING: LEGOLAND 0x004b44d8
    DSCarSprite = LoadSprite("ds_car&m.lls", 1);
    if (DSCarSprite != NULL) {
        lls = GetLLSForSprite((struct Sprite *)DSCarSprite);
        if (lls != 0) {
            LLSStop(lls);
        }
    }
}

// FUNCTION: LEGOLAND 0x00405460
void UnloadDrivingSchoolResources(void) {
    struct Element *local;

    // STRING: LEGOLAND 0x004b454c
    if (LLIDB_FindElement("DSCHOOL MAPPING", &local, 0) == 0) {
        LLIDB_UnLoadData(local);
    }

    // STRING: LEGOLAND 0x004b4538
    if (LLIDB_FindElement("DSCHOOL BLUE CAR", &local, 0) == 0) {
        LLIDB_UnLoadData(local);
    }

    while (DrivingSchoolCars != NULL) {
        RemoveDrivingSchoolCar(DrivingSchoolCars);
    }

    RemoveAllPumps();

    while (RoadEntryList != NULL) {
        struct RideQueueEntry *next = RoadEntryList->next;
        free(RoadEntryList);
        RoadEntryList = next;
    }

    while (DrivingSchoolCountList != NULL) {
        void *next = ((struct Node8 *)DrivingSchoolCountList)->next;
        free(DrivingSchoolCountList);
        DrivingSchoolCountList = next;
    }

    Kill_FXList(DRIVING_SCHOOL_SFX, 6);
    KillSprite(DSchoolMatteSprite);
    free(DSchoolBluePalette);
    free(DSchoolYellowPalette);
    free(DSchoolRedPalette);
    KillSprite(DSCarSprite);
    DrivingSchoolRide = NULL;
}

// FUNCTION: LEGOLAND 0x00405570
void DrivingSchoolSetEditMode(void) {
    EditMode.unk0 = 1;
    EditMode.unk8 = DrivingSchoolRide;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&EditMode.unk8->footprint);
    DefaultCursor(&ExtraEditCursor1);
    memcpy(ExtraEditCursor1.field_1414, DrivingSchoolPart1Footprint, 20);
    ExtraEditCursor1.field_1828 |= 0x100;
    DefaultCursor(&ExtraEditCursor2);
    memcpy(ExtraEditCursor2.field_1414, DrivingSchoolPart2Footprint, 20);
    ExtraEditCursor2.field_1828 |= 0x200;
    DefaultCursor(&ExtraEditCursor3);
    memcpy(ExtraEditCursor3.field_1414, DrivingSchoolPart3Footprint, 20);
    EditCursor.next = &ExtraEditCursor1;
    ExtraEditCursor1.next = &ExtraEditCursor2;
}

// FUNCTION: LEGOLAND 0x00405630
void DrivingSchoolAddObject(struct Element *param_1, int *coords) {
    TileId tile;
    struct CountNode *node;
    struct Cursor *cursor;
    int x;
    int y;
    struct Element *elem;

    tile.pos.x = coords[0];
    tile.pos.y = coords[1];
    AddBasicObject(param_1, coords);

    node = (struct CountNode *)malloc(sizeof(struct CountNode));
    node->tile = tile;
    node->next = DrivingSchoolCountList;
    DrivingSchoolCountList = node;
    cursor = EditCursor.next;
    x = cursor->tile_x;
    y = cursor->tile_y;

    AddRoadEntry(tile, x - 3, y - 4, 6, 1);
    AddRoadEntry(tile, x + 1, y - 4, 0, 1);
    AddRoadEntry(tile, x + 5, y - 4, 3, 1);
    AddRoadEntry(tile, x + 5, y, 0, 0);
    AddRoadEntry(tile, x + 5, y + 4, 0, 0);

    // STRING: LEGOLAND 0x004b455c
    elem = (struct Element *)ElemID("Driving School Roads");
    if (elem != NULL && (elem->flags & 4) != 0) {
        IncrementObjectCount(elem->obj);
        IncrementObjectCount(elem->obj);
        IncrementObjectCount(elem->obj);
        IncrementObjectCount(elem->obj);
        IncrementObjectCount(elem->obj);
    }

    node->field_4 = 5;
    ComputeRoadDistances(tile);
}

// FUNCTION: LEGOLAND 0x00405740
void DrivingSchoolCalcCursor(struct Element *param_1, int *param_2, unsigned int param_3) {
    struct Ride *src = param_1->ride;
    struct Ride *c694;
    unsigned int mapx;
    unsigned int mapy;

    memcpy(EditCursor.field_1414, &src->footprint, sizeof(EditCursor.field_1414));
    EditCursor.field_1830 = 0;
    EditCursor.field_1828 = 0x4408;
    ScreenToMapRef(param_2, &EditCursor.tile_x, param_3);

    c694 = DrivingSchoolRide;
    mapx = EditCursor.tile_x;
    memcpy(ExtraEditCursor1.field_1414, DrivingSchoolPart1Footprint, 20);
    ExtraEditCursor1.field_1830 = 0;
    mapy = EditCursor.tile_y;
    ExtraEditCursor1.field_1828 = 0x4108;
    ExtraEditCursor1.tile_x = c694->footprint.x0 + mapx;
    ExtraEditCursor1.tile_y = c694->footprint.y0 + mapy;

    memcpy(ExtraEditCursor2.field_1414, DrivingSchoolPart2Footprint, 20);
    ExtraEditCursor2.field_1830 = 0;
    ExtraEditCursor2.field_1828 = 0x4208;
    ExtraEditCursor2.tile_x = c694->footprint.x0 + mapx;
    ExtraEditCursor2.tile_y = c694->footprint.y0 + mapy;

    memcpy(ExtraEditCursor3.field_1414, DrivingSchoolPart3Footprint, 20);
    ExtraEditCursor3.field_1830 = 0;
    ExtraEditCursor3.field_1828 = 0x5008;
    ExtraEditCursor3.tile_x = c694->footprint.x0 + mapx;
    ExtraEditCursor3.tile_y = c694->footprint.y0 + mapy;

    ValidateCursor(&ExtraEditCursor3, src);
    ValidateCursor(&ExtraEditCursor2, src);
    ValidateCursor(&ExtraEditCursor1, src);
    ValidateCursor(&EditCursor, src);

    EditCursor.next = &ExtraEditCursor1;
    ExtraEditCursor1.next = &ExtraEditCursor2;
    ExtraEditCursor2.next = &ExtraEditCursor3;
    PropagateCursorBlocked(&EditCursor);
}

// FUNCTION: LEGOLAND 0x004058a0
void DrivingSchoolDCalcCursor(struct Element *param_1, struct Point *param_2) {
    struct RideQueueEntry *node = RoadEntryList;

    BasicObjectDCalcCursor(param_1, param_2);
    DefaultCursor(&ExtraEditCursor1);
    memcpy(ExtraEditCursor1.field_1414, RoadPieceFootprint, 20);

    while (node != NULL) {
        if (node->id == QueryObj.id) {
            ExtraEditCursor1.tile_x = node->x;
            ExtraEditCursor1.tile_y = node->y;
            ResetCursorBuildable(&ExtraEditCursor1);
            ExtraEditCursor1.field_1828 = 0x18;
            BuildCursorPtr(&ExtraEditCursor1, 0, 0);
            RenderCursor(&ExtraEditCursor1);
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00405940
void DrivingSchoolRemoveObject(Element *obj, TileId tile, struct Cursor *param_3) {
    struct RideQueueEntry *queue = RoadEntryList;
    struct CountNode *count = DrivingSchoolCountList;
    struct NewBloke *blokes = DrivingSchoolCars;
    struct RideQueueEntry *next;
    struct NewBloke *nextBloke;

    StandardRemoveObject(obj, tile, (struct Cursor *)param_3);
    DefaultCursor(&ExtraEditCursor1);
    memcpy(ExtraEditCursor1.field_1414, RoadPieceFootprint, 20);

    if (count->tile.id == tile.id) {
        DrivingSchoolCountList = count->next;
        free(count);
    } else {
        while (count->next != NULL) {
            if (count->next->tile.id == tile.id) {
                count->next = count->next->next;
                free(count->next);
                break;
            }
            count = count->next;
        }
    }

    RemovePumpsWithId(QueryObj.id);

    while (queue != NULL) {
        next = queue->next;
        if (queue->id == QueryObj.id) {
            if (queue->field_14 & 0x10) {
                queue->field_14 &= 0xef;
                UpdateQueuePathShape(queue->id, queue->x, queue->y);
                AddBricks(((struct DSObjClass *)ZebraCrossingRide)->cost);
            }
            ExtraEditCursor1.tile_x = queue->x;
            ExtraEditCursor1.tile_y = queue->y;
            StandardRemoveObject((Element *)((struct DSObjClass *)DrivingSchoolRoadsClass)->element, *(TileId *)&queue->id, &ExtraEditCursor1);
            RemoveRoadEntry(queue->x, queue->y);
        }
        queue = next;
    }

    AddBricks(((struct DSObjClass *)DrivingSchoolRoadsClass)->cost * 5);

    while (blokes != NULL) {
        nextBloke = blokes->next;
        if ((short)blokes->id == (short)QueryObj.id) {
            RemoveDrivingSchoolCar(blokes);
        }
        blokes = nextBloke;
    }

    RemoveAllBlokesFromRide(obj->ride, tile);
}

// FUNCTION: LEGOLAND 0x00405ad0
struct RideSpriteInfo *GetDrivingSchoolSpriteInfo(struct Element *arg1, unsigned short arg2) {
    struct Ride *sub = arg1->ride;
    struct Sprite *inner = sub->layer;

    RideSpriteInfoBuffer.sprite = inner;
    RideSpriteInfoBuffer.x = sub->field_14;
    RideSpriteInfoBuffer.y = sub->field_18;
    RideSpriteInfoBuffer.id = arg2;

    inner = sub->layer;
    inner->flags |= 0x2000;

    return &RideSpriteInfoBuffer;
}

// FUNCTION: LEGOLAND 0x00405b10
void RenderDrivingSchool(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int clip) {
    struct Ride *ride = obj->ride;
    struct RideNode *node;
    struct Point ref;
    int bounds[4];
    int dx;
    int dy;

    node = ride->riders;
    ref.x = ((unsigned char *)tile)[0];
    ref.y = ((unsigned char *)tile)[1];
    for (; node != NULL; node = node->next) {
        if (*tile == node->tile.id) {
            unsigned char state = node->rider->param_action;
            if (state <= 1 || state >= 3) {
                IP_RenderBlokeIn3DNow(node->rider);
            }
        }
    }
    GetTileBounds(&ref, bounds);
    if (DSchoolMatteSprite != NULL) {
        dx = ride->field_14;
        dy = ride->field_18;
        if (dx < 0) {
            dx = -(-dx >> 1);
        } else {
            dx = dx >> 1;
        }
        if (dy < 0) {
            dy = -(-dy >> 1);
        } else {
            dy = dy >> 1;
        }
        PrintSprite(DSchoolMatteSprite, bounds[0] + dx, bounds[1] + dy, clip, 0);
    }
}

// FUNCTION: LEGOLAND 0x00405bd0
void DrivingSchoolUpdate(Element *obj) {
    int tile_id;
    int ride_y0;
    unsigned int ride_y3;
    int ride_x0;
    int flags;
    volatile int tile_y;
    int ride_x3;
    struct Ride *ride = obj->ride;
    struct RideNode *node = ride->riders;
    struct RideNode *next;
    struct Bloke *bloke;
    struct SampleSource source;
    struct SampleSource source2;
    unsigned char move;
    int res;
    volatile unsigned int count;
    struct Sample *sample;
    int r;

    UpdateDrivingSchoolCars();
    UpdateTrafficLights();
    while (node) {
        bloke = node->rider;
        next = node->next;
        if (0 == bloke->low_level_action) {
            switch (bloke->param_action) {
            case 0:
                ride_x0 = ride->x;
                ride_y0 = ride->y;
                bloke->dest.x = (node->tile.pos.x + ride_x0) << 8;
                tile_y = node->tile.pos.y;
                bloke->dest.y = (tile_y - 3 + ride_y0) << 8;
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = 0x10 + move;
                NewDirForAction(bloke, ((unsigned char)(move + 0x10) >> 5) + 3);
                bloke->field_58 = (rand() & 7) + 1;
                bloke->param_action++;
                break;
            case 1:
                if (!bloke->field_58) {
                    tile_id = node->tile.id;
                    count = CountNewBlokesWithId(tile_id);
                    /* The original compares the weighted rider count as a signed value (jge). */
                    if ((int)(5 * count) < (int)CountRoadEntriesWithId(node->tile.id)) {
                        res = AddDrivingSchoolCar(node->tile.id, bloke);
                        if (0 == res) {
                            source.type = 1;
                            source.bloke = bloke;
                            PlayInstanceOfSample(DRIVING_SCHOOL_SFX[0].sample, 0, 1, &source);
                            r = rand() % 4 + 1;
                            sample = PlayInstanceOfSample(DRIVING_SCHOOL_SFX[r].sample, 1, 1, &source);
                            AdjustPSampleFreq(sample, 10);
                            BlokeSitAnim(bloke);
                            BlokeSetFrame(bloke, 0);
                            /* Advance the state before setting flag 0x80: MSVC6 then
                             * schedules this load/increment and flag update as in the original. */
                            bloke->param_action++;
                            bloke->flags = bloke->flags | 0x80;
                        } else if (res == -1) {
                            bloke->param_action = 3;
                        } else if (res == -2) {
                            bloke->field_58 = (rand() & 0x1f) + 2;
                        }
                    }
                } else {
                    bloke->field_58--;
                }
                break;
            case 3:
                bloke->flags = 0xff7f & bloke->flags;
                BlokeWalkAnim(bloke);
                BlokeSetFrame(bloke, 0);
                bloke->pos.x = (node->tile.pos.x + ride->x) << 8;
                bloke->pos.y = (node->tile.pos.y - 3 + ride->y) << 8;
                bloke->dest.x = ((node->tile.pos.x + ride->x) << 8) + 0x80;
                bloke->dest.y = ((node->tile.pos.y + ride->y) << 8) + 0x80;
                move = CalcMoveLine(bloke->pos, bloke->dest, &bloke->nav);
                bloke->low_level_action = 7;
                bloke->target_dir = move + 0x10;
                NewDirForAction(bloke, ((unsigned char)(move + 0x10) >> 5) + 3);
                bloke->param_action++;
                break;
            case 4:
                RemoveBlokeFromRide(ride, node);
                bloke->flags &= 0xfff7;
                source2.type = 1;
                source2.bloke = bloke;
                KillAllSamplesFromSource(&source2);
                break;
            }
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x00405e70
int DrivingSchool_Save(struct Element *element) {
    struct CountNode *countNode;
    struct RideQueueEntry *queue;
    struct PumpNode *pump;
    struct NewBloke *bloke;
    struct CountNode *countCur;
    struct RideQueueEntry *queueCur;
    struct PumpNode *pumpCur;
    struct NewBloke *blokeCur;
    int count;
    struct NewBloke buf;

    count = 0;
    for (countCur = DrivingSchoolCountList; countCur != NULL; countCur = countCur->next) {
        count++;
    }
    countNode = DrivingSchoolCountList;
    SaveGameWrite(&count, 4);
    while (count--) {
        SaveGameWrite(countNode, sizeof(struct CountNode));
        countNode = countNode->next;
    }

    count = 0;
    for (queueCur = RoadEntryList; queueCur != NULL; queueCur = queueCur->next) {
        count++;
    }
    queue = RoadEntryList;
    SaveGameWrite(&count, 4);
    while (count--) {
        SaveGameWrite(queue, sizeof(struct RideQueueEntry));
        queue = queue->next;
    }

    count = 0;
    for (pumpCur = PumpList; pumpCur != NULL; pumpCur = pumpCur->next) {
        count++;
    }
    pump = PumpList;
    SaveGameWrite(&count, 4);
    while (count--) {
        SaveGameWrite(pump, sizeof(struct PumpNode));
        pump = pump->next;
    }

    count = 0;
    for (blokeCur = DrivingSchoolCars; blokeCur != NULL; blokeCur = blokeCur->next) {
        count++;
    }
    bloke = DrivingSchoolCars;
    SaveGameWrite(&count, 4);
    while (count--) {
        memcpy(&buf, bloke, sizeof(buf));
        buf.owner = GetBlokeNum(buf.bloke); /* the bloke by number */
        SaveGameWrite(&buf, sizeof(buf));
        bloke = bloke->next;
    }

    return 1;
}

// FUNCTION: LEGOLAND 0x00406020
void AddDrivingSchoolScore(unsigned short arg1, unsigned int arg2) {
    struct CountNode *current = DrivingSchoolCountList;
    while (current != NULL) {
        if (current->tile.id == arg1) {
            if (current != NULL) {
                current->field_4 += arg2;
            }
            break;
        }
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x00406050
int GetBestDrivingSchoolScore(void) {
    struct CountNode *current = DrivingSchoolCountList;
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
int DrivingSchool_Load(struct Element *element) {
    struct CountNode *countPrev;
    struct CountNode *countNode;
    struct RideQueueEntry *queuePrev;
    struct RideQueueEntry *queueNode;
    struct PumpNode *pumpPrev;
    struct PumpNode *pumpNode;
    struct NewBloke *blokePrev;
    struct NewBloke *blokeNode;
    int count;

    countPrev = NULL;
    queuePrev = NULL;
    pumpPrev = NULL;
    blokePrev = NULL;

    DrivingSchoolCountList = NULL;
    SaveGameRead(&count, 4);
    while (count--) {
        if (countPrev == NULL) {
            DrivingSchoolCountList = (struct CountNode *)malloc(sizeof(struct CountNode));
            countPrev = DrivingSchoolCountList;
        } else {
            countNode = (struct CountNode *)malloc(sizeof(struct CountNode));
            countPrev->next = countNode;
            countPrev = countNode;
        }
        SaveGameRead(countPrev, sizeof(struct CountNode));
    }

    RoadEntryList = NULL;
    SaveGameRead(&count, 4);
    while (count--) {
        if (queuePrev == NULL) {
            RoadEntryList = (struct RideQueueEntry *)malloc(sizeof(struct RideQueueEntry));
            queuePrev = RoadEntryList;
        } else {
            queueNode = (struct RideQueueEntry *)malloc(sizeof(struct RideQueueEntry));
            queuePrev->next = queueNode;
            queuePrev = queueNode;
        }
        SaveGameRead(queuePrev, sizeof(struct RideQueueEntry));
    }

    PumpList = NULL;
    SaveGameRead(&count, 4);
    while (count--) {
        if (pumpPrev == NULL) {
            PumpList = (struct PumpNode *)malloc(sizeof(struct PumpNode));
            pumpPrev = PumpList;
        } else {
            pumpNode = (struct PumpNode *)malloc(sizeof(struct PumpNode));
            pumpPrev->next = pumpNode;
            pumpPrev = pumpNode;
        }
        SaveGameRead(pumpPrev, sizeof(struct PumpNode));
    }

    DrivingSchoolCars = NULL;
    SaveGameRead(&count, 4);
    while (count--) {
        if (blokePrev == NULL) {
            DrivingSchoolCars = (struct NewBloke *)malloc(sizeof(struct NewBloke));
            blokePrev = DrivingSchoolCars;
        } else {
            blokeNode = (struct NewBloke *)malloc(sizeof(struct NewBloke));
            blokePrev->next = blokeNode;
            blokePrev = blokeNode;
        }
        SaveGameRead(blokePrev, sizeof(struct NewBloke));
        blokePrev->bloke = GetBlokePtr(blokePrev->owner);
    }

    for (countNode = DrivingSchoolCountList; countNode != NULL; countNode = countNode->next) {
        ComputeRoadDistances(countNode->tile);
    }

    return 1;
}
