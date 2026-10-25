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
unsigned int CountRidersAtTile(struct Ride *ride, TileId *tile);
unsigned int IsRideFull(struct Ride *ride, TileId *tile);
struct Point;
int IsAtRideEntrance(struct Point *pos, struct Ride *ride);
char GetBlokeHungerLevel(struct Bloke *bloke);
int IsRideWorking(struct Ride *ride, TileId *tile);
LEGO_EXPORT void DoHighLevelAI(struct Bloke *bloke);
int PutBlokeOnRide(struct Bloke *bloke, struct Ride *ride, int wait);
LEGO_EXPORT void PushLongTermAction(struct Bloke *bloke);
void UpdateBlokeMoodFromSurroundings(struct Bloke *bloke);

void PauseHoveredBloke(struct Bloke *bloke);
LEGO_EXPORT void PopLongTermAction(struct Bloke *bloke);
LEGO_EXPORT void ControlPeople(void);
void SaveBuildObjList(void);
void LoadBuildObjArray(void);
