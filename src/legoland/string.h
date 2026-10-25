#pragma once

#include "legoland.h"

LEGO_EXPORT char *GetString(int n);
LEGO_EXPORT void DeleteStrings(void);
unsigned int PauseGameTimer(void);
void ResetGameTimer(void);
void ResumeGameTimer(void);
void LoadStringTable(void);
void AddString(const char *text, int key);
int UppercaseStringAndGetLength(char *str);
