#include "legoland.h"

#include "gamemain.h"
#include "gamemap.h"
#include "title.h"

struct GameMainNode {
    struct GameMainNode *next;
    unsigned char pad_4[0x4];
    unsigned int field_8;
    unsigned int field_c;
};

struct GameMainArg {
    unsigned int field_0;
    unsigned int field_4;
};

struct GameMainEntry {
    unsigned char pad_0[0x4];
    char *name;
};

struct LegoConfig {
    unsigned char pad_0[0x1a];
    unsigned short field_1a;
    unsigned char pad_1c[0x30 - 0x1c];
    unsigned int field_30;
    unsigned int field_34;
    unsigned int field_38;
};

extern struct GameMainNode *DAT_00668fc0;
extern struct LegoConfig *lpConfig;

extern unsigned short NEWFLC_Repeat;
extern unsigned int NEWFLC_BuffSize;
extern unsigned int NEWFLC_CheckDuplicate;
extern unsigned int DAT_00669054;
extern char DAT_00669058[0x100];
extern unsigned char DAT_00669050;
extern unsigned int DAT_004bb5ac;
extern unsigned int DAT_0066879c;
extern unsigned int DAT_00669098;
extern unsigned int DAT_00832920;
extern unsigned int DAT_00832924;
extern unsigned int DAT_007fdca4;
extern unsigned int DAT_004bb5b0;
extern unsigned int DAT_00832974;
extern unsigned int DAT_00832994;
extern unsigned int DAT_00832980;
extern unsigned int DAT_00832ba8;

extern int __strcmpi(const char *s1, const char *s2);
extern int FUN_004a04b9(const char *str);
extern char *strcpy(char *dst, const char *src);

extern void FUN_004689a0(void);
extern unsigned int FUN_004689f0(unsigned int param_1, unsigned int param_2, unsigned int param_3);
extern void FUN_004441f0(void);
extern void FUN_0044db20(void);
extern void FUN_0044db80(void);
extern void FUN_00468840(void);
extern void FUN_004688e0(void);
extern void FUN_0044dc70(unsigned int param_1, unsigned int param_2);
extern void FUN_00468830(void);
extern void FUN_00482d70(void);
extern void FUN_00462e90(void);
extern void FUN_00476000(void);
extern void FUN_00476050(void);
extern void FUN_00463560(void);
extern void FUN_00482b10(void);
extern void FUN_0046b240(unsigned int param_1);

extern char DAT_004d8bb0[0x100];

// FUNCTION: LEGOLAND 0x004777c0
struct GameMainNode *FUN_004777c0(struct GameMainArg *arg) {
    struct GameMainNode *node = DAT_00668fc0;
    unsigned int first;

    if (node == NULL) {
        return NULL;
    }
    first = arg->field_0;
    do {
        if (node->field_8 == first && node->field_c == arg->field_4) {
            return node;
        }
        node = node->next;
    } while (node != NULL);
    return NULL;
}

// FUNCTION: LEGOLAND 0x004777f0
void FUN_004777f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00477980
unsigned int FUN_00477980(unsigned int param_1, unsigned int param_2) {
    return ((param_1 & param_2) != 0 ? 0xfffffffc : 0u) + 4;
}

// FUNCTION: LEGOLAND 0x004779a0
void FUN_004779a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004779d0
void FUN_004779d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00477bd0
void FUN_00477bd0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00478110
void FUN_00478110(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004781b0
int FUN_004781b0(const char *param_1, const void *param_2, int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x004781f0
int FUN_004781f0(int param_1, const void *param_2, int param_3, int param_4) { STUB(); }

// FUNCTION: LEGOLAND 0x00478280
void FUN_00478280(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004784c0
void FUN_004784c0(void) {
    NEWFLC_Repeat = 0;
    NEWFLC_BuffSize = 1;
    NEWFLC_CheckDuplicate = 1;
    DAT_00669054 = 0;
    DAT_004bb5ac = 1;
    DAT_00669050 = 0;
    DAT_0066879c = 0;
    DAT_00669098 = 0;

    lpConfig->field_30 = 0;
    lpConfig->field_38 = 1;
    lpConfig->field_34 = 1;
    lpConfig->field_1a = 0xc8;
    DAT_00832920 = lpConfig->field_1a;
    DAT_00832924 = 0;

    FUN_004689a0();
    DAT_007fdca4 = FUN_004689f0(0, 0, 0);
    DAT_004bb5b0 = 1;

    FUN_004441f0();
    FUN_0044db20();
    FUN_0044db80();
    FUN_00468840();
    FUN_004688e0();
    FUN_0044dc70(0, 0);
    FUN_00468830();
    FUN_00482d70();
    FUN_00462e90();
    FUN_00476000();
    FUN_00476050();
    FUN_00490610(DAT_004d8bb0);
    FUN_00463560();
    FUN_00482b10();
    FUN_00459960();

    DAT_00832974 = 1000;
    DAT_00832994 = 0;
    DAT_00832980 = 0;
    DAT_00832ba8 = 1;

    FUN_0046b240(0);
}

// FUNCTION: LEGOLAND 0x004785d0
void FUN_004785d0(char *param_1, unsigned int param_2) {
    strcpy(DAT_00669058, param_1);
    DAT_00669054 = param_2;
}

// FUNCTION: LEGOLAND 0x00478610
void FUN_00478610(unsigned int param_1) {
    DAT_004bb5ac = param_1;
    switch (param_1) {
    case 1:
        DAT_00669050 = 1;
        break;
    case 2:
        DAT_00669050 = 2;
        break;
    case 3:
        DAT_00669050 = 4;
        break;
    default:
        DAT_00669050 = 0;
        break;
    }
}

// FUNCTION: LEGOLAND 0x00478650
void FUN_00478650(unsigned int param_1, unsigned int param_2) {
    // param_1 is an opaque command-block handle reinterpreted as a GameMainEntry
    struct GameMainEntry *entry = (struct GameMainEntry *)param_1;
    if (param_2 == 0) {
        return;
    }
    // STRING: LEGOLAND 0x004bb9ec
    if (__strcmpi(entry->name, "PURGE") == 0) {
        DAT_00669050 |= 0x8;
    } else {
        DAT_00669050 &= 0xf7;
    }
}

// FUNCTION: LEGOLAND 0x00478690
int FUN_00478690(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    return (int)param_2 >= (int)param_3;
}

// FUNCTION: LEGOLAND 0x004786a0
int FUN_004786a0(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    return (DAT_00669054 & param_3) != 0;
}

// FUNCTION: LEGOLAND 0x004786c0
unsigned int FUN_004786c0(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4) {
    if (FUN_004786a0(param_1, param_2, param_3) == 0) {
        return 0;
    }
    return FUN_00478690(param_1, param_2, param_4) != 0;
}

// FUNCTION: LEGOLAND 0x00478700
void FUN_00478700(int *param_1, char **param_2, int param_3) {
    int temp;

    param_1[0] = FUN_004a04b9(param_2[param_3 + 0]);
    param_1[1] = FUN_004a04b9(param_2[param_3 + 1]);
    param_1[2] = FUN_004a04b9(param_2[param_3 + 2]);
    param_1[3] = FUN_004a04b9(param_2[param_3 + 3]);

    if (param_1[0] > param_1[2]) {
        temp = param_1[0];
        param_1[0] = param_1[2];
        param_1[2] = temp;
    }
    if (param_1[1] > param_1[3]) {
        temp = param_1[1];
        param_1[1] = param_1[3];
        param_1[3] = temp;
    }
}

// FUNCTION: LEGOLAND 0x00478770
void FUN_00478770(int *param_1, char **param_2, int param_3) {
    param_1[0] = FUN_004a04b9(param_2[param_3 + 0]);
    param_1[1] = FUN_004a04b9(param_2[param_3 + 1]);
}

// FUNCTION: LEGOLAND 0x004787a0
unsigned int FUN_004787a0(unsigned int param_1, unsigned int param_2) {
    // param_1 is an opaque command-block handle reinterpreted as a GameMainArg
    return ((struct GameMainArg *)param_1)->field_4;
}
