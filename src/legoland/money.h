#pragma once

#include "legoland.h"

struct BuyItemArg;
union TileId;

LEGO_EXPORT void LoadMoneySFX(void);
LEGO_EXPORT void KillMoneySFX(void);
LEGO_EXPORT void PlayMoneySFX(union TileId *tile, int sfx, int a2);
LEGO_EXPORT void StopMoneySFX(unsigned char *param_1);
LEGO_EXPORT void BuyItem(struct BuyItemArg *item, union TileId *tile, int sfx);
