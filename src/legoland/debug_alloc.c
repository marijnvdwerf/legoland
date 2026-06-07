#include "legoland.h"
#include "crt.h"

#include "debug_alloc.h"
#include "globals.h"


// FUNCTION: LEGOLAND 0x00453a20
LEGO_EXPORT void DBPrintf(const char *format, ...) {}

// FUNCTION: LEGOLAND 0x00453a30
LEGO_EXPORT void __DEBUG_TAG(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00453a80
LEGO_EXPORT void __DEBUG_MALLOC(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00453b00
LEGO_EXPORT void __DEBUG_SMALLOC(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00453b70
LEGO_EXPORT void __DEBUG_REALLOC(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00453bb0
LEGO_EXPORT void __DEBUG_CALLOC(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00453bf0
LEGO_EXPORT void __DEBUG_FREE(void *ptr) {
    void *block_base = (char *)ptr - 0x10;
    unsigned int block_size = _msize(block_base);
    DAT_00813a10 += 0x10 - block_size;
    free(block_base);
}
