#pragma once

#include "legoland.h"

LEGO_EXPORT void AddRollerCoasterPath(int *coords);
LEGO_EXPORT void RemoveRollerCoasterPath(int *coords);

LEGO_EXPORT void AddBasicPath(void);
LEGO_EXPORT unsigned int *DrawBasicPath(void);
LEGO_EXPORT void RemoveBasicPath(void);
