#pragma once

#include "legoland.h"

// Per-TU header for gfx.c — canonical declarations for the graphics/palette helpers.

struct Image;

LEGO_EXPORT char *GetGFXFName(const char *name, unsigned int type, char *out);
LEGO_EXPORT struct Image *__BMPLoader(struct Image *image);
