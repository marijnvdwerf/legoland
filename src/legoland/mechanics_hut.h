#pragma once

#include "legoland.h"
#include "llidb.h"

struct Cursor;
struct Ride;

void FUN_0043d250(Element *ctx);
unsigned int FUN_0043d2a0(unsigned int param1, unsigned int param2);
void FUN_0043d2c0(Element *obj, unsigned int tile, struct Cursor *cursor);
void FUN_0043d2f0(Element *obj);
void FUN_0043d580(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile);
void FUN_0043d730();
void FUN_0043d740();
struct RideSpriteInfo *FUN_0043d780(void *ptr, unsigned short arg2);
void FUN_0043d7c0(struct Ride *hut, unsigned int tile, int flag);
