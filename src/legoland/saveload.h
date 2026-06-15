#pragma once

#include "legoland.h"

LEGO_EXPORT int SaveGame(char *filename);
LEGO_EXPORT int FindeIneList(int *param_1);
LEGO_EXPORT unsigned int GeteListPtr(int idx);
void FUN_0047f810(void);
unsigned int FUN_0047f830(const char *path);
int FUN_0047f840(void);
void FUN_0047f850(void);
LEGO_EXPORT void UnloadSaveGameMap(void);
LEGO_EXPORT void LoadGame(char *path);
