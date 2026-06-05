#include "legoland.h"
#include <windows.h>

// FUNCTION: LEGOLAND 0x00450f30
void FUN_00450f30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004510e0
void FUN_004510e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004511e0
void FUN_004511e0(void) { return; }

// FUNCTION: LEGOLAND 0x004511f0
void FUN_004511f0(void) { return; }

// FUNCTION: LEGOLAND 0x00451200
void FUN_00451200(void) { return; }

// FUNCTION: LEGOLAND 0x00451210
void FUN_00451210(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00451280
void FUN_00451280(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00451390
void FUN_00451390(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00451410
void FUN_00451410(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00451480
int FUN_00451480(void) {
    // STRING: LEGOLAND 0x004b8634
    HANDLE result = CreateFileA("\\\\.\\vwin32", 0, 0, NULL, 0, 0x4000000, NULL);
    return (int)result;
}

// FUNCTION: LEGOLAND 0x004514a0
BOOL __stdcall FUN_004514a0(HANDLE param_1) { return CloseHandle(param_1); }

// FUNCTION: LEGOLAND 0x004514b0
void FUN_004514b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00451550
void FUN_00451550(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004515e0
void FUN_004515e0(void) { STUB(); }
