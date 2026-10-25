#pragma once

#include "legoland.h"

struct Ride;

struct Element;

LEGO_EXPORT void AddRollerCoasterPath(int *coords);
LEGO_EXPORT void RemoveRollerCoasterPath(int *coords);

LEGO_EXPORT void AddBasicPath(struct Element *editObj, int *coords);
LEGO_EXPORT void DrawBasicPath(unsigned int idx, unsigned int x, unsigned int y, unsigned int mode);
struct Cursor;
LEGO_EXPORT void RemoveBasicPath(struct Element *editObj, int dummy, struct Cursor *cursor);
