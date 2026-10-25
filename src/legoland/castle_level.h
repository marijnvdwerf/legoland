#pragma once

#include "legoland.h"

struct Element;
#include "llidb.h"

struct Cursor;

struct Ride;
void CastleLevel1_GetInterfaces(struct Element *name, struct Ride *ci);
void LoadCastleLevelMatteSprite(Element *obj);
void KillCastleLevelMatteSprite(void);
void RenderCastleLevel(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int clip);
void CastleLevel1Update(Element *obj);
void CastleLevelSetEditMode(void);
void CastleLevelRemoveObject(Element *obj, TileId tile, struct Cursor *cursor);
void CastleLevelAddObject(struct Element *param1, int *param2);
