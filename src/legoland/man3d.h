#pragma once

struct Position;
struct Person;
struct Bloke;

void UnloadPos(struct Position *pos);
void RenderBlokeIn3D(struct Bloke *bloke);
void SortBlokeIn3D(struct Bloke *bloke);
void IP_RenderBlokeIn3DNow(struct Bloke *bloke);
void UpdatePerson(struct Person *person);
void Control3DPeople(void);
void Add3DBlokeToList(struct Bloke *bloke, unsigned int param_2);
void BlokeWalkAnim(struct Bloke *bloke);
