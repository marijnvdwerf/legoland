#include "legoland.h"

extern unsigned int DAT_00813a10;

extern unsigned int FUN_0049fdc2(void *block);
extern void FUN_0049e4d0(void *block);

// FUNCTION: LEGOLAND 0x00453a20
void DBPrintf(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00453a30
void __DEBUG_TAG(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00453a80
void __DEBUG_MALLOC(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00453b00
void __DEBUG_SMALLOC(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00453b70
void __DEBUG_REALLOC(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00453bb0
void __DEBUG_CALLOC(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00453bf0
void __DEBUG_FREE(void *ptr) {
    void *block_base = (char *)ptr - 0x10;
    unsigned int block_size = FUN_0049fdc2(block_base);
    DAT_00813a10 += 0x10 - block_size;
    FUN_0049e4d0(block_base);
}
