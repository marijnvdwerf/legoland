#pragma once

#include "legoland.h"

// Per-TU header for debug_alloc.c — canonical declarations for the debug printf /
// instrumented allocator helpers.

LEGO_EXPORT void DBPrintf(const char *format, ...);
