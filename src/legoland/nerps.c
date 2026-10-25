#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "bricks.h"
#include "challenge.h"
#include "debug_alloc.h"
#include "draw.h"
#include "gamemap.h"
#include "help.h"
#include "interface.h"
#include "llidb.h"
#include "map_object.h"
#include "nerps.h"
#include "obj_instance.h"
#include "objclass.h"
#include "objectives.h"
#include "popupinfo.h"
#include "saveload.h"
#include "screens.h"
#include "timer.h"
#include "worker.h"

#pragma intrinsic(strcpy, strlen)

// FUNCTION: LEGOLAND 0x0046a040
void SetScriptFeature(unsigned int param_1, unsigned int param_2) {
    switch (param_1) {
    case 0:
        MapStats.field_184 = param_2;
        break;
    case 1:
        MapStats.ride_wear = param_2;
        break;
    case 3:
        MapStats.field_188 = param_2;
        break;
    case 4:
        MapStats.power_enabled = param_2;
        break;
    case 5:
        MapStats.field_190 = param_2;
        break;
    case 6:
        MapStats.timer_minutes = param_2;
        StartAppraisalTimer();
        break;
    case 7:
        lpConfig->repair_orders_enabled = param_2;
        break;
    case 8:
        lpConfig->field_2c = param_2;
        break;
    case 9:
        MapStats.brick_meter_max = param_2;
        break;
    case 10:
        MapStats.show_capacity = param_2;
        break;
    case 0xb:
        MapStats.field_3a8 = param_2;
    }
    PopInfoSizeMayChange();
}

// FUNCTION: LEGOLAND 0x0046a120
unsigned int ScriptEventFeature(struct ObjectiveEvent *arg) {
    SetScriptFeature(arg->field_1c, arg->field_14);
    return 1;
}

// GLOBAL: LEGOLAND 0x004b7e38
void (*ReportGoalSetters[25])(unsigned int, unsigned int) = {
    (void (*)(unsigned int, unsigned int))SetReportLegolandZoneGoal,
    (void (*)(unsigned int, unsigned int))SetReportAdventurerZoneGoal,
    (void (*)(unsigned int, unsigned int))SetReportCastleZoneGoal,
    SetReportWesternZoneGoal,
    SetReportCastleScoreGoal,
    SetReportDrivingSchoolScoreGoal,
    SetReportLogFlumeScoreGoal,
    SetReportBoatingSchoolScoreGoal,
    SetReportJungleCruiseScoreGoal,
    SetReportType1Or3ObjectCountGoal,
    SetReportType1Or3ClassCountGoal,
    SetReportLinkedObjectPercentGoal,
    SetReportType2ObjectCountGoal,
    SetReportType2ClassCountGoal,
    SetReportSceneryCoverageGoal,
    SetReportFoodStallObjectCountGoal,
    SetReportFoodStallClassCountGoal,
    SetReportType4ObjectCountGoal,
    SetReportType4ClassCountGoal,
    SetReportVisitorCountGoal,
    SetReportVisitorHappinessGoal,
    SetReportVisitorFullnessGoal,
    SetReportPowerSupplyGoal,
    SetReportWorkingRidesGoal,
    SetReportMapTileCountGoal,
};

// FUNCTION: LEGOLAND 0x0046a140
void CallReportHandler(int index, unsigned int param_2, unsigned int param_3) {
    if (index >= 0 && index < 0x19) {
        ReportGoalSetters[index](param_3, param_2);
    }
}

// FUNCTION: LEGOLAND 0x0046a170
unsigned int ScriptEventReport(struct ObjectiveEvent *arg) {
    CallReportHandler(arg->field_18, arg->field_1c, arg->field_14);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a190
unsigned int GenerateWorkers(struct ObjectiveEvent *arg) {
    int i;

    SetBricksLimited(0);
    i = 0;
    if ((int)arg->field_1c > 0) {
        do {
            if (arg->field_14 != 0) {
                GenerateGardener(&arg->field_20, 0);
            } else {
                GenerateMechanic(&arg->field_20, 0);
            }
            i++;
        } while (i < (int)arg->field_1c);
    }
    SetBricksLimited(1);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a1f0
unsigned int ScriptEventWorkers(struct ObjectiveEvent *arg) {
    if ((int)arg->field_1c >= 0) {
        lpConfig->gardeners_enabled = (arg->field_1c != 0);
    }
    if ((int)arg->field_14 >= 0) {
        lpConfig->mechanics_enabled = (arg->field_14 != 0);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a230
unsigned int ScriptEventDegrade(struct ObjectiveEvent *arg) {
    struct Ride *group;
    struct ObjInstance *node;
    struct MapElement *tile;
    int value;
    int limit;
    unsigned char threshold;
    struct Point pt;

    group = ((struct Element *)arg->element)->ride;
    node = group->instances;
    value = (unsigned int)group->durability * arg->field_14;
    threshold = (char)(value / 100);
    limit = arg->field_1c;
    if (limit > (int)group->field_8 || limit == 0) {
        limit = group->field_8;
    }
    while (node != NULL && limit != 0) {
        pt.x = node->anchor.pos.x;
        pt.y = node->anchor.pos.y;
        if (pt.x >= 0 && pt.x < lpConfig->width && pt.y >= 0 && pt.y < lpConfig->height) {
            tile = &GameMap[pt.y][pt.x];
        } else {
            tile = NULL;
        }
        if (tile->durability_level > threshold) {
            tile->durability_level = threshold;
            DegradeObject(tile, &pt);
            ScriptDirtyCategories |= 0x200;
        }
        node = node->next;
        limit--;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a300
unsigned int ApplyCapacityEvent(struct ObjectiveEvent *arg) {
    if (arg->field_14 != 0) {
        MapStats.capacity_max = arg->field_1c;
    } else {
        MapStats.capacity_min = arg->field_1c;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a330
unsigned int ScriptEventCapacityScale(struct ObjectiveEvent *arg) {
    SetClassPercent(arg->field_14, arg->field_1c);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a350
unsigned int ScriptEventCapacityCap(struct ObjectiveEvent *arg) {
    SetClassLimit(arg->field_14, arg->field_1c);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a370
unsigned int NerpsSetEntranceFee(struct ObjectiveEvent *arg) {
    MapStats.entrance_fee = (short)arg->field_1c;
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a390
unsigned int ScriptEventEndLevel(void) {
    if (MapStats.timer_minutes == 0) {
        SetScriptStopped(1);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a3b0
unsigned int ScriptEventLookAt(struct ObjectiveEvent *arg) {
    int dimX;
    int dimY;
    int v24;
    int v20;
    struct Point pt;

    v24 = arg->field_24;
    v20 = arg->field_20;
    GetTileDimensions(&dimX, &dimY);
    pt.x = (v20 - v24) * dimX;
    pt.y = (v20 + v24) * dimY;
    pt.x >>= 9;
    pt.y >>= 9;
    ScrollX = (pt.x - (lpConfig->view_width >> 1)) * 0x100;
    ScrollY = (pt.y - (lpConfig->view_height >> 1)) * 0x100;
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a420
unsigned int ScriptEventThemeIcon(struct ObjectiveEvent *arg) {
    SetObjectiveCounter(arg->field_14, arg->field_1c);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a440
unsigned int ScriptEventAddFlag(struct ObjectiveEvent *arg) {
    AddObjectiveCounter(arg->field_14, arg->field_1c);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a460
unsigned int ScriptEventBridges(struct ObjectiveEvent *arg) {
    ScriptOpenBridge(arg->field_14, arg->field_1c);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a480
unsigned int ScriptEventBriefingFile(struct ObjectiveEvent *arg) {
    /* TODO: fold into NerpsArg — field_8 is used both as unsigned int (SetHintsFileName) and a string ptr here */
    SetBriefingFile(arg->text);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a4a0
unsigned int ScriptEventHintsFile(struct ObjectiveEvent *arg) {
    /* TODO: fold into NerpsArg — field_8 doubles as the name string pointer here */
    SetHintsFileName(arg->text);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a4c0
unsigned int ApplyFlashButtonEvent(struct ObjectiveEvent *arg) {
    SetButtonFlashMask(arg->field_1c, arg->field_14);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a4e0
unsigned int ScriptEventPurge(void) {
    ScriptPurgePending = 1;
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a4f0
unsigned int ScriptEventNeed(struct ObjectiveEvent *arg) {
    int count;

    /* TODO: fold into NerpsArg — field_4 is used both as a class_id (unsigned int)
       and, here, as the ObjCountWrap* that ObjCount() dereferences */
    count = ObjCount((struct Element *)arg->element);
    if (count < (int)arg->field_1c) {
        QueueBuildMoreObjectsHint(arg, arg->field_4, arg->field_1c - count);
        ScriptNeedRemaining = arg->field_1c - count;
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a540
unsigned int ScriptEventNeedAt(struct ObjectiveEvent *arg) {
    struct MapElement *tile;
    int x;
    int y;

    x = arg->field_20;
    if (x < 0 || x >= (int)(unsigned int)lpConfig->width || (y = arg->field_24, y < 0) ||
        y >= (int)(unsigned int)lpConfig->height) {
        tile = NULL;
    } else {
        tile = &GameMap[y][x];
    }
    if ((tile->flags & 0x80) != 0 && tile->field_0 == arg->element) {
        return 1;
    }
    QueueBuildMoreObjectsHint(arg, arg->field_4, 1);
    return 0;
}

/* Counts anchored instances of the requested object within the objective rectangle. Returns success at the target
 * count; otherwise queues a build-more hint. */
// FUNCTION: LEGOLAND 0x0046a5b0
unsigned int ScriptEventNeedIn(struct ObjectiveEvent *arg) {
    struct MapElement *tile;
    int x;
    int count;
    unsigned int y;

    count;
    count = 0;
    for (y = arg->field_2c; y <= arg->field_34; y++) {
        for (x = arg->field_28; x <= arg->field_30; x++) {
            if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                tile = &GameMap[y][x];
            } else {
                tile = NULL;
            }
            if ((0x80 & tile->flags) != 0 && tile->field_0 == arg->element && tile->field_4 == x && tile->field_5 == y) {
                ++count;
            }
        }
    }
    if (count >= (int)arg->field_1c) {
        return 1;
    }
    QueueBuildMoreObjectsHint(arg, arg->field_4, arg->field_1c - count);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046a690
unsigned int ScriptEventConnect(struct ObjectiveEvent *arg) {
    struct Ride *group;
    struct ObjInstance *node;
    struct MapElement *tile;
    int x;
    int y;

    group = ((struct Element *)arg->element)->ride;
    for (node = group->instances; node != NULL; node = node->next) {
        x = node->anchor.pos.x + group->x;
        y = node->anchor.pos.y + group->y;
        if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
            tile = &GameMap[y][x];
        } else {
            tile = NULL;
        }
        /* `!tile->flags & 0x10` is always false: operator precedence bug in the original */
        if (tile == NULL || !tile->flags & 0x10) {
            QueueConnectToPathHint(arg, arg->field_4);
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a730
unsigned int IsLinkableRideType(struct Ride *arg) {
    int value;

    value = arg->type;
    if (value == 1 || (value > 3 && value <= 5)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046a750
unsigned int ScriptEventLink(struct ObjectiveEvent *arg) {
    struct MapElement *robj;
    struct Ride *group;
    struct ObjInstance *node;
    struct MapElement *tile;
    int total;
    int blocked;
    Point pt;
    int ox;
    int oy;

    total = 0;
    blocked = 0;
    if (arg->field_4 != 0) {
        group = ((struct Element *)arg->element)->ride;
        node = group->instances;
        if (node == NULL) {
            goto ok;
        }
        do {
            ox = node->anchor.pos.x;
            oy = node->anchor.pos.y;
            pt.x = group->x + ox;
            pt.y = group->y + oy;
            tile = GetTileAtPoint(&pt);
            if (tile == NULL || ((tile->flags == 0) & 0x10)) {
                total++;
                blocked++;
            } else if (IsPathPointLinked(&pt) == 0) {
                total++;
            }
            node = node->next;
        } while (node != NULL);
    } else {
        robj = (struct MapElement *)GetFirstRenderObject();
        if (robj == NULL) {
            goto ok;
        }
        do {
            group = robj->field_0->ride;
            if (IsLinkableRideType(group) != 0) {
                ox = robj->field_4;
                oy = robj->field_5;
                pt.x = group->x + ox;
                pt.y = group->y + oy;
                tile = GetTileAtPoint(&pt);
                if (tile == NULL || ((tile->flags == 0) & 0x10)) {
                    total++;
                    blocked++;
                } else if (IsPathPointLinked(&pt) == 0) {
                    total++;
                }
            }
            robj = (struct MapElement *)GetNextRenderObject((MapElement *)robj);
        } while (robj != NULL);
    }
    if (blocked != 0) {
        QueueConnectToPathHint(arg, arg->field_4);
        return 0;
    }
    if (total != 0) {
        QueueLinkToEntranceHint(arg, arg->field_4);
        return 0;
    }
ok:
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a900
unsigned int ScriptEventRange(struct ObjectiveEvent *arg) {
    struct Ride *node;
    int count;
    int sum;

    node = ObjectClassList;
    count = 0;
    sum = 0;
    if (node != NULL) {
        do {
            if (node->theme_element == arg->element && node->field_8 != 0) {
                sum += node->field_8;
                count++;
            }
            node = node->next;
        } while (node != NULL);
    }
    if (count < (int)arg->field_14 || sum < (int)arg->field_1c) {
        QueueBuildRangeAttractionsHint(arg, arg->field_4, arg->field_14 - count, arg->field_1c - sum);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a960
unsigned int ScriptEventClearArea(struct ObjectiveEvent *arg) {
    struct MapElement *tile;
    int count;
    int x;
    int y;

    count = 0;
    for (y = arg->field_2c; y <= arg->field_34; y++) {
        for (x = arg->field_28; x <= arg->field_30; x++) {
            if (x >= 0 && x < lpConfig->width && y >= 0 && y < lpConfig->height) {
                tile = &GameMap[y][x];
            } else {
                tile = NULL;
            }
            if ((tile->flags & 0x80) != 0 && tile->field_4 == x && tile->field_5 == y) {
                count++;
            }
        }
    }
    if (count > (int)arg->field_14) {
        QueueClearAreaHint(arg, count - arg->field_14);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046aa30
unsigned int ScriptEventRemove(struct ObjectiveEvent *arg) {
    int count;

    count = ObjCount((struct Element *)arg->element);
    if (count > (int)arg->field_1c) {
        QueueDeleteObjectsHint(arg, arg->field_4, count - arg->field_1c);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046aa70
unsigned int ScriptEventRemoveRange(struct ObjectiveEvent *arg) {
    struct Ride *node;
    int count;
    int sum;

    node = ObjectClassList;
    count = 0;
    sum = 0;
    if (node != NULL) {
        do {
            if (node->theme_element == arg->element && node->field_8 != 0) {
                sum += node->field_8;
                count++;
            }
            node = node->next;
        } while (node != NULL);
    }
    if (arg->field_14 == 0xffffffff) {
        count |= arg->field_14;
    }
    if (arg->field_1c == 0xffffffff) {
        sum |= arg->field_1c;
    }
    if (count > (int)arg->field_14 || sum > (int)arg->field_1c) {
        QueueDeleteRangeAttractionsHint(arg, arg->field_4, count - arg->field_14, sum - arg->field_1c);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046aae0
unsigned int ScriptEventComposite(struct ObjectiveEvent *arg) {
    struct Ride *node;
    int count;
    int sum;

    node = ObjectClassList;
    if ((int)ObjCount((struct Element *)arg->element) < 1) {
        QueueBuildMoreObjectsHint(arg, arg->field_4, 1);
        return 0;
    }
    count = 0;
    sum = 0;
    if (node != NULL) {
        do {
            if (node->parent_element == arg->element && node->field_8 != 0) {
                sum += node->field_8;
                count++;
            }
            node = node->next;
        } while (node != NULL);
    }
    if (count >= (int)arg->field_14 && sum >= (int)arg->field_1c) {
        return 1;
    }
    QueueAddRidePartsHint(arg, arg->field_4, arg->field_1c - sum, arg->field_14 - count);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046ab70
unsigned int ScriptEventLoopComposite(struct ObjectiveEvent *arg) {
    struct Element *object;
    struct Ride *vtable;

    object = arg->element;
    vtable = object->ride;
    if (vtable->cb_c0 != NULL) {
        if (vtable->cb_c0(object, 1) < (int)arg->field_1c) {
            /* TODO: fold LoopArg/NerpsArg — same objective object, two struct views */
            QueueCustomObjectiveHintIfDue((struct ObjectiveEvent *)arg);
            return 0;
        }
    } else {
        // STRING: LEGOLAND 0x004ba7e4
        DBPrintf("Cannot evaluate LoopSize for %s\n", object->name);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046abc0
int ScriptEventTechLevel(struct ObjectiveEvent *param_1) {
    /* param_1 is a RewardArg* carried as unsigned int (table callback ABI) */
    return ProcessUnimplementedReward((struct ObjectiveEvent *)param_1);
}

// FUNCTION: LEGOLAND 0x0046abd0
unsigned int ScriptEventParkVisitors(struct ObjectiveEvent *arg) {
    if (ParkVisitorCount < (int)arg->field_1c) {
        QueueAttractVisitorsHint(arg, arg->field_1c - ParkVisitorCount);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046ac00
unsigned int ScriptEventRiders(struct ObjectiveEvent *arg) {
    struct RideNode *node;
    int count;

    count = 0;
    node = ((struct Element *)arg->element)->ride->riders;
    while (node != NULL) {
        count++;
        if (count >= (int)arg->field_1c) {
            return 1;
        }
        node = node->next;
    }
    QueueRideVisitorsHint(arg, arg->field_4, arg->field_1c - count);
    return 0;
}

struct ClassListNode {
    struct ClassListNode *next;
    unsigned char pad_4[0x8 - 0x4];
    struct Ride *iface;
};

// FUNCTION: LEGOLAND 0x0046ac50
unsigned int ScriptEventRideVisitors(struct ObjectiveEvent *arg) {
    struct ClassListNode *node;
    struct MapElement *robj;
    int count;
    unsigned int value;
    struct Point pt;

    count = 0;
    node = (struct ClassListNode *)((struct Element *)arg->element)->ride->instances;
    while (node != NULL) {
        if (_stricmp(*(char **)node->iface->element, *(char **)arg->element) == 0 &&
            (robj = (struct MapElement *)GetFirstObjectMatching((Element *)node->iface->element)) != NULL) {
            pt.x = robj->field_4;
            pt.y = robj->field_5;
            value = GetObjInstanceCount(&pt) & 0xffff;
            if ((int)value >= (int)arg->field_1c) {
                return 1;
            }
            if ((int)value > count) {
                count = value;
            }
        }
        node = node->next;
    }
    QueueRideVisitorsHint(arg, arg->field_4, arg->field_1c - count);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046ad00
unsigned int ScriptEventSceneryCoverage(struct ObjectiveEvent *arg) {
    if (SceneryCoveredSquares < (int)arg->field_14) {
        QueueCoverSquaresHint(arg, 2, arg->field_14 - SceneryCoveredSquares);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046ad30
unsigned int ScriptEventPathScenery(struct ObjectiveEvent *arg) {
    UpdatePathSceneryPercent();
    if (PathSceneryPercent < (int)arg->field_14) {
        QueuePathSceneryHint(arg, arg->field_14 - PathSceneryPercent);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046ad60
unsigned int ScriptEventRideCoverage(struct ObjectiveEvent *arg) {
    if (RideCoveredSquares < (int)arg->field_14) {
        QueueCoverSquaresHint(arg, 1, arg->field_14 - RideCoveredSquares);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046ad90
unsigned int ScriptEventShopCoverage(struct ObjectiveEvent *arg) {
    if (ShopCoveredSquares < (int)arg->field_14) {
        QueueCoverSquaresHint(arg, 4, arg->field_14 - ShopCoveredSquares);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046adc0
unsigned int ScriptEventFoodCoverage(struct ObjectiveEvent *arg) {
    if (FoodCoveredSquares < (int)arg->field_14) {
        QueueCoverSquaresHint(arg, 5, arg->field_14 - FoodCoveredSquares);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046adf0
unsigned int ScriptEventTotalCoverage(struct ObjectiveEvent *arg) {
    if (TotalCoveredSquares < (int)arg->field_14) {
        QueueCoverSquaresHint(arg, 0, arg->field_14 - TotalCoveredSquares);
        return 0;
    }
    return 1;
}

/* Does nothing but return 1. */
// FUNCTION: LEGOLAND 0x0046ae20
unsigned int ReturnOne_0046ae20(void) {
    return 1;
}

// FUNCTION: LEGOLAND 0x0046ae30
int ScriptEventStudArea(struct ObjectiveEvent *param_1) {
    return ProcessUnimplementedReward((struct ObjectiveEvent *)param_1);
}

// FUNCTION: LEGOLAND 0x0046ae40
unsigned int ScriptEventSave(struct ObjectiveEvent *arg) {
    if (GetBrickCount() >= (int)arg->field_1c) {
        return 1;
    }
    QueueSaveCoinsHint(arg, arg->field_1c - GetBrickCount());
    return 0;
}

// FUNCTION: LEGOLAND 0x0046ae70
unsigned int ScriptEventHappiness(struct ObjectiveEvent *arg) {
    struct Bloke *bloke;
    int count;

    count = 0;
    bloke = FirstBloke;
    while (bloke != NULL) {
        if (bloke->mood < (int)arg->field_14) {
            bloke = bloke->next;
            continue;
        }
        count++;
        if (count >= (int)arg->field_1c) {
            return 1;
        }
        bloke = bloke->next;
    }
    QueueHappyVisitorsHint(arg, arg->field_1c - count, arg->field_14);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046aec0
unsigned int AdjustGardenerCount(struct ObjectiveEvent *arg) {
    int current;
    int target;

    current = GetGardenerCount();
    target = arg->field_1c;
    if (0 < target) {
        if (target <= current) {
            return 1;
        }
        QueueMoreGardenersHint(arg, target - current);
        return 0;
    }
    if (-target >= current) {
        return 1;
    }
    QueueFewerGardenersHint(arg, target + current);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046af10
unsigned int ScriptEventNeedMechanics(struct ObjectiveEvent *arg) {
    int current;
    int target;

    current = GetMechanicCount();
    target = arg->field_1c;
    if (0 < target) {
        if (target <= current) {
            return 1;
        }
        QueueMoreMechanicsHint(arg, target - current);
        return 0;
    }
    if (-target >= current) {
        return 1;
    }
    QueueFewerMechanicsHint(arg, target + current);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046af60
unsigned int ScriptEventHunger(struct ObjectiveEvent *arg) {
    struct Bloke *bloke;
    int count;

    count = 0;
    if (FirstBloke != NULL) {
        bloke = FirstBloke;
        do {
            if (arg->field_18 != 0) {
                if ((int)(unsigned int)bloke->field_7c >= (int)arg->field_14) {
                    count++;
                }
            } else if ((int)(unsigned int)bloke->field_7c <= (int)arg->field_14) {
                count++;
            }
            bloke = bloke->next;
        } while (bloke != NULL);
    }
    if (arg->field_18 != 0) {
        if (count > (int)arg->field_1c) {
            QueueFewerHungryVisitorsHint(arg, count - arg->field_1c, arg->field_14);
            return 0;
        }
    } else if (count < (int)arg->field_1c) {
        QueueMoreFedVisitorsHint(arg, arg->field_1c - count, arg->field_14);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046afe0
unsigned int ScriptEventFixRides(struct ObjectiveEvent *arg) {
    struct MapElement *robj;
    int count;
    struct Ride *group;
    int pct;

    count = 0;
    for (robj = (struct MapElement *)GetFirstRenderObject(); robj != NULL;
        robj = (struct MapElement *)GetNextRenderObject((MapElement *)robj)) {
        if (robj->durability_level != 0) {
            group = robj->field_0->ride;
            if (group->durability != 0) {
                pct = robj->durability_level * 100 / group->durability;
            } else {
                pct = 100;
            }
            if (arg->field_14 == 0x19 && (robj->flags & 4) != 0) {
                pct = 100;
            }
            if (pct < (int)arg->field_14) {
                count++;
            }
        }
    }
    if (count > (int)arg->field_1c) {
        QueueRepairObjectsHint(arg, count - arg->field_1c, arg->field_14);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046b080
unsigned int ScriptEventPowerRides(struct ObjectiveEvent *arg) {
    if (MapStats.unpowered_count > (int)arg->field_1c) {
        QueueRepairObjectsHint(arg, MapStats.unpowered_count - arg->field_1c, 0);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046b0b0
unsigned int ScriptEventZoning(void) {
    return 1;
}

// FUNCTION: LEGOLAND 0x0046b0c0
unsigned int ScriptEventCheckFlag(struct ObjectiveEvent *arg) {
    if (GetObjectiveCounter(arg->field_14) >= (int)arg->field_1c) {
        return 1;
    }
    if (IsObjectiveHintDue() != 0) {
        QueueCustomObjectiveHint(arg);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046b100
unsigned int ScriptEventSelectTheme(struct ObjectiveEvent *arg) {
    if (arg->field_1c == SelectedBuildTheme) {
        return 1;
    }
    if (IsObjectiveHintDue() != 0) {
        QueueCustomObjectiveHint(arg);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046b130
unsigned int ScriptEventSelectTab(struct ObjectiveEvent *arg) {
    if (SelectedBuildTheme != 5) {
        if (arg->field_1c != 0 && ResearchTabSelected != 0) {
            return 1;
        }
        if (arg->field_1c == 0 && ResearchTabSelected == 0) {
            return 1;
        }
    }
    if (IsObjectiveHintDue() != 0) {
        QueueCustomObjectiveHint(arg);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046b180
unsigned int ScriptEventSelectMode(struct ObjectiveEvent *arg) {
    if (arg->field_1c == 3) {
        if (EditMode.unk0 == 1 && EditMode.unk8 == PathControlObject) {
            return 1;
        }
    } else if (EditMode.unk0 == arg->field_1c) {
        return 1;
    }
    if (IsObjectiveHintDue() == 0) {
        return 0;
    }
    QueueCustomObjectiveHint(arg);
    return 0;
}

/* Only forwards to ProcessUnimplementedObjective. */
// FUNCTION: LEGOLAND 0x0046b1e0
int CallProcessUnimplementedObjective(struct ObjectiveEvent *param_1) {
    return ProcessUnimplementedObjective((struct ObjectiveEvent *)param_1);
}

// FUNCTION: LEGOLAND 0x0046b1f0
unsigned int ScriptEventForever(void) {
    return 0;
}

// FUNCTION: LEGOLAND 0x0046b200
unsigned int IsScriptEventDue(struct ObjectiveEvent *event) {
    if (ScriptSectionStarted == 0) {
        if ((ScriptEventCategories[event->type] & ScriptDirtyCategories) == 0) {
            if ((int)(ScriptEvalTime - event->timestamp) <= 0x1388) {
                return 0;
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046b240
void SetScriptStopped(unsigned int param_1) {
    MapStats.script_stopped = param_1;
    UpdateScriptEndIcon((void *)1);
    if (param_1 != 0) {
        MarkLevelComplete();
        if (MapStats.field_3a8 != 0) {
            return;
        }
        EndLevel(1);
    }
}

// FUNCTION: LEGOLAND 0x0046b280
unsigned int IsScriptStopped(void) {
    return MapStats.script_stopped;
}

// FUNCTION: LEGOLAND 0x0046b290
void FreePurgedScriptEvents(void) {
    struct ObjectiveEvent *node;
    struct ObjectiveEvent *prev;
    struct ObjectiveEvent *next;

    prev = NULL;
    node = ScriptEventList;
    while (node != NULL) {
        next = node->next;
        if ((node->flags_10 & 8) != 0) {
            if (prev != NULL) {
                prev->next = next;
            } else {
                ScriptEventList = next;
            }
            FreeObjectiveEvent(node);
            node = prev;
        }
        prev = node;
        node = next;
    }
}

typedef int (*EventHandler)(struct ObjectiveEvent *);

// GLOBAL: LEGOLAND 0x004b9d44
EventHandler ScriptEventHandlers[70] = {
    NULL,
    NULL,
    (EventHandler)ScriptEventGive,
    (EventHandler)ScriptEventMakeResearchable,
    (EventHandler)ScriptEventTake,
    (EventHandler)ObjectiveEventAddBricks,
    (EventHandler)ObjectiveEventSetBricks,
    (EventHandler)ScriptEventPlace,
    (EventHandler)ScriptEventClear,
    (EventHandler)ScriptEventUnglue,
    (EventHandler)ScriptEventGlue,
    (EventHandler)ScriptEventExtendPark,
    (EventHandler)ScriptEventFmv,
    (EventHandler)ScriptEventInterval,
    (EventHandler)ScriptEventMessage,
    (EventHandler)ScriptEventFeature,
    (EventHandler)GenerateWorkers,
    (EventHandler)ScriptEventWorkers,
    (EventHandler)ScriptEventDegrade,
    (EventHandler)ApplyCapacityEvent,
    (EventHandler)ScriptEventCapacityScale,
    (EventHandler)ScriptEventCapacityCap,
    (EventHandler)NerpsSetEntranceFee,
    (EventHandler)ScriptEventLookAt,
    (EventHandler)ScriptEventReport,
    (EventHandler)ScriptEventThemeIcon,
    (EventHandler)ScriptEventAddFlag,
    (EventHandler)ScriptEventBridges,
    (EventHandler)ScriptEventBriefingFile,
    (EventHandler)ScriptEventHintsFile,
    (EventHandler)ApplyFlashButtonEvent,
    (EventHandler)ScriptEventPurge,
    (EventHandler)ScriptEventEndLevel,
    (EventHandler)ScriptEventNeed,
    (EventHandler)ScriptEventNeedAt,
    (EventHandler)ScriptEventNeedIn,
    (EventHandler)ScriptEventConnect,
    (EventHandler)ScriptEventLink,
    (EventHandler)ScriptEventRange,
    (EventHandler)ScriptEventClearArea,
    (EventHandler)ScriptEventRemove,
    (EventHandler)ScriptEventRemoveRange,
    (EventHandler)ScriptEventComposite,
    (EventHandler)ScriptEventLoopComposite,
    (EventHandler)ScriptEventTechLevel,
    (EventHandler)ScriptEventParkVisitors,
    (EventHandler)ScriptEventRideVisitors,
    (EventHandler)ScriptEventRiders,
    (EventHandler)ScriptEventSceneryCoverage,
    (EventHandler)ScriptEventPathScenery,
    (EventHandler)ScriptEventRideCoverage,
    (EventHandler)ScriptEventShopCoverage,
    (EventHandler)ScriptEventFoodCoverage,
    (EventHandler)ScriptEventTotalCoverage,
    (EventHandler)ReturnOne_0046ae20,
    (EventHandler)ScriptEventStudArea,
    (EventHandler)ScriptEventSave,
    (EventHandler)ScriptEventHappiness,
    (EventHandler)AdjustGardenerCount,
    (EventHandler)ScriptEventNeedMechanics,
    (EventHandler)ScriptEventHunger,
    (EventHandler)ScriptEventFixRides,
    (EventHandler)ScriptEventPowerRides,
    (EventHandler)ScriptEventZoning,
    (EventHandler)ScriptEventCheckFlag,
    (EventHandler)ScriptEventSelectTheme,
    (EventHandler)ScriptEventSelectTab,
    (EventHandler)ScriptEventSelectMode,
    (EventHandler)CallProcessUnimplementedObjective,
    (EventHandler)ScriptEventForever,
};

// FUNCTION: LEGOLAND 0x0046b2d0
void RunLevelScript(void) {
    struct ObjectiveEvent *node;
    struct ObjectiveEvent *next;
    struct ObjectiveEvent *prev;
    int active;
    int changed;

    node = ScriptEventList;
    prev = NULL;
    active = 0;
    changed = 0;
    ClearNewObjectsList();
    if (IsScriptStopped() != 0) {
        return;
    }
    for (;;) {
        if ((ScriptDirtyCategories & 0x10) != 0) {
            UpdatePathLinks(1);
        }
        ScriptEvalTime = GetGameTimer();
        while (node != NULL) {
            next = node->next;
            if (ScriptEventHandlers[node->type] != NULL) {
                if (IsScriptEventDue((struct ObjectiveEvent *)node) != 0) {
                    node->timestamp = ScriptEvalTime;
                    if (ScriptEventHandlers[node->type](node) != 0) {
                        if ((node->flags_10 & 7) == 0) {
                            if (prev != NULL) {
                                prev->next = node->next;
                            } else {
                                ScriptEventList = node->next;
                            }
                            FreeObjectiveEvent(node);
                            node = prev;
                        } else {
                            node->flags_10 |= 0x80;
                        }
                    } else if ((node->flags_10 & 4) != 0) {
                        node->flags_10 |= 0x80;
                    } else {
                        active++;
                        node->flags_10 &= 0x7f;
                    }
                } else if ((node->flags_10 & 0x80) == 0) {
                    active++;
                }
            }
            prev = node;
            node = next;
        }
        if (BuildMenuRefreshPending != 0) {
            UpdateMenu();
            BuildMenuRefreshPending = 0;
            ShowNewObjectsInfo();
        }
        if (ScriptPurgePending != 0) {
            ScriptPurgePending = 0;
            FreePurgedScriptEvents();
        }
        if (ScriptSectionStarted != 0) {
            ScriptSectionStarted = 0;
            if (active != 0) {
                PostScriptSectionIntro(CurrentScriptSection, 1);
                ResetObjectiveHintTimer();
            }
        }
        if (ScriptSectionList != NULL && active == 0) {
            ClearAdvisorHelp();
            prev = NULL;
            node = ScriptEventList;
            while (node != NULL) {
                next = node->next;
                if ((node->flags_10 & 6) == 0) {
                    if (prev != NULL) {
                        prev->next = next;
                    } else {
                        ScriptEventList = next;
                    }
                    FreeObjectiveEvent(node);
                    node = prev;
                }
                prev = node;
                node = next;
            }
            if (CurrentScriptSection != 0) {
                AppendSectionRewards(CurrentScriptSection);
                RemoveScriptSection(CurrentScriptSection);
                FreeScriptSection(CurrentScriptSection);
                changed = 1;
            }
            CurrentScriptSection = ScriptSectionList;
            if (ScriptSectionList != NULL) {
                ScriptSectionStarted = 1;
                AppendObjectiveEventList((struct NerpsListNode *)ScriptSectionList);
                ResetObjectiveHintTimer();
            }
        }
        ScriptDirtyCategories = 0xffffffff;
        if (changed != 0) {
            node = ScriptEventList;
            prev = NULL;
            active = 0;
            changed = 0;
            ClearNewObjectsList();
            if (IsScriptStopped() == 0) {
                continue;
            }
            return;
        }
        ScriptDirtyCategories = 0x100;
        return;
    }
}

// FUNCTION: LEGOLAND 0x0046b4f0
struct NerpsListNode *AllocScriptSection(unsigned int param_1) {
    struct NerpsListNode *node;

    node = (struct NerpsListNode *)malloc(sizeof(struct NerpsListNode));
    if (node != NULL) {
        node->field_8 = 0;
        node->field_4 = param_1;
        node->field_c = 0;
        node->field_10 = 0;
        node->next = NULL;
    }
    return node;
}

// FUNCTION: LEGOLAND 0x0046b520
void FreeScriptSection(struct NerpsListNode *node) {
    if (node->field_c != NULL) {
        FreeObjectiveEventList(node->field_c);
    }
    if (node->field_10 != NULL) {
        FreeObjectiveEventList(node->field_10);
    }
    if (node->field_8 != NULL) {
        free(node->field_8);
    }
    free(node);
}

// FUNCTION: LEGOLAND 0x0046b560
void FreeScriptSectionList(struct NerpsListNode *node) {
    if (node != NULL) {
        if (node->next != NULL) {
            FreeScriptSectionList(node->next);
        }
        FreeScriptSection(node);
    }
}

// FUNCTION: LEGOLAND 0x0046b590
void InsertScriptSection(struct NerpsListNode *node) {
    struct NerpsListNode *prev;
    struct NerpsListNode *cur;

    prev = NULL;
    cur = ScriptSectionList;
    if (cur != NULL) {
        while (cur != NULL) {
            if (cur->field_4 >= node->field_4) {
                break;
            }
            prev = cur;
            cur = cur->next;
        }
        if (prev != NULL) {
            node->next = prev->next;
            prev->next = node;
            return;
        }
    }
    ScriptSectionList = node;
    node->next = NULL;
}

// FUNCTION: LEGOLAND 0x0046b5d0
int RemoveScriptSection(struct NerpsListNode *node) {
    struct NerpsListNode *cur;

    cur = ScriptSectionList;
    if (cur == node) {
        ScriptSectionList = node->next;
        return 1;
    }
    while (cur != NULL && cur->next != node) {
        cur = cur->next;
    }
    if (cur != NULL) {
        cur->next = node->next;
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046b610
void AddScriptEvent(struct ObjectiveEvent *node, struct NerpsListNode *list) {
    node->field_40 = ScriptPromptStringIndex;
    if (list->field_c != NULL) {
        node->next = list->field_c;
    }
    list->field_c = node;
}

// FUNCTION: LEGOLAND 0x0046b630
void InsertObjectiveEventAtHead(struct ObjectiveEvent *node, struct NerpsListNode *list) {
    struct ObjectiveEvent *head;

    head = list->field_10;
    if (head != NULL) {
        node->next = head;
    }
    list->field_10 = node;
}

// FUNCTION: LEGOLAND 0x0046b650
void SetScriptSectionIntro(const char *src, struct NerpsListNode *holder) {
    char *dst;

    if (holder->field_8 != NULL) {
        free(holder->field_8);
    }
    dst = (char *)malloc(strlen(src) + 1);
    holder->field_8 = dst;
    strcpy(dst, src);
}

// FUNCTION: LEGOLAND 0x0046b6b0
void PostScriptSectionIntro(struct NerpsListNode *ctx, unsigned int param_2) {
    struct ObjectiveEvent *node;

    if (ctx->field_8 != 0) {
        node = AllocObjectiveEvent(1, 2);
        SetObjectiveEventText(node, ctx->field_8, param_2);
        if (param_2 == 0) {
            ctx->field_8 = 0;
            node->flags_10 = 0x20;
        }
        InsertObjectiveMessageSorted(node);
        CurrentObjectivePromptIndex = 0;
    }
}

// FUNCTION: LEGOLAND 0x0046b700
unsigned int RepeatCurrentObjective(void) {
    if (CurrentScriptSection != 0) {
        if (CurrentObjectivePromptIndex != 0) {
            PostObjectiveMessage(PercentSFormat, ScriptStringTable[CurrentObjectivePromptIndex]);
            AdvisorRepeatPending = 1;
            return 1;
        }
        PostScriptSectionIntro(CurrentScriptSection, 1);
        ResetObjectiveHintTimer();
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046b760
unsigned int RepostScriptIntro(void) {
    if (IsScriptStopped() == 0) {
        if (CurrentScriptSection != 0) {
            PostScriptSectionIntro(CurrentScriptSection, 1);
            ResetObjectiveHintTimer();
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046b790
void NewScriptGiveEvent(struct Element *param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(2, 1);
    node->element = param_1;
    node->flags_10 = 0;
    node->field_14 = param_2;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046b7f0
void NewScriptTakeEvent(struct Element *param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(4, 1);
    node->flags_10 = 0;
    node->element = param_1;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046b820
void NewScriptAddBricksEvent(unsigned int param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(5, 1);
    node->flags_10 = 0;
    node->field_1c = param_1;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046b850
void NewScriptCurrencyEvent(unsigned int param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(6, 1);
    node->flags_10 = 0;
    node->field_1c = param_1;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046b880
void NewScriptPlaceEvent(struct Element *param_1, unsigned int *param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(7, 1);
    node->element = param_1;
    node->field_20 = param_2[0];
    node->field_24 = param_2[1];
    node->field_18 = param_3;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046b8c0
void NewScriptClearEvent(struct Vec4 *param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(8, 1);
    node->vec_28 = *param_1;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046b900
void NewScriptUnglueEvent(struct Vec4 *param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(9, 1);
    node->vec_28 = *param_1;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046b940
void NewScriptGlueEvent(struct Vec4 *param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0xa, 1);
    node->vec_28 = *param_1;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046b980
void NewScriptExtendParkEvent(struct Vec4 *param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0xb, 1);
    node->vec_28 = *param_1;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046b9c0
void NewScriptFmvEvent(char *param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0xc, 1);
    SetObjectiveEventText(node, param_1, 1);
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046b9f0
void NewScriptIntervalEvent(char *param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0xd, 1);
    SetObjectiveEventText(node, param_1, 1);
    node->field_1c = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046ba30
void NewScriptMessageEvent(char *param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0xe, 1);
    SetObjectiveEventText(node, param_1, 1);
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046ba60
void NewScriptFeatureEvent(unsigned int param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0xf, 1);
    node->field_1c = param_1;
    node->field_14 = param_2;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046ba90
void NewScriptReportEvent(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x18, 1);
    node->field_18 = param_1;
    node->field_1c = param_2;
    node->field_14 = param_3;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bad0
void NewScriptWorkerEvent(unsigned int param_1, unsigned int param_2, unsigned int *param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x10, 1);
    node->field_1c = param_2;
    node->field_20 = param_3[0];
    node->field_24 = param_3[1];
    node->field_14 = param_1;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bb10
void NewScriptWorkersEvent(unsigned int param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x11, 1);
    node->field_1c = param_1;
    node->field_14 = param_2;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bb40
void NewScriptDegradeEvent(struct Element *param_1, unsigned int param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x12, 1);
    node->element = param_1;
    node->field_14 = param_2;
    node->field_1c = param_3;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bb80
void NewScriptCapacityEvent(unsigned int param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x13, 1);
    node->field_1c = param_2;
    node->field_14 = param_1;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bbb0
void NewScriptCapacityScaleEvent(unsigned int param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x14, 1);
    node->field_1c = param_2;
    node->field_14 = param_1;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bbe0
void NewScriptCapacityCapEvent(unsigned int param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x15, 1);
    node->field_1c = param_2;
    node->field_14 = param_1;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bc10
void NewScriptEntranceFeeEvent(unsigned int param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x16, 1);
    node->flags_10 = 0;
    node->field_1c = param_1;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bc40
void NewScriptEndLevelEvent(void) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x20, 1);
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bc60
void NewScriptPurgeEvent(void) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x1f, 1);
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bc80
void NewScriptThemeIconEvent(unsigned int param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x19, 1);
    node->field_1c = param_2;
    node->field_14 = param_1;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bcb0
void NewScriptAddFlagEvent(unsigned int param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x1a, 1);
    node->field_1c = param_2;
    node->field_14 = param_1;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bce0
void NewScriptBridgesEvent(unsigned int param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x1b, 1);
    node->field_1c = param_2;
    node->field_14 = param_1;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bd10
void NewScriptBriefingFileEvent(char *param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x1c, 1);
    SetObjectiveEventText(node, param_1, 1);
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bd40
void NewScriptHintsFileEvent(char *param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x1d, 1);
    SetObjectiveEventText(node, param_1, 1);
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bd70
void NewScriptFlashButtonEvent(unsigned int param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x1e, 1);
    node->field_1c = param_1;
    node->field_14 = param_2;
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bda0
void NewScriptLookAtEvent(unsigned int *param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x17, 1);
    node->field_20 = param_1[0];
    node->field_24 = param_1[1];
    node->flags_10 = 0;
    InsertObjectiveEventAtHead(node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bdd0
void NewScriptNeedEvent(unsigned char param_1, struct Element *param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x21, 1);
    node->element = param_2;
    node->field_1c = param_3;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046be00
void NewScriptNeedAtEvent(unsigned char param_1, struct Element *param_2, unsigned int *param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x22, 1);
    node->element = param_2;
    node->field_20 = param_3[0];
    node->field_24 = param_3[1];
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046be40
void NewScriptNeedInEvent(unsigned char param_1, struct Element *param_2, unsigned int param_3, struct Vec4 *param_4) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x23, 1);
    node->element = param_2;
    node->field_1c = param_3;
    node->vec_28 = *param_4;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046be90
void NewScriptConnectEvent(unsigned char param_1, struct Element *param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x24, 1);
    node->element = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bec0
void NewScriptLinkEvent(unsigned char param_1, struct Element *param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x25, 1);
    node->element = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bef0
void NewScriptRangeEvent(unsigned char param_1, struct Element *param_2, unsigned int param_3, unsigned int param_4) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x26, 1);
    node->element = param_2;
    node->field_1c = param_3;
    node->field_14 = param_4;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bf30
void NewScriptClearAreaEvent(unsigned char param_1, struct Vec4 *param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x27, 1);
    node->vec_28 = *param_2;
    node->field_14 = param_3;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bf80
void NewScriptRemoveEvent(unsigned char param_1, struct Element *param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x28, 1);
    node->element = param_2;
    node->field_1c = param_3;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bfb0
void NewScriptRemoveRangeEvent(unsigned char param_1, struct Element *param_2, unsigned int param_3, unsigned int param_4) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x29, 1);
    node->element = param_2;
    node->field_1c = param_3;
    node->field_14 = param_4;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046bff0
void NewScriptCompositeEvent(unsigned char param_1, struct Element *param_2, unsigned int param_3, unsigned int param_4) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x2a, 1);
    node->element = param_2;
    node->field_1c = param_3;
    node->field_14 = param_4;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c030
void NewScriptLoopCompositeEvent(unsigned char param_1, struct Element *param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x2b, 1);
    node->element = param_2;
    node->field_1c = param_3;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c060
void NewScriptTechLevelEvent(unsigned char param_1, struct Element *param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x2c, 1);
    node->element = param_2;
    node->field_18 = param_3;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c090
void NewScriptParkVisitorsEvent(unsigned char param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x2d, 1);
    node->field_1c = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c0c0
void NewScriptRideVisitorsEvent(unsigned char param_1, struct Element *param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x2e, 1);
    node->element = param_2;
    node->field_1c = param_3;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c0f0
void NewScriptRidersEvent(unsigned char param_1, struct Element *param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x2f, 1);
    node->element = param_2;
    node->field_1c = param_3;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c120
void NewScriptSceneryCoverageEvent(unsigned char param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x30, 1);
    node->field_14 = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c150
void NewScriptPathSceneryEvent(unsigned char param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x31, 1);
    node->field_14 = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c180
void NewScriptRideCoverageEvent(unsigned char param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x32, 1);
    node->field_14 = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c1b0
void NewScriptShopCoverageEvent(unsigned char param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x33, 1);
    node->field_14 = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c1e0
void NewScriptFoodCoverageEvent(unsigned char param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x34, 1);
    node->field_14 = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c210
void NewScriptTotalCoverageEvent(unsigned char param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x35, 1);
    node->field_14 = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c240
void NewScriptStudAreaEvent(unsigned char param_1, struct Vec4 *param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x37, 1);
    node->vec_28 = *param_2;
    node->field_14 = param_3;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c290
void NewScriptSaveEvent(unsigned char param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x38, 1);
    node->field_1c = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c2c0
void NewScriptHappinessEvent(unsigned char param_1, unsigned int param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x39, 1);
    node->field_1c = param_2;
    node->field_14 = param_3;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c2f0
void NewScriptNeedGardenersEvent(unsigned char param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x3a, 1);
    node->field_1c = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c320
void NewScriptNeedMechanicsEvent(unsigned char param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x3b, 1);
    node->field_1c = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c350
void NewScriptHungerEvent(unsigned char param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x3c, 1);
    node->field_14 = param_3;
    node->field_1c = param_2;
    node->field_18 = param_4;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c390
void NewScriptFixRidesEvent(unsigned char param_1, unsigned int param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x3d, 1);
    node->field_1c = param_2;
    node->field_14 = param_3;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c3c0
void NewScriptPowerRidesEvent(unsigned char param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x3e, 1);
    node->field_1c = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c3f0
void NewScriptZoningEvent(unsigned char param_1, unsigned int param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x3f, 1);
    node->field_1c = param_2;
    node->field_14 = param_3;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c420
void NewScriptCheckFlagEvent(unsigned char param_1, unsigned int param_2, unsigned int param_3) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x40, 1);
    node->field_1c = param_3;
    node->field_14 = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c450
void NewScriptSelectThemeEvent(unsigned char param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x41, 1);
    node->field_1c = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c480
void NewScriptSelectTabEvent(unsigned char param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x42, 1);
    node->field_1c = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c4b0
void NewScriptSelectModeEvent(unsigned char param_1, unsigned int param_2) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x43, 1);
    node->field_1c = param_2;
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c510
void NewScriptForeverEvent(unsigned char param_1) {
    struct ObjectiveEvent *node;

    node = AllocObjectiveEvent(0x45, 1);
    node->flags_10 = param_1;
    AddScriptEvent((struct ObjectiveEvent *)node, CurrentScriptSection);
}

// FUNCTION: LEGOLAND 0x0046c540
void AppendObjectiveEventList(struct NerpsListNode *arg) {
    struct ObjectiveEvent *cur;

    cur = ScriptEventList;
    if (cur != NULL) {
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = arg->field_c;
        arg->field_c = NULL;
    } else {
        ScriptEventList = arg->field_c;
        arg->field_c = NULL;
    }
}

// FUNCTION: LEGOLAND 0x0046c580
void AppendSectionRewards(struct NerpsListNode *arg) {
    struct ObjectiveEvent *cur;

    cur = ScriptEventList;
    if (cur != NULL) {
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = arg->field_10;
        arg->field_10 = NULL;
    } else {
        ScriptEventList = arg->field_10;
        arg->field_10 = NULL;
    }
}

// FUNCTION: LEGOLAND 0x0046c5c0
void FreeAllScriptLists(void) {
    FreeObjectiveEventList(ObjectiveEventList);
    ObjectiveEventList = NULL;
    FreeObjectiveEventList(ObjectiveMessageQueue);
    ObjectiveMessageQueue = NULL;
    FreeObjectiveEventList(ScriptEventList);
    ScriptEventList = NULL;
    FreeScriptSectionList((struct NerpsListNode *)ScriptSectionList);
    ScriptSectionList = NULL;
    CurrentScriptSection = 0;
}

// FUNCTION: LEGOLAND 0x0046c620
unsigned int SaveGameWriteString(char *str) {
    int len;

    if (str != NULL) {
        len = strlen(str);
        if (SaveGameWrite(&len, 4) == 0) {
            return 0;
        }
        if (len != 0 && SaveGameWrite(str, len) == 0) {
            return 0;
        }
    } else {
        len = -1;
        if (SaveGameWrite(&len, 4) == 0) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046c680
void *ReadSaveString(void) {
    int len;
    char *buffer; /* a string: len bytes, then the 0 added here */

    if (SaveGameRead(&len, 4) == 0) {
        ScriptLoadErrorCount++;
        return NULL;
    }
    if (len == -1) {
        return NULL;
    }
    buffer = malloc(len + 1);
    if (SaveGameRead(buffer, len) == 0) {
        free(buffer);
        ScriptLoadErrorCount++;
        return NULL;
    }
    buffer[len] = 0;
    return buffer;
}

// FUNCTION: LEGOLAND 0x0046c700
unsigned int SaveObjectiveEventList(struct ObjectiveEvent *node) {
    unsigned int marker[17];

    while (node != NULL) {
        int ok;

        node->timestamp -= ScriptTimeBase;
        // STRING: LEGOLAND 0x004ba808
        DBPrintf("Saving Event, Timestamp = %d\n", node->timestamp);
        ok = SaveGameWrite(node, sizeof(struct ObjectiveEvent));
        node->timestamp += ScriptTimeBase;
        if (ok == 0) {
            return 0;
        }
        if (node->field_4 != 0) {
            if (SaveGameWriteString(*(char **)node->element) == 0) {
                return 0;
            }
        } else if (SaveGameWriteString(NULL) == 0) {
            return 0;
        }
        if (node->field_8 != 0) {
            if (SaveGameWriteString(node->text) == 0) {
                return 0;
            }
        } else if (SaveGameWriteString(NULL) == 0) {
            return 0;
        }
        node = node->next;
    }
    memset(marker, 0, sizeof(marker));
    marker[0] = 0xffffffff;
    return SaveGameWrite(marker, 0x44) != 0;
}

// FUNCTION: LEGOLAND 0x0046c7e0
struct ObjectiveEvent *LoadObjectiveEventList(void) {
    struct ObjectiveEvent *node;
    struct ObjectiveEvent *prev;
    struct ObjectiveEvent *head;
    char *str;

    head = NULL;
    prev = NULL;
    node = AllocObjectiveEvent((unsigned int)(UINT_PTR)prev, (int)(INT_PTR)prev);
    while (SaveGameRead(node, sizeof(struct ObjectiveEvent)) != 0) {
        if ((unsigned int)(UINT_PTR)node->next == 0xffffffff) {
            free(node);
            if (prev != NULL) {
                prev->next = NULL;
                return head;
            }
            return NULL;
        }
        // STRING: LEGOLAND 0x004ba838
        DBPrintf("Loading Event, TimeStamp = %d", node->timestamp);
        node->timestamp += ScriptTimeBase;
        // STRING: LEGOLAND 0x004ba828
        DBPrintf("Fixed up to %d\n", node->timestamp);
        str = (char *)ReadSaveString();
        if (ScriptLoadErrorCount != 0) {
            free(node);
            FreeObjectiveEventList(head);
            return NULL;
        }
        if (str != NULL) {
            node->element = ElemID(str);
            free(str);
        } else {
            node->field_4 = 0;
        }
        node->text = ReadSaveString();
        if (node->field_8 != 0) {
            node->flags_10 |= 0x20;
        }
        if (ScriptLoadErrorCount != 0) {
            FreeObjectiveEvent(node);
            FreeObjectiveEventList(head);
            return NULL;
        }
        if (head == NULL) {
            head = node;
        } else {
            prev->next = node;
        }
        prev = node;
        node = AllocObjectiveEvent(0, 0);
    }
    free(node);
    FreeObjectiveEventList(head);
    ScriptLoadErrorCount++;
    return NULL;
}

// FUNCTION: LEGOLAND 0x0046c920
unsigned int SaveScripts(void) {
    struct NerpsListNode *node;
    int scratch;
    int i;

    ScriptLoadErrorCount = 0;
    ScriptTimeBase = GetGameTimer();
    if (SaveObjectiveIcons() == 0) {
        return 0;
    }
    if (SaveGameWrite(BriefingFileName, 0x80) == 0) {
        return 0;
    }
    if (SaveGameWrite(HintsFileName, 0x80) == 0) {
        return 0;
    }
    if (SaveGameWrite(&ScriptStringCount, 4) == 0) {
        return 0;
    }
    for (i = 0; i < (int)ScriptStringCount; i++) {
        if (SaveGameWriteString(ScriptStringTable[i]) == 0) {
            return 0;
        }
    }
    scratch = ScriptTimeBase - ObjectiveHintTimer;
    if (SaveGameWrite(&scratch, 4) == 0) {
        return 0;
    }
    i = 0xa;
    if (SaveGameWrite(&i, 4) == 0) {
        return 0;
    }
    if (SaveGameWrite(ObjectiveCounters, 0xa) == 0) {
        return 0;
    }
    if (SaveObjectiveEventList(ScriptEventList) == 0) {
        return 0;
    }
    for (node = (struct NerpsListNode *)ScriptSectionList; node != NULL; node = node->next) {
        if (SaveGameWrite(&node->field_4, 4) == 0) {
            return 0;
        }
        if (SaveObjectiveEventList((struct ObjectiveEvent *)node->field_c) == 0) {
            return 0;
        }
        if (SaveObjectiveEventList((struct ObjectiveEvent *)node->field_10) == 0) {
            return 0;
        }
        if (SaveGameWriteString(node->field_8) == 0) {
            return 0;
        }
    }
    i = 0xffffffff;
    if (SaveGameWrite(&i, 4) == 0) {
        return 0;
    }
    if (CurrentScriptSection != 0) {
        scratch = (int)(CurrentScriptSection - ScriptSectionList) + 1;
    } else {
        scratch = 0;
    }
    return SaveGameWrite(&scratch, 4) != 0;
}

// FUNCTION: LEGOLAND 0x0046cb20
unsigned int UnloadLevelData(void) {
    CurrentObjectivePromptIndex = 0;
    if (LevelLoadedFromSave != 0) {
        UnloadSaveGameMap();
    } else {
        UnloadBaseMap();
    }
    FreeScriptStrings();
    FreeAllScriptLists();
    ClearAdvisorHelp();
    ResetAdvisorMessageTimers();
    return 1;
}

// FUNCTION: LEGOLAND 0x0046cb60
unsigned int LoadScripts(void) {
    struct NerpsListNode *node;
    struct NerpsListNode *prev;
    int i;
    int marker;
    int base;
    unsigned char skip;

    ScriptLoadErrorCount = 0;
    ScriptTimeBase = GetGameTimer();
    if (ScriptLoadErrorCount != 0) {
        return 0;
    }
    if (LoadObjectiveIcons() == 0) {
        return 0;
    }
    if (SaveGameRead(BriefingFileName, 0x80) == 0) {
        return 0;
    }
    if (SaveGameRead(HintsFileName, 0x80) == 0) {
        return 0;
    }
    if (SaveGameRead(&ScriptStringCount, 4) == 0) {
        return 0;
    }
    for (i = 0; i < (int)ScriptStringCount; i++) {
        ScriptStringTable[i] = (char *)ReadSaveString();
        if (ScriptLoadErrorCount != 0) {
            return 0;
        }
    }
    if (SaveGameRead(&base, 4) == 0) {
        return 0;
    }
    ObjectiveHintTimer = ScriptTimeBase + base;
    CurrentObjectivePromptIndex = 0;
    UpdateScriptEndIcon((void *)1);
    if (SaveGameRead(&i, 4) == 0) {
        return 0;
    }
    if (i > 0xa) {
        if (SaveGameRead(ObjectiveCounters, 10) == 0) {
            return 0;
        }
        for (i -= 10; i != 0; i--) {
            if (SaveGameRead(&skip, 1) == 0) {
                return 0;
            }
        }
    } else {
        ClearObjectiveCounters();
        Empty_004688e0();
        if (SaveGameRead(ObjectiveCounters, i) == 0) {
            return 0;
        }
    }
    ScriptEventList = LoadObjectiveEventList();
    if (ScriptLoadErrorCount != 0) {
        return 0;
    }
    prev = NULL;
    if (SaveGameRead(&marker, 4) == 0) {
        return 0;
    }
    while (marker != -1) {
        node = AllocScriptSection(marker);
        if (node == NULL) {
            return 0;
        }
        node->field_c = LoadObjectiveEventList();
        if (ScriptLoadErrorCount != 0) {
            return 0;
        }
        node->field_10 = LoadObjectiveEventList();
        if (ScriptLoadErrorCount != 0) {
            return 0;
        }
        node->field_8 = ReadSaveString();
        if (ScriptLoadErrorCount != 0) {
            return 0;
        }
        if (prev != NULL) {
            prev->next = node;
        } else {
            ScriptSectionList = node;
        }
        if (SaveGameRead(&marker, 4) == 0) {
            return 0;
        }
        prev = node;
    }
    if (SaveGameRead(&base, 4) == 0) {
        return 0;
    }
    if (base != 0) {
        CurrentScriptSection = (ScriptSectionList + base - 1);
        return 1;
    }
    CurrentScriptSection = 0;
    return 1;
}

/* Resets objective hint timing, the advisor clear time, and the last level-script polling time when unloading level
 * data. */
// FUNCTION: LEGOLAND 0x0046ce00
void ResetAdvisorMessageTimers(void) {
    ResetObjectiveHintTimer();
    AdvisorHelpClearTime = 0;
    LastLevelScriptRunTime = 0;
}
