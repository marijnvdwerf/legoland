#pragma once

#include "legoland.h"

LEGO_EXPORT void CheckWorkerOnMouseStatus(int a);
LEGO_EXPORT void ResetMoveAWorkerStruct(void);
LEGO_EXPORT void ResetWorkersOldCoords(void);
unsigned int IsBlokeHovered(void *object);
void *GetWorkerOnMouse(void);
struct Bloke;
void PickUpWorker(unsigned int type, struct Bloke *worker);
int TryDropWorkerOnHomeOrPath(void);
struct Point;
struct WorkOrder *FindHoveredWorkerOrder(struct Point *out);
void CreatePopUpInfoButtons(void *a, void *b);
void KillPUOKAndCBSprites(void);
LEGO_EXPORT void RenderWorkerOnMouse(void);
