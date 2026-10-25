#pragma once

#include "legoland.h"

// Per-TU header for timer.c — canonical declarations for the game timer helpers.

struct Sprite;

LEGO_EXPORT unsigned int GetGameTimer(void);
LEGO_EXPORT unsigned int GetBlink(void);
unsigned int GetGameTimerFrames(void);
unsigned long GetTicks(void);
int EnsureSpriteSurface(struct Sprite *sprite);
