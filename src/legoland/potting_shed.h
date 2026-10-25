#pragma once

#include "legoland.h"

struct Element;
#include "llidb.h"

struct Cursor;

void LoadGShedMatteSprite(Element *obj);
unsigned int PottingShedAddObject(struct Element *param_1, int *param_2);
void PottingShedRemoveObject(Element *obj, unsigned int tile, struct Cursor *cursor);
void PottingShedUpdate(Element *obj);
void RenderPottingShed(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile);
void KillGShedMatteSprite();
void PottingShedSetEditMode();
struct RideSpriteInfo *GetPottingShedSpriteInfo(struct Element *param1, unsigned short param2);