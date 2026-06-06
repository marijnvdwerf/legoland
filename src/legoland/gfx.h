#pragma once

// Per-TU header for gfx.c — canonical declarations for the graphics/palette helpers.

struct Image;

struct Image *__BMPLoader(struct Image *image);
