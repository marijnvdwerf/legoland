#pragma once

#include "legoland.h"

struct EditObject;

LEGO_EXPORT void AddRollerCoasterPath(int *coords);
LEGO_EXPORT void RemoveRollerCoasterPath(int *coords);

LEGO_EXPORT void AddBasicPath(struct EditObject *editObj, int *coords);
LEGO_EXPORT unsigned int *DrawBasicPath(void);
LEGO_EXPORT void RemoveBasicPath(void);
