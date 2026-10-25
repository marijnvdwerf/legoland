#pragma once

#include "legoland.h"

struct Element;

/* An element pointer that FindeIneList replaces with its save-game index. */
LEGO_EXPORT int SaveGame(char *filename);
union SavedElement;
LEGO_EXPORT int FindeIneList(union SavedElement *handle);
LEGO_EXPORT struct Element *GeteListPtr(int idx);
void MarkGameTimer(void);
int GetGameTimerSinceMark(void);
unsigned int OpenLogFile(const char *path);
int CloseLogFile(void);
void Empty_0047f850(void);
LEGO_EXPORT void UnloadSaveGameMap(void);
LEGO_EXPORT int LoadGame(char *path);
int SkipSaveGameDword(void);
