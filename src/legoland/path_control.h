#pragma once

#include "legoland.h"

struct Struct427f70Tmp {
    int field_0;
    int field_4;
};

LEGO_EXPORT void AddRollerCoasterPath(int *coords);
LEGO_EXPORT void RemoveRollerCoasterPath(const struct Struct427f70Tmp *tmp);

LEGO_EXPORT void AddBasicPath(void);
LEGO_EXPORT void DrawBasicPath(void);
LEGO_EXPORT void RemoveBasicPath(void);
