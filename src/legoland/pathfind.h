#pragma once

#include "legoland.h"

struct Point;

struct InstancePos {
    unsigned int x;
    unsigned int y;
};

int FUN_00482710(int *a, int *b, int *out);
LEGO_EXPORT int SuggestNextMove(struct Point *pos, struct Point *goal, struct Point *out);
LEGO_EXPORT int PTPSuggestNextMove(struct Point *pos, struct Point *goal, struct Point *out);
LEGO_EXPORT void AddPathSquare(struct InstancePos *pos);
LEGO_EXPORT void RemovePathSquare(struct InstancePos *pos);
void FUN_00482a40(struct InstancePos *pos);
void FUN_00482a80(void);
void FUN_00482a90(void);
void FUN_004828f0(void);
unsigned int *FUN_00482b00(void);
