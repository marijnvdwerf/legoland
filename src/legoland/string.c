#include <windows.h>
#include "legoland.h"

#include "timer.h"
#include "string.h"

struct StringNode {
    int key;
    char *text;
    struct StringNode *next;
};

extern struct StringNode *strings[10];
extern unsigned int DAT_0079a890;
extern unsigned int DAT_0079a894;
extern unsigned int DAT_0079a898;
extern unsigned int DAT_0079a89c;
extern unsigned int DAT_0079a8a0;
extern unsigned int DAT_008119a4;

// FUNCTION: LEGOLAND 0x00498f50
char *GetString(int n) {
    struct StringNode *node = strings[n % 10];
    while (node != NULL) {
        if (node->key == n) {
            return node->text;
        }
        node = node->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00498f80
void FUN_00498f80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00498ff0
void DeleteStrings(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00499040
void FUN_00499040(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004990c0
void FUN_004990c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00499120
void FUN_00499120(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00499190
void FUN_00499190(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00499240
void FUN_00499240(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00499300
void FUN_00499300(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00499380
unsigned int FUN_00499380(void) {
    if (DAT_0079a890 == 0) {
        DAT_0079a890 = 1;
        DAT_0079a894 = GetTickCount();
        DAT_0079a89c = DAT_008119a4;
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004993c0
void FUN_004993c0(void) {
    if (DAT_0079a890 == 0) {
        return;
    }
    DAT_0079a890 = 0;
    DAT_0079a898 += GetTickCount() - DAT_0079a894;
    DAT_0079a8a0 += DAT_008119a4 - DAT_0079a89c;
}

// FUNCTION: LEGOLAND 0x00499410
void FUN_00499410(void) {
    unsigned int ticks;
    unsigned int counter;

    ticks = GetTicks();
    DAT_0079a894 = ticks;
    DAT_0079a898 = ticks;
    counter = DAT_008119a4;
    DAT_0079a89c = counter;
    DAT_0079a8a0 = counter;
}
