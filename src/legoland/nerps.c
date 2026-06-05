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
extern void FUN_00459970(void);
extern int GetBrickCount(void);
extern int DBPrintf(const char *format, ...);

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
void FUN_0046afe0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b080
void FUN_0046b080(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b0b0
void FUN_0046b0b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b0c0
void FUN_0046b0c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b100
void FUN_0046b100(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b130
void FUN_0046b130(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b180
void FUN_0046b180(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b1e0
void FUN_0046b1e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b1f0
void FUN_0046b1f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b200
void FUN_0046b200(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b240
void FUN_0046b240(unsigned int param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b280
void FUN_0046b280(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b290
void FUN_0046b290(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b2d0
void FUN_0046b2d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b4f0
void FUN_0046b4f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b520
void FUN_0046b520(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b560
void FUN_0046b560(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b590
void FUN_0046b590(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b5d0
void FUN_0046b5d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b610
void FUN_0046b610(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b630
void FUN_0046b630(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b650
void FUN_0046b650(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b6b0
void FUN_0046b6b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b700
void FUN_0046b700(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b760
void FUN_0046b760(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b790
void FUN_0046b790(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b7f0
void FUN_0046b7f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b820
void FUN_0046b820(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b850
void FUN_0046b850(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b880
void FUN_0046b880(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b8c0
void FUN_0046b8c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b900
void FUN_0046b900(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b940
void FUN_0046b940(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b980
void FUN_0046b980(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b9c0
void FUN_0046b9c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046b9f0
void FUN_0046b9f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046ba30
void FUN_0046ba30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046ba60
void FUN_0046ba60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046ba90
void FUN_0046ba90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bad0
void FUN_0046bad0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bb10
void FUN_0046bb10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bb40
void FUN_0046bb40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bb80
void FUN_0046bb80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bbb0
void FUN_0046bbb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bbe0
void FUN_0046bbe0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bc10
void FUN_0046bc10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bc40
void FUN_0046bc40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bc60
void FUN_0046bc60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bc80
void FUN_0046bc80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0046bcb0
void FUN_0046bcb0(void) { STUB(); }

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
