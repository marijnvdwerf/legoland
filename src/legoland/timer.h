#pragma once

#include "legoland.h"

// Per-TU header for timer.c — canonical declarations for the game timer helpers.

struct Sprite;

LEGO_EXPORT unsigned int GetGameTimer(void);
unsigned int FUN_00499460(void);
unsigned long GetTicks(void);
int FUN_00499500(struct Sprite *sprite);
