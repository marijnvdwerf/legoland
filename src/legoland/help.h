#pragma once

#include "legoland.h"

void ClearAdvisorHelp(void);
LEGO_EXPORT int DisplayAdvisorHelp(char *param_1, unsigned int param_2, unsigned int param_3);
LEGO_EXPORT void ProcessFrontEndHelp(void);
void UpdateSpeechPlayback(void);
void ShowHelpById(unsigned int a1);
void ShowHelpByName(UINT_PTR a1);
unsigned int PlaySpeech(unsigned int a1);
void ShowFocusedIconHelp(void);
LEGO_EXPORT void KillHelp(void);
LEGO_EXPORT void ProcessInGameHelp(void);
