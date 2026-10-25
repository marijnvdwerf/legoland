#pragma once

#include "legoland.h"

struct Cursor;
struct Element;

void PlayEntranceArrivalSounds(TileId *tile, struct Bloke *arg);
void RenderEntrance(struct Element *obj, unsigned int param_2, unsigned int param_3, TileId *tile, void *param_5, unsigned int clip);
void LoadEntranceResources(struct Element *param);
void UnloadEntranceResources(struct Element *param);
void EntranceRemoveObject(struct Element *obj, TileId tile, struct Cursor *cursor);
void Entrance1Update(struct Element *elem);
