#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bricks.h"
#include "challenge.h"
#include "clipping.h"
#include "controller.h"
#include "debug_alloc.h"
#include "draw.h"
#include "gamemap.h"
#include "help.h"
#include "icon.h"
#include "interface.h"
#include "llidb.h"
#include "map_object.h"
#include "math.h"
#include "nerps.h"
#include "objectives.h"
#include "popupinfo.h"
#include "sound_music.h"
#include "sound_sfx.h"
#include "stream.h"
#include "string.h"
#include "tilemap.h"
#include "timer.h"
#include "title.h"
#include "worker.h"

#pragma intrinsic(strcpy, strlen)

struct SweepInstance {
    struct Element *object;
    unsigned char tile_x;
    unsigned char tile_y;
    unsigned char pad_6[0xc - 0x6];
    unsigned char flags_c;
};

// FUNCTION: LEGOLAND 0x00468810
void SetHintsFileName(char *name) {
    strncpy(HintsFileName, name, 0x80);
    HintsFileName[0x7f] = 0;
}

// FUNCTION: LEGOLAND 0x00468830
void ClearBriefingAndHintsFileNames(void) {
    HintsFileName[0] = 0;
    BriefingFileName[0] = 0;
}

// FUNCTION: LEGOLAND 0x00468840
void ClearObjectiveCounters(void) {
    *(unsigned int *)ObjectiveCounters = 0;
    *(unsigned int *)(ObjectiveCounters + 4) = 0;
    *(unsigned short *)(ObjectiveCounters + 8) = 0;
}

// FUNCTION: LEGOLAND 0x00468860
void SetObjectiveCounter(int index, signed char value) {
    if (index < 10) {
        ObjectiveCounters[index] = value;
        if (index < 4) {
            SetObjectiveIconShown(index, value);
        }
    }
}

// FUNCTION: LEGOLAND 0x00468890
unsigned char AddObjectiveCounter(int index, unsigned char value) {
    if (index < 10) {
        ObjectiveCounters[index] += value;
        return ObjectiveCounters[index];
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004688c0
char GetObjectiveCounter(int index) {
    if (index < 10) {
        return ObjectiveCounters[index];
    }
    return 0;
}

/* Empty in the original (does nothing). */
// FUNCTION: LEGOLAND 0x004688e0
void Empty_004688e0(void) {}

/* Script bridges event (ScriptEventBridges, game_util): OpenBridge(index) for a valid index (< 4). The second
 * argument is not used. */
// FUNCTION: LEGOLAND 0x004688f0
void ScriptOpenBridge(int index, unsigned char param_2) {
    if (index < 4) {
        OpenBridge(index);
    }
}

// FUNCTION: LEGOLAND 0x00468910
struct ObjectiveEvent *AllocObjectiveEvent(unsigned int type, int sort_key) {
    struct ObjectiveEvent *event;

    event = (struct ObjectiveEvent *)calloc(1, sizeof(struct ObjectiveEvent));
    if (event != NULL) {
        event->next = NULL;
        event->field_8 = 0;
        event->type = type;
        event->sort_key = sort_key;
    }
    return event;
}

// FUNCTION: LEGOLAND 0x00468940
void FreeObjectiveEvent(struct ObjectiveEvent *event) {
    if (event->flags_10 & 0x20) {
        if (event->field_8 != 0) {
            free(event->text);
        }
    }
    free(event);
}

// FUNCTION: LEGOLAND 0x00468970
void FreeObjectiveEventList(struct ObjectiveEvent *event) {
    if (event != NULL) {
        if (event->next != NULL) {
            FreeObjectiveEventList(event->next);
        }
        FreeObjectiveEvent(event);
    }
}

// FUNCTION: LEGOLAND 0x004689a0
void FreeScriptStrings(void) {
    int i;
    char **p;

    i = 0;
    if ((int)ScriptStringCount > 0) {
        p = ScriptStringTable;
        do {
            if (*p != 0) {
                free(*p);
                *p = 0;
            }
            i++;
            p++;
        } while (i < (int)ScriptStringCount);
    }
    ScriptStringCount = 0;
}

// FUNCTION: LEGOLAND 0x004689f0
unsigned int AddScriptString(char *param_1, char *param_2, int param_3) {
    void *buffer;

    if (param_3 != 0) {
        if (param_1 != NULL) {
            if (param_2 != NULL) {
                buffer = malloc(strlen(param_1) + strlen(param_2) + 2);
                ScriptStringTable[ScriptStringCount] = (char *)buffer;
                if (buffer != NULL) {
                    // STRING: LEGOLAND 0x004b9f90
                    sprintf(buffer, "%s%c%s", param_1, 0x40, param_2);
                }
            } else {
                buffer = malloc(strlen(param_1) + 1);
                ScriptStringTable[ScriptStringCount] = (char *)buffer;
                if (buffer != NULL) {
                    sprintf(buffer, (char *)PercentSFormat, param_1);
                }
            }
        } else {
            ScriptStringTable[ScriptStringCount] = 0;
        }
    } else {
        ScriptStringTable[ScriptStringCount] = (char *)param_1;
    }
    return ScriptStringCount++;
}

// FUNCTION: LEGOLAND 0x00468b00
void InsertObjectiveMessageSorted(struct ObjectiveEvent *event) {
    struct ObjectiveEvent *node;
    struct ObjectiveEvent *prev;
    int key;

    node = ObjectiveMessageQueue;
    prev = NULL;
    if (node != NULL) {
        key = event->sort_key;
        while (node != NULL) {
            if (node->sort_key <= key) {
                break;
            }
            prev = node;
            node = node->next;
        }
        if (prev != NULL) {
            event->next = prev->next;
            prev->next = event;
        }
    }
    if (prev == NULL) {
        ObjectiveMessageQueue = event;
        event->next = NULL;
    }
}

// FUNCTION: LEGOLAND 0x00468b40
void SetObjectiveEventText(struct ObjectiveEvent *node, char *param_2, unsigned int param_3) {
    char *buffer;

    if (param_3 != 0) {
        buffer = (char *)malloc(strlen(param_2) + 1);
        node->text = buffer;
        strcpy(buffer, param_2);
        node->flags_10 |= 0x20;
    } else {
        node->text = param_2;
        node->flags_10 &= 0xdf;
    }
}

// FUNCTION: LEGOLAND 0x00468bb0
struct ObjectiveEvent *PostObjectiveMessage(const char *format, ...) {
    struct ObjectiveEvent *event;
    va_list args;

    event = AllocObjectiveEvent(0, 1);
    if (event != NULL) {
        va_start(args, format);
        vsprintf(ObjectiveTextBuffer, format, args);
        SetObjectiveEventText(event, (char *)ObjectiveTextBuffer, 1);
        InsertObjectiveMessageSorted(event);
    }
    return event;
}

// FUNCTION: LEGOLAND 0x00468c00
void ShowNextObjectiveMessage(void) {
    struct ObjectiveEvent *event;
    char *at;

    event = ObjectiveMessageQueue;
    if (event != NULL) {
        at = strchr(event->text, 0x40);
        if (at != NULL) {
            *at = 0;
            SpeechCloseFile();
            SpeechLoadWavFile(at + 1);
            SpeechPlay();
            SetHelpKindObjectiveMessage();
        }
        if (DisplayAdvisorHelp(event->text, event->type == 0, 0) != 0) {
            SetNextAdvisorAnimState(5, 0);
            ObjectiveMessageQueue = ObjectiveMessageQueue->next;
            FreeObjectiveEvent(event);
        }
    }
}

// FUNCTION: LEGOLAND 0x00468c80
void InsertObjectiveEventSorted(struct ObjectiveEvent *event) {
    struct ObjectiveEvent *node;
    struct ObjectiveEvent *prev;
    int key;

    node = ObjectiveEventList;
    prev = NULL;
    if (node != NULL) {
        key = event->sort_key;
        while (node != NULL) {
            if (node->sort_key > key) {
                break;
            }
            prev = node;
            node = node->next;
        }
    }
    if (prev != NULL) {
        event->next = prev->next;
        prev->next = event;
    } else {
        ObjectiveEventList = event;
        event->next = NULL;
    }
    ObjectiveEventTypeCounts[event->type] += 1;
}

// FUNCTION: LEGOLAND 0x00468cd0
struct ObjectiveEvent *AllocTimestampedObjectiveEvent(unsigned int type, int sort_key) {
    struct ObjectiveEvent *event;

    event = AllocObjectiveEvent(type, sort_key);
    if (event != NULL) {
        event->timestamp = GetGameTimer();
    }
    return event;
}

// FUNCTION: LEGOLAND 0x00468d00
void ResetObjectiveHintTimer(void) {
    ObjectiveHintTimer = GetGameTimer();
}

// FUNCTION: LEGOLAND 0x00468d10
int IsObjectiveHintDue(void) {
    if ((int)(GetGameTimer() - ObjectiveHintTimer) > 0xc350) {
        ResetObjectiveHintTimer();
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00468d30
int QueueCustomObjectiveHint(struct ObjectiveEvent *object) {
    struct ObjectiveEvent *event;
    struct ObjectiveEvent *target;

    target = (struct ObjectiveEvent *)object;
    if (target->field_40 != 0 && ScriptStringTable[target->field_40] != 0) {
        if (target->flags_10 & 4) {
            event = AllocTimestampedObjectiveEvent(0, 0);
        } else {
            event = AllocTimestampedObjectiveEvent(0, 1);
        }
        event->field_40 = target->field_40;
        InsertObjectiveEventSorted(event);
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00468d80
void QueueBuildMoreObjectsHint(struct ObjectiveEvent *object, unsigned int a, int b) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(1, 1);
    event->field_4 = a;
    event->field_1c = b;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00468dc0
void QueueConnectToPathHint(struct ObjectiveEvent *object, unsigned int a) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(2, 1);
    event->field_4 = a;
    event->field_1c = 0;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00468e00
void QueueLinkToEntranceHint(struct ObjectiveEvent *object, unsigned int a) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(2, 1);
    event->field_4 = a;
    event->field_1c = 1;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00468e40
void QueueBuildRangeAttractionsHint(struct ObjectiveEvent *arg, unsigned int class_id, int count, int sum) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(arg) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(3, 1);
    if (count < 0) {
        count = 0;
    }
    if (sum < 0) {
        sum = 0;
    }
    event->field_4 = class_id;
    event->field_14 = count;
    event->field_1c = sum;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00468ea0
void QueueDeleteRangeAttractionsHint(struct ObjectiveEvent *arg, unsigned int class_id, int count, int sum) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(arg) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(4, 1);
    if (count < 0) {
        count = 0;
    }
    if (sum < 0) {
        sum = 0;
    }
    event->field_4 = class_id;
    event->field_14 = count;
    event->field_1c = sum;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00468f00
void QueueClearAreaHint(struct ObjectiveEvent *object, int a) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(5, 1);
    event->field_1c = a;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00468f40
void QueueDeleteObjectsHint(struct ObjectiveEvent *arg, unsigned int class_id, int count) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(arg) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(6, 1);
    event->field_1c = count;
    event->field_4 = class_id;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00468f80
void QueueAttractVisitorsHint(struct ObjectiveEvent *object, int a) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(7, 1);
    event->field_1c = a;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00468fc0
void QueueMoreGardenersHint(struct ObjectiveEvent *object, int a) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(8, 1);
    event->field_1c = a;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00469000
void QueueFewerGardenersHint(struct ObjectiveEvent *object, int a) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(8, 1);
    event->field_1c = -a;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00469040
void QueueMoreMechanicsHint(struct ObjectiveEvent *object, unsigned int a) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(9, 1);
    event->field_1c = a;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00469080
void QueueFewerMechanicsHint(struct ObjectiveEvent *object, int a) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(9, 1);
    event->field_1c = -a;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x004690c0
void QueueSaveCoinsHint(struct ObjectiveEvent *arg, int count) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(arg) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(0xd, 1);
    event->field_1c = count;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00469100
void QueueHappyVisitorsHint(struct ObjectiveEvent *object, int a, unsigned int b) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(0xe, 1);
    event->field_1c = a;
    event->field_14 = b;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00469140
void QueueFewerHungryVisitorsHint(struct ObjectiveEvent *object, unsigned int a, unsigned int b) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(0xf, 1);
    event->field_1c = a;
    event->field_14 = b;
    event->field_18 = 1;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00469190
void QueueMoreFedVisitorsHint(struct ObjectiveEvent *object, unsigned int a, unsigned int b) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(0xf, 1);
    event->field_1c = a;
    event->field_14 = b;
    event->field_18 = 0;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x004691e0
void QueueRepairObjectsHint(struct ObjectiveEvent *arg, int param_2, unsigned int param_3) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(arg) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(0x10, 1);
    event->field_1c = param_2;
    event->field_14 = param_3;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00469220
void QueueRideVisitorsHint(struct ObjectiveEvent *object, unsigned int a, unsigned int b) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(0x11, 1);
    event->field_4 = a;
    event->field_1c = b;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00469260
void QueueAddRidePartsHint(struct ObjectiveEvent *arg, unsigned int class_id, int sum, int count) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(arg) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(0x12, 1);
    if (sum < 0) {
        sum = 0;
    }
    if (count < 0) {
        count = 0;
    }
    event->field_4 = class_id;
    event->field_14 = count;
    event->field_1c = sum;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00469310
void QueueCoverSquaresHint(struct ObjectiveEvent *object, unsigned int a, int b) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(0xa, 1);
    event->field_14 = b;
    event->field_1c = a;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00469350
void QueuePathSceneryHint(struct ObjectiveEvent *object, int a) {
    struct ObjectiveEvent *event;

    if (IsObjectiveHintDue() == 0) {
        return;
    }
    if (QueueCustomObjectiveHint(object) != 0) {
        return;
    }
    event = AllocTimestampedObjectiveEvent(0xb, 1);
    event->field_14 = a;
    InsertObjectiveEventSorted(event);
}

// FUNCTION: LEGOLAND 0x00469390
void QueueCustomObjectiveHintIfDue(struct ObjectiveEvent *object) {
    if (IsObjectiveHintDue() != 0) {
        QueueCustomObjectiveHint(object);
    }
}

// FUNCTION: LEGOLAND 0x004693b0
void RemoveObjectiveEventsOfType(unsigned int type) {
    struct ObjectiveEvent *node;
    struct ObjectiveEvent *next;
    struct ObjectiveEvent *prev;

    prev = NULL;
    node = ObjectiveEventList;
    while (node != NULL) {
        next = node->next;
        if (node->type == type) {
            if (prev != NULL) {
                prev->next = next;
            } else {
                ObjectiveEventList = next;
            }
            FreeObjectiveEvent(node);
        } else {
            prev = node;
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x00469400
void PostPendingObjectiveHints(void) {
    struct ObjectiveEvent *node;

    GetGameTimer();
    do {
        node = ObjectiveEventList;
        if (node != NULL) {
            switch (node->type) {
            case 0:
                PostObjectiveMessage((char *)PercentSFormat, ScriptStringTable[node->field_40]);
                CurrentObjectivePromptIndex = node->field_40;
                break;
            case 1:
                // STRING: LEGOLAND 0x004ba6bc
                PostObjectiveMessage("You need to build %d more of object %s", node->field_1c,
                    ((struct Element *)node->element)->ride->name);
                break;
            case 0xc:
                // STRING: LEGOLAND 0x004ba69c
                PostObjectiveMessage("You need to research object %s", ((struct Element *)node->element)->ride->name);
                break;
            case 2:
                if (node->field_1c == 0) {
                    if (node->field_4 != 0) {
                        // STRING: LEGOLAND 0x004ba674
                        PostObjectiveMessage("You need to connect your %s to a path",
                            ((struct Element *)node->element)->ride->name);
                    } else {
                        // STRING: LEGOLAND 0x004ba648
                        PostObjectiveMessage("You need to connect all objects to a path");
                    }
                } else {
                    if (node->field_4 != 0) {
                        // STRING: LEGOLAND 0x004ba60c
                        PostObjectiveMessage("You need to link the path from your %s to the park entrance",
                            ((struct Element *)node->element)->ride->name);
                    } else {
                        // STRING: LEGOLAND 0x004ba5c8
                        PostObjectiveMessage("You need to link the paths from all objects to the park entrance.");
                    }
                }
                break;
            case 3:
                if (node->field_14 != 0) {
                    // STRING: LEGOLAND 0x004ba590
                    PostObjectiveMessage("You need to build %d new attractions from the %s range", node->field_14,
                        *(unsigned int *)node->element);
                } else {
                    // STRING: LEGOLAND 0x004ba558
                    PostObjectiveMessage("You need to build %d more attractions from the %s range", node->field_1c,
                        *(unsigned int *)node->element);
                }
                break;
            case 4:
                if (node->field_14 != 0) {
                    // STRING: LEGOLAND 0x004ba510
                    PostObjectiveMessage("You need to delete all instances of %d attractions from the %s range", node->field_14,
                        *(unsigned int *)node->element);
                } else {
                    // STRING: LEGOLAND 0x004ba4dc
                    PostObjectiveMessage("You need delete %d attractions from the %s range", node->field_1c,
                        *(unsigned int *)node->element);
                }
                break;
            case 5:
                // STRING: LEGOLAND 0x004ba4b0
                PostObjectiveMessage("You need to remove %d items from the area", node->field_1c);
                break;
            case 6:
                // STRING: LEGOLAND 0x004ba48c
                PostObjectiveMessage("You need to delete %d of object %s", node->field_1c,
                    ((struct Element *)node->element)->ride->name);
                break;
            case 7:
                // STRING: LEGOLAND 0x004ba45c
                PostObjectiveMessage("You need to attract %d more people to your park", node->field_1c);
                break;
            case 8:
                if ((int)node->field_1c > 0) {
                    // STRING: LEGOLAND 0x004ba428
                    PostObjectiveMessage("You need %d more gardeners to look after your park", node->field_1c);
                } else {
                    // STRING: LEGOLAND 0x004ba3fc
                    PostObjectiveMessage("You need %d fewer gardeners in your park", -(int)node->field_1c);
                }
                break;
            case 9:
                if ((int)node->field_1c > 0) {
                    // STRING: LEGOLAND 0x004ba3cc
                    PostObjectiveMessage("You need %d more mechanics to help in the park", node->field_1c);
                } else {
                    // STRING: LEGOLAND 0x004ba3a0
                    PostObjectiveMessage("You need %d fewer mechanics in your park", -(int)node->field_1c);
                }
                break;
            case 0xa:
                switch (node->field_1c) {
                case 0:
                    // STRING: LEGOLAND 0x004ba370
                    PostObjectiveMessage("You need to cover %d more squares with objects", node->field_14);
                    break;
                case 1:
                    // STRING: LEGOLAND 0x004ba340
                    PostObjectiveMessage("You need to cover %d more squares with rides", node->field_14);
                    break;
                case 4:
                    // STRING: LEGOLAND 0x004ba310
                    PostObjectiveMessage("You need to cover %d more squares with shops", node->field_14);
                    break;
                case 5:
                    // STRING: LEGOLAND 0x004ba2dc
                    PostObjectiveMessage("You need to cover %d more squares with food outlets", node->field_14);
                    break;
                case 2:
                    // STRING: LEGOLAND 0x004ba2ac
                    PostObjectiveMessage("You need to cover %d more squares with scenery", node->field_14);
                    break;
                case 3:
                    // STRING: LEGOLAND 0x004ba26c
                    PostObjectiveMessage("You need to cover %d more squares with stop 'n' wonder objects", node->field_14);
                    break;
                }
                break;
            case 0xb:
                // STRING: LEGOLAND 0x004ba234
                PostObjectiveMessage("You need to line %d%% more of your path with scenery", node->field_14);
                break;
            case 0xd:
                // STRING: LEGOLAND 0x004ba210
                PostObjectiveMessage("You need to save up %d more coins.", node->field_1c);
                break;
            case 0xe:
                // STRING: LEGOLAND 0x004ba1dc
                PostObjectiveMessage("You need make %d people up to happiness level %d", node->field_1c, node->field_14);
                break;
            case 0xf:
                if (node->field_18 != 0) {
                    // STRING: LEGOLAND 0x004ba198
                    PostObjectiveMessage("You need to get %d fewer people with hunger levels greater than %d", node->field_1c,
                        node->field_14);
                } else {
                    // STRING: LEGOLAND 0x004ba15c
                    PostObjectiveMessage("You need to get %d more people with hunger level below %d", node->field_1c,
                        node->field_14);
                }
                break;
            case 0x10:
                // STRING: LEGOLAND 0x004ba110
                PostObjectiveMessage("You need make repairs to %d objects to bring them to above %d%% of health", node->field_1c,
                    node->field_14);
                break;
            case 0x11:
                // STRING: LEGOLAND 0x004ba0e4
                PostObjectiveMessage("You need to get %d more people on the %s", node->field_1c,
                    ((struct Element *)node->element)->ride->name);
                break;
            case 0x12:
                if (node->field_14 != 0) {
                    // STRING: LEGOLAND 0x004ba0b4
                    PostObjectiveMessage("You need to add %d different parts to the %s", node->field_14,
                        ((struct Element *)node->element)->ride->name);
                } else {
                    // STRING: LEGOLAND 0x004ba08c
                    PostObjectiveMessage("You need to add %d more parts to the %s", node->field_1c,
                        ((struct Element *)node->element)->ride->name);
                }
                break;
            case 0x13:
                switch (node->field_18) {
                case 0:
                    // STRING: LEGOLAND 0x004ba050
                    PostObjectiveMessage("You need to improve the LEGOLAND zoning (from %d%% to %d%%)", node->field_1c,
                        node->field_14);
                    break;
                case 1:
                    // STRING: LEGOLAND 0x004ba010
                    PostObjectiveMessage("You need to improve the ADVENTURER zoning (from %d%% to %d%%)", node->field_1c,
                        node->field_14);
                    break;
                case 2:
                    // STRING: LEGOLAND 0x004b9fd4
                    PostObjectiveMessage("You need to  inprove the CASTLE zoning (from %d%% to %d%%)", node->field_1c,
                        node->field_14);
                    break;
                case 3:
                    // STRING: LEGOLAND 0x004b9f98
                    PostObjectiveMessage("You need to improve the WESTERN zoning (from %d%% to %d%%)", node->field_1c,
                        node->field_14);
                    break;
                }
                break;
            }
            RemoveObjectiveEventsOfType(node->type);
        }
    } while (node != NULL);
}

// FUNCTION: LEGOLAND 0x00469900
void GiveRewardObject(struct Element *object, unsigned int a, unsigned int b) {
    struct Element *obj;
    unsigned int flags;

    obj = (struct Element *)object;
    if (obj != NULL) {
        flags = obj->flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x10002) != 2) {
                obj->flags = (flags & 0xfffeffff) | 2;
                UnlockFreePlayObject((struct Element *)obj);
                if (a != 0) {
                    AddNewObjectIcon(obj->data);
                }
            } else {
                // STRING: LEGOLAND 0x004ba6e4
                DBPrintf("Not giving %d.. Already got it\n", obj->name);
            }
        }
    }
    BuildMenuRefreshPending = 1;
}

// FUNCTION: LEGOLAND 0x00469980
void CountObjectTheme(struct Element *arg) {
    struct Ride *theme;
    struct Element *id;

    theme = arg->ride;
    // STRING: LEGOLAND 0x004ba748
    if (LLIDB_FindElement("LEGOLAND THEME", &id, 0) == 0) {
        if (theme->theme_element == id) {
            LegolandCommonThemeCount++;
            return;
        }
        // STRING: LEGOLAND 0x004ba738
        if (LLIDB_FindElement("COMMON THEME", &id, 0) == 0 && theme->theme_element == id) {
            LegolandCommonThemeCount++;
            return;
        }
    }
    // STRING: LEGOLAND 0x004ba728
    if (LLIDB_FindElement("WESTERN THEME", &id, 0) == 0 && theme->theme_element == id) {
        WesternThemeCount++;
        return;
    }
    // STRING: LEGOLAND 0x004ba718
    if (LLIDB_FindElement("CASTLE THEME", &id, 0) == 0 && theme->theme_element == id) {
        CastleThemeCount++;
        return;
    }
    // STRING: LEGOLAND 0x004ba704
    if (LLIDB_FindElement("ADVENTURERS THEME", &id, 0) == 0 && theme->theme_element == id) {
        AdventurersThemeCount++;
    }
}

/* Makes a valid object researchable by clearing available/buildable flag 2 and setting research-menu flag 0x10000;
 * counts its theme and requests a build-menu refresh. */
// FUNCTION: LEGOLAND 0x00469a80
void MakeObjectResearchable(struct Element *object) {
    unsigned int flags;

    if (object != NULL) {
        flags = object->flags;
        if (flags & 1) {
            object->flags = (flags & 0xfffffffd) | 0x10000;
            CountObjectTheme(object);
        }
    }
    BuildMenuRefreshPending = 1;
}

// FUNCTION: LEGOLAND 0x00469ab0
void TakeRewardObject(struct Element *object) {
    unsigned int flags;

    if (object != NULL) {
        flags = object->flags;
        if (flags & 1) {
            object->flags = flags & 0xfffefffd;
        }
    }
    BuildMenuRefreshPending = 1;
}

// FUNCTION: LEGOLAND 0x00469ae0
int ProcessUnimplementedReward(struct ObjectiveEvent *arg) {
    // STRING: LEGOLAND 0x004ba758
    DBPrintf("Processing unimplemented reward [ Type = %d ]\n", arg->type);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469b00
int ProcessUnimplementedObjective(struct ObjectiveEvent *arg) {
    // STRING: LEGOLAND 0x004ba788
    DBPrintf("Processing unimplemented objective [ Type = %d ]\n", arg->type);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469b20
int ScriptEventGive(struct ObjectiveEvent *event) {
    struct Element *object;

    object = event->element;
    GiveRewardObject(object, event->field_14, 0);
    object = event->element;
    object->flags |= 0x20000;
    return 1;
}

/* Objective event handler that makes its element researchable and reports immediate completion. */
// FUNCTION: LEGOLAND 0x00469b50
int ScriptEventMakeResearchable(struct ObjectiveEvent *event) {
    MakeObjectResearchable(event->element);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469b70
int ScriptEventTake(struct ObjectiveEvent *event) {
    TakeRewardObject(event->element);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469b90
int ObjectiveEventAddBricks(struct ObjectiveEvent *event) {
    SetBrickCount(GetBrickCount() + event->field_1c);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469bb0
int ObjectiveEventSetBricks(struct ObjectiveEvent *event) {
    SetBrickCount(event->field_1c);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469bd0
void PlaceObjectAtTile(struct Element *a, void *b) {
    struct Ride *cls;
    struct Point centre;

    cls = a->ride;
    SetBricksLimited(0);
    GetTileCentre((struct Point *)b, &centre.x);
    EditCursor.tile_x = centre.x;
    EditCursor.tile_y = centre.y;
    cls->cb_90(a, &centre, 0x8f8);
    LayCursorPathFootprint(&EditCursor);
    PutObjOnMap(a->ride, a, (struct Point *)&EditCursor.tile_x);
    SetBricksLimited(1);
}

// FUNCTION: LEGOLAND 0x00469c40
int ScriptEventPlace(struct ObjectiveEvent *event) {
    PlaceObjectAtTile(event->element, &event->field_20);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469c60
int FadeOutSampleCallback(struct Sample *handle) {
    SetSampleFade(handle, -100);
    return 0;
}

// Script "clear" event: removes every object whose footprint overlaps the event's area
// (field_28..field_30 x, field_2c..field_34 y), in three passes over the render list (powered
// parent-flagged objects first, then any powered object, then the rest), each removal done through
// the normal query cursor (saved and restored around it). Plays the inventory sound while it runs.
// FUNCTION: LEGOLAND 0x00469c80
int ScriptEventClear(struct ObjectiveEvent *arg) {
    struct SweepInstance *next;
    RECT rect;
    short tile_x;
    struct SweepInstance *current;
    struct Ride *cls;
    short tile_y;
    struct Cursor saved;
    struct Point point;
    struct Sample *sample;
    void *saved_class;
    int power;
    int phase;

    int right;
    sample = PlayInstanceOfSample(GameFX[FX_INVENTORY_OUT].sample, 1, 1, 0);
    SetSampleFlag20(sample);
    AddSFX_Callback(sample, 3000, (unsigned int (*)(struct Sample *))FadeOutSampleCallback);
    for (phase = 0; phase < 3; phase++) {
        current = (struct SweepInstance *)GetFirstRenderObject();
        while (current != NULL) {
            UpdateSound();
            switch (phase) {
            case 0:
                next = current;
                do {
                    next = (struct SweepInstance *)GetNextRenderObject((MapElement *)next);
                } while (next != NULL && (cls = next->object->ride, power = FindObjectsPower(cls), cls->parent_element != NULL) &&
                    (0x10 & (unsigned char)cls->parent_element->flags) != 0 && power <= 0);
                break;
            case 1:
                next = current;
                do {
                    next = (struct SweepInstance *)GetNextRenderObject((MapElement *)next);
                } while (next != NULL && FindObjectsPower(next->object->ride) <= 0);
                break;
            case 2:
                next = (struct SweepInstance *)GetNextRenderObject((MapElement *)current);
                break;
            }
            if (current->flags_c & 0x80) {
                cls = current->object->ride;
                tile_y = current->tile_y;
                rect.top = cls->footprint.v[1];
                tile_x = current->tile_x;
                rect.bottom = cls->footprint.v[3];
                rect.top += tile_y;
                right = cls->footprint.v[2];
                rect.right = right;
                rect.right += tile_x;
                rect.bottom += tile_y;
                rect.left = cls->footprint.v[0] + tile_x;
                if (rect.left <= arg->field_30 && rect.right >= arg->field_28 && rect.top <= arg->field_34 && rect.bottom >= arg->field_2c) {
                    point.y = tile_y;
                    saved_class = QueryClass;
                    memcpy(&saved, &QueryCursor, sizeof(struct Cursor));
                    QueryCursor.tile_y = tile_y;
                    point.x = tile_x;
                    QueryCursor.tile_x = point.x;
                    QueryObj.pos.x = (unsigned char)point.x;
                    QueryClass = cls;
                    QueryObj.pos.y = (unsigned char)point.y;
                    cls->cb_94(cls->element, &point);
                    BuildCursorPtr(&QueryCursor, 0, 0);
                    if (IsCursorBuildable(&QueryCursor) != 0) {
                        ClearPathUnderFootprint(QueryClass, &point.x);
                        RemObjFromMap(QueryClass, (QueryClass)->element, QueryObj, &QueryCursor);
                    }
                    memcpy(&QueryCursor, &saved, sizeof(struct Cursor));
                    QueryClass = saved_class;
                }
            }
            current = next;
        }
    }
    CalculateMapRenderOrder();
    PlayInstanceOfSample(GameFX[FX_INVENTORY_OUT].sample, 0, 1, 0);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469ed0
int ScriptEventUnglue(struct ObjectiveEvent *arg) {
    int y;
    int x;

    y = arg->field_2c;
    while (y <= arg->field_34) {
        x = arg->field_28;
        while (x <= arg->field_30) {
            GameMap[y][x].flags &= 0xffbf;
            x++;
        }
        y++;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00469f20
int ScriptEventGlue(struct ObjectiveEvent *arg) {
    int y;
    int x;

    y = arg->field_2c;
    while (y <= arg->field_34) {
        x = arg->field_28;
        while (x <= arg->field_30) {
            GameMap[y][x].flags |= 0x40;
            x++;
        }
        y++;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00469f70
int ScriptEventExtendPark(struct ObjectiveEvent *arg) {
    return ProcessUnimplementedReward(arg);
}

// FUNCTION: LEGOLAND 0x00469f80
int ScriptEventFmv(struct ObjectiveEvent *arg) {
    PauseGameTimer();
    SetPointer(0);
    FadeOutSpeechAndEffects(1, 0xf);
    PlayMovie(arg->text, 1, 1);
    ResumeGameTimer();
    ClearAdvisorHelp();
    RepostScriptIntro();
    return 1;
}

// FUNCTION: LEGOLAND 0x00469fc0
int ScriptEventInterval(struct ObjectiveEvent *arg) {
    int retries;

    SetIntervalScreenIsBriefing(0);
    if (LoadIntervalReport(arg->text) != 0) {
        EditMode.unk4 = 2;
        FrontEndSubScreenActive = 1;
        FrontEndScreenMode.unk4 = 0xffffffff;
        FrontEndScreenMode.unk8 = 7;
        return 1;
    }
    retries = arg->field_1c;
    arg->field_1c = retries + 1;
    if (retries < 3) {
        return 0;
    }
    // STRING: LEGOLAND 0x004ba7bc
    DBPrintf("Giving up trying to load Interval %s\n", arg->text);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a030
int ScriptEventMessage(struct ObjectiveEvent *arg) {
    return ProcessUnimplementedReward(arg);
}
