#include <windows.h>
#include "legoland.h"

extern unsigned int DAT_0079a890;
extern unsigned int DAT_0079a894;
extern unsigned int DAT_0079a898;

// FUNCTION: LEGOLAND 0x00499430
unsigned int GetGameTimer(void) {
    unsigned int now;

    if (DAT_0079a890 != 0) {
        now = DAT_0079a894;
    } else {
        now = GetTickCount();
    }
    return now - DAT_0079a898;
}

// FUNCTION: LEGOLAND 0x00499460
void FUN_00499460(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00499480
void GetBlink(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00499500
void FUN_00499500(void) { STUB(); }
