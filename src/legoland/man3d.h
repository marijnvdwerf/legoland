#pragma once

#include "legoland.h"

struct Position;
struct Person;
struct Bloke;
struct PosHeader;

void FUN_0043f840(struct Person *person);
void FUN_0043f870(struct PosHeader *param_1);

LEGO_EXPORT struct Position *LoadPos(const char *path);
LEGO_EXPORT void UnloadPos(struct Position *pos);
LEGO_EXPORT void RenderBlokeIn3D(struct Bloke *bloke);
LEGO_EXPORT void SortBlokeIn3D(struct Bloke *bloke);
LEGO_EXPORT void IP_RenderBlokeIn3DNow(struct Bloke *bloke);
LEGO_EXPORT void UpdatePerson(struct Person *person);
LEGO_EXPORT void Control3DPeople(void);
LEGO_EXPORT void Add3DBlokeToList(struct Bloke *bloke, unsigned int param_2);
LEGO_EXPORT void BlokeWalkAnim(struct Bloke *bloke);
LEGO_EXPORT void Render3DPerson(struct Person *person);
void FUN_004401b0(int param_1, int param_2);
void FUN_0043f810(struct Person *person);
void FUN_00440a30(struct Person *person);
LEGO_EXPORT void SetPersonPosition(struct Person *person, unsigned int x, unsigned int y);
