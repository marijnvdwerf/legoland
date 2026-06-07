#include "legoland.h"
#include <windows.h>
#include <mmsystem.h>

#include "globals.h"

// FUNCTION: LEGOLAND 0x00480200
LEGO_EXPORT void LoadMIDIFile(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004802c0
void FUN_004802c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004802f0
void FUN_004802f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00480330
void FUN_00480330(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00480570
void __stdcall FUN_00480570(unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4, unsigned int p5) { STUB(); }

// FUNCTION: LEGOLAND 0x004805d0
LEGO_EXPORT void PlayMIDI(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00480630
LEGO_EXPORT int InitMIDIManager(void) {
    DAT_007fd634 = 0;
    DAT_007fd630 = timeSetEvent(0x14, 0xa, (LPTIMECALLBACK)FUN_00480570, 0, 1);
    midiOutOpen((LPHMIDIOUT)&DAT_007fd638, (UINT)-1, 0, 0, 0);
    return 1;
}

// FUNCTION: LEGOLAND 0x00480670
LEGO_EXPORT void KillMIDIManager(void) {
    DAT_007fd634 = 0;
    timeKillEvent(DAT_007fd630);
    midiOutClose((HMIDIOUT)DAT_007fd638);
}
