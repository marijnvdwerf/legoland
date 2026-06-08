#pragma once

#include "legoland.h"

LEGO_EXPORT int LoadDateIntoTempProfile(int a1, int a2);
LEGO_EXPORT unsigned char StoreNewSaveGameToDisk(void);
LEGO_EXPORT void RemoveSaveGame(unsigned char slot);
