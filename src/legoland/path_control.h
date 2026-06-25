#pragma once

#include "legoland.h"

struct EditObject;
struct PathEditObject;
struct PathCursor;

LEGO_EXPORT void AddRollerCoasterPath(int *coords);
LEGO_EXPORT void RemoveRollerCoasterPath(int *coords);

LEGO_EXPORT void AddBasicPath(struct EditObject *editObj, int *coords);
LEGO_EXPORT void DrawBasicPath(unsigned int idx, unsigned int x, unsigned int y, unsigned int mode);
LEGO_EXPORT void RemoveBasicPath(struct PathEditObject *editObj, int dummy, struct PathCursor *cursor);
