#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include "legoland.h"

#include "globals.h"
#include "main.h"
#include "screens.h"

// FUNCTION: LEGOLAND 0x00453cd0
void FUN_00453cd0(char *text) {}

// FUNCTION: LEGOLAND 0x00453ce0
void FUN_00453ce0(const char *format, ...) {
    va_list argptr;

    va_start(argptr, format);
    vsprintf(DAT_00667128, format, argptr);
    va_end(argptr);
    FUN_00453cd0(DAT_00667128);
}

// FUNCTION: LEGOLAND 0x00453d10
int __stdcall FUN_00453d10(unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4) {
    int result;

    result = -1;
    __try {
        FUN_00458830((unsigned int)&DAT_0066752c);
        result = (int)wWinMain(arg1, arg2, arg3, arg4);
    } __except (stackdump((void *)GetExceptionInformation(), "main thread")) {
    }

    return result;
}
