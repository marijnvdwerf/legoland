#pragma once

#include "legoland.h"

struct Sprite;
struct SaveIcon;

LEGO_EXPORT int LoadDateIntoTempProfile(int a1, int a2);
LEGO_EXPORT unsigned char StoreNewSaveGameToDisk(void);
LEGO_EXPORT void RemoveSaveGame(unsigned char slot);
LEGO_EXPORT void InitSavedGameScreen(void);
LEGO_EXPORT void PrintSavedGameDetails(void);
LEGO_EXPORT void KillSaveScreenSprites(void);
LEGO_EXPORT void DeleteSavedGameList(void);
LEGO_EXPORT struct Sprite *GetSavePanelBK(signed char slot);
LEGO_EXPORT unsigned char LoadSavedGamesList(unsigned char profile);
unsigned char FUN_0048db10(int param1, unsigned char flags);
unsigned char FUN_0048da50(unsigned int a1, unsigned int flags, unsigned int a3, unsigned int a4);
unsigned char FUN_0048d970(unsigned int a1, unsigned char flags);
unsigned char FUN_0048e4a0(struct SaveIcon *icon, unsigned int flags, unsigned int a3, unsigned int a4);
unsigned char FUN_0048e4f0(struct SaveIcon *icon, unsigned int a2, unsigned int a3, unsigned int a4);
int FUN_0048e720(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4);
