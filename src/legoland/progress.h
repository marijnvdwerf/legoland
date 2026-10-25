#pragma once

#include "legoland.h"

LEGO_EXPORT void InitProgressScreen(void);
void InitTutorialScreen(void);
unsigned char ProgressLevelClicked(unsigned char *arg0, unsigned int arg1, unsigned int arg2, unsigned int arg3);
unsigned char ProgressPlayClicked(unsigned char *arg0, unsigned int arg1, unsigned int arg2, unsigned int arg3);
unsigned char TutorialPlayClicked(unsigned char *arg0, unsigned int arg1, unsigned int arg2, unsigned int arg3);
unsigned char ProgressGoBackClicked(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4);
unsigned char ToggleProgressTutorialScreenClicked(void *param1, unsigned char param2);
void DrawTutorialList(void);
