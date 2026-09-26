#pragma once

#include "legoland.h"
#include "llidb.h"

struct CallbackTable;
struct Cursor;
struct ClassNode;

void FUN_00403080(struct ClassNode *name, struct CallbackTable *ci);
void FUN_00402ca0(Element *obj);
void FUN_00402ce0(void);
void FUN_00402d00(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, unsigned int param_5, unsigned int clip);
void FUN_00402dc0(Element *obj);
void FUN_00402ff0(void);
void FUN_00403030(Element *obj, TileId tile, struct Cursor *cursor);
void FUN_00403060(unsigned int param1, unsigned int param2);
