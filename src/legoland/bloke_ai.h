#pragma once

#include "legoland.h"

struct Bloke;
struct BlokeList;

LEGO_EXPORT void PutBlokeInList(struct BlokeList *list, struct Bloke *bloke);
LEGO_EXPORT void RemoveBlokeFromList(struct BlokeList *list, struct Bloke *bloke);
LEGO_EXPORT void NewLongTermAction(struct Bloke *bloke, unsigned short action);
LEGO_EXPORT int IsFavouriteAttraction(struct Bloke *bloke, unsigned int attraction);
LEGO_EXPORT int IsFavouriteFood(struct Bloke *bloke, unsigned int food);
unsigned int FUN_0044f3d0(struct BlokeList *list, unsigned short *value);
unsigned char FUN_0044eb10(struct Bloke *bloke);
int FUN_0044f360(unsigned int param_1, unsigned char *param_2);
LEGO_EXPORT void DoHighLevelAI(struct Bloke *bloke);
int FUN_0044f4a0(struct Bloke *bloke, int objclass, int param_3);
LEGO_EXPORT void PushLongTermAction(struct Bloke *bloke);
void FUN_00450530(struct Bloke *bloke);
