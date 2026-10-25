#pragma once

struct Element;

struct Vec4 {
    unsigned int x;
    unsigned int y;
    unsigned int z;
    unsigned int w;
};

/* Canonical objective-event node (~0x44 bytes), allocated by AllocObjectiveEvent.
 * objectives.c owns the allocator and treats it as ObjectiveEvent; nerps.c
 * previously viewed the same object as EventNode/EventNodeVec. Unified here so
 * the boundary casts disappear. */
struct ObjectiveEvent {
    struct ObjectiveEvent *next;
    union {
        unsigned int field_4;
        struct Element *element; /* the object class the event is about */
    };
    union {
        unsigned int field_8;
        char *text; /* a message, a file name */
    };
    unsigned int type;
    unsigned char flags_10;
    unsigned char pad_11[0x14 - 0x11];
    unsigned int field_14;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned int field_20;
    unsigned int field_24;
    union {
        struct Vec4 vec_28;
        struct {
            int field_28;
            int field_2c;
            int field_30;
            int field_34;
        };
    };
    int sort_key;
    unsigned int timestamp;
    unsigned int field_40;
};

void SetHintsFileName(char *name);
void ClearBriefingAndHintsFileNames(void);
void ClearObjectiveCounters(void);
void SetObjectiveCounter(int index, signed char value);
unsigned char AddObjectiveCounter(int index, unsigned char value);
char GetObjectiveCounter(int index);
void Empty_004688e0(void);
void ScriptOpenBridge(int index, unsigned char param_2);
struct ObjectiveEvent *AllocObjectiveEvent(unsigned int type, int sort_key);
void FreeObjectiveEvent(struct ObjectiveEvent *node);
void FreeObjectiveEventList(struct ObjectiveEvent *node);
void FreeScriptStrings(void);
unsigned int AddScriptString(char *param_1, char *param_2, int param_3);
void InsertObjectiveMessageSorted(struct ObjectiveEvent *node);
void SetObjectiveEventText(struct ObjectiveEvent *node, char *param_2, unsigned int param_3);
struct ObjectiveEvent *PostObjectiveMessage(const char *format, ...);
void ShowNextObjectiveMessage(void);
void ResetObjectiveHintTimer(void);
int IsObjectiveHintDue(void);
int QueueCustomObjectiveHint(struct ObjectiveEvent *arg);
void QueueBuildMoreObjectsHint(struct ObjectiveEvent *object, unsigned int a, int b);
void QueueConnectToPathHint(struct ObjectiveEvent *object, unsigned int a);
void QueueLinkToEntranceHint(struct ObjectiveEvent *object, unsigned int a);
void QueueClearAreaHint(struct ObjectiveEvent *object, int a);
void QueueBuildRangeAttractionsHint(struct ObjectiveEvent *arg, unsigned int class_id, int count, int sum);
void QueueDeleteRangeAttractionsHint(struct ObjectiveEvent *arg, unsigned int class_id, int count, int sum);
void QueueDeleteObjectsHint(struct ObjectiveEvent *arg, unsigned int class_id, int count);
void QueueAttractVisitorsHint(struct ObjectiveEvent *object, int count);
void QueueMoreGardenersHint(struct ObjectiveEvent *object, int a);
void QueueFewerGardenersHint(struct ObjectiveEvent *object, int a);
void QueueMoreMechanicsHint(struct ObjectiveEvent *object, unsigned int a);
void QueueFewerMechanicsHint(struct ObjectiveEvent *object, int a);
void QueueSaveCoinsHint(struct ObjectiveEvent *arg, int count);
void QueueFewerHungryVisitorsHint(struct ObjectiveEvent *object, unsigned int a, unsigned int b);
void QueueMoreFedVisitorsHint(struct ObjectiveEvent *object, unsigned int a, unsigned int b);
void QueueHappyVisitorsHint(struct ObjectiveEvent *object, int a, unsigned int b);
void QueueRepairObjectsHint(struct ObjectiveEvent *arg, int param_2, unsigned int param_3);
void QueueRideVisitorsHint(struct ObjectiveEvent *object, unsigned int a, unsigned int b);
void QueueAddRidePartsHint(struct ObjectiveEvent *arg, unsigned int class_id, int sum, int count);
void QueueCoverSquaresHint(struct ObjectiveEvent *object, unsigned int param_2, int count);
void QueuePathSceneryHint(struct ObjectiveEvent *object, int count);
void QueueCustomObjectiveHintIfDue(struct ObjectiveEvent *object);
void PostPendingObjectiveHints(void);
void PlaceObjectAtTile(struct Element *a, void *b);
int ScriptEventPlace(struct ObjectiveEvent *event);
int ScriptEventGive(struct ObjectiveEvent *event);
int ScriptEventMakeResearchable(struct ObjectiveEvent *event);
int ScriptEventTake(struct ObjectiveEvent *event);
int ObjectiveEventAddBricks(struct ObjectiveEvent *event);
int ObjectiveEventSetBricks(struct ObjectiveEvent *event);
int ScriptEventClear(struct ObjectiveEvent *arg);
int ScriptEventUnglue(struct ObjectiveEvent *arg);
int ScriptEventGlue(struct ObjectiveEvent *arg);
int ScriptEventExtendPark(struct ObjectiveEvent *arg);
int ScriptEventFmv(struct ObjectiveEvent *arg);
int ScriptEventInterval(struct ObjectiveEvent *arg);
int ScriptEventMessage(struct ObjectiveEvent *arg);
void TakeRewardObject(struct Element *object);
int ProcessUnimplementedReward(struct ObjectiveEvent *arg);
int ProcessUnimplementedObjective(struct ObjectiveEvent *arg);
void GiveRewardObject(struct Element *object, unsigned int a, unsigned int b);
