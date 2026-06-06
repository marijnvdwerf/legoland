#pragma once

// Per-TU header for debug_alloc.c — canonical declarations for the debug printf /
// instrumented allocator helpers.

void DBPrintf(const char *format, ...);
void __DEBUG_TAG(void);
void __DEBUG_MALLOC(void);
void __DEBUG_SMALLOC(void);
void __DEBUG_REALLOC(void);
void __DEBUG_CALLOC(void);
void __DEBUG_FREE(void *ptr);
