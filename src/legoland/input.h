#pragma once

#include "controller.h"
#include "legoland.h"

LEGO_EXPORT int InitInputSystem(void);
LEGO_EXPORT void KillInputSystem(void);
LEGO_EXPORT char GetInputChar(void);
LEGO_EXPORT void ScanKeyboard(void);
LEGO_EXPORT void ScanMouse(void);
struct CtrlBuffer;
LEGO_EXPORT void UpdateControllerFromMouseData(struct CtrlBuffer *buffer);
LEGO_EXPORT void UpdateControllerFromKeyboardData(struct CtrlBuffer *buffer);
int CreateKeyboardDevice(void);
int CreateMouseDevice(void);
unsigned int IsLeftShiftDown(void);
unsigned int IsRightShiftDown(void);
char ReadCheatKey(void);
void SaveInputState(void);
void LoadInputState(void);
