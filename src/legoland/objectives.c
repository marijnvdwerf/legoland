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

struct ThemeQueryArg {
    unsigned char pad_0[0xc];
    struct ThemeData *theme;
};

struct ThemeData {
    unsigned char pad_0[0x5c];
    unsigned int theme_id;
};

struct RewardArg {
    unsigned char pad_0[0x8];
    const char *field_8;
    int reward_type;
    unsigned char pad_10[0x1c - 0x10];
    int field_1c;
};

struct MapRectArg {
    unsigned char pad_0[0x28];
    int x0;
    int y0;
    int x1;
    int y1;
};

struct NerpsTarget {
    unsigned char pad_0[0x10];
    unsigned char flags_10;
    unsigned char pad_11[0x40 - 0x11];
    unsigned int field_40;
};

struct RewardObject {
    unsigned int id;
    unsigned char pad_4[0x8 - 0x4];
    unsigned int flags;
    struct NewObjInfo *info;
};

struct PlaceObject {
    int field_0;
    unsigned char pad_4[0xc - 0x4];
    struct ObjClass *cls;
};

struct SweepInstance {
    struct PlaceObject *object;
    unsigned char tile_x;
    unsigned char tile_y;
    unsigned char pad_6[0xc - 0x6];
    unsigned char flags_c;
};

// FUNCTION: LEGOLAND 0x00468810
void FUN_00468810(char *name) {
    strncpy(DAT_0066869c, name, 0x80);
    DAT_0066869c[0x7f] = 0;
}

// FUNCTION: LEGOLAND 0x00468830
void FUN_00468830(void) {
    DAT_0066869c[0] = 0;
    DAT_0066861c = 0;
}

// FUNCTION: LEGOLAND 0x00468840
void FUN_00468840(void) {
    *(unsigned int *)DAT_007fe930 = 0;
    *(unsigned int *)(DAT_007fe930 + 4) = 0;
    *(unsigned short *)(DAT_007fe930 + 8) = 0;
}

// FUNCTION: LEGOLAND 0x00468860
void FUN_00468860(int index, signed char value) {
    if (index < 10) {
        DAT_007fe930[index] = value;
        if (index < 4) {
            FUN_00476140(index, value);
        }
    }
}

// FUNCTION: LEGOLAND 0x00468890
unsigned char FUN_00468890(int index, unsigned char value) {
    if (index < 10) {
        DAT_007fe930[index] += value;
        return DAT_007fe930[index];
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004688c0
char FUN_004688c0(int index) {
    if (index < 10) {
        return DAT_007fe930[index];
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004688e0
void FUN_004688e0(void) {}

// FUNCTION: LEGOLAND 0x004688f0
void FUN_004688f0(int index, unsigned char param_2) {
    if (index < 4) {
        FUN_00460560(index);
    }
}

// FUNCTION: LEGOLAND 0x00468910
struct ObjectiveEvent *FUN_00468910(unsigned int type, int sort_key) {
    struct ObjectiveEvent *event;

    event = (struct ObjectiveEvent *)calloc(1, 0x44);
    if (event != NULL) {
        event->next = NULL;
        event->field_8 = 0;
        event->type = type;
        event->sort_key = sort_key;
    }
    return event;
}

// FUNCTION: LEGOLAND 0x00468940
void FUN_00468940(struct ObjectiveEvent *event) {
    if (event->flags_10 & 0x20) {
        if (event->field_8 != 0) {
            free((void *)event->field_8);
        }
    }
    free(event);
}

// FUNCTION: LEGOLAND 0x00468970
void FUN_00468970(struct ObjectiveEvent *event) {
    if (event != NULL) {
        if (event->next != NULL) {
            FUN_00468970(event->next);
        }
        FUN_00468940(event);
    }
}

// FUNCTION: LEGOLAND 0x004689a0
void FUN_004689a0(void) {
    int i;
    unsigned int *p;

    i = 0;
    if ((int)DAT_00668720 > 0) {
        p = DAT_007fe120;
        do {
            if (*p != 0) {
                free((void *)*p);
                *p = 0;
            }
            i++;
            p++;
        } while (i < (int)DAT_00668720);
    }
    DAT_00668720 = 0;
}

// FUNCTION: LEGOLAND 0x004689f0
unsigned int FUN_004689f0(char *param_1, char *param_2, int param_3) {
    void *buffer;
    unsigned int index;

    if (param_3 != 0) {
        if (param_1 != NULL) {
            if (param_2 != NULL) {
                buffer = malloc(strlen(param_1) + strlen(param_2) + 2);
                DAT_007fe120[DAT_00668720] = (unsigned int)buffer;
                if (buffer != NULL) {
                    // STRING: LEGOLAND 0x004b9f90
                    sprintf(buffer, "%s%c%s", param_1, 0x40, param_2);
                    index = DAT_00668720;
                    DAT_00668720 = DAT_00668720 + 1;
                    return index;
                }
            } else {
                buffer = malloc(strlen(param_1) + 1);
                DAT_007fe120[DAT_00668720] = (unsigned int)buffer;
                if (buffer != NULL) {
                    sprintf(buffer, (char *)DAT_004b8bbc, param_1);
                    index = DAT_00668720;
                    DAT_00668720 = DAT_00668720 + 1;
                    return index;
                }
            }
        } else {
            index = DAT_00668720;
            DAT_007fe120[index] = 0;
            DAT_00668720 = index + 1;
            return index;
        }
    } else {
        DAT_007fe120[DAT_00668720] = (unsigned int)param_1;
    }
    index = DAT_00668720;
    DAT_00668720 = DAT_00668720 + 1;
    return index;
}

// FUNCTION: LEGOLAND 0x00468b00
void FUN_00468b00(struct ObjectiveEvent *event) {
    struct ObjectiveEvent *node;
    struct ObjectiveEvent *prev;
    int key;

    node = DAT_00668724;
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
        DAT_00668724 = event;
        event->next = NULL;
    }
}

// FUNCTION: LEGOLAND 0x00468b40
void FUN_00468b40(struct ObjectiveEvent *node, unsigned int param_2, unsigned int param_3) {
    char *buffer;

    if (param_3 != 0) {
        buffer = (char *)malloc(strlen((char *)param_2) + 1);
        node->field_8 = (unsigned int)buffer;
        strcpy(buffer, (char *)param_2);
        node->flags_10 |= 0x20;
    } else {
        node->field_8 = param_2;
        node->flags_10 &= 0xdf;
    }
}

// FUNCTION: LEGOLAND 0x00468bb0
struct ObjectiveEvent *FUN_00468bb0(const char *format, ...) {
    struct ObjectiveEvent *event;
    va_list args;

    event = FUN_00468910(0, 1);
    if (event != NULL) {
        va_start(args, format);
        vsprintf(DAT_0066820c, format, args);
        FUN_00468b40(event, (unsigned int)DAT_0066820c, 1);
        FUN_00468b00(event);
    }
    return event;
}

// FUNCTION: LEGOLAND 0x00468c00
void FUN_00468c00(void) {
    struct ObjectiveEvent *event;
    char *at;

    event = DAT_00668724;
    if (event != NULL) {
        at = strchr((char *)event->field_8, 0x40);
        if (at != NULL) {
            *at = 0;
            FUN_00498920();
            FUN_00498630(at + 1);
            FUN_00498b00();
            FUN_0046d3a0();
        }
        if (DisplayAdvisorHelp((char *)event->field_8, event->type == 0, 0) != 0) {
            FUN_00444070(5, 0);
            DAT_00668724 = DAT_00668724->next;
            FUN_00468940(event);
        }
    }
}

// FUNCTION: LEGOLAND 0x00468c80
void FUN_00468c80(struct ObjectiveEvent *event) {
    struct ObjectiveEvent *node;
    struct ObjectiveEvent *prev;
    int key;

    node = DAT_00668728;
    prev = NULL;
    if (node != NULL) {
        key = event->sort_key;
        while (node->sort_key <= key) {
            prev = node;
            node = node->next;
            if (node == NULL) {
                break;
            }
        }
        if (prev != NULL) {
            event->next = prev->next;
            prev->next = event;
            DAT_0066872c[event->type] += 1;
            return;
        }
    }
    DAT_00668728 = event;
    event->next = NULL;
    DAT_0066872c[event->type] += 1;
}

// FUNCTION: LEGOLAND 0x00468cd0
struct ObjectiveEvent *FUN_00468cd0(unsigned int type, int sort_key) {
    struct ObjectiveEvent *event;

    event = FUN_00468910(type, sort_key);
    if (event != NULL) {
        event->timestamp = GetGameTimer();
    }
    return event;
}

// FUNCTION: LEGOLAND 0x00468d00
void FUN_00468d00(void) {
    DAT_00668780 = GetGameTimer();
}

// FUNCTION: LEGOLAND 0x00468d10
int FUN_00468d10(void) {
    if ((int)(GetGameTimer() - DAT_00668780) > 0xc350) {
        FUN_00468d00();
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00468d30
int FUN_00468d30(struct NerpsArg *object) {
    struct ObjectiveEvent *event;
    struct NerpsTarget *target;

    target = (struct NerpsTarget *)object;
    if (target->field_40 != 0 && DAT_007fe120[target->field_40] != 0) {
        if (target->flags_10 & 4) {
            event = FUN_00468cd0(0, 0);
        } else {
            event = FUN_00468cd0(0, 1);
        }
        event->field_40 = target->field_40;
        FUN_00468c80(event);
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00468d80
void FUN_00468d80(struct NerpsArg *object, unsigned int a, int b) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(1, 1);
    event->field_4 = a;
    event->field_1c = b;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00468dc0
void FUN_00468dc0(struct NerpsArg *object, unsigned int a) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(2, 1);
    event->field_4 = a;
    event->field_1c = 0;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00468e00
void FUN_00468e00(struct NerpsArg *object, unsigned int a) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(2, 1);
    event->field_4 = a;
    event->field_1c = 1;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00468e40
void FUN_00468e40(struct NerpsArg *arg, unsigned int class_id, int count, int sum) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(arg) != 0) {
        return;
    }
    event = FUN_00468cd0(3, 1);
    if (count < 0) {
        count = 0;
    }
    if (sum < 0) {
        sum = 0;
    }
    event->field_4 = class_id;
    event->field_14 = count;
    event->field_1c = sum;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00468ea0
void FUN_00468ea0(struct NerpsArg *arg, unsigned int class_id, int count, int sum) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(arg) != 0) {
        return;
    }
    event = FUN_00468cd0(4, 1);
    if (count < 0) {
        count = 0;
    }
    if (sum < 0) {
        sum = 0;
    }
    event->field_4 = class_id;
    event->field_14 = count;
    event->field_1c = sum;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00468f00
void FUN_00468f00(struct NerpsArg *object, int a) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(5, 1);
    event->field_1c = a;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00468f40
void FUN_00468f40(struct NerpsArg *arg, unsigned int class_id, int count) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(arg) != 0) {
        return;
    }
    event = FUN_00468cd0(6, 1);
    event->field_1c = count;
    event->field_4 = class_id;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00468f80
void FUN_00468f80(struct NerpsArg *object, int a) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(7, 1);
    event->field_1c = a;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00468fc0
void FUN_00468fc0(struct NerpsArg *object, int a) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(8, 1);
    event->field_1c = a;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00469000
void FUN_00469000(struct NerpsArg *object, int a) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(8, 1);
    event->field_1c = -a;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00469040
void FUN_00469040(struct NerpsArg *object, unsigned int a) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(9, 1);
    event->field_1c = a;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00469080
void FUN_00469080(struct NerpsArg *object, int a) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(9, 1);
    event->field_1c = -a;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x004690c0
void FUN_004690c0(struct NerpsArg *arg, int count) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(arg) != 0) {
        return;
    }
    event = FUN_00468cd0(0xd, 1);
    event->field_1c = count;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00469100
void FUN_00469100(struct NerpsArg *object, int a, unsigned int b) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(0xe, 1);
    event->field_1c = a;
    event->field_14 = b;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00469140
void FUN_00469140(struct NerpsArg *object, unsigned int a, unsigned int b) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(0xf, 1);
    event->field_1c = a;
    event->field_14 = b;
    event->field_18 = 1;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00469190
void FUN_00469190(struct NerpsArg *object, unsigned int a, unsigned int b) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(0xf, 1);
    event->field_1c = a;
    event->field_14 = b;
    event->field_18 = 0;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x004691e0
void FUN_004691e0(struct NerpsArg *arg, int param_2, unsigned int param_3) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(arg) != 0) {
        return;
    }
    event = FUN_00468cd0(0x10, 1);
    event->field_1c = param_2;
    event->field_14 = param_3;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00469220
void FUN_00469220(struct NerpsArg *object, unsigned int a, unsigned int b) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(0x11, 1);
    event->field_4 = a;
    event->field_1c = b;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00469260
void FUN_00469260(struct NerpsArg *arg, unsigned int class_id, int sum, int count) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(arg) != 0) {
        return;
    }
    event = FUN_00468cd0(0x12, 1);
    if (sum < 0) {
        sum = 0;
    }
    if (count < 0) {
        count = 0;
    }
    event->field_4 = class_id;
    event->field_14 = count;
    event->field_1c = sum;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00469310
void FUN_00469310(struct NerpsArg *object, unsigned int a, int b) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(0xa, 1);
    event->field_14 = b;
    event->field_1c = a;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00469350
void FUN_00469350(struct NerpsArg *object, int a) {
    struct ObjectiveEvent *event;

    if (FUN_00468d10() == 0) {
        return;
    }
    if (FUN_00468d30(object) != 0) {
        return;
    }
    event = FUN_00468cd0(0xb, 1);
    event->field_14 = a;
    FUN_00468c80(event);
}

// FUNCTION: LEGOLAND 0x00469390
void FUN_00469390(struct NerpsArg *object) {
    if (FUN_00468d10() != 0) {
        FUN_00468d30(object);
    }
}

// FUNCTION: LEGOLAND 0x004693b0
void FUN_004693b0(unsigned int type) {
    struct ObjectiveEvent *node;
    struct ObjectiveEvent *next;
    struct ObjectiveEvent *prev;

    prev = NULL;
    node = DAT_00668728;
    while (node != NULL) {
        next = node->next;
        if (node->type == type) {
            if (prev != NULL) {
                prev->next = next;
            } else {
                DAT_00668728 = next;
            }
            FUN_00468940(node);
        } else {
            prev = node;
        }
        node = next;
    }
}

// FUNCTION: LEGOLAND 0x00469400
unsigned int FUN_00469400(void) {
    const char *format;
    int value;
    unsigned int name;

    GetGameTimer();
    while (DAT_00668728 != NULL) {
        switch (DAT_00668728->type) {
        case 0:
            FUN_00468bb0((char *)DAT_004b8bbc, DAT_007fe120[DAT_00668728->field_40]);
            DAT_00668614 = DAT_00668728->field_40;
            FUN_004693b0(DAT_00668728->type);
            continue;
        case 1:
            value = DAT_00668728->field_1c;
            name = (unsigned int)((struct PlaceObject *)DAT_00668728->field_4)->cls->name;
            // STRING: LEGOLAND 0x004ba6bc
            format = "You need to build %d more of object %s";
            break;
        case 2:
            value = DAT_00668728->field_4;
            if (DAT_00668728->field_1c == 0) {
                if (value == 0) {
                    // STRING: LEGOLAND 0x004ba648
                    FUN_00468bb0("You need to connect all objects to a path");
                } else {
                    // STRING: LEGOLAND 0x004ba674
                    FUN_00468bb0("You need to connect your %s to a path", ((struct PlaceObject *)value)->cls->name);
                }
            } else if (value == 0) {
                // STRING: LEGOLAND 0x004ba5c8
                FUN_00468bb0("You need to link the paths from all objects to the park entrance.");
            } else {
                // STRING: LEGOLAND 0x004ba60c
                FUN_00468bb0("You need to link the path from your %s to the park entrance", ((struct PlaceObject *)value)->cls->name);
            }
            FUN_004693b0(DAT_00668728->type);
            continue;
        case 3:
            value = DAT_00668728->field_14;
            if (value == 0) {
                name = *(unsigned int *)DAT_00668728->field_4;
                value = DAT_00668728->field_1c;
                // STRING: LEGOLAND 0x004ba558
                format = "You need to build %d more attractions from the %s range";
            } else {
                name = *(unsigned int *)DAT_00668728->field_4;
                // STRING: LEGOLAND 0x004ba590
                format = "You need to build %d new attractions from the %s range";
            }
            break;
        case 4:
            value = DAT_00668728->field_14;
            if (value == 0) {
                name = *(unsigned int *)DAT_00668728->field_4;
                value = DAT_00668728->field_1c;
                // STRING: LEGOLAND 0x004ba4dc
                format = "You need delete %d attractions from the %s range";
            } else {
                name = *(unsigned int *)DAT_00668728->field_4;
                // STRING: LEGOLAND 0x004ba510
                format = "You need to delete all instances of %d attractions from the %s range";
            }
            break;
        case 5:
            // STRING: LEGOLAND 0x004ba4b0
            FUN_00468bb0("You need to remove %d items from the area", DAT_00668728->field_1c);
            FUN_004693b0(DAT_00668728->type);
            continue;
        case 6:
            value = DAT_00668728->field_1c;
            name = (unsigned int)((struct PlaceObject *)DAT_00668728->field_4)->cls->name;
            // STRING: LEGOLAND 0x004ba48c
            format = "You need to delete %d of object %s";
            break;
        case 7:
            // STRING: LEGOLAND 0x004ba45c
            FUN_00468bb0("You need to attract %d more people to your park", DAT_00668728->field_1c);
            FUN_004693b0(DAT_00668728->type);
            continue;
        case 8:
            value = DAT_00668728->field_1c;
            if (value < 1) {
                // STRING: LEGOLAND 0x004ba3fc
                FUN_00468bb0("You need %d fewer gardeners in your park", -value);
                FUN_004693b0(DAT_00668728->type);
            } else {
                // STRING: LEGOLAND 0x004ba428
                FUN_00468bb0("You need %d more gardeners to look after your park", value);
                FUN_004693b0(DAT_00668728->type);
            }
            continue;
        case 9:
            value = DAT_00668728->field_1c;
            if (value < 1) {
                // STRING: LEGOLAND 0x004ba3a0
                FUN_00468bb0("You need %d fewer mechanics in your park", -value);
                FUN_004693b0(DAT_00668728->type);
            } else {
                // STRING: LEGOLAND 0x004ba3cc
                FUN_00468bb0("You need %d more mechanics to help in the park", value);
                FUN_004693b0(DAT_00668728->type);
            }
            continue;
        case 0xa:
            switch (DAT_00668728->field_1c) {
            case 0:
                // STRING: LEGOLAND 0x004ba370
                FUN_00468bb0("You need to cover %d more squares with objects", DAT_00668728->field_14);
                FUN_004693b0(DAT_00668728->type);
                break;
            case 1:
                // STRING: LEGOLAND 0x004ba340
                FUN_00468bb0("You need to cover %d more squares with rides", DAT_00668728->field_14);
                FUN_004693b0(DAT_00668728->type);
                break;
            case 2:
                // STRING: LEGOLAND 0x004ba2ac
                FUN_00468bb0("You need to cover %d more squares with scenery", DAT_00668728->field_14);
                FUN_004693b0(DAT_00668728->type);
                break;
            case 3:
                // STRING: LEGOLAND 0x004ba26c
                FUN_00468bb0("You need to cover %d more squares with stop 'n' wonder objects", DAT_00668728->field_14);
                FUN_004693b0(DAT_00668728->type);
                break;
            case 4:
                // STRING: LEGOLAND 0x004ba310
                FUN_00468bb0("You need to cover %d more squares with shops", DAT_00668728->field_14);
                FUN_004693b0(DAT_00668728->type);
                break;
            case 5:
                // STRING: LEGOLAND 0x004ba2dc
                FUN_00468bb0("You need to cover %d more squares with food outlets", DAT_00668728->field_14);
                FUN_004693b0(DAT_00668728->type);
                break;
            default:
                FUN_004693b0(DAT_00668728->type);
                continue;
            }
            continue;
        case 0xb:
            // STRING: LEGOLAND 0x004ba234
            FUN_00468bb0("You need to line %d%% more of your path with scenery", DAT_00668728->field_14);
            FUN_004693b0(DAT_00668728->type);
            continue;
        case 0xc:
            // STRING: LEGOLAND 0x004ba69c
            FUN_00468bb0("You need to research object %s", ((struct PlaceObject *)DAT_00668728->field_4)->cls->name);
            FUN_004693b0(DAT_00668728->type);
            continue;
        case 0xd:
            // STRING: LEGOLAND 0x004ba210
            FUN_00468bb0("You need to save up %d more coins.", DAT_00668728->field_1c);
            FUN_004693b0(DAT_00668728->type);
            continue;
        case 0xe:
            name = DAT_00668728->field_14;
            value = DAT_00668728->field_1c;
            // STRING: LEGOLAND 0x004ba1dc
            format = "You need make %d people up to happiness level %d";
            break;
        case 0xf:
            if (DAT_00668728->field_18 == 0) {
                name = DAT_00668728->field_14;
                value = DAT_00668728->field_1c;
                // STRING: LEGOLAND 0x004ba15c
                format = "You need to get %d more people with hunger level below %d";
            } else {
                name = DAT_00668728->field_14;
                value = DAT_00668728->field_1c;
                // STRING: LEGOLAND 0x004ba198
                format = "You need to get %d fewer people with hunger levels greater than %d";
            }
            break;
        case 0x10:
            name = DAT_00668728->field_14;
            value = DAT_00668728->field_1c;
            // STRING: LEGOLAND 0x004ba110
            format = "You need make repairs to %d objects to bring them to above %d%% of health";
            break;
        case 0x11:
            value = DAT_00668728->field_1c;
            name = (unsigned int)((struct PlaceObject *)DAT_00668728->field_4)->cls->name;
            // STRING: LEGOLAND 0x004ba0e4
            format = "You need to get %d more people on the %s";
            break;
        case 0x12:
            value = DAT_00668728->field_14;
            if (value == 0) {
                value = DAT_00668728->field_1c;
                name = (unsigned int)((struct PlaceObject *)DAT_00668728->field_4)->cls->name;
                // STRING: LEGOLAND 0x004ba08c
                format = "You need to add %d more parts to the %s";
            } else {
                name = (unsigned int)((struct PlaceObject *)DAT_00668728->field_4)->cls->name;
                // STRING: LEGOLAND 0x004ba0b4
                format = "You need to add %d different parts to the %s";
            }
            break;
        case 0x13:
            switch (DAT_00668728->field_18) {
            case 0:
                name = DAT_00668728->field_14;
                value = DAT_00668728->field_1c;
                // STRING: LEGOLAND 0x004ba050
                format = "You need to improve the LEGOLAND zoning (from %d%% to %d%%)";
                break;
            case 1:
                name = DAT_00668728->field_14;
                value = DAT_00668728->field_1c;
                // STRING: LEGOLAND 0x004ba010
                format = "You need to improve the ADVENTURER zoning (from %d%% to %d%%)";
                break;
            case 2:
                name = DAT_00668728->field_14;
                value = DAT_00668728->field_1c;
                // STRING: LEGOLAND 0x004b9fd4
                format = "You need to  inprove the CASTLE zoning (from %d%% to %d%%)";
                break;
            case 3:
                name = DAT_00668728->field_14;
                value = DAT_00668728->field_1c;
                // STRING: LEGOLAND 0x004b9f98
                format = "You need to improve the WESTERN zoning (from %d%% to %d%%)";
                break;
            default:
                FUN_004693b0(DAT_00668728->type);
                continue;
            }
            break;
        default:
            FUN_004693b0(DAT_00668728->type);
            continue;
        }
        FUN_00468bb0(format, value, name);
        FUN_004693b0(DAT_00668728->type);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00469900
void FUN_00469900(struct NerpsArg *object, unsigned int a, unsigned int b) {
    struct RewardObject *obj;
    unsigned int flags;

    obj = (struct RewardObject *)object;
    if (obj == NULL) {
        DAT_0066871c = 1;
        return;
    }
    flags = obj->flags;
    if ((flags & 1) != 0) {
        if ((flags & 0x10002) == 2) {
            // STRING: LEGOLAND 0x004ba6e4
            DBPrintf("Not giving %d.. Already got it\n", obj->id);
            DAT_0066871c = 1;
            return;
        }
        obj->flags = (flags & 0xfffeffff) | 2;
        FUN_0048a6e0((struct ClippedObject *)obj);
        if (a != 0) {
            FUN_00471c10(obj->info);
        }
    }
    DAT_0066871c = 1;
}

// FUNCTION: LEGOLAND 0x00469980
void FUN_00469980(struct ThemeQueryArg *arg) {
    struct ThemeData *theme;
    unsigned int id;

    theme = arg->theme;
    // STRING: LEGOLAND 0x004ba748
    if (LLIDB_FindElement("LEGOLAND THEME", &id, 0) == 0) {
        if (theme->theme_id == id) {
            DAT_007fe114++;
            return;
        }
        // STRING: LEGOLAND 0x004ba738
        if (LLIDB_FindElement("COMMON THEME", &id, 0) == 0 && theme->theme_id == id) {
            DAT_007fe114++;
            return;
        }
    }
    // STRING: LEGOLAND 0x004ba728
    if (LLIDB_FindElement("WESTERN THEME", &id, 0) == 0 && theme->theme_id == id) {
        DAT_007fe115++;
        return;
    }
    // STRING: LEGOLAND 0x004ba718
    if (LLIDB_FindElement("CASTLE THEME", &id, 0) == 0 && theme->theme_id == id) {
        DAT_007fe116++;
        return;
    }
    // STRING: LEGOLAND 0x004ba704
    if (LLIDB_FindElement("ADVENTURERS THEME", &id, 0) == 0 && theme->theme_id == id) {
        DAT_007fe117++;
    }
}

// FUNCTION: LEGOLAND 0x00469a80
void FUN_00469a80(struct NerpsArg *object) {
    unsigned int flags;

    if (object != NULL) {
        flags = ((struct ObjectiveEvent *)object)->field_8;
        if (flags & 1) {
            ((struct ObjectiveEvent *)object)->field_8 = (flags & 0xfffffffd) | 0x10000;
            FUN_00469980((struct ThemeQueryArg *)object);
        }
    }
    DAT_0066871c = 1;
}

// FUNCTION: LEGOLAND 0x00469ab0
void FUN_00469ab0(struct NerpsArg *object) {
    unsigned int flags;

    if (object != NULL) {
        flags = ((struct ObjectiveEvent *)object)->field_8;
        if (flags & 1) {
            ((struct ObjectiveEvent *)object)->field_8 = flags & 0xfffefffd;
        }
    }
    DAT_0066871c = 1;
}

// FUNCTION: LEGOLAND 0x00469ae0
int FUN_00469ae0(struct RewardArg *arg) {
    // STRING: LEGOLAND 0x004ba758
    DBPrintf("Processing unimplemented reward [ Type = %d ]\n", arg->reward_type);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469b00
int FUN_00469b00(struct RewardArg *arg) {
    // STRING: LEGOLAND 0x004ba788
    DBPrintf("Processing unimplemented objective [ Type = %d ]\n", arg->reward_type);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469b20
int FUN_00469b20(struct ObjectiveEvent *event) {
    struct NerpsArg *object;

    object = (struct NerpsArg *)event->field_4;
    FUN_00469900(object, event->field_14, 0);
    object = (struct NerpsArg *)event->field_4;
    ((struct ObjectiveEvent *)object)->field_8 |= 0x20000;
    return 1;
}

// FUNCTION: LEGOLAND 0x00469b50
int FUN_00469b50(struct ObjectiveEvent *event) {
    FUN_00469a80((struct NerpsArg *)event->field_4);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469b70
int FUN_00469b70(struct ObjectiveEvent *event) {
    FUN_00469ab0((struct NerpsArg *)event->field_4);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469b90
int FUN_00469b90(struct ObjectiveEvent *event) {
    FUN_00457900(GetBrickCount() + event->field_1c);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469bb0
int FUN_00469bb0(struct ObjectiveEvent *event) {
    FUN_00457900(event->field_1c);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469bd0
void FUN_00469bd0(unsigned int a, void *b) {
    struct PlaceObject *object;
    struct ObjClass *cls;
    struct Point centre;

    object = (struct PlaceObject *)a;
    cls = object->cls;
    FUN_00457870(0);
    GetTileCentre((struct Point *)b, &centre.x);
    EditCursor.field_1404 = centre.x;
    EditCursor.field_1408 = centre.y;
    cls->method_90(a, &centre, 0x8f8);
    FUN_0045d770(&EditCursor);
    PutObjOnMap(object->cls, a, (struct Point *)&EditCursor.field_1404);
    FUN_00457870(1);
}

// FUNCTION: LEGOLAND 0x00469c40
int FUN_00469c40(struct ObjectiveEvent *event) {
    FUN_00469bd0(event->field_4, &event->field_20);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469c60
int FUN_00469c60(unsigned int handle) {
    SetSampleFade((struct Sample *)handle, -100);
    return 0;
}

// FUNCTION: LEGOLAND 0x00469c80
int FUN_00469c80(struct MapRectArg *arg) {
    struct Cursor saved;
    struct Point point;
    struct Sample *sample;
    struct SweepInstance *current;
    struct SweepInstance *next;
    struct ObjClass *cls;
    int phase;
    int power;
    int found;
    unsigned int tile_x;
    unsigned int tile_y;
    int left;
    int bottom;

    sample = PlayInstanceOfSample(DAT_004b92fc, 1, 1, 0);
    FUN_00496d10(sample);
    AddSFX_Callback((struct CallbackEntry *)sample, 3000, (unsigned int (*)(struct CallbackEntry *))FUN_00469c60);
    phase = 0;
    do {
        current = (struct SweepInstance *)GetFirstRenderObject();
        while (current != NULL) {
            FUN_004969d0();
            next = current;
            if (phase == 0) {
                while ((next = (struct SweepInstance *)GetNextRenderObject((struct RenderObject *)current)) != NULL) {
                    cls = next->object->cls;
                    power = FindObjectsPower(cls);
                    if (cls->field_58 == NULL || (cls->field_58[8] & 0x10) == 0 || (current = next, power > 0)) {
                        break;
                    }
                }
            } else if (phase == 1) {
                do {
                    next = (struct SweepInstance *)GetNextRenderObject((struct RenderObject *)current);
                    if (next == NULL) {
                        break;
                    }
                    power = FindObjectsPower(next->object->cls);
                    current = next;
                } while (power < 1);
            } else if (phase == 2) {
                next = (struct SweepInstance *)GetNextRenderObject((struct RenderObject *)current);
            }
            if (current->flags_c & 0x80) {
                tile_x = current->tile_x;
                QueryClass = current->object->cls;
                QueryObj = *(unsigned short *)&current->tile_x;
                tile_y = current->tile_y;
                cls = (struct ObjClass *)QueryClass;
                left = cls->field_40 + tile_y;
                bottom = cls->field_48 + tile_y;
                if (cls->field_3c + (int)tile_x <= arg->x1 && arg->x0 <= cls->field_44 + (int)tile_x &&
                    left <= arg->y1 && arg->y0 <= bottom) {
                    saved = QueryCursor;
                    DAT_00811564 = tile_x;
                    DAT_00811568 = tile_y;
                    point.x = tile_x;
                    point.y = tile_y;
                    cls->method_94(cls->field_c4, &point);
                    BuildCursorPtr(&QueryCursor, 0, 0);
                    found = FUN_0045f4b0(&QueryCursor);
                    if (found != 0) {
                        FUN_0045d3d0((struct PathFootprint *)cls, &point.x);
                        RemObjFromMap(cls, cls->field_c4, QueryObj, &QueryCursor);
                    }
                    QueryCursor = saved;
                }
            }
            current = next;
        }
        phase++;
    } while (phase <= 2);
    CalculateMapRenderOrder();
    PlayInstanceOfSample(DAT_004b92fc, 0, 1, 0);
    return 1;
}

// FUNCTION: LEGOLAND 0x00469ed0
int FUN_00469ed0(struct MapRectArg *arg) {
    int y;
    int x;

    y = arg->y0;
    while (y <= arg->y1) {
        x = arg->x0;
        while (x <= arg->x1) {
            GameMap[y][x].flags &= 0xffbf;
            x++;
        }
        y++;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00469f20
int FUN_00469f20(struct MapRectArg *arg) {
    int y;
    int x;

    y = arg->y0;
    while (y <= arg->y1) {
        x = arg->x0;
        while (x <= arg->x1) {
            GameMap[y][x].flags |= 0x40;
            x++;
        }
        y++;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00469f70
int FUN_00469f70(struct RewardArg *arg) {
    return FUN_00469ae0(arg);
}

// FUNCTION: LEGOLAND 0x00469f80
int FUN_00469f80(struct RewardArg *arg) {
    FUN_00499380();
    SetPointer(0);
    FUN_00496e60(1, 0xf);
    FUN_004771f0(arg->field_8, 1, 1);
    FUN_004993c0();
    FUN_0046ce20();
    FUN_0046b760();
    return 1;
}

// FUNCTION: LEGOLAND 0x00469fc0
int FUN_00469fc0(struct RewardArg *arg) {
    int retries;

    FUN_00490600(0);
    if (FUN_004907a0(arg->field_8) != 0) {
        EditMode.unk4 = 2;
        DAT_00668e38 = 1;
        DAT_0080ff80.unk4 = 0xffffffff;
        DAT_0080ff80.unk8 = 7;
        return 1;
    }
    retries = arg->field_1c;
    arg->field_1c = retries + 1;
    if (retries < 3) {
        return 0;
    }
    // STRING: LEGOLAND 0x004ba7bc
    DBPrintf("Giving up trying to load Interval %s\n", arg->field_8);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a030
int FUN_0046a030(struct RewardArg *arg) {
    return FUN_00469ae0(arg);
}
