#pragma once

#include "legoland.h"

struct Ride;

void FUN_00489ee0(void);
void FUN_0048a040(void);
LEGO_EXPORT void RemoveBlokeFromRide(void *ride, void *node);
LEGO_EXPORT void RemoveAllBlokesFromRide(unsigned int arg1, void *arg2);
LEGO_EXPORT int GetAllBlokesOffRide(struct Ride *ride, unsigned short uid);
