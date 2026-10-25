#pragma once

#include "legoland.h"

struct Element;
#include "llidb.h"

struct Cursor;
struct Ride;

void LoadMechHutMaskSprite(Element *ctx);
unsigned int MechanicsHutAddObject(struct Element *param1, int *param2);
void MechanicsHutRemoveObject(Element *obj, unsigned int tile, struct Cursor *cursor);
void MechanicsHutUpdate(Element *obj);
void RenderMechanicsHut(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile);
void KillMechHutMaskSprite();
void MechanicsHutSetEditMode();
struct RideSpriteInfo *GetMechanicsHutSpriteInfo(struct Element *ptr, unsigned short arg2);
void DetachWorkersFromRemovedHome(struct Ride *hut, unsigned int tile, int flag);
