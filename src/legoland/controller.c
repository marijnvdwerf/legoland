#include "legoland.h"
#include "crt.h"

#include "globals.h"


// FUNCTION: LEGOLAND 0x00451e70
LEGO_EXPORT void SetupControllers(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00451f40
void FUN_00451f40(void) {
    if (DAT_00667104 != 0) {
        free(CONTROLLERBUFFER);
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
LEGO_EXPORT void ReadGameButtons(void) { STUB(); }
