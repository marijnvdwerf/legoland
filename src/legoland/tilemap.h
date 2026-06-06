#pragma once

struct MapTile;

void FreeTileSpace(unsigned short index, unsigned short count);
void ScreenToMapRef(unsigned int x, void *out, unsigned int y);
int FUN_0045ce10(struct MapTile *tile);
void RemovePathTile(unsigned int param_1, unsigned short param_2);
