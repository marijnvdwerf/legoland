#pragma once

// Per-TU header for timer.c — canonical declarations for the game timer helpers.

struct Sprite;

unsigned int GetGameTimer(void);
unsigned int FUN_00499460(void);
unsigned long GetTicks(void);
unsigned int GetBlink(void);
int FUN_00499500(struct Sprite *sprite);
