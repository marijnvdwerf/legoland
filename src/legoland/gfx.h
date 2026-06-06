#pragma once

// Per-TU header for gfx.c — canonical declarations for the graphics/palette helpers.

struct Image;

void GetGFXFName(void);
struct Image *__BMPLoader(struct Image *image);
void LoadColourTable(void);
void ResendPalette(void);
unsigned int GetTransparentColour(void);
unsigned int GetNearestColour(int r, int g, int b);
