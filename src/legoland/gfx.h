#pragma once

#include "legoland.h"

// Per-TU header for gfx.c — canonical declarations for the graphics/palette helpers.

struct Image;

LEGO_EXPORT char *GetGFXFName(const char *name, unsigned int type, char *out);
LEGO_EXPORT int __BMPLoader(struct Image *image);
LEGO_EXPORT unsigned int GetNearestColour(int r, int g, int b);
LEGO_EXPORT void ResendPalette(void);
