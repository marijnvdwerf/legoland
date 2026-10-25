#pragma once

#include "legoland.h"

struct Element;

LEGO_EXPORT void LoadMoneySFX(void);
LEGO_EXPORT void KillMoneySFX(void);
LEGO_EXPORT void PlayMoneySFX(TileId *tile, int sfx, int a2);
LEGO_EXPORT void StopMoneySFX(unsigned char *param_1);
LEGO_EXPORT void BuyItem(struct Element *item, TileId *tile, int sfx);
