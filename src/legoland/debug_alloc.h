#pragma once

#include "legoland.h"

// Per-TU header for debug_alloc.c — canonical declarations for the debug printf /
// instrumented allocator helpers.

LEGO_EXPORT void DBPrintf(const char *format, ...);
LEGO_EXPORT void __DEBUG_TAG(char *name);
LEGO_EXPORT void *__DEBUG_MALLOC(char *file, int line, unsigned int size);
LEGO_EXPORT void *__DEBUG_SMALLOC(char *name, unsigned int size);
LEGO_EXPORT void *__DEBUG_REALLOC(void *ptr, unsigned int size);
LEGO_EXPORT void *__DEBUG_CALLOC(char *file, int line, unsigned int count, unsigned int size);
LEGO_EXPORT void __DEBUG_FREE(void *ptr);
