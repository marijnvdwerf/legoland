#pragma once

#include "controller.h"
#include "legoland.h"

LEGO_EXPORT int InitInputSystem(void);
LEGO_EXPORT void KillInputSystem(void);
LEGO_EXPORT char GetInputChar(void);
LEGO_EXPORT void ScanKeyboard(void);
LEGO_EXPORT void ScanMouse(void);
LEGO_EXPORT void UpdateControllerFromMouseData(struct CtrlBuffer *buffer);
LEGO_EXPORT void UpdateControllerFromKeyboardData(struct CtrlBuffer *buffer);
int FUN_004738b0(void);
int FUN_00473970(void);
unsigned int FUN_00474070(void);
unsigned int FUN_00474080(void);
char FUN_00474130(void);
