#pragma once

#include "legoland.h"

struct Bloke;
struct Ride;
struct RideNode;
struct Element;

LEGO_EXPORT void PutBlokeInList(struct Ride *ride, struct RideNode *node);
LEGO_EXPORT void RemoveBlokeFromList(struct Ride *ride, struct RideNode *node);
LEGO_EXPORT void NewLongTermAction(struct Bloke *bloke, unsigned short action);
LEGO_EXPORT int IsFavouriteAttraction(struct Bloke *bloke, struct Element *attraction);
LEGO_EXPORT int IsFavouriteFood(struct Bloke *bloke, struct Element *food);
unsigned int FUN_0044f3d0(struct Ride *ride, TileId *tile);
unsigned int FUN_0044f400(struct Ride *ride, TileId *tile);
int FUN_0044f180(struct Point *pos, struct Ride *ride);
char FUN_0044eb10(struct Bloke *bloke);
int FUN_0044f360(struct Ride *ride, TileId *tile);
LEGO_EXPORT void DoHighLevelAI(struct Bloke *bloke);
int FUN_0044f4a0(struct Bloke *bloke, struct Ride *ride, int wait);
LEGO_EXPORT void PushLongTermAction(struct Bloke *bloke);
void FUN_00450530(struct Bloke *bloke);

void FUN_00450a40(struct Bloke *bloke);
