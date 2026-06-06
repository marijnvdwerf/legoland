#include "legoland.h"

#include "controller.h"

extern unsigned int DAT_00667104;
extern void *CONTROLLERBUFFER;

extern void FUN_0049e4d0(void *ptr);

// FUNCTION: LEGOLAND 0x00451e70
void SetupControllers(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00451f40
void FUN_00451f40(void) {
    if (DAT_00667104 != 0) {
        FUN_0049e4d0(CONTROLLERBUFFER);
        DAT_00667104 = 0;
    }
}

// FUNCTION: LEGOLAND 0x00451f70
void FUN_00451f70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00452030
void FUN_00452030(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00452390
void FUN_00452390(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00452460
void ReadGameButtons(void) { STUB(); }
