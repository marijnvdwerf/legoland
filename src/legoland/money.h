#pragma once

#include "legoland.h"

struct BuyItemArg;

LEGO_EXPORT void LoadMoneySFX(void);
LEGO_EXPORT void KillMoneySFX(void);
LEGO_EXPORT void PlayMoneySFX(int a0, int a1, int a2);
LEGO_EXPORT void StopMoneySFX(unsigned char *param_1);
LEGO_EXPORT void BuyItem(struct BuyItemArg *item, int a2, int a3);
