#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include "debug_alloc.h"
#include "globals.h"

// FUNCTION: LEGOLAND 0x00453a20
LEGO_EXPORT void DBPrintf(const char *format, ...) {}

// FUNCTION: LEGOLAND 0x00453a30
LEGO_EXPORT void __DEBUG_TAG(char *name) {
    char *tag = (char *)malloc(16);
    int len = strlen(name);
    int count;
    int i;

    count = len >= 12 ? 12 : len;
    tag[0] = '*';
    tag[1] = '-';
    tag[2] = '-';
    tag[3] = '>';
    for (i = 0; i < count; i++) {
        tag[i + 4] = name[i];
    }
}

// FUNCTION: LEGOLAND 0x00453a80
LEGO_EXPORT void *__DEBUG_MALLOC(char *file, int line, unsigned int size) {
    char *block = (char *)malloc(size + 16);
    int len = strlen(file);
    int count;
    int start;
    int i;

    DAT_00813a10 += size;
    if (len >= 11) {
        count = 11;
        start = len - 11;
    } else {
        count = len;
        start = 0;
    }
    for (i = 0; i < count; i++) {
        block[i] = file[start + i];
    }
    // STRING: LEGOLAND 0x004b8a80
    sprintf(block + 11, "%d", line);
    return block + 16;
}

// FUNCTION: LEGOLAND 0x00453b00
LEGO_EXPORT void *__DEBUG_SMALLOC(char *name, unsigned int size) {
    char *block = (char *)malloc(size + 16);
    int len = strlen(name);
    int count;
    int start;
    int i;

    DAT_00813a10 += size;
    if (len >= 16) {
        count = 16;
        start = len - 16;
    } else {
        count = len;
        start = 0;
    }
    for (i = 0; i < count; i++) {
        block[i] = name[start + i];
    }
    return block + 16;
}

// FUNCTION: LEGOLAND 0x00453b70
LEGO_EXPORT void *__DEBUG_REALLOC(void *ptr, unsigned int size) {
    void *block_base = (char *)ptr - 0x10;
    unsigned int block_size = _msize(block_base);

    DAT_00813a10 += size - block_size + 0x10;
    return (char *)realloc(block_base, size + 0x10) + 0x10;
}

// FUNCTION: LEGOLAND 0x00453bb0
LEGO_EXPORT void *__DEBUG_CALLOC(char *file, int line, unsigned int count, unsigned int size) {
    unsigned int total = count * size;
    void *block = __DEBUG_MALLOC(file, line, total);

    memset(block, 0, total);
    return block;
}

// FUNCTION: LEGOLAND 0x00453bf0
LEGO_EXPORT void __DEBUG_FREE(void *ptr) {
    void *block_base = (char *)ptr - 0x10;
    unsigned int block_size = _msize(block_base);
    DAT_00813a10 += 0x10 - block_size;
    free(block_base);
}
