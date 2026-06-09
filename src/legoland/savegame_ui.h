#pragma once

#include "legoland.h"

LEGO_EXPORT int LoadDateIntoTempProfile(int a1, int a2);
LEGO_EXPORT unsigned char StoreNewSaveGameToDisk(void);
LEGO_EXPORT void RemoveSaveGame(unsigned char slot);
LEGO_EXPORT void InitSavedGameScreen(void);
LEGO_EXPORT void PrintSavedGameDetails(void);
LEGO_EXPORT void KillSaveScreenSprites(void);
LEGO_EXPORT void DeleteSavedGameList(void);
