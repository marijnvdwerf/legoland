#include <windows.h>
#include "legoland.h"

extern unsigned int DAT_0079a890;  /* nonzero while the timer is frozen/paused */
extern unsigned int DAT_0079a894;  /* tick captured when it was frozen */
extern unsigned int DAT_0079a898;  /* tick at timer start (the epoch) */

// FUNCTION: LEGOLAND 0x00499430
unsigned int GetGameTimer(void) {
    unsigned int now;

    if (DAT_0079a890 != 0) {
        now = DAT_0079a894;     /* frozen: use the captured tick */
    } else {
        now = GetTickCount();   /* running: current tick */
    }
    return now - DAT_0079a898;  /* elapsed since start */
}

// FUNCTION: LEGOLAND 0x00499460
void FUN_00499460(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00499480
void GetBlink(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00499500
void FUN_00499500(void) { STUB(); }
