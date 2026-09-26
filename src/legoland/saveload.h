#pragma once

#include "legoland.h"

struct Element;

LEGO_EXPORT int SaveGame(char *filename);
LEGO_EXPORT int FindeIneList(struct Element **element);
LEGO_EXPORT struct Element *GeteListPtr(int idx);
void FUN_0047f810(void);
int FUN_0047f820(void);
unsigned int FUN_0047f830(const char *path);
int FUN_0047f840(void);
void FUN_0047f850(void);
LEGO_EXPORT void UnloadSaveGameMap(void);
LEGO_EXPORT void LoadGame(char *path);
