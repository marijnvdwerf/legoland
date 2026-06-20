#pragma once

#include "legoland.h"

struct MapObject;
struct EditObject;
struct Cursor;

/* Callback-table entry points for the western-town shops (General Store,
 * Sheriff, Jail Cells, Bank, Saloon), registered by the shops dispatcher
 * (FUN_0043a400). */
void FUN_004375d0(struct MapObject *obj);
void FUN_00437610(void);
void FUN_00437630(void);
void FUN_00437670(struct MapObject *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, unsigned int param_5, unsigned int param_6);
void FUN_004378e0(struct MapObject *param_1);
void FUN_00437ba0(struct MapObject *obj);
void FUN_00437bd0(void);
void FUN_00437bf0(void);
void FUN_00437c30(struct MapObject *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, unsigned int param_5, unsigned int param_6);
void FUN_00437c90(struct MapObject *param_1);
void FUN_00437f60(struct EditObject *editObj, int *coords);
void FUN_00438020(struct MapObject *editObj, unsigned int coords, struct Cursor *cursor);
void FUN_00438070(struct MapObject *obj);
void FUN_004380f0(void);
void FUN_00438110(void);
void FUN_00438150(struct MapObject *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, unsigned int param_5, unsigned int param_6);
void FUN_00438430(struct MapObject *param_1);
LEGO_EXPORT unsigned int SaveJailCells(void);
LEGO_EXPORT unsigned int LoadJailCells(void);
void FUN_00438870(struct MapObject *obj);
void FUN_004388a0(void);
void FUN_004388c0(void);
void FUN_00438900(struct MapObject *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, unsigned int param_5, unsigned int param_6);
void FUN_00438960(struct MapObject *param_1);
void FUN_00438c60(struct MapObject *obj);
void FUN_00438ca0(void);
void FUN_00438cc0(void);
void FUN_00438d00(struct MapObject *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, unsigned int param_5, unsigned int param_6);
void FUN_00438f10(struct MapObject *param_1);
