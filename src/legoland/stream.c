#include <windows.h>
#include "legoland.h"
#include "crt.h"
#include "globals.h"

#include "stream.h"

// FUNCTION: LEGOLAND 0x00497f60
unsigned int FUN_00497f60(void) {
    int head;
    int tail;

    head = DAT_0079a7dc;
    tail = DAT_0079a7d8;
    if (head >= tail) {
        if (tail == 0) {
            return 0xffff - head;
        }
        return 0x10000 - head;
    }
    return tail - head - 1;
}

// FUNCTION: LEGOLAND 0x00497f90
int FUN_00497f90(void) {
    int head;
    int tail;

    head = DAT_0079a7dc;
    tail = DAT_0079a7d8;
    if (head < tail) {
        head = 0x10000;
    }
    return head - tail;
}

// FUNCTION: LEGOLAND 0x00497fb0
int FUN_00497fb0(void) {
    int used;
    int limit;

    limit = DAT_0079a7e4[0];
    used = (DAT_0079a7dc - DAT_0079a7d8) & 0xffff;
    if (limit == 0) {
        memcpy(DAT_0079a7e4, &DAT_0079a7e4[1], 0x13 * sizeof(unsigned int));
        DAT_0079a7e4[19] = 0;
        if (DAT_0079a7e0 != 0) {
            DAT_0079a7e0--;
        }
    }
    if (used <= limit) {
        return used;
    }
    return limit;
}

// FUNCTION: LEGOLAND 0x00498000
void FUN_00498000(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00498100
void FUN_00498100(void) {
    FUN_00498120();
    DAT_0079a83c |= 8;
}

// FUNCTION: LEGOLAND 0x00498120
void FUN_00498120(void) {
    _lseek(DAT_007caca8, DAT_007cacb4, 0);
    DAT_007cacac = DAT_0079ac04;
}

// FUNCTION: LEGOLAND 0x00498150
void FUN_00498150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004981e0
unsigned int FUN_004981e0(void) {
    int head;
    int tail;

    head = DAT_0079a838;
    tail = DAT_0079a834;
    if (head >= tail) {
        if (tail == 0) {
            return 0x1ffff - head;
        }
        return 0x20000 - head;
    }
    return tail - head - 1;
}

// FUNCTION: LEGOLAND 0x00498210
int FUN_00498210(void) {
    int head;
    int tail;

    head = DAT_0079a838;
    tail = DAT_0079a834;
    if (head < tail) {
        head = 0x20000;
    }
    return head - tail;
}

// FUNCTION: LEGOLAND 0x00498230
unsigned int FUN_00498230(void) {
    return (DAT_0079a838 - DAT_0079a834) & 0x1ffff;
}

// FUNCTION: LEGOLAND 0x00498250
void FUN_00498250(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004983a0
void FUN_004983a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00498420
void FUN_00498420(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00498630
void FUN_00498630(const char *param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x00498870
void FUN_00498870(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004988c0
int FUN_004988c0(void) {
    if (DAT_0079a84c == 0 || DAT_0079a84c == 1) {
        return 0;
    }
    ((struct KLIBAUDIO_Stop *)((struct KLIBAUDIO_Object *)DAT_0079a848)->vtable)->func_48(DAT_0079a848);
    FUN_00498870();
    FUN_00498120();
    DAT_0079a84c = 1;
    return 1;
}

// FUNCTION: LEGOLAND 0x00498900
void FUN_00498900(unsigned int param_1) {
    DAT_0079a7d0 = param_1;
    if (DAT_0079a848 != NULL) {
        ((struct KLIBAUDIO_Vtbl *)((struct KLIBAUDIO_Object *)DAT_0079a848)->vtable)->func_3c(DAT_0079a848, param_1);
    }
}

// FUNCTION: LEGOLAND 0x00498920
void FUN_00498920(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004989b0
void FUN_004989b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00498b00
int FUN_00498b00(void) {
    if (DAT_0079a84c == 0 || DAT_0079a84c == 3) {
        return 0;
    }
    FUN_004989b0();
    ((struct KLIBAUDIO_Vtbl *)((struct KLIBAUDIO_Object *)DAT_0079a848)->vtable)->func_30(DAT_0079a848, 0, 0, 1);
    DAT_0079a84c = 3;
    return 1;
}

// FUNCTION: LEGOLAND 0x00498b40
void FUN_00498b40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00498cf0
int FUN_00498cf0(void) {
    return DAT_0079a84c == 3;
}
