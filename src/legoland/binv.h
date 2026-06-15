#pragma once

#include "legoland.h"

// Per-TU header for binv.c — canonical declarations for the BinV model loader.

struct BinVFile;
struct BinVFrame;
struct BinVObject;
struct Vertex;

LEGO_EXPORT void FreeBinV(void *binv);
LEGO_EXPORT struct BinVFrame *GetBinVFrame(struct BinVFile *file, int index);
LEGO_EXPORT struct BinVObject *GetObjectFromName(struct BinVFrame *frame, const char *name);
LEGO_EXPORT struct Vertex *GetVertex(struct BinVObject *object, int index);
