#pragma once

struct Ride;

void FUN_00489ee0(void);
void FUN_0048a040(void);
void RemoveBlokeFromRide(void *ride, void *node);
void RemoveAllBlokesFromRide(unsigned int arg1, void *arg2);
int GetAllBlokesOffRide(struct Ride *ride, unsigned short uid);
