#include "legoland.h"

extern unsigned int DAT_00669098;
extern unsigned int DAT_0066879c;
extern unsigned int DAT_007fdca4;

extern unsigned int DAT_004bb5b0;

extern void FUN_004785d0(char *param_1, unsigned int param_2);
extern void FUN_0046b590(unsigned int param_1);
extern unsigned int FUN_0046b4f0(unsigned int param_1);
extern int FUN_004786a0(unsigned int param_1, unsigned int param_2, unsigned int param_3);
extern int FUN_00478690(unsigned int param_1, unsigned int param_2, unsigned int param_3);
extern unsigned int FUN_004786c0(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);
extern void FUN_00478610(unsigned int param_1);
extern void FUN_00478650(unsigned int param_1, unsigned int param_2);
extern unsigned int FUN_004787a0(unsigned int param_1, unsigned int param_2);
extern unsigned int ElemID(unsigned int id);
extern unsigned int LoadObjectClass(unsigned int cls);
extern void FUN_00469ab0(unsigned int object);
extern void ClearObjectCounters(void);
extern unsigned int LoadBaseMap(unsigned int param_1);
extern void CalculateMapRenderOrder(void);
extern void FUN_0045a060(void);
extern void FUN_0046bdd0(unsigned char param_1, unsigned int param_2, unsigned int param_3);
extern void FUN_0046bef0(unsigned char param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);
extern void FUN_0046c030(unsigned char param_1, unsigned int param_2, unsigned int param_3);
extern void FUN_0046c090(unsigned char param_1, unsigned int param_2);
extern void FUN_0046c0c0(unsigned char param_1, unsigned int param_2, unsigned int param_3);
extern void FUN_0046c120(unsigned char param_1, unsigned int param_2);
extern void FUN_0046c290(unsigned char param_1, unsigned int param_2);
extern void FUN_0046c2f0(unsigned char param_1, unsigned int param_2);
extern void FUN_0046c320(unsigned char param_1, unsigned int param_2);
extern void FUN_0046c390(unsigned char param_1, unsigned int param_2, unsigned int param_3);
extern void FUN_0046c3c0(unsigned char param_1, unsigned int param_2);
extern void FUN_0046c420(unsigned char param_1, unsigned int param_2, unsigned int param_3);
extern void FUN_00468890(unsigned int param_1, unsigned int param_2);
extern void FUN_0046bcb0(unsigned int param_1, unsigned int param_2);
extern void FUN_0046b790(unsigned int param_1, unsigned int param_2);
extern void FUN_0046b7f0(unsigned int param_1);
extern void FUN_0046b9c0(unsigned int param_1);
extern void FUN_00490610(unsigned int param_1);
extern void FUN_0046ba30(unsigned int param_1);
extern int FUN_004781b0(const char *param_1, const void *param_2, int param_3);
extern void FUN_00482d60(int param_1, int param_2);
extern void FUN_00462e50(unsigned int param_1, int param_2);
extern void FUN_0046bbb0(unsigned int param_1, int param_2);
extern void FUN_0046bb80(unsigned int param_1, int param_2);
extern void FUN_0046bc60(void);
extern void FUN_0046bc40(void);
extern int FUN_00492980(void);
extern void LLIDB_ClearOnLevel(void);
extern int FUN_004784c0(void);
extern int FUN_004781f0(int param_1, const void *param_2, int param_3, int param_4);
extern void FUN_00492990(void);
extern int __strcmpi(const char *s1, const char *s2);
extern int FUN_004a04b9(const char *str);

extern unsigned char DAT_00669050;
extern unsigned int DAT_00669054;
extern unsigned int DAT_00832924;
extern unsigned int DAT_004bb688;
extern unsigned int DAT_004bb6bc;
extern unsigned int DAT_004bb6f8;

struct GameObject {
    unsigned char pad_0[8];
    unsigned int flags;
};

struct CommandArgs {
    unsigned char pad_0[4];
    unsigned int field_4;
    char *field_8;
    char *field_c;
};

unsigned int FUN_00478be0(unsigned int param_1, unsigned int param_2, unsigned int param_3);

// FUNCTION: LEGOLAND 0x004787b0
int FUN_004787b0(char **pp_str) {
    if (strlen(*pp_str) == 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004787d0
void FUN_004787d0(void) {
    if (DAT_0066879c != 0) {
        FUN_0046b590(DAT_0066879c);
    }
    DAT_0066879c = 0;
}

// FUNCTION: LEGOLAND 0x004787f0
void FUN_004787f0(void) {
    unsigned int id = DAT_00669098;
    DAT_00669098 = DAT_00669098 + 1;
    DAT_0066879c = FUN_0046b4f0(id);
    DAT_007fdca4 = 0;
}

// FUNCTION: LEGOLAND 0x00478820
int FUN_00478820(void) {
    // STRING: LEGOLAND 0x004bc0a4
    FUN_004785d0("Uninitialised", 0);
    return 1;
}

// FUNCTION: LEGOLAND 0x00478840
int FUN_00478840(void) {
    // STRING: LEGOLAND 0x004bc0b4
    FUN_004785d0("Closed", 0);
    FUN_004787d0();
    DAT_0066879c = 0;
    return 1;
}

// FUNCTION: LEGOLAND 0x00478870
int FUN_00478870(unsigned int *param_1) {
    FUN_004785d0((char *)*param_1, 1);
    return 1;
}

// FUNCTION: LEGOLAND 0x00478890
void FUN_00478890(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00478930
void FUN_00478930(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00478980
int FUN_00478980(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786a0(param_1, param_2, 2) == 0) {
        return 0;
    }
    FUN_00478610(0);
    return 1;
}

// FUNCTION: LEGOLAND 0x004789c0
void FUN_004789c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00478a00
void FUN_00478a00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00478a40
int FUN_00478a40(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786a0(param_1, param_2, 2) == 0) {
        return 0;
    }
    FUN_00478610(3);
    FUN_00478650(param_1, param_2);
    return 1;
}

// FUNCTION: LEGOLAND 0x00478a80
int FUN_00478a80(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bb5b0 != 0) {
        if (FUN_004786a0(param_1, param_2, 2) == 0) {
            return 0;
        }
        FUN_004785d0(*(char **)param_1, 4);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00478ac0
int FUN_00478ac0(unsigned int param_1, unsigned int param_2) {
    unsigned int map;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_00478690(param_1, param_2, 1) == 0) {
        return 0;
    }
    map = FUN_004787a0(param_1, param_2);
    ClearObjectCounters();
    if (LoadBaseMap(map) == 0) {
        return -1;
    }
    CalculateMapRenderOrder();
    FUN_0045a060();
    return 1;
}

// FUNCTION: LEGOLAND 0x00478b20
int FUN_00478b20(unsigned int arg) { STUB(); }

// FUNCTION: LEGOLAND 0x00478b70
int FUN_00478b70(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0(param_1, param_2, 1, 1) == 0) {
        return 0;
    }
    return FUN_00478b20(FUN_004787a0(param_1, param_2)) != 0;
}

// FUNCTION: LEGOLAND 0x00478bc0
unsigned int FUN_00478bc0(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    return FUN_00478be0(param_1, param_2, param_3);
}

// FUNCTION: LEGOLAND 0x00478be0
unsigned int FUN_00478be0(unsigned int param_1, unsigned int param_2, unsigned int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x00478c60
void FUN_00478c60(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00478cd0
void FUN_00478cd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00478d30
void FUN_00478d30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00478e20
void FUN_00478e20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00478e90
void FUN_00478e90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00478f00
void FUN_00478f00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00478fa0
void FUN_00478fa0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479060
void FUN_00479060(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479120
void FUN_00479120(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004791a0
void FUN_004791a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004791f0
int FUN_004791f0(struct CommandArgs *arg, int argc) {
    unsigned int id;
    unsigned int count;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 2, 1) == 0) {
        return 0;
    }
    id = ElemID(arg->field_4);
    if (argc <= 1) {
        count = 1;
    } else {
        count = FUN_004a04b9(arg->field_8);
        if (count == 0) {
            count = 1;
        }
    }
    if (id != 0) {
        FUN_0046bdd0(DAT_00669050, id, count);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00479270
void FUN_00479270(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479300
void FUN_00479300(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479390
void FUN_00479390(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004793e0
void FUN_004793e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479450
int FUN_00479450(struct CommandArgs *arg, int argc) {
    unsigned int id;
    int v1;
    unsigned int v2;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 2, 2) == 0) {
        return 0;
    }
    id = ElemID(arg->field_4);
    v1 = FUN_004a04b9(arg->field_8);
    if (argc >= 3) {
        v2 = FUN_004a04b9(arg->field_c);
    } else {
        v2 = 0;
    }
    if (id != 0) {
        FUN_0046bef0(DAT_00669050, id, v2, v1);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004794d0
void FUN_004794d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479550
void FUN_00479550(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004795c0
void FUN_004795c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479640
void FUN_00479640(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004796d0
int FUN_004796d0(struct CommandArgs *arg, int argc) {
    unsigned int id;
    int count;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 2, 2) == 0) {
        return 0;
    }
    id = ElemID(arg->field_4);
    count = FUN_004a04b9(arg->field_8);
    if (count == 0) {
        count = 1;
    }
    if (id != 0) {
        FUN_0046c030(DAT_00669050, id, count);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00479740
void FUN_00479740(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004797b0
void FUN_004797b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479800
int FUN_00479800(struct CommandArgs *arg, int argc) {
    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 2, 1) == 0) {
        return 0;
    }
    FUN_0046c090(DAT_00669050, FUN_004a04b9((char *)arg->field_4));
    return 1;
}

// FUNCTION: LEGOLAND 0x00479850
int FUN_00479850(struct CommandArgs *arg, int argc) {
    unsigned int id;
    int value;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 2, 2) == 0) {
        return 0;
    }
    id = ElemID(arg->field_4);
    value = FUN_004a04b9(arg->field_8);
    if (id != 0) {
        FUN_0046c0c0(DAT_00669050, id, value);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004798c0
void FUN_004798c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479930
int FUN_00479930(struct CommandArgs *arg, int argc) {
    int value;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 2, 1) == 0) {
        return 0;
    }
    value = FUN_004a04b9((char *)arg->field_4);
    FUN_0046c120(DAT_00669050, value);
    return 1;
}

// FUNCTION: LEGOLAND 0x00479980
void FUN_00479980(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004799d0
void FUN_004799d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479a20
void FUN_00479a20(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479a70
void FUN_00479a70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479ac0
void FUN_00479ac0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479b10
void FUN_00479b10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479c40
void FUN_00479c40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479cb0
int FUN_00479cb0(struct CommandArgs *arg, int argc) {
    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 2, 1) == 0) {
        return 0;
    }
    FUN_0046c290(DAT_00669050, FUN_004a04b9((char *)arg->field_4));
    return 1;
}

// FUNCTION: LEGOLAND 0x00479d00
void FUN_00479d00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479d60
int FUN_00479d60(struct CommandArgs *arg, int argc) {
    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 2, 1) == 0) {
        return 0;
    }
    FUN_0046c2f0(DAT_00669050, FUN_004a04b9((char *)arg->field_4));
    return 1;
}

// FUNCTION: LEGOLAND 0x00479db0
int FUN_00479db0(struct CommandArgs *arg, int argc) {
    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 2, 1) == 0) {
        return 0;
    }
    FUN_0046c320(DAT_00669050, FUN_004a04b9((char *)arg->field_4));
    return 1;
}

// FUNCTION: LEGOLAND 0x00479e00
void FUN_00479e00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479e80
int FUN_00479e80(struct CommandArgs *arg, int argc) {
    int n1;
    int n2;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 2, 2) == 0) {
        return 0;
    }
    n1 = FUN_004a04b9((char *)arg->field_4);
    n2 = FUN_004a04b9(arg->field_8);
    FUN_0046c390(DAT_00669050, n1, n2);
    return 1;
}

// FUNCTION: LEGOLAND 0x00479ee0
int FUN_00479ee0(struct CommandArgs *arg, int argc) {
    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 2, 1) == 0) {
        return 0;
    }
    FUN_0046c3c0(DAT_00669050, FUN_004a04b9((char *)arg->field_4));
    return 1;
}

// FUNCTION: LEGOLAND 0x00479f30
void FUN_00479f30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00479fa0
int FUN_00479fa0(struct CommandArgs *arg, int argc) {
    int n1;
    int n2;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 2, 1) == 0) {
        return 0;
    }
    n1 = FUN_004a04b9((char *)arg->field_4);
    if (argc >= 2) {
        n2 = FUN_004a04b9(arg->field_8);
    } else {
        n2 = 1;
    }
    FUN_0046c420(DAT_00669050, n1, n2);
    return 1;
}

// FUNCTION: LEGOLAND 0x0047a020
void FUN_0047a020(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a0b0
int FUN_0047a0b0(char **argv, int argc) {
    int v1;
    int v2;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)argv, argc, 5, 1) == 0) {
        return 0;
    }
    v1 = FUN_004a04b9(argv[1]);
    if (argc >= 2) {
        v2 = FUN_004a04b9(argv[2]);
    } else {
        v2 = 1;
    }
    if (DAT_00669054 == 1) {
        FUN_00468890(v1, v2);
    } else {
        FUN_0046bcb0(v1, v2);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0047a140
void FUN_0047a140(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a1d0
void FUN_0047a1d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a2f0
void FUN_0047a2f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a360
void FUN_0047a360(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a3d0
void FUN_0047a3d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a440
void FUN_0047a440(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a480
int FUN_0047a480(struct CommandArgs *arg, int argc) {
    unsigned int id;
    int popup = 1;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 4, 1) == 0) {
        return 0;
    }
    id = ElemID(arg->field_4);
    if (argc >= 2) {
        // STRING: LEGOLAND 0x004bc0c4
        if (__strcmpi(arg->field_8, "NOPOPUP") == 0) {
            popup = 0;
        }
    }
    if (id != 0) {
        FUN_0046b790(id, popup);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0047a500
int FUN_0047a500(struct CommandArgs *arg, int argc) {
    unsigned int id;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 4, 1) == 0) {
        return 0;
    }
    id = ElemID(arg->field_4);
    if (id != 0) {
        FUN_0046b7f0(id);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0047a550
void FUN_0047a550(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a5a0
void FUN_0047a5a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a650
void FUN_0047a650(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a6a0
void FUN_0047a6a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a6f0
void FUN_0047a6f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a7b0
void FUN_0047a7b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a800
int FUN_0047a800(struct CommandArgs *arg, int argc) {
    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 5, 1) == 0) {
        return 0;
    }
    if (DAT_00669054 == 4) {
        FUN_0046b9c0(arg->field_4);
    } else {
        FUN_00490610(arg->field_4);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0047a860
void FUN_0047a860(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a8a0
int FUN_0047a8a0(struct CommandArgs *arg, int argc) {
    if (DAT_004bb5b0 != 0) {
        if (FUN_004786c0((unsigned int)arg, argc, 4, 1) == 0) {
            return 0;
        }
        FUN_0046ba30(arg->field_4);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0047a8e0
void FUN_0047a8e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047a960
void FUN_0047a960(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047aa90
int FUN_0047aa90(struct CommandArgs *arg, int argc) {
    int n;
    int r;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 1, 2) == 0) {
        return 0;
    }
    n = FUN_004a04b9(arg->field_8);
    r = FUN_004781b0((char *)arg->field_4, &DAT_004bb688, 13);
    if (r == -1) {
        return 0;
    }
    FUN_00482d60(r, n);
    return 1;
}

// FUNCTION: LEGOLAND 0x0047ab00
int FUN_0047ab00(struct CommandArgs *arg, int argc) {
    int index;
    int value;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 5, 2) == 0) {
        return 0;
    }
    index = FUN_004781b0((char *)arg->field_4, &DAT_004bb6bc, 6);
    value = FUN_004a04b9(arg->field_8);
    if (index == -1) {
        return 0;
    }
    if (DAT_00669054 == 1) {
        FUN_00462e50(index, value);
    } else {
        FUN_0046bbb0(index, value);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0047ab80
void FUN_0047ab80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047ac00
void FUN_0047ac00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047ac80
void FUN_0047ac80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047ace0
void FUN_0047ace0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047ad40
int FUN_0047ad40(struct CommandArgs *arg, int argc) {
    unsigned int value;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 5, 1) == 0) {
        return 0;
    }
    value = FUN_004a04b9((char *)arg->field_4);
    if (DAT_00669054 == 1) {
        DAT_00832924 = value;
    } else {
        FUN_0046bb80(0, value);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0047ada0
void FUN_0047ada0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047ae00
void FUN_0047ae00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047aea0
void FUN_0047aea0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047af50
int FUN_0047af50(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786a0(param_1, param_2, 4) == 0) {
        return 0;
    }
    FUN_0046bc60();
    return 1;
}

// FUNCTION: LEGOLAND 0x0047af80
int FUN_0047af80(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bb5b0 != 0) {
        if (FUN_004786a0(param_1, param_2, 4) == 0) {
            return 0;
        }
        FUN_0046bc40();
        return 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0047afb0
int FUN_0047afb0(int param_1) {
    int result;
    int v1;

    FUN_00492980();
    LLIDB_ClearOnLevel();
    FUN_004784c0();
    result = FUN_004781f0(param_1, &DAT_004bb6f8, 0x5d, 0);
    FUN_00492990();
    v1 = (result >= 0) - 1;
    return v1 & 2;
}
