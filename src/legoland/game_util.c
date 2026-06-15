#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "bloke.h"
#include "bricks.h"
#include "draw.h"
#include "game_util.h"
#include "gamemain.h"
#include "gamemap.h"
#include "globals.h"
#include "llidb.h"
#include "map_object.h"
#include "nerps.h"
#include "objclass.h"
#include "objectives.h"
#include "sound_sfx.h"
#include "title.h"
#include "worker.h"

struct CommandArgs {
    unsigned char pad_0[4];
    unsigned int field_4;
    char *field_8;
    char *field_c;
};

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
        /* TODO: fold into struct SortNode/NerpsListNode views of DAT_0066879c */
        FUN_0046b590((struct SortNode *)DAT_0066879c);
    }
    DAT_0066879c = 0;
}

// FUNCTION: LEGOLAND 0x004787f0
void FUN_004787f0(void) {
    unsigned int id = DAT_00669098;
    DAT_00669098 = DAT_00669098 + 1;
    DAT_0066879c = (unsigned int)FUN_0046b4f0(id);
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
int FUN_00478890(struct CommandArgs *arg, int argc) {
    unsigned int n1;
    unsigned int n2;

    if (argc == 0) {
        DAT_004bb5b0 = 1;
        return 1;
    }
    if (argc == 1) {
        n1 = atoi((char *)arg->field_4);
        DAT_004bb5b0 = n1 <= DAT_0080ffc0;
        return 1;
    }
    n1 = atoi((char *)arg->field_4);
    n2 = atoi(arg->field_8);
    if (n1 > n2) {
        return 0;
    }
    if (DAT_0080ffc0 < n1 || (DAT_004bb5b0 = 1, DAT_0080ffc0 > n2)) {
        DAT_004bb5b0 = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00478930
int FUN_00478930(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bb5b0 != 0) {
        if (FUN_004786a0(param_1, param_2, 7) == 0) {
            return 0;
        }
        FUN_004785d0(*(char **)param_1, 2);
        FUN_00478610(1);
        FUN_004787d0();
        FUN_004787f0();
    }
    return 1;
}

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
int FUN_004789c0(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786a0(param_1, param_2, 2) == 0) {
        return 0;
    }
    FUN_00478610(1);
    return 1;
}

// FUNCTION: LEGOLAND 0x00478a00
int FUN_00478a00(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786a0(param_1, param_2, 2) == 0) {
        return 0;
    }
    FUN_00478610(2);
    FUN_00478650(param_1, param_2);
    return 1;
}

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
int FUN_00478b20(unsigned int arg) {
    struct GameObject *obj = (struct GameObject *)ElemID((const char *)arg);
    if (obj == NULL) {
        return 0;
    }
    if ((obj->flags & 4) == 0) {
        if (LoadObjectClass(obj) == 0) {
            return 0;
        }
        obj->flags |= 4;
        FUN_00469ab0(obj);
    }
    return 1;
}

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
unsigned int FUN_00478be0(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    unsigned int name;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0(param_1, param_2, 5, 1) == 0) {
        return 0;
    }
    if (DAT_00669054 == 1) {
        name = FUN_004787a0(param_1, param_2);
        if (FUN_00478b20(name) != 0) {
            FUN_00469900((struct NerpsArg *)ElemID((const char *)name), 0, 1);
        }
        return 1;
    }
    return FUN_0047a480(param_1, param_2, param_3);
}

// FUNCTION: LEGOLAND 0x00478c60
int FUN_00478c60(struct CommandArgs *arg, int argc) {
    if (DAT_004bb5b0 != 0) {
        if (FUN_004786c0((unsigned int)arg, argc, 5, 1) == 0) {
            return 0;
        }
        if (DAT_00669054 == 1) {
            FUN_00457900(atoi((char *)arg->field_4));
            return 1;
        }
        FUN_0046b850(atoi((char *)arg->field_4));
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00478cd0
int FUN_00478cd0(char **argv, int argc) {
    int *out;

    if (DAT_004bb5b0 != 0) {
        if (FUN_004786c0((unsigned int)argv, argc, 5, 5) == 0) {
            return 0;
        }
        if (DAT_00669054 == 1) {
            out = &DAT_00832928;
            do {
                argv = argv + 1;
                *out = atoi(*argv);
                out = out + 1;
            } while ((int)out < (int)DAT_0083293c);
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00478d30
int FUN_00478d30(int param_1, int param_2) {
    int local[2];
    int a8;
    int b8;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0(param_1, param_2, 5, 2) == 0) {
        return 0;
    }
    FUN_00478770(local, (char **)param_1, 1);
    a8 = local[0] << 8;
    b8 = local[1] << 8;
    local[0] = a8;
    local[1] = b8;
    if (DAT_00669054 == 1) {
        GetTileDimensions(&local[0], &local[1]);
        ScrollX = ((((a8 - b8) * local[0]) >> 9) - (lpConfig->field_0 >> 1)) << 8;
        ScrollY = ((((a8 + b8) * local[1]) >> 9) - (lpConfig->field_2 >> 1)) << 8;
        return 1;
    }
    FUN_0046bda0((unsigned int *)local);
    return 1;
}

// FUNCTION: LEGOLAND 0x00478e20
int FUN_00478e20(int param_1, int param_2) {
    char *name = DAT_004d8bb0;
    if (DAT_004bb5b0 != 0) {
        if (FUN_004786c0(param_1, param_2, 5, 0) == 0) {
            return 0;
        }
        if (param_2 >= 1) {
            name = *(char **)(param_1 + 4);
        }
        if (DAT_00669054 == 1) {
            FUN_004687f0(name);
            return 1;
        }
        FUN_0046bd10((unsigned int)name);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00478e90
int FUN_00478e90(int param_1, int param_2) {
    char *name = DAT_004d8bb0;
    if (DAT_004bb5b0 != 0) {
        if (FUN_004786c0(param_1, param_2, 5, 0) == 0) {
            return 0;
        }
        if (param_2 >= 1) {
            name = *(char **)(param_1 + 4);
        }
        if (DAT_00669054 == 1) {
            FUN_00468810(name);
            return 1;
        }
        FUN_0046bd40((unsigned int)name);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00478f00
int FUN_00478f00(struct CommandArgs *arg, int argc) {
    int v1;
    int v2;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 5, 2) == 0) {
        return 0;
    }
    v1 = atoi((char *)arg->field_4);
    v2 = atoi(arg->field_8);
    if (DAT_00669054 == 1) {
        if (v1 >= 0) {
            lpConfig->field_38 = v1 != 0;
        }
        if (v2 >= 0) {
            lpConfig->field_34 = v2 != 0;
            return 1;
        }
    } else {
        FUN_0046bb10(v1, v2);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00478fa0
int FUN_00478fa0(struct CommandArgs *arg, int argc) {
    int coords[2];
    int count;

    if (DAT_004bb5b0 != 0) {
        if (FUN_004786c0((unsigned int)arg, argc, 5, 2) == 0) {
            return 0;
        }
        coords[0] = atoi((char *)arg->field_4);
        coords[1] = atoi(arg->field_8);
        if (argc < 3 || (count = atoi(arg->field_c)) < 1) {
            count = 1;
        }
        if (DAT_00669054 == 1) {
            int i = count;
            if (i != 0) {
                do {
                    GenerateGardener(coords, 0);
                    i = i - 1;
                } while (i != 0);
                return 1;
            }
        } else {
            FUN_0046bad0(1, count, (unsigned int *)coords);
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00479060
int FUN_00479060(struct CommandArgs *arg, int argc) {
    int coords[2];
    int count;

    if (DAT_004bb5b0 != 0) {
        if (FUN_004786c0((unsigned int)arg, argc, 5, 2) == 0) {
            return 0;
        }
        coords[0] = atoi((char *)arg->field_4);
        coords[1] = atoi(arg->field_8);
        if (argc < 3 || (count = atoi(arg->field_c)) < 1) {
            count = 1;
        }
        if (DAT_00669054 == 1) {
            int i = count;
            if (i != 0) {
                do {
                    GenerateMechanic(coords, 0);
                    i = i - 1;
                } while (i != 0);
                return 1;
            }
        } else {
            FUN_0046bad0(0, count, (unsigned int *)coords);
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00479120
int FUN_00479120(struct CommandArgs *arg, int argc) {
    if (DAT_004bb5b0 != 0) {
        if (FUN_004786c0((unsigned int)arg, argc, 2, 0) == 0) {
            return 0;
        }
        if (argc > 0) {
            if (argc > 1) {
                DAT_007fdca4 = FUN_004689f0((char *)arg->field_4, arg->field_8, 1);
                return 1;
            }
            DAT_007fdca4 = FUN_004689f0((char *)arg->field_4, NULL, 1);
            return 1;
        }
        DAT_007fdca4 = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004791a0
int FUN_004791a0(struct CommandArgs *arg, int argc) {
    if (DAT_004bb5b0 != 0) {
        if (FUN_004786c0((unsigned int)arg, argc, 2, 1) == 0) {
            return 0;
        }
        if (DAT_0066879c != 0) {
            FUN_0046b650((const char *)arg->field_4, (struct StringHolder *)DAT_0066879c);
        }
    }
    return 1;
}

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
    id = ElemID((const char *)arg->field_4);
    if (argc <= 1) {
        count = 1;
    } else {
        count = atoi(arg->field_8);
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
int FUN_00479270(struct CommandArgs *arg, int argc) {
    unsigned int id;
    unsigned int vals[2];

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 2, 3) == 0) {
        return 0;
    }
    id = ElemID((const char *)arg->field_4);
    vals[0] = atoi(arg->field_8);
    vals[1] = atoi(arg->field_c);
    if (id != 0) {
        FUN_0046be00(DAT_00669050, id, vals);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00479300
int FUN_00479300(struct CommandArgs *arg, int argc) {
    unsigned int id;
    unsigned int v;
    struct Vec4 vec;

    if (DAT_004bb5b0 == 0) {
        return 1;
    }
    if (FUN_004786c0((unsigned int)arg, argc, 2, 6) == 0) {
        return 0;
    }
    id = ElemID((const char *)arg->field_4);
    v = atoi(arg->field_8);
    FUN_00478700((int *)&vec, (char **)arg, 3);
    if (id != 0) {
        FUN_0046be40(DAT_00669050, id, v, &vec);
    }
    return 1;
}

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
    id = ElemID((const char *)arg->field_4);
    v1 = atoi(arg->field_8);
    if (argc >= 3) {
        v2 = atoi(arg->field_c);
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
    id = ElemID((const char *)arg->field_4);
    count = atoi(arg->field_8);
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
    FUN_0046c090(DAT_00669050, atoi((char *)arg->field_4));
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
    id = ElemID((const char *)arg->field_4);
    value = atoi(arg->field_8);
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
    value = atoi((char *)arg->field_4);
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
    FUN_0046c290(DAT_00669050, atoi((char *)arg->field_4));
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
    FUN_0046c2f0(DAT_00669050, atoi((char *)arg->field_4));
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
    FUN_0046c320(DAT_00669050, atoi((char *)arg->field_4));
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
    n1 = atoi((char *)arg->field_4);
    n2 = atoi(arg->field_8);
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
    FUN_0046c3c0(DAT_00669050, atoi((char *)arg->field_4));
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
    n1 = atoi((char *)arg->field_4);
    if (argc >= 2) {
        n2 = atoi(arg->field_8);
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
    v1 = atoi(argv[1]);
    if (argc >= 2) {
        v2 = atoi(argv[2]);
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
    id = ElemID((const char *)arg->field_4);
    if (argc >= 2) {
        // STRING: LEGOLAND 0x004bc0c4
        if (_stricmp(arg->field_8, "NOPOPUP") == 0) {
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
    id = ElemID((const char *)arg->field_4);
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
        FUN_00490610((const char *)arg->field_4);
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
    n = atoi(arg->field_8);
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
    value = atoi(arg->field_8);
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
    value = atoi((char *)arg->field_4);
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
