#pragma once

#include "legoland.h"

struct Bloke;
struct BlokeList;

LEGO_EXPORT void PutBlokeInList(struct BlokeList *list, struct Bloke *bloke);
LEGO_EXPORT void RemoveBlokeFromList(struct BlokeList *list, struct Bloke *bloke);
LEGO_EXPORT void NewLongTermAction(struct Bloke *bloke, unsigned short action);
unsigned char FUN_0044eb10(struct Bloke *bloke);
int FUN_0044f360(unsigned int param_1, unsigned char *param_2);
LEGO_EXPORT void DoHighLevelAI(struct Bloke *bloke);
