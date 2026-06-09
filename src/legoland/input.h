#pragma once

#include "legoland.h"

LEGO_EXPORT int InitInputSystem(void);
LEGO_EXPORT void KillInputSystem(void);
LEGO_EXPORT char GetInputChar(void);
LEGO_EXPORT void ScanKeyboard(void);
LEGO_EXPORT void ScanMouse(void);
LEGO_EXPORT void UpdateControllerFromMouseData(void *buffer);
LEGO_EXPORT void UpdateControllerFromKeyboardData(void *buffer);
