#pragma once

#include "legoland.h"
#include "controller.h"

LEGO_EXPORT int InitInputSystem(void);
LEGO_EXPORT void KillInputSystem(void);
LEGO_EXPORT char GetInputChar(void);
LEGO_EXPORT void ScanKeyboard(void);
LEGO_EXPORT void ScanMouse(void);
LEGO_EXPORT void UpdateControllerFromMouseData(struct CtrlBuffer *buffer);
LEGO_EXPORT void UpdateControllerFromKeyboardData(struct CtrlBuffer *buffer);
