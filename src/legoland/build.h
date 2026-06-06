#pragma once

// Per-TU header for build.c — building/construction list helpers.

void AddObjectToBuildList(void);
void FUN_00450c00(void);
int GetBuildTime(int objClass);
unsigned int FUN_00450c70(void);
void ProcessBuildingTimes(void);
void GetBuildAnimFrame(void);
void DoBuildEffects(void);
void ClearBuildObjList(void);
