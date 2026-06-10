#pragma once

#include "legoland.h"

struct InstancePos {
    unsigned int x;
    unsigned int y;
};

LEGO_EXPORT void AddPathSquare(struct InstancePos *pos);
LEGO_EXPORT void RemovePathSquare(struct InstancePos *pos);
void FUN_00482a40(struct InstancePos *pos);
void FUN_00482a90(void);
