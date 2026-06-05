#include "legoland.h"

struct NerpsArg {
    unsigned char pad_0[0x4];
    unsigned int field_4;
    unsigned int field_8;
    unsigned char pad_c[0x14 - 0xc];
    unsigned int field_14;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned int field_20;
};

struct LegoConfig {
    unsigned char pad_0[0x34];
    unsigned int field_34;
    unsigned int field_38;
};

struct ObjectClass {
    struct ObjectClass *next;
    unsigned char pad_4[0x8 - 0x4];
    unsigned int count;
    unsigned char pad_c[0x58 - 0xc];
    unsigned int class_id_58;
    unsigned int class_id_5c;
};

struct Bloke {
    struct Bloke *next;
    unsigned char pad_4[0x7a - 0x4];
    short field_7a;
};

struct LoopArg {
    unsigned char pad_0[0x4];
    struct LoopObject *field_4;
    unsigned char pad_8[0x1c - 0x8];
    int field_1c;
};

struct LoopVtable {
    unsigned char pad_0[0xc0];
    int (*eval)(struct LoopObject *object, int param_2);
};

struct LoopObject {
    const char *name;
    unsigned char pad_4[0xc - 0x4];
    struct LoopVtable *vtable;
};

struct SortNode;

struct EventNode {
    struct EventNode *next;
    unsigned int field_4;
    unsigned char pad_8[0x10 - 0x8];
    unsigned char flags_10;
    unsigned char pad_11[0x14 - 0x11];
    unsigned int field_14;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned int field_20;
    unsigned int field_24;
    unsigned int field_28;
    unsigned int field_2c;
    unsigned int field_30;
    unsigned int field_34;
};

struct EventList {
    unsigned char pad_0[0x10];
    struct EventNode *head;
};

struct RenderVtableEntry {
    unsigned char pad_0[0x2c];
    unsigned char field_2c;
};

struct RenderVtable {
    unsigned char pad_0[0xc];
    struct RenderVtableEntry *field_c;
};

struct RenderObject {
    struct RenderVtable *vtable;
    unsigned char pad_4[0xc - 0x4];
    unsigned int field_c;
    unsigned char pad_10[0x11 - 0x10];
    unsigned char field_11;
};

struct Vec4 {
    unsigned int x;
    unsigned int y;
    unsigned int z;
    unsigned int w;
};

struct EventNodeVec {
    unsigned char pad_0[0x10];
    unsigned char flags_10;
    unsigned char pad_11[0x28 - 0x11];
    struct Vec4 vec_28;
};

extern struct LegoConfig *lpConfig;
extern struct ObjectClass *ObjectClassList;
extern struct Bloke *FirstBloke;
extern unsigned int DAT_00832920;
extern unsigned int DAT_00832924;
extern int DAT_00832970;
extern unsigned int DAT_00832978;
extern unsigned int DAT_00668788;
extern int DAT_0066878c;
extern int DAT_006661bc;
extern int DAT_00667cf8;
extern int DAT_00667d08;
extern int DAT_00667ce4;
extern int DAT_00667ce8;
extern int DAT_00667cec;
extern int DAT_00667ce0;

extern void FUN_00457870(unsigned int param_1);
extern void GenerateGardener(void *object, int param_2);
extern void GenerateMechanic(void *object, int param_2);
extern void FUN_00462e50(unsigned int param_1, unsigned int param_2);
extern void FUN_00462e70(unsigned int param_1, unsigned int param_2);
extern void FUN_0046b240(unsigned int param_1);
extern void FUN_00468860(unsigned int param_1, unsigned char param_2);
extern unsigned char FUN_00468890(unsigned int param_1, unsigned char param_2);
extern void FUN_004688f0(unsigned int param_1, unsigned char param_2);
extern void FUN_004687f0(unsigned int param_1);
extern void FUN_00468810(unsigned int param_1);
extern void FUN_00476070(unsigned int param_1, unsigned int param_2);
extern int ObjCount(unsigned int class_id);
extern void FUN_00468d80(struct NerpsArg *arg, unsigned int class_id, int count);
extern void FUN_00468e40(struct NerpsArg *arg, unsigned int class_id, int count, int sum);
extern void FUN_00468ea0(struct NerpsArg *arg, unsigned int class_id, int count, int sum);
extern void FUN_00468f40(struct NerpsArg *arg, unsigned int class_id, int count);
extern void FUN_00469260(struct NerpsArg *arg, unsigned int class_id, int sum, int count);
extern void FUN_00468f80(struct NerpsArg *arg, int count);
extern void FUN_00469310(struct NerpsArg *arg, unsigned int param_2, int count);
extern void FUN_00469350(struct NerpsArg *arg, int count);
extern void FUN_004690c0(struct NerpsArg *arg, int count);
extern void FUN_00469100(struct NerpsArg *arg, int count, unsigned int param_3);
extern void FUN_00469390(struct LoopArg *arg);
extern int FUN_00469ae0(unsigned int param_1);
extern int FUN_00469b00(unsigned int param_1);
extern void FUN_00459970(void);
extern int GetBrickCount(void);
extern int DBPrintf(const char *format, ...);

extern struct EventList *DAT_0066879c;
extern struct EventNode *FUN_00468910(unsigned int type, int sort_key);
extern void FUN_00468940(struct EventNode *node);
extern void FUN_00468b40(struct EventNode *node, unsigned int param_2, unsigned int param_3);
extern void FUN_00468b00(struct EventNode *node);
extern void FUN_00468d00(void);
extern void FUN_00468bb0(void *param_1, unsigned int param_2);
extern unsigned int DAT_00668614;
extern unsigned int DAT_00668618;
extern unsigned int DAT_007fe120[];
extern unsigned char DAT_004b8bbc[];

extern unsigned int DAT_00832ba4;
extern int DAT_00832bdc;
extern void FUN_004691e0(struct NerpsArg *arg, int param_2, unsigned int param_3);
extern struct RenderObject *GetFirstRenderObject(void);
extern struct RenderObject *GetNextRenderObject(struct RenderObject *object);
extern char FUN_004688c0(int index);
extern int FUN_00468d10(void);
extern int FUN_00468d30(struct NerpsArg *arg);
extern unsigned int DAT_004baff8;
extern unsigned int DAT_00668e34;
extern unsigned int DAT_007fdca4;
extern struct SortNode *DAT_00668798;
extern struct EventNode *DAT_00668784;
extern unsigned int DAT_00668790;
extern unsigned int DAT_00668794;
extern unsigned int DAT_00668610;
extern unsigned int DAT_004b9e5c[];
extern unsigned int EditMode;
extern unsigned int DAT_008119b8;
extern unsigned int DAT_007fd624;

extern void *_malloc(unsigned int size);
extern void FUN_0049e4d0(void *block);
extern void FUN_00468970(struct EventNode *node);
extern unsigned int DAT_00832ba8;
extern void FUN_004748a0(unsigned int param_1);
extern unsigned int FUN_00458be0(void);
extern void FUN_00459820(unsigned int param_1);

void FUN_0046b630(struct EventNode *node, struct EventList *list);
unsigned int FUN_0046b280(void);

void FUN_0046a040(unsigned int param_1, unsigned int param_2);
void FUN_0046a140(int index, unsigned int param_2, unsigned int param_3);

// FUNCTION: LEGOLAND 0x0046a040
void FUN_0046a040(unsigned int param_1, unsigned int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x0046a120
unsigned int FUN_0046a120(struct NerpsArg *arg) {
    FUN_0046a040(arg->field_1c, arg->field_14);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a140
void FUN_0046a140(int index, unsigned int param_2, unsigned int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x0046a170
unsigned int FUN_0046a170(struct NerpsArg *arg) {
    FUN_0046a140(arg->field_18, arg->field_1c, arg->field_14);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a190
unsigned int FUN_0046a190(struct NerpsArg *arg) {
    int i;

    FUN_00457870(0);
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
    FUN_00457870(1);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a1f0
unsigned int FUN_0046a1f0(struct NerpsArg *arg) {
    if ((int)arg->field_1c >= 0) {
        lpConfig->field_38 = (arg->field_1c != 0);
    }
    if ((int)arg->field_14 >= 0) {
        lpConfig->field_34 = (arg->field_14 != 0);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a230
void FUN_0046a230(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046a300
unsigned int FUN_0046a300(struct NerpsArg *arg) {
    if (arg->field_14 != 0) {
        DAT_00832920 = arg->field_1c;
    } else {
        DAT_00832924 = arg->field_1c;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a330
unsigned int FUN_0046a330(struct NerpsArg *arg) {
    FUN_00462e50(arg->field_14, arg->field_1c);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a350
unsigned int FUN_0046a350(struct NerpsArg *arg) {
    FUN_00462e70(arg->field_14, arg->field_1c);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a370
unsigned int FUN_0046a370(struct NerpsArg *arg) {
    DAT_00832970 = (short)arg->field_1c;
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a390
unsigned int FUN_0046a390(void) {
    if (DAT_00832978 == 0) {
        FUN_0046b240(1);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a3b0
void FUN_0046a3b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046a420
unsigned int FUN_0046a420(struct NerpsArg *arg) {
    FUN_00468860(arg->field_14, arg->field_1c);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a440
unsigned int FUN_0046a440(struct NerpsArg *arg) {
    FUN_00468890(arg->field_14, arg->field_1c);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a460
unsigned int FUN_0046a460(struct NerpsArg *arg) {
    FUN_004688f0(arg->field_14, arg->field_1c);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a480
unsigned int FUN_0046a480(struct NerpsArg *arg) {
    FUN_004687f0(arg->field_8);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a4a0
unsigned int FUN_0046a4a0(struct NerpsArg *arg) {
    FUN_00468810(arg->field_8);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a4c0
unsigned int FUN_0046a4c0(struct NerpsArg *arg) {
    FUN_00476070(arg->field_1c, arg->field_14);
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a4e0
unsigned int FUN_0046a4e0(void) {
    DAT_00668788 = 1;
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a4f0
unsigned int FUN_0046a4f0(struct NerpsArg *arg) {
    int count;

    count = ObjCount(arg->field_4);
    if (count < (int)arg->field_1c) {
        FUN_00468d80(arg, arg->field_4, arg->field_1c - count);
        DAT_0066878c = arg->field_1c - count;
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a540
void FUN_0046a540(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046a5b0
void FUN_0046a5b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046a690
void FUN_0046a690(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046a730
unsigned int FUN_0046a730(struct NerpsArg *arg) {
    int value;

    value = (short)arg->field_20;
    if (value == 1 || (value > 3 && value <= 5)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046a750
void FUN_0046a750(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046a900
unsigned int FUN_0046a900(struct NerpsArg *arg) {
    struct ObjectClass *node;
    int count;
    int sum;

    node = ObjectClassList;
    count = 0;
    sum = 0;
    if (node != NULL) {
        do {
            if (node->class_id_5c == arg->field_4 && node->count != 0) {
                sum += node->count;
                count++;
            }
            node = node->next;
        } while (node != NULL);
    }
    if (count < (int)arg->field_14 || sum < (int)arg->field_1c) {
        FUN_00468e40(arg, arg->field_4, arg->field_14 - count, arg->field_1c - sum);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046a960
void FUN_0046a960(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046aa30
unsigned int FUN_0046aa30(struct NerpsArg *arg) {
    int count;

    count = ObjCount(arg->field_4);
    if (count > (int)arg->field_1c) {
        FUN_00468f40(arg, arg->field_4, count - arg->field_1c);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046aa70
unsigned int FUN_0046aa70(struct NerpsArg *arg) {
    struct ObjectClass *node;
    int count;
    int sum;

    node = ObjectClassList;
    count = 0;
    sum = 0;
    if (node != NULL) {
        do {
            if (node->class_id_5c == arg->field_4 && node->count != 0) {
                sum += node->count;
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
        FUN_00468ea0(arg, arg->field_4, count - arg->field_14, sum - arg->field_1c);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046aae0
unsigned int FUN_0046aae0(struct NerpsArg *arg) {
    struct ObjectClass *node;
    int count;
    int sum;

    node = ObjectClassList;
    if (ObjCount(arg->field_4) < 1) {
        FUN_00468d80(arg, arg->field_4, 1);
        return 0;
    }
    count = 0;
    sum = 0;
    if (node != NULL) {
        do {
            if (node->class_id_58 == arg->field_4 && node->count != 0) {
                sum += node->count;
                count++;
            }
            node = node->next;
        } while (node != NULL);
    }
    if (count >= (int)arg->field_14 && sum >= (int)arg->field_1c) {
        return 1;
    }
    FUN_00469260(arg, arg->field_4, arg->field_1c - sum, arg->field_14 - count);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046ab70
unsigned int FUN_0046ab70(struct LoopArg *arg) {
    struct LoopObject *object;
    struct LoopVtable *vtable;

    object = arg->field_4;
    vtable = object->vtable;
    if (vtable->eval != NULL) {
        if (vtable->eval(object, 1) < arg->field_1c) {
            FUN_00469390(arg);
            return 0;
        }
    } else {
        // STRING: LEGOLAND 0x004ba7e4
        DBPrintf("Cannot evaluate LoopSize for %s\n", object->name);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046abc0
int FUN_0046abc0(unsigned int param_1) {
    return FUN_00469ae0(param_1);
}

// FUNCTION: LEGOLAND 0x0046abd0
unsigned int FUN_0046abd0(struct NerpsArg *arg) {
    if (DAT_006661bc < (int)arg->field_1c) {
        FUN_00468f80(arg, arg->field_1c - DAT_006661bc);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046ac00
void FUN_0046ac00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046ac50
void FUN_0046ac50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046ad00
unsigned int FUN_0046ad00(struct NerpsArg *arg) {
    if (DAT_00667cf8 < (int)arg->field_14) {
        FUN_00469310(arg, 2, arg->field_14 - DAT_00667cf8);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046ad30
unsigned int FUN_0046ad30(struct NerpsArg *arg) {
    FUN_00459970();
    if (DAT_00667d08 < (int)arg->field_14) {
        FUN_00469350(arg, arg->field_14 - DAT_00667d08);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046ad60
unsigned int FUN_0046ad60(struct NerpsArg *arg) {
    if (DAT_00667ce4 < (int)arg->field_14) {
        FUN_00469310(arg, 1, arg->field_14 - DAT_00667ce4);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046ad90
unsigned int FUN_0046ad90(struct NerpsArg *arg) {
    if (DAT_00667ce8 < (int)arg->field_14) {
        FUN_00469310(arg, 4, arg->field_14 - DAT_00667ce8);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046adc0
unsigned int FUN_0046adc0(struct NerpsArg *arg) {
    if (DAT_00667cec < (int)arg->field_14) {
        FUN_00469310(arg, 5, arg->field_14 - DAT_00667cec);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046adf0
unsigned int FUN_0046adf0(struct NerpsArg *arg) {
    if (DAT_00667ce0 < (int)arg->field_14) {
        FUN_00469310(arg, 0, arg->field_14 - DAT_00667ce0);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046ae20
unsigned int FUN_0046ae20(void) {
    return 1;
}

// FUNCTION: LEGOLAND 0x0046ae30
int FUN_0046ae30(unsigned int param_1) {
    return FUN_00469ae0(param_1);
}

// FUNCTION: LEGOLAND 0x0046ae40
unsigned int FUN_0046ae40(struct NerpsArg *arg) {
    if (GetBrickCount() >= (int)arg->field_1c) {
        return 1;
    }
    FUN_004690c0(arg, arg->field_1c - GetBrickCount());
    return 0;
}

// FUNCTION: LEGOLAND 0x0046ae70
unsigned int FUN_0046ae70(struct NerpsArg *arg) {
    struct Bloke *bloke;
    int count;

    count = 0;
    bloke = FirstBloke;
    while (bloke != NULL) {
        if (bloke->field_7a < (int)arg->field_14) {
            bloke = bloke->next;
            continue;
        }
        count++;
        if (count >= (int)arg->field_1c) {
            return 1;
        }
        bloke = bloke->next;
    }
    FUN_00469100(arg, arg->field_1c - count, arg->field_14);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046aec0
void FUN_0046aec0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046af10
void FUN_0046af10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046af39
void FUN_0046af39(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046af60
void FUN_0046af60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046afe0
unsigned int FUN_0046afe0(struct NerpsArg *arg) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b080
unsigned int FUN_0046b080(struct NerpsArg *arg) {
    if (DAT_00832bdc > (int)arg->field_1c) {
        FUN_004691e0(arg, DAT_00832bdc - arg->field_1c, 0);
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046b0b0
unsigned int FUN_0046b0b0(void) {
    return 1;
}

// FUNCTION: LEGOLAND 0x0046b0c0
unsigned int FUN_0046b0c0(struct NerpsArg *arg) {
    if (FUN_004688c0(arg->field_14) >= (int)arg->field_1c) {
        return 1;
    }
    if (FUN_00468d10() != 0) {
        FUN_00468d30(arg);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046b100
unsigned int FUN_0046b100(struct NerpsArg *arg) {
    if (arg->field_1c == DAT_004baff8) {
        return 1;
    }
    if (FUN_00468d10() != 0) {
        FUN_00468d30(arg);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046b130
unsigned int FUN_0046b130(struct NerpsArg *arg) {
    if (DAT_004baff8 != 5) {
        if (arg->field_1c != 0 && DAT_00668e34 != 0) {
            return 1;
        }
        if (arg->field_1c == 0 && DAT_00668e34 == 0) {
            return 1;
        }
    }
    if (FUN_00468d10() != 0) {
        FUN_00468d30(arg);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046b180
unsigned int FUN_0046b180(struct NerpsArg *arg) {
    if (arg->field_1c == 3) {
        if (EditMode == 1 && DAT_008119b8 == DAT_007fd624) {
            return 1;
        }
    } else if (EditMode == arg->field_1c) {
        return 1;
    }
    if (FUN_00468d10() == 0) {
        return 0;
    }
    FUN_00468d30(arg);
    return 0;
}

// FUNCTION: LEGOLAND 0x0046b1e0
int FUN_0046b1e0(unsigned int param_1) {
    return FUN_00469b00(param_1);
}

// FUNCTION: LEGOLAND 0x0046b1f0
unsigned int FUN_0046b1f0(void) {
    return 0;
}

struct TimedEvent {
    unsigned char pad_0[0xc];
    unsigned int field_c;
    unsigned char pad_10[0x3c - 0x10];
    unsigned int field_3c;
};

// FUNCTION: LEGOLAND 0x0046b200
unsigned int FUN_0046b200(struct TimedEvent *event) {
    if (DAT_00668790 == 0) {
        if ((DAT_004b9e5c[event->field_c] & DAT_00668610) == 0) {
            if ((int)(DAT_00668794 - event->field_3c) <= 0x1388) {
                return 0;
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0046b240
void FUN_0046b240(unsigned int param_1) {
    DAT_00832ba4 = param_1;
    FUN_004748a0(1);
    if (param_1 != 0) {
        FUN_00458be0();
        if (DAT_00832ba8 != 0) {
            return;
        }
        FUN_00459820(1);
    }
}

// FUNCTION: LEGOLAND 0x0046b280
unsigned int FUN_0046b280(void) {
    return DAT_00832ba4;
}

// FUNCTION: LEGOLAND 0x0046b290
void FUN_0046b290(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b2d0
void FUN_0046b2d0(void) { STUB(); }

struct ListNode {
    struct ListNode *field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
};

// FUNCTION: LEGOLAND 0x0046b4f0
struct ListNode *FUN_0046b4f0(unsigned int param_1) {
    struct ListNode *node;

    node = (struct ListNode *)_malloc(0x14);
    if (node != NULL) {
        node->field_8 = 0;
        node->field_4 = param_1;
        node->field_c = 0;
        node->field_10 = 0;
        node->field_0 = NULL;
    }
    return node;
}

struct WrapperNode {
    struct WrapperNode *field_0;
    unsigned char pad_4[0x8 - 0x4];
    void *field_8;
    struct EventNode *field_c;
    struct EventNode *field_10;
};

// FUNCTION: LEGOLAND 0x0046b520
void FUN_0046b520(struct WrapperNode *node) {
    if (node->field_c != NULL) {
        FUN_00468970(node->field_c);
    }
    if (node->field_10 != NULL) {
        FUN_00468970(node->field_10);
    }
    if (node->field_8 != NULL) {
        FUN_0049e4d0(node->field_8);
    }
    FUN_0049e4d0(node);
}

// FUNCTION: LEGOLAND 0x0046b560
void FUN_0046b560(struct WrapperNode *node) {
    if (node != NULL) {
        if (node->field_0 != NULL) {
            FUN_0046b560(node->field_0);
        }
        FUN_0046b520(node);
    }
}

struct SortNode {
    struct SortNode *next;
    int val;
};

// FUNCTION: LEGOLAND 0x0046b590
void FUN_0046b590(struct SortNode *node) {
    struct SortNode *prev;
    struct SortNode *cur;

    prev = NULL;
    cur = DAT_00668798;
    if (cur != NULL) {
        while (cur != NULL) {
            if (cur->val >= node->val) {
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
    DAT_00668798 = node;
    node->next = NULL;
}

// FUNCTION: LEGOLAND 0x0046b5d0
int FUN_0046b5d0(struct SortNode *node) {
    struct SortNode *cur;

    cur = DAT_00668798;
    if (cur == node) {
        DAT_00668798 = node->next;
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

struct TimedNode {
    struct TimedNode *field_0;
    unsigned char pad_4[0x40 - 0x4];
    unsigned int field_40;
};

struct TimedList {
    unsigned char pad_0[0xc];
    struct TimedNode *field_c;
};

// FUNCTION: LEGOLAND 0x0046b610
void FUN_0046b610(struct TimedNode *node, struct TimedList *list) {
    node->field_40 = DAT_007fdca4;
    if (list->field_c != NULL) {
        node->field_0 = list->field_c;
    }
    list->field_c = node;
}

// FUNCTION: LEGOLAND 0x0046b630
void FUN_0046b630(struct EventNode *node, struct EventList *list) {
    struct EventNode *head;

    head = list->head;
    if (head != NULL) {
        node->next = head;
    }
    list->head = node;
}

struct StringHolder {
    unsigned char pad_0[0x8];
    char *field_8;
};

// FUNCTION: LEGOLAND 0x0046b650
void FUN_0046b650(const char *src, struct StringHolder *holder) { STUB(); }

struct Ctx6b0 {
    unsigned char pad_0[0x8];
    unsigned int field_8;
};

// FUNCTION: LEGOLAND 0x0046b6b0
void FUN_0046b6b0(struct Ctx6b0 *ctx, unsigned int param_2) {
    struct EventNode *node;

    if (ctx->field_8 != 0) {
        node = FUN_00468910(1, 2);
        FUN_00468b40(node, ctx->field_8, param_2);
        if (param_2 == 0) {
            ctx->field_8 = 0;
            node->flags_10 = 0x20;
        }
        FUN_00468b00(node);
        DAT_00668614 = 0;
    }
}

// FUNCTION: LEGOLAND 0x0046b700
unsigned int FUN_0046b700(void) {
    if (DAT_0066879c != NULL) {
        if (DAT_00668614 != 0) {
            FUN_00468bb0(DAT_004b8bbc, DAT_007fe120[DAT_00668614]);
            DAT_00668618 = 1;
            return 1;
        }
        FUN_0046b6b0((struct Ctx6b0 *)DAT_0066879c, 1);
        FUN_00468d00();
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046b760
unsigned int FUN_0046b760(void) {
    if (FUN_0046b280() == 0) {
        if (DAT_0066879c != NULL) {
            FUN_0046b6b0((struct Ctx6b0 *)DAT_0066879c, 1);
            FUN_00468d00();
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0046b790
void FUN_0046b790(unsigned int param_1, unsigned int param_2) {
    struct EventNode *node;

    node = FUN_00468910(2, 1);
    node->field_4 = param_1;
    node->flags_10 = 0;
    node->field_14 = param_2;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046b7f0
void FUN_0046b7f0(unsigned int param_1) {
    struct EventNode *node;

    node = FUN_00468910(4, 1);
    node->flags_10 = 0;
    node->field_4 = param_1;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046b820
void FUN_0046b820(unsigned int param_1) {
    struct EventNode *node;

    node = FUN_00468910(5, 1);
    node->flags_10 = 0;
    node->field_1c = param_1;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046b850
void FUN_0046b850(unsigned int param_1) {
    struct EventNode *node;

    node = FUN_00468910(6, 1);
    node->flags_10 = 0;
    node->field_1c = param_1;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046b880
void FUN_0046b880(unsigned int param_1, unsigned int *param_2, unsigned int param_3) {
    struct EventNode *node;

    node = FUN_00468910(7, 1);
    node->field_4 = param_1;
    node->field_20 = param_2[0];
    node->field_24 = param_2[1];
    node->field_18 = param_3;
    node->flags_10 = 0;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046b8c0
void FUN_0046b8c0(struct Vec4 *param_1) {
    struct EventNodeVec *node;

    node = (struct EventNodeVec *)FUN_00468910(8, 1);
    node->vec_28 = *param_1;
    node->flags_10 = 0;
    FUN_0046b630((struct EventNode *)node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046b900
void FUN_0046b900(struct Vec4 *param_1) {
    struct EventNodeVec *node;

    node = (struct EventNodeVec *)FUN_00468910(9, 1);
    node->vec_28 = *param_1;
    node->flags_10 = 0;
    FUN_0046b630((struct EventNode *)node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046b940
void FUN_0046b940(struct Vec4 *param_1) {
    struct EventNodeVec *node;

    node = (struct EventNodeVec *)FUN_00468910(0xa, 1);
    node->vec_28 = *param_1;
    node->flags_10 = 0;
    FUN_0046b630((struct EventNode *)node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046b980
void FUN_0046b980(struct Vec4 *param_1) {
    struct EventNodeVec *node;

    node = (struct EventNodeVec *)FUN_00468910(0xb, 1);
    node->vec_28 = *param_1;
    node->flags_10 = 0;
    FUN_0046b630((struct EventNode *)node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046b9c0
void FUN_0046b9c0(unsigned int param_1) {
    struct EventNode *node;

    node = FUN_00468910(0xc, 1);
    FUN_00468b40(node, param_1, 1);
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046b9f0
void FUN_0046b9f0(unsigned int param_1) {
    struct EventNode *node;

    node = FUN_00468910(0xd, 1);
    FUN_00468b40(node, param_1, 1);
    node->field_1c = 0;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046ba30
void FUN_0046ba30(unsigned int param_1) {
    struct EventNode *node;

    node = FUN_00468910(0xe, 1);
    FUN_00468b40(node, param_1, 1);
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046ba60
void FUN_0046ba60(unsigned int param_1, unsigned int param_2) {
    struct EventNode *node;

    node = FUN_00468910(0xf, 1);
    node->field_1c = param_1;
    node->field_14 = param_2;
    node->flags_10 = 0;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046ba90
void FUN_0046ba90(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    struct EventNode *node;

    node = FUN_00468910(0x18, 1);
    node->field_18 = param_1;
    node->field_1c = param_2;
    node->field_14 = param_3;
    node->flags_10 = 0;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046bad0
void FUN_0046bad0(unsigned int param_1, unsigned int param_2, unsigned int *param_3) {
    struct EventNode *node;

    node = FUN_00468910(0x10, 1);
    node->field_1c = param_2;
    node->field_20 = param_3[0];
    node->field_24 = param_3[1];
    node->field_14 = param_1;
    node->flags_10 = 0;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046bb10
void FUN_0046bb10(unsigned int param_1, unsigned int param_2) {
    struct EventNode *node;

    node = FUN_00468910(0x11, 1);
    node->field_1c = param_1;
    node->field_14 = param_2;
    node->flags_10 = 0;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046bb40
void FUN_0046bb40(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    struct EventNode *node;

    node = FUN_00468910(0x12, 1);
    node->field_4 = param_1;
    node->field_14 = param_2;
    node->field_1c = param_3;
    node->flags_10 = 0;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046bb80
void FUN_0046bb80(unsigned int param_1, unsigned int param_2) {
    struct EventNode *node;

    node = FUN_00468910(0x13, 1);
    node->field_1c = param_2;
    node->field_14 = param_1;
    node->flags_10 = 0;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046bbb0
void FUN_0046bbb0(unsigned int param_1, unsigned int param_2) {
    struct EventNode *node;

    node = FUN_00468910(0x14, 1);
    node->field_1c = param_2;
    node->field_14 = param_1;
    node->flags_10 = 0;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046bbe0
void FUN_0046bbe0(unsigned int param_1, unsigned int param_2) {
    struct EventNode *node;

    node = FUN_00468910(0x15, 1);
    node->field_1c = param_2;
    node->field_14 = param_1;
    node->flags_10 = 0;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046bc10
void FUN_0046bc10(unsigned int param_1) {
    struct EventNode *node;

    node = FUN_00468910(0x16, 1);
    node->flags_10 = 0;
    node->field_1c = param_1;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046bc40
void FUN_0046bc40(void) {
    struct EventNode *node;

    node = FUN_00468910(0x20, 1);
    node->flags_10 = 0;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046bc60
void FUN_0046bc60(void) {
    struct EventNode *node;

    node = FUN_00468910(0x1f, 1);
    node->flags_10 = 0;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046bc80
void FUN_0046bc80(unsigned int param_1, unsigned int param_2) {
    struct EventNode *node;

    node = FUN_00468910(0x19, 1);
    node->field_1c = param_2;
    node->field_14 = param_1;
    node->flags_10 = 0;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046bcb0
void FUN_0046bcb0(unsigned int param_1, unsigned int param_2) {
    struct EventNode *node;

    node = FUN_00468910(0x1a, 1);
    node->field_1c = param_2;
    node->field_14 = param_1;
    node->flags_10 = 0;
    FUN_0046b630(node, DAT_0066879c);
}

// FUNCTION: LEGOLAND 0x0046bce0
void FUN_0046bce0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bd10
void FUN_0046bd10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bd40
void FUN_0046bd40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bd70
void FUN_0046bd70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bda0
void FUN_0046bda0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bdd0
void FUN_0046bdd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046be00
void FUN_0046be00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046be40
void FUN_0046be40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046be90
void FUN_0046be90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bec0
void FUN_0046bec0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bef0
void FUN_0046bef0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bf30
void FUN_0046bf30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bf80
void FUN_0046bf80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bfb0
void FUN_0046bfb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bff0
void FUN_0046bff0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c030
void FUN_0046c030(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c060
void FUN_0046c060(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c090
void FUN_0046c090(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c0c0
void FUN_0046c0c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c0f0
void FUN_0046c0f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c120
void FUN_0046c120(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c150
void FUN_0046c150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c180
void FUN_0046c180(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c1b0
void FUN_0046c1b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c1e0
void FUN_0046c1e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c210
void FUN_0046c210(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c240
void FUN_0046c240(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c290
void FUN_0046c290(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c2c0
void FUN_0046c2c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c2f0
void FUN_0046c2f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c320
void FUN_0046c320(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c350
void FUN_0046c350(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c390
void FUN_0046c390(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c3c0
void FUN_0046c3c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c3f0
void FUN_0046c3f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c420
void FUN_0046c420(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c450
void FUN_0046c450(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c480
void FUN_0046c480(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c4b0
void FUN_0046c4b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c510
void FUN_0046c510(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c540
void FUN_0046c540(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c580
void FUN_0046c580(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c5c0
void FUN_0046c5c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c620
void FUN_0046c620(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c680
void FUN_0046c680(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c700
void FUN_0046c700(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c7e0
void FUN_0046c7e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046c920
void FUN_0046c920(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046cb20
void FUN_0046cb20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046cb60
void FUN_0046cb60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046ce00
void FUN_0046ce00(void) { STUB(); }
