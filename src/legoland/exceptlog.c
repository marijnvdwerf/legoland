#include "legoland.h"
#include "crt.h"

struct ExceptionEntry {
    unsigned int code;
    const char *message;
};


// FUNCTION: LEGOLAND 0x00453da0
void FUN_00453da0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00454290
void FUN_00454290(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004542e0
void FUN_004542e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00454380
void FUN_00454380(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00454500
void FUN_00454500(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004545a0
void FUN_004545a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00454700
const char *FUN_00454700(unsigned int code) {
    struct ExceptionEntry table[24] = {
        // STRING: LEGOLAND 0x004b8fd8
        {0x40010005, "a Control-C"},
        // STRING: LEGOLAND 0x004b8fc8
        {0x40010008, "a Control-Break"},
        // STRING: LEGOLAND 0x004b8fb0
        {0x80000002, "a Datatype Misalignment"},
        // STRING: LEGOLAND 0x004b8fa0
        {0x80000003, "a Breakpoint"},
        // STRING: LEGOLAND 0x004b8f8c
        {0xc0000005, "an Access Violation"},
        // STRING: LEGOLAND 0x004b8f78
        {0xc0000006, "an In Page Error"},
        // STRING: LEGOLAND 0x004b8f6c
        {0xc0000017, "a No Memory"},
        // STRING: LEGOLAND 0x004b8f54
        {0xc000001d, "an Illegal Instruction"},
        // STRING: LEGOLAND 0x004b8f38
        {0xc0000025, "a Noncontinuable Exception"},
        // STRING: LEGOLAND 0x004b8f20
        {0xc0000026, "an Invalid Disposition"},
        // STRING: LEGOLAND 0x004b8f08
        {0xc000008c, "a Array Bounds Exceeded"},
        // STRING: LEGOLAND 0x004b8eec
        {0xc000008d, "a Float Denormal Operand"},
        // STRING: LEGOLAND 0x004b8ed4
        {0xc000008e, "a Float Divide by Zero"},
        // STRING: LEGOLAND 0x004b8ebc
        {0xc000008f, "a Float Inexact Result"},
        // STRING: LEGOLAND 0x004b8ea0
        {0xc0000090, "a Float Invalid Operation"},
        // STRING: LEGOLAND 0x004b8e8c
        {0xc0000091, "a Float Overflow"},
        // STRING: LEGOLAND 0x004b8e78
        {0xc0000092, "a Float Stack Check"},
        // STRING: LEGOLAND 0x004b8e64
        {0xc0000093, "a Float Underflow"},
        // STRING: LEGOLAND 0x004b8e48
        {0xc0000094, "an Integer Divide by Zero"},
        // STRING: LEGOLAND 0x004b8e34
        {0xc0000095, "an Integer Overflow"},
        // STRING: LEGOLAND 0x004b8e18
        {0xc0000096, "a Privileged Instruction"},
        // STRING: LEGOLAND 0x004b8e04
        {0xc00000fd, "a Stack Overflow"},
        // STRING: LEGOLAND 0x004b8de8
        {0xc0000142, "a DLL Initialization Failed"},
        // STRING: LEGOLAND 0x004b8dcc
        {0xe06d7363, "a Microsoft C++ Exception"}
    };
    unsigned int i = 0;
    while (i < 24) {
        if (code == table[i].code) {
            return table[i].message;
        }
        i++;
    }
    // STRING: LEGOLAND 0x004b8db4
    return "Unknown exception type";
}

// FUNCTION: LEGOLAND 0x004548f0
char *FUN_004548f0(char *path) {
    char *last_backslash = _strrchr(path, '\\');
    if (last_backslash != NULL) {
        return last_backslash + 1;
    }
    return path;
}
