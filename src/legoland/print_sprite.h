#pragma once

#include "legoland.h"

struct Person;
struct Sprite;

LEGO_EXPORT void ResetHitInfo(void);
LEGO_EXPORT unsigned int PrintSprite(struct Sprite *sprite, unsigned int x, unsigned int y, unsigned int param_4, int *param_5);
LEGO_EXPORT void SortPerson(struct Person *person, unsigned int param_2, void *param_3);
void FUN_00485f00(struct Sprite *param_1, unsigned int param_2, unsigned int param_3);
void FUN_00485f20(void *ptr);
void FUN_00485fa0(void);
