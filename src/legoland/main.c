#include "legoland.h"
#include "crt.h"
#include <stdarg.h>

#include "globals.h"


// FUNCTION: LEGOLAND 0x00453cd0
void FUN_00453cd0(char *text) {}

// FUNCTION: LEGOLAND 0x00453ce0
void FUN_00453ce0(const char *format, ...) {
    va_list argptr;

    va_start(argptr, format);
    FUN_0049fdeb(DAT_00667128, format, argptr);
    va_end(argptr);
    FUN_00453cd0(DAT_00667128);
}

// FUNCTION: LEGOLAND 0x00453d10
void FUN_00453d10(void) { STUB(); }
