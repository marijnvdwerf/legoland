#include "legoland.h"
#include <stdlib.h>
#include <string.h>
#include "globals.h"

#include "bricks.h"
#include "debug_alloc.h"
#include "timer.h"
#include "sound_sfx.h"
#include "objectives.h"
#include "interface.h"
#include "llidb.h"
#include "map_object.h"

struct ThemeQueryArg {
    unsigned char pad_0[0xc];
    struct ThemeData *theme;
};

struct ThemeData {
    unsigned char pad_0[0x5c];
    unsigned int theme_id;
};

struct RewardArg {
    unsigned char pad_0[0xc];
    int reward_type;
};

struct MapRectArg {
    unsigned char pad_0[0x28];
    int x0;
    int y0;
    int x1;
    int y1;
};

struct MapElement {
    unsigned char pad_0[0xc];
    unsigned short flags;
    unsigned char pad_e[0x14 - 0xe];
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
unsigned int FUN_004689f0(char *param_1, char *param_2, int param_3) { STUB(); }

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
void FUN_00468b40(struct ObjectiveEvent *node, unsigned int param_2, unsigned int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x00468bb0
void FUN_00468bb0(void *param_1, unsigned int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x00468c00
void FUN_00468c00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00468c80
void FUN_00468c80(struct ObjectiveEvent *event) { STUB(); }

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
int FUN_00468d30(struct NerpsArg *object) { STUB(); }

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
void FUN_00468e40(struct NerpsArg *arg, unsigned int class_id, int count, int sum) { STUB(); }

// FUNCTION: LEGOLAND 0x00468ea0
void FUN_00468ea0(struct NerpsArg *arg, unsigned int class_id, int count, int sum) { STUB(); }

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
void FUN_00468f40(struct NerpsArg *arg, unsigned int class_id, int count) { STUB(); }

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
void FUN_004690c0(struct NerpsArg *arg, int count) { STUB(); }

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
void FUN_004691e0(struct NerpsArg *arg, int param_2, unsigned int param_3) { STUB(); }

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
void FUN_00469260(struct NerpsArg *arg, unsigned int class_id, int sum, int count) { STUB(); }

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
unsigned int FUN_00469400(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00469900
void FUN_00469900(struct NerpsArg *object, unsigned int a, unsigned int b) { STUB(); }

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
void FUN_00469bd0(unsigned int a, void *b) { STUB(); }

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
void FUN_00469c80(void) { STUB(); }

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
void FUN_00469f20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00469f70
int FUN_00469f70(struct RewardArg *arg) {
    return FUN_00469ae0(arg);
}

// FUNCTION: LEGOLAND 0x00469f80
void FUN_00469f80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00469fc0
void FUN_00469fc0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046a030
int FUN_0046a030(struct RewardArg *arg) {
    return FUN_00469ae0(arg);
}
