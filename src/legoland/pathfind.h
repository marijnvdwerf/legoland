#pragma once

#include "legoland.h"

struct InstancePos {
    unsigned int x;
    unsigned int y;
};

LEGO_EXPORT int SuggestNextMove(int *param_1, int *param_2, int *param_3);
LEGO_EXPORT int PTPSuggestNextMove(int *param_1, int *param_2, int *param_3);
LEGO_EXPORT void AddPathSquare(struct InstancePos *pos);
LEGO_EXPORT void RemovePathSquare(struct InstancePos *pos);
void FUN_00482a40(struct InstancePos *pos);
void FUN_00482a90(void);
void FUN_004828f0(void);
unsigned int *FUN_00482b00(void);
